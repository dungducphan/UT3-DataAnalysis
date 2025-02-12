#pragma once

#include "CameraProperties.h"

class FLIRCameraProperties : public CameraProperties {
public:
    FLIRCameraProperties(const std::string& serial_number);

    std::string GetSerialNumber() const override;

    float GetGain() const override;
    void SetGain(float gain) override;

    int GetExposureTime() const override;
    void SetExposureTime(int exposure_time) override;

    std::string GetName() const override;
    void SetName(const std::string& name) override;

    std::string GetIPAddress() const override;
    void SetIPAddress(const std::string& ipAddress) override;

private:
    std::string serial_number_;
    float gain_;
    int exposure_time_;
    std::string name_;
    std::string ip_address_;
};