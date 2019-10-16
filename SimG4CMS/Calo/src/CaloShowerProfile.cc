#include "SimG4Core/Notification/interface/BeginOfJob.h"
#include "SimG4Core/Notification/interface/BeginOfRun.h"
#include "SimG4Core/Notification/interface/BeginOfEvent.h"
#include "SimG4Core/Notification/interface/EndOfEvent.h"

// to retreive hits
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "SimG4CMS/Calo/interface/CaloShowerProfile.h"
#include "SimG4CMS/Calo/interface/HCalSD.h"
#include "SimG4CMS/Calo/interface/CaloG4Hit.h"
#include "SimG4CMS/Calo/interface/CaloG4HitCollection.h"
#include "DataFormats/Math/interface/Point3D.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/Records/interface/HcalSimNumberingRecord.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"

#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4HCofThisEvent.hh"
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "CLHEP/Units/GlobalPhysicalConstants.h"
#include "Randomize.hh"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

CaloShowerProfile::CaloShowerProfile(const edm::ParameterSet &p) {
  edm::ParameterSet m_Anal = p.getParameter<edm::ParameterSet>("CaloShowerProfile");
  eta0         = m_Anal.getParameter<double>("Eta0");
  phi0         = m_Anal.getParameter<double>("Phi0");
  fileName     = m_Anal.getParameter<std::string>("FileName");

} 
   
CaloShowerProfile::~CaloShowerProfile() {
}

//==================================================================== per JOB

void CaloShowerProfile::update(const BeginOfJob * job) {
  edm::Service<TFileService> fs;
//  m_p_pt    = new float;
//  m_p_eta   = new float;
//  m_p_phi   = new float;
//  m_p_pdgid = new int;

  eventTree = fs->make<TTree>("eventTree", "Event tree");
  eventTree->Branch("p_E",     &m_p_E, "p_E/F");
  eventTree->Branch("p_eta",    &m_p_eta, "p_eta/F");
  eventTree->Branch("p_phi",    &m_p_phi, "p_phi/F");
  eventTree->Branch("p_pdgid",  &m_p_pdgid,"p_pdgid/I");

  eventTree->Branch("p_ePi0First",    &m_p_ePi0First, "p_ePi0First/F");
  eventTree->Branch("p_ePi0Late",    &m_p_ePi0Late, "p_ePi0Late/F");

  eventTree->Branch("sim_eTotal",    &m_sim_eTotal, "sim_eTotal/F");
  eventTree->Branch("sim_ePi0First",    &m_sim_ePi0First, "sim_ePi0First/F");
  eventTree->Branch("sim_ePi0Late",    &m_sim_ePi0Late, "sim_ePi0Late/F");
  eventTree->Branch("sim_eHadronic",    &m_sim_eHadronic, "sim_eHadronic/F");


  eventTree->Branch("sim_pvInt_x",    &m_pvInt_x, "sim_pvInt_x/F");
  eventTree->Branch("sim_pvInt_y",    &m_pvInt_y, "sim_pvInt_y/F");
  eventTree->Branch("sim_pvInt_z",    &m_pvInt_z, "sim_pvInt_z/F");

  eventTree->Branch("sim_pvIneInt_x",    &m_pvIneInt_x, "sim_pvIneInt_x/F");
  eventTree->Branch("sim_pvIneInt_y",    &m_pvIneInt_y, "sim_pvIneInt_y/F");
  eventTree->Branch("sim_pvIneInt_z",    &m_pvIneInt_z, "sim_pvIneInt_z/F");

  eventTree->Branch("h_eHadronic","TH1F",&h_eHadronic,32000,0);
  eventTree->Branch("h_ePi0First","TH1F",&h_ePi0First,32000,0);
  eventTree->Branch("h_ePi0Late" ,"TH1F",&h_ePi0Late ,32000,0);
}

//==================================================================== per RUN
void CaloShowerProfile::update(const BeginOfRun * run) {


}

//=================================================================== per EVENT
void CaloShowerProfile::update(const BeginOfEvent * evt) {

  firstInter = false;
  firstInel  = false;

  h_eHadronic    = new TH1F("h_eHadronic","Hadronic longitudinal shower energy profile",3000,0,30000);
  h_ePi0First    = new TH1F("h_ePi0First","First Pi0 longitudinal shower energy profile",3000,0,30000);
  h_ePi0Late     = new TH1F("h_ePi0Late" ,"Late Pi0 longitudinal shower energy profile",3000,0,30000);

  G4PrimaryParticle* thePrim=0;
  int nvertex = (*evt)()->GetNumberOfPrimaryVertex();

  for (int i = 0 ; i<nvertex; i++) {
    G4PrimaryVertex* avertex = (*evt)()->GetPrimaryVertex(i);
    if (avertex == 0) {
       std::cout << " HcalTestAnalysis::EndOfEvent ERR: pointer "<< "to vertex = 0 for event " << std::endl;
    } else {
      if (thePrim==0) thePrim=avertex->GetPrimary(0);
    }
  }

  if (thePrim != 0) {
    float px = thePrim->GetPx()/GeV;
    float py = thePrim->GetPy()/GeV;
    float pz = thePrim->GetPz()/GeV;
    pvMomentum = G4ThreeVector(px,py,pz);
    float p  = std::sqrt(pow(px,2.)+pow(py,2.)+pow(pz,2.));
    m_p_E = p;
    m_p_eta = pvMomentum.eta();
    m_p_phi = pvMomentum.phi();
    m_p_pdgid = thePrim->GetPDGcode();

    double beamThet = pvMomentum.theta();
    double beamPhi  = (pvMomentum.phi() < 0.0) ? pvMomentum.phi()+CLHEP::twopi : pvMomentum.phi();


    beamline_RM = new G4RotationMatrix;
    beamline_RM->rotateZ(-beamPhi);
    beamline_RM->rotateY(-beamThet);
    }
   ePi0first = ePi0late  = 0.0;
   fPi0ID.clear();
   lPi0ID.clear();
}

