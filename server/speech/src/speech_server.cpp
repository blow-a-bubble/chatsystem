#include "speech_server.hpp"
// 日志
DEFINE_bool(enable_debug, true, "是否开启debug模式");
DEFINE_string(filename, "async.log", "日志文件名");
DEFINE_int32(level, spdlog::level::level_enum::debug, "日志输出最低等级");

// 服务
DEFINE_int32(brpc_server_port, 7000, "brpc服务器端口");
DEFINE_string(etcd_host, "htpp://127.0.0.1:2379", "etcd的地址");
DEFINE_string(base_service, "/service", "服务器监控根目录");
DEFINE_string(instance_name, "/speech_service/instance", "当前实例名称");
DEFINE_string(instance_host, "127.0.0.1:7000", "当前实例地址");

//百度语音key
DEFINE_string(app_id, "120128701", "AppID");
DEFINE_string(api_key, "pdQD7nOBTTkB2XZJS2FaIilp", "API Key");
DEFINE_string(secret_key, "uYZmWaUsF9td2Bkq3JpJ1g43psbyCGcG", "Secret Key");
int main(int argc, char *argv[])
{
    // 初始化日志模块
    bubble::init_logger(FLAGS_enable_debug, FLAGS_filename, (spdlog::level::level_enum)FLAGS_level);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    // 取消brpc自带的日志
    logging::LoggingSettings settings;
    settings.logging_dest = logging::LoggingDestination::LOG_TO_NONE;
    logging::InitLogging(settings);
    // 创建服务
    bubble::SpeechServerBuilder builder;
    builder.setAsrClient(FLAGS_app_id, FLAGS_api_key, FLAGS_secret_key);
    builder.setBrpcServer(FLAGS_brpc_server_port);
    builder.setRegistry(FLAGS_etcd_host, FLAGS_base_service + FLAGS_instance_name, FLAGS_instance_host);

    bubble::SpeechServer::ptr server = builder.build();
    server->start();
    return 0;
}