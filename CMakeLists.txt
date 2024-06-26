cmake_minimum_required(VERSION 3.28)
project(UT3DA)

set(CMAKE_CXX_STANDARD 17)

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

find_package(TIFF REQUIRED)
include_directories(${TIFF_INCLUDE_DIRS})

add_executable(UT3DA main.cpp)
target_link_libraries(UT3DA ${ROOT_LIBRARIES} ${TIFF_LIBRARIES})
