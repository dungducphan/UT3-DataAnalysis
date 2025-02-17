#pragma once

#include <PSDevice.h>

class PSControlView {
public:
    explicit PSControlView(PSDevice* pdDev);

    // Main method to render the GUI
    void Render();

private:
    PSDevice* ps_device;

    // Constants
    const char* timebase_numeric[9] = {"1", "2", "5", "10", "20", "50", "100", "200", "500"};
    const char* timebase_unit[4] = {"ns", "us", "ms", "s"};
    const char* channel_range[10] = {"20mV", "50mV", "100mV", "200mV", "500mV", "1V", "2V", "5V", "10V", "20V"};
    const char* channel_coupling[2] = {"AC", "DC"};
    const char* trigger_sources[3] = {"Channel 0", "Channel 1", "External"};
    const char* trigger_directions[2] = {"Rising Edge", "Falling Edge"};
};
