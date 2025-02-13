#include "PCH.h"

// opengl backend
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// imgui
#include "imgui.h"
#include "implot.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// scope
#include "PSDataProcessor.h"
#include "PSConfig.h"

// ui
#include "ImGuiConfig.h"
#include "PSPostProcessingView.h"

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1600, 1200, "CameraControl", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setup ImGui
    SetupImGui(window);

    // Create ImPlot context
    ImPlot::CreateContext();

    auto processor = new PSDataProcessor();
    PSPostProcessingView view(processor);

    // Setup the Picoscope
    printf("ICT Beam Charge Measurement with PicoScope 3206D\n");
    printf("Searching PicoScope...\n");
    UNIT unit;
    PICO_STATUS status = openDevice(&unit);
    displaySettings(&unit);
    collectBlockTriggered(&unit);

    // Frame Update Loop
    while (!glfwWindowShouldClose(window)) {
        // Handle events
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a docking space and set it as the default docking space
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);

        // Show the PicoScope Control View
        // Anything in Render() will be executed every frame
        view.Render();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Closing PicoScope
    closeDevice(&unit);

    // Destroy ImPlot context
    ImPlot::DestroyContext();

    // Clean up
    CleanupImGui();
    glfwTerminate();

    delete processor;

    return 0;
}
