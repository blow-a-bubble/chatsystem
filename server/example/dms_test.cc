#include "dms.hpp"
#include <gflags/gflags.h>
DEFINE_bool(enable_debug, true, "是否开启debug模式");
DEFINE_string(filename, "async.log", "日志文件名");
DEFINE_int32(level, spdlog::level::level_enum::debug, "日志输出最低等级");
int main(int argc, char *argv[])
{
    bubble::init_logger(FLAGS_enable_debug, FLAGS_filename, (spdlog::level::level_enum)FLAGS_level);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    bubble::DmsClient dms("", "");
    dms.sendCode("13235698376", "213141");
    return 0;
}