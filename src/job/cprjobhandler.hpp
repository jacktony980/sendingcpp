
#pragma once

#include <boost/asio.hpp>
#include "jobinterface.hpp"

namespace Kazv
{
    struct CprJobHandler : public JobInterface
    {
        CprJobHandler(boost::asio::io_context::executor_type executor);
        ~CprJobHandler() override;
        void async(std::function<void()> func) override;
        void fetch(const BaseJob &job,
                   std::function<void(std::shared_future<BaseJob::Response>)> callback) override;
    private:
        boost::asio::io_context::executor_type executor;
    };
}
