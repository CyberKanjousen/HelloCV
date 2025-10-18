cmake_minimum_required(VERSION 4.0)
project(EncryptApplication)

set(CMAKE_CXX_STANDARD 23)

add_executable(CryptoTool main.cpp
        crypto_tool.cpp
        crypto_tool.h
)
