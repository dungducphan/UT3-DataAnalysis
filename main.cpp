#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <tiffio.h>

#include <TGraph.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>

std::vector<std::string> osclist;
std::vector<std::string> pointlist;
const double ICTCalib = 4.935E-9;

void GetList(std::vector<std::string> &osclist, std::vector<std::string> &pointlist) {
    osclist.clear();
    std::ifstream file("/home/dphan/Documents/GitHub/UT3-20240625/osc.csv");
    std::string line;
    while (file >> line) {
        osclist.push_back(line);
    }
    file.close();

    pointlist.clear();
    file = std::ifstream("/home/dphan/Documents/GitHub/UT3-20240625/point.csv");
    while (file >> line) {
        pointlist.push_back(line);
    }
    file.close();
}

TGraph* GetOscData(const int& id) {
    auto filename = osclist[id];

    std::ifstream file("/home/dphan/Documents/GitHub/UT3-20240625/Oscilloscope/" + filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file" << std::endl;
        return nullptr;
    }

    auto gr = new TGraph();

    std::string linedata;
    file >> linedata;
    file >> linedata;
    file >> linedata;
    file >> linedata;
    file >> linedata;
    file >> linedata;
    while (file >> linedata) {
        std::stringstream ss(linedata);
        std::string token;
        std::getline(ss, token, ',');
        double first = std::stod(token);
        std::getline(ss, token, ',');
        double second = std::stod(token);
        gr->SetPoint(gr->GetN(), first, second);
    }
    file.close();

    return gr;
}

void PlotOscData(const int& id) {
    auto gr = GetOscData(id);
    if (gr != nullptr) {
        auto c = new TCanvas("c", "c", 1600, 800);
        gr->Draw("ALP");
        c->SaveAs(("/home/dphan/Documents/GitHub/UT3-20240625/Plot/Osc/" + osclist[id] + ".png").c_str());
        delete c;
    }
}

void PlotBeamChargeDist(const std::vector<double>& EBeamCharge) {
    gStyle->SetOptStat(0);

    auto h = new TH1D("h", "h", 50, 0, 50);
    for (auto& elem : EBeamCharge) {
        h->Fill(elem);
    }

    auto c = new TCanvas("c", "c", 1600, 1600);
    h->SetLineColor(kRed);
    h->SetLineWidth(3);
    h->GetXaxis()->SetTitle("Beam Charge (pC)");
    h->GetYaxis()->SetTitle("Counts");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetXaxis()->SetNdivisions(505);
    h->GetYaxis()->SetNdivisions(505);
    h->GetYaxis()->SetRangeUser(0, 8);
    h->SetTitle("");
    h->Draw();

    auto leg = new TLegend(0.4, 0.7, 0.87, 0.87);
    leg->SetBorderSize(0);
    leg->AddEntry((TObject*)0, Form("Q = %.1f #pm %.1f (pC)", h->GetMean(), h->GetStdDev()), "");
    leg->Draw();

    c->SaveAs("/home/dphan/Documents/GitHub/UT3-20240625/Plot/BeamChargeDist.png");
    delete c;
}

void GetHistsFromImage(const int& id, TH1D* pxVal, TH2D* pxVal2D) {
    pxVal->Reset();
    pxVal2D->Reset();
    auto filename = pointlist[id];
    auto focusImage = TIFFOpen(("/home/dphan/Documents/GitHub/UT3-20240625/Point/" + filename).c_str(), "r");
    int NPixelX, NPixelY;
    if (focusImage) {
        // Get pixel data from the image
        TIFFGetField(focusImage, TIFFTAG_IMAGEWIDTH, &NPixelX);
        TIFFGetField(focusImage, TIFFTAG_IMAGELENGTH, &NPixelY);
        auto pixelArrayData = (uint32_t *) _TIFFmalloc(NPixelX * NPixelY * sizeof(uint32_t));
        if (pixelArrayData != nullptr) {
            if (TIFFReadRGBAImage(focusImage, NPixelX, NPixelY, pixelArrayData, 0)) {
                for (size_t i = 0; i < NPixelX * NPixelY; i++) {
                    pxVal->Fill(pixelArrayData[i] % 65536);
                    pxVal2D->Fill(i % NPixelX, i / NPixelX, pixelArrayData[i] % 65536);
                }
            }
        }
        TIFFClose(focusImage);
    }
}

void PlotPointHist1D(TH1D* pxVal, const int& id) {
    auto c = new TCanvas("c", "c", 1600, 1600);
    c->SetLogy();
    pxVal->SetLineColor(kRed);
    pxVal->SetLineWidth(3);
    pxVal->GetXaxis()->SetTitle("Pixel Value");
    pxVal->GetYaxis()->SetTitle("Counts");
    pxVal->GetXaxis()->CenterTitle();
    pxVal->GetYaxis()->CenterTitle();
    pxVal->GetXaxis()->SetNdivisions(505);
    pxVal->GetYaxis()->SetNdivisions(505);
    pxVal->SetTitle("");
    pxVal->Draw();
    c->SaveAs(("/home/dphan/Documents/GitHub/UT3-20240625/Plot/PointHist1D/" + pointlist[id] + ".png").c_str());
    delete c;
}

int main() {
    GetList(osclist, pointlist);

    std::vector<double> EBeamCharge;
    EBeamCharge.clear();
    for (int i = 0; i < osclist.size(); i++) {
        auto gr = GetOscData(i);
        EBeamCharge.push_back(gr->Integral() * 1000. / ICTCalib);
    }
    PlotBeamChargeDist(EBeamCharge);

    for (int i = 0; i < pointlist.size(); i++) {
        auto pxVal = new TH1D(Form("pxVal_%05i", i), "pxVal", 65536, 0, 65536);
        auto pxVal2D = new TH2D(Form("pxVal2D_%05i", i), "pxVal2D", 2048, -0.5, 2047.5, 1536, -0.5, 1535.5);
        GetHistsFromImage(i, pxVal, pxVal2D);
        PlotPointHist1D(pxVal, i);
        delete pxVal;
        delete pxVal2D;
    }

    return 0;
}
