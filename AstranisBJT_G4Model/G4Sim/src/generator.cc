#include "generator.hh"

generator::generator() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    fGeneralParticleSource = new G4GeneralParticleSource();

    G4SingleParticleSource *sps = fGeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");

    sps->SetParticleDefinition(particle);
    sps->SetNumberOfParticles(1);

    sps->GetPosDist()->SetPosDisType("Point"); // Point, Beam, Plane, Surface, Volume
    sps->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 50*mm, 0));
    sps->GetPosDist()->ConfineSourceToVolume("NULL");

    sps->GetEneDist()->SetEnergyDisType("Gauss"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
    sps->GetEneDist()->SetMonoEnergy(75 * MeV);
    sps->GetEneDist()->SetBeamSigmaInE(5 * MeV);

    fRandom = new TRandom3();
}

generator::~generator() {
    delete fGeneralParticleSource;
}

void generator::GeneratePrimaries(G4Event *anEvent) {
    fGeneralParticleSource->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(RandomizedDirection());
    fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}

G4ThreeVector generator::RandomizedDirection() const {
    double openingAngle = 20; // mrad
    double rdm1 = fRandom->Gaus(0, openingAngle);
    double y_mom = TMath::Cos(rdm1 * mrad);
    double tmp = TMath::Sin(rdm1 * mrad);

    double rdm2 = fRandom->Uniform(0, TMath::TwoPi());
    double x_mom = TMath::Sin(rdm2 * rad) * tmp;
    double z_mom = TMath::Cos(rdm2 * rad) * tmp;

    // return {0, -1, 0};
    return {x_mom, -y_mom, z_mom};
}
