#ifndef SimG4CMS_CaloShowerProfile_H
#define SimG4CMS_CaloShowerProfile_H
///////////////////////////////////////////////////////////////////////////////
// File: CaloShowerProfile.h
// Analysis of simhits inside the OSCAR framework
///////////////////////////////////////////////////////////////////////////////

#include "SimG4Core/Notification/interface/Observer.h"
#include "SimG4Core/Watcher/interface/SimWatcher.h"

#include "SimDataFormats/CaloHit/interface/CaloHit.h"
#include "SimDataFormats/CaloTest/interface/HcalTestHistoClass.h"
#include "SimG4CMS/Calo/interface/HcalQie.h"
#include "SimG4CMS/Calo/interface/HcalTestHistoManager.h"
#include "SimG4CMS/Calo/interface/HcalTestNumberingScheme.h"
#include "Geometry/HcalCommonData/interface/HcalNumberingFromDDD.h"
#include "Geometry/HcalCommonData/interface/HcalDDDSimConstants.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <TROOT.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2F.h>
#include <TH1F.h>



#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Step;
class BeginOfJob;
class BeginOfRun;
class BeginOfEvent;
class EndOfEvent;

namespace CLHEP {
  class HepRandomEngine;
}

class CaloShowerProfile : public SimWatcher,
			 public Observer<const BeginOfJob *>, 
			 public Observer<const BeginOfRun *>, 
			 public Observer<const BeginOfEvent *>, 
			 public Observer<const EndOfEvent *>, 
			 public Observer<const G4Step *> {

public:
  CaloShowerProfile(const edm::ParameterSet &p);
  ~CaloShowerProfile() override;

private:
  // observer classes
  void update(const BeginOfJob * run) override;
  void update(const BeginOfRun * run) override;
  void update(const BeginOfEvent * evt) override;
  void update(const EndOfEvent * evt) override;
  void update(const G4Step * step) override;


private:

  G4ThreeVector             pvPosition, pvMomentum, pvUVW;
  std::string               fileName;
  double                    eta0, phi0;
  bool                      firstInter, firstInel;
  G4RotationMatrix*  beamline_RM;
  double                    ePi0late, ePi0first, eHadtot;
  G4ThreeVector      hitPosition, hitXYZ;
  std::vector<int> fPi0ID, lPi0ID;



  TTree* eventTree;
  float m_p_E;
  float m_p_eta;
  float m_p_phi;
  int   m_p_pdgid;

  float m_sim_eTotal;
  float m_sim_ePi0First;
  float m_sim_ePi0Late;
  float m_sim_eHadronic;

  float m_p_ePi0First;
  float m_p_ePi0Late;

  float m_pvInt_x;
  float m_pvInt_y;
  float m_pvInt_z;

  float m_pvIneInt_x;
  float m_pvIneInt_y;
  float m_pvIneInt_z;

  TH1F *h_eHadronic;
  TH1F *h_ePi0First;
  TH1F *h_ePi0Late;

};

#endif
