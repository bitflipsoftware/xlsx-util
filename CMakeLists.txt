project(xlsx-util)
cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)

# add subdirectory dependencies
add_subdirectory(src/third-party/libexpat-R_2_2_5/expat)
add_subdirectory(src/third-party/zlib-1.2.11)

# add minizip sources?
set (
        SOURCE
        ${SOURCE}
        src/third-party/zlib-1.2.11/contrib/minizip/ioapi.c
#        src/third-party/zlib-1.2.11/contrib/minizip/iowin32.c // TODO - compile conditionally?
        src/third-party/zlib-1.2.11/contrib/minizip/mztools.c
        src/third-party/zlib-1.2.11/contrib/minizip/unzip.c
        src/third-party/zlib-1.2.11/contrib/minizip/zip.c
    )

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

# xlsxio needs this defined
set(ENV{USE_MINIZIP} "SURE")

# create the main project file
add_executable(test-main ${SOURCE})
target_include_directories(test-main PRIVATE src/third-party/xlsxio/lib)
target_include_directories(test-main PRIVATE src/third-party/xlsxio/include)
target_include_directories(test-main PRIVATE src/third-party/zlib-1.2.11/contrib/minizip)


# link subdirectory libraries
target_link_libraries(test-main expat)
target_link_libraries(test-main zlib)