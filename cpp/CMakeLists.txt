project(xlsx-util)
cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)

# add subdirectory dependencies
add_subdirectory(third-party/libexpat)
add_subdirectory(third-party/zlib-gyp)

## add minizip sources?
#set (
#        SOURCE
#        ${SOURCE}
#        cpp/third-party/zlib-1.2.11/contrib/minizip/ioapi.c
##        src/third-party/zlib-1.2.11/contrib/minizip/iowin32.c // TODO - compile conditionally?
#        cpp/third-party/zlib-1.2.11/contrib/minizip/mztools.c
#        cpp/third-party/zlib-1.2.11/contrib/minizip/unzip.c
#        cpp/third-party/zlib-1.2.11/contrib/minizip/zip.c
#    )

# add xlsx-util sources
#set (
#        SOURCE
#        ${SOURCE}
#        ../cpp/parse-node.cpp
#    )

# add xlsxio sources directly to our own target because the xlsxio
# cmake file uses find_package for zlib and expat
set (
        SOURCE
        ${SOURCE}
        third-party/xlsxio/lib/xlsxio_read_sharedstrings.c
        third-party/xlsxio/lib/xlsxio_read.c
        third-party/xlsxio/lib/xlsxio_write.c
    )

# xlsxio needs this defined
set(ENV{USE_MINIZIP} "SURE")

# create the main project file
add_library(xlsx-util SHARED ${SOURCE})
target_include_directories(xlsx-util PRIVATE cpp/third-party/xlsxio/lib)
target_include_directories(xlsx-util PRIVATE cpp/third-party/xlsxio/include)
target_include_directories(xlsx-util PRIVATE cpp/third-party/zlib-1.2.11/contrib/minizip)

# Include N-API wrappers
target_include_directories(xlsx-util PRIVATE "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api")

# link subdirectory libraries
target_link_libraries(xlsx-util expat)
target_link_libraries(xlsx-util zlib)

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})