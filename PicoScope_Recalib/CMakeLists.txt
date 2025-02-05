cmake_minimum_required(VERSION 3.10)

# Set the project name
project(PicoScope_Recalib)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

# Add the executable
add_executable(PicoScope_Recalib main.cpp)
target_link_libraries(PicoScope_Recalib PUBLIC ${ROOT_LIBRARIES})
