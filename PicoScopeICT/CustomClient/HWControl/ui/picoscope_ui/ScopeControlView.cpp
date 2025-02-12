#include "ScopeControlView.h"
#include "ScopeManager.h"
#include "imgui.h" // ImGui for rendering

ScopeControlView::ScopeControlView(ScopeManager& cameraManager)
    : cameraManager_(cameraManager) {}

void ScopeControlView::Render() {
    // Render camera list and actions in the GUI
    ImGui::Begin("Scope Control View");

    // Render the "Add Scope" section
    RenderAddScope();

    // Render the list of cameras
    RenderScopeList();

    ImGui::End();
}

void ScopeControlView::RenderScopeList() {
    ImGui::SeparatorText("LIST OF CAMERAS");

    const auto& cameras = cameraManager_.GetAllScopes();
    // If there are no cameras, display a message
    if (cameras.empty()) {
        ImGui::Text("No cameras connected.");
        return;
    }

    // Create the table for displaying camera information
    if (ImGui::BeginTable("ScopeListTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        // Define table headers
        ImGui::TableSetupColumn("Scope Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Vendor", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Serial Number", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("IP Address", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableHeadersRow();

        // Render a row for each camera
        for (const auto& camera : cameras) {
            // Retrieve camera properties safely from ScopeParameter (std::variant)
            const auto getStringValue = [](const ScopeParameter& param) -> std::string {
                if (std::holds_alternative<std::string>(param)) {
                    return std::get<std::string>(param); // Return the string if it exists.
                }
                return ""; // Default to an empty string if the value isn't a string.
            };

            const std::string cameraName = getStringValue(camera->Get("Name"));
            const std::string vendor = getStringValue(camera->Get("Vendor"));
            const std::string serialNumber = getStringValue(camera->Get("SerialNumber"));
            const std::string ipAddress = getStringValue(camera->Get("IPAddress"));

            ImGui::TableNextRow();

            // Scope Name Column
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", cameraName.c_str());

            // Vendor Column
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", vendor.c_str());

            // Serial Number Column
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", serialNumber.c_str());

            // IP Address Column
            ImGui::TableSetColumnIndex(3);
            if (!ipAddress.empty()) {
                ImGui::Text("%s", ipAddress.c_str());
            } else {
                ImGui::Text("N/A");
            }

            // Actions Column
            ImGui::TableSetColumnIndex(4);

            // Add "Remove" button
            if (ImGui::Button(("Remove##" + serialNumber).c_str())) {
                if (onRemoveScopeCallback_) {
                    onRemoveScopeCallback_(serialNumber);
                }
            }

            // Add a space between buttons
            ImGui::SameLine();

            // Add "Edit" button
            if (ImGui::Button(("Edit##" + serialNumber).c_str())) {
                // Here you can define what happens when "Edit" is clicked
                // e.g., open a popup for changing camera properties
                ImGui::OpenPopup(("EditPopup##" + serialNumber).c_str());
            }

            // Handle Edit popup
            if (ImGui::BeginPopup(("EditPopup##" + serialNumber).c_str())) {
                int newExposureValue = 0;

                ImGui::Text("Edit Scope Properties");
                ImGui::InputInt("Exposure", &newExposureValue);

                if (ImGui::Button("Save")) {
                    // Perform the "edit" operation (define this logic later)
                    // Example: apply to camera settings

                    // Example: Updating camera exposure
                    Scope* cam = cameraManager_.GetScope(serialNumber);
                    if (cam) {
                        cam->Set("ExposureTime", newExposureValue);
                    }
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine();

                if (ImGui::Button("Cancel")) {
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }

        ImGui::EndTable();
    }

}

void ScopeControlView::RenderAddScope() {
    ImGui::SeparatorText("ADD CAMERA");

    static char camera_name[128] = "";
    ImGui::InputText("Scope Name", camera_name, sizeof(camera_name));

    static int selected_vendor = 0;
    const char* vendors[] = { "Basler", "FLIR", "PCO" };
    ImGui::Combo("Vendor", &selected_vendor, vendors, IM_ARRAYSIZE(vendors));

    static char serialInput[128] = "";
    ImGui::InputText("Serial Number", serialInput, sizeof(serialInput));

    static bool is_gige = false;
    ImGui::Checkbox("Gig-E", &is_gige);

    static char ip_address[128] = "";
    if (is_gige) {
        ImGui::Text("Leave IP Address blank for DHCP");
        ImGui::InputText("IP Address", ip_address, IM_ARRAYSIZE(ip_address));
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Add Scope")) {
        if (onAddScopeCallback_ && strlen(vendors[selected_vendor]) > 0 && strlen(serialInput) > 0) {
            onAddScopeCallback_(vendors[selected_vendor], serialInput, camera_name);
        }
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
}

void ScopeControlView::SetOnAddScopeCallback(std::function<void(const std::string&, const std::string&, const std::string&)> callback) {
    onAddScopeCallback_ = callback;
}

void ScopeControlView::SetOnRemoveScopeCallback(std::function<void(const std::string&)> callback) {
    onRemoveScopeCallback_ = callback;
}