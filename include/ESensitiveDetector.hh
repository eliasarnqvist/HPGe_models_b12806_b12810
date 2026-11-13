#ifndef ESENSITIVEDETECTOR_HH
#define ESENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"

class ESensitiveDetector : public G4VSensitiveDetector
{
public:
    ESensitiveDetector(G4String);
    ~ESensitiveDetector();

private:
    G4double fTotalEnergyDeposited_b12806;
    G4double fTotalEnergyDeposited_b12810;
    G4double fUpdatedEnergyDeposited_b12806;
    G4double fUpdatedEnergyDeposited_b12810;
    // Parameters for the specific HPGe detectors
    // b12806
    G4float a_b12806 = 1.19608129e+00;
    G4float b_b12806 = 4.03592826e-02;
    G4float c_b12806 = 3.16773356e-04;
    // b12810
    G4float a_b12810 = 1.17465223e+00;
    G4float b_b12810 = 4.11479228e-02;
    G4float c_b12810 = 4.91060172e-04;

    virtual void Initialize(G4HCofThisEvent *) override;
    virtual void EndOfEvent(G4HCofThisEvent *) override;

    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif