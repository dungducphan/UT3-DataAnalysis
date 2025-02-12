#include "PCH.h"

#include "PCOCameraProperties.h"

PCOCameraProperties::PCOCameraProperties(const std::string& serial_number)
    : serial_number_(serial_number), gain_(0.0f), exposure_time_(0) {
    // Initialize camera properties using PCO SDK
}

std::string PCOCameraProperties::GetSerialNumber() const {
    return serial_number_;
}

float PCOCameraProperties::GetGain() const {
    // Get gain using PCO SDK
    return gain_;
}

void PCOCameraProperties::SetGain(float gain) {
    // Set gain using PCO SDK
    gain_ = gain;
}

int PCOCameraProperties::GetExposureTime() const {
    // Get exposure time using PCO SDK
    return exposure_time_;
}

void PCOCameraProperties::SetExposureTime(int exposure_time) {
    // Set exposure time using PCO SDK
    exposure_time_ = exposure_time;
}

std::string PCOCameraProperties::GetName() const {
    // Get camera name using PCO SDK
    return name_;
}

void PCOCameraProperties::SetName(const std::string& name) {
    // Set camera name using PCO SDK
    name_ = name;
}

std::string PCOCameraProperties::GetIPAddress() const {
    // Get IP address using PCO SDK
    return ip_address_;
}

void PCOCameraProperties::SetIPAddress(const std::string& ip_address) {
    // Set IP address using PCO SDK
    ip_address_ = ip_address;
}
