#include "event.hh"

MyEventAction::MyEventAction(MyRunAction* run)
{
    fEdep = 0.;
    fEdepHpge = 0.;
    fRun = run;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.;
    fEdepHpge = 0.;
    fCrossed = false;
}

void MyEventAction::EndOfEventAction(const G4Event* anEvent)
{
    // G4cout << "Energy deposition: " << fEdep / keV << " keV" << G4endl;

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->FillNtupleDColumn(0, 4, fEdep / keV);
    man->FillNtupleDColumn(1, 0, fEdepHpge / keV);

    man->AddNtupleRow(0);
    man->AddNtupleRow(1);

    if (fCrossed)
        fRun->AddOneCross();

    G4PrimaryVertex* primaryVertex = anEvent->GetPrimaryVertex(0);
    if (!primaryVertex) {
        G4cerr << "No primary vertex found!" << G4endl;
        return;
    }

    // Get the primary particle
    G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary();
    if (!primaryParticle) {
        G4cerr << "No primary particle found!" << G4endl;
        return;
    }

    // Get the kinetic energy of the primary particle
    G4double primaryEnergy = primaryParticle->GetKineticEnergy();

    if (fEdepHpge == primaryEnergy)
        fRun->AddOnePhotoevent();
}