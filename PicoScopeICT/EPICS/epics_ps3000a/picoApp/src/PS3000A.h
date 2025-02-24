/* 
** Device support for PicoScope 3000 Series is written based on 
** the example of "testAsynPortDriver" from asyn module,
** the C/C++ example of "ps3000a" from Pico Technology
** (https://github.com/picotech/picosdk-c-examples/tree/master/ps3000a),
** and the "pico_2000" EPICS driver from GSI 
** (https://git.gsi.de/r3b_public/epics/pico_2000)
**
*/

#pragma once

#include "asynPortDriver.h"

#define P_RunString             "PICOSCOPE_RUN"               /* asynInt32,         r/w */
#define P_MaxPointsString       "PICOSCOPE_MAX_POINTS"        /* asynInt32,         r/o */
#define P_PicoConnectString     "PICOSCOPE_CONNECT"           /* asynInt32,         r/w */
#define P_PicoConnectedString   "PICOSCOPE_CONNECTED"         /* asynInt32,         r/o */
#define P_SampleLengthString    "PICOSCOPE_SAMPLE_LENGTH"     /* asynInt32,         r/o */
#define P_SampleFrequencyString "PICOSCOPE_SAMPLE_FREQUENCY"  /* asynInt32,         r/o */
#define P_TriggerSourceString   "PICOSCOPE_TRIGGER_SOURCE   " /* asynInt32,         r/o */
#define P_ChannelRangeStringA   "PICOSCOPE_CHA_RANGE"         /* asynInt32,         r/w */
#define P_ChannelRangeStringB   "PICOSCOPE_CHB_RANGE"         /* asynInt32,         r/w */

#define P_TimeBaseString        "PICOSCOPE_TIMEBASE"          /* asynFloat64Array,  r/o */
#define P_WaveformStringA       "PICOSCOPE_WAVEFORM_A"        /* asynFloat64Array,  r/o */
#define P_WaveformStringB       "PICOSCOPE_WAVEFORM_B"        /* asynFloat64Array,  r/o */



#include <ps3000aApi.h>

struct PS3000AChannel {
	PS3000A_RANGE    range;
	int              range_v;
	PS3000A_COUPLING coupling;
};

struct PS3000AConfig {
	PS3000AChannel ch[2];
};

struct BlockInfo {
	int16_t ready;
};

struct PS3000AModule {
	int16_t  handle;
	struct   PS3000AConfig config;
	int16_t  max_value;
	float    maximumVoltage;
	float    minimumVoltage;
	float    time_interval_ns;
	uint32_t time_base;
	int32_t  max_points;
	uint32_t max_down_sample_ratio;
};


class PS3000A : public asynPortDriver {
public:
    PS3000A(const char *portName, int maxArraySize);
                 
    /* These are the methods that we override from asynPortDriver */
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus readFloat64Array(asynUser *pasynUser, epicsFloat64 *value, size_t nElements, size_t *nIn);

    /* These are the methods that are new to this class */
    void RunTask(void);

protected:
    /** Values used for pasynUser->reason, and indexes into the parameter library. */

    int P_Run             = 0;
    int P_MaxPoints       = 0;
    int P_PicoStatus      = 0;
    int P_PicoConnect     = 0;
    int P_PicoConnected   = 0;
    int P_SampleLength    = 0;
    int P_SampleFrequency = 0;
    int P_TriggerSource   = 0;
    int P_TimeBase        = 0;
    int P_WaveformA       = 0;
    int P_WaveformB       = 0;
    int P_ChannelRangeA   = 0;
    int P_ChannelRangeB   = 0;
 
private:
    /* Our data */
    epicsEventId eventId_;
    epicsFloat64 *pData_[2];
    epicsFloat64 *pTimeBase_;


    void ConnectPicoScope(void);
    int ClosePS3000A(void);
    int SetTimeBase(void);
    void SetTimeBaseArray(void);
    void SetChannels(void);
    int SetTrigger(void);
    int PicoRunBlock(void);
    int OpenPS3000A(void);
    void PrintUnitInfo(void);

    int16_t mv_to_adc(int16_t, int);
    int32_t adc_to_uv(int32_t, int);
    float s_to_ns(int32_t);

    /* pico data members */
    int16_t *data_buffer[2];
    epicsInt32 max_sample_length;
    struct PS3000AModule ps;
};

#define CHKOK_OR_RETURN(name) do {\
	setIntegerParam(P_PicoStatus, ok);\
	if (ok != PICO_OK) {\
		printf("ERROR %d in %s: %s\n", ok, name, ps3000a_strings[ok]);\
		return ok;\
	}\
}while (0)
#define CHKOK(name) do {\
	setIntegerParam(P_PicoStatus, ok);\
	if (ok != PICO_OK) {\
		printf("ERROR %d in %s: %s\n", ok, name, ps3000a_strings[ok]);\
	}\
}while (0)

