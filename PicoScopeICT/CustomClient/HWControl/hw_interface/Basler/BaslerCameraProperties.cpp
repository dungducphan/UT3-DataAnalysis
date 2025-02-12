#include "PCH.h"

#include "BaslerCameraProperties.h"

BaslerCameraProperties::BaslerCameraProperties(const std::string& serial_number)
    : serial_number_(serial_number), gain_(0.0f), exposure_time_(0) {
    // Initialize camera properties using Basler SDK
}

std::string BaslerCameraProperties::GetSerialNumber() const {
    return serial_number_;
}

float BaslerCameraProperties::GetGain() const {
    // Get gain using Basler SDK
    return gain_;
}

void BaslerCameraProperties::SetGain(float gain) {
    // Set gain using Basler SDK
    gain_ = gain;
}

int BaslerCameraProperties::GetExposureTime() const {
    // Get exposure time using Basler SDK
    return exposure_time_;
}

void BaslerCameraProperties::SetExposureTime(int exposure_time) {
    // Set exposure time using Basler SDK
    exposure_time_ = exposure_time;
}

std::string BaslerCameraProperties::GetName() const {
    // Get camera name using Basler SDK
    return name_;
}

void BaslerCameraProperties::SetName(const std::string& name) {
    // Set camera name using Basler SDK
    name_ = name;
}

std::string BaslerCameraProperties::GetIPAddress() const {
    // Get IP address using Basler SDK
    return ip_address_;
}

void BaslerCameraProperties::SetIPAddress(const std::string& ip_address) {
    // Set IP address using Basler SDK
    ip_address_ = ip_address;
}

