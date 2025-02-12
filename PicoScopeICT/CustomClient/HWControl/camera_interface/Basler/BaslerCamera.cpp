#include "PCH.h"

#include "BaslerCamera.h"
#include "BaslerCameraProperties.h"

Camera* BaslerCameraFactory::CreateCamera(const std::string& serial_number) {
    return new BaslerCamera(serial_number);
}

BaslerCamera::BaslerCamera(const std::string& serial_number) {
    properties_ = new BaslerCameraProperties(serial_number);
}

void BaslerCamera::Connect() {
    // Connect to the camera using Basler SDK
}

void BaslerCamera::Disconnect() {
    // Disconnect from the camera using Basler SDK
}

void BaslerCamera::Start() {
    // Start the camera using Basler SDK
}

void BaslerCamera::Stop() {
    // Stop the camera using Basler SDK
}

std::string BaslerCamera::GetSerialNumber() const {
    return properties_->GetSerialNumber();
}

void BaslerCamera::Set(const std::string& parameter, CameraParameter value) {
    // Set the parameter using Basler SDK
    parameters_[parameter] = value;
    if (parameter == "Gain") {
        properties_->SetGain(std::get<float>(value));
    } else if (parameter == "ExposureTime") {
        properties_->SetExposureTime(std::get<int>(value));
    } else if (parameter == "Name") {
        properties_->SetName(std::get<std::string>(value));
    } else if (parameter == "IPAddress") {
        properties_->SetIPAddress(std::get<std::string>(value));
    } else if (parameter == "SerialNumber") {
        std::cerr << "Cannot set serial number after initialization." << std::endl;
    }
    // Handle other parameters as needed
    std::cerr << "Parameter " << parameter << " not supported." << std::endl;
}

CameraParameter BaslerCamera::Get(const std::string& parameter) const {
    // Get the parameter using Basler SDK
    if (parameter == "Gain") {
        return properties_->GetGain();
    } else if (parameter == "ExposureTime") {
        return properties_->GetExposureTime();
    } else if (parameter == "Name") {
        return properties_->GetName();
    } else if (parameter == "Vendor") {
        return "Basler";
    } else if (parameter == "IPAddress") {
        return properties_->GetIPAddress();
    } else if (parameter == "SerialNumber") {
        return properties_->GetSerialNumber();
    }
    // Handle other parameters as needed
    return parameters_.at(parameter);
}