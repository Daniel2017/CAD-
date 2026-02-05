cmake_minimum_required(VERSION 3.10)

project(cad_model_manager VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(SOURCES
    src/model_manager.cpp
    src/geometry_algorithm.cpp
    src/topology_checker.cpp
    src/main.cpp
)

add_executable(cad_model_manager ${SOURCES})

# 设置编译选项
if(MSVC)
    target_compile_options(cad_model_manager PRIVATE /W4 /WX)
else()
    target_compile_options(cad_model_manager PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 安装配置
install(TARGETS cad_model_manager DESTINATION bin)
install(DIRECTORY include/ DESTINATION include/cad_model_manager)

# 测试目标
enable_testing()
add_test(NAME cad_model_manager_test COMMAND cad_model_manager)
