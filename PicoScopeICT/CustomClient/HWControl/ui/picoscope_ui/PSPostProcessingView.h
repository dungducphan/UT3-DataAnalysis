#pragma once

class PSDataProcessor;
class PSDevice;

class PSPostProcessingView {
public:
    explicit PSPostProcessingView(PSDataProcessor* ps_data_processor, PSDevice* device);

    // Main method to render the GUI
    void Render();

private:
    PSDevice* ps_device;
    PSDataProcessor* ps_data_processor;

    std::thread acquisitionThread;

    // Render components
    void RenderDatasetView();
    void RenderHistogramView() const;
    void RenderErrorBarPlotView() const;

    // Dataset to show histogram
    int selectedDataset;
};