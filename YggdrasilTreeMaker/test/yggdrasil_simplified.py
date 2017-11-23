#cms# -*- coding: utf-8 -*-

"""
Example config file that shows how to setup all recipes we use for Moriond17.

At the beginning, several default input collections (cms.InputTag's) are defined as "placeholders".
These collections might get overwritten at various places in this config, depending on the recipes
that should be used, e.g., when deterministic seeds are required, all subsequent recipes should make
use of the newly created collections. This is transparently implemented using the placeholders.

This file is compatible to the CMSSW setup described at:
https://gitlab.cern.ch/ttH/reference/blob/master/definitions/Moriond17.md#11-cmssw
"""


import os
import FWCore.ParameterSet.Config as cms
#import PhysicsTools.PythonAnalysis.LumiList as LumiList
import FWCore.PythonUtilities.LumiList as LumiList
from FWCore.ParameterSet.VarParsing import VarParsing


#
# options
#

options = VarParsing("python")

options.register("DataList","",VarParsing.multiplicity.list,VarParsing.varType.string,"List of input datasets")
options.DataList = ( # Era B
#"/store/data/Run2016B/SingleElectron/MINIAOD/03Feb2017_ver2-v2/110000/003B2C1F-50EB-E611-A8F1-002590E2D9FE.root",
"/store/data/Run2016B/SingleElectron/MINIAOD/03Feb2017_ver2-v2/110000/0043D133-4DEB-E611-BFE4-003048F5ADEC.root",
"/store/data/Run2016B/SingleMuon/MINIAOD/03Feb2017_ver2-v2/100000/000C6E52-8BEC-E611-B3FF-0025905C42FE.root",
"/store/data/Run2016B/SingleMuon/MINIAOD/03Feb2017_ver2-v2/100000/001E3E7D-57EB-E611-8469-0CC47A7C35D2.root",
"/store/data/Run2016B/DoubleEG/MINIAOD/03Feb2017_ver2-v2/50000/00054938-CEEA-E611-889E-0CC47A4D7650.root",
"/store/data/Run2016B/DoubleEG/MINIAOD/03Feb2017_ver2-v2/50000/001BC16F-90EA-E611-87E2-F04DA27540BB.root"
)
#options.DataList = ( # Era C
#"/store/data/Run2016C/SingleElectron/MINIAOD/03Feb2017-v1/100000/02169BE7-81EB-E611-BB99-02163E0137CD.root",
#"/store/data/Run2016C/SingleElectron/MINIAOD/03Feb2017-v1/100000/0244B3B4-78EB-E611-9D42-0CC47A009E24.root",
#"/store/data/Run2016C/SingleMuon/MINIAOD/03Feb2017-v1/50000/001CF316-1AEB-E611-BBBD-0CC47A4C8EE2.root",
#"/store/data/Run2016C/SingleMuon/MINIAOD/03Feb2017-v1/50000/0022D65B-05EB-E611-84E9-0025905A6104.root",
#"/store/data/Run2016C/DoubleEG/MINIAOD/03Feb2017-v1/80000/00371362-6AEC-E611-9845-842B2B758BAA.root",
#"/store/data/Run2016C/DoubleEG/MINIAOD/03Feb2017-v1/80000/006E86B9-77EC-E611-BA8F-02163E019CE7.root"
#)
#options.DataList = ( # Era D
#"/store/data/Run2016D/SingleElectron/MINIAOD/03Feb2017-v1/110000/001A5DEC-77EB-E611-95CD-0CC47A4C8EE8.root",
#"/store/data/Run2016D/SingleElectron/MINIAOD/03Feb2017-v1/110000/001F5992-6DEA-E611-AF62-A4BF01013DD5.root",
#"/store/data/Run2016D/SingleMuon/MINIAOD/03Feb2017-v1/100000/00622F98-20EB-E611-A0A4-28924A33AFF6.root",
#"/store/data/Run2016D/SingleMuon/MINIAOD/03Feb2017-v1/100000/023071E7-97EA-E611-A89A-0025904C67B6.root",
#"/store/data/Run2016D/DoubleEG/MINIAOD/03Feb2017-v1/100000/002CE21C-0BEB-E611-8597-001E67E6F8E6.root",
#"/store/data/Run2016D/DoubleEG/MINIAOD/03Feb2017-v1/100000/006BB826-05EB-E611-97F4-0025904A8ED2.root"
#)
#options.DataList = ( # Era E
#"/store/data/Run2016E/SingleElectron/MINIAOD/03Feb2017-v1/110000/00022327-8BEA-E611-86CB-0025905B8566.root",
#"/store/data/Run2016E/SingleElectron/MINIAOD/03Feb2017-v1/110000/002070E4-1BEB-E611-BA80-0025905A608E.root",
#"/store/data/Run2016E/SingleMuon/MINIAOD/03Feb2017-v1/110000/001D567A-0CEB-E611-A438-D8D385AE8848.root",
#"/store/data/Run2016E/SingleMuon/MINIAOD/03Feb2017-v1/110000/004309C7-E6EA-E611-92B5-0025905A60DA.root",
#"/store/data/Run2016E/DoubleEG/MINIAOD/03Feb2017-v1/110000/003AF399-ABEA-E611-92CF-002590E2DA08.root",
#"/store/data/Run2016E/DoubleEG/MINIAOD/03Feb2017-v1/110000/048E7FCD-98EA-E611-AB8A-002590D9D966.root"
#)
#options.DataList = ( # Era F
#"/store/data/Run2016F/SingleElectron/MINIAOD/03Feb2017-v1/100000/00B336D6-6AEC-E611-8581-E0071B7AC7B0.root",
#"/store/data/Run2016F/SingleElectron/MINIAOD/03Feb2017-v1/100000/0229A4E9-5DEC-E611-B931-A0000420FE80.root",
#"/store/data/Run2016F/SingleMuon/MINIAOD/03Feb2017-v1/100000/00E6AB6D-BCEC-E611-8F6E-0025905C3D98.root",
#"/store/data/Run2016F/SingleMuon/MINIAOD/03Feb2017-v1/100000/040B13AE-C3EC-E611-8082-0025904C6564.root",
#"/store/data/Run2016F/DoubleEG/MINIAOD/03Feb2017-v1/80000/0006AFD8-F8EA-E611-9F9D-0CC47A13D09C.root",
#"/store/data/Run2016F/DoubleEG/MINIAOD/03Feb2017-v1/80000/00A7E4D9-F8EA-E611-A62B-002590E3A004.root"
#)
#options.DataList = ( # Era G
#"/store/data/Run2016G/SingleElectron/MINIAOD/03Feb2017-v1/50000/004A75AB-B2EA-E611-B000-24BE05CEFDF1.root",
#"/store/data/Run2016G/SingleElectron/MINIAOD/03Feb2017-v1/50000/00539EDD-F8EA-E611-AC1D-0025900B5648.root",
#"/store/data/Run2016G/SingleMuon/MINIAOD/03Feb2017-v1/100000/00E6DF50-70EA-E611-ACC4-0CC47A1E089C.root",
#"/store/data/Run2016G/SingleMuon/MINIAOD/03Feb2017-v1/100000/02382B19-D1EA-E611-B2F9-0CC47ABAC11C.root",
#"/store/data/Run2016G/DoubleEG/MINIAOD/03Feb2017-v1/100000/002F14FF-D0EA-E611-952E-008CFA197AF4.root",
#"/store/data/Run2016G/DoubleEG/MINIAOD/03Feb2017-v1/100000/02642443-F0EA-E611-9D24-008CFA197D74.root"
#)
#options.DataList = ( # Era Hv2
#"/store/data/Run2016H/SingleElectron/MINIAOD/03Feb2017_ver2-v1/100000/00553E5F-29EC-E611-ADB0-00259074AE8C.root",
#"/store/data/Run2016H/SingleElectron/MINIAOD/03Feb2017_ver2-v1/100000/006EC85C-29EC-E611-B2E1-20CF3019DF17.root",
#"/store/data/Run2016H/SingleMuon/MINIAOD/03Feb2017_ver2-v1/110000/00633FF0-85EA-E611-811C-001E674FB25C.root",
#"/store/data/Run2016H/SingleMuon/MINIAOD/03Feb2017_ver2-v1/110000/006517CB-8AEA-E611-8CF6-0CC47AC08BD4.root",
#"/store/data/Run2016H/DoubleEG/MINIAOD/03Feb2017_ver2-v1/100000/023E858B-F7EC-E611-889C-047D7BD6DDF2.root",
#"/store/data/Run2016H/DoubleEG/MINIAOD/03Feb2017_ver2-v1/100000/02AD337A-95ED-E611-B1AE-047D7B881D90.root"
#)
#options.DataList = ( # Era Hv3
#"/store/data/Run2016H/SingleElectron/MINIAOD/03Feb2017_ver3-v1/110000/02973E99-69EC-E611-9913-5065F381A2F1.root",
#"/store/data/Run2016H/SingleElectron/MINIAOD/03Feb2017_ver3-v1/110000/04C8C9AF-62EC-E611-AB90-A0000420FE80.root",
#"/store/data/Run2016H/SingleMuon/MINIAOD/03Feb2017_ver3-v1/80000/0040ECBB-76EA-E611-8FE7-A0000420FE80.root",
#"/store/data/Run2016H/SingleMuon/MINIAOD/03Feb2017_ver3-v1/80000/16F28614-84EA-E611-8083-A0369F310374.root",
#"/store/data/Run2016H/DoubleEG/MINIAOD/03Feb2017_ver3-v1/1030000/D41C6358-4DF0-E611-BBAC-002590DB927A.root",
#"/store/data/Run2016H/DoubleEG/MINIAOD/03Feb2017_ver3-v1/110000/78C7FC4D-7CED-E611-870F-0CC47A7FC858.root"
#)

