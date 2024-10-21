#include "event.hh"

MyEventAction::MyEventAction(MyRunAction* run)
{
    fEdep = 0.;
    fRun = run;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.;
    fCrossed = false;
}

void MyEventAction::EndOfEventAction(const G4Event* anEvent)
{
    // G4cout << "Energy deposition: " << fEdep / keV << " keV" << G4endl;

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->FillNtupleDColumn(0, 4, fEdep / keV);

    man->AddNtupleRow(0);

    if (fCrossed)
        fRun->AddOneCross();
}