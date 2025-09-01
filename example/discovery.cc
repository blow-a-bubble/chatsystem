#include "etcd.hpp"
#include "echo.pb.h"
#include "channel.hpp"
#include <brpc/server.h>
#include <butil/logging.h>
#include <gflags/gflags.h>
#include <iostream>
#include <memory>
DEFINE_bool(enable_debug, true, "是否开启debug模式");
DEFINE_string(filename, "async.log", "日志文件名");
DEFINE_int32(level, spdlog::level::level_enum::debug, "日志输出最低等级");

DEFINE_int32(brpc_server_port, 7070, "brpc服务器端口");
DEFINE_string(etcd_host, "htpp://127.0.0.1:2379", "etcd的地址");
DEFINE_string(care_service, "/server/echo", "关心的服务名称");


int main(int argc, char** argv)
{
    init_logger(FLAGS_enable_debug, FLAGS_filename, (spdlog::level::level_enum)FLAGS_level);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    // 1.定义服务管理对象
    std::shared_ptr<ServiceManager> sm = std::make_shared<ServiceManager>();
    // 2.关心服务
    sm->declared(FLAGS_care_service);
    // 3.注册和发现中心
    auto put_cb = std::bind(&ServiceManager::onServiceOnline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    auto del_cb = std::bind(&ServiceManager::onServiceOffline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    Discovery discovery(FLAGS_etcd_host, FLAGS_care_service, put_cb, del_cb);
    
    while(1)
    {
        // 4.获取通讯节点
        auto node = sm->choose(FLAGS_care_service);
        if(!node)
        {
            break;
        }
        // 5.调用rpc的echo服务
        example::EchoService_Stub echo(node.get());
        brpc::Controller ctl;
        example::EchoRequest req;
        req.set_message("你好");
        example::EchoResponse resp;
        echo.Echo(&ctl, &req, &resp, nullptr);
        if(ctl.Failed())
        {
            std::cout << "调用失败: " << ctl.ErrorText() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        std::cout << "调用成功, 响应: " << resp.message() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}