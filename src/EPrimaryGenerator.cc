#include "EPrimaryGenerator.hh"

EPrimaryGenerator::EPrimaryGenerator()
{
    fMessenger = new G4GenericMessenger(this, "/E_source/", "Settings for the source");

    fMessenger->DeclareProperty("sourceRadius", sourceRadius, "Radius of the source (cm)");
	fMessenger->DeclareProperty("gammaEnergy", gammaEnergy, "Energy of the gamma ray (MeV)");

    gammaEnergy = 0.1;

    G4int n_particle = 1;
    fParticleGun  = new G4ParticleGun(n_particle);

    // G4ThreeVector pos(0. * cm, 0.* cm, -source_radius);

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("gamma");

    // G4ThreeVector mom(0., 0., 1.);

    // fParticleGun->SetParticlePosition(pos);
    // fParticleGun->SetParticleMomentumDirection(mom);
    
    fParticleGun->SetParticleDefinition(particle);

    sourceRadius = 7.4;
}

EPrimaryGenerator::~EPrimaryGenerator()
{
    delete fParticleGun;
}

void EPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    fParticleGun->SetParticleEnergy(gammaEnergy * MeV);

    G4double sourceRadiusCm = sourceRadius * cm;

    // Random position
    G4double phi = 2  * pi * G4UniformRand();
    G4double theta = std::acos(1 - 2 * G4UniformRand());

    G4double rhat_x = std::sin(theta) * std::cos(phi);
    G4double rhat_y = std::sin(theta) * std::sin(phi);
    G4double rhat_z = std::cos(theta);

    G4double thetahat_x = std::cos(theta) * std::cos(phi);
    G4double thetahat_y = std::cos(theta) * std::sin(phi);
    G4double thetahat_z = - std::sin(theta);

    G4double phihat_x = - std::sin(phi);
    G4double phihat_y = std::cos(phi);
    G4double phihat_z = 0;

    G4double disk_phi = G4UniformRand() * 2 * pi;
    G4double disk_r = std::sqrt(G4UniformRand() * sourceRadiusCm * sourceRadiusCm);

    G4double x = sourceRadiusCm * rhat_x + disk_r * (std::cos(disk_phi) * thetahat_x + std::sin(disk_phi) * phihat_x);
    G4double y = sourceRadiusCm * rhat_y + disk_r * (std::cos(disk_phi) * thetahat_y + std::sin(disk_phi) * phihat_y);
    G4double z = sourceRadiusCm * rhat_z + disk_r * (std::cos(disk_phi) * thetahat_z + std::sin(disk_phi) * phihat_z);

    // For testing geometry
    // G4double x = 0 * cm;
    // G4double y = (7.62/2 - 0.4) * cm;
    // G4double z = 13.0 * cm;

    // New random position but pointing towards detector
    fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(-rhat_x,-rhat_y,-rhat_z));

    // Create vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}