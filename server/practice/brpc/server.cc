#include "echo.pb.h"
#include <iostream>
#include <brpc/server.h>
#include <butil/logging.h>
class EchoServiceImpl : public example::EchoService {
public:
    virtual void Echo(google::protobuf::RpcController* cntl_base,
                      const example::EchoRequest* request,
                      example::EchoResponse* response,
                      google::protobuf::Closure* done) {
        brpc::ClosureGuard done_guard(done);
        std::cout << "服务器收到：" << request->message() << std::endl;
        response->set_message(request->message());
    }
};
int main(int argc, char* argv[])
{
    // 0. 取消brpc的日志功能
    logging::LoggingSettings settings;
    settings.logging_dest = logging::LoggingDestination::LOG_TO_NONE;
    logging::InitLogging(settings);
    // 1. 创建服务端
    brpc::Server server;
    // 2. 注册服务
    EchoServiceImpl echo_server;
    if(server.AddService(&echo_server, brpc::ServiceOwnership::SERVER_DOESNT_OWN_SERVICE) != 0)
    {
        std::cout << "添加服务失败" << std::endl;
        return -1;
    }
    // 3. 启动服务
    brpc::ServerOptions options;
    options.idle_timeout_sec = -1;
    options.num_threads = 10;
    if(server.Start(8080, &options) != 0)
    {
        std::cout << "启动服务失败" << std::endl;
        return -1;
    }
    // 4. 等待服务停止
    server.RunUntilAskedToQuit();
    // 5. 停止服务
    server.Stop(0);
    return 0;
}

