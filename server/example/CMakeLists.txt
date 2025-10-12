cmake_minimum_required(VERSION 3.2)
project(example)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
# add_executable(discovery discovery.cc echo.pb.cc)
# target_include_directories(discovery PRIVATE ${CMAKE_SOURCE_DIR}/../common/)
# target_link_libraries(discovery PRIVATE
#     etcd-cpp-api 
#     cpprest
#     brpc 
#     leveldb
#     gflags 
#     ssl 
#     crypto 
#     protobuf
#     fmt
# )

# add_executable(registry registry.cc echo.pb.cc)
# target_include_directories(registry PRIVATE ${CMAKE_SOURCE_DIR}/../common/)
# target_link_libraries(registry PRIVATE
#     etcd-cpp-api 
#     cpprest
#     brpc 
#     leveldb
#     gflags 
#     ssl 
#     crypto 
#     protobuf
#     fmt
# )


# add_executable(es_test es_test.cc)
# target_include_directories(es_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/)
# target_link_libraries(es_test PRIVATE
#     jsoncpp 
#     spdlog 
#     fmt 
#     gflags
#     elasticlient
#     cpr
# )


# add_executable(mq_server mq_server.cc)
# add_executable(mq_client mq_client.cc)
# target_include_directories(mq_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/)
# target_include_directories(mq_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/)
# target_link_libraries(mq_server PRIVATE
#     spdlog
#     fmt
#     gflags
#     amqpcpp 
#     ev 
#     pthread
# )
# target_link_libraries(mq_client PRIVATE
#     spdlog
#     fmt
#     gflags
#     amqpcpp 
#     ev 
#     pthread
# )


# add_executable(asr_test asr_test.cc)
# target_include_directories(asr_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/ ${CMAKE_CURRENT_SOURCE_DIR}/../third_party)
# target_link_libraries(asr_test PRIVATE
#     /usr/lib/x86_64-linux-gnu/libjsoncpp.so.1.8.4 
#     curl 
#     crypto
#     gflags
#     fmt
#     spdlog
# )

add_executable(dms_test dms_test.cc)
target_include_directories(dms_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/)
target_link_libraries(dms_test PRIVATE
    gflags
    fmt
    spdlog
    alibabacloud-sdk-core
)