# set defaults of common options
# ttH
options.setDefault("inputFiles", "/store/mc/RunIISummer16MiniAODv2/ttHTobb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/44949CF4-96C6-E611-B9A0-0025905A6122.root")
options.register("SyncType","ttH",VarParsing.multiplicity.singleton,VarParsing.varType.string,"Which type of file to sync on (ttH, ttJets, data)")
options.setDefault("outputFile", "ttH_test.root")
options.register("SyncFile","ttH_test.csv",VarParsing.multiplicity.singleton,VarParsing.varType.string,"Name of sync .csv file")
options.register("realData",False,VarParsing.multiplicity.singleton,VarParsing.varType.bool,"input dataset contains real data")

# ttjets
#options.setDefault("inputFiles","/store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0693E0E7-97BE-E611-B32F-0CC47A78A3D8.root")
#options.register("SyncType","ttJets_test",VarParsing.multiplicity.singleton,VarParsing.varType.string,"Which type of file to sync on (ttH, ttJets, data)")
#options.setDefault("outputFile", "ttJets_test.root")
#options.register("SyncFile","ttJets_test.csv",VarParsing.multiplicity.singleton,VarParsing.varType.string,"Name of sync .csv file")
#options.register("realData",False,VarParsing.multiplicity.singleton,VarParsing.varType.bool,"input dataset contains real data")

