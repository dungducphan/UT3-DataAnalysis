#include "PCH.h"

#include "PSDevice.h"

PSDevice::PSDevice() {
}

PSDevice::~PSDevice() {
}

bool PSDevice::IsDeviceOpen() const {
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

void PSDevice::SetChannel(int &channelID, bool &isChannelEnabled, bool &isACCoupled, int &channelRange) {
    unit.channelSettings[channelID].enabled = isChannelEnabled ? TRUE : FALSE;  // Enable/Disable Channel
    unit.channelSettings[channelID].DCcoupled = isACCoupled ? FALSE : TRUE;   // DC/AC Coupled
    unit.channelSettings[channelID].range = static_cast<PS3000A_RANGE>(channelRange + 1); // Range Enum
}

void PSDevice::SetTrigger(int &triggerSource, float &triggerThreshold, int &triggerDirection, int &triggerDelayInDivs) {
    struct tPS3000ATriggerChannelProperties sourceDetails{};
    struct tPS3000ATriggerConditionsV2 conditions{};
    struct tPwq pulseWidth{};
    struct tTriggerDirections directions{};
    int16_t	triggerVoltage = 0;
    int delayInSamples = 0;

    std::cout << "Setting trigger..." << std::endl;

    if (triggerSource == 0) {
        triggerVoltage = mv_to_adc(1000, unit.channelSettings[PS3000A_CHANNEL_A].range, &unit);
        sourceDetails = {	triggerVoltage,
                            256 * 10,
                            triggerVoltage,
                            256 * 10,
                            PS3000A_CHANNEL_A,
                            PS3000A_LEVEL};
        conditions = {PS3000A_CONDITION_TRUE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE};
        if (triggerDirection == 0) {
            directions = {PS3000A_RISING,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE};
        } else {
            directions = {PS3000A_FALLING,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE};
        }
    } else if (triggerSource == 1) {
        triggerVoltage = mv_to_adc(1000, unit.channelSettings[PS3000A_CHANNEL_B].range, &unit);
        sourceDetails = {	triggerVoltage,
                            256 * 10,
                            triggerVoltage,
                            256 * 10,
                            PS3000A_CHANNEL_B,
                            PS3000A_LEVEL};
        conditions = {PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_TRUE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE};
        if (triggerDirection == 0) {
            directions = {PS3000A_NONE,
                            PS3000A_RISING,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE};
        } else {
            directions = {PS3000A_NONE,
                            PS3000A_FALLING,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE};
        }
    } else {
        triggerVoltage = mv_to_adc(1000, unit.channelSettings[PS3000A_EXTERNAL].range, &unit);
        sourceDetails = {	triggerVoltage,
                            256 * 10,
                            triggerVoltage,
                            256 * 10,
                            PS3000A_EXTERNAL,
                            PS3000A_LEVEL};
        conditions = {PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_TRUE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE,
                        PS3000A_CONDITION_DONT_CARE};
        if (triggerDirection == 0) {
            directions = {PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_RISING,
                            PS3000A_NONE};
        } else {
            directions = {PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_NONE,
                            PS3000A_FALLING,
                            PS3000A_NONE};
        }
    }
    memset(&pulseWidth, 0, sizeof(struct tPwq));
    setTrigger(&unit, &sourceDetails, 1, &conditions, 1, &directions, &pulseWidth, 0, 0, 0, nullptr, 0);
}

