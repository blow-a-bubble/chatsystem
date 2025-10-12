#include "asr.hpp"
#include "etcd.hpp"
#include "logger.hpp"
#include "speech.pb.h"
#include "channel.hpp"
#include <butil/logging.h>
#include <gflags/gflags.h>
#include <memory>
// 日志
DEFINE_bool(enable_debug, true, "是否开启debug模式");
DEFINE_string(filename, "async.log", "日志文件名");
DEFINE_int32(level, spdlog::level::level_enum::debug, "日志输出最低等级");

// 服务
DEFINE_string(etcd_host, "http://127.0.0.1:2379", "etcd的地址");
DEFINE_string(discovery_host, "/service", "注册中心关心的服务");
DEFINE_string(care_service, "/service/speech_service", "关心服务名称");

int main(int argc, char *argv[])
{
     // 初始化日志模块
    bubble::init_logger(FLAGS_enable_debug, FLAGS_filename, (spdlog::level::level_enum)FLAGS_level);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    // 取消brpc自带的日志
    logging::LoggingSettings settings;
    settings.logging_dest = logging::LoggingDestination::LOG_TO_NONE;
    logging::InitLogging(settings);

    bubble::ServiceManager services;
    services.declared(FLAGS_care_service);
    auto put_cb = std::bind(&bubble::ServiceManager::onServiceOnline, &services, std::placeholders::_1, std::placeholders::_2);
    auto del_cb = std::bind(&bubble::ServiceManager::onServiceOffline, &services, std::placeholders::_1, std::placeholders::_2);
    bubble::Discovery centor(FLAGS_etcd_host, FLAGS_discovery_host, put_cb, del_cb);
    while(1)
    {
        // 4.获取通讯节点
        auto node = services.choose(FLAGS_care_service);
        if(!node)
        {
            break;
        }
        // 5.调用rpc的echo服务
        bubble::SpeechService_Stub speech(node.get());
        brpc::Controller ctl;
        std::string file_content;
        aip::get_file_content("./test.pcm", &file_content);
        bubble::SpeechRecognitionReq req;
        req.set_request_id("1");
        req.set_speech_content(file_content);
        bubble::SpeechRecognitionRsp resp;
        speech.SpeechRecognition(&ctl, &req, &resp, nullptr);
        if(ctl.Failed())
        {
            std::cout << "调用失败: " << ctl.ErrorText() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        std::cout << "调用成功, 响应: " << resp.recognition_result() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        break;
    }
    return 0;
}