# data
#options.setDefault("inputFiles",options.DataList)
#options.register("SyncType","Data",VarParsing.multiplicity.singleton,VarParsing.varType.string,"Which type of file to sync on (ttH, ttJets, data)")
#options.setDefault("outputFile", "Data_test.root")
#options.register("SyncFile","Data_test.csv",VarParsing.multiplicity.singleton,VarParsing.varType.string,"Name of sync .csv file")
#options.register("realData",True,VarParsing.multiplicity.singleton,VarParsing.varType.bool,"input dataset contains real data")

options.setDefault("maxEvents", 1000) # good enough for ttH, ttJets
#options.setDefault("maxEvents", 2000) # Need for RunB Datah to get an event or two
#options.setDefault("maxEvents", -1) # run all events

# add custom options
options.register("globalTag",
    "80X_mcRun2_asymptotic_2016_TrancheIV_v8",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "the global tag to use"
)
options.register("dataEra",
    "2016B", # Overwritten in treemaker since MiniAODHelper has dead code preventing 2016 data
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "the era of the data taking period, e.g. '2016B', empty for MC"
)
options.register("lumiFile",
    "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "file for selecting runs and lumis"
)
options.register("deterministicSeeds",
    True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "create collections with deterministic seeds"
)
options.register("electronRegression",
    "GT",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "'GT' or an absolute path to a sqlite file for electron energy regression"
)
options.register("electronSmearing",
    "Moriond17_23Jan",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "correction type for electron energy smearing"
)
options.register("recorrectMET",
    True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "recorrect MET using latest JES and e/g corrections"
)
options.register("updatePUJetId",
    True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "update the PUJetId values"
)
options.register("addTtbarGenId",
    options.SyncType=="ttJets", # should be set to True for all ttbar datasets
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "creates the ttbar gen id"
)
options.register("tagTTHF",
    options.SyncType=="ttJets", # should be set to True for all ttbar datasets
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "performs ttbar heavy flavour tagging"
)
options.register("reportEvery",
    100,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "number of events after which a report message is written"
)
options.register("summary",
    False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "print a summary at the end?"
)


##TreeMaker options

options.register("DoSync",True,VarParsing.multiplicity.singleton,VarParsing.varType.bool,"Do the Sync Code, output sync")
options.register("SyncDebug",False,VarParsing.multiplicity.singleton,VarParsing.varType.bool,"Extra Sync Variables for debugging")
options.register("SkipEvents",False,VarParsing.multiplicity.singleton,VarParsing.varType.bool,"Only run on certain events")
options.register("doSystematics",True,VarParsing.multiplicity.singleton,VarParsing.varType.bool,"Do the systematics loops")
options.register("saveOnlySelected",True,VarParsing.multiplicity.singleton,VarParsing.varType.bool,"Only save selected events")


options.parseArguments()


#
# collection placeholders
#

electronCollection = cms.InputTag("slimmedElectrons", "", "PAT")
muonCollection     = cms.InputTag("slimmedMuons", "", "PAT")
tauCollection      = cms.InputTag("slimmedTaus", "", "PAT")
photonCollection   = cms.InputTag("slimmedPhotons", "", "PAT")
METCollection      = cms.InputTag("slimmedMETs", "", "PAT")
jetCollection      = cms.InputTag("slimmedJets", "", "PAT")

genjetCollection   = cms.InputTag("slimmedGenJets","","")

#
# process and basic settings
#

process = cms.Process("TTH")

# source defintion
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles)
    
)

# max events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

# further options
process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True),
    wantSummary      = cms.untracked.bool(options.summary)
)

# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = options.reportEvery

