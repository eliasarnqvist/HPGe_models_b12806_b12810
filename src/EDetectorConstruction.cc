#include "EDetectorConstruction.hh"

EDetectorConstruction::EDetectorConstruction()
{
    // fMessenger = new G4GenericMessenger(this, "/E_detector/", "Settings for the detector");

    // fMessenger->DeclareProperty("detectorDiameter", detectorDiameter, "Diameter of the detector (cm)");
    // fMessenger->DeclareProperty("detectorLength", detectorLength, "Length of the detector (cm)");

    // detectorDiameter = 5.;
    // detectorLength = 5.;
}

EDetectorConstruction::~EDetectorConstruction()
{
    
}

G4VPhysicalVolume *EDetectorConstruction::Construct()
{
    G4bool checkOverlaps = true;

    // These materials will be used

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *MatWorld = nist->FindOrBuildMaterial("G4_AIR");

    // Carbon fiber reinforced polymer (CFRP) material
    MatCFRP = new G4Material("CFRP", 1.4 * g/cm3, 2);
    MatCFRP->AddElement(nist->FindOrBuildElement("C"), 0.85);
    MatCFRP->AddElement(nist->FindOrBuildElement("O"), 0.15);

    // World
    G4double xWorld = 1. * m;
    G4double yWorld = 1. * m;
    G4double zWorld = 1. * m;
    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, MatWorld, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    G4double b12806_crystalLength = 35.0 * mm;
    G4double b12806_crystalOuterDeadLayer = 60.0/1000 * mm;
    G4double b12806_crystalInnerDeadLayer = 1.0 * mm;
    G4double b12806_crystalOuterDiameter = 45.0 * mm;
    G4double b12806_crystalTopDistance = 7.5 * mm;
    logicDetector_b12806 = ConstructHPGe(logicWorld, b12806_crystalLength, b12806_crystalOuterDeadLayer, b12806_crystalInnerDeadLayer, b12806_crystalOuterDiameter, b12806_crystalTopDistance, 0.25 * mm, 0. * deg, 0, checkOverlaps);

    G4double b12810_crystalLength = 33.2 * mm;
    G4double b12810_crystalOuterDeadLayer = 60.0/1000 * mm;
    G4double b12810_crystalInnerDeadLayer = 1.0 * mm;
    G4double b12810_crystalOuterDiameter = 45.8 * mm;
    G4double b12810_crystalTopDistance = 6 * mm;
    logicDetector_b12810 = ConstructHPGe(logicWorld, b12810_crystalLength, b12810_crystalOuterDeadLayer, b12810_crystalInnerDeadLayer, b12810_crystalOuterDiameter, b12810_crystalTopDistance, 0.25 * mm, 180. * deg, 1, checkOverlaps);

    // Set smaller range cuts for regions with small geometries, default is 1 mm
    G4ProductionCuts *cutsOuterDeadLayer = new G4ProductionCuts();
    cutsOuterDeadLayer->SetProductionCut(0.01*mm);
    regionOuterDeadLayer->SetProductionCuts(cutsOuterDeadLayer);

    G4ProductionCuts *cutsCapWindow = new G4ProductionCuts();
    cutsCapWindow->SetProductionCut(0.1*mm);
    regionCapWindow->SetProductionCuts(cutsCapWindow);

    return physWorld;
}

void EDetectorConstruction::ConstructSDandField()
{
    if (sensDet == nullptr)
    {
        ESensitiveDetector *sensDet = new ESensitiveDetector("ESensitiveDetector");
        logicDetector_b12806->SetSensitiveDetector(sensDet);
        logicDetector_b12810->SetSensitiveDetector(sensDet);
        G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
    }
}

