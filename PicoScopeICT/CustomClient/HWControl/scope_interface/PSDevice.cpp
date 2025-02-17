#include "PCH.h"

#include "PSDevice.h"

PSDevice::PSDevice() {
}

PSDevice::~PSDevice() {
}

bool PSDevice::IsDeviceOpen() const {
    std::cout << "unit.handle: " << unit.handle << std::endl;
    return unit.handle > 0;
}

void PSDevice::OpenDevice() {
    printf("PicoScope 3000 Series (A API) Driver Example Program\n");
    printf("\nOpening the device...\n");

    status = openDevice(&unit);
}

void PSDevice::CloseDevice() {
    closeDevice(&unit);
    unit = {};
    status = PICO_OK;
}

void PSDevice::SetTimebase(int &timebase_value, int &timebase_unit) {
}

void PSDevice::SetChannel(int &channelID, bool &isChannelEnabled, bool &isACCoupled, float &channelRange,
    float &channelOffset) {
}

void PSDevice::SetTrigger(int &triggerSource, float &triggerThreshold, int &triggerDirection, float &preTrigger) {
}

