#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <filesystem>

// double QToDoseFactor = 47E-3; // Gy/pC // For whole package
double QToDoseFactor = 202E-3; // Gy/pC // For silicon die only
double preExistingDose = 0;
double GyTokrad = 0.1; // Gy to krad
std::string filename = "Sum_0219_0312_A.csv";


double maxTotalDose = 100; // krad
double maxInstantDose = 150; // rad
double nBinsTotalDose = 30; // krad
double nBinsInstantDose = 30; // rad

using TUPLE_T = std::tuple<double, double, double, double, double>; 
// [Collector Max Voltage, ICT Total Charge, Enclosed Percentage, Enclosed Charge, Current Response]

std::vector<TUPLE_T> ReadDataFromCSV(const std::string &filename) {
    std::vector<TUPLE_T> shotData;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return shotData;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::vector<double> values(5, 0.0);
        std::stringstream ss(line);
        std::string token;
        int index = 0;

        while (std::getline(ss, token, ',')) {
            if (!token.empty()) {
                values[index] = std::stod(token);
            }
            index++;
        }

        TUPLE_T aShot(values[0], values[1], values[2], values[3], values[4]);
        shotData.push_back(aShot);
    }

    file.close();
    return shotData;
}

std::vector<double> CalculateCumulativeDose(const std::vector<TUPLE_T> &data) {
    std::vector<double> cumulativeDose;
    double sum = preExistingDose * GyTokrad; // Gy
    for (const auto &shot : data) {
        double instantDose = std::get<3>(shot) * QToDoseFactor * GyTokrad; // krad

        // If instant dose is negative, set it to zero
        if (instantDose <= 0) {
            instantDose = 0;
        }

        sum += instantDose; // krad
        cumulativeDose.push_back(sum);
    }
    return cumulativeDose;
}

std::vector<double> CalculateInstantDose(const std::vector<TUPLE_T> &data) {
    std::vector<double> instantDoseVec;
    for (const auto &shot : data) {
        double instantDose = std::get<3>(shot) * QToDoseFactor * GyTokrad * 1E3; // rad
        // If instant dose is negative, set it to zero
        if (instantDose <= 0) {
            instantDose = 0;
        }
        instantDoseVec.push_back(instantDose);
    }
    return instantDoseVec;
}

