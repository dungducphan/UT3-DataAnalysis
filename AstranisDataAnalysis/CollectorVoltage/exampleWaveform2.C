void exampleWaveform() {

    std::ifstream file("C1--20250225--00713.txt");
    double time, voltage;
    auto gr = new TGraph();
    while (file >> time >> voltage) {
        time *= 1E6; 
        if (time < 1 && time > 0) gr->SetPoint(gr->GetN(), time, voltage);
    }

    auto c = new TCanvas("c", "c", 1800, 800);
    gr->Draw("AL");
    gr->SetLineColor(kRed);
    gr->SetLineWidth(2);
    gr->GetXaxis()->SetTitle("Time [us]");
    gr->GetYaxis()->SetTitle("Voltage [V]");
    gr->GetXaxis()->SetRangeUser(0, 1);
    gr->GetYaxis()->SetRangeUser(-0.6, 0.2);
    gr->GetXaxis()->CenterTitle();
    gr->GetYaxis()->CenterTitle();
    gr->GetXaxis()->SetTitleSize(0.05);
    gr->GetYaxis()->SetTitleSize(0.05);
    gr->GetXaxis()->SetTitleOffset(0.9);
    gr->GetYaxis()->SetTitleOffset(0.9);
    gr->GetXaxis()->SetLabelSize(0.05);
    gr->GetYaxis()->SetLabelSize(0.05);
    gr->GetYaxis()->SetNdivisions(505);
    c->SaveAs("exampleWaveform2.png");
    c->SaveAs("exampleWaveform2.pdf");
}