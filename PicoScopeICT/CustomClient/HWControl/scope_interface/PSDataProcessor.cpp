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


waveform PSDataProcessor::ReadSingleWaveform(const std::string& filename) {
    std::vector<double> x, y;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return std::make_pair(x, y);
    }

    std::string line;

    // Skip the header lines
    file >> line;
    file >> line;
    file >> line;
    file >> line;

    // Read the data lines
    int counter = 0;
    while (file >> line) {
        std::istringstream ss(line);
        std::string x_str, y_str;
        if (std::getline(ss, x_str, ',') && std::getline(ss, y_str, ',')) {
            double x_val = std::stod(x_str);
            double y_val = std::stod(y_str);
            x.push_back(x_val * 1E-9); // ns
            y.push_back(y_val * 1E-3); // mV
        }
        counter++;
    }

    file.close();

    return std::make_pair(x, y);
}

TGraph* PSDataProcessor::WaveformToTGraph(const waveform& wf) {
    std::vector<double> x = wf.first;
    std::vector<double> y = wf.second;

    auto gr = new TGraph(x.size(), x.data(), y.data());
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(0.5);
    gr->SetMarkerColor(kBlack);
    gr->SetLineColor(kBlack);

    return gr;
}

void PSDataProcessor::BackgroundSubtraction(TGraph* gr, int idx) {
    double mean = 0;
    for (int i = 0; i < idx; i++) {
        double x, y;
        gr->GetPoint(i, x, y);
        mean += y;
    }
    mean = mean / idx;

    for (int i = 0; i < gr->GetN(); i++) {
        double x, y;
        gr->GetPoint(i, x, y);
        gr->SetPoint(i, x, y - mean);
    }
}

double PSDataProcessor::IntegrateTGraph(TGraph* gr, int idx_min) {
    return gr->Integral(idx_min, -1);
}