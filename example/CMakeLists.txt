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


add_executable(es_test es_test.cc)
target_include_directories(es_test PRIVATE ${CMAKE_SOURCE_DIR}/../common/)
target_link_libraries(es_test PRIVATE
    jsoncpp 
    spdlog 
    fmt 
    gflags
    elasticlient
    cpr
)






