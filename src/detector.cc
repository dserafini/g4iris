#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

void MySensitiveDetector::Initialize(G4HCofThisEvent*)
{
    // G4cout << "MySensitiveDetector::Initialize" << G4endl;
    fEdepHpge = 0.;
}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // G4cout << "MySensitiveDetector::ProcessHits" << G4endl;
    // track->SetTrackStatus(fStopAndKill);
    
    G4double edep = aStep->GetTotalEnergyDeposit();
    fEdepHpge += edep;
    
    return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    // G4cout << "MySensitiveDetector::EndOfEvent" << G4endl;
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(1, 0, fEdepHpge / keV);
    man->AddNtupleRow(1);

    G4RunManager* runManager = G4RunManager::GetRunManager();
    const G4Event* anEvent = runManager->GetCurrentEvent();
    G4PrimaryVertex* primaryVertex = anEvent->GetPrimaryVertex(0);
    G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary();
    G4double primaryEnergy = primaryParticle->GetKineticEnergy();
    if (fEdepHpge == primaryEnergy)
    {
        // Cast al tuo MyRunAction
        const MyRunAction* constRunAction = static_cast<const MyRunAction*>(runManager->GetUserRunAction());
        MyRunAction* runAction = const_cast<MyRunAction*>(constRunAction);
        if (runAction)
            runAction->AddOnePhotoevent();
    }
    // G4cout << "energy dep " << fEdepHpge / keV << " keV" << G4endl;
}