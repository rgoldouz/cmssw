#!/usr/bin/env python
import os
import sys
import re
import string
import os.path
from os import path
#before running this do 
#source /cvmfs/cms.cern.ch/cmsset_default.sh
#source /cvmfs/cms.cern.ch/crab3/crab.sh

from CRABAPI.RawCommand import crabCommand
from CRABClient.UserUtilities import config
config = config()
key='TTto2L2Nu'
name = 'NanoAodPostProcessingUL24'
config.General.workArea = 'crab_'+name
config.General.transferLogs = False
config.General.transferOutputs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'PSet.py'
config.JobType.scriptExe  = 'crab_script.sh'
config.JobType.inputFiles = ['Lobster_postproc.py', 'haddnano.py','Cert_Collisions2024_378981_386951_Golden.json', 'keep_and_drop.txt' ]
config.JobType.allowUndistributedCMSSW = True
config.Data.splitting     = 'FileBased'
config.Data.inputDBS      = 'global'
config.Site.storageSite = 'T3_US_NotreDame'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.JobType.outputFiles = ['tree.root']
config.General.requestName = key
config.Data.inputDataset = '/TTto2L2Nu_TuneCP5_13p6TeV_powheg-pythia8/RunIII2024Summer24NanoAODv15-150X_mcRun3_2024_realistic_v2-v3/NANOAODSIM'
config.Data.outLFNDirBase = '/store/user/rgoldouz/NanoAodPostProcessingUL/UL24/v1/' + key
config.JobType.scriptArgs  = ['--DataProcessing=mc' ,'--year=2024' , '--run=D' , '--json=l' , '--cr=Yes']
