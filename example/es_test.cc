#include "es.hpp"
#include <gflags/gflags.h>
DEFINE_bool(enable_debug, true, "是否开启debug模式");
DEFINE_string(filename, "async.log", "日志文件名");
DEFINE_int32(level, spdlog::level::level_enum::debug, "日志输出最低等级");
int main(int argc, char *argv[]) 
{
    init_logger(FLAGS_enable_debug, FLAGS_filename, (spdlog::level::level_enum)FLAGS_level);
    std::shared_ptr<elasticlient::Client> client(new elasticlient::Client({"http://localhost:9200/"}));
    ESIndex index(client, "test_user");
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
    // 插入
    ESInsert esi(client, "test_user");
    ret = esi.append("nickname", "张三")
         .append("phone", "123456")
         .insert("2");
    if(ret == false)
    {
        ERROR__LOG("insert data failed");
    }
    else
    {
        INFO__LOG("insert data success");
    }
    // 删除
    ESRemove esr(client, "test_user");
    ret = esr.remove("1");
    if(ret == false)
    {
        ERROR__LOG("delete data failed");
    }
    else
    {
        INFO__LOG("delete data success");
    }

    // 查询
    ESSearch esq(client, "test_user");
    esq.append_must_not_terms("nickname.keyword", {"李四"});
    esq.append_should_match("phone.keyword", "123456");
    Json::Value data = esq.search();
    if(!data.isNull())
    {
        for(int i = 0; i < data.size(); ++i)
        {
            std::cout << "nickname" << data[i]["_source"]["nickname"] << std::endl;
            std::cout << "phone" << data[i]["_source"]["phone"] << std::endl;
        }
    }
    return 0;
}