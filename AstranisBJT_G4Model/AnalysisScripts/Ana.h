#pragma once

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

std::string DataPath = "/home/dphan/Documents/GitHub/UT3-DataAnalysis/AstranisBJT_G4Model/Output";
std::string filename = "EDep.1pC.root";

class Ana {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   Int_t           IsActiveRegion;
   Double_t        EDep;

   TBranch        *b_IsActiveRegion;   //!
   TBranch        *b_EDep;   //!

   Ana(TTree *tree=0);
   virtual ~Ana();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

Ana::Ana(TTree *tree) : fChain(0) {
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(Form("%s/%s", DataPath.c_str(), filename.c_str()));
      if (!f || !f->IsOpen()) {
         f = new TFile(Form("%s/%s", DataPath.c_str(), filename.c_str()));
      }
      f->GetObject("Hits",tree);
   }
   Init(tree);
}

Ana::~Ana() {
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Ana::GetEntry(Long64_t entry) {
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t Ana::LoadTree(Long64_t entry) {
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Ana::Init(TTree *tree) {
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("IsActiveRegion", &IsActiveRegion, &b_IsActiveRegion);
   fChain->SetBranchAddress("EDep", &EDep, &b_EDep);
   Notify();
}

Bool_t Ana::Notify() {
   return kTRUE;
}

void Ana::Show(Long64_t entry) {
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t Ana::Cut(Long64_t entry) {
   return 1;
}
