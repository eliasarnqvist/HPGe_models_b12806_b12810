#ifndef EDETECTORCONSTRUCTION_HH
#define EDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4MultiUnion.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"

#include "ESensitiveDetector.hh"

#include "G4Region.hh"
#include "G4ProductionCuts.hh"

// #include "G4GenericMessenger.hh"

class EDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    EDetectorConstruction();
    virtual ~EDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();

private:
    G4LogicalVolume *logicDetector_b12806;
    G4LogicalVolume *logicDetector_b12810;
    ESensitiveDetector *sensDet = nullptr;

    G4Material *MatCFRP;
    
    // G4GenericMessenger *fMessenger;
    // G4double detectorDiameter, detectorLength;

    virtual void ConstructSDandField();
    virtual G4LogicalVolume *ConstructHPGe(G4LogicalVolume* logicWorld, G4double crystalOuterLength, G4double crystalOuterDeadLayer, G4double crystalInnerDeadLayer, G4double crystalOuterDiameter, G4double crystalTopDistance, G4double zPosition, G4double detectorAngle, G4int copyNo, G4bool checkOverlaps);

    G4Region *regionOuterDeadLayer = new G4Region("regionOuterDeadLayer");
    G4Region *regionCapWindow = new G4Region("regionCapWindow");
};

#endif