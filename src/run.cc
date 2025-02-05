#include "../include/run.hh"

MyRunAction::MyRunAction(){
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    //if change some Column, i have to change in detectot man->fill
    man->CreateNtuple("Photons", "Photons");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fwlen");
    man->CreateNtupleDColumn("fT");
    man->FinishNtuple(0);


    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(1);

    man->CreateNtuple("Scoring", "Scoring");
    man->CreateNtupleDColumn("fEdep");
    man->FinishNtuple(2);


};

MyRunAction::~MyRunAction(){};

void MyRunAction::BeginOfRunAction(const G4Run *run){

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int runId = run->GetRunID();

    std::stringstream strRunId;
    strRunId << runId;

    man->OpenFile("output" + strRunId.str() + ".root");

};

void MyRunAction::EndOfRunAction(const G4Run*){

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();

};
