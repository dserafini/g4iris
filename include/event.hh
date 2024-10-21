#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

#include "run.hh"

class MyEventAction : public G4UserEventAction
{
public:
    MyEventAction(MyRunAction*);
    ~MyEventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    
    void Cross() {fCrossed = true; }
    bool isCrossed() {return fCrossed; }
    
private:
    G4bool fCrossed;
    MyRunAction* fRun;
};

#endif