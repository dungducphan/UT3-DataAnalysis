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

void PSDevice::SetTimebase(int &tb) {
    timebase_ = tb;
}

void PSDevice::SetChannel(int &channelID, bool &isChannelEnabled, bool &isACCoupled, int &channelRange) {
    unit.channelSettings[channelID].enabled = isChannelEnabled ? TRUE : FALSE;  // Enable/Disable Channel
    unit.channelSettings[channelID].DCcoupled = isACCoupled ? FALSE : TRUE;   // DC/AC Coupled
    unit.channelSettings[channelID].range = channelRange + 1; // Range Enum
    status = ps3000aSetChannel( unit.handle,
                                static_cast<PS3000A_CHANNEL>(channelID),
                                unit.channelSettings[channelID].enabled,
                                static_cast<PS3000A_COUPLING>(unit.channelSettings[channelID].DCcoupled),
                                static_cast<PS3000A_RANGE>(unit.channelSettings[channelID].range),
                                0 );
}

void PSDevice::SetSimpleTrigger(
    const int& triggerSource,
    const int& triggerThreshold,
    const int& triggerDirection,
    const int& triggerDelay,
    const int& triggerTimeOutInMilliseconds) {
    int16_t triggerVoltage = 0;
    PS3000A_CHANNEL triggerSourceChannel;
    PS3000A_THRESHOLD_DIRECTION directions;

    if (triggerSource == 0) {
        triggerVoltage = mv_to_adc(static_cast<int16_t>(triggerThreshold), unit.channelSettings[PS3000A_CHANNEL_A].range, &unit);
        triggerSourceChannel = PS3000A_CHANNEL_A;
    } else if (triggerSource == 1) {
        triggerVoltage = mv_to_adc(static_cast<int16_t>(triggerThreshold), unit.channelSettings[PS3000A_CHANNEL_B].range, &unit);
        triggerSourceChannel = PS3000A_CHANNEL_B;
    } else {
        triggerVoltage = mv_to_adc(static_cast<int16_t>(triggerThreshold), unit.channelSettings[PS3000A_EXTERNAL].range, &unit);
        triggerSourceChannel = PS3000A_EXTERNAL;
    }

    if (triggerDirection == 0) {
        directions = PS3000A_RISING;
    } else {
        directions = PS3000A_FALLING;
    }

    if ((status = ps3000aSetSimpleTrigger(unit.handle, 1, triggerSourceChannel, triggerVoltage, directions, triggerDelay,
    static_cast<int16_t>(triggerTimeOutInMilliseconds))) !=
    PICO_OK) {
        printf("Error setting trigger\n");
    }

}

int32_t PSDevice::GetSamplingDurationInNanoseconds() const {
    return samplingDurationInNanoseconds_;
}

void PSDevice::GetTimebaseInfo(const int& TIME_BASE, int& samplingDurationInNanoseconds, int& samplingIntervalInNanoseconds) {
    status = ps3000aGetTimebase(unit.handle, TIME_BASE, BUFFER_SIZE, &samplingIntervalInNanoseconds, 0, nullptr, 0);
    samplingDurationInNanoseconds = BUFFER_SIZE * samplingIntervalInNanoseconds;
    samplingDurationInNanoseconds_ = samplingDurationInNanoseconds;
}

void PSDevice::CollectOneWaveform() {
    int16_t retry;
    int16_t bit;

    uint16_t bitValue;
    uint16_t digiValue;

    int16_t * buffers[PS3000A_MAX_CHANNEL_BUFFERS];
    int16_t * digiBuffer[PS3000A_MAX_DIGITAL_PORTS];

    int32_t i, j;
    float timeInterval;
    int32_t sampleCount = BUFFER_SIZE;
    int32_t maxSamples;
    int32_t timeIndisposed;

    FILE * fp = nullptr;
    FILE * digiFp = nullptr;

    PICO_STATUS status;
    PS3000A_RATIO_MODE ratioMode = PS3000A_RATIO_MODE_NONE;

    for (i = 0; i < unit.channelCount; i++) {
        if (unit.channelSettings[i].enabled) {
            buffers[i * 2] = static_cast<int16_t *>(calloc(sampleCount, sizeof(int16_t)));
            buffers[i * 2 + 1] = static_cast<int16_t *>(calloc(sampleCount, sizeof(int16_t)));
            ps3000aSetDataBuffers(unit.handle, static_cast<PS3000A_CHANNEL>(i), buffers[i * 2], buffers[i * 2 + 1], sampleCount, 0, ratioMode);
        }
    }

    // Calculate time interval (in nanoseconds)
    ps3000aGetTimebase2(unit.handle, timebase_, sampleCount, &timeInterval, 0, &maxSamples, 0);

    /* Start the device collecting, then wait for completion*/
    g_ready = FALSE;

    do {
        retry = 0;
        status = ps3000aRunBlock(unit.handle, 0, sampleCount, timebase_, oversample, &timeIndisposed, 0, callBackBlock, nullptr);
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
    // printf("Waiting for trigger...\n");

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
		    // Actual readout code
		    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
			for (j = 0; j < unit.channelCount; j++) {
				if (unit.channelSettings[j].enabled) {
				    for (i = 0; i < BUFFER_SIZE; i++) {
				        currentTimeArray[i] = static_cast<float>(i) * timeInterval;
				        if (j == 0) {
				            currentWaveformChannelA[i] = adc_to_mv_float(buffers[j * 2][i], unit.channelSettings[PS3000A_CHANNEL_A + j].range, &unit);
				        } else {
				            currentWaveformChannelB[i] = adc_to_mv_float(buffers[j * 2][i], unit.channelSettings[PS3000A_CHANNEL_A + j].range, &unit);
				        }
                    }
				}
			}
		    sampleCount = ps_min(sampleCount, BUFFER_SIZE);
		    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	} else {
		printf("\nData collection aborted.\n");
		_getch();
	}

    if ((status = ps3000aStop(unit.handle)) != PICO_OK) printf("BlockDataHandler:ps3000aStop ------ 0x%08lx \n", status);

    for (i = 0; i < unit.channelCount; i++) {
        if (unit.channelSettings[i].enabled) {
            free(buffers[i * 2]);
            free(buffers[i * 2 + 1]);
        }
    }

    clearDataBuffers(&unit);
}


