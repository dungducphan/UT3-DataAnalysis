#pragma once

#include <cstdio>

/* Headers for Windows */
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include "ps3000aApi.h"
#include "PicoStatus.h"
#else
#include <sys/types.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include <libps3000a/ps3000aApi.h>
#ifndef PICO_STATUS
#include <libps3000a/PicoStatus.h>
#endif

#define Sleep(a) usleep(1000*a)
#define scanf_s scanf
#define fscanf_s fscanf
#define memcpy_s(a,b,c,d) memcpy(a,c,d)

typedef enum enBOOL{FALSE,TRUE} BOOL;

/* A function to detect a keyboard press on Linux */
int32_t _getch();
int32_t _kbhit();
int32_t fopen_s(FILE ** a, const char * b, const char * c);

/* A function to get a single character on Linux */
#define max(a,b) ((a) > (b) ? a : b)
#define min(a,b) ((a) < (b) ? a : b)
#endif

#define PREF4 __stdcall

int32_t cycles = 0;

#define BUFFER_SIZE 	1024

#define QUAD_SCOPE		4
#define DUAL_SCOPE		2

// AWG Parameters

#define AWG_DAC_FREQUENCY			20e6
#define AWG_DAC_FREQUENCY_PS3207B	100e6
#define	AWG_PHASE_ACCUMULATOR		4294967296.0

typedef enum {
	ANALOGUE,
	DIGITAL,
	AGGREGATED,
	MIXED
} MODE;


typedef struct {
	int16_t DCcoupled;
	int16_t range;
	int16_t enabled;
} CHANNEL_SETTINGS;

typedef enum {
	SIGGEN_NONE = 0,
	SIGGEN_FUNCTGEN = 1,
	SIGGEN_AWG = 2
} SIGGEN_TYPE;

typedef struct tTriggerDirections {
	PS3000A_THRESHOLD_DIRECTION channelA;
	PS3000A_THRESHOLD_DIRECTION channelB;
	PS3000A_THRESHOLD_DIRECTION channelC;
	PS3000A_THRESHOLD_DIRECTION channelD;
	PS3000A_THRESHOLD_DIRECTION ext;
	PS3000A_THRESHOLD_DIRECTION aux;
} TRIGGER_DIRECTIONS;

typedef struct tPwq {
	PS3000A_PWQ_CONDITIONS_V2 * conditions;
	int16_t nConditions;
	PS3000A_THRESHOLD_DIRECTION direction;
	uint32_t lower;
	uint32_t upper;
	PS3000A_PULSE_WIDTH_TYPE type;
} PWQ;

typedef struct {
	int16_t					handle;
	int8_t					model[8];
	PS3000A_RANGE			firstRange;
	PS3000A_RANGE			lastRange;
	int16_t					channelCount;
	int16_t					maxValue;
	int16_t					sigGen;
	int16_t					ETS;
	int32_t					AWGFileSize;
	CHANNEL_SETTINGS		channelSettings [PS3000A_MAX_CHANNELS];
	int16_t					digitalPorts;
} UNIT;

uint32_t	timebase = 8;
int16_t     oversample = 1;
BOOL		scaleVoltages = TRUE;

uint16_t inputRanges [PS3000A_MAX_RANGES] = {10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000};

BOOL     	g_ready = FALSE;
int32_t 	g_times [PS3000A_MAX_CHANNELS] = {0, 0, 0, 0};
int16_t     g_timeUnit;
int32_t     g_sampleCount;
uint32_t	g_startIndex;
int16_t		g_autoStopped;
int16_t		g_trig = 0;
uint32_t	g_trigAt = 0;

char BlockFile[20]		= "block.txt";
char DigiBlockFile[20]	= "digiBlock.txt";
char StreamFile[20]		= "stream.txt";

typedef struct tBufferInfo {
	UNIT * unit;
	MODE mode;
	int16_t **driverBuffers;
	int16_t **appBuffers;
	int16_t **driverDigBuffers;
	int16_t **appDigBuffers;

} BUFFER_INFO;

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif


