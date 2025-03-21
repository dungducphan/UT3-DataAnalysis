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
    sps->GetPosDist()->ConfineSourceToVolume("NULL");

    sps->GetEneDist()->SetEnergyDisType("Gauss"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
    sps->GetEneDist()->SetMonoEnergy(75 * MeV);
    sps->GetEneDist()->SetBeamSigmaInE(5 * MeV);

    sps->GetAngDist()->SetParticleMomentumDirection({0, -1, 0});

    fRandom = new TRandom3();
}

generator::~generator() {
    delete fGeneralParticleSource;
}

void generator::GeneratePrimaries(G4Event *anEvent) {
    fGeneralParticleSource->GetCurrentSource()->GetPosDist()->SetCentreCoords(RandomizedPosition());
    fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}

G4ThreeVector generator::RandomizedPosition() const {
    double rdm_x = fRandom->Uniform(-1450, 1450);
    double rdm_z = fRandom->Uniform(-650, 650);
    return {rdm_x * um, 10 * mm, rdm_z * um};
}
