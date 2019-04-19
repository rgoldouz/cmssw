from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = "pLHE_TT_M500to800"
config.General.workArea = 'crab_privateMCProduction_New_Full_200M_ext6'
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'TT_M500to800_cfg.py'
config.JobType.numCores = 8
config.JobType.disableAutomaticOutputCollection = False

config.section_("Data")
config.Data.outputPrimaryDataset = 'privateMCProductionLHE'
config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 10000
config.Data.totalUnits = 100000000
config.Data.publication = False
config.Data.outputDatasetTag = 'eventLHE_TT_New_Full_200M_ext6'


config.section_("Site")
config.Site.storageSite = 'T2_BE_IIHE'
