#include "../include/detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name){
    quEff = new G4PhysicsFreeVector();

    std::ifstream datafile;
    datafile.open("eff.dat");
    // quantum efficiency for a given wavelength
    while (1){
        G4double wlen, queff;
        datafile >> wlen >> queff;

        if (datafile.eof())
            break;

        G4cout << wlen << " " << queff << G4endl;

        quEff->InsertValues(wlen, queff / 100.);
    }

    datafile.close();
}
MySensitiveDetector::~MySensitiveDetector() {}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist){
    G4Track *track = aStep->GetTrack();

    // track->SetTrackStatus(fStopAndKill);

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *posStepPoint = aStep->GetPostStepPoint();

    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();

    G4double time = preStepPoint->GetGlobalTime();

    G4double wlen = (1.239841939 * eV / momPhoton.mag()) * 1E+03;

    // G4cout << "wlen: " << wlen << G4endl;

    // Calculates the wavelength (wlen) of the photon in nanometers (nm).
    // The constant 1.239841939 * eV approximately represents the product of
    // h (Planck constant) and c (speed of light) in units of eV·µm.
    // We divide this constant by the photon energy (momPhoton.mag() in eV)
    // and we multiply by 1E+03 to convert from micrometers (µm) to nanometers (nm).

    // G4cout << "Photon position: " << posPhoton << G4endl;

    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

    G4int copNo = touchable->GetCopyNumber();

    // G4cout << "Copy number: " << copNo << G4endl;

    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();

#ifndef G4MULTITHREADED
    // G4cout << "Detector position: " << posDetector << G4endl;
#endif

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, 0, evt);
    man->FillNtupleDColumn(0, 1, posPhoton[0]);
    man->FillNtupleDColumn(0, 2, posPhoton[1]);
    man->FillNtupleDColumn(0, 3, posPhoton[2]);
    man->FillNtupleDColumn(0, 4, wlen);
    man->FillNtupleDColumn(0, 5, time);
    man->AddNtupleRow(0);

    // photon is detected based on quantum efficiency for a given wavelength
    if (G4UniformRand() < quEff->Value(wlen)){
        man->FillNtupleIColumn(1, 0, evt);
        man->FillNtupleDColumn(1, 1, posDetector[0]);
        man->FillNtupleDColumn(1, 2, posDetector[1]);
        man->FillNtupleDColumn(1, 3, posDetector[2]);
        man->AddNtupleRow(1);
    }
    return true;
}