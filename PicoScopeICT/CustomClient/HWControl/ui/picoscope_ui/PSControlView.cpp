#include "PCH.h"

#include "imgui.h"
#include "implot.h"
#include "ImGuiFileDialog.h"

#include "PSControlView.h"

PSControlView::PSControlView(PSDevice* pdDev) : ps_device(pdDev) {}

void PSControlView::Render() {
    ImGui::Begin("WAVEFORM VIEWER");
    ImGui::End();

    ImGui::Begin("CONTROL PANEL");
        ImGui::SeparatorText("CONNECTION STATUS");

        // Update button text based on connection status
        bool isConnected = ps_device->IsDeviceOpen();
        if (ImGui::Button(isConnected ? "DISCONNECT" : "CONNECT", ImVec2(250, 50))) {
            isConnected ? ps_device->CloseDevice() : ps_device->OpenDevice();
        }
        ImGui::Dummy(ImVec2(0.0f, 30.0f));

        if (isConnected) {
            ImGui::SeparatorText("TIMEBASE SETTINGS");

            static int timebase_numeric_current = 0;
            static int timebase_unit_current = 0;
            ImGui::Combo("Timebase Numerical Value", &timebase_numeric_current, timebase_numeric, IM_ARRAYSIZE(timebase_numeric));
            ImGui::Combo("Timebase Unit", &timebase_unit_current, timebase_unit, IM_ARRAYSIZE(timebase_unit));
            ImGui::Text("Timebase: %s %s / div", timebase_numeric[timebase_numeric_current], timebase_unit[timebase_unit_current]);
            ImGui::Dummy(ImVec2(0.0f, 30.0f));

            ImGui::SeparatorText("CHANNEL SETTINGS");
            static bool channel_enabled[4] = {false, false, false, false};
            for (int i = 0; i < 4; i++) {
                ImGui::Checkbox(("Channel " + std::to_string(i)).c_str(), &channel_enabled[i]);
                ImGui::SameLine();
            }
            ImGui::Dummy(ImVec2(0.0f, 30.0f));
            
            ImGui::Dummy(ImVec2(0.0f, 30.0f));

            ImGui::SeparatorText("TRIGGER SETTINGS");
            ImGui::Dummy(ImVec2(0.0f, 30.0f));
        }
    ImGui::End();
}
