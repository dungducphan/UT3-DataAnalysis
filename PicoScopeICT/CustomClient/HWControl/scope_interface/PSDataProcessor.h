#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>

#include "TH1.h"
#include "TROOT.h"
#include "TGraph.h"

using waveform = std::pair<std::vector<double>, std::vector<double>>;

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

private:
    std::vector<Dataset> datasets;

    static waveform ReadSingleWaveform(const std::string& filename);
    static TGraph* WaveformToTGraph(const waveform& wf);
    static void BackgroundSubtraction(TGraph* gr, int idx);
    static double IntegrateTGraph(TGraph* gr, int idx_min);
};

