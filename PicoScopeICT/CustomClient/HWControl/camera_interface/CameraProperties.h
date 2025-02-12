#pragma once

class CameraProperties {
public:
    virtual ~CameraProperties() = default;

    virtual std::string GetSerialNumber() const = 0;

    virtual float GetGain() const = 0;
    virtual void SetGain(float gain) = 0;

    virtual int GetExposureTime() const = 0;
    virtual void SetExposureTime(int exposure_time) = 0;

    virtual std::string GetName() const = 0;
    virtual void SetName(const std::string& name) = 0;

    virtual std::string GetIPAddress() const = 0;
    virtual void SetIPAddress(const std::string& ipAddress) = 0;
};