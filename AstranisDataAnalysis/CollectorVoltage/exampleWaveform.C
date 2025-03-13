#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

std::string date = "20250227";
std::string shotID = "00693";

void exampleWaveform() {
    std::fstream file(Form("%s/C1--%s--%s.txt", date.c_str(), date.c_str(), shotID.c_str()));
    if (!file.is_open()) {
        std::cout << "Error opening file. " << std::endl;
    }

    std::string line;
    for (int i = 0; i < 6 && std::getline(file, line); ++i) {
        // Read lines until the fourth line
    }

    auto graph = new TGraph();
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string firstFloat, secondFloat;
        std::getline(iss, firstFloat, ',');
        std::getline(iss, secondFloat, ',');
        float firstNumber = std::stod(firstFloat);
        float secondNumber = std::stod(secondFloat);
        graph->SetPoint(graph->GetN(), firstNumber, secondNumber);
    }

    gStyle->SetLabelSize(0.05, "XY");
    gStyle->SetTitleSize(0.05, "XY");
    gStyle->SetOptStat(0);

    auto c = new TCanvas("c", "c", 1800, 800);
    graph->Draw("AL");
    graph->GetXaxis()->SetTitle("Time (us)");
    graph->GetYaxis()->SetTitle("Voltage (V)");
    graph->GetXaxis()->CenterTitle();
    graph->GetYaxis()->CenterTitle();
    graph->SetLineColor(kRed);
    graph->SetLineWidth(2);

    // auto leg = new TLegend(0.5, 0.6, 0.8, 0.85);
    // leg->SetBorderSize(0);
    // leg->AddEntry(graph, "Collector Voltage", "l");
    // leg->Draw();

    c->SaveAs(Form("wf-%s-%s.png", date.c_str(), shotID.c_str()));
}