std::vector<double> CalculateCollectorVoltageResponse(const std::vector<TUPLE_T> &data) {
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

std::vector<double> CalculateCollectorVoltage(const std::vector<TUPLE_T> &data) {
    std::vector<double> collectorMaxVoltage;
    for (const auto &shot : data) {
        double collectorVoltage = std::get<0>(shot);
        double instantDose = std::get<3>(shot) * QToDoseFactor * GyTokrad; // krad
        if (collectorVoltage >= 0 || instantDose <= 0) {
            collectorMaxVoltage.push_back(0);
        } else {
            collectorMaxVoltage.push_back(TMath::Abs(collectorVoltage));
        }
    }
    return collectorMaxVoltage;
}

std::vector<double> CalculateCollectorCurrent(const std::vector<TUPLE_T> &data) {
    std::vector<double> collectorCurrentResponse;
    for (const auto &shot : data) {
        double currentResponse = std::get<4>(shot);
        double instantDose = std::get<3>(shot) * QToDoseFactor * GyTokrad; // krad
        if (currentResponse >= 0 || instantDose <= 0) {
            collectorCurrentResponse.push_back(0);
        } else {
            collectorCurrentResponse.push_back(TMath::Abs(currentResponse));
        }
    }
    return collectorCurrentResponse;
}

std::vector<double> CalculateCollectorCurrentResponse(const std::vector<TUPLE_T> &data) {
    std::vector<double> collectorCurrentResponse;
    for (const auto &shot : data) {
        double currentResponse = std::get<4>(shot);
        double instantDose = std::get<3>(shot) * QToDoseFactor * GyTokrad * 1000; // rad
        if (currentResponse >= 0 || instantDose <= 0) {
            collectorCurrentResponse.push_back(0);
        } else {
            collectorCurrentResponse.push_back(TMath::Abs(currentResponse / instantDose));
        }
    }
    return collectorCurrentResponse;
}

void makeROOTTree(const std::vector<TUPLE_T> &shotData) {
    TFile *f = new TFile("data.root", "RECREATE");
    TTree *tree = new TTree("data", "data");
    double maxVoltageInVolts, enclosedChargeInpC, collectorCurrentInAU;
    tree->Branch("maxVoltageInVolts",  &maxVoltageInVolts,  "maxVoltageInVolts/D");
    tree->Branch("enclosedChargeInpC", &enclosedChargeInpC, "enclosedChargeInpC/D");
    tree->Branch("collectorCurrentInAU", &collectorCurrentInAU, "collectorCurrentInAU/D");
    for (const auto &shot : shotData) {
        maxVoltageInVolts = std::get<0>(shot);
        enclosedChargeInpC = std::get<3>(shot);
        collectorCurrentInAU = std::get<4>(shot);
        tree->Fill();
    }
    tree->Write();
    f->Write();
    f->Close();

    delete tree;
    delete f;
}

void analysis_voltageResponse_vs_totalDose(const std::vector<TUPLE_T> &shotData) {
    auto cumulativeDose = CalculateCumulativeDose(shotData); // krad
    auto collectorResponse = CalculateCollectorVoltageResponse(shotData);

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
    gr_CollectorResponse->GetYaxis()->SetTitle("Voltage Response (A.U.)");
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
    c->SaveAs("VoltageResponse_1D.png");

    c->SetLogy();
    gr_CollectorResponse->GetYaxis()->SetRangeUser(1E-7, 1E4);
    c->SaveAs("VoltageResponse_1D_log.png");

    delete gr_CollectorResponse;
    delete c;
}

void analysis_currentResponse_vs_totalDose(const std::vector<TUPLE_T> &shotData) {
    auto cumulativeDose = CalculateCumulativeDose(shotData); // krad
    auto collectorResponse = CalculateCollectorCurrentResponse(shotData);

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
    gr_CollectorResponse->GetYaxis()->SetTitle("Current Response (A.U.)");
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
    c->SaveAs("CurrentReponse_1D.png");

    c->SetLogy();
    gr_CollectorResponse->GetYaxis()->SetRangeUser(1E-8, 1E3);
    c->SaveAs("CurrentReponse_1D_log.png");

    delete gr_CollectorResponse;
    delete c;
}

void analysis_voltage_vs_instantDose_vs_totalDose(const std::vector<TUPLE_T> &shotData) {
    auto cumulativeDose = CalculateCumulativeDose(shotData); // krad
    auto collectorMaxVoltage = CalculateCollectorVoltage(shotData);
    auto instantDose = CalculateInstantDose(shotData); // rad

    auto h2d_Voltage = new TH2D("h2d_Voltage", "h2d_Voltage", nBinsTotalDose, 0, maxTotalDose, nBinsInstantDose, 0, maxInstantDose);
    auto h2d_N       = new TH2D("h2d_N",       "h2d_N",       nBinsTotalDose, 0, maxTotalDose, nBinsInstantDose, 0, maxInstantDose);
    for (int i = 0; i < shotData.size(); i++) {
        h2d_Voltage->Fill(cumulativeDose[i], instantDose[i], collectorMaxVoltage[i]);
        h2d_N->Fill(cumulativeDose[i], instantDose[i], 1);
    }

    gStyle->SetOptStat(0);
    auto c = new TCanvas("c", "c", 800, 600);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.15);
    c->SetBottomMargin(0.15);
    c->SetTopMargin(0.15);
    h2d_Voltage->Divide(h2d_N);
    h2d_Voltage->Draw("colz");
    h2d_Voltage->SetTitle("");
    h2d_Voltage->GetXaxis()->SetTitle("Accumulative Dose (krad)");
    h2d_Voltage->GetYaxis()->SetTitle("Instant Dose (rad)");
    h2d_Voltage->GetZaxis()->SetTitle("Collector Maximum Voltage (V)");
    h2d_Voltage->GetXaxis()->CenterTitle();
    h2d_Voltage->GetYaxis()->CenterTitle();
    h2d_Voltage->GetXaxis()->SetTitleSize(0.05);
    h2d_Voltage->GetYaxis()->SetTitleSize(0.05);
    h2d_Voltage->GetXaxis()->SetLabelSize(0.05);
    h2d_Voltage->GetYaxis()->SetLabelSize(0.05);
    h2d_Voltage->GetXaxis()->SetTitleOffset(1.3);
    h2d_Voltage->GetYaxis()->SetTitleOffset(1.3);
    h2d_Voltage->SetMarkerStyle(22);
    h2d_Voltage->SetMarkerSize(0.8);
    c->SaveAs("MaximumVoltage_2D.png");
}

