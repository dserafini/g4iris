#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "analysis.hh"

class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction();
    ~MyRunAction();
    
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void AddOneCross() { fCross++; }
    void AddOnePhotoevent() { fPhotopeakCount++; }

private:
    G4int fCross;
    G4int fPhotopeakCount;
};

#endif