#pragma once

#include <vector>
#include <memory>

#include "Camera.h"

class CameraManager {
public:
    void AddCamera(std::unique_ptr<Camera> camera);
    void RemoveCamera(const std::string& serial_number);
    Camera* GetCamera(const std::string& serial_number) const;
    const std::vector<std::unique_ptr<Camera>>& GetAllCameras() const;

private:
    std::vector<std::unique_ptr<Camera>> cameras_;
};