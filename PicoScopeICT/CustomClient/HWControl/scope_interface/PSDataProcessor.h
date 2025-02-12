#pragma once

#include<vector>
#include<string>

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
};

