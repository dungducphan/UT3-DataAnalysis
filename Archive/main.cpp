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
#include <TMultiGraph.h>
#include <TGaxis.h>

std::vector<std::string> osclist;
std::vector<std::string> pointlist;
const double ICTCalib = 4.935E-9;

class FIRFilter {
public:
    // Constructor to initialize filter coefficients
    FIRFilter(const std::vector<double>& coefficients)
        : coeffs(coefficients) {}

    // Apply the filter to a single sample
    double filter(double sample) {
        delayLine.insert(delayLine.begin(), sample); // Add new sample at beginning
        if (delayLine.size() > coeffs.size()) {
            delayLine.pop_back(); // Remove oldest sample
        }

        double output = 0.0;
        for (size_t i = 0; i < delayLine.size(); ++i) {
            output += delayLine[i] * coeffs[i];
        }
        return output;
    }

private:
    std::vector<double> coeffs; // Filter coefficients
    std::vector<double> delayLine; // Delay line
};

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

    std::vector<double> coeffs =
        {
        0.000000000000000000,
        -0.000011466433343440,
        -0.000048159680438716,
        -0.000070951498745996,
        0.000000000000000000,
        0.000226394896924650,
        0.000570593070542985,
        0.000843882357908127,
        0.000742644459879189,
        -0.000000000000000001,
        -0.001387330856962112,
        -0.002974017320060804,
        -0.003876072294410999,
        -0.003078546062261788,
        0.000000000000000002,
        0.004911281747189231,
        0.009897689392343489,
        0.012239432700612913,
        0.009302643950572093,
        -0.000000000000000005,
        -0.013947257358995015,
        -0.027649479941983943,
        -0.034045985830080311,
        -0.026173578588735643,
        0.000000000000000007,
        0.043288592274982135,
        0.096612134128292462,
        0.148460098539443669,
        0.186178664190551207,
        0.199977588313552862,
        0.186178664190551235,
        0.148460098539443669,
        0.096612134128292462,
        0.043288592274982128,
        0.000000000000000007,
        -0.026173578588735653,
        -0.034045985830080325,
        -0.027649479941983936,
        -0.013947257358995019,
        -0.000000000000000005,
        0.009302643950572094,
        0.012239432700612920,
        0.009897689392343489,
        0.004911281747189235,
        0.000000000000000002,
        -0.003078546062261790,
        -0.003876072294411004,
        -0.002974017320060808,
        -0.001387330856962112,
        -0.000000000000000001,
        0.000742644459879189,
        0.000843882357908127,
        0.000570593070542984,
        0.000226394896924650,
        0.000000000000000000,
        -0.000070951498745996,
        -0.000048159680438716,
        -0.000011466433343440,
        0.000000000000000000
        };
    FIRFilter filter(coeffs);

    for (int i = 0; i < gr->GetN(); i++) {
        double x, y;
        gr->GetPoint(i, x, y);
        gr->SetPoint(i, x, filter.filter(y));
    }

    return gr;
}