/****************************************************************************
* Streaming callback
* Used by ps3000a data streaming collection calls, on receipt of data.
* Used to set global flags etc. checked by user routines
****************************************************************************/
void PREF4 callBackStreaming(
	int16_t handle,
	int32_t		noOfSamples,
	uint32_t	startIndex,
	int16_t		overflow,
	uint32_t	triggerAt,
	int16_t		triggered,
	int16_t		autoStop,
	void		*pParameter);

/****************************************************************************
* Block Callback
* used by ps3000a data block collection calls, on receipt of data.
* used to set global flags etc. checked by user routines
****************************************************************************/
void PREF4 callBackBlock( int16_t handle, PICO_STATUS status, void * pParameter);

/****************************************************************************
* setDefaults - restore default settings
****************************************************************************/
void setDefaults(UNIT * unit);

/****************************************************************************
* setDigitals - enable or disable Digital Channels
****************************************************************************/
PICO_STATUS setDigitals(const UNIT *unit, const int16_t state);

/****************************************************************************
* disableAnalogue - Disable Analogue Channels
****************************************************************************/
PICO_STATUS disableAnalogue(UNIT *unit);

/****************************************************************************
* restoreAnalogueSettings - Restores Analogue Channel settings
****************************************************************************/
PICO_STATUS restoreAnalogueSettings(UNIT *unit);

/****************************************************************************
* adc_to_mv
* Convert an 16-bit ADC count into millivolts
****************************************************************************/
int32_t adc_to_mv(int32_t raw, int32_t ch, UNIT * unit);

/****************************************************************************
* mv_to_adc
* Convert a millivolt value into a 16-bit ADC count
*  (useful for setting trigger thresholds)
****************************************************************************/
int16_t mv_to_adc(int16_t mv, int16_t ch, UNIT * unit);

/****************************************************************************************
* changePowerSource - function to handle switches between +5 V supply, and USB-only power
* Only applies to PicoScope 34xxA/B/D/D MSO units
******************************************************************************************/
PICO_STATUS changePowerSource(int16_t handle, PICO_STATUS status);

/****************************************************************************
* clearDataBuffers
* stops GetData writing values to memory that has been released
****************************************************************************/
PICO_STATUS clearDataBuffers(const UNIT* unit);

/****************************************************************************
* blockDataHandler
* - Used by all block data routines
* - acquires data (user sets trigger mode before calling), displays 10 items
*   and saves all to block.txt
* Input :
* - unit : the unit to use.
* - text : the text to display before the display of data slice
* - offset : the offset into the data buffer to start the display's slice.
****************************************************************************/
void blockDataHandler(UNIT * unit, char * text, int32_t offset, MODE mode);

/****************************************************************************
* Stream Data Handler
* - Used by the two stream data examples - untriggered and triggered
* Inputs:
* - unit - the unit to sample on
* - preTrigger - the number of samples in the pre-trigger phase
*					(0 if no trigger has been set)
***************************************************************************/
void streamDataHandler(UNIT * unit, uint32_t preTrigger, MODE mode);

/****************************************************************************
* setTrigger
* - Used to call all the functions required to set up triggering
***************************************************************************/
PICO_STATUS setTrigger(	UNIT * unit,

						struct tPS3000ATriggerChannelProperties * channelProperties,
							int16_t nChannelProperties,
							PS3000A_TRIGGER_CONDITIONS_V2 * triggerConditionsV2,
							int16_t nTriggerConditions,
							TRIGGER_DIRECTIONS * directions,

						struct tPwq * pwq,
							uint32_t delay,
							int16_t auxOutputEnabled,
							int32_t autoTriggerMs,
							PS3000A_DIGITAL_CHANNEL_DIRECTIONS * digitalDirections,
							int16_t nDigitalDirections);

/****************************************************************************
* collectBlockImmediate
*  this function demonstrates how to collect a single block of data
*  from the unit (start collecting immediately)
****************************************************************************/
void collectBlockImmediate(UNIT * unit);

/****************************************************************************
* collectBlockEts
*  this function demonstrates how to collect a block of
*  data using equivalent time sampling (ETS).
****************************************************************************/
void collectBlockEts(UNIT * unit);

