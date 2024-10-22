#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
  G4cout << "MyDetectorConstruction::MyDetectorConstruction" << G4endl;

  fMessengerTablet = new G4GenericMessenger(this, "/tablet/", "Tablet properties");
  fMessengerTablet->DeclarePropertyWithUnit("thickness", "mm", tabletThickness, "Thickness of the tablet");
  fMessengerTablet->DeclarePropertyWithUnit("xpos", "mm", tabletXpos, "X position of the tablet");
  tabletThickness = 1.5 * mm;
  tabletXpos = 0 * cm;
  
  fMessengerHpge = new G4GenericMessenger(this, "/hpge/", "HPGe detector properties");
  fMessengerHpge->DeclarePropertyWithUnit("distance", "mm", hpgeFaceCentreDistance, "HPGe detector - source distance");
  hpgeFaceCentreDistance = 0 * cm;
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

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
  germanium = new G4Material("germanium", 5.323 * g / cm3, 1);
  germanium->AddElement(nist->FindOrBuildElement("Ge"), 1);
}

void MyDetectorConstruction::BuildHpge()
{
  hpgeDiameter = 2 * 76 * mm; // assumption based on datasheet
  hpgeThickness = hpgeDiameter * .75; // assumption
  hpgePosition = G4ThreeVector(0, 0, hpgeFaceCentreDistance + hpgeThickness / 2 + tabletThickness);
  solidHpge = new G4Tubs("solidHpge", 0, hpgeDiameter / 2, hpgeThickness / 2, 0, 360 * deg);
  logicalHpge = new G4LogicalVolume(solidHpge, germanium, "logicalHpge");
  new G4PVPlacement(nullptr, hpgePosition, logicalHpge, "physHpge", logicWorld, false, 0, checkOverlaps);
}

void MyDetectorConstruction::BuildTablet()
{
  tabletDiameter = 13 * mm;
  tabletPosition = G4ThreeVector(0, 0, + tabletThickness / 2); // primaries are generated on tablet surface
  solidTablet = new G4Tubs("solidTablet", 0, tabletDiameter / 2, tabletThickness / 2, 0, 360 * deg);
  logicalTablet = new G4LogicalVolume(solidTablet, sorbitol, "logicalTablet");
  new G4PVPlacement(nullptr, tabletPosition, logicalTablet, "physTablet", logicWorld, false, 0, checkOverlaps);
}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Option to switch on/off checking of volumes overlaps
  checkOverlaps = true;

  // World
  G4double world_sizeXY = 25 * cm;
  G4double world_sizeZ  = 25 * cm;

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