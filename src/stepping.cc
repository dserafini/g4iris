#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{   
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    const G4LogicalVolume *logicalHpge = G4LogicalVolumeStore::GetInstance()->GetVolume("logicalHpge");

    if(volume == logicalHpge && !fEventAction->isCrossed())
        fEventAction->Cross();
}