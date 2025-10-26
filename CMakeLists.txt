cmake_minimum_required(VERSION 3.28)
project(open_cv_test)

set(CMAKE_CXX_STANDARD 26)

find_package(OpenCV REQUIRED)

add_executable(open_cv_test main.cpp)

target_link_libraries(open_cv_test ${OpenCV_LIBS})
