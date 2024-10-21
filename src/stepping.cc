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

    const G4LogicalVolume *logicalTablet = G4LogicalVolumeStore::GetInstance()->GetVolume("logicalTablet");
    const G4LogicalVolume *logicalPoint = G4LogicalVolumeStore::GetInstance()->GetVolume("logicalPoint");

    if(volume == logicalPoint && !fEventAction->isCrossed())
        fEventAction->Cross();
    
    if(volume == logicalTablet)
    {
        G4double edep = step->GetTotalEnergyDeposit();
        fEventAction->AddEdep(edep);
    }
}