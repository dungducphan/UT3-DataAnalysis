#include "PCH.h"

#include "imgui.h"
#include "implot.h"
#include "ImGuiFileDialog.h"

#include "PSControlView.h"

PSControlView::PSControlView(PSDataProcessor* psDataProp, PSDevice* pdDev) :
ps_data_processor(psDataProp),
ps_device(pdDev),
selectedDataset(-1) {}

void PSControlView::Render() {
    RenderDatasetView();
    RenderHistogramView();
    RenderErrorBarPlotView();
    RenderWaveformViewer();
    RenderControlPanel();
}

void PSControlView::RenderDatasetView() {
    ImGui::Begin("Dataset Configuration");

    static int scanNumber = -1;

    ImGui::SeparatorText("ADD SAVED DATASET");

    static std::string selectedFolderPath;
    static char path[128] = "";

    if (ImGui::Button("Select Directory")) {
        IGFD::FileDialogConfig config;
        config.path = ".";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey", "Dataset Directory", nullptr, config);
    }

    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseDirDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            selectedFolderPath = ImGuiFileDialog::Instance()->GetCurrentPath();
            // only change path when select a new path in this dialog
            strncpy(path, selectedFolderPath.c_str(), sizeof(path) - 1);
            path[sizeof(path) - 1] = '\0'; // Ensure null-termination
        }
        ImGuiFileDialog::Instance()->Close();
    }

    ImGui::InputText("Selected Path", path, IM_ARRAYSIZE(path), ImGuiInputTextFlags_ElideLeft);
    if (ImGui::Button("ADD")) {
        ps_data_processor->AddDataset(path);
    }

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::SeparatorText("LIVE DATASET");

    static std::string selectedSaveBasePath;
    static char save_path[128] = "";

    if (ImGui::Button("Select Base Directory to Save")) {
        IGFD::FileDialogConfig config;
        config.path = ".";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey_2", "Save Directory", nullptr, config);
    }

    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseDirDlgKey_2")) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            selectedSaveBasePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            // only change path when select a new path in this dialog
            strncpy(save_path, selectedSaveBasePath.c_str(), sizeof(save_path) - 1);
            path[sizeof(save_path) - 1] = '\0'; // Ensure null-termination
        }
        ImGuiFileDialog::Instance()->Close();
    }

    ImGui::InputText("Selected Base Path", save_path, IM_ARRAYSIZE(save_path), ImGuiInputTextFlags_ElideLeft);

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    static int numberOfWaveforms = 10;
    static int numberOfWaveformsCollectedInScan = 0;
    if (!ps_device->IsDeviceOpen()) {
        ImGui::Text("Device is not connected.");
    }
    if (ImGui::InputInt("Number of Waveforms", &numberOfWaveforms)) {
        if (numberOfWaveforms < 1) numberOfWaveforms = 1;
    }
    if (ImGui::Button("START ACQUISITION", ImVec2(250, 50))) {
        scanNumber++;

        std::string baseSavePath = selectedSaveBasePath + "\\Scan_" + std::to_string(scanNumber);
        try {
            std::filesystem::create_directory(baseSavePath);
            std::filesystem::create_directory(baseSavePath + "\\ChannelA");
            std::filesystem::create_directory(baseSavePath + "\\ChannelB");
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
        }

        ps_data_processor->AddLiveDataset(scanNumber);
        numberOfWaveformsCollectedInScan = 0;
        if (ps_device->IsDeviceOpen()) {
            acquisitionThread = std::thread([this, baseSavePath]() {
                for (int i = 0; i < numberOfWaveforms; i++) {
                    ps_device->CollectOneWaveform();
                    currentWaveform = PSDataProcessor::ReadSingleWaveformLive(ps_device->currentTimeArray, ps_device->currentWaveformChannelB, BUFFER_SIZE);
                    ps_data_processor->AddWaveformToLiveDataset(scanNumber, currentWaveform, numberOfWaveforms);
                    PSDataProcessor::SaveWaveformToCSV(currentWaveform, baseSavePath + "\\ChannelB\\wf_" + std::to_string(i) + ".csv");
                    auto ps300Waveform = PSDataProcessor::ReadSingleWaveformLive(ps_device->currentTimeArray, ps_device->currentWaveformChannelA, BUFFER_SIZE);
                    PSDataProcessor::SaveWaveformToCSV(ps300Waveform, baseSavePath + "\\ChannelA\\wf_" + std::to_string(i) + ".csv");
                    numberOfWaveformsCollectedInScan++;
                }
            });
            acquisitionThread.detach();
        }
    }

    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    if (scanNumber >= 0) {
        ImGui::Text("Scan %i", scanNumber);
        ImGui::Text("Acquired %i waveforms.", numberOfWaveformsCollectedInScan);
    }
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::SeparatorText("INTEGRATION CURSORS");
    ImGui::Text("Cursor Left: %.2f ns", ps_data_processor->cursorLeft);
    ImGui::Text("Cursor Right: %.2f ns", ps_data_processor->cursorRight);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::SeparatorText("LIST OF DATASETS");
    if (ImGui::TreeNode("root")) {
        for (int i = 0; i < ps_data_processor->GetDatasetSize(); i++) {
            // Use SetNextItemOpen() so set the default state of a node to be open. We could
            // also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
            if (i == 0) ImGui::SetNextItemOpen(true, ImGuiCond_Once);

            // Here we use PushID() to generate a unique base ID, and then the "" used as TreeNode id won't conflict.
            // An alternative to using 'PushID() + TreeNode("", ...)' to generate a unique ID is to use 'TreeNode((void*)(intptr_t)i, ...)',
            // aka generate a dummy pointer-sized value to be hashed. The demo below uses that technique. Both are fine.
            ImGui::PushID(i);
            ImGui::Separator();
            if (ImGui::TreeNode("", "Scan %d", i)) {
                ImGui::Separator();
                ImGui::Text("Number of Waveforms: %i", ps_data_processor->GetScanSize(i));
                ImGui::Text("Mean Charge: %.2f pC", ps_data_processor->GetScanMeanCharge(i));
                ImGui::Text("Std Dev Charge: %.2f pC", ps_data_processor->GetScanStdDevCharge(i));
                ImGui::Separator();
                if (ImGui::SmallButton("Show Distribution")) {
                    // Plot Charge Distribution Histogram
                    selectedDataset = i;
                }
                ImGui::SameLine();
                if (ImGui::SmallButton("Remove")) {
                    // Remove the dataset
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
    ImGui::End();
}

void PSControlView::RenderHistogramView() const {
    ImGui::Begin("ICT Charge Distribution");

    if (selectedDataset == -1) {
        ImGui::Text("Select a dataset to show the charge distribution.");
        ImGui::End();
        return;
    }

    if (ImPlot::BeginPlot("ICT Charge Distribution")) {
        // Extract from the selected dataset
        auto dataset = ps_data_processor->GetDataset(selectedDataset);

        static const int bins = 100;
        static int count = dataset.waveforms.size();

        static double data[5000];
        for (int i = 0; i < count; ++i) {
            data[i] = dataset.waveforms[i].chargeValue;
        }
        ImPlot::SetupAxes("ICT Charge (pc)","Counts",ImPlotAxisFlags_Lock ,ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoGridLines);
        ImPlot::SetupAxisLimits(ImAxis_X1, 0, 100);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
        ImPlot::PlotHistogram(Form("Scan %d", selectedDataset), data, count, bins);
        ImPlot::EndPlot();
    }

    ImGui::End();
}

void PSControlView::RenderErrorBarPlotView() const {
    ImGui::Begin("ICT Charge versus Scan Number");
    double scan_evolution_scanID[1000] = {0};
    double scan_evolution_meanCharge[1000] = {0};
    double scan_evolution_stdDevCharge[1000] = {0};
    if (ImPlot::BeginPlot("Scan Evolution of ICT Charge")) {
        // Extract from all scans (all dataset)
        ps_data_processor->CreateDataForErrorBarPlot(scan_evolution_scanID, scan_evolution_meanCharge, scan_evolution_stdDevCharge);
        ImPlot::SetupAxes("Scan No.","ICT Charge (pC)",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_Lock);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 150);
        ImPlot::SetNextErrorBarStyle(ImPlot::GetColormapColor(2), 0);
        ImPlot::SetNextLineStyle(ImPlot::GetColormapColor(2), 0);
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
        ImPlot::PlotErrorBars("ICT Charge vs. Scan No.", scan_evolution_scanID, scan_evolution_meanCharge, scan_evolution_stdDevCharge, ps_data_processor->GetDatasetSize());
        ImPlot::PlotLine("ICT Charge vs. Scan No.", scan_evolution_scanID, scan_evolution_meanCharge, ps_data_processor->GetDatasetSize());
        ImPlot::EndPlot();
    }
    ImGui::End();
}


void PSControlView::RenderWaveformViewer() {
    static double cursorLeft = 0.0f;
    static double cursorRight = 2048.0f;
    ImGui::Begin("WAVEFORM VIEWER");
        if (ImPlot::BeginPlot("PicoScope Waveform Viewer")) {
            // Set multiple y-axes
            ImPlot::SetupAxes("Time (ns)","Va (mV)",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit |
            ImPlotAxisFlags_NoGridLines);
            ImPlot::SetupAxis(ImAxis_Y2, "Vb (mV)", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_Opposite | ImPlotAxisFlags_NoGridLines);

            // Plot data channel A
            ImPlot::SetAxes(ImAxis_X1, ImAxis_Y1);
            ImPlot::PlotLine("A", ps_device->currentTimeArray, ps_device->currentWaveformChannelA, BUFFER_SIZE);

            // Plot data channel B
            ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);

            ImPlot::PlotLine("B", ps_device->currentTimeArray, ps_device->currentWaveformChannelB, BUFFER_SIZE);

            // FIXME: Dragline to select a region
            if (ImPlot::DragLineX(0, &cursorLeft, ImVec4(1,1,1,1), 1, ImPlotDragToolFlags_None)) {
                ps_data_processor->cursorLeft = cursorLeft;
            }
            if (ImPlot::DragLineX(1, &cursorRight, ImVec4(1,1,1,1), 1, ImPlotDragToolFlags_None)) {
                ps_data_processor->cursorRight = cursorRight;
            }
            // End plot
            ImPlot::EndPlot();
        }
    ImGui::End();
}

void PSControlView::RenderControlPanel() {
     ImGui::Begin("CONTROL PANEL");
        ImGui::SeparatorText("CONNECTION STATUS");

        // This is not STATIC so that every frame update, this is reset to false
        bool isScopeStateChanged = false;

        // Scope connection status
        static bool isConnected = false;

        // Timebase settings
        static int32_t gui_timebase = 1;
        static int32_t samplingDurationInNanoseconds = 0;
        static int32_t samplingIntervalInNanoseconds = 0;

        // Channel settings
        static bool channel_0_AC_coupling = false;
        static bool channel_1_AC_coupling = false;
        static bool channel_enabled[2] = {true, true};
        static int  channel_0_range_current = 8;
        static int  channel_0_coupling_current = 1;
        static int  channel_1_range_current = 1;
        static int  channel_1_coupling_current = 1;

        // Trigger settings
        static int   trigger_wait_in_milliseconds = 20000;
        static int   trigger_source_current = 2;
        static int   trigger_threshold = 1500;
        static int   trigger_direction_current = 0;
        static int   triggerDelay = 0;

        if (ImGui::Button(isConnected ? "DISCONNECT" : "CONNECT", ImVec2(250, 50))) {
            if (isConnected) {
                ps_device->CloseDevice();
            } else {
                ps_device->OpenDevice();
                ps_device->SetTimebase(gui_timebase);
                int chID = 0;
                ps_device->SetChannel(chID, channel_enabled[0], channel_0_AC_coupling, channel_0_range_current);
                chID = 1;
                ps_device->SetChannel(chID, channel_enabled[1], channel_1_AC_coupling, channel_1_range_current);
                ps_device->SetSimpleTrigger(trigger_source_current, trigger_threshold, trigger_direction_current, triggerDelay, trigger_wait_in_milliseconds);
            }
            isConnected = ps_device->IsDeviceOpen();
        }
        ImGui::Dummy(ImVec2(0.0f, 30.0f));

        if (isConnected) {
            //////////////////////////////////////////////////////////////////////////
            ImGui::SeparatorText("TIMEBASE SETTINGS");
            if (ImGui::InputInt("Timebase", &gui_timebase)) isScopeStateChanged = true;
            ps_device->GetTimebaseInfo(gui_timebase, samplingDurationInNanoseconds, samplingIntervalInNanoseconds);
            ImGui::Text("Sampling Interval: %d ns", samplingIntervalInNanoseconds);
            ImGui::Text("Sampling Duration: %d ns", samplingDurationInNanoseconds);
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
            if (ImGui::InputInt("Trigger Threshold (mV)", &trigger_threshold)) isScopeStateChanged = true;
            if (ImGui::Combo("Trigger Direction", &trigger_direction_current, trigger_directions, IM_ARRAYSIZE(trigger_directions))) isScopeStateChanged = true;
            if (ImGui::SliderInt("Trigger Delay (SP)", &triggerDelay, 0, 1000, "%d sampling periods", ImGuiSliderFlags_AlwaysClamp)
            ) isScopeStateChanged = true;
            if (ImGui::InputInt("Trigger Wait (ms)", &trigger_wait_in_milliseconds)) isScopeStateChanged = true;
            ImGui::Dummy(ImVec2(0.0f, 30.0f));

            //////////////////////////////////////////////////////////////////////////
            if (isScopeStateChanged) {
                ps_device->SetTimebase(gui_timebase);
                int chID = 0;
                ps_device->SetChannel(chID, channel_enabled[0], channel_0_AC_coupling, channel_0_range_current);
                chID = 1;
                ps_device->SetChannel(chID, channel_enabled[1], channel_1_AC_coupling, channel_1_range_current);
                ps_device->SetSimpleTrigger(trigger_source_current, trigger_threshold, trigger_direction_current, triggerDelay, trigger_wait_in_milliseconds);
            }
        } // end if (isConnected)
    ImGui::End(); // CONTROL PANEL
}
