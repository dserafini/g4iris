#ifndef HPGEHIT_HH
#define HPGEHIT_HH 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class hpgeHit : public G4VHit
{
  public:
    hpgeHit() = default;
    hpgeHit(const hpgeHit&) = default;
    ~hpgeHit() override = default;

    // operators
    hpgeHit& operator=(const hpgeHit&) = default;
    G4bool operator==(const hpgeHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    void Print() override;
    void Draw() override;

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetEdep     (G4double de)      { fEdep = de; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4double GetEdep() const     { return fEdep; };

  private:
    G4int         fTrackID = -1;
    G4double      fEdep = 0.;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using hpgeHitsCollection = G4THitsCollection<hpgeHit>;

extern G4ThreadLocal G4Allocator<hpgeHit>* hpgeHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* hpgeHit::operator new(size_t)
{
  if(!hpgeHitAllocator)
      hpgeHitAllocator = new G4Allocator<hpgeHit>;
  return (void *) hpgeHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void hpgeHit::operator delete(void *hit)
{
  hpgeHitAllocator->FreeSingle((hpgeHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif