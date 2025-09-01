#include "es.hpp"
#include <gflags/gflags.h>
DEFINE_bool(enable_debug, true, "是否开启debug模式");
DEFINE_string(filename, "async.log", "日志文件名");
DEFINE_int32(level, spdlog::level::level_enum::debug, "日志输出最低等级");
int main(int argc, char *argv[]) 
{
    init_logger(FLAGS_enable_debug, FLAGS_filename, (spdlog::level::level_enum)FLAGS_level);
    ESIndex index("http://localhost:9200/", "test_user");
    bool ret = index.append("nickname", "text", "ik_max_word")
         .append("phone", "keyword", "standard")
         .create();
    if(ret == false)
    {
        ERROR__LOG("create index failed");
    }
    else
    {
        INFO__LOG("create index success");
    }
    return 0;
}