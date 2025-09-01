cmake_minimum_required(VERSION 3.10)
project(TEST)

add_executable(main main.cc)
add_executable(test_c test.cc)

# 测试
include(CTest)
add_test(NAME TEST COMMAND test_c)

# 下载
include(GNUInstallDirs)
install(TARGETS main)

# 打包
include(CPack)


