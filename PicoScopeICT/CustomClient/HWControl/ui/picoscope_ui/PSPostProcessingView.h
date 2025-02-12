#pragma once

#include <string>
#include <functional>

class PSDataProcessor;

class PSPostProcessingView {
public:
    explicit PSPostProcessingView(PSDataProcessor* ps_data_processor);

    // Main method to render the GUI
    void Render();

private:
    PSDataProcessor* ps_data_processor;

    // Render components
    void RenderDatasetView();
    void RenderHistogramView();

    // Dataset to show histogram
    int selectedDataset;
};