/****************************************************************************
* collectBlockTriggered
*  this function demonstrates how to collect a single block of data from the
*  unit, when a trigger event occurs.
****************************************************************************/
void collectBlockTriggered(UNIT* unit);

/****************************************************************************
* collectRapidBlock
*  this function demonstrates how to collect a set of captures using
*  rapid block mode.
****************************************************************************/
void collectRapidBlock(UNIT * unit);

/****************************************************************************
* Initialise unit structure with Variant specific defaults
****************************************************************************/
void get_info(UNIT * unit);

/****************************************************************************
* setVoltages
* Select input voltage ranges for channels
****************************************************************************/
void setVoltages(UNIT * unit);

/****************************************************************************
* Select timebase, set oversample to on and time units as nanoseconds
****************************************************************************/
void setTimebase(UNIT unit);

/****************************************************************************
* Sets the signal generator
* - allows user to set frequency and waveform
* - allows for custom waveform (values -32768..32767)
* - of up to 8192 samples long (PicoScope 3x04B & PS3x05B),
*	or 16384 samples long (PicoScope 3x06B),
*   or 32768 samples long (PicoScope 3207B, 3X0X D & D MSO devices)
******************************************************************************/
void setSignalGenerator(UNIT unit);

/****************************************************************************
* collectStreamingImmediate
*  this function demonstrates how to collect a stream of data
*  from the unit (start collecting immediately)
***************************************************************************/
void collectStreamingImmediate(UNIT * unit);

/****************************************************************************
* collectStreamingTriggered
*  this function demonstrates how to collect a stream of data
*  from the unit (start collecting on trigger)
***************************************************************************/
void collectStreamingTriggered(UNIT * unit);

/****************************************************************************
* displaySettings
* Displays information about the user configurable settings in this example
* Parameters
* - unit        pointer to the UNIT structure
*
* Returns       none
***************************************************************************/
void displaySettings(UNIT *unit);

/****************************************************************************
* openDevice
* Parameters
* - unit        pointer to the UNIT structure, where the handle will be stored
*
* Returns
* - PICO_STATUS to indicate success, or if an error occurred
***************************************************************************/
PICO_STATUS openDevice(UNIT *unit);

/****************************************************************************
* closeDevice
****************************************************************************/
void closeDevice(const UNIT *unit);

/****************************************************************************
* andAnalogueDigital
* This function shows how to collect a block of data from the analogue
* ports and the digital ports at the same time, triggering when the
* digital conditions AND the analogue conditions are met
*
* Returns       none
***************************************************************************/
void andAnalogueDigitalTriggered(UNIT * unit);

/****************************************************************************
* orAnalogueDigital
* This function shows how to collect a block of data from the analogue
* ports and the digital ports at the same time, triggering when either the
* digital conditions OR the analogue conditions are met
*
* Returns       none
***************************************************************************/
void orAnalogueDigitalTriggered(UNIT * unit);

/****************************************************************************
* digitalBlockTriggered
* This function shows how to collect a block of data from the digital ports
* with triggering enabled
*
* Returns       none
***************************************************************************/
void digitalBlockTriggered(UNIT * unit);

/****************************************************************************
* digitalBlockImmediate
* This function shows how to collect a block of data from the digital ports
* with triggering disabled
*
* Returns       none
***************************************************************************/
void digitalBlockImmediate(UNIT *unit);

/****************************************************************************
*  digitalStreamingAggregated
*  this function demonstrates how to collect a stream of Aggregated data
*  from the unit's Digital inputs (start collecting immediately)
***************************************************************************/
void digitalStreamingAggregated(UNIT * unit);

/****************************************************************************
*  digitalStreamingImmediate
*  this function demonstrates how to collect a stream of data
*  from the unit's Digital inputs (start collecting immediately)
***************************************************************************/
void digitalStreamingImmediate(UNIT * unit);

/****************************************************************************
* digitalMenu
* Displays digital examples available
* Parameters
* - unit        pointer to the UNIT structure
*
* Returns       none
***************************************************************************/
void digitalMenu(UNIT *unit);
