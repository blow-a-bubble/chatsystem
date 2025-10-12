cmake_minimum_required(VERSION 3.2)
project(speech_server)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(PROTOC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../proto)
set(COMMON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../common)
set(THIRD_PPARTY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third_party)
set(PROTOC_FILES speech.proto)
set(PROTOC_SRCS "")
foreach(protoc_file ${PROTOC_FILES})
    string(REPLACE "proto" "pb.cc" protoc_cxx ${protoc_file})
    string(REPLACE ".hxx" "pb.h" protoc_hxx ${protoc_file})
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${protoc_cxx} ${CMAKE_CURRENT_BINARY_DIR}/${protoc_hxx}
        COMMAND protoc 
        ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR} -I${PROTOC_PATH} --experimental_allow_proto3_optional ${protoc_file} 
        DEPENDS ${PROTOC_PATH}/${protoc_file}
        COMMENT "生成protoc框架文件 for ${protoc_file}"
    )
    list(APPEND PROTOC_SRCS ${CMAKE_CURRENT_BINARY_DIR}/${protoc_cxx})
endforeach()
file(GLOB SRCS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(speech_server ${PROTOC_SRCS} ${SRCS_FILES})
target_include_directories(speech_server PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${COMMON_PATH}
    ${THIRD_PPARTY_PATH}
)
target_link_libraries(speech_server
    /usr/lib/x86_64-linux-gnu/libjsoncpp.so.1.8.4 
    curl 
    brpc
    crypto
    gflags
    fmt
    spdlog
    protobuf
    etcd-cpp-api 
    cpprest 
    leveldb
    ssl
)


# 测试客户端
add_executable(speech_client ${PROTOC_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/test/speech_client.cpp)
target_include_directories(speech_client PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${COMMON_PATH}
    ${THIRD_PPARTY_PATH}
)

target_link_libraries(speech_client
    /usr/lib/x86_64-linux-gnu/libjsoncpp.so.1.8.4 
    curl 
    brpc
    crypto
    gflags
    fmt
    spdlog
    protobuf
    etcd-cpp-api 
    cpprest 
    leveldb
    ssl
)

add_custom_command(
    TARGET speech_client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/video/test.pcm
            ${CMAKE_CURRENT_BINARY_DIR}/test.pcm
)
