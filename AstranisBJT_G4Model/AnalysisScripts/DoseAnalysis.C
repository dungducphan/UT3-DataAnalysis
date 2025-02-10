#include <TH2.h>
#include <TROOT.h>
#include <TTree.h>
#include <TChain.h>
#include <TSystem.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TMath.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>

std::string voxelDataPath_All_1E7 = "/home/dphan/Documents/GitHub/CarlsbadRadShield/AnalysisScripts/doseDepAll_1E7.csv";
std::string voxelDataPath_All_2E7 = "/home/dphan/Documents/GitHub/CarlsbadRadShield/AnalysisScripts/doseDepAll_2E7.csv";
double numberOfElectrons = 3E7;
double rescaleFactorTo100pC = (100E-12 / 1.6E-19) / numberOfElectrons;
double rep_rate = 100;
double seconds_in_a_business_year = 3600 * 8 * 250;
double gy_to_rem = 100;
double totalFactor = rescaleFactorTo100pC * rep_rate * seconds_in_a_business_year * gy_to_rem;


using voxelData = std::tuple<int, int, int, double, double, int>;

std::vector<voxelData> ReadVoxels(const std::string& filename) {
    std::vector<voxelData> voxels;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return voxels;
    }

    // Ignore headers
    std::string line;
    file >> line;
    file >> line;
    file >> line;
    file >> line;

    // Read data
    while (file >> line) {
        std::istringstream ss(line);
        std::string str_x, str_y, str_z, str_dose, str_doseSSq, str_nofEntries;
        if (std::getline(ss, str_x, ',') &&
            std::getline(ss, str_y, ',') &&
            std::getline(ss, str_z, ',') &&
            std::getline(ss, str_dose, ',') &&
            std::getline(ss, str_doseSSq, ',')&&
            std::getline(ss, str_nofEntries, ',')) {
            voxels.push_back(
                std::make_tuple(std::stoi(str_x),
                                std::stoi(str_y),
                                std::stoi(str_z),
                                std::stod(str_dose),
                                std::stod(str_doseSSq),
                                std::stoi(str_nofEntries))
            );
        }
    }
    file.close();

    return voxels;
}

double MaxDose(const std::vector<voxelData>& data) {
    double maxDose = 0;
    int maxDoseIdx = 0;
    for (int i = 0; i < data.size(); i++) {
        if (std::get<3>(data[i]) > maxDose) {
            maxDose = std::get<3>(data[i]);
            maxDoseIdx = i;
        }
    }
    std::cout << "Max dose: " << maxDose << " Gy." << std::endl;
    std::cout << "Max dose voxel: (" << std::get<0>(data[maxDoseIdx]) << ", " << std::get<1>(data[maxDoseIdx]) << ", " << std::get<2>(data[maxDoseIdx]) << ")." << std::endl;
    return maxDose;
}

void DoseSliceXZ(const std::vector<voxelData>& data, int y_Idx = 19) {
    auto h = new TH2D("h", Form("Dose Slice (Y = %02i)", y_Idx), 40, 0, 40, 20, 0, 20);
    h->GetXaxis()->SetTitle("Z");
    h->GetYaxis()->SetTitle("X");
    for (int i = 0; i < data.size(); i++) {
        if (std::get<1>(data[i]) == y_Idx) {
            int Z = std::get<2>(data[i]);
            int X = std::get<0>(data[i]);
            double dose = std::get<3>(data[i]);
            h->Fill(0.5 + (double) Z, 0.5 + (double) X, dose * totalFactor);
        }
    }

    auto c = new TCanvas("c", "c", 1600, 800);
    c->SetMargin(0.1, 0.15, 0.15, 0.15);
    h->Draw("colz");
    h->GetZaxis()->SetTitle("Dose (rem)");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetZaxis()->CenterTitle();
    c->SaveAs(Form("DoseSliceXZ_Y_%02i.png", y_Idx));

    delete h;
    delete c;
}

