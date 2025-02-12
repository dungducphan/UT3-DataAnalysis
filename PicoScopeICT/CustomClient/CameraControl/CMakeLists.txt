# Set CMake minimum version
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

# Set project name and version
project(CameraControl VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
endif()

# imgui
include_directories(SYSTEM "external/imgui")

# glad
include_directories(SYSTEM "external/glad/include")
add_subdirectory(external/glad)

# glfw
include_directories(SYSTEM "external/glfw-3.4/include")
add_subdirectory(external/glfw-3.4)

# Copy fonts
file(COPY "external/fonts/FiraMonoNerdFont-Medium.otf" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# ===================================================================
# hw_interface library
# ===================================================================
add_library(hw_interface STATIC
        hw_interface/Camera.cpp
        hw_interface/Basler/BaslerCamera.cpp
        hw_interface/Basler/BaslerCameraProperties.cpp
        hw_interface/FLIR/FLIRCamera.cpp
        hw_interface/FLIR/FLIRCameraProperties.cpp
        hw_interface/PCO/PCOCamera.cpp
        hw_interface/PCO/PCOCameraProperties.cpp
        hw_interface/CameraManager.cpp
        hw_interface/CameraController.cpp
)

# Specify include directories for hw_interface
target_include_directories(hw_interface PUBLIC
        ${CMAKE_SOURCE_DIR}/hw_interface
        ${CMAKE_SOURCE_DIR}/hw_interface/Basler
        ${CMAKE_SOURCE_DIR}/hw_interface/FLIR
        ${CMAKE_SOURCE_DIR}/hw_interface/PCO
)

# Set library output name
set_target_properties(hw_interface PROPERTIES OUTPUT_NAME "hw_interface")

# ===================================================================
# CameraControl executable
# ===================================================================

# Add executable
include_directories(${CMAKE_SOURCE_DIR})
include_directories("ui")
include_directories("hw_interface")
add_executable(CameraControl
        # imgui core library
        external/imgui/imgui_demo.cpp
        external/imgui/imgui.h
        external/imgui/imgui.cpp
        external/imgui/imconfig.h
        external/imgui/imgui_internal.h
        external/imgui/imstb_rectpack.h
        external/imgui/imstb_textedit.h
        external/imgui/imstb_truetype.h
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui_tables.cpp

        # opengl/glfw imgui backends
        external/imgui/backends/imgui_impl_glfw.h
        external/imgui/backends/imgui_impl_glfw.cpp
        external/imgui/backends/imgui_impl_opengl3.h
        external/imgui/backends/imgui_impl_opengl3.cpp
        external/imgui/backends/imgui_impl_opengl3_loader.h

        # UI
        ui/ImGuiConfig.h
        ui/CameraControlView.cpp
        ui/CameraControlView.h

        # Entrypoint
        main.cpp

        # PCH
        PCH.h
)

# Specify include directories
target_include_directories(CameraControl PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/ui)

# Link libraries
target_link_libraries(CameraControl PRIVATE glfw glad hw_interface)