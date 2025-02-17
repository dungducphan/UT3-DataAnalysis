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

    int GetDatasetSize() const;
    int GetScanSize(int datasetID) const;
    double GetScanMeanCharge(int datasetID) const;
    double GetScanStdDevCharge(int datasetID) const;
    Dataset GetDataset(int datasetID) const;
    void CreateDataForErrorBarPlot(double* x, double* y, double* y_err) const;

private:
    std::vector<Dataset> datasets;

    static Waveform_t ReadSingleWaveform(const std::string& filename);
    static TGraph* WaveformToTGraph(const Waveform_t& wf);
    static void BackgroundSubtraction(TGraph* gr);
    static double IntegrateTGraph(TGraph* gr);
    static std::vector<std::string> GetCSVFiles(const std::string& directoryPath);
};