//=================================================================== each STEP
void CaloShowerProfile::update(const G4Step * aStep) {
  if (aStep != NULL) {
// Get Step properties
    G4ThreeVector thePreStepPoint  = (*beamline_RM)*(aStep->GetPreStepPoint()->GetPosition());
    G4ThreeVector thePostStepPoint;

// Get Tracks properties
    G4Track*      aTrack   = aStep->GetTrack();
    int           trackID  = aTrack->GetTrackID();
    int           parentID = aTrack->GetParentID();
    G4ThreeVector position = aTrack->GetPosition();
    G4ThreeVector momentum = aTrack->GetMomentum();
    G4String      partType = aTrack->GetDefinition()->GetParticleType();
    G4VPhysicalVolume* curPV  = aStep->GetPreStepPoint()->GetPhysicalVolume();

    G4String thePostPVname = "NoName";
    G4StepPoint * thePostPoint = aStep->GetPostStepPoint();
    if (thePostPoint) {
       thePostStepPoint = thePostPoint->GetPosition();
       G4VPhysicalVolume * thePostPV = thePostPoint->GetPhysicalVolume ();
       if (thePostPV) thePostPVname = thePostPV->GetName ();
    }

    G4String name01 = curPV->GetName();
    G4String name02, name03;
    name02.assign(name01,0,3);
    name03.assign(name01,0,2);


/*
    if ( name02=="HBS" || name02=="HBL" || name02=="HES" || name02=="HEP" ||
         name02=="HEB" || name02=="EBR" || name02=="EFR" || name03=="SF"  ||
         name03=="EE"  || name02=="EIL" || name02=="ESP" || name02=="EHA" ||
         name02=="EWA" || name02=="ECL" || name02=="EWR" || name02=="ESG" ||
         name02=="ECA" || name02=="EBC" || name02=="EMB" || name02=="EBB" ||
         name02=="EBP" || name02=="EFA" ) {
*/

      hitPosition = G4ThreeVector(thePostStepPoint.x(),thePostStepPoint.y(),thePostStepPoint.z());
      hitXYZ      = (*beamline_RM)*(hitPosition);
  
      if ( !firstInter ) {
        pvPosition = thePreStepPoint;
        firstInter = true;
      }
  
      G4String pross = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  
      if ( (pross=="pi-Inelastic" || pross=="pi+Inelastic" || pross=="protonInelastic" ) && !firstInel )
      {
       pvUVW = thePreStepPoint;
       firstInel = true;
      }
      if ( trackID !=1 && (aTrack->GetCurrentStepNumber()==1) ) {
        if ( aTrack->GetDefinition()->GetParticleName() == "pi0") {                    
          if ( parentID==1) {
            fPi0ID.push_back(trackID);
            ePi0first += aTrack->GetKineticEnergy()/GeV;}
          else{
            ePi0late += aTrack->GetKineticEnergy()/GeV;
            lPi0ID.push_back(trackID);}
        }
      }



      if(std::find(fPi0ID.begin(), fPi0ID.end(), parentID) != fPi0ID.end()){
        h_ePi0First->Fill(hitXYZ.z(),aStep->GetTotalEnergyDeposit()/GeV);
        if(!(std::find(fPi0ID.begin(), fPi0ID.end(), trackID) != fPi0ID.end())) fPi0ID.push_back(trackID);}
      else if(std::find(lPi0ID.begin(), lPi0ID.end(), parentID) != lPi0ID.end()){ 
        h_ePi0Late->Fill(hitXYZ.z(),aStep->GetTotalEnergyDeposit()/GeV);
        if(!(std::find(lPi0ID.begin(), lPi0ID.end(), trackID) != lPi0ID.end())) lPi0ID.push_back(trackID);}
      else{
        h_eHadronic->Fill(hitXYZ.z(),aStep->GetTotalEnergyDeposit()/GeV);}
     
//    }
  }  
}

//================================================================ End of EVENT
void CaloShowerProfile::update(const EndOfEvent * evt) {
  m_p_ePi0First = ePi0first;
  m_p_ePi0Late = ePi0late;

  m_sim_ePi0First = h_ePi0First->Integral();
  m_sim_ePi0Late = h_ePi0Late->Integral();
  m_sim_eHadronic = h_eHadronic->Integral();
  m_sim_eTotal = h_ePi0First->Integral() + h_ePi0Late->Integral() + h_eHadronic->Integral();


  m_pvInt_x = pvPosition.x();
  m_pvInt_y = pvPosition.y();
  m_pvInt_z = pvPosition.z();

  m_pvIneInt_x = pvUVW.x();
  m_pvIneInt_y = pvUVW.y();
  m_pvIneInt_z = pvUVW.z();

  eventTree->Fill();
}

