#include "../include/construction.hh"

MyDetectorConstuction::MyDetectorConstuction(){
    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Contruction");

    // control/execute vis.mac
    DefineMaterials();

    xWorld = 20*km;
    yWorld = 20*km;
    zWorld = 10*km;

};

void MyDetectorConstuction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();

    G4double energy[2] = {1.239841939 * eV / 0.9, 1.239841939 * eV / 0.2}; 
    // corte em energia hcortado (eV)/lambda (um)

    // definindo ar
    worldMat = nist->FindOrBuildMaterial("G4_AIR"); // mais tarde definir vacuo
    // indice de refracao
    G4double rindexWorld[2] = {1.0, 1.0};
    // setting
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
    worldMat->SetMaterialPropertiesTable(mptWorld);

    // definindo ch4
    // composicao
    G4int natoms;
    G4double Z;
    G4double A;

    G4Element *elH = new G4Element("hydrogen", "el_H", Z = 1., A = 1.00794 * g / mole);

    G4Element *elC = new G4Element("carbon", "el_C", Z = 6., A = 12.0107 * g / mole);

    G4double density = 0.000667151 * g / cm3;
    G4double pressure = 101325 * pascal;
    G4double temperature = CLHEP::STP_Temperature;
    matCH4 = new G4Material("CH4", density, 2, kStateGas, temperature, pressure);
    matCH4->AddElement(elC, natoms = 1);
    matCH4->AddElement(elH, natoms = 4);
    matCH4->GetIonisation()->SetMeanExcitationEnergy(41.7 * eV);

    // indice de refracao
    G4double rindexCH4[2] = {1.004, 1.004};

    // setting
    G4MaterialPropertiesTable *mptCH4 = new G4MaterialPropertiesTable();
    mptCH4->AddProperty("RINDEX", energy, rindexCH4, 2);
    matCH4->SetMaterialPropertiesTable(mptCH4);

    // Carbon dioxide, STP
    G4Element *elO = new G4Element("Oxygen", "elO", Z = 8., A = 15.9994 * g / mole);

    density = 0.00184212 * g / cm3;
    matCO2 = new G4Material("CO2", density, 2,
                            kStateGas, 273.15 * kelvin, 1. * atmosphere);
    matCO2->AddElement(elC, natoms = 1);
    matCO2->AddElement(elO, natoms = 2);
    matCO2->GetIonisation()->SetMeanExcitationEnergy(85 * eV);

    G4double rindexCO2[2] = {1.00, 1.00};
    // setting
    G4MaterialPropertiesTable *mptCO2 = new G4MaterialPropertiesTable();
    mptCO2->AddProperty("RINDEX", energy, rindexCO2, 2);
    matCO2->SetMaterialPropertiesTable(mptCO2);

}

MyDetectorConstuction::~MyDetectorConstuction(){};

void MyDetectorConstuction::ConstructAtmosphere(){
    solidAtmosphere = new G4Box("solidAir", xWorld - 2 * km, yWorld - 2 * km, zWorld);
    logicAtmosphere = new G4LogicalVolume(solidAtmosphere, matCO2, "logicAtmosphere");
    physAtmosphere = new G4PVPlacement(0, G4ThreeVector(0,0,0), logicAtmosphere, "physAtmosphere", logicWorld, false, 0, true);

}

G4VPhysicalVolume *MyDetectorConstuction::Construct(){

    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    //insert material into volume
    logicWorld =  new G4LogicalVolume(solidWorld, worldMat,"logicWorld");

    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),logicWorld,"physWorld", 0,false,true);
    
    ConstructAtmosphere();
        
    return physWorld;
}


void MyDetectorConstuction::ConstructSDandField(){
    
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    if(logicAtmosphere != NULL)
        G4cout << " Im here SetSensitiveDetector" << G4endl;
        logicAtmosphere->SetSensitiveDetector(sensDet);
    
}