#pragma once
#include "asr.hpp"
#include "etcd.hpp"
#include "speech.pb.h"
#include <brpc/server.h>
#include <butil/logging.h>
namespace bubble
{
    class SpeechServiceImp : public SpeechService
    {
    public:
        SpeechServiceImp(const AsrClient::ptr &client)
            : _client(client)
        {
        }
        using ptr = std::shared_ptr<SpeechServiceImp>;
        void SpeechRecognition(::google::protobuf::RpcController* controller,
                       const ::bubble::SpeechRecognitionReq* request,
                       ::bubble::SpeechRecognitionRsp* response,
                       ::google::protobuf::Closure* done) override
        {
            DEBUG__LOG("收到语音识别请求");
            brpc::ClosureGuard done_guard(done);
            std::string res = _client->recognize(request->speech_content());
            response->set_request_id(request->request_id());
            if(res.empty())
            {
                response->set_success(false);
                response->set_errmsg("语音识别失败");
                return;
            }
            response->set_success(true);
            response->set_recognition_result(res);
            DEBUG__LOG("语音识别成功: {}", res);
        }
    private:
        AsrClient::ptr _client; // 语音识别客户端
    };

    class SpeechServer
    {
    public:
        using ptr = std::shared_ptr<SpeechServer>;
        SpeechServer(const AsrClient::ptr &asr_client,
            const std::shared_ptr<brpc::Server> &server,
            const Registry::ptr &registry)
            : _server(server), _asr_client(asr_client), _registry(registry)
        {}
        void start()
        {
            _server->RunUntilAskedToQuit();
        }
    private:
        AsrClient::ptr _asr_client; // 语音识别客户端
        std::shared_ptr<brpc::Server> _server; // brpc服务器
        Registry::ptr _registry; // 服务注册
    };
    // 建造者设计模式
    class SpeechServerBuilder
    {
    public:
        SpeechServerBuilder& setAsrClient(std::string app_id, const std::string &ak, const std::string &sk)
        {
            _asr_client = std::make_shared<AsrClient>(app_id, ak, sk);
            return *this;
        }
        SpeechServerBuilder& setBrpcServer(int port, int num_threads = 1)
        {
            if (!_asr_client) {
                ERROR__LOG("还未初始化语音识别模块");
                abort();
            }
            _server = std::make_shared<brpc::Server>();
            SpeechServiceImp *speech_service = new SpeechServiceImp(_asr_client);
            if(_server->AddService(speech_service, brpc::ServiceOwnership::SERVER_OWNS_SERVICE) != 0)
            {
                ERROR__LOG("注册语音识别服务失败");
                abort();
            }
            brpc::ServerOptions options;
            options.num_threads = 1;
            options.idle_timeout_sec = -1;
            if(_server->Start(port, &options) != 0)
            {
                ERROR__LOG("语音服务启动失败");
                abort();
            }
            return *this;
        }
        SpeechServerBuilder& setRegistry(const std::string &etcd_host,
            const std::string &service_name,
            const std::string &sercive_host)
        {
            _registry = std::make_shared<Registry>(etcd_host);
            if(_registry->registerService(service_name, sercive_host) == false)
            {
                ERROR__LOG("向网关注册语音服务失败");
                abort();
            }
            return *this;
        }
        SpeechServer::ptr build()
        {
            if(!_asr_client)
            {
                ERROR__LOG("语音客户端未初始化");
                abort();
            }
            if(!_server)
            {
                ERROR__LOG("brpc服务器未初始化");
                abort();
            }
            if(!_registry)
            {
                ERROR__LOG("注册中心未初始化");
                abort();
            }
            return std::make_shared<SpeechServer>(_asr_client, _server, _registry);
        }
    private:
        AsrClient::ptr _asr_client; // 语音识别客户端
        std::shared_ptr<brpc::Server> _server; // brpc服务器
        Registry::ptr _registry; // 服务注册
    };
}
