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

        static bool isConnected = ps_device->IsDeviceOpen();
        static int timebase_numeric_current = 0;
        static int timebase_unit_current = 0;
        static int channel_0_range_current = 5;
        static int channel_0_coupling_current = 1;
        static int channel_1_range_current = 5;
        static int channel_1_coupling_current = 1;
        static int trigger_source_current = 0;
        static float trigger_threshold = 0.0f;
        static int trigger_direction_current = 0;
        static float pre_trigger = 0.0f;

        if (ImGui::Button(isConnected ? "DISCONNECT" : "CONNECT", ImVec2(250, 50))) {
            isConnected ? ps_device->CloseDevice() : ps_device->OpenDevice();
            isConnected = ps_device->IsDeviceOpen();
        }
        ImGui::Dummy(ImVec2(0.0f, 30.0f));

        if (isConnected) {
            ImGui::SeparatorText("TIMEBASE SETTINGS");
            ImGui::Combo("Timebase Numerical Value", &timebase_numeric_current, timebase_numeric, IM_ARRAYSIZE(timebase_numeric));
            ImGui::Combo("Timebase Unit", &timebase_unit_current, timebase_unit, IM_ARRAYSIZE(timebase_unit));
            ImGui::Text("Timebase: %s %s / div", timebase_numeric[timebase_numeric_current], timebase_unit[timebase_unit_current]);
            ImGui::Dummy(ImVec2(0.0f, 30.0f));
            ImGui::SeparatorText("CHANNEL SETTINGS");
            if (ImGui::BeginTabBar("Channels")) {
                if (ImGui::BeginTabItem("Channel 00")) {
                    ImGui::Checkbox("Enable Channel 00", &channel_enabled[0]);
                    if (channel_enabled[0]) {
                        ImGui::Combo("Channel 0 Coupling", &channel_0_coupling_current, channel_coupling, IM_ARRAYSIZE(channel_coupling));
                        ImGui::Combo("Channel 0 Vertical Range", &channel_0_range_current, channel_range, IM_ARRAYSIZE(channel_range));
                        ImGui::InputFloat("Offset", &channel_0_offset);
                        channel_0_range = std::stof(channel_range[channel_0_range_current]);
                        if (channel_0_coupling_current == 0) {
                            channel_0_AC_coupling = true;
                        } else {
                            channel_0_AC_coupling = false;
                        }
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Channel 01")) {
                    ImGui::Checkbox("Enable Channel 01", &channel_enabled[1]);
                    if (channel_enabled[1]) {
                        ImGui::Combo("Channel 0 Coupling", &channel_1_coupling_current, channel_coupling, IM_ARRAYSIZE(channel_coupling));
                        ImGui::Combo("Channel 0 Vertical Range", &channel_1_range_current, channel_range, IM_ARRAYSIZE(channel_range));
                        ImGui::InputFloat("Offset", &channel_1_offset);
                        channel_1_range = std::stof(channel_range[channel_1_range_current]);
                        if (channel_1_coupling_current == 0) {
                            channel_1_AC_coupling = true;
                        } else {
                            channel_1_AC_coupling = false;
                        }
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            
            ImGui::Dummy(ImVec2(0.0f, 30.0f));

            ImGui::SeparatorText("TRIGGER SETTINGS");
            ImGui::Combo("Trigger Source", &trigger_source_current, trigger_sources, IM_ARRAYSIZE(trigger_sources));
            ImGui::InputFloat("Trigger Threshold (mV)", &trigger_threshold);
            ImGui::Combo("Trigger Direction", &trigger_direction_current, trigger_directions, IM_ARRAYSIZE(trigger_directions));
            ImGui::SliderFloat("Pre-Trigger (%)", &pre_trigger, 0.0f, 100.0f, "%.0f%%", ImGuiSliderFlags_AlwaysClamp);
            ImGui::Dummy(ImVec2(0.0f, 30.0f));

            // ps_device->SetTimebase(timebase_numeric_current, timebase_unit_current);
            // int chID = 0;
            // ps_device->SetChannel(chID, channel_enabled[0], channel_0_AC_coupling, channel_0_range, channel_0_offset);
            // chID = 1;
            // ps_device->SetChannel(chID, channel_enabled[1], channel_1_AC_coupling, channel_1_range, channel_1_offset);
            // ps_device->SetTrigger(trigger_source_current, trigger_threshold, trigger_direction_current, pre_trigger);
        } // end if (isConnected)
    ImGui::End(); // CONTROL PANEL
}
