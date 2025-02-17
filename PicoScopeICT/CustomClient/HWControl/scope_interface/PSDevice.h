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

    void CollectOneWaveform();

    int32_t currentWaveformChannelA[BUFFER_SIZE];
    int32_t currentWaveformChannelB[BUFFER_SIZE];
    int32_t currentTimeArray[BUFFER_SIZE];

private:
    PICO_STATUS status{};
    UNIT unit{};
};
