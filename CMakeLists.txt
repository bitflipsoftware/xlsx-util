project(xlsx-util)
cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)

# add subdirectory dependencies
add_subdirectory(src/third-party/libexpat-R_2_2_5/expat)
add_subdirectory(src/third-party/zlib-1.2.11)

# add xlsx-util sources
set (
        SOURCE
        ${SOURCE}
        src/main.cpp
    )

# add xlsxio sources directly to our own target because the xlsxio
# cmake file uses find_package for zlib and expat
set (
        SOURCE
        ${SOURCE}
        src/third-party/xlsxio/lib/xlsxio_read_sharedstrings.c
        src/third-party/xlsxio/lib/xlsxio_read.c
        src/third-party/xlsxio/lib/xlsxio_write.c
    )



# create the main project file
add_executable(test-main ${SOURCE})

# link subdirectory libraries
target_link_libraries(test-main expat)
target_link_libraries(test-main zlib)