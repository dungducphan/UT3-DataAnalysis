//
// Created by dphan on 2/22/23.
//

#include <ParticleSD.hh>
#include <G4SystemOfUnits.hh>

ParticleSD::ParticleSD(const G4String &name) : G4VSensitiveDetector(name) {
    man = G4AnalysisManager::Instance();
}

ParticleSD::~ParticleSD() noexcept {}

G4bool ParticleSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {
    // Get logical volume of the hit
    auto lv = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();
    int isActive = lv == "V-SiliconDie-4" ? 1 : 0;

    // Get energy deposition in eV
    auto edep = aStep->GetTotalEnergyDeposit() / eV;

    man->FillNtupleIColumn(0, isActive);
    man->FillNtupleDColumn(1, edep);
    man->AddNtupleRow();

    return true;
}