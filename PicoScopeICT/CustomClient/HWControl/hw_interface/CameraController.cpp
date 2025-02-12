#include "PCH.h"

#include "CameraController.h"
#include "Camera.h"

CameraController::CameraController(CameraManager& cameraManager)
    : cameraManager_(cameraManager) {}

void CameraController::AddCamera(const std::string& vendor, const std::string& serialNumber, const std::string& humanReadableName) const {
    Vendor vendorEnum;
    if (vendor == "Basler") {
        vendorEnum = Vendor::Basler;
    } else if (vendor == "FLIR") {
        vendorEnum = Vendor::FLIR;
    } else if (vendor == "PCO") {
        vendorEnum = Vendor::PCO;
    } else {
        throw std::invalid_argument("Unknown vendor");
    }

    auto camera = std::unique_ptr<Camera>(CameraFactory().CreateCamera(vendorEnum, serialNumber));
    camera->Set("Name", humanReadableName);
    cameraManager_.AddCamera(std::move(camera));
}

void CameraController::RemoveCamera(const std::string& serialNumber) const {
    cameraManager_.RemoveCamera(serialNumber);
}
