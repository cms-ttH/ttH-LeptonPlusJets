from WMCore.Configuration import Configuration 
config = Configuration() 
config.section_("General") 
config.General.requestName = 'treeMaker_PHYS14' ## change

config.section_("JobType") 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'yggdrasil_treeMaker_cfg.py' 
config.JobType.allowUndistributedCMSSW = False

config.section_("Data") 
config.Data.inputDataset = '/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/MINIAODSIM' ## change
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/' 
config.Data.splitting = 'FileBased' 
config.Data.unitsPerJob = 1
#config.Data.totalUnits = 100
config.Data.publication = True 
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/' 
config.Data.publishDataName = 'Validation_v1'
### change user Space 
config.Data.outLFNDirBase = '/store/user/lwming/'
config.Data.ignoreLocality = True

config.section_("Site") 
config.Site.storageSite = 'T3_US_FNALLPC'
