#include "hpgeSD.hh"

MySensitiveHpge::MySensitiveHpge(G4String name, const G4String& hitsCollectionName) : G4VSensitiveDetector(name)
{
    fHitsCollection = nullptr;
    collectionName.insert(hitsCollectionName);
}

MySensitiveHpge::~MySensitiveHpge()
{}

void MySensitiveHpge::Initialize(G4HCofThisEvent* hce)
{
    // G4cout << "MySensitiveHpge::Initialize" << G4endl;
    fEdepHpge = 0.;
    fHpgeCrossed = false;
    fHitsCollection = new hpgeHitsCollection(SensitiveDetectorName, collectionName[0]);
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, fHitsCollection );

    // G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    // G4cout << "ready for Event ID: " << eventID << G4endl;
}

G4bool MySensitiveHpge::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // G4cout << "MySensitiveHpge::ProcessHits" << G4endl;
    // track->SetTrackStatus(fStopAndKill);

    // G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

    if (!fHpgeCrossed)
        fHpgeCrossed = true;
    
    G4double edep = aStep->GetTotalEnergyDeposit();
    fEdepHpge += edep;

    hpgeHit* newHit = new hpgeHit();
    newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
    newHit->SetEdep(edep);
    if (fHitsCollection)
        fHitsCollection->insert( newHit );

    // G4cout << "Event ID: " << eventID << "\t " << fHpgeCrossed << "\t dep " << edep / keV << " keV" << G4endl;
    return true;
}

void MySensitiveHpge::EndOfEvent(G4HCofThisEvent*)
{
    // G4cout << "MySensitiveHpge::EndOfEvent" << G4endl;

    G4int nofHits = fHitsCollection->entries();
    G4double totalEdep = 0;
    for ( G4int i=0; i<nofHits; i++ )
      totalEdep += (*fHitsCollection)[i]->GetEdep();
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(1, 0, totalEdep / keV);
    if (fHpgeCrossed)
        man->FillNtupleIColumn(1, 1, 1);
    else
        man->FillNtupleIColumn(1, 1, 0);
}