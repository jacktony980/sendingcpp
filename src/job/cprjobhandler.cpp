
#include <cpr/cpr.h>
#include "cprjobhandler.hpp"

namespace Kazv
{
    CprJobHandler::CprJobHandler(boost::asio::io_context::executor_type executor)
        : executor(std::move(executor))
    {
    }

    CprJobHandler::~CprJobHandler() = default;

    void CprJobHandler::async(std::function<void()> func)
    {
        std::thread([func=std::move(func), guard=boost::asio::executor_work_guard(executor)]() {
                        func();
                    }).detach();
    }

    void CprJobHandler::fetch(const BaseJob &job, std::function<void(std::shared_future<BaseJob::Response>)> userCallback)
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

        auto callback = [returnType](cpr::Response r) -> BaseJob::Response {
                            BaseJob::Body body = r.text;

                            if (returnType == BaseJob::ReturnType::Json) {
                                try {
                                    body = BaseJob::JsonBody(std::move(json::parse(r.text)));
                                } catch (const json::exception &) {
                                    // the response is not valid json
                                }
                            }

                            return { r.status_code, body, BaseJob::Header(r.header.begin(), r.header.end()) };
                        };

        std::shared_future<BaseJob::Response> res = std::visit(lager::visitor{
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

        async([=]() {
                  userCallback(res);
              });
    }
}
