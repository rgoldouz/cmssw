#!/usr/bin/env python
import os
import sys
import re
import string
sys.path.append(os.path.abspath("."))
import Files_2024
import os.path
from os import path
#before running this do 
#slc9 CRAB JUST WORKS IN SINGULARITIES
#source /cvmfs/cms.cern.ch/cmsset_default.sh
#source /cvmfs/cms.cern.ch/crab3/crab.sh

SAMPLES = {}
UL_YEAR = 'UL24'

if UL_YEAR=='UL24':
    SAMPLES.update(Files_2024.mc_samples)
    SAMPLES.update(Files_2024.data_samples)

if __name__ == '__main__':
    from CRABAPI.RawCommand import crabCommand

    def submit(config):
        res = crabCommand('submit', config = config)

    from CRABClient.UserUtilities import config
    config = config()

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
    config.JobType.disableAutomaticOutputCollection = True
    for key, value in SAMPLES.items():
        if (path.exists('/cms/cephfs/data/store/user/rgoldouz/NanoAodPostProcessingUL/UL24/v1/' + key) or path.exists('/cms/cephfs/data/store/user/rgoldouz/NanoAodPostProcessingUL/UL24/compSamples/' + key)):
            continue
        if value[5]!='DAS':
            continue
        print (key)
        config.General.requestName = key
        config.Data.inputDataset = value[0][0]
        config.Data.outLFNDirBase = '/store/user/rgoldouz/NanoAodPostProcessingUL/UL24/v1/' + key
        config.JobType.scriptArgs  = ['--DataProcessing=' + value[1],'--year=' + value[2], '--run=' +  value[3], '--json=' + value[4], '--cr=Yes']
        submit(config)
