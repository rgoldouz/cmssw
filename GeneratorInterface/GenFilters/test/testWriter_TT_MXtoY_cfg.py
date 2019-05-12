#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms
import os 
process = cms.Process("Writer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.StandardSequences.Services_cff')



process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
                            )

directories =  '/store/user/pdong/privateMCProductionLHE/eventLHE_TT_New_Full_200M_ext6_fullRun/mergedFiles/' 
with open('ttfiles.tex', 'r') as f:
    for line in f:
        if 'str' in line:
            break
        process.source.fileNames.append( "root://cmsxrootd.fnal.gov//"+directories+line)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

process.writer = cms.EDAnalyzer("LHEWriter")
process.writer.externalLHEProducer = cms.InputTag("externalLHEProducer"                  , ""                ,"LHE"  )

process.lheGenericMassFilter = cms.EDFilter("LHEGenericMassFilter",
    src = cms.InputTag("externalLHEProducer"),
    NumRequired = cms.int32(2),
    ParticleID = cms.vint32(11,13,15),
    MinMass = cms.double(500.0),
    MaxMass = cms.double(800.0),
)
process.outpath = cms.Path(process.lheGenericMassFilter * process.writer)
