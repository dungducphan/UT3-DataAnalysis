#include "PCH.h"

#include "PSDataProcessor.h"

PSDataProcessor::PSDataProcessor() {
    datasets.clear();
}

void PSDataProcessor::AddLiveDataset(int scanID) {
    Dataset dataset{};
    dataset.path = "";
    dataset.name = "LiveScan_" + std::to_string(scanID);
    dataset.meanCharge = 0;
    dataset.stdDevCharge = 0;
    datasets.push_back(dataset);
}

void PSDataProcessor::AddWaveformToLiveDataset(int scanID, const Waveform_t& wf, int expectedNoOfWaveforms) {
    Waveform waveform{};
    waveform.wfID = static_cast<int>(datasets.at(scanID).waveforms.size());
    auto grWf = WaveformToTGraph(wf);
    BackgroundSubtraction(grWf);
    waveform.chargeValue = IntegrateTGraph(grWf) / 50;
    delete grWf;
    std::cout << "Charge: " << waveform.chargeValue << "pC." << std::endl;
    datasets.at(scanID).waveforms.push_back(waveform);

    if (datasets.at(scanID).waveforms.size() == expectedNoOfWaveforms) {
        double meanCharge = 0;
        for (const auto& w : datasets.at(scanID).waveforms) {
            meanCharge += w.chargeValue;
        }
        meanCharge /= static_cast<double>(datasets.at(scanID).waveforms.size());

        double stdDevCharge = 0;
        for (const auto& w : datasets.at(scanID).waveforms) {
            stdDevCharge += (w.chargeValue - meanCharge) * (w.chargeValue - meanCharge);
        }
        stdDevCharge = std::sqrt(stdDevCharge / static_cast<double>(datasets.at(scanID).waveforms.size()));

        datasets.at(scanID).meanCharge = meanCharge;
        datasets.at(scanID).stdDevCharge = stdDevCharge;
    }
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
        h->Fill(integral / 50);
        Waveform waveform{};
        waveform.wfID = i;
        waveform.chargeValue = integral / 50;
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

Waveform_t PSDataProcessor::ReadSingleWaveformLive(const float* t, const float* w, size_t size) {
    return std::make_pair(std::vector<double>(t, t + size), std::vector<double>(w, w + size));
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
            x.push_back(x_val); // ns
            y.push_back(y_val); // mV
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

void PSDataProcessor::BackgroundSubtraction(TGraph* gr) const {
    if (cursorLeft > 0 && cursorRight > 0) {
        auto bkgd = new TH1D("bkgd", "bkgd", 100, gr->GetMinimum(), gr->GetMaximum());
        for (int i = 0; i < gr->GetN(); i++) {
            double x, y;
            gr->GetPoint(i, x, y);
            if (x < cursorLeft || x > cursorRight) bkgd->Fill(y);
        }
        double mean = bkgd->GetMean();
        delete bkgd;

        for (int i = 0; i < gr->GetN(); i++) {
            double x, y;
            gr->GetPoint(i, x, y);
            gr->SetPoint(i, x, y - mean);
        }
    } else {
        auto bkgd = new TH1D("bkgd", "bkgd", 100, gr->GetMinimum(), gr->GetMaximum());
        for (int i = 0; i < gr->GetN(); i++) {
            double x, y;
            gr->GetPoint(i, x, y);
            bkgd->Fill(y);
        }

        double mean = bkgd->GetMean();
        double stdDev = bkgd->GetStdDev();
        delete bkgd;

        auto bkgd_refined = new TH1D("bkgd_refined", "bkgd_refined", 100, gr->GetMinimum(), gr->GetMaximum());
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
}

double PSDataProcessor::IntegrateTGraph(TGraph* gr) {
    if (cursorLeft > 0 && cursorRight > 0) {
        int cursorLeftIndex = gr->GetXaxis()->FindBin(cursorLeft);
        int cursorRightIndex = gr->GetXaxis()->FindBin(cursorRight);
        return gr->Integral(cursorLeftIndex, cursorRightIndex);
    } else {
        return gr->Integral();
    }
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

void PSDataProcessor::SaveWaveformToCSV(const Waveform_t& wf, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    // Write header
    file << "Time,Voltage\n";
    file << "(ns),(mV)\n";
    file << "\n";

    // Write data
    const auto& times = wf.first;
    const auto& voltages = wf.second;
    for (size_t i = 0; i < times.size(); ++i) {
        file << times[i] << "," << voltages[i] << "\n";
    }

    file.close();
}