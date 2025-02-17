#pragma once

#include <PSDevice.h>
#include <PSDataProcessor.h>

class PSControlView {
public:
    explicit PSControlView(PSDataProcessor* dataProp, PSDevice* pdDev);

    // Main method to render the GUI
    void Render();

private:
    PSDataProcessor* ps_data_processor;
    PSDevice* ps_device;

    // Constants
    const char* timebase_numeric[9] = {"1", "2", "5", "10", "20", "50", "100", "200", "500"};
    const char* timebase_unit[4] = {"ns", "us", "ms", "s"};
    const char* channel_range[10] = {"20mV", "50mV", "100mV", "200mV", "500mV", "1V", "2V", "5V", "10V", "20V"};
    const char* channel_coupling[2] = {"AC", "DC"};
    const char* trigger_sources[3] = {"Channel A", "Channel B", "External"};
    const char* trigger_directions[2] = {"Rising Edge", "Falling Edge"};
};