# good run and lumi selection
if options.lumiFile and options.realData:
    process.source.lumisToProcess = LumiList.LumiList(filename=options.lumiFile).getVLuminosityBlockRange()

# standard seuquences with global tag
if options.globalTag:
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
    process.GlobalTag.globaltag = options.globalTag

# geometry sequences
process.load("Configuration.StandardSequences.GeometryDB_cff")

# particle data table
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# sequence for additional modules collected within this cfg
seq = cms.Sequence()


#
# deterministic seed producer
#

if options.deterministicSeeds:
    process.load("PhysicsTools.PatUtils.deterministicSeeds_cfi")
    process.deterministicSeeds.produceCollections = cms.bool(True)
    process.deterministicSeeds.produceValueMaps   = cms.bool(False)
    process.deterministicSeeds.electronCollection = electronCollection
    process.deterministicSeeds.muonCollection     = muonCollection
    process.deterministicSeeds.tauCollection      = tauCollection
    process.deterministicSeeds.photonCollection   = photonCollection
    process.deterministicSeeds.jetCollection      = jetCollection
    process.deterministicSeeds.METCollection      = METCollection

    # overwrite output collections
    electronCollection = cms.InputTag("deterministicSeeds", "electronsWithSeed", process.name_())
    muonCollection     = cms.InputTag("deterministicSeeds", "muonsWithSeed", process.name_())
    tauCollection      = cms.InputTag("deterministicSeeds", "tausWithSeed", process.name_())
    photonCollection   = cms.InputTag("deterministicSeeds", "photonsWithSeed", process.name_())
    jetCollection      = cms.InputTag("deterministicSeeds", "jetsWithSeed", process.name_())
    METCollection      = cms.InputTag("deterministicSeeds", "METsWithSeed", process.name_())


#
# electron energy regression
#

if options.electronRegression:
    if options.electronRegression == "GT":
        from EgammaAnalysis.ElectronTools.regressionWeights_cfi import regressionWeights
        process = regressionWeights(process)
    else:
        from EgammaAnalysis.ElectronTools.regressionWeights_local_cfi import GBRDWrapperRcd
        GBRDWrapperRcd.connect = cms.string("sqlite_file:" + options.electronRegression)
        process.regressions = GBRDWrapperRcd
        process.regressions.DumpStat = cms.untracked.bool(False)
        process.es_prefer_regressions = cms.ESPrefer("PoolDBESSource", "regressions")
    process.load("EgammaAnalysis.ElectronTools.regressionApplication_cff")
    seq += process.regressionApplication

    # set the electron and photon sources
    process.slimmedElectrons.src = electronCollection
    process.slimmedPhotons.src = photonCollection

    # overwrite output collections
    electronCollection = cms.InputTag("slimmedElectrons", "", process.name_())
    photonCollection = cms.InputTag("slimmedPhotons", "", process.name_())







#
# electron energy smearing
#

# only allowed when regression was used
if options.electronSmearing and options.electronRegression:
    process.selectedElectrons = cms.EDFilter("PATElectronSelector",
        src = electronCollection,
        cut = cms.string("pt>5 && abs(eta)")
    )
    electronCollection = cms.InputTag("selectedElectrons", "", process.name_())

    process.load("EgammaAnalysis.ElectronTools.calibratedPatElectronsRun2_cfi")
    from EgammaAnalysis.ElectronTools.calibratedPatElectronsRun2_cfi import files
    process.calibratedPatElectrons.isMC           = cms.bool(not options.realData)
    process.calibratedPatElectrons.correctionFile = cms.string(files[options.electronSmearing])
    process.calibratedPatElectrons.electrons      = electronCollection
    seq += process.calibratedPatElectrons
    if options.deterministicSeeds:
        process.calibratedPatElectrons.seedUserInt = process.deterministicSeeds.seedUserInt
    else:
      process.load("Configuration.StandardSequences.Services_cff")
      process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
          calibratedPatElectrons = cms.PSet(
              initialSeed = cms.untracked.uint32(81),
              engineName  = cms.untracked.string("TRandom3")
          )
      )



    # overwrite output collections
    electronCollection = cms.InputTag("calibratedPatElectrons", "", process.name_())


#
# electron VIDs
#


eleVIDModules = [
    "RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring15_25ns_Trig_V1_cff",
    "RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring16_GeneralPurpose_V1_cff",
    "RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Summer16_80X_V1_cff"
]

from PhysicsTools.SelectorUtils.tools.vid_id_tools import DataFormat, \
        switchOnVIDElectronIdProducer, setupAllVIDIdsInModule, setupVIDElectronSelection
switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)

for mod in eleVIDModules:
    setupAllVIDIdsInModule(process, mod, setupVIDElectronSelection)

# update the modules
process.egmGsfElectronIDs.physicsObjectSrc = electronCollection
process.electronRegressionValueMapProducer.srcMiniAOD = electronCollection
process.electronMVAValueMapProducer.srcMiniAOD = electronCollection


#
# MET corrections and uncertainties
#

