#include "PCH.h"

#include "CameraManager.h"

void CameraManager::AddCamera(std::unique_ptr<Camera> camera) {
    cameras_.push_back(std::move(camera));
}

void CameraManager::RemoveCamera(const std::string& serial_number) {
    cameras_.erase(std::remove_if(cameras_.begin(), cameras_.end(),
        [&serial_number](const std::unique_ptr<Camera>& camera) {
            return camera->GetSerialNumber() == serial_number;
        }), cameras_.end());
}

Camera* CameraManager::GetCamera(const std::string& serial_number) const {
    for (const auto& camera : cameras_) {
        if (camera->GetSerialNumber() == serial_number) {
            return camera.get();
        }
    }
    return nullptr;
}

const std::vector<std::unique_ptr<Camera>>& CameraManager::GetAllCameras() const {
    return cameras_;
}