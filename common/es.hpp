#pragma once
#include "logger.hpp"
#include <elasticlient/client.h>
#include <cpr/response.h>
#include <json/json.h>
bool serialize(const Json::Value &root, std::string &data)
{
    Json::StreamWriterBuilder builder;
    std::shared_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::stringstream json_str;
    int ret = writer->write(root, &json_str);
    if(ret != 0)
    {
        return false;
    }
    data = json_str.str();
    return true;
}

bool deserialize(const std::string &data, Json::Value &root)
{
    Json::CharReaderBuilder builder;
    std::shared_ptr<Json::CharReader> reader(builder.newCharReader());
    std::string errs;
    bool ret = reader->parse(data.c_str(), data.c_str() + data.size(), &root, &errs);
    if(ret == false)
    {
        return false;
    }  
    return true; 
}

// es建立索引
class ESIndex
{
public:
    // host 必须要/结尾 eg: "http://localhost:9200/"
    ESIndex(const std::string& host, std::string index_name, std::string index_type = "_doc")
    :_index_name(index_name), _index_type(index_type)
    {
        std::vector<std::string> hostlist = {host};
        _client = std::make_shared<elasticlient::Client>(hostlist);
        Json::Value settings;
        Json::Value analysis;
        Json::Value analyzer;
        Json::Value ik;
        ik["tokenizer"] = "ik_max_word";
        analyzer["ik"] = ik;
        analysis["analyzer"] = analyzer;
        settings["analysis"] = analysis;
        _req_body["settings"] = settings;
        _mappings["dynamic"] = true;
    }
    ESIndex& append(const std::string& field_name, const std::string& type, const std::string& analyzer, bool enabled = true)
    {
        Json::Value field;
        if(!type.empty()) field["type"] = type;
        if(!analyzer.empty()) field["analyzer"] = analyzer;
        if(enabled == false) field["enabled"] = false;
        _properties[field_name] = field;
        return *this;
    }
    bool create(const std::string &index_id = "default")
    {
        _mappings["properties"] = _properties;
        _req_body["mappings"] = _mappings;
        std::string req;
        bool ret = serialize(_req_body, req);
        if(ret == false)
        {
            ERROR__LOG("创建索引请求进行序列化失败");
            return false;
        }
        DEBUG__LOG("body: {}", req);
        try
        {
            auto resp = _client->index(_index_name, _index_type, index_id, req);
            if(resp.status_code < 200 || resp.status_code >= 300)
            {
                ERROR__LOG("创建es索引失败, 返回状态码: {}", resp.status_code);
                return false;
            }
        }
        catch(const std::exception& e)
        {
            ERROR__LOG("创建es索引失败: {}", e.what());
            return false;
        }
        return true;
    }
private:
    std::shared_ptr<elasticlient::Client> _client;
    std::string _index_name;
    std::string _index_type;
    Json::Value _req_body;
    Json::Value _mappings;
    Json::Value _properties;
};

// es插入数据
class ESInsert
{

};

// ES查询数据
class ESQuery
{
    
};