G4LogicalVolume *EDetectorConstruction::ConstructHPGe(G4LogicalVolume* logicWorld, G4double crystalOuterLength, G4double crystalOuterDeadLayer, G4double crystalInnerDeadLayer, G4double crystalOuterDiameter, G4double crystalTopDistance, G4double zPosition, G4double detectorAngle, G4int copyNo, G4bool checkOverlaps)
{
    // Define materials
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *MatWorld = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *MatGe = nist->FindOrBuildMaterial("G4_Ge");
    G4Material *MatAl = nist->FindOrBuildMaterial("G4_Al");
    G4Material *MatVac = nist->FindOrBuildMaterial("G4_Galactic");
    // High density polyethylene (HDPE) material
    G4Material *MatHDPE = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

    G4double sliceAngle = 360. * deg;
    // G4double detectorAngle = 0.0;

    // Assembly measurements
    G4double capOuterDiameter = 76.2 * mm;
    G4double capOuterLength = 63.0 * mm;
    G4double capThickness = 1.6 * mm;
    G4double capInnerDiameter = capOuterDiameter - (2 * capThickness);
    G4double capWindowDiameter = 63.0 * mm;
    G4double capWindowThickness = 0.6 * mm; // 0.6 mm in datasheet
    G4double capBaseThickness = 5.0 * mm;
    // Holder measurements
    G4double holderInnerDiameter = 48.0 * mm;
    G4double holderOuterDiameter = 49.6 * mm;
    G4double holderRingOuterDiameter = 54.0 * mm;
    G4double holderRingHeight = 8.6 * mm;
    G4double holderBottomRingDistance = 19.4 * mm;
    G4double holderInnerLength = 40.0 * mm;
    G4double holderOuterLength = (40.0 + 10.0) * mm;
    G4double holderBottomThickness = holderOuterLength - holderInnerLength;
    G4double holderDistanceFromCapTop = 5.5 * mm;
    // Crystal measurements
    // G4double crystalOuterDiameter = 45.8 * mm;
    // G4double crystalOuterLength = 33.5 * mm;
    G4double crystalHoleDiameter = 9.5 * mm;
    G4double crystalHoleLength = 15.0 * mm;
    G4double crystalGroveOuterDiameter = 25.0 * mm;
    G4double crystalGroveInnerDiameter = 17.0 * mm;
    G4double crystalGroveDepth = 2.0 * mm;
    // G4double crystalOuterDeadLayer = 300.0/1000 * mm; // 0.3 um in datasheet
    // G4double crystalInnerDeadLayer = 2 * mm; // 0.5 mm in datasheet
    // G4double crystalDistanceFromCapTop = (6.0 + 0.6) * mm;
    G4double crystalDistanceFromCapTop = (crystalTopDistance + 0.6) * mm;
    // Insert measurements
    G4double insertOuterDiameter = holderInnerDiameter;
    G4double insertInnerDiameter = crystalOuterDiameter;
    G4double insertLength = holderInnerLength;
    G4double insertTopThickness = crystalDistanceFromCapTop - holderDistanceFromCapTop;

    // Cap
    G4Tubs *solidCap = new G4Tubs("solidCap", 0. * mm, 0.5 * capOuterDiameter, 0.5 * capOuterLength, 0. * deg, sliceAngle);
    G4LogicalVolume *logicCap = new G4LogicalVolume(solidCap, MatAl, "logicCap");
    G4Rotate3D myRoration(detectorAngle, G4ThreeVector(0, 1, 0));
    G4Translate3D myTranslation(G4ThreeVector(0., 0., 0.5 * capOuterLength + zPosition));
    G4Transform3D myTransform = (myRoration) * (myTranslation);
    G4VPhysicalVolume *physCap = new G4PVPlacement(myTransform, logicCap, "physCap", logicWorld, false, copyNo, checkOverlaps);
    // Vacuum inside cap
    G4Tubs *solidVac = new G4Tubs("solidVac", 0. * mm, 0.5 * capInnerDiameter, 0.5 * (capOuterLength - capThickness - capBaseThickness), 0. * deg, sliceAngle);
    G4LogicalVolume *logicVac = new G4LogicalVolume(solidVac, MatVac, "logicVac");
    G4VPhysicalVolume *physVac = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * capOuterLength + 0.5 * (capOuterLength - capThickness - capBaseThickness) + capThickness), logicVac, "physVac", logicCap, false, 0, checkOverlaps);
    G4Tubs *solidVacWindow = new G4Tubs("solidVacWindow", 0. * mm, 0.5 * capWindowDiameter, 0.5 * (capThickness - capWindowThickness), 0. * deg, sliceAngle);
    G4LogicalVolume *logicVacWindow = new G4LogicalVolume(solidVacWindow, MatVac, "logicVacWindow");
    G4VPhysicalVolume *physVacWindow = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * capOuterLength + capWindowThickness + 0.5 * (capThickness - capWindowThickness)), logicVacWindow, "physVacWindow", logicCap, false, 0, checkOverlaps);    
    // Window
    G4Tubs *solidWindow = new G4Tubs("solidWindow", 0. * mm, 0.5 * capWindowDiameter, 0.5 * capWindowThickness, 0. * deg, sliceAngle);
    G4LogicalVolume *logicWindow = new G4LogicalVolume(solidWindow, MatCFRP, "logicWindow");
    G4VPhysicalVolume *physWindow = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * capOuterLength + 0.5 * capWindowThickness), logicWindow, "physWindow", logicCap, false, 0, checkOverlaps);

    regionCapWindow->AddRootLogicalVolume(logicWindow);

    // Holder
    G4Tubs *solidHolderSides = new G4Tubs("solidHolderSides", 0.5 * holderInnerDiameter, 0.5 * holderOuterDiameter, 0.5 * holderOuterLength, 0. * deg, sliceAngle);
    G4Tubs *solidHolderTopRing = new G4Tubs("solidHolderTopRing", 0.5 * holderInnerDiameter, 0.5 * holderRingOuterDiameter, 0.5 * holderRingHeight, 0. * deg, sliceAngle);
    G4Tubs *solidHolderBottomRing = new G4Tubs("solidHolderBottomRing", 0.5 * holderInnerDiameter, 0.5 * holderRingOuterDiameter, 0.5 * holderRingHeight, 0. * deg, sliceAngle);
    G4Tubs *solidHolderBottom = new G4Tubs("solidHolderBottom", 0, 0.5 * holderInnerDiameter, 0.5 * holderBottomThickness, 0. * deg, sliceAngle);
    G4MultiUnion* munionHolder = new G4MultiUnion("munionHolder");
    munionHolder->AddNode(*solidHolderSides, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0., 0., holderDistanceFromCapTop + 0.5 * holderOuterLength)));
    munionHolder->AddNode(*solidHolderTopRing, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0., 0., holderDistanceFromCapTop + 0.5 * holderRingHeight)));
    munionHolder->AddNode(*solidHolderBottomRing, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0., 0., holderDistanceFromCapTop + holderBottomRingDistance + 0.5 * holderRingHeight)));
    munionHolder->AddNode(*solidHolderBottom, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0., 0., holderDistanceFromCapTop + holderOuterLength - 0.5 * holderBottomThickness)));
    munionHolder->Voxelize();
    G4LogicalVolume *logicHolder = new G4LogicalVolume(munionHolder, MatAl, "logicHolder");
    G4VPhysicalVolume *physHolder = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * (capOuterLength - capThickness - capBaseThickness) - capThickness), logicHolder, "physHolder", logicVac, false, 0, checkOverlaps);
    
    // Insert
    G4Tubs *solidInsertSides = new G4Tubs("solidInsertSides", 0.5 * insertInnerDiameter, 0.5 * insertOuterDiameter, 0.5 * insertLength, 0. * deg, sliceAngle);
    G4LogicalVolume *logicInsertSides = new G4LogicalVolume(solidInsertSides, MatHDPE, "logicInsertSides");
    G4VPhysicalVolume *physInsertSides = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * (capOuterLength - capThickness - capBaseThickness) - capThickness + holderDistanceFromCapTop + 0.5 * insertLength), logicInsertSides, "physInsertSides", logicVac, false, 0, checkOverlaps);
    G4Tubs *solidInsertTop = new G4Tubs("solidInsertTop", 0. * mm, 0.5 * insertInnerDiameter, 0.5 * insertTopThickness, 0. * deg, sliceAngle);
    G4LogicalVolume *logicInsertTop = new G4LogicalVolume(solidInsertTop, MatHDPE, "logicInsertTop");
    G4VPhysicalVolume *physInsertTop = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * (capOuterLength - capThickness - capBaseThickness) - capThickness + holderDistanceFromCapTop + 0.5 * insertTopThickness), logicInsertTop, "physInsertTop", logicVac, false, 0, checkOverlaps);

    // Outer dead layer
    G4Tubs *solidCrystalOuterDeadLayerSides = new G4Tubs("solidCrystalOuterDeadLayerSides", 0.5 * crystalOuterDiameter - crystalOuterDeadLayer, 0.5 * crystalOuterDiameter, 0.5 * crystalOuterLength, 0. * deg, sliceAngle);
    G4LogicalVolume *logicCrystalOuterDeadLayerSides = new G4LogicalVolume(solidCrystalOuterDeadLayerSides, MatGe, "logicCrystalOuterDeadLayerSides");
    G4VPhysicalVolume *physCrystalOuterDeadLayerSides = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * (capOuterLength - capThickness - capBaseThickness) - capThickness + holderDistanceFromCapTop + insertTopThickness + 0.5 * crystalOuterLength), logicCrystalOuterDeadLayerSides, "physCrystalOuterDeadLayerSides", logicVac, false, 0, checkOverlaps);
    G4Tubs *solidCrystalOuterDeadLayerTop = new G4Tubs("solidCrystalOuterDeadLayerTop", 0. * mm, 0.5 * crystalOuterDiameter - crystalOuterDeadLayer, 0.5 * crystalOuterDeadLayer, 0. * deg, sliceAngle);
    G4LogicalVolume *logicCrystalOuterDeadLayerTop = new G4LogicalVolume(solidCrystalOuterDeadLayerTop, MatGe, "logicCrystalOuterDeadLayerTop");
    G4VPhysicalVolume *physCrystalOuterDeadLayerTop = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * (capOuterLength - capThickness - capBaseThickness) - capThickness + holderDistanceFromCapTop + insertTopThickness + 0.5 * crystalOuterDeadLayer), logicCrystalOuterDeadLayerTop, "physCrystalOuterDeadLayerTop", logicVac, false, 0, checkOverlaps);

    regionOuterDeadLayer->AddRootLogicalVolume(logicCrystalOuterDeadLayerSides);
    regionOuterDeadLayer->AddRootLogicalVolume(logicCrystalOuterDeadLayerTop);

    // Inner dead layer
    G4Tubs *solidCrystalInnerDeadLayerSides = new G4Tubs("solidCrystalInnerDeadLayerSides", 0.5 * crystalHoleDiameter, 0.5 * crystalHoleDiameter + crystalInnerDeadLayer, 0.5 * crystalHoleLength, 0. * deg, sliceAngle);
    G4LogicalVolume *logicCrystalInnerDeadLayerSides = new G4LogicalVolume(solidCrystalInnerDeadLayerSides, MatGe, "logicCrystalInnerDeadLayerSides");
    G4VPhysicalVolume *physCrystalInnerDeadLayerSides = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * (capOuterLength - capThickness - capBaseThickness) - capThickness + holderDistanceFromCapTop + insertTopThickness + crystalOuterLength - 0.5 * crystalHoleLength), logicCrystalInnerDeadLayerSides, "physCrystalInnerDeadLayerSides", logicVac, false, 0, checkOverlaps);
    G4Tubs *solidCrystalInnerDeadLayerTop = new G4Tubs("solidCrystalInnerDeadLayerTop", 0. * mm, 0.5 * crystalHoleDiameter + crystalInnerDeadLayer, 0.5 * crystalInnerDeadLayer, 0. * deg, sliceAngle);
    G4LogicalVolume *logicCrystalInnerDeadLayerTop = new G4LogicalVolume(solidCrystalInnerDeadLayerTop, MatGe, "logicCrystalInnerDeadLayerTop");
    G4VPhysicalVolume *physCrystalInnerDeadLayerTop = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * (capOuterLength - capThickness - capBaseThickness) - capThickness + holderDistanceFromCapTop + insertTopThickness + crystalOuterLength - crystalHoleLength - 0.5 * crystalInnerDeadLayer), logicCrystalInnerDeadLayerTop, "physCrystalInnerDeadLayerTop", logicVac, false, 0, checkOverlaps);

    // Crystal
    G4Tubs *solidCrystal1 = new G4Tubs("solidCrystal1", 0. * mm, 0.5 * crystalOuterDiameter - crystalOuterDeadLayer, 0.5 * (crystalOuterLength - crystalHoleLength - crystalOuterDeadLayer - crystalInnerDeadLayer), 0. * deg, sliceAngle);
    G4Tubs *solidCrystal2 = new G4Tubs("solidCrystal2", 0.5 * crystalHoleDiameter + crystalInnerDeadLayer, 0.5 * crystalOuterDiameter - crystalOuterDeadLayer, 0.5 * (crystalHoleLength - crystalGroveDepth + crystalInnerDeadLayer), 0. * deg, sliceAngle);
    G4Tubs *solidCrystal3 = new G4Tubs("solidCrystal3", 0.5 * crystalGroveOuterDiameter, 0.5 * crystalOuterDiameter - crystalOuterDeadLayer, 0.5 * crystalGroveDepth, 0. * deg, sliceAngle);
    G4Tubs *solidCrystal4 = new G4Tubs("solidCrystal4", 0.5 * crystalHoleDiameter + crystalInnerDeadLayer, 0.5 * crystalGroveInnerDiameter, 0.5 * crystalGroveDepth, 0. * deg, sliceAngle);
    G4MultiUnion* munionCrystal = new G4MultiUnion("munionCrystal");
    munionCrystal->AddNode(*solidCrystal1, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0., 0., 0.5 * (crystalOuterLength - crystalHoleLength - crystalOuterDeadLayer - crystalInnerDeadLayer))));
    munionCrystal->AddNode(*solidCrystal2, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0., 0., (crystalOuterLength - crystalHoleLength - crystalOuterDeadLayer - crystalInnerDeadLayer) + 0.5 * (crystalHoleLength - crystalGroveDepth + crystalInnerDeadLayer))));
    munionCrystal->AddNode(*solidCrystal3, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0., 0., crystalOuterLength - crystalOuterDeadLayer - 0.5 * crystalGroveDepth)));
    munionCrystal->AddNode(*solidCrystal4, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0., 0., crystalOuterLength - crystalOuterDeadLayer - 0.5 * crystalGroveDepth)));
    munionCrystal->Voxelize();
    G4LogicalVolume *logicDetector = new G4LogicalVolume(munionCrystal, MatGe, "logicDetector");
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * (capOuterLength - capThickness - capBaseThickness) - capThickness + holderDistanceFromCapTop + insertTopThickness + crystalOuterDeadLayer), logicDetector, "physDetector", logicVac, false, 0, checkOverlaps);

    // Show pretty colors in the visualization
    G4VisAttributes *capVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 0.5));
    capVisAtt->SetForceSolid(true);
    logicCap->SetVisAttributes(capVisAtt);
    G4VisAttributes *vacVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 1.0, 0.5));
    vacVisAtt->SetForceSolid(true);
    logicVac->SetVisAttributes(vacVisAtt);
    G4VisAttributes *windVisAtt = new G4VisAttributes(G4Color(0.0, 0.0, 1.0, 0.5));
    windVisAtt->SetForceSolid(true);
    logicWindow->SetVisAttributes(windVisAtt);
    G4VisAttributes *holderVisAtt = new G4VisAttributes(G4Color(0.0, 1.0, 0.0, 0.5));
    holderVisAtt->SetForceSolid(true);
    logicHolder->SetVisAttributes(holderVisAtt);
    // G4VisAttributes *deadVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 1.0, 0.5));
    // deadVisAtt->SetForceSolid(true);
    // logicInnerDeadLayer->SetVisAttributes(deadVisAtt);
    // logicOuterDeadLayer->SetVisAttributes(deadVisAtt);
    // G4VisAttributes *insertVisAtt = new G4VisAttributes(G4Color(0.0, 1.0, 1.0, 0.5));
    // insertVisAtt->SetForceSolid(true);
    // logicInsert->SetVisAttributes(insertVisAtt);
    G4VisAttributes *detVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.5));
    detVisAtt->SetForceSolid(true);
    logicDetector->SetVisAttributes(detVisAtt);

    return logicDetector;
}