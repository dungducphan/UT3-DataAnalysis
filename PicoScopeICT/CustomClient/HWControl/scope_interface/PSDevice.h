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
    void SetChannel(int& channelID, bool& isChannelEnabled, bool& isACCoupled, float& channelRange, float& channelOffset);
    void SetTrigger(int& triggerSource, float& triggerThreshold, int& triggerDirection, float& preTrigger);

private:
    PICO_STATUS status{};
    UNIT unit{};
};
