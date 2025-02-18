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

    int32_t currentWaveformChannelA[BUFFER_SIZE];
    int32_t currentWaveformChannelB[BUFFER_SIZE];
    int32_t currentTimeArray[BUFFER_SIZE];

private:
    int32_t timebase_{};
    PICO_STATUS status{};
    UNIT unit{};
};
