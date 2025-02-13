# Set CMake minimum version
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

# Set project name and version
project(CameraControl)

# If WIN32 is defined
if (WIN32)
    add_definitions(-DWIN32)
endif()

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

# ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

# imgui
include_directories(SYSTEM "external/imgui")
include_directories(SYSTEM "external/imgui/backends")
include_directories(SYSTEM "external/implot")
include_directories(SYSTEM "external/ImGuiFileDialog")

# glad
include_directories(SYSTEM "external/glad/include")
add_subdirectory(external/glad)

# glfw
include_directories(SYSTEM "external/glfw-3.4/include")
add_subdirectory(external/glfw-3.4)

# picoscope sdk
include_directories(SYSTEM "external/pico_sdk/inc")
link_directories("external/pico_sdk/lib")

# Copy fonts
file(COPY "external/fonts/FiraMonoNerdFont-Medium.otf" DESTINATION "${CMAKE_BINARY_DIR}")

# ===================================================================
# hw_interface library
# ===================================================================
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
        scope_interface/PSDataProcessor.cpp
        scope_interface/PSDriver.cpp
)

# Specify include directories for hw_interface
target_include_directories(hw_interface PRIVATE
        ${CMAKE_SOURCE_DIR}/camera_interface
        ${CMAKE_SOURCE_DIR}/camera_interface/Basler
        ${CMAKE_SOURCE_DIR}/camera_interface/FLIR
        ${CMAKE_SOURCE_DIR}/camera_interface/PCO
        ${CMAKE_SOURCE_DIR}/scope_interface
        ${CMAKE_SOURCE_DIR}/external/pico_sdk/inc
)

# Set library output name
set_target_properties(hw_interface PROPERTIES OUTPUT_NAME "hw_interface")
target_link_libraries(hw_interface PRIVATE ps3000a.lib)

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

        # implot
        external/implot/implot.h
        external/implot/implot_internal.h
        external/implot/implot.cpp
        external/implot/implot_items.cpp

        # imguiFileDialog
        external/ImGuiFileDialog/ImGuiFileDialogConfig.h
        external/ImGuiFileDialog/ImGuiFileDialog.h
        external/ImGuiFileDialog/ImGuiFileDialog.cpp

        # UI
        ui/ImGuiConfig.h
        ui/camera_ui/CameraControlView.cpp
        ui/camera_ui/CameraControlView.h
        ui/picoscope_ui/PSPostProcessingView.cpp
        ui/picoscope_ui/PSPostProcessingView.h

        # Entrypoint
        main.cpp

        # PCH
        PCH.h
)

# Specify include directories
target_include_directories(HardwareControl PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/ui)

# Link libraries
target_link_libraries(HardwareControl PRIVATE glfw glad hw_interface ${ROOT_LIBRARIES})