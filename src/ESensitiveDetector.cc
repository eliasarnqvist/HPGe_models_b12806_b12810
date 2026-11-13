#include "ESensitiveDetector.hh"

ESensitiveDetector::ESensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    fTotalEnergyDeposited_b12806 = 0.;
    fTotalEnergyDeposited_b12810 = 0.;
}

ESensitiveDetector::~ESensitiveDetector()
{
    
}

void ESensitiveDetector::Initialize(G4HCofThisEvent *)
{
    fTotalEnergyDeposited_b12806 = 0.;
    fTotalEnergyDeposited_b12810 = 0.;
}

G4double GaussianPeak(G4double depositedEnergy, G4double a, G4double b, G4double c)
{
    // Careful with MeV and keV units...
    G4double sigma = std::sqrt(a*a + b*b*depositedEnergy*1e3 + c*c*depositedEnergy*depositedEnergy*1e6) / 2.355 / 1e3;
    G4double updatedEnergy = G4RandGauss::shoot(depositedEnergy, sigma);
    return updatedEnergy;
}

void ESensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
    if (fTotalEnergyDeposited_b12806 > 0)
    {
        fUpdatedEnergyDeposited_b12806 = GaussianPeak(fTotalEnergyDeposited_b12806, a_b12806, b_b12806, c_b12806);
        analysisManager->FillH1(0, fUpdatedEnergyDeposited_b12806);
    }
    if (fTotalEnergyDeposited_b12810 > 0)
    {
        fUpdatedEnergyDeposited_b12810 = GaussianPeak(fTotalEnergyDeposited_b12810, a_b12810, b_b12810, c_b12810);
        analysisManager->FillH1(1, fUpdatedEnergyDeposited_b12810);
    }

    // analysisManager->FillNtupleIColumn(0, 0, eventID);
    // analysisManager->FillNtupleDColumn(0, 0, fTotalEnergyDeposited);
    // analysisManager->AddNtupleRow(0);

    // G4cout << "Deposited energy: " << fTotalEnergyDeposited_0 << G4endl;
}

G4bool ESensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4double fEnergyDeposited = aStep->GetTotalEnergyDeposit();

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    // The 2 here means the second level of the touchable hierarchy
    // In other words, the parent's parent
    // Or the CopyNo of the can housing the detector
    G4int copyNo = touchable->GetCopyNumber(2);

    // G4cout << "Copy number: " << copyNo << G4endl;

    if (fEnergyDeposited > 0)
    {
        if (copyNo == 0)
        {
            fTotalEnergyDeposited_b12806 += fEnergyDeposited;
        }
        if (copyNo == 1)
        {
            fTotalEnergyDeposited_b12810 += fEnergyDeposited;
        }
    }
    
    return true;
}