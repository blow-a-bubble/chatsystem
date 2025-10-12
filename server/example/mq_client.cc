#include "logger.hpp"
#include "rabbitmq.hpp"
#include <gflags/gflags.h>
DEFINE_bool(enable_debug, true, "是否开启debug模式");
DEFINE_string(filename, "async.log", "日志文件名");
DEFINE_int32(level, spdlog::level::level_enum::debug, "日志输出最低等级");
void messageCallback(const char *data, size_t len)
{
    std::string message(data, len);
    bubble::INFO__LOG("消费mq数据: {}", message);
}
int main(int argc, char *argv[])
{
    bubble::init_logger(FLAGS_enable_debug, FLAGS_filename, (spdlog::level::level_enum)FLAGS_level);
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    bubble::MQClient client("root", "Zyk200388", "localhost:5672");
    client.declareExchangeAndQueue("test-exchange", "test-queue", "test-queue-key");
    client.comsume("test-queue", messageCallback);
    client.run();
    std::this_thread::sleep_for(std::chrono::seconds(60));
    return 0;
}