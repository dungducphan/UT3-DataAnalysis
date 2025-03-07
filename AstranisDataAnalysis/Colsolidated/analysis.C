#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

double QToDoseFactor = 47E-3; // Gy/pC
double preExistingDose = 0; // Gy before 20250224
double GyTokrad = 0.1; // Gy to krad

using TUPLE_T = std::tuple<double, double, double, double>;

std::vector<TUPLE_T> ReadDataFromCSV(const std::string &filename) {
    std::vector<TUPLE_T> shotData;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return shotData;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::vector<double> values(4, 0.0);
        std::stringstream ss(line);
        std::string token;
        int index = 0;

        while (std::getline(ss, token, ',')) {
            if (!token.empty()) {
                values[index] = std::stod(token);
            }
            index++;
        }

        TUPLE_T aShot(values[0], values[1], values[2], values[3]);
        shotData.push_back(aShot);
    }

    file.close();
    return shotData;
}

std::vector<double> CalculateCumulativeDose(const std::vector<TUPLE_T> &data) {
    std::vector<double> cumulativeDose;
    double sum = preExistingDose * GyTokrad; // Gy
    for (const auto &shot : data) {
        double instantDose = std::get<3>(shot) * QToDoseFactor; // Gy

        // If instant dose is negative, set it to zero
        if (instantDose <= 0) {
            instantDose = 0;
        }

        sum += instantDose * GyTokrad; // krad
        cumulativeDose.push_back(sum);
    }

    return cumulativeDose;
}

std::vector<double> CalculateCollectorResponse(const std::vector<TUPLE_T> &data) {
    std::vector<double> collectorResponse;
    for (const auto &shot : data) {
        double collectorVoltage = std::get<0>(shot);
        double instantDose = std::get<3>(shot) * QToDoseFactor * GyTokrad; // krad
        if (collectorVoltage >= 0 || instantDose <= 0) {
            collectorResponse.push_back(0);
        } else {
            collectorResponse.push_back(1E-3 * TMath::Abs(collectorVoltage/instantDose));
        }
    }

    return collectorResponse;
}

void analysis() {
    auto shotData = ReadDataFromCSV("Sum_27_26_25_24_20_19.csv");
    auto cumulativeDose = CalculateCumulativeDose(shotData); // krad
    auto collectorResponse = CalculateCollectorResponse(shotData);

    std::cout << "Size of cumulative dose: " << cumulativeDose.size() << std::endl;
    std::cout << "Size of collector response: " << collectorResponse.size() << std::endl;

    auto c = new TCanvas("c", "c", 800, 600);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.15);
    c->SetBottomMargin(0.15);
    c->SetTopMargin(0.15);
    auto gr_CollectorResponse = new TGraph(cumulativeDose.size(), cumulativeDose.data(), collectorResponse.data());
    gr_CollectorResponse->SetTitle("");
    gr_CollectorResponse->GetXaxis()->SetTitle("Accumulative Dose (krad)");
    gr_CollectorResponse->GetYaxis()->SetTitle("Transistor Response (A.U.)");
    gr_CollectorResponse->GetXaxis()->CenterTitle();
    gr_CollectorResponse->GetYaxis()->CenterTitle();
    gr_CollectorResponse->GetXaxis()->SetTitleSize(0.05);
    gr_CollectorResponse->GetYaxis()->SetTitleSize(0.05);
    gr_CollectorResponse->GetXaxis()->SetLabelSize(0.05);
    gr_CollectorResponse->GetYaxis()->SetLabelSize(0.05);
    gr_CollectorResponse->GetXaxis()->SetTitleOffset(1.3);
    gr_CollectorResponse->GetYaxis()->SetTitleOffset(1.3);
    gr_CollectorResponse->SetMarkerStyle(22);
    gr_CollectorResponse->SetMarkerSize(0.8);
    gr_CollectorResponse->SetMarkerColor(kRed);
    gr_CollectorResponse->Draw("AP");
    // gr_CollectorResponse->GetXaxis()->SetRangeUser(9, 9.5);
    // gr_CollectorResponse->GetYaxis()->SetRangeUser(0, 4);
    c->SaveAs("CollectorResponseVsAccumulativeDose.png");
}