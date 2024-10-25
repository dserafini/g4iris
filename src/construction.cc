#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
  G4cout << "MyDetectorConstruction::MyDetectorConstruction" << G4endl;

  fMessengerTablet = new G4GenericMessenger(this, "/tablet/", "Tablet properties");
  fMessengerTablet->DeclarePropertyWithUnit("thickness", "mm", tabletThickness, "Thickness of the tablet");
  fMessengerTablet->DeclarePropertyWithUnit("xpos", "mm", tabletXpos, "X position of the tablet");
  fMessengerTablet->DeclarePropertyWithUnit("zpos", "mm", tabletZpos, "Z position of the tablet");
  fMessengerTablet->DeclarePropertyWithUnit("yrot", "deg", tabletYrot, "Y rotation of the tablet");
  tabletThickness = 1.5 * mm;
  tabletXpos = 0 * mm;
  tabletZpos = - tabletThickness / 2;
  tabletYrot = 0 * deg;
  
  // keeped for reference but I should only move the tablet
  fMessengerHpge = new G4GenericMessenger(this, "/hpge/", "HPGe detector properties");
  fMessengerHpge->DeclarePropertyWithUnit("diameter", "mm", hpgeDiameter, "HPGe detector diameter");
  // fMessengerHpge->DeclarePropertyWithUnit("distance", "mm", hpgeFaceCentreDistance, "HPGe detector - source distance");
  hpgeDiameter = 76 * mm; // assumption based on datasheet
  hpgeFaceCentreDistance = 0 * cm;
  hpgeActiveVolumeDiameter = 70 * mm;
  hpgeCapThickness = .5 * mm;
  hpgeCaseWallThickness = .75 * mm;
}

MyDetectorConstruction::~MyDetectorConstruction()
{
	delete fMessengerTablet;
}

void MyDetectorConstruction::DefineMaterials()
{
  G4NistManager *nist = G4NistManager::Instance();

  // air
  air = nist->FindOrBuildMaterial("G4_AIR");

  // sorbitol
  G4double density = 275 * mg / ( CLHEP::pi * (13 / 2 * mm) * 1.5 * mm );
  sorbitol = new G4Material("sorbitol", density, 3);
  sorbitol->AddElement(nist->FindOrBuildElement("C"), 6);
  sorbitol->AddElement(nist->FindOrBuildElement("H"), 14);
  sorbitol->AddElement(nist->FindOrBuildElement("O"), 6);

  // germanium
  germanium = nist->FindOrBuildMaterial("G4_Ge");

  // aluminum
  aluminum = nist->FindOrBuildMaterial("G4_Al");

  // beryllium
  beryllium = nist->FindOrBuildMaterial("G4_Be");
}

void MyDetectorConstruction::BuildHpge()
{
  hpgeThickness = hpgeDiameter; // assumption
  hpgePosition = G4ThreeVector(0, 0, hpgeFaceCentreDistance + hpgeThickness / 2);
  solidHpge = new G4Tubs("solidHpge", 0, hpgeDiameter / 2, hpgeThickness / 2, 0, 360 * deg);
  logicalHpge = new G4LogicalVolume(solidHpge, air, "logicalHpge");
  new G4PVPlacement(nullptr, hpgePosition, logicalHpge, "physHpge", logicWorld, false, 0, checkOverlaps);

  // add aluminum case
  G4double hpgeCaseOuterRadius = hpgeDiameter / 2;
  G4double hpgeCaseInnerRadius = hpgeCaseOuterRadius - hpgeCaseWallThickness;
  solidHpgeCase = new G4Tubs("solidHpgeCase", hpgeCaseInnerRadius, hpgeCaseOuterRadius, hpgeThickness / 2, 0, 360 * deg);
  logicalHpgeCase = new G4LogicalVolume(solidHpgeCase, aluminum, "logicalHpgeCase");
  new G4PVPlacement(nullptr, G4ThreeVector(), logicalHpgeCase, "physHpgeCase", logicalHpge, false, 0, checkOverlaps);

  // add beryllium cap
  G4double hpgeCapRadius = hpgeCaseInnerRadius;
  G4ThreeVector hpgeCapPosition = G4ThreeVector(0, 0, - hpgeThickness / 2 + hpgeCapThickness / 2);
  solidHpgeCap = new G4Tubs("solidHpgeCap", 0, hpgeCapRadius, hpgeCapThickness / 2, 0, 360 * deg);
  logicalHpgeCap = new G4LogicalVolume(solidHpgeCap, beryllium, "logicalHpgeCap");
  new G4PVPlacement(nullptr, hpgeCapPosition, logicalHpgeCap, "physHpgeCap", logicalHpge, false, 0, checkOverlaps);

  // add active volume
  hpgeActiveVolumeThickness = hpgeActiveVolumeDiameter;
  solidHpgeActiveVolume = new G4Tubs("solidHpgeActiveVolume", 0, hpgeActiveVolumeDiameter / 2, hpgeActiveVolumeThickness / 2, 0, 360 * deg);
  logicalHpgeActiveVolume = new G4LogicalVolume(solidHpgeActiveVolume, germanium, "logicalHpgeActiveVolume");
  new G4PVPlacement(nullptr, G4ThreeVector(), logicalHpgeActiveVolume, "physHpgeActiveVolume", logicalHpge, false, 0, checkOverlaps);
}

void MyDetectorConstruction::BuildTablet()
{
  tabletDiameter = 13 * mm;
  tabletPosition = G4ThreeVector(0, 0, tabletZpos); // primaries are generated on tablet surface
  rotation = new G4RotationMatrix();
  rotation->rotateY( tabletYrot );
  solidTablet = new G4Tubs("solidTablet", 0, tabletDiameter / 2, tabletThickness / 2, 0, 360 * deg);
  logicalTablet = new G4LogicalVolume(solidTablet, sorbitol, "logicalTablet");
  new G4PVPlacement(rotation, tabletPosition, logicalTablet, "physTablet", logicWorld, false, 0, checkOverlaps);
}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Option to switch on/off checking of volumes overlaps
  checkOverlaps = true;

  // World
  G4double world_sizeXY = 50 * cm;
  G4double world_sizeZ  = 50 * cm;

  solidWorld = new G4Box("World",                           // its name
    0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  logicWorld = new G4LogicalVolume(solidWorld,  // its solid
    air,                                       // its material
    "World");                                        // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                           // at (0,0,0)
    logicWorld,                                // its logical volume
    "World",                                   // its name
    nullptr,                                   // its mother  volume
    false,                                     // no boolean operation
    0,                                         // copy number
    checkOverlaps);                            // overlaps checking

  // tablet
  BuildTablet();

  // HPGe
  BuildHpge();

  // always return the physical World
  return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
  MySensitiveHpge *sensDetHpge = new MySensitiveHpge("SensitiveDetectorHpge", "hpgeHitsCollection");

  if(logicalHpge != NULL)
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDetHpge);
    logicalHpge->SetSensitiveDetector(sensDetHpge);
}
