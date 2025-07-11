cmake_minimum_required(VERSION 3.18)
project(mixcons LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

set(SOURCES
    mixcons/mix_file.cpp
    mixcons/cc_file.cpp
    mixcons/cc_structures.cpp
    mixcons/fname.cpp
    mixcons/pal_file.cpp
    mixcons/palet.cpp
    mixcons/shp_ts_file.cpp
    mixcons/shp_decode.cpp
    mixcons/png_file.cpp
    mixcons/virtual_image.cpp
    mixcons/virtual_binary.cpp
    mixcons/image_file.cpp
    mixcons/image_tools.cpp
    mixcons/shp_images.cpp
    mixcons/video_decoder.cpp
)

add_library(xcc ${SOURCES})
set_target_properties(xcc PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(xcc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/mixcons>
)

find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(BZip2 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LZO2 REQUIRED lzo2)

# vorbis optional, not needed for mixcons extraction

# Link libraries
 target_link_libraries(xcc PUBLIC
    ZLIB::ZLIB
    PNG::PNG
    JPEG::JPEG
    BZip2::BZip2
    ${LZO2_LIBRARIES}
)
 target_include_directories(xcc PUBLIC ${LZO2_INCLUDE_DIRS})

add_executable(mixcons mixcons/mixcons.cpp)
target_link_libraries(mixcons PRIVATE xcc)

