#pragma once

#include <string>
#include <functional>

class CameraManager;

class CameraControlView {
public:
    // Constructor requires a reference to the CameraManager (Model)
    explicit CameraControlView(CameraManager& cameraManager);

    // Main method to render the GUI
    void Render();

    // Event handlers for user interactions
    void SetOnAddCameraCallback(std::function<void(const std::string&, const std::string&, const std::string&)> callback);
    void SetOnRemoveCameraCallback(std::function<void(const std::string&)> callback);

private:
    // Reference to the CameraManager (Model)
    CameraManager& cameraManager_;

    // Callbacks for user actions
    std::function<void(const std::string&, const std::string&, const std::string&)> onAddCameraCallback_;
    std::function<void(const std::string&)> onRemoveCameraCallback_;

    // Helper methods to render GUI components
    void RenderAddCamera();
    void RenderCameraList();
};