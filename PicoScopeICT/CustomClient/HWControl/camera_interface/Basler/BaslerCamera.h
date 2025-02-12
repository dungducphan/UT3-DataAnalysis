#pragma once

#include <unordered_map>

#include "Camera.h"

class BaslerCameraFactory : public CameraFactory {
public:
    Camera* CreateCamera(const std::string& serial_number);
};

class BaslerCamera : public Camera {
public:
    explicit BaslerCamera(const std::string& serial_number); // Initialize camera using Basler SDK

    void Connect() override;
    void Disconnect() override;
    void Start() override;
    void Stop() override;

    std::string GetSerialNumber() const override; // Get serial number using Basler SDK

    void Set(const std::string& parameter, CameraParameter value) override;
    CameraParameter Get(const std::string& parameter) const override;

private:
    std::unordered_map<std::string, CameraParameter> parameters_;
};
