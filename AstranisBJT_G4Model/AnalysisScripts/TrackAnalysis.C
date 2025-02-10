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

std::string voxelDataPath_All          = "/home/dphan/Documents/GitHub/CarlsbadRadShield/AnalysisScripts/nOfTrackAll.csv";
std::string voxelDataPath_Neutrino     = "/home/dphan/Documents/GitHub/CarlsbadRadShield/AnalysisScripts/nOfTrackNeutrino.csv";
std::string voxelDataPath_AntiNeutrino = "/home/dphan/Documents/GitHub/CarlsbadRadShield/AnalysisScripts/nOfTrackAntiNeutrino.csv";
std::string voxelDataPath_Gamma        = "/home/dphan/Documents/GitHub/CarlsbadRadShield/AnalysisScripts/nOfTrackGamma.csv";
std::string voxelDataPath_Neutron      = "/home/dphan/Documents/GitHub/CarlsbadRadShield/AnalysisScripts/nOfTrackNeutron.csv";

const int nBinX = 20;
const int nBinY = 20;
const int nBinZ = 40;

using voxelData = std::tuple<int, int, int, int, int, int>;

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
        std::string str_x, str_y, str_z, str_nTrack, str_nTrackSSq, str_nofEntries;
        if (std::getline(ss, str_x, ',') &&
            std::getline(ss, str_y, ',') &&
            std::getline(ss, str_z, ',') &&
            std::getline(ss, str_nTrack, ',') &&
            std::getline(ss, str_nTrackSSq, ',') &&
            std::getline(ss, str_nofEntries, ',')) {
            voxels.push_back(
                std::make_tuple(std::stoi(str_x),
                                  std::stoi(str_y),
                                  std::stoi(str_z),
                                  (int) std::stod(str_nTrack),
                                  (int) std::stod(str_nTrackSSq),
                                  std::stoi(str_nofEntries))
            );
        }
    }
    file.close();
    return voxels;
}

void TrackSliceXZ(const std::vector<voxelData>& data, int y_Idx = nBinY - 1) {
    auto h = new TH2D("h", Form("Track Slice (Y = %02i)", y_Idx), nBinZ, 0, nBinZ, nBinX, 0, nBinX);
    h->GetXaxis()->SetTitle("Z");
    h->GetYaxis()->SetTitle("X");
    for (int i = 0; i < data.size(); i++) {
        if (std::get<1>(data[i]) == y_Idx) {
            int Z = std::get<2>(data[i]);
            int X = std::get<0>(data[i]);
            int nTrack = std::get<3>(data[i]);
            h->Fill(0.5 + (double) Z, 0.5 + (double) X, nTrack);
        }
    }

    auto c = new TCanvas("c", "c", 1600, 800);
    c->SetMargin(0.1, 0.15, 0.15, 0.15);
    h->Draw("colz");
    h->GetZaxis()->SetTitle("No. of Tracks");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetZaxis()->CenterTitle();
    c->SaveAs(Form("TrackSliceXZ_Y_%02i.png", y_Idx));

    delete h;
    delete c;
}

void TrackSliceYZ(const std::vector<voxelData>& data, int x_Idx = 0) {
    auto h = new TH2D("h", Form("Track Slice (X = %02i)", x_Idx), nBinZ, 0, nBinZ, nBinY, 0, nBinY);
    h->GetXaxis()->SetTitle("Z");
    h->GetYaxis()->SetTitle("Y");
    for (int i = 0; i < data.size(); i++) {
        if (std::get<0>(data[i]) == x_Idx) {
            int Z = std::get<2>(data[i]);
            int Y = std::get<1>(data[i]);
            int nTrack = std::get<3>(data[i]);
            h->Fill(0.5 + (double) Z, 0.5 + (double) Y, nTrack);
        }
    }

    auto c = new TCanvas("c", "c", 1600, 800);
    c->SetMargin(0.1, 0.15, 0.15, 0.15);
    h->Draw("colz");
    h->GetZaxis()->SetTitle("No. of Tracks");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetZaxis()->CenterTitle();
    c->SaveAs(Form("TrackSliceYZ_X_%02i.png", x_Idx));

    delete h;
    delete c;
}

void TrackSliceXY(const std::vector<voxelData>& data, int z_Idx = 0) {
    auto h = new TH2D("h", Form("Track Slice (Z = %02i)", z_Idx), nBinX, 0, nBinX, nBinY, 0, nBinY);
    h->GetXaxis()->SetTitle("X");
    h->GetYaxis()->SetTitle("Y");
    for (int i = 0; i < data.size(); i++) {
        if (std::get<2>(data[i]) == z_Idx) {
            int Y = std::get<1>(data[i]);
            int X = std::get<0>(data[i]);
            int nTrack = std::get<3>(data[i]);
            h->Fill(0.5 + (double) X, 0.5 + (double) Y, nTrack);
        }
    }

    auto c = new TCanvas("c", "c", 800, 800);
    c->SetMargin(0.2, 0.2, 0.2, 0.2);
    h->Draw("colz");
    h->GetZaxis()->SetTitle("No. of Tracks");
    h->GetXaxis()->SetTitleOffset(1.5);
    h->GetYaxis()->SetTitleOffset(1.5);
    h->GetZaxis()->SetTitleOffset(2);
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetZaxis()->CenterTitle();
    c->SaveAs(Form("TrackSliceXY_Z_%02i.png", z_Idx));

    delete h;
    delete c;
}

std::vector<voxelData> AggregateTrack(const std::vector<voxelData>& sample_a, const std::vector<voxelData>& sample_b) {
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

void TrackComparison(const std::vector<voxelData>& sample_a, const std::vector<voxelData>& sample_b) {
    auto h = new TH1D("h", "Track Ratio", 300, 0, 1);

    for (int i = 0; i < sample_a.size(); i++) {
        if (std::get<0>(sample_a[i]) != 0) {
            continue;
        }
        double nTrack_a = std::get<3>(sample_a[i]);
        double nTrack_b = std::get<3>(sample_b[i]);
        // double normedDiff = 2 * (nTrack_a - nTrack_b) / (nTrack_a + nTrack_b);
        double normedDiff = nTrack_a / nTrack_b;
        h->Fill(normedDiff);
    }

    auto c = new TCanvas("c", "c", 800, 800);
    c->SetLogy();
    h->Draw();
    c->SaveAs("TrackComparison.png");

    delete h;
    delete c;
}

#if !defined(__CINT__) && !defined(__CLING__) && !defined(__ACLIC__)

int main() {
    gStyle->SetOptStat(0);

    auto data_All = ReadVoxels(voxelDataPath_All);
    auto data_Neutrino = ReadVoxels(voxelDataPath_Neutrino);
    auto data_AntiNeutrino = ReadVoxels(voxelDataPath_AntiNeutrino);
    auto data_Gamma = ReadVoxels(voxelDataPath_Gamma);
    auto data_Neutron = ReadVoxels(voxelDataPath_Neutron);
    auto data_GammaNeutron = AggregateTrack(data_Gamma, data_Neutron);
    auto data_Nue = AggregateTrack(data_Neutrino, data_AntiNeutrino);
    auto data_All_Recon = AggregateTrack(data_GammaNeutron, data_Nue);
    TrackComparison(data_Nue, data_All);

    return 0;
}
#endif