project(xlsx-util)
cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)

# add subdirectory dependencies
add_subdirectory(src/third-party/libexpat-R_2_2_5/expat)
add_subdirectory(src/third-party/zlib-1.2.11)

# create the main project file
add_executable(test-main src/main.cpp)

# link subdirectory libraries
target_link_libraries(test-main expat)
target_link_libraries(test-main zlib)