void PlotOscData(const int& id) {
    auto gr = GetOscData(id);
    auto gr_filtered = GetOscData(id);
        std::vector<double> coeffs =
        {
        0.000000000000000000,
        -0.000011466433343440,
        -0.000048159680438716,
        -0.000070951498745996,
        0.000000000000000000,
        0.000226394896924650,
        0.000570593070542985,
        0.000843882357908127,
        0.000742644459879189,
        -0.000000000000000001,
        -0.001387330856962112,
        -0.002974017320060804,
        -0.003876072294410999,
        -0.003078546062261788,
        0.000000000000000002,
        0.004911281747189231,
        0.009897689392343489,
        0.012239432700612913,
        0.009302643950572093,
        -0.000000000000000005,
        -0.013947257358995015,
        -0.027649479941983943,
        -0.034045985830080311,
        -0.026173578588735643,
        0.000000000000000007,
        0.043288592274982135,
        0.096612134128292462,
        0.148460098539443669,
        0.186178664190551207,
        0.199977588313552862,
        0.186178664190551235,
        0.148460098539443669,
        0.096612134128292462,
        0.043288592274982128,
        0.000000000000000007,
        -0.026173578588735653,
        -0.034045985830080325,
        -0.027649479941983936,
        -0.013947257358995019,
        -0.000000000000000005,
        0.009302643950572094,
        0.012239432700612920,
        0.009897689392343489,
        0.004911281747189235,
        0.000000000000000002,
        -0.003078546062261790,
        -0.003876072294411004,
        -0.002974017320060808,
        -0.001387330856962112,
        -0.000000000000000001,
        0.000742644459879189,
        0.000843882357908127,
        0.000570593070542984,
        0.000226394896924650,
        0.000000000000000000,
        -0.000070951498745996,
        -0.000048159680438716,
        -0.000011466433343440,
        0.000000000000000000
        };
    FIRFilter filter(coeffs);

    for (int i = 0; i < gr_filtered->GetN(); i++) {
        double x, y;
        gr_filtered->GetPoint(i, x, y);
        gr_filtered->SetPoint(i, x, filter.filter(y));
    }

    if (gr != nullptr) {
        auto c = new TCanvas("c", "c", 1600, 800);
        gr->Draw("AL");
        gr_filtered->SetLineColor(kRed);
        gr_filtered->Draw("SAME L");
        c->SaveAs(("/home/dphan/Documents/GitHub/UT3-DataAnalysis/Plot/Osc/" + osclist[id] + ".png").c_str());
        delete c;
    }
}

