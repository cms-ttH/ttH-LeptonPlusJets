import FWCore.ParameterSet.Config as cms

process = cms.Process("MAOD")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.load( "Configuration.StandardSequences.FrontierConditions_GlobalTag_cff" )
process.GlobalTag.globaltag = 'MCRUN2_74_V9::All'
#process.GlobalTag.globaltag = 'PHYS14_25_V2::All'
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.options.allowUnscheduled = cms.untracked.bool(True)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

from JetMETCorrections.Configuration.JetCorrectionServices_cff import *

process.ak4PFCHSL1Fastjet = cms.ESProducer(
    'L1FastjetCorrectionESProducer',
    level       = cms.string('L1FastJet'),
    algorithm   = cms.string('AK4PFchs'),
    srcRho      = cms.InputTag( 'fixedGridRhoFastjetAll' )
    )

process.ak4PFchsL2Relative = ak4CaloL2Relative.clone( algorithm = 'AK4PFchs' )
process.ak4PFchsL3Absolute = ak4CaloL3Absolute.clone( algorithm = 'AK4PFchs' )

process.ak4PFchsL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring(
	'ak4PFCHSL1Fastjet', 
        'ak4PFchsL2Relative', 
        'ak4PFchsL3Absolute')
)

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
#        'root://xrootd-cms.infn.it//store/user/shwillia/Spring15_HbbSync/ttHTobb_Spring15_HbbSync.root',
        'root://xrootd-cms.infn.it//store/user/shwillia/Spring15_HbbSync/ttbar_Spring15_HbbSync.root',

#        'root://xrootd-cms.infn.it//store/user/shwillia/Spring15_Sync/ttHbb_spring15_25ns_plusboostedjets.root',
#        '/store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00C90EFC-3074-E411-A845-002590DB9262.root',
#        '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/00000/FC4E6E16-5C7F-E411-8843-002590200AE4.root',

#	    'root://cmsxrootd-site.fnal.gov//store/user/puigh/TTHSync/ttjets_phys14_20bx25_withfatjets_v2.root'

#       '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/00000/08B36E8F-5E7F-E411-9D5A-002590200AE4.root',
#       '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/00000/FC4E6E16-5C7F-E411-8843-002590200AE4.root',
#       '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/10000/629CE4D5-687F-E411-BF71-001E673969FA.root',
#       '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/20000/14587980-CB7E-E411-A0F4-001E67397701.root',
#       '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/30000/9E314FC0-067F-E411-9500-001E67397B11.root',
#       '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/40000/78722DCF-E57E-E411-B437-002590A4FFB8.root',
#       '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/40000/F87FB415-E57E-E411-B7CF-002590A4FFB8.root'

            #'/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/00000/08B36E8F-5E7F-E411-9D5A-002590200AE4.root'
            #'/store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00C90EFC-3074-E411-A845-002590DB9262.root'
            #'/store/mc/Spring14miniaod/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/1E4F9BDC-3E1E-E411-A56C-001E67396EAA.root'
            #'/store/mc/Spring14miniaod/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/00000/004C6DA7-FB03-E411-96BD-0025905A497A.root'
            )
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
from PhysicsTools.JetMCAlgos.sequences.GenHFHadronMatching_cff import genJetFlavourPlusLeptonInfos
process.genJetFlavourPlusLeptonInfos = genJetFlavourPlusLeptonInfos.clone(
    jets = genJetCollection,
    rParam = cms.double(0.4),
    jetAlgorithm = cms.string("AntiKt")
)


# Plugin for analysing B hadrons
# MUST use the same particle collection as in selectedHadronsAndPartons
from PhysicsTools.JetMCAlgos.sequences.GenHFHadronMatching_cff import matchGenBHadron
process.matchGenBHadron = matchGenBHadron.clone(
    genParticles = genParticleCollection
)

# Plugin for analysing C hadrons
# MUST use the same particle collection as in selectedHadronsAndPartons
from PhysicsTools.JetMCAlgos.sequences.GenHFHadronMatching_cff import matchGenCHadron
process.matchGenCHadron = matchGenCHadron.clone(
    genParticles = genParticleCollection
)

## Producer for ttbar categorisation ID
# MUST use same genJetCollection as used for tools above
from PhysicsTools.JetMCAlgos.GenTtbarCategorizer_cfi import categorizeGenTtbar
process.categorizeGenTtbar = categorizeGenTtbar.clone(
    genJetPtMin = 20.,
    genJetAbsEtaMax = 2.4,
    genJets = genJetCollection,
)

process.ttHsyncExercise = cms.EDAnalyzer('TTHSyncExercise',
    genTtbarId = cms.InputTag("categorizeGenTtbar", "genTtbarId"),
    SysType = cms.string(""),
#    # phase space of jets to be stored
#    genJetPtMin = cms.double(20),
#    genJetAbsEtaMax = cms.double(2.4),
#    # input tags holding information about matching
#    genJets = cms.InputTag(genJetCollection),
#    genBHadJetIndex = cms.InputTag("matchGenBHadron", "genBHadJetIndex"),
#    genBHadFlavour = cms.InputTag("matchGenBHadron", "genBHadFlavour"),
#    genBHadFromTopWeakDecay = cms.InputTag("matchGenBHadron", "genBHadFromTopWeakDecay"),
#    genBHadPlusMothers = cms.InputTag("matchGenBHadron", "genBHadPlusMothers"),
#    genBHadPlusMothersIndices = cms.InputTag("matchGenBHadron", "genBHadPlusMothersIndices"),
#    genBHadIndex = cms.InputTag("matchGenBHadron", "genBHadIndex"),
#    genBHadLeptonHadronIndex = cms.InputTag("matchGenBHadron", "genBHadLeptonHadronIndex"),
#    genBHadLeptonViaTau = cms.InputTag("matchGenBHadron", "genBHadLeptonViaTau"),
#    genCHadJetIndex = cms.InputTag("matchGenCHadron", "genCHadJetIndex"),
#    genCHadFlavour = cms.InputTag("matchGenCHadron", "genCHadFlavour"),
#    genCHadFromTopWeakDecay = cms.InputTag("matchGenCHadron", "genCHadFromTopWeakDecay"),
#    genCHadBHadronId = cms.InputTag("matchGenCHadron", "genCHadBHadronId"),
    )




process.TFileService = cms.Service("TFileService",
	fileName = cms.string('ttH_sync_exercise.root')
)

process.p = cms.Path(process.ttHsyncExercise)
