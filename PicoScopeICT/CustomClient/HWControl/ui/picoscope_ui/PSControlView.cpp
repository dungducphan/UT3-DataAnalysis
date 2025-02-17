#include "PCH.h"

#include "imgui.h"
#include "implot.h"
#include "ImGuiFileDialog.h"

#include "PSControlView.h"

PSControlView::PSControlView(PSDataProcessor* psDataProp, PSDevice* pdDev) : ps_data_processor(psDataProp), ps_device(pdDev) {}

void PSControlView::Render() {
    ImGui::Begin("WAVEFORM VIEWER");
        if (ImPlot::BeginPlot("PicoScope Waveform Viewer")) {
            // Set multiple y-axes
            ImPlot::SetupAxes("Time (ns)","Va (mV)",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxis(ImAxis_Y2, "Vb (mV)", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_Opposite);

            // Plot data channel A
            ImPlot::SetAxes(ImAxis_X1, ImAxis_Y1);
            ImPlot::PlotLine("A", ps_device->currentTimeArray, ps_device->currentWaveformChannelA, BUFFER_SIZE);

            // Plot data channel B
            ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);

            ImPlot::PlotLine("B", ps_device->currentTimeArray, ps_device->currentWaveformChannelB, BUFFER_SIZE);

            // End plot
            ImPlot::EndPlot();
        }
    ImGui::End();

    ImGui::Begin("CONTROL PANEL");
        ImGui::SeparatorText("CONNECTION STATUS");

        // This is not STATIC so that every frame update, this is reset to false
        bool isScopeStateChanged = false;

        // Scope connection status
        static bool isConnected = false;

        // Timebase settings
        static int timebase_numeric_current = 0;
        static int timebase_unit_current = 0;

        // Channel settings
        static bool channel_0_AC_coupling = false;
        static bool channel_1_AC_coupling = false;
        static bool channel_enabled[2] = {true, true};
        static int  channel_0_range_current = 8;
        static int  channel_0_coupling_current = 1;
        static int  channel_1_range_current = 1;
        static int  channel_1_coupling_current = 1;

        // Trigger settings
        static int   trigger_source_current = 2;
        static float trigger_threshold = 1500;
        static int   trigger_direction_current = 0;
        static int   triggerDelay = 0;

        if (ImGui::Button(isConnected ? "DISCONNECT" : "CONNECT", ImVec2(250, 50))) {
            if (isConnected) {
                ps_device->CloseDevice();
            } else {
                ps_device->OpenDevice();
                ps_device->SetTimebase(timebase_numeric_current, timebase_unit_current);
                int chID = 0;
                ps_device->SetChannel(chID, channel_enabled[0], channel_0_AC_coupling, channel_0_range_current);
                chID = 1;
                ps_device->SetChannel(chID, channel_enabled[1], channel_1_AC_coupling, channel_0_range_current);
                ps_device->SetTrigger(trigger_source_current, trigger_threshold, trigger_direction_current, triggerDelay);
            }
            isConnected = ps_device->IsDeviceOpen();
        }
        ImGui::Dummy(ImVec2(0.0f, 30.0f));

        if (isConnected) {
            //////////////////////////////////////////////////////////////////////////
            ImGui::SeparatorText("TIMEBASE SETTINGS");
            if (ImGui::Combo("Timebase Numerical Value", &timebase_numeric_current, timebase_numeric, IM_ARRAYSIZE(timebase_numeric))) isScopeStateChanged = true;
            if (ImGui::Combo("Timebase Unit", &timebase_unit_current, timebase_unit, IM_ARRAYSIZE(timebase_unit))) isScopeStateChanged = true;
            ImGui::Text("Timebase: %s %s / div", timebase_numeric[timebase_numeric_current], timebase_unit[timebase_unit_current]);
            ImGui::Dummy(ImVec2(0.0f, 30.0f));

            //////////////////////////////////////////////////////////////////////////
            ImGui::SeparatorText("CHANNEL SETTINGS");
            if (ImGui::BeginTabBar("Channels")) {
                if (ImGui::BeginTabItem("Channel A")) {
                    ImGui::Text("Note: Connected to PS300 Power Supply.");
                    ImGui::Dummy(ImVec2(0.0f, 30.0f));
                    ImGui::Dummy(ImVec2(0.0f, 30.0f));
                    if (ImGui::Checkbox("Enable Channel A", &channel_enabled[0])) isScopeStateChanged = true;
                    if (channel_enabled[0]) {
                        if (ImGui::Combo("Channel A Coupling", &channel_0_coupling_current, channel_coupling, IM_ARRAYSIZE(channel_coupling))) isScopeStateChanged = true;
                        if (ImGui::Combo("Channel A Vertical Range", &channel_0_range_current, channel_range, IM_ARRAYSIZE(channel_range))) isScopeStateChanged = true;
                        if (channel_0_coupling_current == 0) {
                            channel_0_AC_coupling = true;
                        } else {
                            channel_0_AC_coupling = false;
                        }
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Channel B")) {
                    ImGui::Text("Note: Connected to ICT. Tee-off with 50 Ohm terminator.");
                    ImGui::Dummy(ImVec2(0.0f, 30.0f));
                    ImGui::Dummy(ImVec2(0.0f, 30.0f));
                    if (ImGui::Checkbox("Enable Channel B", &channel_enabled[1])) isScopeStateChanged = true;
                    if (channel_enabled[1]) {
                        if (ImGui::Combo("Channel B Coupling", &channel_1_coupling_current, channel_coupling, IM_ARRAYSIZE(channel_coupling))) isScopeStateChanged = true;
                        if (ImGui::Combo("Channel B Vertical Range", &channel_1_range_current, channel_range, IM_ARRAYSIZE(channel_range))) isScopeStateChanged = true;
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

            //////////////////////////////////////////////////////////////////////////
            ImGui::SeparatorText("TRIGGER SETTINGS");
            if (ImGui::Combo("Trigger Source", &trigger_source_current, trigger_sources, IM_ARRAYSIZE(trigger_sources))) isScopeStateChanged = true;
            if (ImGui::InputFloat("Trigger Threshold (mV)", &trigger_threshold)) isScopeStateChanged = true;
            if (ImGui::Combo("Trigger Direction", &trigger_direction_current, trigger_directions, IM_ARRAYSIZE(trigger_directions))) isScopeStateChanged = true;
            if (ImGui::SliderInt("Trigger Delay (div)", &triggerDelay, 0, 10, "%d%%", ImGuiSliderFlags_AlwaysClamp)) isScopeStateChanged = true;
            ImGui::Dummy(ImVec2(0.0f, 30.0f));

            //////////////////////////////////////////////////////////////////////////
            if (isScopeStateChanged) {
                ps_device->SetTimebase(timebase_numeric_current, timebase_unit_current);
                int chID = 0;
                ps_device->SetChannel(chID, channel_enabled[0], channel_0_AC_coupling, channel_0_range_current);
                chID = 1;
                ps_device->SetChannel(chID, channel_enabled[1], channel_1_AC_coupling, channel_0_range_current);
                ps_device->SetTrigger(trigger_source_current, trigger_threshold, trigger_direction_current, triggerDelay);
            }
        } // end if (isConnected)
    ImGui::End(); // CONTROL PANEL
}
