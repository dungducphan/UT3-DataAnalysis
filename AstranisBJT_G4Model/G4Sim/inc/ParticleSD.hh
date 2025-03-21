//
// Created by dphan on 2/22/23.
//

#pragma once

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4OpticalPhoton.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "globals.hh"

class G4Step;

class ParticleSD : public G4VSensitiveDetector {
public:
    ParticleSD(const G4String& name);
    virtual ~ParticleSD();
    virtual G4bool  ProcessHits(G4Step*, G4TouchableHistory*);
private:
    G4AnalysisManager* man = nullptr;
};