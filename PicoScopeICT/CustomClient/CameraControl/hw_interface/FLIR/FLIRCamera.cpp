#include "PCH.h"

#include "FLIRCamera.h"
#include "FLIRCameraProperties.h"

Camera* FLIRCameraFactory::CreateCamera(const std::string& serial_number) {
    return new FLIRCamera(serial_number);
}

FLIRCamera::FLIRCamera(const std::string& serial_number) {
    properties_ = new FLIRCameraProperties(serial_number);
}

void FLIRCamera::Connect() {
    // Connect to the camera using FLIR SDK
}

void FLIRCamera::Disconnect() {
    // Disconnect from the camera using FLIR SDK
}

void FLIRCamera::Start() {
    // Start the camera using FLIR SDK
}

void FLIRCamera::Stop() {
    // Stop the camera using FLIR SDK
}

std::string FLIRCamera::GetSerialNumber() const {
    return properties_->GetSerialNumber();
}

void FLIRCamera::Set(const std::string& parameter, CameraParameter value) {
    // Set the parameter using FLIR SDK
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

CameraParameter FLIRCamera::Get(const std::string& parameter) const {
    // Get the parameter using FLIR SDK
    if (parameter == "Gain") {
        return properties_->GetGain();
    } else if (parameter == "ExposureTime") {
        return properties_->GetExposureTime();
    } else if (parameter == "Name") {
        return properties_->GetName();
    } else if (parameter == "Vendor") {
        return "FLIR";
    } else if (parameter == "IPAddress") {
        return properties_->GetIPAddress();
    } else if (parameter == "SerialNumber") {
        return properties_->GetSerialNumber();
    }
    // Handle other parameters as needed
    return parameters_.at(parameter);
}