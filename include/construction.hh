#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "detector.hh"
#include "G4SDManager.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;
    void DefineMaterials();
    void BuildHpge();
private:
    G4bool checkOverlaps;

    // world
    G4Material *air;
    G4Box *solidWorld;
    G4LogicalVolume *logicWorld;

    // tablet point
    G4double pointSide, pointDiameter, pointThickness;
    G4Tubs *solidPoint;
    G4LogicalVolume *logicalPoint;

    // tablet
    G4Material *sorbitol;
    G4double tabletDiameter;
    G4double tabletThickness;
    G4ThreeVector tabletPosition;
    G4Tubs *solidTablet;
    G4LogicalVolume *logicalTablet;
    G4GenericMessenger *fMessengerTablet;

    // HPGe
    G4Material *germanium;
    G4double hpgeDiameter;
    G4double hpgeThickness;
    G4double hpgeFaceCentreDistance;
    G4ThreeVector hpgePosition;
    G4Tubs *solidHpge;
    G4LogicalVolume *logicalHpge;
    G4GenericMessenger *fMessengerHpge;
};

#endif