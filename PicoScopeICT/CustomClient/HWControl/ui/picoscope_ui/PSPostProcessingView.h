#pragma once

#include <string>
#include <functional>

class PSDataProcessor;

class PSPostProcessingView {
public:
    // Constructor requires a reference to the CameraManager (Model)
    explicit PSPostProcessingView(PSDataProcessor& ps_data_processor);

    // Main method to render the GUI
    void Render();


private:
    // Reference to the CameraManager (Model)
    PSDataProcessor& ps_data_processor;

    // Render components
    void RenderDatasetView();
    void RenderHistogramView();
};