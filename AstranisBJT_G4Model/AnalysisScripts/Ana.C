#include <iostream>

#include "Ana.h"

const double mass_MoldResin_gram = 0.00406;
const double mass_ActiveRegion_gram = 2.25544e-05;

void Ana::Loop() {
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   double sumEdep_SiliconDie = 0;
   double sumEdep_Total = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      sumEdep_SiliconDie += IsActiveRegion == 1 ? EDep : 0;
      sumEdep_Total += EDep;
   }

   std::cout << "Total dose: " << 10 * sumEdep_Total * 1.60218e-19 / ((mass_MoldResin_gram + mass_ActiveRegion_gram) / 1000.) << " Gy." << std::endl;
   std::cout << "Dose in the active region: " << 10 * sumEdep_SiliconDie * 1.60218e-19 / (mass_ActiveRegion_gram / 1000.) << " Gy." << std::endl;
}

#if !defined(__CINT__) && !defined(__CLING__) && !defined(__ACLIC__)

int main() {
   auto ana = new Ana();
   ana->Loop();

   return 0;
}
#endif