#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>


void exampleWaveform() {
    std::fstream file("/home/dphan/Documents/GitHub/UT3-DataAnalysis/AstranisDataAnalysis/CollectorVoltage/C1--20250227--00700.txt");

    double time, voltage;
    auto graph = new TGraph();
    while (file >> time >> voltage) {
        graph->SetPoint(graph->GetN(), time * 1E6, voltage);
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
    graph->GetXaxis()->SetRangeUser(0, 5);
    graph->SetLineColor(kRed);
    graph->SetLineWidth(2);

    auto leg = new TLegend(0.5, 0.6, 0.8, 0.85);
    leg->SetBorderSize(0);
    leg->AddEntry(graph, "Collector Voltage", "l");
    leg->Draw();

    c->SaveAs("exampleWaveform.png");
    c->SaveAs("exampleWaveform.pdf");
}