#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

inline void SetupImGui(GLFWwindow* window) {
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
    io.Fonts->AddFontFromFileTTF("FiraMonoNerdFont-Medium.otf", 21.0f);
    io.IniFilename = "imgui.ini";

    // Set ImGui style
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 8.0f; // Set the desired rounding value
    style.WindowBorderSize = 0.0f; // Remove window border
    style.PopupBorderSize = 0.0f; // Remove popup border
    style.AntiAliasedLines = true; // Enable anti-aliased lines
    style.AntiAliasedFill = true; // Enable anti-aliased fill
    style.AntiAliasedLinesUseTex = true; // Enable anti-aliased lines using texture
    style.ScaleAllSizes(1); // Apply scaling

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

inline void CleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}