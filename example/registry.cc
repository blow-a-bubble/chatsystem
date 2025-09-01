#include "etcd.hpp"
#include "echo.pb.h"
#include "logger.hpp"
#include <brpc/server.h>
#include <butil/logging.h>
#include <gflags/gflags.h>
#include <iostream>
DEFINE_bool(enable_debug, true, "是否开启debug模式");
DEFINE_string(filename, "async.log", "日志文件名");
DEFINE_int32(level, spdlog::level::level_enum::debug, "日志输出最低等级");
DEFINE_int32(brpc_server_port, 7070, "brpc服务器端口");
DEFINE_string(etcd_host, "htpp://127.0.0.1:2379", "etcd的地址");
DEFINE_string(etcd_key, "/server/echo/instance", "注册的服务名称");
DEFINE_string(etcd_value, "127.0.0.1:7070", "注册的服务地址");
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
int main(int argc, char** argv)
{
    init_logger(FLAGS_enable_debug, FLAGS_filename, (spdlog::level::level_enum)FLAGS_level);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    // 0.取消brpc的日志功能
    // logging::LoggingSettings settings;
    // settings.logging_dest = logging::LoggingDestination::LOG_TO_NONE;
    // logging::InitLogging(settings);
    // 1.创建服务端
    brpc::Server server;
    // 2. 注册服务
    EchoServiceImpl echo_server;
    if(server.AddService(&echo_server, brpc::ServiceOwnership::SERVER_DOESNT_OWN_SERVICE) != 0)
    {
        std::cout << "添加服务失败" << std::endl;
        return -1;
    }
    // 3.启动服务
    brpc::ServerOptions options;
    options.idle_timeout_sec = -1;
    options.num_threads = 10;
    if(server.Start(FLAGS_brpc_server_port, &options) != 0)
    {
        std::cout << "启动服务失败" << std::endl;
        return -1;
    }
    // 4.创建注册对象
    Registry registry(FLAGS_etcd_host);
    // 5.注册服务
    registry.registerService(FLAGS_etcd_key + FLAGS_etcd_value, FLAGS_etcd_value);
    // 6. 等待服务停止
    server.RunUntilAskedToQuit();
    // 7. 停止服务
    server.Stop(0);
    return 0;
}