void analysis_current_vs_instantDose_vs_totalDose(const std::vector<TUPLE_T> &shotData) {
    auto cumulativeDose = CalculateCumulativeDose(shotData); // krad
    auto collectorMaxVoltage = CalculateCollectorCurrent(shotData);
    auto instantDose = CalculateInstantDose(shotData); // rad

    auto h2d_Voltage = new TH2D("h2d_Voltage", "h2d_Voltage", nBinsTotalDose, 0, maxTotalDose, nBinsInstantDose, 0, maxInstantDose);
    auto h2d_N       = new TH2D("h2d_N",       "h2d_N",       nBinsTotalDose, 0, maxTotalDose, nBinsInstantDose, 0, maxInstantDose);
    for (int i = 0; i < shotData.size(); i++) {
        h2d_Voltage->Fill(cumulativeDose[i], instantDose[i], collectorMaxVoltage[i]);
        h2d_N->Fill(cumulativeDose[i], instantDose[i], 1);
    }

    gStyle->SetOptStat(0);
    auto c = new TCanvas("c", "c", 800, 600);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.15);
    c->SetBottomMargin(0.15);
    c->SetTopMargin(0.15);
    h2d_Voltage->Divide(h2d_N);
    h2d_Voltage->Draw("colz");
    h2d_Voltage->SetTitle("");
    h2d_Voltage->GetXaxis()->SetTitle("Accumulative Dose (krad)");
    h2d_Voltage->GetYaxis()->SetTitle("Instant Dose (rad)");
    h2d_Voltage->GetZaxis()->SetTitle("Collector Current (A.U.)");
    h2d_Voltage->GetXaxis()->CenterTitle();
    h2d_Voltage->GetYaxis()->CenterTitle();
    h2d_Voltage->GetXaxis()->SetTitleSize(0.05);
    h2d_Voltage->GetYaxis()->SetTitleSize(0.05);
    h2d_Voltage->GetXaxis()->SetLabelSize(0.05);
    h2d_Voltage->GetYaxis()->SetLabelSize(0.05);
    h2d_Voltage->GetXaxis()->SetTitleOffset(1.3);
    h2d_Voltage->GetYaxis()->SetTitleOffset(1.3);
    h2d_Voltage->SetMarkerStyle(22);
    h2d_Voltage->SetMarkerSize(0.8);
    c->SaveAs("Current_2D.png");
}

void analysis() {
    std::vector<TUPLE_T> shotData;
    // Check if data.root exists using ROOT
    TFile *file = TFile::Open("data.root");
    if (!file || file->IsZombie()) {
        std::cout << "Data file data.root does not exist or is not accessible. Creating ROOT file." << std::endl;
        delete file; // Clean up the file pointer
        shotData = ReadDataFromCSV(filename.c_str());
        makeROOTTree(shotData);
    } else {
        std::cout << "data.root exists. Reading data from ROOT file." << std::endl;
        // Read data from ROOT file
        TTree *tree = (TTree*)file->Get("data");
        double maxVoltageInVolts, enclosedChargeInpC, collectorCurrentInAU;
        tree->SetBranchAddress("maxVoltageInVolts", &maxVoltageInVolts);
        tree->SetBranchAddress("enclosedChargeInpC", &enclosedChargeInpC);
        tree->SetBranchAddress("collectorCurrentInAU", &collectorCurrentInAU);
        for (int i = 0; i < tree->GetEntries(); i++) {
            tree->GetEntry(i);
            TUPLE_T aShot(maxVoltageInVolts, 0, 0, enclosedChargeInpC, collectorCurrentInAU);
            shotData.push_back(aShot);
        }
        file->Close();
        delete file; // Clean up the file pointer
    }

    // Perform analysis
    analysis_voltageResponse_vs_totalDose(shotData);
    analysis_currentResponse_vs_totalDose(shotData);
    analysis_voltage_vs_instantDose_vs_totalDose(shotData);
    analysis_current_vs_instantDose_vs_totalDose(shotData);
}