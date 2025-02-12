#include "imgui.h"
#include "ImGuiFileDialog.h"

#include "PSPostProcessingView.h"
#include "PSDataProcessor.h"

PSPostProcessingView::PSPostProcessingView(PSDataProcessor* ps_data_processor)
    : ps_data_processor(ps_data_processor) {}

void PSPostProcessingView::Render() {
    ImGui::Begin("Dataset Configuration");
    RenderDatasetView();
    ImGui::End();

    ImGui::Begin("Charge Distribution");
    RenderHistogramView();
    ImGui::End();
}

void PSPostProcessingView::RenderDatasetView() const {
    ImGui::SeparatorText("ADD DATASET");

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
                ImGui::Text("Mean Charge: %.2f", ps_data_processor->GetScanMeanCharge(i));
                ImGui::Text("Std Dev Charge: %.2f", ps_data_processor->GetScanStdDevCharge(i));
                ImGui::Separator();
                if (ImGui::SmallButton("Show Distribution")) {
                    // Plot Charge Distribution Histogram
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

void PSPostProcessingView::RenderHistogramView() {

}

