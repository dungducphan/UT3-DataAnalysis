#include "PSDataProcessor.h"

PSDataProcessor::PSDataProcessor() {
    datasets.clear();
}

void PSDataProcessor::AddDataset(const std::string& path) {
    Dataset dataset;
    dataset.path = path;
    dataset.name = "";
    dataset.meanCharge = 0.0;
    dataset.stdDevCharge = 0.0;
    datasets.push_back(dataset);
}

int PSDataProcessor::GetDatasetSize() const {
    return static_cast<int>(datasets.size());
}

int PSDataProcessor::GetScanSize(int datasetID) const {
    return static_cast<int>((datasets.at(datasetID)).waveforms.size());
}

double PSDataProcessor::GetScanMeanCharge(int datasetID) const {
    return (datasets.at(datasetID)).meanCharge;
}

double PSDataProcessor::GetScanStdDevCharge(int datasetID) const {
    return (datasets.at(datasetID)).stdDevCharge;
}