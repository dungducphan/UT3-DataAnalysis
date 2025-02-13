#include "PCH.h"

#include "PSDataProcessor.h"

PSDataProcessor::PSDataProcessor() {
    datasets.clear();
}


void PSDataProcessor::AddDataset(const std::string& path) {
    // Get list of all .csv files in the directory
    std::vector<std::string> csvFiles = GetCSVFiles(path);

    if (csvFiles.empty()) {
        std::cerr << "Error: No .csv files found in directory " << path << std::endl;
        return;
    }

    // Import csv files and make dataset
    Dataset dataset;
    auto h = new TH1D(Form("h_%03i", datasets.size() + 1), Form("h_%03i", datasets.size() + 1), 1000, 0, 200);
    for (unsigned int i = 0; i < csvFiles.size(); i++) {
        Waveform_t wf = ReadSingleWaveform(csvFiles.at(i));
        TGraph* gr = WaveformToTGraph(wf);
        BackgroundSubtraction(gr);
        double integral = IntegrateTGraph(gr);
        h->Fill(integral * 1E12 / 50);
        Waveform waveform{};
        waveform.wfID = i;
        waveform.chargeValue = integral * 1E12 / 50;
        dataset.waveforms.push_back(waveform);
    }

    dataset.path = path;
    dataset.name = "Scan " + std::to_string(datasets.size() + 1);
    dataset.meanCharge = h->GetMean();
    dataset.stdDevCharge = h->GetStdDev();
    datasets.push_back(dataset);
    delete h;
}

void PSDataProcessor::CreateDataForErrorBarPlot(
    double* x,
    double* y,
    double* y_err) const {
    for (int i = 0; i < GetDatasetSize(); i++) {
        x[i] = i;
        y[i] = datasets.at(i).meanCharge;
        y_err[i] = datasets.at(i).stdDevCharge;
    }
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

Dataset PSDataProcessor::GetDataset(int datasetID) const {
    return datasets.at(datasetID);
}

Waveform_t PSDataProcessor::ReadSingleWaveform(const std::string& filename) {
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

TGraph* PSDataProcessor::WaveformToTGraph(const Waveform_t& wf) {
    std::vector<double> x = wf.first;
    std::vector<double> y = wf.second;
    auto gr = new TGraph(x.size(), x.data(), y.data());
    return gr;
}

void PSDataProcessor::BackgroundSubtraction(TGraph* gr) {
    TH1D* bkgd = new TH1D("bkgd", "bkgd", 100, gr->GetMinimum(), gr->GetMaximum());
    for (int i = 0; i < gr->GetN(); i++) {
        double x, y;
        gr->GetPoint(i, x, y);
        bkgd->Fill(y);
    }

    double mean = bkgd->GetMean();
    double stdDev = bkgd->GetStdDev();
    delete bkgd;

    TH1D* bkgd_refined = new TH1D("bkgd_refined", "bkgd_refined", 100, gr->GetMinimum(), gr->GetMaximum());
    for (int i = 0; i < gr->GetN(); i++) {
        double x, y;
        gr->GetPoint(i, x, y);
        if (TMath::Abs(y - mean) < 3 * stdDev) bkgd_refined->Fill(y);
    }
    mean = bkgd_refined->GetMean();
    delete bkgd_refined;

    for (int i = 0; i < gr->GetN(); i++) {
        double x, y;
        gr->GetPoint(i, x, y);
        gr->SetPoint(i, x, y - mean);
    }
}

double PSDataProcessor::IntegrateTGraph(TGraph* gr) {
    return gr->Integral();
}

std::vector<std::string> PSDataProcessor::GetCSVFiles(const std::string& directoryPath) {
    std::vector<std::string> csvFiles;
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            csvFiles.push_back(entry.path().string());
        }
    }
    return csvFiles;
}