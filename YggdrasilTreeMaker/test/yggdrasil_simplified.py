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
from FWCore.ParameterSet.VarParsing import VarParsing


#
# options
#

options = VarParsing("python")

# set defaults of common options
options.setDefault("inputFiles", "/store/user/sflowers/44949CF4-96C6-E611-B9A0-0025905A6122.root") 
#options.setDefault("inputFiles","root://cmsxrootd.fnal.gov//store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0693E0E7-97BE-E611-B32F-0CC47A78A3D8.root")
#options.setDefault("outputFile", "yggdrasil_treeMaker_simplified.root")
options.setDefault("maxEvents", 1000)

# add custom options
options.register("globalTag",
    "80X_mcRun2_asymptotic_2016_TrancheIV_v8",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "the global tag to use"
)


#options.register("triggers",
 #   [],
  #  VarParsing.multiplicity.list,
  #  VarParsing.varType.string,
   # "triggers to use"
#)
options.register("realData",
    False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "input dataset contains real data"
)
#options.register("dataEra",
 #   "",
 #   VarParsing.multiplicity.singleton,
 #   VarParsing.varType.string,
 #   "the era of the data taking period, e.g. '2016B', empty for MC"
#)
#options.register("lumiFile",
    #"os.environ.get('CMSSW_BASE')+'/src/ttH-LeptonPlusJets/YggdrasilTreeMaker/data/Cert_271036-275783_13TeV_PromptReco_Collisions16_JSON.txt'",
   # VarParsing.multiplicity.singleton,
  #  VarParsing.varType.string,
 #   "file for selecting runs and lumis"
#)
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
    False, # set to True for all ttbar datasets
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "creates the ttbar gen id"
)
options.register("tagTTHF",
    False, # set to True for all ttbar datasets
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "performs ttbar heavy flavour tagging"
)
options.register("reportEvery",
    1000,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "number of events after which a report message is written"
)
options.register("summary",
    True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "print a summary at the end?"
)


##TreeMaker options

options.register("DoSync",True,VarParsing.multiplicity.singleton,VarParsing.varType.bool,"Do the Sync Code, output sync")
options.register("SyncDebug",False,VarParsing.multiplicity.singleton,VarParsing.varType.bool,"Extra Sync Variables for debugging")
options.register("SkipEvents",False,VarParsing.multiplicity.singleton,VarParsing.varType.bool,"Only run on certain events")


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
#if options.lumiFile:
#    process.source.lumisToProcess = LumiList.LumiList(filename=options.lumiFile).getVLuminosityBlockRange()

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

#if options.electronRegression:
  #  if options.electronRegression == "GT":
  #      from EgammaAnalysis.ElectronTools.regressionWeights_cfi import regressionWeights
 #       process = regressionWeights(process)
#    else:
       # from EgammaAnalysis.ElectronTools.regressionWeights_local_cfi import GBRDWrapperRcd
      #  GBRDWrapperRcd.connect = cms.string("sqlite_file:" + options.electronRegression)
     #   process.regressions = GBRDWrapperRcd
    #    process.regressions.DumpStat = cms.untracked.bool(False)
   #     process.es_prefer_regressions = cms.ESPrefer("PoolDBESSource", "regressions")
  #  process.load("EgammaAnalysis.ElectronTools.regressionApplication_cff")
 #   seq += process.regressionApplication
#
   # # set the electron and photon sources
  #  process.slimmedElectrons.src = electronCollection
 #   process.slimmedPhotons.src = photonCollection
#
   # # overwrite output collections
  #  electronCollection = cms.InputTag("slimmedElectrons", "", process.name_())
 #   photonCollection = cms.InputTag("slimmedPhotons", "", process.name_())
#






#
# electron energy smearing
#

