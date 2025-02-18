#pragma once

#include "PSConfig.h"

class PSDevice {
public:
    PSDevice();
    ~PSDevice();

    [[nodiscard]] bool IsDeviceOpen() const;
    void OpenDevice();
    void CloseDevice();

    void SetTimebase(int& timebase);
    void SetChannel(int& channelID, bool& isChannelEnabled, bool& isACCoupled, int& channelRange);
    void SetSimpleTrigger(const int& triggerSource,
                          const int& triggerThreshold,
                          const int& triggerDirection,
                          const int& triggerDelay,
                          const int& triggerTimeOutInMilliseconds);

    void CollectOneWaveform();
    void GetTimebaseInfo(const int& timebase, int& samplingDurationInNanoseconds, int& samplingIntervalInNanoseconds);

    float currentWaveformChannelA[BUFFER_SIZE];
    float currentWaveformChannelB[BUFFER_SIZE];
    float currentTimeArray[BUFFER_SIZE];

private:
    int32_t timebase_{};
    PICO_STATUS status{};
    UNIT unit{};
};
