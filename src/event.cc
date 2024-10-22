#include "event.hh"

MyEventAction::MyEventAction(MyRunAction* run)
{
    fRun = run;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
    fCrossed = false;
}

void MyEventAction::EndOfEventAction(const G4Event* anEvent)
{
    G4cout << "MyEventAction::EndOfEventAction" << G4endl;
    // G4cout << "Energy deposition: " << fEdep / keV << " keV" << G4endl;

    if ( fHpgeHCID == -1 )
        fHpgeHCID = G4SDManager::GetSDMpointer()->GetCollectionID("hpgeHitsCollection");
    hpgeHitsCollection* hpgeHC = GetHitsCollection(fHpgeHCID, anEvent);
    hpgeHit * oneHit;
    G4double eventEnergy = 0;
    G4int nofHits = hpgeHC->entries();
    if (nofHits > 0)
    {
        oneHit = (*hpgeHC)[nofHits-1];
        for ( G4int i=0; i<nofHits; i++ )
            eventEnergy += (*hpgeHC)[i]->GetEdep();
    }
    else
        G4cout << "no hits here" << G4endl;

    // Print per event (modulo n)
    //
    auto eventID = anEvent->GetEventID();
    auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) && oneHit ) {
        PrintEventStatistics( oneHit->GetEdep());
        G4cout << "--> End of event: " << eventID << "\n" << G4endl;
    }

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->AddNtupleRow(0);
    man->AddNtupleRow(1);

    if (fCrossed)
        fRun->AddOneCross();

    G4double primaryEnergy = anEvent->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();
    if (eventEnergy == primaryEnergy)
        fRun->AddOnePhotoevent();
}

void MyEventAction::PrintEventStatistics(G4double absoEdep) const
{
    G4cout << "MyEventAction::PrintEventStatistics" << G4endl;
  // print event statistics
  G4cout
     << "   Absorber: total energy: "
     << std::setw(7) << G4BestUnit(absoEdep, "Energy")
     << G4endl;
}

hpgeHitsCollection* MyEventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
    G4cout << "MyEventAction::GetHitsCollection" << G4endl;
  auto hitsCollection
    = static_cast<hpgeHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));

  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("EventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}