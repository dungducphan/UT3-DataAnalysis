#pragma once

#include <string>

#include "CameraManager.h"

class CameraController {
private:
    CameraManager& cameraManager_;

public:
    explicit CameraController(CameraManager& cameraManager);

    void AddCamera(const std::string& vendor, const std::string& serialNumber, const std::string& humanReadableName) const;
    void RemoveCamera(const std::string& serialNumber) const;
};