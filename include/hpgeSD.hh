#ifndef HPGESD_HH
#define HPGESD_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "run.hh"
#include "hpgeHit.hh"

class MySensitiveHpge : public G4VSensitiveDetector
{
public:
    MySensitiveHpge(G4String, const G4String&);
    ~MySensitiveHpge();
    
private:
    G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
    void Initialize(G4HCofThisEvent*) override;
    void EndOfEvent(G4HCofThisEvent*) override;
    G4double fEdepHpge;
    G4bool fHpgeCrossed;
    hpgeHitsCollection* fHitsCollection;
};

#endif