void DoseSliceYZ(const std::vector<voxelData>& data, int x_Idx = 0) {
    auto h = new TH2D("h", Form("Dose Slice (X = %02i)", x_Idx), 40, 0, 40, 20, 0, 20);
    h->GetXaxis()->SetTitle("Z");
    h->GetYaxis()->SetTitle("Y");
    for (int i = 0; i < data.size(); i++) {
        if (std::get<0>(data[i]) == x_Idx) {
            int Z = std::get<2>(data[i]);
            int Y = std::get<1>(data[i]);
            double dose = std::get<3>(data[i]);
            h->Fill(0.5 + (double) Z, 0.5 + (double) Y, dose * totalFactor);
        }
    }

    auto c = new TCanvas("c", "c", 1600, 800);
    c->SetMargin(0.1, 0.15, 0.15, 0.15);
    h->Draw("colz");
    h->GetZaxis()->SetTitle("Dose (rem)");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetZaxis()->CenterTitle();
    c->SaveAs(Form("DoseSliceYZ_X_%02i.png", x_Idx));

    delete h;
    delete c;
}

void DoseSliceXY(const std::vector<voxelData>& data, int z_Idx = 0) {
    auto h = new TH2D("h", Form("Dose Slice (Z = %02i)", z_Idx), 20, 0, 20, 20, 0, 20);
    h->GetXaxis()->SetTitle("X");
    h->GetYaxis()->SetTitle("Y");
    for (int i = 0; i < data.size(); i++) {
        if (std::get<2>(data[i]) == z_Idx) {
            int Y = std::get<1>(data[i]);
            int X = std::get<0>(data[i]);
            double dose = std::get<3>(data[i]);
            h->Fill(0.5 + (double) X, 0.5 + (double) Y, dose * totalFactor);
        }
    }

    auto c = new TCanvas("c", "c", 800, 800);
    c->SetMargin(0.2, 0.2, 0.2, 0.2);
    h->Draw("colz");
    h->GetZaxis()->SetTitle("Dose (rem)");
    h->GetXaxis()->SetTitleOffset(1.5);
    h->GetYaxis()->SetTitleOffset(1.5);
    h->GetZaxis()->SetTitleOffset(2);
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetZaxis()->CenterTitle();
    c->SaveAs(Form("DoseSliceXY_Z_%02i.png", z_Idx));

    delete h;
    delete c;
}

std::vector<voxelData> AggregateDose(const std::vector<voxelData>& sample_a, const std::vector<voxelData>& sample_b) {
    std::vector<voxelData> aggregated;
    for (int i = 0; i < sample_a.size(); i++) {
        aggregated.push_back(
            std::make_tuple(
                            std::get<0>(sample_a[i]),
                            std::get<1>(sample_a[i]),
                            std::get<2>(sample_a[i]),
                            std::get<3>(sample_a[i]) + std::get<3>(sample_b[i]),
                            std::get<4>(sample_a[i]) + std::get<4>(sample_b[i]),
                            std::get<5>(sample_a[i]) + std::get<5>(sample_b[i])
            )
        );
    }
    return aggregated;
}

void DoseComparison(const std::vector<voxelData>& sample_a, const std::vector<voxelData>& sample_b) {
    auto h = new TH1D("h", "Dose Ratio", 300, 0, 15);

    for (int i = 0; i < sample_a.size(); i++) {
        double dose_a = std::get<3>(sample_a[i]);
        double dose_b = std::get<3>(sample_b[i]);
        double normedDiff = 2 * (dose_a - dose_b) / (dose_a + dose_b);
        h->Fill(normedDiff);
        std::cout << "Dose A: " << dose_a << " Gy." << std::endl;
        std::cout << "Dose B: " << dose_b << " Gy." << std::endl;
    }

    auto c = new TCanvas("c", "c", 800, 800);
    c->SetLogy();
    h->Draw();
    c->SaveAs("DoseComparison.png");

    delete h;
    delete c;
}

#if !defined(__CINT__) && !defined(__CLING__) && !defined(__ACLIC__)

int main() {
    gStyle->SetOptStat(0);

    auto data_All_1E7 = ReadVoxels(voxelDataPath_All_1E7);
    auto data_All_2E7 = ReadVoxels(voxelDataPath_All_2E7);
    auto data_All_Total = AggregateDose(data_All_1E7, data_All_2E7);

    DoseSliceYZ(data_All_Total, 0);
    DoseSliceYZ(data_All_Total, 19);
    DoseSliceXZ(data_All_Total, 0);
    DoseSliceXZ(data_All_Total, 19);
    DoseSliceXY(data_All_Total, 0);
    DoseSliceXY(data_All_Total, 39);

    return 0;
}
#endif