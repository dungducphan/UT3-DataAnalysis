#include "PCH.h"

#include "FLIRCameraProperties.h"

FLIRCameraProperties::FLIRCameraProperties(const std::string& serial_number)
    : serial_number_(serial_number), gain_(0.0f), exposure_time_(0) {
    // Initialize camera properties using FLIR SDK
}

std::string FLIRCameraProperties::GetSerialNumber() const {
    return serial_number_;
}

float FLIRCameraProperties::GetGain() const {
    // Get gain using FLIR SDK
    return gain_;
}

void FLIRCameraProperties::SetGain(float gain) {
    // Set gain using FLIR SDK
    gain_ = gain;
}

int FLIRCameraProperties::GetExposureTime() const {
    // Get exposure time using FLIR SDK
    return exposure_time_;
}

void FLIRCameraProperties::SetExposureTime(int exposure_time) {
    // Set exposure time using FLIR SDK
    exposure_time_ = exposure_time;
}

std::string FLIRCameraProperties::GetName() const {
    // Get camera name using FLIR SDK
    return name_;
}

void FLIRCameraProperties::SetName(const std::string& name) {
    // Set camera name using FLIR SDK
    name_ = name;
}

std::string FLIRCameraProperties::GetIPAddress() const {
    // Get IP address using FLIR SDK
    return ip_address_;
}

void FLIRCameraProperties::SetIPAddress(const std::string& ip_address) {
    // Set IP address using FLIR SDK
    ip_address_ = ip_address;
}
