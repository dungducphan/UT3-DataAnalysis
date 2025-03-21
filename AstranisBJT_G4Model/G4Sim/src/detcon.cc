#include <detcon.hh>
#include <ParticleSD.hh>

#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include <TString.h>
#include <cstdlib>

detcon::detcon(const G4GDMLParser &parser) :
G4VUserDetectorConstruction(),
fWorldVolume(nullptr),
LV_MoldResin(nullptr),
LV_ActiveRegion(nullptr) {
    fWorldVolume = parser.GetWorldVolume();

    LV_MoldResin = parser.GetVolume("V-MoldResin-2");
    LV_ActiveRegion = parser.GetVolume("V-SiliconDie-4");

    visAtt_MoldResin = new G4VisAttributes();
    visAtt_MoldResin->SetVisibility(true);
    visAtt_MoldResin->SetForceSolid(true);
    visAtt_MoldResin->SetColour(1, 1, 1, 0.8);
    LV_MoldResin->SetVisAttributes(visAtt_MoldResin);

    visAtt_ActiveRegion = new G4VisAttributes();
    visAtt_ActiveRegion->SetVisibility(true);
    visAtt_ActiveRegion->SetForceSolid(true);
    visAtt_ActiveRegion->SetColour(0, 1, 0, 0.9);
    LV_ActiveRegion->SetVisAttributes(visAtt_ActiveRegion);
}

detcon::~detcon() {
    delete LV_MoldResin;
    delete LV_ActiveRegion; // <- this is the sensitive detector

    delete visAtt_MoldResin;
    delete visAtt_ActiveRegion;

    delete fWorldVolume;
}

G4VPhysicalVolume *detcon::Construct() {
    G4NistManager* nist = G4NistManager::Instance();

    // FR4 = Epoxy + Silica
    auto elH = new G4Element("Hydrogen", "H", 1, 1.01 * g / mole);
    auto elC = new G4Element("Carbon", "C", 6, 12.01 * g / mole);
    auto Epoxy = new G4Material("Epoxy", 1.2 * g/cm3, 2);
    Epoxy->AddElement(elH, 2);
    Epoxy->AddElement(elC, 2);
    auto elSi = new G4Element("Silicon", "Si", 14, 28.09 * g / mole);
    auto elO = new G4Element("Oxygen", "O", 8, 16.00 * g / mole);
    auto  SiO2 = new G4Material("SiO2", 2.2 * g/cm3, 2);
    SiO2->AddElement(elSi, 1);
    SiO2->AddElement(elO , 2);
    auto FR4 = new G4Material("FR4", 1.86 * g/cm3, 2);
    FR4->AddMaterial(Epoxy, 0.472);
    FR4->AddMaterial(SiO2, 0.528);

    // Mold Resin = 0.72 Silica + 0.15 CAS_29690822 + 0.13 CAS_9003354
    auto CAS_29690822 = new G4Material("CAS_29690822", 1.1 * g/cm3, 3);
    CAS_29690822->AddElement(elC, 33);
    CAS_29690822->AddElement(elH, 42);
    CAS_29690822->AddElement(elO, 9);
    auto CAS_9003354 = new G4Material("CAS_9003354", 1.18 * g/cm3, 3);
    CAS_9003354->AddElement(elC, 7);
    CAS_9003354->AddElement(elH, 8);
    CAS_9003354->AddElement(elO, 2);
    auto MoldResin = new G4Material("MoldResin", 1.2 * g/cm3, 3);
    MoldResin->AddMaterial(SiO2, 0.72);
    MoldResin->AddMaterial(CAS_29690822, 0.15);
    MoldResin->AddMaterial(CAS_9003354, 0.13);

    // DRZ
    auto DRZ_material = new G4Material("DRZ_material", 5 * g / cm3, 3);
    DRZ_material->AddElement(nist->FindOrBuildElement("Gd"), 2);
    DRZ_material->AddElement(nist->FindOrBuildElement("O"), 2);
    DRZ_material->AddElement(nist->FindOrBuildElement("S"), 1);
    auto Polyester = new G4Material("Polyester", 1.35 * g / cm3, 3);
    Polyester->AddElement(nist->FindOrBuildElement("C"), 10);
    Polyester->AddElement(nist->FindOrBuildElement("H"), 8);
    Polyester->AddElement(nist->FindOrBuildElement("O"), 4);

    fWorldVolume->GetLogicalVolume()->SetMaterial(nist->FindOrBuildMaterial("G4_Galactic"));
    LV_MoldResin->SetMaterial(nist->FindOrBuildMaterial("MoldResin"));
    LV_ActiveRegion->SetMaterial(nist->FindOrBuildMaterial("G4_Si"));

    G4bool checkOverlaps = true;

    // Print mass of the volumes
    G4double MoldResinMass = LV_MoldResin->GetMass();
    G4double ActiveRegionMass = LV_ActiveRegion->GetMass();
    std::cout << "MoldResin mass: " << MoldResinMass / g << " g" << std::endl;
    std::cout << "ActiveRegion mass: " << ActiveRegionMass / g << " g" << std::endl;

    return fWorldVolume;
}

void detcon::ConstructSDandField() {
    // Set sensitive detector
    auto particleSD = new ParticleSD("SD");
    G4SDManager::GetSDMpointer()->AddNewDetector(particleSD);
    SetSensitiveDetector(LV_MoldResin, particleSD);
    SetSensitiveDetector(LV_ActiveRegion, particleSD);
}