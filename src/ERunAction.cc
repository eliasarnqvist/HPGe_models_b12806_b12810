#include "ERunAction.hh"

ERunAction::ERunAction()
{
    fMessenger = new G4GenericMessenger(this, "/E_file_settings/", "Settings for the filename");
	fMessenger->DeclareProperty("fileName", fileName, "Name of the output file");
    fileName = "output_temporary.root";

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->CreateH1("Edet_b12806", "Detected energy", 2048, 0, 3 * MeV);
    analysisManager->CreateH1("Edet_b12810", "Detected energy", 2048, 0, 3 * MeV);
    
    // analysisManager->CreateNtuple("Scoring", "Scoring");
    // analysisManager->CreateNtupleIColumn("iEvent");
    // analysisManager->CreateNtupleDColumn("energy");
    // analysisManager->FinishNtuple(0);
}

ERunAction::~ERunAction()
{
    
}

void ERunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    // G4int runID = run->GetRunID();
    // std::stringstream strRunID;
    // strRunID << runID;

    // fileName = "output" + strRunID.str() + ".root";

    // Output root file
    analysisManager->OpenFile(fileName);
}

void ERunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    // Write data to our output root file and then close the file
    analysisManager->Write();
    analysisManager->CloseFile();

    G4int runID = run->GetRunID();

    G4cout << "Finishing run " << runID << G4endl;
}