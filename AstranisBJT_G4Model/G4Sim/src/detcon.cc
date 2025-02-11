#include <detcon.hh>
#include <ParticleSD.hh>

#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include <TString.h>
#include <cstdlib>

detcon::detcon(const G4GDMLParser &parser) :
G4VUserDetectorConstruction(),
fWorldVolume(nullptr),
LV_SOT23(nullptr),
LV_ActiveRegion(nullptr),
LV_PCB(nullptr),
LV_AlFoil000(nullptr),
LV_AlFoil001(nullptr),
LV_AlFoil002(nullptr),
LV_MylarWindow000(nullptr),
LV_MylarWindow001(nullptr),
LV_PE_BaseLayer(nullptr),
LV_PE_Protection(nullptr),
LV_DRZActiveLayer(nullptr),
LV_AIRBOX(nullptr) {
    fWorldVolume = parser.GetWorldVolume();

    LV_SOT23 = parser.GetVolume("LV_MoldResin");
    LV_ActiveRegion = parser.GetVolume("LV_ActiveRegion");
    LV_PCB = parser.GetVolume("LV_PCB");
    LV_AlFoil000 = parser.GetVolume("LV_AlFoil000");
    LV_AlFoil001 = parser.GetVolume("LV_AlFoil001");
    LV_AlFoil002 = parser.GetVolume("LV_AlFoil002");
    LV_MylarWindow000 = parser.GetVolume("LV_MylarWindow000");
    LV_MylarWindow001 = parser.GetVolume("LV_MylarWindow001");
    LV_PE_BaseLayer = parser.GetVolume("LV_PE_BaseLayer");
    LV_PE_Protection = parser.GetVolume("LV_PE_Protection");
    LV_DRZActiveLayer = parser.GetVolume("LV_DRZActiveLayer");
    LV_AIRBOX = parser.GetVolume("LV_AIR");

    visAtt_SOT23 = new G4VisAttributes();
    visAtt_SOT23->SetVisibility(true);
    visAtt_SOT23->SetForceSolid(true);
    visAtt_SOT23->SetColour(1, 1, 1, 0.3);
    LV_SOT23->SetVisAttributes(visAtt_SOT23);

    visAtt_ActiveRegion = new G4VisAttributes();
    visAtt_ActiveRegion->SetVisibility(true);
    visAtt_ActiveRegion->SetForceSolid(true);
    visAtt_ActiveRegion->SetColour(0, 1, 0, 0.9);
    LV_ActiveRegion->SetVisAttributes(visAtt_ActiveRegion);

    visAtt_PCB = new G4VisAttributes();
    visAtt_PCB->SetVisibility(true);
    visAtt_PCB->SetForceSolid(true);
    visAtt_PCB->SetColour(0, 0, 1, 0.3);
    LV_PCB->SetVisAttributes(visAtt_PCB);

    visAtt_AlFoil = new G4VisAttributes();
    visAtt_AlFoil->SetVisibility(true);
    visAtt_AlFoil->SetForceSolid(true);
    visAtt_AlFoil->SetColour(1, 1, 0, 0.3);
    LV_AlFoil000->SetVisAttributes(visAtt_AlFoil);
    LV_AlFoil001->SetVisAttributes(visAtt_AlFoil);
    LV_AlFoil002->SetVisAttributes(visAtt_AlFoil);

    visAtt_MylarWindow = new G4VisAttributes();
    visAtt_MylarWindow->SetVisibility(true);
    visAtt_MylarWindow->SetForceSolid(true);
    visAtt_MylarWindow->SetColour(0, 1, 1, 0.3);
    LV_MylarWindow000->SetVisAttributes(visAtt_MylarWindow);
    LV_MylarWindow001->SetVisAttributes(visAtt_MylarWindow);

    visAtt_DRZScreen = new G4VisAttributes();
    visAtt_DRZScreen->SetVisibility(true);
    visAtt_DRZScreen->SetForceSolid(true);
    visAtt_DRZScreen->SetColour(1, 0, 0, 0.3);
    LV_DRZActiveLayer->SetVisAttributes(visAtt_DRZScreen);
    LV_PE_Protection->SetVisAttributes(visAtt_DRZScreen);
    LV_PE_BaseLayer->SetVisAttributes(visAtt_DRZScreen);

    visAtt_AIRBOX = new G4VisAttributes();
    visAtt_AIRBOX->SetVisibility(false);
    visAtt_AIRBOX->SetForceSolid(true);
    visAtt_AIRBOX->SetColour(0, 0, 0, 0);
    LV_AIRBOX->SetVisAttributes(visAtt_AIRBOX);
}

detcon::~detcon() {
    delete LV_SOT23;
    delete LV_ActiveRegion; // <- this is the sensitive detector
    delete LV_PCB;
    delete LV_AlFoil000;
    delete LV_AlFoil001;
    delete LV_AlFoil002;
    delete LV_MylarWindow000;
    delete LV_MylarWindow001;

    delete visAtt_SOT23;
    delete visAtt_ActiveRegion;
    delete visAtt_PCB;
    delete visAtt_DRZScreen;
    delete visAtt_AlFoil;
    delete visAtt_MylarWindow;

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
    LV_SOT23->SetMaterial(nist->FindOrBuildMaterial("MoldResin"));
    LV_ActiveRegion->SetMaterial(nist->FindOrBuildMaterial("G4_Si"));
    LV_PCB->SetMaterial(nist->FindOrBuildMaterial("FR4"));
    LV_AlFoil000->SetMaterial(nist->FindOrBuildMaterial("G4_Al"));
    LV_AlFoil001->SetMaterial(nist->FindOrBuildMaterial("G4_Al"));
    LV_AlFoil002->SetMaterial(nist->FindOrBuildMaterial("G4_Al"));
    LV_MylarWindow000->SetMaterial(nist->FindOrBuildMaterial("G4_MYLAR"));
    LV_MylarWindow001->SetMaterial(nist->FindOrBuildMaterial("G4_MYLAR"));
    LV_PE_BaseLayer->SetMaterial(Polyester);
    LV_PE_Protection->SetMaterial(Polyester);
    LV_DRZActiveLayer->SetMaterial(DRZ_material);
    LV_AIRBOX->SetMaterial(nist->FindOrBuildMaterial("G4_AIR"));

    G4bool checkOverlaps = true;

    return fWorldVolume;
}

void detcon::ConstructSDandField() {
}