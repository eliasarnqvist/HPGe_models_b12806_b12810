#include "EPhysicsList.hh"

EPhysicsList::EPhysicsList()
{
    // Register the physics lists with G4VModularPhysicsList
    // EM physics
    RegisterPhysics(new G4EmStandardPhysics_option4());
    // Decay physics
    RegisterPhysics(new G4DecayPhysics());
    // Radioactive decay
    RegisterPhysics(new G4RadioactiveDecayPhysics());
    // Hadronic physics
    RegisterPhysics(new G4HadronPhysicsFTFP_BERT());

    // G4EmParameters::Instance()->SetDefaults();
    // G4EmParameters::Instance()->SetAugerCascade(true);
    // G4EmParameters::Instance()->SetDeexcitationIgnoreCut(true);

    // update G4NuclideTable time limit
    const G4double meanLife = 1*picosecond;
    G4NuclideTable::GetInstance()->SetMeanLifeThreshold(meanLife);
    G4NuclideTable::GetInstance()->SetLevelTolerance(1.0*eV);
    
    // define flags for nuclear gamma de-excitation model
    G4DeexPrecoParameters* deex = G4NuclearLevelData::GetInstance()->GetParameters();
    deex->SetCorrelatedGamma(true);
    deex->SetStoreAllLevels(true);
    deex->SetInternalConversionFlag(true);
    deex->SetIsomerProduction(true);
    deex->SetMaxLifeTime(meanLife);
}

EPhysicsList::~EPhysicsList()
{
    
}
