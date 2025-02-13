#pragma once

#include <PSDevice.h>

class PSControlView {
public:
    explicit PSControlView(PSDevice* pdDev);

    // Main method to render the GUI
    void Render();

private:
    PSDevice* ps_device;

    void RenderChannelSettings();
    void RenderTriggerSettings();
    void RenderTimebaseSettings();
    void RenderWaveformDisplay();
};
