#include "PCH.h"

#include "PSDevice.h"

PSDevice::PSDevice() {
    printf("ICT Beam Charge Measurement with PicoScope 3206D\n");
    printf("Searching PicoScope...\n");
}

PSDevice::~PSDevice() {
    printf("Closing PicoScope...\n");
    CloseDevice();
}

void PSDevice::OpenDevice() {
    status = openDevice(&unit);
    displaySettings(&unit);
}

void PSDevice::CloseDevice() const {
    closeDevice(&unit);
}

