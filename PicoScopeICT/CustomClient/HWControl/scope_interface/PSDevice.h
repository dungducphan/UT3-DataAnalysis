#pragma once

#include <PSConfig.h>

#include "PSDevice.h"

class PSDevice {
public:
    PSDevice();
    ~PSDevice();

public:
    void OpenDevice();
    void CloseDevice() const;

private:
    UNIT unit{};
    PICO_STATUS status{};
};

