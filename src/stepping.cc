#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{   
    // G4cout << "MySteppingAction::UserSteppingAction" << G4endl;
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    G4ParticleDefinition * partdef = step->GetTrack()->GetDynamicParticle()->GetDefinition();
    G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

    if (partdef == G4Gamma::Definition())
        G4cout << "eventID " << eventID << "\t " << partdef->GetParticleName() << " in " << volume->GetName() << G4endl;
}