# only allowed when regression was used
if options.electronSmearing and options.electronRegression and False:
    process.load("Configuration.StandardSequences.Services_cff")
    process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
        calibratedPatElectrons = cms.PSet(
            initialSeed = cms.untracked.uint32(81),
            engineName  = cms.untracked.string("TRandom3")
        )
    )
    process.load("EgammaAnalysis.ElectronTools.calibratedPatElectronsRun2_cfi")
    from EgammaAnalysis.ElectronTools.calibratedPatElectronsRun2_cfi import files
    process.calibratedPatElectrons.isMC           = cms.bool(not options.realData)
    process.calibratedPatElectrons.correctionFile = cms.string(files[options.electronSmearing])
    process.calibratedPatElectrons.electrons      = electronCollection
    if options.fillSeeds:
        process.calibratedPatElectrons.seedUserInt = process.deterministicSeeds.seedUserInt
    seq += process.calibratedPatElectrons

    # overwrite output collections
    electronCollection = cms.InputTag("calibratedPatElectrons", "", process.name_())


#
# electron VIDs
#

from PhysicsTools.SelectorUtils.tools.vid_id_tools import DataFormat, \
        switchOnVIDElectronIdProducer, setupAllVIDIdsInModule, setupVIDElectronSelection

eleVIDModules = [
    "RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring15_25ns_Trig_V1_cff",
    "RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring16_GeneralPurpose_V1_cff",
    "RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Summer16_80X_V1_cff"
]

switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)

for mod in eleVIDModules:
    setupAllVIDIdsInModule(process, mod, setupVIDElectronSelection)

# update some VID modules to work with potentially changed electron collections
process.selectedElectrons = cms.EDFilter("PATElectronSelector",
    src = electronCollection,
    cut = cms.string("pt>5 && abs(eta)")
)
# overwrite the electron output collection
electronCollection = cms.InputTag("selectedElectrons", "", process.name_())
# update the modules
process.egmGsfElectronIDs.physicsObjectSrc = electronCollection
process.electronRegressionValueMapProducer.srcMiniAOD = electronCollection
process.electronMVAValueMapProducer.srcMiniAOD = electronCollection


#
# MET corrections and uncertainties
#

if options.recorrectMET:
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
    import PhysicsTools.PatUtils.patPFMETCorrections_cff as metCors
    metCors.patMultPhiCorrParams_T1Txy_25ns = metPhiCorrParams

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
        process.slimmedMETsMuEGClean = process.slimmedMETsRecorrected.clone(
            src             = cms.InputTag("patPFMetT1MuEGClean"),
            rawVariation    = cms.InputTag("patPFMetRawMuEGClean"),
            t1Uncertainties = cms.InputTag("patPFMetT1%sMuEGClean")
        )
        del process.slimmedMETsMuEGClean.caloMET

        # overwrite output collections
        METCollection = cms.InputTag("slimmedMETsMuEGClean", "", process.name_())


#
# custom MET filters
#

process.load("RecoMET.METFilters.BadPFMuonFilter_cfi")
process.BadPFMuonFilter.muons        = muonCollection
process.BadPFMuonFilter.PFCandidates = cms.InputTag("packedPFCandidates")

process.load("RecoMET.METFilters.BadChargedCandidateFilter_cfi")
process.BadChargedCandidateFilter.muons        = muonCollection
process.BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")

#process.BadPFMuonFilter.taggingMode = cms.bool(True)
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
# ttbar related setup
#

#if options.addTtbarGenId:
#    process.load("FOO.BAR.ttHFGenFilter_cff") # replace FOO.BAR with your subystem.module

#if options.tagTThf:
 #   process.load("FOO.BAR.ttbarCategorization_cff") # replace FOO.BAR with your subystem.module


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
                39, 12,14,16),
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




# common configs
#process.ttHTreeMaker.outputFile = cms.untracked.string(options.__getattr__("outputFile", noTags=True))
# process.myAnalyzer.triggers   = cms.vstring(options.triggers)

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
	fileName = cms.string('yggdrasil_treeMaker2017.root')
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
process.ttHTreeMaker.DoSync = cms.bool(options.DoSync)
process.ttHTreeMaker.SyncDebug = cms.bool(options.SyncDebug)
process.ttHTreeMaker.SkipEvents = cms.bool(options.SkipEvents)


# electron VID collections
process.ttHTreeMaker.electronVIDCollections = cms.VInputTag(
     "egmGsfElectronIDs:mvaEleID-Spring15-25ns-Trig-V1-wp80",
     "egmGsfElectronIDs:mvaEleID-Spring15-25ns-Trig-V1-wp90",
     "egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp80",
     "egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp90",
     "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-veto",
     "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose",
     "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium",
     "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight"
 )

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
