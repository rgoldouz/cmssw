#ifndef LHEGenericMassFilter_h
#define LHEGenericMassFilter_h
// -*- C++ -*-
//
// Package:    LHEGenericMassFilter
// Class:      LHEGenericMassFilter
// 
/* 

 Description: Filter to select events with defined inv. mass.

 Implementation: derived from MCSingleParticleFilter
     
*/
//
// Author:  Reza Goldouzian
//         Created:  Feb 3 2018
//
//

// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

//
// class declaration
//

class LHEGenericMassFilter : public edm::EDFilter {
 public:
  explicit LHEGenericMassFilter(const edm::ParameterSet&);
  ~LHEGenericMassFilter();
  
 private:
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob();
  
  // ----------member data ---------------------------
  
  edm::EDGetTokenT<LHEEventProduct> src_;
  int numRequired_;                // number of particles required to pass filter
  std::vector<int> particleID_;    // vector of particle IDs to look for
  double minMass_;
  double maxMass_;
  int totalEvents_;                // counters
  int passedEvents_;
};
#endif
