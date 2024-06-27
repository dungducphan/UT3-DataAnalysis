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
#include <TF2.h>
#include <TStyle.h>

std::vector<std::string> osclist;
std::vector<std::string> pointlist;
const double ICTCalib = 4.935E-9;

void GetList(std::vector<std::string> &osclist, std::vector<std::string> &pointlist) {
    osclist.clear();
    std::ifstream file("/home/dphan/Documents/GitHub/UT3-DataAnalysis/osc.csv");
    std::string line;
    while (file >> line) {
        osclist.push_back(line);
    }
    file.close();

    pointlist.clear();
    file = std::ifstream("/home/dphan/Documents/GitHub/UT3-DataAnalysis/point.csv");
    while (file >> line) {
        pointlist.push_back(line);
    }
    file.close();
}

TGraph* GetOscData(const int& id) {
    auto filename = osclist[id];

    std::ifstream file("/home/dphan/Documents/GitHub/UT3-DataAnalysis/Oscilloscope/" + filename);
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
        c->SaveAs(("/home/dphan/Documents/GitHub/UT3-DataAnalysis/Plot/Osc/" + osclist[id] + ".png").c_str());
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

    c->SaveAs("/home/dphan/Documents/GitHub/UT3-DataAnalysis/Plot/BeamChargeDist.png");
    delete c;
}

void GetHistsFromImage(const int& id, TH1D* pxVal, TH2D* pxVal2D, double& totalPixelValue) {
    totalPixelValue = 0;
    pxVal->Reset();
    pxVal2D->Reset();
    auto filename = pointlist[id];
    auto focusImage = TIFFOpen(("/home/dphan/Documents/GitHub/UT3-DataAnalysis/Point/" + filename).c_str(), "r");
    int NPixelX, NPixelY;
    if (focusImage) {
        // Get pixel data from the image
        TIFFGetField(focusImage, TIFFTAG_IMAGEWIDTH, &NPixelX);
        TIFFGetField(focusImage, TIFFTAG_IMAGELENGTH, &NPixelY);
        auto pixelArrayData = (uint32_t *) _TIFFmalloc(NPixelX * NPixelY * sizeof(uint32_t));
        if (pixelArrayData != nullptr) {
            if (TIFFReadRGBAImage(focusImage, NPixelX, NPixelY, pixelArrayData, 0)) {
                for (size_t i = 0; i < NPixelX * NPixelY; i++) {
                    double val = pixelArrayData[i] % 65536;
                    pxVal->Fill(val);
                    pxVal2D->Fill(i % NPixelX, i / NPixelX, val);
                    if (val > 10000) totalPixelValue += val;
                }
            }
        }
        TIFFClose(focusImage);
    }
}

void PlotPointHist1D(TH1D* pxVal, const int& id) {
    auto c = new TCanvas("c", "c", 3200, 1600);
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
    c->SaveAs(("/home/dphan/Documents/GitHub/UT3-DataAnalysis/Plot/PointHist1D/" + pointlist[id] + ".png").c_str());
    delete c;
}

void PlotPointHist2D(TH2D* pxVal2D, const int& id) {

    double range_x_min = 900;
    double range_x_max = 1150;
    double range_y_min = 700;
    double range_y_max = 900;
    double fit_x_mean = 1025;
    double fit_x_sigma = 50;
    double fit_y_mean = 775;
    double fit_y_sigma = 50;
    double fit_rho = 0.2;

    TF2 *f = new TF2("f", "[0] * ROOT::Math::bigaussian_pdf(x, y, [2], [4], [5], [1], [3])", range_x_min, range_x_max,
                     range_y_min, range_y_max);
    f->SetParameters(1, fit_x_mean, fit_x_sigma, fit_y_mean, fit_y_sigma, fit_rho);
    f->SetContour(6);
    f->SetNpy(1000);
    f->SetNpx(1000);
    pxVal2D->Fit(f, "R");

    auto c = new TCanvas("c", "c", 2048*2, 1536*2);
    pxVal2D->GetXaxis()->SetTitle("");
    pxVal2D->GetYaxis()->SetTitle("");
    pxVal2D->GetXaxis()->SetNdivisions(505);
    pxVal2D->GetYaxis()->SetNdivisions(505);
    pxVal2D->SetTitle("");
    pxVal2D->Draw("COLZ");
    c->SaveAs(("/home/dphan/Documents/GitHub/UT3-DataAnalysis/Plot/PointHist2D/" + pointlist[id] + ".png").c_str());
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

    std::vector<double> totalPixelValues;
    for (int i = 0; i < pointlist.size(); i++) {
        auto pxVal = new TH1D(Form("pxVal_%05i", i), "pxVal", 656, 0, 65600);
        auto pxVal2D = new TH2D(Form("pxVal2D_%05i", i), "pxVal2D", 2048, -0.5, 2047.5, 1536, -0.5, 1535.5);
        double totalPixelValue = 0;
        GetHistsFromImage(i, pxVal, pxVal2D, totalPixelValue);
        totalPixelValues.push_back(totalPixelValue);
        delete pxVal;
        delete pxVal2D;
    }

    auto corr = new TGraph();
    for (int i = 0; i < totalPixelValues.size(); i++) {
        corr->SetPoint(corr->GetN(), EBeamCharge[i], totalPixelValues[i]);
    }
    auto c = new TCanvas("c", "c", 1600, 1600);
    corr->SetMarkerStyle(20);
    corr->SetMarkerSize(3);
    corr->Draw("AP");
    c->SaveAs("/home/dphan/Documents/GitHub/UT3-DataAnalysis/Plot/Correlation.png");

    return 0;
}
