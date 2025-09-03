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
    ESIndex(const std::shared_ptr<elasticlient::Client> &client, 
            std::string index_name, std::string index_type = "_doc")
    :_index_name(index_name), _index_type(index_type), _client(client)
    {
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
    ESIndex &append(const std::string& field_name, const std::string& type, const std::string& analyzer, bool enabled = true)
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
public:
    ESInsert(const std::shared_ptr<elasticlient::Client> &client, 
            std::string index_name, std::string index_type = "_doc")
    :_index_name(index_name), _index_type(index_type), _client(client)
    {}
    ESInsert &append(const std::string &key, const std::string &value)
    {
        _req_body[key] = value;
    }
    bool insert(const std::string &id)
    {
        std::string req;
        bool ret = serialize(_req_body, req);
        if(ret == false)
        {
            ERROR__LOG("增加数据请求进行序列化失败");
            return false;
        }
        DEBUG__LOG("body: {}", req);
        try
        {
            auto resp = _client->index(_index_name, _index_type, id, req);
            if(resp.status_code < 200 || resp.status_code >= 300)
            {
                ERROR__LOG("增加数据失败, 返回状态码: {}", resp.status_code);
                return false;
            }
        }
        catch(const std::exception& e)
        {
            ERROR__LOG("增加数据es索引失败: {}", e.what());
            return false;
        }
        return true;
    }
private:
    std::shared_ptr<elasticlient::Client> _client;
    std::string _index_name;
    std::string _index_type;
    Json::Value _req_body;
};

// es修改数据: 修改和插入一模一样直接插入覆盖即可
class ESModify
{
public:
    ESModify(const std::shared_ptr<elasticlient::Client> &client,
            std::string index_name, std::string index_type = "_doc")
    :_index_name(index_name), _index_type(index_type), _client(client)
    {}
    ESModify &append(const std::string &key, const std::string &value)
    {
        _req_body[key] = value;
    }
    bool insert(const std::string &id)
    {
        std::string req;
        bool ret = serialize(_req_body, req);
        if(ret == false)
        {
            ERROR__LOG("修改数据请求进行序列化失败");
            return false;
        }
        DEBUG__LOG("body: {}", req);
        try
        {
            auto resp = _client->index(_index_name, _index_type, id, req);
            if(resp.status_code < 200 || resp.status_code >= 300)
            {
                ERROR__LOG("修改数据失败, 返回状态码: {}", resp.status_code);
                return false;
            }
        }
        catch(const std::exception& e)
        {
            ERROR__LOG("修改数据es索引失败: {}", e.what());
            return false;
        }
        return true;
    }
private:
    std::shared_ptr<elasticlient::Client> _client;
    std::string _index_name;
    std::string _index_type;
    Json::Value _req_body;
};
// es删除数据
class ESRemove
{
public:
    ESRemove(const std::shared_ptr<elasticlient::Client> &client, 
            std::string index_name, std::string index_type = "_doc")
    :_index_name(index_name), _index_type(index_type), _client(client)
    {}
    bool remove(const std::string &id)
    {
        try
        {
            auto resp = _client->remove(_index_name, _index_type);
            if(resp.status_code < 200 || resp.status_code >= 300)
            {
                ERROR__LOG("删除数据失败, 返回状态码: {}", resp.status_code);
                return false;
            }
        }
        catch(const std::exception& e)
        {
            ERROR__LOG("删除数据es索引失败: {}", e.what());
            return false;
        }
        return true;
    }
private:
    td::shared_ptr<elasticlient::Client> _client;
    std::string _index_name;
    std::string _index_type;
};
// ES查询数据
class ESQuery
{
public:
    ESQuery(const std::shared_ptr<elasticlient::Client> &client, 
            std::string index_name, std::string index_type = "_doc")
    :_client(client), _index_name(index_name), _index_type(index_type)
    {}
private:
    td::shared_ptr<elasticlient::Client> _client;
    std::string _index_name;
    std::string _index_type;
};
