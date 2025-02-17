#include "PCH.h"

#include "imgui.h"
#include "implot.h"
#include "ImGuiFileDialog.h"

#include "PSPostProcessingView.h"
#include "PSDataProcessor.h"

PSPostProcessingView::PSPostProcessingView(PSDataProcessor* ps_data_processor)
    : ps_data_processor(ps_data_processor), selectedDataset(-1) {}

void PSPostProcessingView::Render() {
    ImGui::Begin("Dataset Configuration");
    RenderDatasetView();
    ImGui::End();

    ImGui::Begin("ICT Charge Distribution");
    RenderHistogramView();
    ImGui::End();

    ImGui::Begin("ICT Charge versus Scan Number");
    RenderErrorBarPlotView();
    ImGui::End();
}

void PSPostProcessingView::RenderDatasetView() {
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
    if (ImGui::Button("START ACQUISITION", ImVec2(250, 50))) {
        // Start the acquisition
    }
    ImGui::SameLine();
    if (ImGui::Button("STOP ACQUISITION", ImVec2(250, 50))) {
        // Stop the acquisition
    }
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    ImGui::Text("Scan %i", 0);
    ImGui::Text("Acquired %i waveforms.", 0);

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
}

void PSPostProcessingView::RenderHistogramView() const {
    if (selectedDataset == -1) {
        ImGui::Text("Select a dataset to show the charge distribution.");
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

        ImPlot::SetupAxes("ICT Charge (pc)","Counts",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
        ImPlot::PlotHistogram(Form("Scan %d", selectedDataset), data, count, bins);
        ImPlot::EndPlot();
    }
}

void PSPostProcessingView::RenderErrorBarPlotView() const {
    double scan_evolution_scanID[1000] = {0};
    double scan_evolution_meanCharge[1000] = {0};
    double scan_evolution_stdDevCharge[1000] = {0};
    if (ImPlot::BeginPlot("Scan Evolution of ICT Charge")) {
        // Extract from all scans (all dataset)
        ps_data_processor->CreateDataForErrorBarPlot(scan_evolution_scanID, scan_evolution_meanCharge, scan_evolution_stdDevCharge);
        ImPlot::SetupAxes("Scan No.","ICT Charge (pC)",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
        ImPlot::PlotErrorBars("ICT Charge vs. Scan No.", scan_evolution_scanID, scan_evolution_meanCharge, scan_evolution_stdDevCharge, ps_data_processor->GetDatasetSize());
        ImPlot::EndPlot();
    }
}

