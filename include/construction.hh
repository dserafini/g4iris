#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    G4VPhysicalVolume* Construct() override;
    void DefineMaterials();
private:
    G4Material *air, *sorbitol;

    // tablet
    G4double tabletDiameter = 13 * mm;
    G4double tabletThickness = 1.5 * mm;
    G4Tubs *solidTablet;
    G4LogicalVolume *logicalTablet;
};

#endif