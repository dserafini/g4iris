#ifndef TABLETSD_HH
#define TABLETSD_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "run.hh"

class MySensitiveTablet : public G4VSensitiveDetector
{
public:
    MySensitiveTablet(G4String);
    ~MySensitiveTablet();
    
private:
    G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
    void Initialize(G4HCofThisEvent*) override;
    void EndOfEvent(G4HCofThisEvent*) override;
    G4double fEdepTablet;
};

#endif