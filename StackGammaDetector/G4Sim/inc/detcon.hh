#pragma once

#include <utility>
#include <tuple>
#include <fstream>

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SDManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4VIStore.hh"
#include "G4GDMLParser.hh"

#include "TMath.h"

#include "ParticleSD.hh"

class G4VPhysicalVolume;

class G4LogicalVolume;

class detcon : public G4VUserDetectorConstruction {
public:

    detcon(const G4GDMLParser& parser);
    ~detcon() override;

    G4VPhysicalVolume *Construct() override;

    void ConstructSDandField() override;

    G4VPhysicalVolume* GetWorldVolume() {
        return fWorldVolume;
    }

private:
    G4VPhysicalVolume* fWorldVolume;

    G4LogicalVolume* LV_SOT23;
    G4LogicalVolume* LV_ActiveRegion; // <- this is the sensitive detector
    G4LogicalVolume* LV_PCB;
    G4LogicalVolume* LV_PE_BaseLayer;
    G4LogicalVolume* LV_PE_Protection;
    G4LogicalVolume* LV_DRZActiveLayer;
    G4LogicalVolume* LV_AlFoil000;
    G4LogicalVolume* LV_AlFoil001;
    G4LogicalVolume* LV_AlFoil002;
    G4LogicalVolume* LV_MylarWindow000;
    G4LogicalVolume* LV_MylarWindow001;
    G4LogicalVolume* LV_AIRBOX;

    G4VisAttributes* visAtt_SOT23;
    G4VisAttributes* visAtt_ActiveRegion;
    G4VisAttributes* visAtt_PCB;
    G4VisAttributes* visAtt_DRZScreen;
    G4VisAttributes* visAtt_AlFoil;
    G4VisAttributes* visAtt_MylarWindow;
    G4VisAttributes* visAtt_AIRBOX;
};