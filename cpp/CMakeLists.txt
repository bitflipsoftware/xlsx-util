project(xlsx-test)
cmake_minimum_required(VERSION 3.1)

# Require C99 and C++11 compilers
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(xlsxio)
add_executable(xlsx-test main.cpp)
target_link_libraries(xlsx-test xlsxio)