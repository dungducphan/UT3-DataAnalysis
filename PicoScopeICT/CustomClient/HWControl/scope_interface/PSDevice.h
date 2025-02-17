#pragma once

#include "PSConfig.h"

class PSDevice {
public:
    PSDevice();
    ~PSDevice();

    [[nodiscard]] bool IsDeviceOpen() const;
    void OpenDevice();
    void CloseDevice();

    void SetTimebase(int& timebase_value, int& timebase_unit);
    void SetChannel(int& channelID, bool& isChannelEnabled, bool& isACCoupled, int& channelRange);
    void SetTrigger(int& triggerSource, float& triggerThreshold, int& triggerDirection, int& triggerDelay);

private:
    PICO_STATUS status{};
    UNIT unit{};
};
