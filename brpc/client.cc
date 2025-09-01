#include "echo.pb.h"
#include <iostream>
#include <brpc/channel.h>
#include <memory>
#include <thread>
#include <chrono>
// 异步回调
void Callback(brpc::Controller *controller, example::EchoResponse *response)
{
    
    std::unique_ptr<brpc::Controller> controller_guard(controller);
    std::unique_ptr<example::EchoResponse> response_guard(response);
    if(controller->Failed())
    {
        std::cout << "调用失败: " << controller->ErrorText() << std::endl;
        return;
    }
    std::cout << "调用成功, 响应: " << response->message() << std::endl;
}
int main()
{
    // 1. 创建客户端对象
    brpc::Channel channel;
    brpc::ChannelOptions options;
    options.protocol = "baidu_std";
    options.timeout_ms = -1;
    options.max_retry = 3;
    if(channel.Init("127.0.0.1:8080", &options) != 0)
    {
        std::cout << "初始化客户端失败" << std::endl;
        return -1;
    }
    // 2. 创建服务请求代理对象
    example::EchoService_Stub stub(&channel);
/*
    // 3. 同步发送请求
    example::EchoRequest request;
    request.set_message("hello world");
    example::EchoResponse response;
    brpc::Controller cntl;
    stub.Echo(&cntl, &request, &response, nullptr);
    // 4.处理响应
    if(cntl.Failed())
    {
        std::cout << "调用失败: " << cntl.ErrorText() << std::endl;
        return -1;
    }
    std::cout << "调用成功, 响应: " << response.message() << std::endl;
*/
    // 3. 异步发送请求
    example::EchoRequest *request = new example::EchoRequest();
    request->set_message("hello world");
    example::EchoResponse *response = new example::EchoResponse();
    brpc::Controller *cntl = new brpc::Controller();
    google::protobuf::Closure *closure = google::protobuf::NewCallback(Callback, cntl, response);
    stub.Echo(cntl, request, response, closure);
    std::cout << "异步调用结束" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}