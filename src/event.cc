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
    // G4cout << "Energy deposition: " << fEdep / keV << " keV" << G4endl;

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->AddNtupleRow(0);
    man->AddNtupleRow(1);

    if (fCrossed)
        fRun->AddOneCross();
}