void PlotBeamChargeDist(const std::vector<double>& EBeamCharge) {
    gStyle->SetOptStat(0);

    auto h = new TH1D("h", "h", 30, 9.5, 39.5);
    for (auto& elem : EBeamCharge) {
        h->Fill(elem);
    }

    auto c = new TCanvas("c", "c", 1800, 1800);
    h->SetLineColor(kRed);
    h->SetFillColorAlpha(kRed, 0.4);
    h->SetLineWidth(3);
    h->GetXaxis()->SetTitle("Beam Charge (pC)");
    h->GetYaxis()->SetTitle("Counts");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetXaxis()->SetNdivisions(505);
    h->GetYaxis()->SetNdivisions(505);
    h->GetYaxis()->SetRangeUser(0, 15);
    h->GetXaxis()->SetLimits(10, 40);
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

std::vector<std::tuple<double, double, double>> GetEnergyStability() {
    std::vector<std::tuple<double, double, double>> energyStability;

    energyStability.push_back(std::make_tuple(95, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(101, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(79, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(100, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(91, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(89, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(96, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(98, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(85, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(81, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(73, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(103, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(96, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(98, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(91, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(104, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(94, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(99, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(99, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(106, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(86, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(94, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(91, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(107, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(70, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(88, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(100, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(105, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(85, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(98, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(95, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(101, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(98, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(91, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(97, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(101, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(88, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(107, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(89, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(112, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(105, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(99, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(79, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(100, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(105, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(104, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(97, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(91, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(90, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(107, 0.0, 0.0));
    energyStability.push_back(std::make_tuple(89, 0.0, 0.0));

    std::cout << "Energy Stability Size: " << energyStability.size() << std::endl;
    return energyStability;
}

void PlotChargeAndEnergy(const std::vector<double>& charges, const std::vector<std::tuple<double, double, double>>& energy) {
    auto gr_charge = new TGraph();
    auto gr_energy = new TGraph();

    std::ofstream fileout("/home/dphan/Documents/GitHub/UT3-DataAnalysis/Plot/ChargeEnergy.txt");
    fileout << "BeamCharge(pC),PeakEnergy(MeV)" << std::endl;
    for (int i = 0; i < charges.size(); i++) {
        std::cout << charges[i] << " " << std::get<0>(energy[i]) << std::endl;
        gr_charge->SetPoint(gr_charge->GetN(), i, charges[i]);
        gr_energy->SetPoint(gr_energy->GetN(), i, std::get<0>(energy[i]));
        fileout << charges[i] << "," << std::get<0>(energy[i]) << std::endl;
    }
    fileout.close();
    gr_charge->SetMarkerStyle(20);
    gr_charge->SetMarkerSize(3);
    gr_charge->SetMarkerColor(kRed);
    gr_energy->SetMarkerStyle(20);
    gr_energy->SetMarkerSize(3);
    gr_energy->SetMarkerColor(kBlue);

    auto c = new TCanvas("c", "c", 3600, 1800);
    gStyle->SetNdivisions(505, "XY");
    gr_energy->Draw("AP");
    gr_energy->GetXaxis()->SetTitle("Shot ID");
    gr_energy->GetYaxis()->SetTitle("Mean Beam Energy (MeV)");
    gr_energy->GetXaxis()->SetLimits(0, 50);
    gr_energy->GetYaxis()->SetRangeUser(0, 150);
    gr_energy->GetYaxis()->SetAxisColor(kBlue);
    gr_energy->GetYaxis()->SetLabelColor(kBlue);
    gr_energy->GetYaxis()->SetTitleColor(kBlue);
    gr_energy->GetYaxis()->SetTitleFont(62);
    gr_energy->GetYaxis()->SetLabelFont(62);
    gr_energy->GetYaxis()->CenterTitle();
    gr_energy->GetYaxis()->SetTitleOffset(1.2);
    gr_energy->GetYaxis()->SetLabelOffset(0.01);
    gr_energy->GetXaxis()->SetTitleFont(62);
    gr_energy->GetXaxis()->SetLabelFont(62);
    gr_energy->GetXaxis()->CenterTitle();
    gr_charge->Draw("P SAME");

    c->Update();

    auto charge_axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
         gPad->GetUxmax(), gPad->GetUymax(),0,150,505,"+L");
    charge_axis->SetLineColor(kRed);
    charge_axis->SetLabelColor(kRed);
    charge_axis->SetTitleColor(kRed);
    charge_axis->SetTitle("Beam Charge (pC)");
    charge_axis->CenterTitle();
    charge_axis->SetLabelOffset(0.01);
    charge_axis->SetTitleOffset(1.2);
    charge_axis->Draw();

    c->SaveAs("/home/dphan/Documents/GitHub/UT3-DataAnalysis/Plot/ChargeEnergy.png");
}

int main() {
    GetList(osclist, pointlist);

    std::vector<double> EBeamCharge;
    EBeamCharge.clear();
    for (int i = 0; i < osclist.size(); i++) {
        auto gr = GetOscData(i);
        EBeamCharge.push_back(gr->Integral() * 1000. / ICTCalib);
        // PlotOscData(i);
    }
    PlotBeamChargeDist(EBeamCharge);

    auto eData = GetEnergyStability();

    PlotChargeAndEnergy(EBeamCharge, eData);



    // std::vector<double> totalPixelValues;
    // for (int i = 0; i < pointlist.size(); i++) {
    //     auto pxVal = new TH1D(Form("pxVal_%05i", i), "pxVal", 656, 0, 65600);
    //     auto pxVal2D = new TH2D(Form("pxVal2D_%05i", i), "pxVal2D", 2048, -0.5, 2047.5, 1536, -0.5, 1535.5);
    //     double totalPixelValue = 0;
    //     GetHistsFromImage(i, pxVal, pxVal2D, totalPixelValue);
    //     totalPixelValues.push_back(totalPixelValue);
    //     delete pxVal;
    //     delete pxVal2D;
    // }
    //
    // auto corr = new TGraph();
    // for (int i = 0; i < totalPixelValues.size(); i++) {
    //     corr->SetPoint(corr->GetN(), EBeamCharge[i], totalPixelValues[i]);
    // }
    // auto c = new TCanvas("c", "c", 1600, 1600);
    // corr->SetMarkerStyle(20);
    // corr->SetMarkerSize(3);
    // corr->Draw("AP");
    // c->SaveAs("/home/dphan/Documents/GitHub/UT3-DataAnalysis/Plot/Correlation.png");

    return 0;
}
