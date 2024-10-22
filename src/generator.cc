#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("chargedgeantino");

    G4ThreeVector pos(0.,0.,0.);
    G4ThreeVector mom(0.,0.,1.);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleMomentum(100.*GeV);
    fParticleGun->SetParticleDefinition(particle);

    fParticleGps = new G4GeneralParticleSource();
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // G4cout << "MyPrimaryGenerator::GeneratePrimaries" << G4endl;
    // G4double energy = 10. * keV;
    // fParticleGun->SetParticleDefinition(G4Gamma::Definition());
    // fParticleGun->SetParticleEnergy( energy );
    fParticleGps->GeneratePrimaryVertex(anEvent);

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    // man->FillNtupleDColumn(0, 3, fParticleGun->GetParticleEnergy() / keV); // initial energy
    // fParticleGun->GeneratePrimaryVertex(anEvent);
    // G4ParticleDefinition* particle = fParticleGun->GetParticleDefinition();
    // G4cout << "generated " << fParticleGun->GetParticleDefinition()->GetParticleName() ;
    // G4cout << " of energy " << fParticleGun->GetParticleEnergy() / keV << " keV" << G4endl;

    man->FillNtupleDColumn(0, 3, fParticleGps->GetParticleEnergy() / keV); // initial energy
    man->FillNtupleDColumn(0, 0, fParticleGps->GetParticlePosition().getX() / mm); // [mm]
    man->FillNtupleDColumn(0, 1, fParticleGps->GetParticlePosition().getY() / mm); // [mm]
    man->FillNtupleDColumn(0, 2, fParticleGps->GetParticlePosition().getZ() / mm); // [mm]
    // G4int eventID = anEvent->GetEventID();
    // G4cout << "eventID " << eventID << "\t pos " << fParticleGps->GetParticlePosition() << G4endl;
}