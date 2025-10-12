#include "asr.hpp"
#include <gflags/gflags.h>
#include <iostream>
DEFINE_bool(enable_debug, true, "是否开启debug模式");
DEFINE_string(filename, "async.log", "日志文件名");
DEFINE_int32(level, spdlog::level::level_enum::debug, "日志输出最低等级");

DEFINE_string(app_id, "120128701", "语言识别app_id");
DEFINE_string(ak, "pdQD7nOBTTkB2XZJS2FaIilp", "语言识别key");
DEFINE_string(sk, "uYZmWaUsF9td2Bkq3JpJ1g43psbyCGcG", "语音识别secret");
int main(int argc, char *argv[])
{
    bubble::init_logger(FLAGS_enable_debug, FLAGS_filename, (spdlog::level::level_enum)FLAGS_level);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    bubble::AsrClient client(FLAGS_app_id, FLAGS_ak, FLAGS_sk);
    std::string file_content;
    aip::get_file_content("./test.pcm", &file_content);
    std::string ret = client.recognize(file_content);
    if(!ret.empty())
    {
        std::cout << ret << std::endl;
    }
    return 0;
}