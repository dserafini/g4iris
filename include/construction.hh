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
#include "hpgeSD.hh"
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
    void BuildTablet();
private:
    G4bool checkOverlaps;

    // world
    G4Material *air;
    G4Box *solidWorld;
    G4LogicalVolume *logicWorld;

    // tablet
    G4Material *sorbitol;
    G4double tabletDiameter;
    G4double tabletThickness;
    G4double tabletXpos;
    G4double tabletZpos;
    G4double tabletYrot;
    G4ThreeVector tabletPosition;
    G4RotationMatrix* rotation;
    G4Tubs *solidTablet;
    G4LogicalVolume *logicalTablet;
    G4GenericMessenger *fMessengerTablet;

    // HPGe
    G4Material *germanium;
    G4double hpgeDiameter;
    G4double hpgeThickness;
    G4double hpgeActiveVolumeDiameter;
    G4double hpgeActiveVolumeThickness;
    G4double hpgeCaseDiameter;
    G4double hpgeCaseThickness;
    G4double hpgeCaseWallThickness;
    G4double hpgeCapDiameter;
    G4double hpgeCapThickness;
    G4double hpgeFaceCentreDistance;
    G4ThreeVector hpgePosition;
    G4Tubs *solidHpge;
    G4LogicalVolume *logicalHpge;
    G4Tubs *solidHpgeActiveVolume;
    G4LogicalVolume *logicalHpgeActiveVolume;
    G4Tubs *solidHpgeCase;
    G4LogicalVolume *logicalHpgeCase;
    G4Tubs *solidHpgeCap;
    G4LogicalVolume *logicalHpgeCap;
    G4GenericMessenger *fMessengerHpge;
};

#endif