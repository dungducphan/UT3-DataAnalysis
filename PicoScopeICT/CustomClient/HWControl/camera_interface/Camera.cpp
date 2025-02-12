#include "../PCH.h"

#include "Basler/BaslerCamera.h"
#include "FLIR/FLIRCamera.h"
#include "PCO/PCOCamera.h"

/**
 * Creates a Camera instance based on the specified vendor and serial number.
 *
 * @param vendor The vendor type of the camera. Must be one of the defined Vendor enum values.
 * @param serial_number The unique serial number of the camera to be created.
 * @return A pointer to the created Camera instance associated with the specified vendor and serial number.
 * @throws std::invalid_argument If the vendor type is invalid or unsupported.
 */
Camera* CameraFactory::CreateCamera(const Vendor vendor, const std::string& serial_number) {
    switch (vendor) {
        case Vendor::Basler:
            return (new BaslerCameraFactory())->CreateCamera(serial_number);
        case Vendor::FLIR:
            return (new FLIRCameraFactory())->CreateCamera(serial_number);
        case Vendor::PCO:
            return (new PCOCameraFactory())->CreateCamera(serial_number);
        default: // Other vendors
            throw std::invalid_argument("Invalid vendor");
    }
}


