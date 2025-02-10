#include <detcon.hh>
#include <ParticleSD.hh>

#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include <TString.h>
#include <cstdlib>

detcon::detcon(const G4GDMLParser &parser) :
G4VUserDetectorConstruction(),
logicalDetector(nullptr),
arcVisAtt(nullptr),
vacuumChamberVisAtt(nullptr),
glassWindowVisAtt(nullptr),
HDPEOuterVisAtt(nullptr),
LeadVisAtt(nullptr),
TungstenVisAtt(nullptr),
PhantomVisAtt(nullptr) {
    fWorldVolume = parser.GetWorldVolume();
    logicalDetector    = parser.GetVolume("LV_Phantom");
    logicalArc         = parser.GetVolume("LV_Arc");
    logicalChamber     = parser.GetVolume("LV_VacuumChamber");
    logicalFusedSilica = parser.GetVolume("LV_Window");
    logicalPbBlock1    = parser.GetVolume("LV_Pb_Block_01");
    logicalPbBlock2    = parser.GetVolume("LV_PB_Block_02");
    logicalHDPEBlock1  = parser.GetVolume("LV_HDPE_Block_01");
    logicalHDPEBlock2  = parser.GetVolume("LV_HDPE_Block_02");
    logicalWBlock      = parser.GetVolume("LV_W_Plate");

    arcVisAtt = new G4VisAttributes();
    arcVisAtt->SetVisibility(true);
    arcVisAtt->SetForceSolid(true);
    arcVisAtt->SetColour(1, 1, 1, 0.05);
    logicalArc->SetVisAttributes(arcVisAtt);

    vacuumChamberVisAtt = new G4VisAttributes();
    vacuumChamberVisAtt->SetVisibility(true);
    vacuumChamberVisAtt->SetForceSolid(true);
    vacuumChamberVisAtt->SetColour(0, 0.8, 0, 0.5);
    logicalChamber->SetVisAttributes(vacuumChamberVisAtt);

    glassWindowVisAtt = new G4VisAttributes();
    glassWindowVisAtt->SetVisibility(true);
    glassWindowVisAtt->SetForceSolid(true);
    glassWindowVisAtt->SetColour(0.8, 0.8, 0, 0.5);
    logicalFusedSilica->SetVisAttributes(glassWindowVisAtt);

    HDPEOuterVisAtt = new G4VisAttributes();
    HDPEOuterVisAtt->SetVisibility(true);
    HDPEOuterVisAtt->SetForceSolid(true);
    HDPEOuterVisAtt->SetColour(0.8, 0.8, 0.8, 0.7);
    logicalHDPEBlock1->SetVisAttributes(HDPEOuterVisAtt);
    logicalHDPEBlock2->SetVisAttributes(HDPEOuterVisAtt);

    LeadVisAtt = new G4VisAttributes();
    LeadVisAtt->SetVisibility(true);
    LeadVisAtt->SetForceSolid(true);
    LeadVisAtt->SetColour(0.7, 0.2, 0.6, 0.9);
    logicalPbBlock1->SetVisAttributes(LeadVisAtt);
    logicalPbBlock2->SetVisAttributes(LeadVisAtt);

    TungstenVisAtt = new G4VisAttributes();
    TungstenVisAtt->SetVisibility(true);
    TungstenVisAtt->SetForceSolid(true);
    TungstenVisAtt->SetColour(0.1, 0.1, 0.1, 1);
    logicalWBlock->SetVisAttributes(TungstenVisAtt);

    PhantomVisAtt = new G4VisAttributes();
    PhantomVisAtt->SetVisibility(true);
    PhantomVisAtt->SetForceSolid(true);
    PhantomVisAtt->SetColour(0.1, 0.0, 0.1, 0.2);
    logicalDetector->SetVisAttributes(PhantomVisAtt);
}

detcon::~detcon() {
    delete logicalDetector;
    delete logicalArc;
    delete logicalChamber;
    delete logicalFusedSilica;
    delete logicalPbBlock1;
    delete logicalPbBlock2;
    delete logicalHDPEBlock1;
    delete logicalHDPEBlock2;
    delete logicalWBlock;

    delete arcVisAtt;
    delete vacuumChamberVisAtt;
    delete glassWindowVisAtt;
    delete HDPEOuterVisAtt;
    delete LeadVisAtt;
    delete TungstenVisAtt;
    delete PhantomVisAtt;
}

G4VPhysicalVolume *detcon::Construct() {

    std::cout << "________________________________________________________________________" << std::endl;
    std::cout << "Mass of Arc " << logicalArc->GetMass() / kg << " kg" << std::endl;
    std::cout << "Mass of Chamber " << logicalChamber->GetMass() / kg << " kg" << std::endl;
    std::cout << "Mass of Beam Dump: " << logicalPbBlock1->GetMass() / kg + logicalPbBlock2->GetMass() / kg + logicalHDPEBlock1->GetMass() / kg + logicalHDPEBlock2->GetMass() / kg + logicalWBlock->GetMass() / kg << " kg" << std::endl;
    std::cout << "________________________________________________________________________" << std::endl;

    return fWorldVolume;
}

void detcon::ConstructSDandField() {
    if (logicalDetector == nullptr) {
        G4cout << "Logical volume for sensitive detector is not set." << G4endl;
        return;
    }

    // G4SDManager *SDman = G4SDManager::GetSDMpointer();
    // G4String SDname;
    //
    // // Create sensitive detectors
    // auto particleSD = new ParticleSD("ParticleSD");
    // SDman->AddNewDetector(particleSD);
    // logicalDetector->SetSensitiveDetector(particleSD);
}