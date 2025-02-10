#include "runaction.hh"

runAction::runAction() : G4UserRunAction() {
    man = G4AnalysisManager::Instance();
    man->CreateNtuple("EDep", "EDep");
    man->CreateNtupleDColumn("EDep");
    man->CreateNtupleDColumn("Weight");
    man->CreateNtupleIColumn("HumanPhantom_ID");
    man->CreateNtupleDColumn("Energy");
    man->CreateNtupleDColumn("Px");
    man->CreateNtupleDColumn("Py");
    man->CreateNtupleDColumn("Pz");
    man->CreateNtupleDColumn("X");
    man->CreateNtupleDColumn("Y");
    man->CreateNtupleDColumn("Z");
    man->CreateNtupleIColumn("PID");
    man->FinishNtuple(0);
}

runAction::~runAction() = default;

void runAction::BeginOfRunAction(const G4Run *run) {
    man = G4AnalysisManager::Instance();
    man->OpenFile(Form("EDep_Beam_%03i.root", (int) run->GetRunID()));
}

void runAction::EndOfRunAction(const G4Run *) {
    man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}

