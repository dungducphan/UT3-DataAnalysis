# Set CMake minimum version
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

# Set project name and version
project(CameraControl VERSION 0.1.0 LANGUAGES CXX)

# If WIN32 is defined
add_definitions(-DWIN32)

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
set(PICO_SDK_DIR "C:/Program\ Files/Pico\ Technology/SDK")
add_library(hw_interface STATIC
        # camera
        camera_interface/Camera.cpp
        camera_interface/Basler/BaslerCamera.cpp
        camera_interface/Basler/BaslerCameraProperties.cpp
        camera_interface/FLIR/FLIRCamera.cpp
        camera_interface/FLIR/FLIRCameraProperties.cpp
        camera_interface/PCO/PCOCamera.cpp
        camera_interface/PCO/PCOCameraProperties.cpp
        camera_interface/CameraManager.cpp
        camera_interface/CameraController.cpp
        # scope
        scope_interface/ScopeManager.h
        scope_interface/ScopeManager.cpp
)

# Specify include directories for hw_interface
target_include_directories(hw_interface PUBLIC
        ${CMAKE_SOURCE_DIR}/camera_interface
        ${CMAKE_SOURCE_DIR}/camera_interface/Basler
        ${CMAKE_SOURCE_DIR}/camera_interface/FLIR
        ${CMAKE_SOURCE_DIR}/camera_interface/PCO
        ${CMAKE_SOURCE_DIR}/scope_interface
        ${PICO_SDK_DIR}/inc
)

# Set library output name
set_target_properties(hw_interface PROPERTIES OUTPUT_NAME "hw_interface")

# Link libraries
if (WIN32)
    target_link_libraries(hw_interface PRIVATE user32 gdi32 kernel32)
endif()
link_directories(${PICO_SDK_DIR}/lib)
target_link_libraries(hw_interface PRIVATE ps3000)

# ===================================================================
# HardwareControl executable
# ===================================================================

# Add executable
include_directories(${CMAKE_SOURCE_DIR})
include_directories("ui")
include_directories("ui/camera_ui")
include_directories("ui/picoscope_ui")
include_directories("camera_interface")
include_directories("scope_interface")

add_executable(HardwareControl
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
        ui/camera_ui/CameraControlView.cpp
        ui/camera_ui/CameraControlView.h
        ui/picoscope_ui/ScopeControlView.cpp
        ui/picoscope_ui/ScopeControlView.h

        # Entrypoint
        main.cpp

        # PCH
        PCH.h
)

# Specify include directories
target_include_directories(HardwareControl PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/ui)

# Link libraries
if (WIN32)
    target_link_libraries(HardwareControl PRIVATE user32 gdi32 kernel32)
endif()
target_link_libraries(HardwareControl PRIVATE glfw glad hw_interface)