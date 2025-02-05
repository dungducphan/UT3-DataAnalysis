#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>

#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TLegend.h"
#include "TGraph.h"

using waveform = std::pair<std::vector<double>, std::vector<double>>;

std::string LeCroyDataDir = "/home/dphan/Documents/GitHub/UT3-DataAnalysis/PicoScope_Recalib/20250114-LeCroy/";
std::string PicoScopeDataDir = "/home/dphan/Documents/GitHub/UT3-DataAnalysis/PicoScope_Recalib/20250114-PicoScope/";

waveform ReadSingleWaveformLeCroy(const std::string& filename) {
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
  file >> line;
  file >> line;

  // Read the data lines
  while (file >> line) {
    std::istringstream ss(line);
    double x_val, y_val;
    std::string x_str, y_str;
    if (std::getline(ss, x_str, ',') && std::getline(ss, y_str, ',')) {
      double x_val = std::stod(x_str);
      double y_val = std::stod(y_str);
      x.push_back(x_val);
      y.push_back(y_val);
    }
  }

  file.close();

  return std::make_pair(x, y);
}

waveform ReadSingleWaveformPicoScope(const std::string& filename) {
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
  while (file >> line) {
    std::cout << line << std::endl;
    std::istringstream ss(line);
    std::string x_str, y_str;
    if (std::getline(ss, x_str, ',') && std::getline(ss, y_str, ',')) {
      double x_val = std::stod(x_str);
      double y_val = std::stod(y_str);
      if (x_val > 500) {
        x.push_back(x_val * 1E-9); // ns
        y.push_back(y_val * 1E-3); // mV
      }
    }
  }

  file.close();

  return std::make_pair(x, y);
}

TGraph* WaveformToTGraph(const waveform& wf) {
  std::vector<double> x = wf.first;
  std::vector<double> y = wf.second;

  auto gr = new TGraph(x.size(), x.data(), y.data());
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(0.5);
  gr->SetMarkerColor(kBlack);
  gr->SetLineColor(kBlack);

  return gr;
}

int main() {
  std::string filename = "C2--20250114--00120.txt";
  auto wf_01 = ReadSingleWaveformLeCroy(LeCroyDataDir + filename);
  // std::string filename = "20250114_01.csv";
  // auto wf_01 = ReadSingleWaveformPicoScope(PicoScopeDataDir + filename);
  auto gr_01 = WaveformToTGraph(wf_01);

  auto c = new TCanvas("c", "c", 800, 600);
  gStyle->SetOptStat(0);
  gr_01->Draw("APL");
  c->SaveAs("test.png");

  return 0;
}