if options.recorrectMET:
    # use the standard tool
    from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
    # do not use a postfix here!
    runMetCorAndUncFromMiniAOD(process,
        isData           = options.realData,
        electronColl     = electronCollection.value(),
        muonColl         = muonCollection.value(),
        tauColl          = tauCollection.value(),
        photonColl       = photonCollection.value(),
        jetCollUnskimmed = jetCollection.value(),
        recoMetFromPFCs  = True
    )

    # overwrite output collections
    METCollection = cms.InputTag("slimmedMETs", "", process.name_())

    # also add MET corrections due to e/g corrections, such as the slew rate fix in reMiniAOD
    if options.realData:
        from PhysicsTools.PatUtils.tools.corMETFromMuonAndEG import corMETFromMuonAndEG
        corMETFromMuonAndEG(process,
            pfCandCollection      = "",
            electronCollection    = "slimmedElectronsBeforeGSFix",
            photonCollection      = "slimmedPhotonsBeforeGSFix",
            corElectronCollection = electronCollection.value(),
            corPhotonCollection   = photonCollection.value(),
            allMETEGCorrected     = True,
            muCorrection          = False,
            eGCorrection          = True,
            runOnMiniAOD          = True,
            postfix               = "MuEGClean"
        )
        process.slimmedMETsMuEGClean = getattr(process, METCollection.getModuleLabel()).clone(
            src             = cms.InputTag("patPFMetT1MuEGClean"),
            rawVariation    = cms.InputTag("patPFMetRawMuEGClean"),
            t1Uncertainties = cms.InputTag("patPFMetT1%sMuEGClean")
        )
        del process.slimmedMETsMuEGClean.caloMET

        # overwrite output collections
        METCollection = cms.InputTag("slimmedMETsMuEGClean", "", process.name_())

    # patch the phi correction parameter sets that are used in runMetCorAndUncFromMiniAOD,
    # we only need to overwrite patMultPhiCorrParams_T1Txy_25ns with the new one
    if options.realData:
        if options.dataEra in ("2016B", "2016C", "2016D", "2016E", "2016F"):
            from MetTools.MetPhiCorrections.tools.multPhiCorr_ReMiniAOD_Data_BCDEF_80X_sumPt_cfi \
                    import multPhiCorr_Data_BCDEF_80X as metPhiCorrParams
        else: # "2016G", "2016Hv2", "2016Hv3"
            from MetTools.MetPhiCorrections.tools.multPhiCorr_ReMiniAOD_Data_GH_80X_sumPt_cfi \
                    import multPhiCorr_Data_GH_80X as metPhiCorrParams
    else:
        from MetTools.MetPhiCorrections.tools.multPhiCorr_Summer16_MC_DY_80X_sumPt_cfi \
                import multPhiCorr_MC_DY_sumPT_80X as metPhiCorrParams

    # actual patch
    getattr(process, "patPFMetTxyCorr").parameters = cms.VPSet(pset for pset in metPhiCorrParams)

#    import PhysicsTools.PatUtils.patPFMETCorrections_cff as metCors
#    metCors.patMultPhiCorrParams_T1Txy_25ns = metPhiCorrParams



#
# custom MET filters
#

process.load("RecoMET.METFilters.BadPFMuonFilter_cfi")
process.BadPFMuonFilter.muons        = muonCollection
process.BadPFMuonFilter.PFCandidates = cms.InputTag("packedPFCandidates")

process.load("RecoMET.METFilters.BadChargedCandidateFilter_cfi")
process.BadChargedCandidateFilter.muons        = muonCollection
process.BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")

process.load("RecoMET.METFilters.badGlobalMuonTaggersMiniAOD_cff")
process.badGlobalMuonTaggerMAOD.muons         = muonCollection
process.badGlobalMuonTaggerMAOD.taggingMode   = cms.bool(True)
process.cloneGlobalMuonTaggerMAOD.muons       = muonCollection
process.cloneGlobalMuonTaggerMAOD.taggingMode = cms.bool(True)


#
# update PUJetId values
#

if options.updatePUJetId:
    process.load("RecoJets.JetProducers.PileupJetID_cfi")
    process.pileupJetIdUpdated = process.pileupJetId.clone(
        jets             = jetCollection,
        vertexes         = cms.InputTag("offlineSlimmedPrimaryVertices"),
        inputIsCorrected = cms.bool(True),
        applyJec         = cms.bool(True)
    )
    seq += process.pileupJetIdUpdated

    process.load("PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff")
    process.updatedPatJets.jetSource         = jetCollection
    process.updatedPatJets.addJetCorrFactors = cms.bool(False)
    process.updatedPatJets.userData.userFloats.src.append("pileupJetIdUpdated:fullDiscriminant")
    process.updatedPatJets.userData.userInts.src.append("pileupJetIdUpdated:fullId")

    # overwrite output collections
    jetCollection = cms.InputTag("updatedPatJets", "", process.name_())

#
# top pt reweighting
#
# Not used -- unnecessary!
#process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")
#process.makeGenEvt
#process.decaySubset.fillMode = cms.string("kME")
#process.myWeightAnalyzer.ttGenEvent = cms.InputTag('genEvt')



