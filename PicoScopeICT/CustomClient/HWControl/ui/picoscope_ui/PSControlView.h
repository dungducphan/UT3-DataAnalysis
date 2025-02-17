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
    bool channel_enabled[2] = {false, false};
    const char* channel_range[10] = {"20mV", "50mV", "100mV", "200mV", "500mV", "1V", "2V", "5V", "10V", "20V"};
    const char* channel_coupling[2] = {"AC", "DC"};
    bool  channel_0_AC_coupling = false;
    bool  channel_1_AC_coupling = false;
    float channel_0_range = 1.0f;
    float channel_0_offset = 0.0f;
    float channel_1_range = 1.0f;
    float channel_1_offset = 0.0f;
    const char* trigger_sources[3] = {"Channel 0", "Channel 1", "External"};
    const char* trigger_directions[2] = {"Rising Edge", "Falling Edge"};
};
