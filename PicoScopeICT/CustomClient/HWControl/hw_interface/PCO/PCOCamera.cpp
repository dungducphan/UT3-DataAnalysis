#include "PCH.h"

#include "PCOCamera.h"
#include "PCOCameraProperties.h"

Camera* PCOCameraFactory::CreateCamera(const std::string& serial_number) {
    return new PCOCamera(serial_number);
}

PCOCamera::PCOCamera(const std::string& serial_number) {
    properties_ = new PCOCameraProperties(serial_number);
}

void PCOCamera::Connect() {
    // Connect to the camera using PCO SDK
}

void PCOCamera::Disconnect() {
    // Disconnect from the camera using PCO SDK
}

void PCOCamera::Start() {
    // Start the camera using PCO SDK
}

void PCOCamera::Stop() {
    // Stop the camera using PCO SDK
}

std::string PCOCamera::GetSerialNumber() const {
    return properties_->GetSerialNumber();
}

void PCOCamera::Set(const std::string& parameter, CameraParameter value) {
    // Set the parameter using PCO SDK
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
    } else {
        std::cerr << "Parameter " << parameter << " not supported." << std::endl;
    }
    // Handle other parameters as needed
}

CameraParameter PCOCamera::Get(const std::string& parameter) const {
    // Get the parameter using PCO SDK
    if (parameter == "Gain") {
        return properties_->GetGain();
    } else if (parameter == "ExposureTime") {
        return properties_->GetExposureTime();
    } else if (parameter == "Name") {
        return properties_->GetName();
    } else if (parameter == "Vendor") {
        return "PCO";
    } else if (parameter == "IPAddress") {
        return properties_->GetIPAddress();
    } else if (parameter == "SerialNumber") {
        return properties_->GetSerialNumber();
    }
    // Handle other parameters as needed
    return parameters_.at(parameter);
}