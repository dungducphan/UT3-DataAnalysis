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