#
# ttbar related setup
#

if options.SyncType=="ttjets":
#    if options.addTtbarGenId:
#        process.load("FOO.BAR.ttHFGenFilter_cff") # replace FOO.BAR with your subystem.module

    if options.tagTThf:
        process.load("ttH-LeptonPlusJets.YggdrasilTreeMaker.ttbarCategorization_cff") # replace FOO.BAR with your subystem.module


#
# your custom analyzer
#


# at this point, you need to configure your analyzer with all the information added above
# this following is just an example:



if not options.realData:
    from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets

    setattr( process , 'myGenParticlesForJets' ,
             cms.EDProducer("InputGenJetsParticleSelector",
                            src = cms.InputTag("packedGenParticles"),
                            ignoreParticleIDs = cms.vuint32(
                              1000022,
                              1000012, 1000014, 1000016,
                              2000012, 2000014, 2000016,
                              1000039, 5100039,
                              4000012, 4000014, 4000016,
                              9900012, 9900014, 9900016,
                              39, 12,14,16
                            ),
                            partonicFinalState = cms.bool(False),
                            excludeResonances = cms.bool(False),
                            excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
                            tausAsJets = cms.bool(False)
                            ) )
    
    genJetInputParticleCollection = 'myGenParticlesForJets' 
    
    from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
    process.ak4GenJetsReproduced = ak4GenJets.clone(
        src = genJetInputParticleCollection,
        rParam = cms.double(0.4),
        jetAlgorithm = cms.string("AntiKt")
        )
    
    
    process.GenParticleWithoutChargedLeptonFromTop = cms.EDProducer('GenParticleTopOriginChargedleptonFilter',
                                                                    PackedGenParticle = cms.InputTag( "packedGenParticles" )
                                                                    , PrunedGenParticle = cms.InputTag( "prunedGenParticles" )
                                                                    )
    
    process.myGenParticlesWithChargedLeptonFromTopForJet = process.myGenParticlesForJets . clone(
        src = cms.InputTag("GenParticleWithoutChargedLeptonFromTop","TopOriginChargedleptonFilteredGenParticle","")
        )
    process.ak4GenJetsWithChargedLepFromTop = ak4GenJets.clone(
        src = cms.InputTag( 'myGenParticlesWithChargedLeptonFromTopForJet' ),
        rParam = cms.double(0.4),
        jetAlgorithm = cms.string("AntiKt")
        )
	
###############
#### tt+X
###############



# Setting input particle collections to be used by the tools
    genJetCollection = 'ak4GenJetsCustom'
    genParticleCollection = 'prunedGenParticles'
    genJetInputParticleCollection = 'packedGenParticles'
    
## producing a subset of particles to be used for jet clustering
    from RecoJets.Configuration.GenJetParticles_cff import genParticlesForJetsNoNu
    process.genParticlesForJetsNoNu = genParticlesForJetsNoNu.clone(
	src = genJetInputParticleCollection
        )
    
# Supplies PDG ID to real name resolution of MC particles
    process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
    
# Producing own jets for testing purposes
    from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
    process.ak4GenJetsCustom = ak4GenJets.clone(
        src = 'genParticlesForJetsNoNu',
        #    src = genJetInputParticleCollection,
        rParam = cms.double(0.4),
        jetAlgorithm = cms.string("AntiKt")
        )
    
    # Ghost particle collection used for Hadron-Jet association 
    # MUST use proper input particle collection
    from PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi import selectedHadronsAndPartons
    process.selectedHadronsAndPartons = selectedHadronsAndPartons.clone(
        particles = genParticleCollection
        )
    
    # Input particle collection for matching to gen jets (partons + leptons) 
    # MUST use use proper input jet collection: the jets to which hadrons should be associated
    # rParam and jetAlgorithm MUST match those used for jets to be associated with hadrons
    # More details on the tool: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagMCTools#New_jet_flavour_definition
    from PhysicsTools.JetMCAlgos.AK4PFJetsMCFlavourInfos_cfi import ak4JetFlavourInfos
    process.genJetFlavourInfos = ak4JetFlavourInfos.clone(
        jets = genJetCollection,
        rParam = cms.double(0.4),
        jetAlgorithm = cms.string("AntiKt")
        )
    
    
# Plugin for analysing B hadrons
# MUST use the same particle collection as in selectedHadronsAndPartons
    from PhysicsTools.JetMCAlgos.GenHFHadronMatcher_cff import matchGenBHadron
    process.matchGenBHadron = matchGenBHadron.clone(
        genParticles = genParticleCollection
        )
    
    # Plugin for analysing C hadrons
    # MUST use the same particle collection as in selectedHadronsAndPartons
    from PhysicsTools.JetMCAlgos.GenHFHadronMatcher_cff import matchGenCHadron
    process.matchGenCHadron = matchGenCHadron.clone(
        genParticles = genParticleCollection
        )
    
