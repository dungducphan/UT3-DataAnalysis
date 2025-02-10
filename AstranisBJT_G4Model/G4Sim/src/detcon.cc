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
LV_MylarWindow001(nullptr) {
    fWorldVolume = parser.GetWorldVolume();

    LV_SOT23 = parser.GetVolume("LV_MoldResin");
    LV_ActiveRegion = parser.GetVolume("LV_ActiveRegion");
    LV_PCB = parser.GetVolume("LV_PCB");
    LV_AlFoil000 = parser.GetVolume("LV_AlFoil000");
    LV_AlFoil001 = parser.GetVolume("LV_AlFoil001");
    LV_AlFoil002 = parser.GetVolume("LV_AlFoil002");
    LV_MylarWindow000 = parser.GetVolume("LV_MylarWindow000");
    LV_MylarWindow001 = parser.GetVolume("LV_MylarWindow001");

    visAtt_SOT23 = new G4VisAttributes();
    visAtt_SOT23->SetVisibility(true);
    visAtt_SOT23->SetForceSolid(true);
    visAtt_SOT23->SetColour(1, 1, 1, 0.9);
    LV_SOT23->SetVisAttributes(visAtt_SOT23);

    visAtt_ActiveRegion = new G4VisAttributes();
    visAtt_ActiveRegion->SetVisibility(true);
    visAtt_ActiveRegion->SetForceSolid(true);
    visAtt_ActiveRegion->SetColour(0, 1, 0, 0.9);
    LV_ActiveRegion->SetVisAttributes(visAtt_ActiveRegion);

    visAtt_PCB = new G4VisAttributes();
    visAtt_PCB->SetVisibility(true);
    visAtt_PCB->SetForceSolid(true);
    visAtt_PCB->SetColour(0, 0, 1, 0.9);
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
    return fWorldVolume;
}

void detcon::ConstructSDandField() {
}