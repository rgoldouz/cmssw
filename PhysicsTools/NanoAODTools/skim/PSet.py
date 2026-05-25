# this fake PSET is needed for local test and for crab to figure the output
# filename you do not need to edit it unless you want to do a local test using
# a different input file than the one marked below
import FWCore.ParameterSet.Config as cms
process = cms.Process('NANO')
process.source = cms.Source(
    "PoolSource",
    fileNames=cms.untracked.vstring(),
    # lumisToProcess=cms.untracked.VLuminosityBlockRange("254231:1-254231:24")
)
process.source.fileNames = [
        '/cms/cephfs/data/store/user/rgoldouz/RunIIIDatasets/UL24_TTto2L2Nu/aa5cc9e8-4ba4-42f5-9a53-e9364813ebff.root',
        '/cms/cephfs/data/store/user/rgoldouz/RunIIIDatasets/UL24_TTto2L2Nu/4a981dab-673b-4c9e-827f-0cccca01a646.root',
#    '/store/user/rgoldouz/NanoAodPostProcessingUL/UL24/v1/UL24_TTTo2L2Nu/TTto2L2Nu_TuneCP5_13p6TeV_powheg-pythia8/crab_UL24_TTTo2L2Nu/260304_130452/0000/tlkree_13.root'  # you can change only this line
]
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(10))
