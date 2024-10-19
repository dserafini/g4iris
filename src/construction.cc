#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    G4cout << "MyDetectorConstruction::MyDetectorConstruction" << G4endl;
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
  G4NistManager *nist = G4NistManager::Instance();

  air = nist->FindOrBuildMaterial("G4_AIR");
  sorbitol = new G4Material("sorbitol", 1.4 * g/cm3, 3);

  sorbitol->AddElement(nist->FindOrBuildElement("C"), 6);
  sorbitol->AddElement(nist->FindOrBuildElement("H"), 14);
  sorbitol->AddElement(nist->FindOrBuildElement("O"), 6);
}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  // World
  G4double world_sizeXY = 10 * cm;
  G4double world_sizeZ  = 10 * cm;

  auto solidWorld = new G4Box("World",                           // its name
    0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
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
  solidTablet = new G4Tubs("solidTablet", 0, tabletDiameter / 2, tabletThickness / 2, 0, 360 * deg);
  logicalTablet = new G4LogicalVolume(solidTablet, sorbitol, "logicalTablet");
  new G4PVPlacement(nullptr, G4ThreeVector(), logicalTablet, "physTablet", logicWorld, false, 0, checkOverlaps);

  // always return the physical World
  return physWorld;
}