## Producer for ttbar categorisation ID
# MUST use same genJetCollection as used for tools above
    from TopQuarkAnalysis.TopTools.GenTtbarCategorizer_cfi import categorizeGenTtbar
    process.categorizeGenTtbar = categorizeGenTtbar.clone(
        genJetPtMin = 20.,
        genJetAbsEtaMax = 2.4,
        genJets = genJetCollection,
    )



# Set up JetCorrections chain to be used in MiniAODHelper
# Note: name is hard-coded to ak4PFchsL1L2L3 and does not
# necessarily reflect actual corrections level
from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
process.ak4PFCHSL1Fastjet = cms.ESProducer(
  'L1FastjetCorrectionESProducer',
  level = cms.string('L1FastJet'),
  algorithm = cms.string('AK4PFchs'),
  srcRho = cms.InputTag( 'fixedGridRhoFastjetAll' )
  )
process.ak4PFchsL2Relative = ak4CaloL2Relative.clone( algorithm = 'AK4PFchs' )
process.ak4PFchsL3Absolute = ak4CaloL3Absolute.clone( algorithm = 'AK4PFchs' )
process.ak4PFchsResidual = ak4CaloResidual.clone( algorithm = 'AK4PFchs' )
process.ak4PFchsL1L2L3 = cms.ESProducer("JetCorrectionESChain",
  correctors = cms.vstring(
    'ak4PFCHSL1Fastjet',
    'ak4PFchsL2Relative',
    'ak4PFchsL3Absolute')
)
if options.realData:
  process.ak4PFchsL1L2L3.correctors.append('ak4PFchsResidual') # add residual JEC for data

process.ak8PFCHSL1Fastjet = cms.ESProducer(
  'L1FastjetCorrectionESProducer',
  level = cms.string('L1FastJet'),
  algorithm = cms.string('AK8PFchs'),
  srcRho = cms.InputTag( 'fixedGridRhoFastjetAll' )
  )
process.ak8PFchsL2Relative = ak4CaloL2Relative.clone( algorithm = 'AK8PFchs' )
process.ak8PFchsL3Absolute = ak4CaloL3Absolute.clone( algorithm = 'AK8PFchs' )
process.ak8PFchsResidual = ak4CaloResidual.clone( algorithm = 'AK8PFchs' )
process.ak8PFchsL1L2L3 = cms.ESProducer("JetCorrectionESChain",
  correctors = cms.vstring(
    'ak8PFCHSL1Fastjet',
    'ak8PFchsL2Relative',
    'ak8PFchsL3Absolute')
)

if options.realData:
  process.ak8PFchsL1L2L3.correctors.append('ak8PFchsResidual') # add residual JEC for data
  

## update jes
if options.realData:
    process.GlobalTag.toGet.append(
        cms.PSet(
            connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Summer16_23Sep2016AllV3_DATA_AK4PFchs'),
            label  = cms.untracked.string('AK4PFchs')
            )
        )
    process.GlobalTag.toGet.append(
        cms.PSet(
            connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Summer16_23Sep2016AllV3_DATA_AK4PFchs'),
            label  = cms.untracked.string('AK8PFchs')
            )
        )

# process.myAnalyzer = ...
process.ttHTreeMaker = cms.EDAnalyzer('YggdrasilTreeMaker2017')
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(options.outputFile)
)


# pat object collections
process.ttHTreeMaker.electronCollection = electronCollection
process.ttHTreeMaker.muonCollection     = muonCollection
process.ttHTreeMaker.tauCollection      = tauCollection
process.ttHTreeMaker.photonCollection   = photonCollection
process.ttHTreeMaker.METCollection      = METCollection
process.ttHTreeMaker.jetCollection      = jetCollection
process.ttHTreeMaker.genjetCollection   = genjetCollection

process.ttHTreeMaker.realData = cms.bool(options.realData)
process.ttHTreeMaker.dataEra = cms.string(options.dataEra)
process.ttHTreeMaker.DoSync = cms.bool(options.DoSync)
process.ttHTreeMaker.SyncType = cms.string(options.SyncType)
process.ttHTreeMaker.SyncFile = cms.string(options.SyncFile)
process.ttHTreeMaker.SyncDebug = cms.bool(options.SyncDebug)
process.ttHTreeMaker.SkipEvents = cms.bool(options.SkipEvents)
process.ttHTreeMaker.doSystematics = cms.bool(options.doSystematics)
process.ttHTreeMaker.saveOnlySelected = cms.bool(options.saveOnlySelected)



# electron VID collections - currently not used
#process.ttHTreeMaker.electronVIDCollections = cms.VInputTag(
#     "egmGsfElectronIDs:mvaEleID-Spring15-25ns-Trig-V1-wp80",
#     "egmGsfElectronIDs:mvaEleID-Spring15-25ns-Trig-V1-wp90",
#     "egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp80",
#     "egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp90",
#     "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-veto",
#     "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose",
#     "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium",
#     "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight"
# )

