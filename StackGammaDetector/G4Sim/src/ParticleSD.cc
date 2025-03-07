//
// Created by dphan on 2/22/23.
//

#include <ParticleSD.hh>
#include <G4SystemOfUnits.hh>

ParticleSD::ParticleSD(const G4String &name) : G4VSensitiveDetector(name) {}

ParticleSD::~ParticleSD() noexcept {}

G4bool ParticleSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {

    // Get track and track weight
    auto track = aStep->GetTrack();

    // Get PID
    auto PID = track->GetParticleDefinition()->GetPDGEncoding();
    std::cout << "PID: " << PID << std::endl;

    return true;
}