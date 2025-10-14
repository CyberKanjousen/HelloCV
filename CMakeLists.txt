cmake_minimum_required(VERSION 4.0)
project(HelloCV)

set(CMAKE_CXX_STANDARD 20)

include_directories("D:\\Myproject\\opencv\\build\\include")
include_directories("D:\\Myproject\\opencv\\build\\include\\opencv2")
link_directories("D:\\Myproject\\opencv\\build\\x64\\vc16\\lib")

add_executable(HelloCV main.cpp)

target_link_libraries(HelloCV opencv_world4120d.lib)