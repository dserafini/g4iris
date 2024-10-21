#include "run.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->CreateNtuple("Photons", "Photons");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fEini");
    man->CreateNtupleDColumn("fEdep");
    man->FinishNtuple(0);

    man->CreateNtuple("Hpge", "Hpge");
    man->CreateNtupleDColumn("fEdep");
    man->FinishNtuple(1);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    // man->OpenFile("output"+strRunID.str()+".root");
    man->OpenFile("output.root");

    fPhotopeakCount = 0;
}

void MyRunAction::EndOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();

    G4int nEvents = run->GetNumberOfEvent();
    G4cout << "Crossed " << fCross << " / " << nEvents << G4endl;
    G4cout << "Photopeak events in the HPGe " << fPhotopeakCount << " / " << nEvents << G4endl;
}