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
  int counter = 0;
  while (file >> line) {
    std::istringstream ss(line);
    double x_val, y_val;
    std::string x_str, y_str;
    if (std::getline(ss, x_str, ',') && std::getline(ss, y_str, ',')) {
      double x_val = std::stod(x_str);
      double y_val = std::stod(y_str);
      // if (x_val > 0.5E-6 && x_val < 0.52E-6) std::cout << "lecroy: " << counter << std::endl;
      x.push_back(x_val);
      y.push_back(y_val);
    }
    counter++;
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
  int counter = 0;
  while (file >> line) {
    std::istringstream ss(line);
    std::string x_str, y_str;
    if (std::getline(ss, x_str, ',') && std::getline(ss, y_str, ',')) {
      double x_val = std::stod(x_str);
      double y_val = std::stod(y_str);
      // if (x_val > 500 && x_val < 510) std::cout << "picoscope: " << counter << std::endl;
      x.push_back(x_val * 1E-9); // ns
      y.push_back(y_val * 1E-3); // mV
    }
    counter++;
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

void BackgroundSubtraction(TGraph* gr, int idx) {
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

void PlotLeCroy(bool bkgdSub = false, int idx = 1000) {
  int shotID = 120;

  for (shotID = 75; shotID < 124; shotID++) {
    std::string filename = Form("C2--20250114--%05i.txt", shotID);
    auto wf_01 = ReadSingleWaveformLeCroy(LeCroyDataDir + filename);
    auto gr_01 = WaveformToTGraph(wf_01);
    if (bkgdSub) BackgroundSubtraction(gr_01, idx);

    auto c = new TCanvas("c", "c", 800, 600);
    gStyle->SetOptStat(0);
    gr_01->Draw("APL");
    c->SaveAs(Form("test/lecroy/test_%05i.png", shotID));

    delete gr_01;
    delete c;
  }
}

void PlotPicoScope(bool bkgdSub = false, int idx = 1000) {
  int shotID = 0;

  for (shotID = 1; shotID < 53; shotID++) {
    std::string filename = Form("20250114_%02i.csv", shotID);
    auto wf_01 = ReadSingleWaveformPicoScope(PicoScopeDataDir + filename);
    auto gr_01 = WaveformToTGraph(wf_01);
    if (bkgdSub) BackgroundSubtraction(gr_01, idx);

    auto c = new TCanvas("c", "c", 800, 600);
    gStyle->SetOptStat(0);
    gr_01->Draw("APL");
    c->SaveAs(Form("test/picoscope/test_%05i.png", shotID));

    delete gr_01;
    delete c;
  }
}

double IntegrateTGraph(TGraph* gr, int idx_min) {
  return gr->Integral(idx_min, -1);
}

int main() {
  // PlotLeCroy(true, 1750);
  // PlotPicoScope(true, 1200);

  int integralMarkerLeCroy = 1752;
  int integralMarkerPicoScope = 1202;
  int nbins = 40;
  int min_charge = 0;
  int max_charge = 20;

  auto h_LeCroy = new TH1D("h_LeCroy", "h_LeCroy", nbins, min_charge, max_charge);
  for (int shotID = 75; shotID < 124; shotID++) {
    std::string filename = Form("C2--20250114--%05i.txt", shotID);
    auto wf_01 = ReadSingleWaveformLeCroy(LeCroyDataDir + filename);
    auto gr_01 = WaveformToTGraph(wf_01);
    BackgroundSubtraction(gr_01, integralMarkerLeCroy - 1);
    double integral = IntegrateTGraph(gr_01, integralMarkerLeCroy);
    h_LeCroy->Fill(integral * 1E12 / 50);
    std::cout << "LeCroy: " << shotID << " " << integral * 1E9 << std::endl;
    delete gr_01;
  }

  auto h_PicoScope = new TH1D("h_PicoScope", "h_PicoScope", nbins, min_charge, max_charge);
  for (int shotID = 1; shotID < 53; shotID++) {
    std::string filename = Form("20250114_%02i.csv", shotID);
    auto wf_01 = ReadSingleWaveformPicoScope(PicoScopeDataDir + filename);
    auto gr_01 = WaveformToTGraph(wf_01);
    BackgroundSubtraction(gr_01, integralMarkerPicoScope - 1);
    double integral = IntegrateTGraph(gr_01, integralMarkerPicoScope);
    h_PicoScope->Fill(integral * 1E12 / 50);
    std::cout << "PicoScope: " << shotID << " " << integral * 1E9 << std::endl;
    delete gr_01;
  }

  gStyle->SetOptStat(0);
  auto c = new TCanvas("c", "c", 800, 800);
  h_LeCroy->SetLineColor(kRed);
  h_LeCroy->SetFillColorAlpha(kRed, 0.5);
  h_LeCroy->SetTitle("Data Collected on 2025-01-14");
  h_LeCroy->GetXaxis()->SetTitle("Charge (pC)");
  h_LeCroy->GetYaxis()->SetTitle("Counts");
  h_LeCroy->GetXaxis()->CenterTitle();
  h_LeCroy->GetYaxis()->CenterTitle();
  h_LeCroy->Draw();
  h_PicoScope->SetLineColor(kBlue);
  h_PicoScope->SetFillColorAlpha(kBlue, 0.5);
  h_PicoScope->Draw("SAME");

  auto leg = new TLegend(0.45, 0.65, 0.85, 0.85);
  leg->AddEntry(h_LeCroy, Form("LeCroy: %1.2f #pm %1.2f pC", h_LeCroy->GetMean(), h_LeCroy->GetStdDev()), "f");
  leg->AddEntry(h_PicoScope, Form("PicoScope: %1.2f  #pm %1.2f pC", h_PicoScope->GetMean(), h_PicoScope->GetStdDev()), "f");
  leg->SetBorderSize(0);
  leg->Draw();

  c->SaveAs("test/hist.png");

  return 0;
}