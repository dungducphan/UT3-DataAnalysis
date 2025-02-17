#include "PCH.h"

#include "PSDevice.h"


PSDevice::PSDevice() : test_device_open(false) {
}

PSDevice::~PSDevice() {
}

bool PSDevice::IsDeviceOpen() const {
    return test_device_open;
}

void PSDevice::OpenDevice() {
    test_device_open = true;
}

void PSDevice::CloseDevice() {
    test_device_open = false;
}

