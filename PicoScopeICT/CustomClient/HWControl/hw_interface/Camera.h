#pragma once

#include <string>
#include <variant>

#include "CameraProperties.h"

/* How to use this API
 *
 * Create a new vendor-specific camera object using the CameraFactory class.
 * Call the CreateCamera method with the vendor type and serial number as arguments.
 * Use the returned camera object to stream frames and set exposure.
 *
 * Example:
 *      Camera* camera = CameraFactory::CreateCamera(Vendor::Basler, "12345");
 *      camera->Connect();
 *      camera->Set("Gain", 10);
 *      camera->Set("ExposureTime", 100);
 *      camera->Set("TriggerMode", kSoftware | kExternal);
 *      camera->Set("NumberOfFrames", -1);
 *      camera->Start();
 *      camera->Stop();
 *      auto gain = camera->Get("Gain");
 *      auto exposureTime = camera->Get("ExposureTime");
 *      auto triggerMode = camera->Get("TriggerMode");
 *      auto nFrames = camera->Get("NumberOfFrames");
 *      camera->Disconnect();
 *
 */

using CameraParameter = std::variant<int, float, std::string>;

/**
 * @enum Vendor
 * @brief Enumerates supported camera vendors for identification and management purposes.
 */
enum class Vendor {
    Basler,
    FLIR,
    PCO
    // Add other vendors here, if any
};

/**
 * @class Camera
 * @brief Abstract base class for camera devices, providing a unified interface for managing camera operations and parameters.
 */
class Camera {
    /**
     * @class Camera
     * @brief Abstract base class representing a camera. Provides a virtual destructor for proper cleanup in derived classes.
     */
public:
    virtual ~Camera() = default;

    /**
     * @brief Pure virtual method for establishing a connection.
     *
     * This method must be implemented by derived classes to handle
     * the connection logic specific to their use case.
     */
    virtual void Connect() = 0;

    /**
     * @brief Pure virtual function to disconnect the current connection.
     *
     * This method is intended to be overridden by derived classes to implement
     * specific disconnection logic for different types of connections or devices.
     */
    virtual void Disconnect() = 0;

    /**
     * @brief Pure virtual function to start the image acquisition.
     *
     * Implemented by derived classes, using vendor-specific SDK/API, to provide the specific logic for starting the image acquisition.
     */
    virtual void Start() = 0;

    /**
     * @brief Pure virtual function to start the image acquisition.
     *
     * Implemented by derived classes, using vendor-specific SDK/API, to provide the specific logic for stopping the image acquisition.
     */
    virtual void Stop() = 0;

    /**
     * @brief Retrieves the serial number of the camera.
     *
     * This method provides a unique identifier for the camera in the form of its serial number.
     * It is expected to be implemented by derived classes.
     *
     * @return The serial number of the camera as a string.
     */
    virtual std::string GetSerialNumber() const = 0;

    /**
     * @brief Abstract method for setting a camera parameter to a specific value.
     *
     * This method is designed to update a specific parameter of a camera with the
     * provided value. The exact behavior depends on the concrete implementation
     * in derived classes.
     *
     * @param parameter A string representing the name of the parameter to set (e.g., "ExposureTime").
     * @param value The new value to be assigned to the specified camera parameter.
     */
    virtual void Set(const std::string& parameter, CameraParameter value) = 0;

    /**
     * @brief Retrieves the value of a specified camera parameter.
     * @param parameter The name of the parameter to retrieve.
     * @return The value of the requested camera parameter.
     */
    virtual CameraParameter Get(const std::string& parameter) const = 0;

    /**
     * @brief Assigns the provided CameraProperties object to the internal properties' member.
     * @param properties Pointer to a CameraProperties object containing the desired properties to be set.
     */
    void SetProperties(CameraProperties* properties) { properties_ = properties; }
    /**
     * @brief Retrieves the camera properties.
     * @return A pointer to the CameraProperties object associated with the camera.
     */
    CameraProperties* GetProperties() const { return properties_; }

    /**
     * @brief Pointer to a CameraProperties object that holds camera-specific details such as serial number and vendor information.
     */
protected:
    CameraProperties* properties_;
    /**
     * @class Camera
     * @brief Represents a Camera object with associated properties.
     *
     * @return Does not return a value as this is a constructor.
     */
    Camera() : properties_(nullptr) {}
};


/**
 * @class CameraFactory
 * @brief Abstract factory class for creating Camera objects based on the vendor type and serial number.
 */
class CameraFactory {
    /**
     * @brief Virtual destructor for the CameraFactory class.
     *
     * Cleans up resources used by the CameraFactory instance. Declared
     * as virtual to ensure derived class destructors are called
     * correctly during object destruction.
     */
public:
    virtual ~CameraFactory() = default;

    /**
     * Creates a camera instance for the specified vendor with the given serial number.
     *
     * This method acts as a factory for creating cameras supported by different
     * vendor-specific factories. The created camera object is specific to the vendor
     * and should be handled accordingly.
     *
     * @param vendor An enumeration value specifying the vendor type (e.g., Basler, FLIR, PCO).
     * @param serial_number A string containing the serial number of the camera to create.
     * @return A pointer to the created Camera object.
     * @throws std::invalid_argument if an invalid vendor is provided.
     */
    virtual Camera* CreateCamera(Vendor vendor, const std::string& serial_number);
};
