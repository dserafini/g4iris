#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
  G4cout << "MyDetectorConstruction::MyDetectorConstruction" << G4endl;

  fMessengerTablet = new G4GenericMessenger(this, "/tablet/", "Tablet properties");
  fMessengerTablet->DeclarePropertyWithUnit("thickness", "mm", tabletThickness, "Thickness of the tablet");
  tabletThickness = 1.5 * mm;
  
  fMessengerHpge = new G4GenericMessenger(this, "/hpge/", "HPGe detector properties");
  fMessengerHpge->DeclarePropertyWithUnit("distance", "mm", hpgeFaceCentreDistance, "HPGe detector - source distance");
  hpgeFaceCentreDistance = 3 * cm;

  fMessengerPoint = new G4GenericMessenger(this, "/point/", "Detection point properties");
  fMessengerPoint->DeclarePropertyWithUnit("xpos", "mm", pointXpos, "X position of the tablet");
  pointXpos = 1 * cm;

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
  hpgeDiameter = 76 * mm; // assumption based on datasheet
  hpgeThickness = hpgeDiameter; // assumption
  hpgePosition = G4ThreeVector(0, 0, hpgeFaceCentreDistance + hpgeThickness / 2 + pointThickness);
  solidHpge = new G4Tubs("solidHpge", 0, hpgeDiameter / 2, hpgeThickness / 2, 0, 360 * deg);
  logicalHpge = new G4LogicalVolume(solidHpge, germanium, "logicalHpge");
  new G4PVPlacement(nullptr, hpgePosition, logicalHpge, "physHpge", logicWorld, false, 0, checkOverlaps);
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
  tabletDiameter = 13 * mm;
  tabletPosition = G4ThreeVector(0, 0, 0); // source is 2 um from the tablet surface, no sense bu it's temporary

  // detection point
  pointSide = 2 * cm;
  pointDiameter = tabletDiameter + 2 * um;
  pointThickness = tabletThickness + 2 * um;
  pointPosition = G4ThreeVector(pointXpos, 0, + pointThickness / 2);
  solidPoint = new G4Tubs("solidPoint", 0, pointDiameter / 2, pointThickness / 2, 0, 360 * deg);
  logicalPoint = new G4LogicalVolume(solidPoint, air, "logicalPoint");
  new G4PVPlacement(nullptr, pointPosition, logicalPoint, "physPoint", logicWorld, false, 0, checkOverlaps);

  // tablet
  solidTablet = new G4Tubs("solidTablet", 0, tabletDiameter / 2, tabletThickness / 2, 0, 360 * deg);
  logicalTablet = new G4LogicalVolume(solidTablet, sorbitol, "logicalTablet");
  new G4PVPlacement(nullptr, tabletPosition, logicalTablet, "physTablet", logicalPoint, false, 0, checkOverlaps);

  // HPGe
  BuildHpge();

  // always return the physical World
  return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
  MySensitiveDetector *sensDetHpge = new MySensitiveDetector("SensitiveDetectorHpge");

  if(logicalHpge != NULL)
    logicalHpge->SetSensitiveDetector(sensDetHpge);
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDetHpge);
}