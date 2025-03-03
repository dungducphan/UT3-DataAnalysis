#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "TH1.h"
#include "TROOT.h"
#include "TGraph.h"

using Waveform_t = std::pair<std::vector<double>, std::vector<double>>;

struct Waveform {
    int wfID;
    double chargeValue;
};

struct Dataset {
    std::string path;
    std::string name;
    std::vector<Waveform> waveforms;
    double meanCharge;
    double stdDevCharge;
};

class PSDataProcessor {
public:
    PSDataProcessor();
    void AddDataset(const std::string& path);
    void AddLiveDataset(int scanID);
    void AddWaveformToLiveDataset(int scanID, const Waveform_t& wf, int expectedNoOfWaveforms);

    [[nodiscard]] int GetDatasetSize() const;
    [[nodiscard]] int GetScanSize(int datasetID) const;
    [[nodiscard]] double GetScanMeanCharge(int datasetID) const;
    [[nodiscard]] double GetScanStdDevCharge(int datasetID) const;
    [[nodiscard]] Dataset GetDataset(int datasetID) const;
    void CreateDataForErrorBarPlot(double* x, double* y, double* y_err) const;

    std::vector<Dataset> datasets;

    static Waveform_t ReadSingleWaveform(const std::string& filename);
    static Waveform_t ReadSingleWaveformLive(const float*, const float*, size_t size);
    static TGraph* WaveformToTGraph(const Waveform_t& wf);
    void BackgroundSubtraction(TGraph* gr) const;
    double IntegrateTGraph(TGraph* gr);
    static std::vector<std::string> GetCSVFiles(const std::string& directoryPath);

    static void SaveWaveformToCSV(const Waveform_t& wf, const std::string& filename);

    double cursorLeft = -1.0f;
    double cursorRight = -1.0f;
};
