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

    G4LogicalVolume* LV_MoldResin;
    G4LogicalVolume* LV_ActiveRegion;

    G4VisAttributes* visAtt_MoldResin;
    G4VisAttributes* visAtt_ActiveRegion;
};