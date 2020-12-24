/*
 * Copyright (C) 2020 Tusooa Zhu
 *
 * This file is part of libkazv.
 *
 * libkazv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * libkazv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with libkazv.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <deque>

#include <zug/into_vector.hpp>
#include <zug/transducer/map.hpp>

#include <cpr/cpr.h>
#include "cprjobhandler.hpp"
#include "debug.hpp"

namespace Kazv
{
    struct CprJobHandler::Private
    {
        CprJobHandler *q;
        boost::asio::io_context::executor_type executor;
        using TimerSP = std::shared_ptr<boost::asio::steady_timer>;
        using TimerSPList = std::vector<TimerSP>;
        using TimerMap = std::unordered_map<std::optional<std::string>, TimerSPList>;
        TimerMap timerMap;

        enum Status
        {
            Waiting,
            Running,
        };
        using Callback = std::function<void(Response)>;
        struct JobDesc
        {
            BaseJob job;
            Callback callback;
            Status status;
        };
        using JobQueue = std::deque<JobDesc>;
        using JobMap = std::unordered_map<std::string, JobQueue>;
        JobMap jobQueues;

        void submitImpl(BaseJob job, std::function<void(Response)> userCallback);

        void addToQueue(BaseJob job, Callback callback) {
            boost::asio::post(
                executor,
                [=] {
                    // precondition: job has a queueId
                    auto queueId = job.queueId().value();

                    jobQueues[queueId].push_back(JobDesc{job, callback, Waiting});
                });
        }

        void clearQueue(std::string queueId) {
            boost::asio::post(
                executor,
                [=] { clearQueueImpl(queueId); });
        }

        void clearQueueImpl(std::string queueId) {
            Response fakeResponse;
            fakeResponse.statusCode = headFailureCancelledStatusCode;
            fakeResponse.body = JsonBody(
                json{ {"errcode", headFailureCancelledErrorCode},
                      {"error", headFailureCancelledErrorMsg} }
                );
            dbgJob << "clearQueueImpl called with " << queueId << std::endl;

            for (auto [job, callback, status] : jobQueues[queueId]) {
                dbgJob << "this job is " << (status == Waiting ? "Waiting" : "Running") << std::endl;
                if (status == Waiting) {
                    // Run callback in a different thread, just as in submitImpl().
                    q->async([=] { callback(job.genResponse(fakeResponse)); } );
                }
                // if status is Running, the callback is already called
            }
            jobQueues[queueId].clear();
        }

        void popJob(std::string queueId) {
            boost::asio::post(
                executor,
                [=] { popJobImpl(queueId); });
        }

        void popJobImpl(std::string queueId) {
            dbgJob << "popJobImpl called with " << queueId << std::endl;
            if (! jobQueues[queueId].empty()) {
                dbgJob << "the first job is "
                       << (jobQueues[queueId].front().status == Waiting ? "Waiting" : "Running") << std::endl;
                jobQueues[queueId].pop_front();
            }
        }

        void monitorQueues() {
            boost::asio::post(
                executor,
                [=] {
                    // precondition: job has a queueId
                    for (auto &[queueId, queue] : jobQueues) { // need to change queue
                        dbgJob << "monitoring queue " << queueId << std::endl;
                        if (! queue.empty()) {
                            auto [job, callback, status] = queue.front();
                            dbgJob << "the first job is "
                                   << (status == Waiting ? "Waiting" : "Running") << std::endl;
                            if (status == Waiting) {
                                queue.front().status = Running;
                                submitImpl(
                                    job,
                                    [=](Response r) { // in new thread
                                        callback(r);

                                        if (! r.success() // should be enough for now
                                            && job.queuePolicy() == CancelFutureIfFailed) {
                                            clearQueue(queueId); // in executor thread
                                        } else {
                                            popJob(queueId); // in executor thread
                                        }
                                    });
                            }
                        }
                    }
                });
        }

        void addTimerToMap(TimerSP timer, std::optional<std::string> timerId) {
            boost::asio::post(
                executor,
                [=] {
                    timerMap[timerId].push_back(timer);
                });
        }

        void clearTimer(TimerSP timer, std::optional<std::string> timerId) {
            boost::asio::post(
                executor,
                [=] {
                    std::remove(timerMap[timerId].begin(), timerMap[timerId].end(), timer);
                });
        }

        void cancelAllTimers(std::optional<std::string> timerId) {
            boost::asio::post(
                executor,
                [=] {
                    cancelAllTimersImpl(timerId);
                });
        }

        void cancelAllTimersImpl(std::optional<std::string> timerId) {
            auto timers = timerMap[timerId];
            timerMap.erase(timerId);
            for (auto timer : timers) {
                timer->cancel();
            }
        }

        void intervalTimerCallback(TimerSP timer,
                                   std::function<void()> func,
                                   int ms,
                                   const boost::system::error_code &error) {
            if (! error) {
                func();
                auto dur = boost::asio::chrono::milliseconds(ms);
                timer->expires_at(timer->expiry() + dur);
                timer->async_wait(
                    [=](const boost::system::error_code &error) {
                        intervalTimerCallback(timer, func, ms, error);
                    });
            }
        }
    };

    CprJobHandler::CprJobHandler(boost::asio::io_context::executor_type executor)
        : m_d(Private{this, std::move(executor), Private::TimerMap{}, Private::JobMap{}})
    {
        setInterval(
            [=] {
                m_d->monitorQueues();
            },
            50, // ms
            "-queue-monitor");
    }

    CprJobHandler::~CprJobHandler() = default;

    void CprJobHandler::async(std::function<void()> func)
    {
        std::thread([func=std::move(func), guard=boost::asio::executor_work_guard(m_d->executor)]() {
                        func();
                    }).detach();
    }

    void CprJobHandler::setTimeout(std::function<void()> func, int ms, std::optional<std::string> timerId)
    {
        auto timer=std::make_shared<boost::asio::steady_timer>(
            m_d->executor, boost::asio::chrono::milliseconds(ms));

        m_d->addTimerToMap(timer, timerId);

        timer->async_wait(
            [=, timer=timer](const boost::system::error_code &error){
                if (! error) {
                    func();
                    this->m_d->clearTimer(timer, timerId);
                }
            });
    }

    void CprJobHandler::setInterval(std::function<void()> func, int ms, std::optional<std::string> timerId)
    {
        auto dur = boost::asio::chrono::milliseconds(ms);
        auto timer = std::make_shared<boost::asio::steady_timer>(m_d->executor, dur);

        m_d->addTimerToMap(timer, timerId);

        timer->async_wait(
            [=](const boost::system::error_code &error) {
                m_d->intervalTimerCallback(timer, func, ms, error);
            });
    }

    void CprJobHandler::cancel(std::string timerId)
    {
        m_d->cancelAllTimers(timerId);
    }

    void CprJobHandler::submit(BaseJob job, std::function<void(Response)> userCallback)
    {
        if (job.queueId()) {
            m_d->addToQueue(job, userCallback);
        } else {
            m_d->submitImpl(job, userCallback);
        }
    }

    void CprJobHandler::Private::submitImpl(BaseJob job, std::function<void(Response)> userCallback)
    {
        cpr::Url url{job.url()};
        cpr::Body body(job.requestBody());
        BaseJob::Header origHeader = job.requestHeader();
        cpr::Header header(origHeader.get().begin(), origHeader.get().end());
        cpr::Parameters params;
        BaseJob::Query query = job.requestQuery();
        BaseJob::ReturnType returnType = job.returnType();
        BaseJob::Method method = job.requestMethod();

        if (! query.empty()) {
            // from cpr/parameters.cpp
            cpr::CurlHolder holder;
            for (const auto kv : query) {
                std::string key = kv.first;
                std::string value = kv.second;
                params.AddParameter(cpr::Parameter(std::move(key), std::move(value)), holder);
            }
        }

        auto callback = [returnType](cpr::Response r) -> Response {
                            Body body = r.text;

                            if (returnType == BaseJob::ReturnType::Json) {
                                try {
                                    body = BaseJob::JsonBody(std::move(json::parse(r.text)));
                                } catch (const json::exception &e) {
                                    // the response is not valid json
                                    dbgJob << "body is not correct json: " << e.what() << std::endl;
                                }
                            }

                            return { r.status_code,
                                     body,
                                     BaseJob::Header(r.header.begin(), r.header.end()),
                                     {} // extraData, will be added in genResponse
                            };
                        };

        std::shared_future<Response> res = std::visit(lager::visitor{
                [=](BaseJob::Get) {
                    return cpr::GetCallback(callback, url, header, body, params);
                },
                [=](BaseJob::Post) {
                    return cpr::PostCallback(callback, url, header, body, params);
                },
                [=](BaseJob::Put) {
                    return cpr::PutCallback(callback, url, header, body, params);
                },
                [=](BaseJob::Delete) {
                    return cpr::DeleteCallback(callback, url, header, body, params);
                }
            }, method).share();

        q->async([=]() {
                     userCallback(job.genResponse(res.get()));
                 });
    }

    void CprJobHandler::stop()
    {
        boost::asio::post(
            m_d->executor,
            [=] {
                auto ids = zug::into_vector(
                    zug::map([](auto i) { return i.first; }),
                    m_d->timerMap);
                for (auto id : ids) {
                    m_d->cancelAllTimersImpl(id);
                }
                m_d->jobQueues.clear();
            });
    }
}
