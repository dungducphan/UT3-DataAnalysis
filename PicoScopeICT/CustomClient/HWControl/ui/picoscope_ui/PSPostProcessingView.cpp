#include "imgui.h" // ImGui for rendering

#include "PSPostProcessingView.h"
#include "PSDataProcessor.h"

PSPostProcessingView::PSPostProcessingView(PSDataProcessor& ps_data_processor)
    : ps_data_processor(ps_data_processor) {}

void PSPostProcessingView::Render() {
    ImGui::Begin("Dataset Configuration");
    RenderDatasetView();
    ImGui::End();

    ImGui::Begin("Charge Distribution");
    RenderHistogramView();
    ImGui::End();
}

void PSPostProcessingView::RenderDatasetView() {
    static char path[128] = "/path/to/some/folder/with/long/filename.cpp";
    ImGui::InputText("Data Path", path, IM_ARRAYSIZE(path), ImGuiInputTextFlags_ElideLeft);

    if (ImGui::Button("Add Dataset")) {

    }

    if (ImGui::TreeNode("LIST OF DATASETS")) {
        for (int i = 0; i < 5; i++) {
            // Use SetNextItemOpen() so set the default state of a node to be open. We could
            // also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
            if (i == 0)
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);

            // Here we use PushID() to generate a unique base ID, and then the "" used as TreeNode id won't conflict.
            // An alternative to using 'PushID() + TreeNode("", ...)' to generate a unique ID is to use 'TreeNode((void*)(intptr_t)i, ...)',
            // aka generate a dummy pointer-sized value to be hashed. The demo below uses that technique. Both are fine.
            ImGui::PushID(i);
            if (ImGui::TreeNode("", "Child %d", i)) {
                ImGui::Text("blah blah");
                ImGui::SameLine();
                if (ImGui::SmallButton("button")) {}
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
}

void PSPostProcessingView::RenderHistogramView() {
}

