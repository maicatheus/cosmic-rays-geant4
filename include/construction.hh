#ifndef CONTRUCTION_HH
#define CONTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4LogicalSkinSurface.hh"
#include "detector.hh"


class MyDetectorConstuction : public G4VUserDetectorConstruction{ 
public:
    MyDetectorConstuction();
    ~MyDetectorConstuction();

    G4LogicalVolume *GetScoringVolume() const {
        return fScoringVolume;
    }

    virtual G4VPhysicalVolume *Construct();
    void ConstructAtmosphere();

private:
    
    G4int nCols, nRows;

    G4double xWorld, yWorld, zWorld;
    G4bool isCherenkov, isScintillator, isTOF, isAtmosphere;

    G4OpticalSurface *mirrorSurface;

    G4Box *solidWorld, *solidRadiator, *solidDetector, *solidScintillator, *solidAtmosphere;
    G4LogicalVolume *logicWorld, *logicAtmosphere;
    G4VPhysicalVolume *physWorld, *physAtmosphere;
    
    void DefineMaterials();

    virtual void ConstructSDandField();

    G4GenericMessenger *fMessenger;

    G4Material *worldMat, *Air, *matCH4, *matCO2;

    G4LogicalVolume *fScoringVolume;

};
 
#endif