#include "PCH.h"

#include "imgui.h"
#include "implot.h"
#include "ImGuiFileDialog.h"

#include "PSControlView.h"

PSControlView::PSControlView(PSDevice* pdDev)
    : ps_device(pdDev) {}

void PSControlView::Render() {
    ImGui::Begin("CHANNEL SETTINGS");
    RenderChannelSettings();
    ImGui::End();

    ImGui::Begin("TRIGGER SETTINGS");
    RenderTriggerSettings();
    ImGui::End();

    ImGui::Begin("TIMEBASE SETTINGS");
    RenderTimebaseSettings();
    ImGui::End();

    ImGui::Begin("WAVEFORM DISPLAY");
    RenderWaveformDisplay();
    ImGui::End();
}

void PSControlView::RenderChannelSettings() {
    ImGui::Text("Channel Settings");

    if (ImGui::Button("Open Device")) {
        ps_device->OpenDevice();
    }

    if (ImGui::Button("Close Device")) {
        ps_device->CloseDevice();
    }
}

void PSControlView::RenderTriggerSettings() {
    ImGui::Text("Trigger Settings");
}

void PSControlView::RenderTimebaseSettings() {
    ImGui::Text("Timebase Settings");
}

void PSControlView::RenderWaveformDisplay() {
    ImGui::Text("Waveform Display");
}