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
        triggerVoltage =16000;
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

void PSDevice::CollectOneWaveform() {
    int16_t retry;
    int16_t bit;

    uint16_t bitValue;
    uint16_t digiValue;

    int16_t * buffers[PS3000A_MAX_CHANNEL_BUFFERS];
    int16_t * digiBuffer[PS3000A_MAX_DIGITAL_PORTS];

    int32_t i, j;
    int32_t timeInterval;
    int32_t sampleCount = BUFFER_SIZE;
    int32_t maxSamples;
    int32_t timeIndisposed;

    FILE * fp = nullptr;
    FILE * digiFp = nullptr;

    PICO_STATUS status;
    PS3000A_RATIO_MODE ratioMode = PS3000A_RATIO_MODE_NONE;

    for (i = 0; i < unit.channelCount; i++) {
        if(unit.channelSettings[i].enabled) {
            buffers[i * 2] = static_cast<int16_t *>(calloc(sampleCount, sizeof(int16_t)));
            buffers[i * 2 + 1] = static_cast<int16_t *>(calloc(sampleCount, sizeof(int16_t)));
            status = ps3000aSetDataBuffers(unit.handle, static_cast<PS3000A_CHANNEL>(i), buffers[i * 2], buffers[i * 2 + 1], sampleCount, 0, ratioMode);
            printf(status?"BlockDataHandler:ps3000aSetDataBuffers(channel %d) ------ 0x%08lx \n":"", i, status);
        }
    }

    // /* Find the maximum number of samples and the time interval (in nanoseconds) */
    // while (ps3000aGetTimebase(unit.handle, timebase, sampleCount, &timeInterval, oversample, &maxSamples, 0)) {
    //     timebase++;
    // }
    // printf("\nTimebase: %lu  Sample interval: %ld ns \n", timebase, timeInterval);

    /* Start the device collecting, then wait for completion*/
    g_ready = FALSE;

    do {
        retry = 0;
        status = ps3000aRunBlock(unit.handle, 0, sampleCount, timebase, oversample, &timeIndisposed, 0, callBackBlock, nullptr);
        if (status != PICO_OK) {
            if (status == PICO_POWER_SUPPLY_CONNECTED || status == PICO_POWER_SUPPLY_NOT_CONNECTED ||
                status == PICO_POWER_SUPPLY_UNDERVOLTAGE) {      // PicoScope 340XA/B/D/D MSO devices...+5 V PSU connected or removed
                status = changePowerSource(unit.handle, status);
                retry = 1;
                } else {
                    printf("BlockDataHandler:ps3000aRunBlock ------ 0x%08lx \n", status);
                    return;
                }
        }
    } while (retry);
    printf("Waiting for trigger...\n");

    while (!g_ready && !_kbhit()) Sleep(0);

	if (g_ready) {
		status = ps3000aGetValues(unit.handle, 0, reinterpret_cast<uint32_t *>(&sampleCount), 1, ratioMode, 0, nullptr);
		if (status != PICO_OK) {
			if (status == PICO_POWER_SUPPLY_CONNECTED || status == PICO_POWER_SUPPLY_NOT_CONNECTED || status == PICO_POWER_SUPPLY_UNDERVOLTAGE) {
				if (status == PICO_POWER_SUPPLY_UNDERVOLTAGE) {
					changePowerSource(unit.handle, status);
				} else {
					printf("\nPower Source Changed. Data collection aborted.\n");
				}
			} else {
				printf("BlockDataHandler:ps3000aGetValues ------ 0x%08lx \n", status);
			}
		} else {
			printf("Channels are in %s\n\n", ( scaleVoltages ) ? ("mV") : ("ADC Counts"));

			for (j = 0; j < unit.channelCount; j++) {
				if (unit.channelSettings[j].enabled) printf("Channel %c:    ", 'A' + j);
			}
			printf("\n");

			for (j = 0; j < unit.channelCount; j++) {
				if (unit.channelSettings[j].enabled) {
					printf("  %d     ", scaleVoltages ?
						adc_to_mv(buffers[j * 2][i], unit.channelSettings[PS3000A_CHANNEL_A + j].range, &unit)	// If scaleVoltages, print mV value
						: buffers[j * 2][i]);																	// else print ADC Count
				}
			}
			printf("\n");

		    sampleCount = min(sampleCount, BUFFER_SIZE);
		    std::cout << "Starting data capture for " << sampleCount << " samples..." << std::endl;
		}
	} else {
		printf("\nData collection aborted.\n");
		_getch();
	}

    if ((status = ps3000aStop(unit.handle)) != PICO_OK)
        printf("BlockDataHandler:ps3000aStop ------ 0x%08lx \n", status);

    for (i = 0; i < unit.channelCount; i++) {
        if (unit.channelSettings[i].enabled) {
            free(buffers[i * 2]);
            free(buffers[i * 2 + 1]);
        }
    }

    clearDataBuffers(&unit);

    std::cout << "Data capture complete." << std::endl;
}