process.ttHTreeMaker.BadPFMuonFilter = cms.InputTag("BadPFMuonFilter")
process.ttHTreeMaker.BadChargedCandidateFilter = cms.InputTag("BadChargedCandidateFilter")
process.ttHTreeMaker.badGlobalMuonTaggerMAOD = cms.InputTag("badGlobalMuonTaggerMAOD")
process.ttHTreeMaker.cloneGlobalMuonTaggerMAOD = cms.InputTag("cloneGlobalMuonTaggerMAOD")

# additional MET filters
#process.ttHTreeMaker.additionalMETFilterCollections = cms.VInputTag(
#     "BadPFMuonFilter",
#     "BadChargedCandidateFilter",
#     "badGlobalMuonTaggerMAOD",
#     "cloneGlobalMuonTaggerMAOD"
# )
 
#from RecoMET.METFilters.metFilters_cff import HBHENoiseFilterResultProducer, HBHENoiseFilter, HBHENoiseIsoFilter, hcalLaserEventFilter
#from RecoMET.METFilters.metFilters_cff import EcalDeadCellTriggerPrimitiveFilter, eeBadScFilter, ecalLaserCorrFilter, EcalDeadCellBoundaryEnergyFilter
#from RecoMET.METFilters.metFilters_cff import primaryVertexFilter, CSCTightHaloFilter, CSCTightHaloTrkMuUnvetoFilter, CSCTightHalo2015Filter, globalTightHalo2016Filter, globalSuperTightHalo2016Filter, HcalStripHaloFilter
#from RecoMET.METFilters.metFilters_cff import goodVertices, trackingFailureFilter, trkPOGFilters, manystripclus53X, toomanystripclus53X, logErrorTooManyClusters
#from RecoMET.METFilters.metFilters_cff import chargedHadronTrackResolutionFilter, muonBadTrackFilter
#from RecoMET.METFilters.metFilters_cff import metFilters

# individual filters
#Flag_HBHENoiseFilter = cms.Path(HBHENoiseFilterResultProducer * HBHENoiseFilter)
#Flag_HBHENoiseIsoFilter = cms.Path(HBHENoiseFilterResultProducer * HBHENoiseIsoFilter)
#Flag_CSCTightHaloFilter = cms.Path(CSCTightHaloFilter)
#Flag_CSCTightHaloTrkMuUnvetoFilter = cms.Path(CSCTightHaloTrkMuUnvetoFilter)
#Flag_CSCTightHalo2015Filter = cms.Path(CSCTightHalo2015Filter)
#Flag_globalTightHalo2016Filter = cms.Path(globalTightHalo2016Filter)
#Flag_globalSuperTightHalo2016Filter = cms.Path(globalSuperTightHalo2016Filter)
#Flag_HcalStripHaloFilter = cms.Path(HcalStripHaloFilter)
#Flag_hcalLaserEventFilter = cms.Path(hcalLaserEventFilter)
#Flag_EcalDeadCellTriggerPrimitiveFilter = cms.Path(EcalDeadCellTriggerPrimitiveFilter)
#Flag_EcalDeadCellBoundaryEnergyFilter = cms.Path(EcalDeadCellBoundaryEnergyFilter)
#Flag_goodVertices = cms.Path(primaryVertexFilter)
#Flag_trackingFailureFilter = cms.Path(goodVertices + trackingFailureFilter)
#Flag_eeBadScFilter = cms.Path(eeBadScFilter)
#Flag_ecalLaserCorrFilter = cms.Path(ecalLaserCorrFilter)
#Flag_trkPOGFilters = cms.Path(trkPOGFilters)
#Flag_chargedHadronTrackResolutionFilter = cms.Path(chargedHadronTrackResolutionFilter)
#Flag_muonBadTrackFilter = cms.Path(muonBadTrackFilter)
# and the sub-filters
#Flag_trkPOG_manystripclus53X = cms.Path(~manystripclus53X)
#Flag_trkPOG_toomanystripclus53X = cms.Path(~toomanystripclus53X)
#Flag_trkPOG_logErrorTooManyClusters = cms.Path(~logErrorTooManyClusters)







# and the summary
#Flag_METFilters = cms.Path(metFilters)
 



# ttbar-related collections
#process.ttHTreeMaker.genTtbarIdCollection    = cms.InputTag("categorizeGenTtbar", "genTtbarId")
#process.ttHTreeMaker.ttHFGenFilterCollection = cms.InputTag("ttHFGenFilter")


#
# final path definition
#

#process.p = cms.Path(seq + process.BadPFMuonFilter*process.BadChargedCandidateFilter*process.GenParticleWithoutChargedLeptonFromTop * process.myGenParticlesWithChargedLeptonFromTopForJet * process.ak4GenJetsWithChargedLepFromTop *  
process.p = cms.Path(seq + process.BadPFMuonFilter*process.BadChargedCandidateFilter*process.ttHTreeMaker) 
       #process.ttHTreeMaker)
#print seq
#print process.p
# print process.dumpPython()
