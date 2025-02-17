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
};
