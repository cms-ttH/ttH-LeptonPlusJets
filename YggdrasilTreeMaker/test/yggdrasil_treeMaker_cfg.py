import FWCore.ParameterSet.Config as cms

process = cms.Process("MAOD")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'PHYS14_25_V1', '')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
    )

from JetMETCorrections.Configuration.JetCorrectionServices_cff import *

process.ak4PFCHSL1Fastjet = cms.ESProducer(
    'L1FastjetCorrectionESProducer',
    level       = cms.string('L1FastJet'),
    algorithm   = cms.string('AK4PFchs'),
    srcRho      = cms.InputTag( 'fixedGridRhoAll' )
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
            '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/00000/08B36E8F-5E7F-E411-9D5A-002590200AE4.root'
            #'/store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00C90EFC-3074-E411-A845-002590DB9262.root'
            #'/store/mc/Spring14miniaod/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/1E4F9BDC-3E1E-E411-A56C-001E67396EAA.root'
            #'/store/mc/Spring14miniaod/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/00000/004C6DA7-FB03-E411-96BD-0025905A497A.root'
            )
)


from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets

process.chs = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))

process.ak4PFJets = ak4PFJets.clone(src = 'packedPFCandidates', doAreaFastjet = True) # no idea while doArea is false by default, but it's True in RECO so we have to set it
process.ak4PFJetsCHS = ak4PFJets.clone(src = 'chs', doAreaFastjet = True) # no idea while doArea is false by default, but it's True in RECO so we have to set it
process.ak4GenJets = ak4GenJets.clone(src = 'packedGenParticles')

process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
addJetCollection(
   process,
   postfix   = "",
   labelName = 'AK4PFCHS',
   jetSource = cms.InputTag('ak4PFJetsCHS'),
   trackSource = cms.InputTag('unpackedTracksAndVertices'), 	
   pvSource = cms.InputTag('unpackedTracksAndVertices'), 
   jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'Type-2'),
   btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
   #btagDiscriminators = [      'combinedSecondaryVertexBJetTags','combinedMVABJetTags','combinedInclusiveSecondaryVertexV2BJetTags'     ]
   )


process.patJetPartonMatchAK4PFCHS.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
process.patJetPartonsLegacy.src = "prunedGenParticles" # if using legacy jet flavour (not used by default)
process.patJetPartonsLegacy.skipFirstN = cms.uint32(0) # do not skip first 6 particles, we already pruned some!
process.patJetPartonsLegacy.acceptNoDaughters = cms.bool(True) # as we drop intermediate stuff, we need to accept quarks with no siblings
process.patJetCorrFactorsAK4PFCHS.primaryVertices = "offlineSlimmedPrimaryVertices"

process.slimmedJets = cms.EDProducer("PATJetSlimmer",
    dropTagInfos = cms.string('1'),
    src = cms.InputTag("patJetsAK4PFCHS"),
    dropJetVars = cms.string('1'),
    dropSpecific = cms.string('0'),
    packedPFCandidates = cms.InputTag("packedPFCandidates"),
    dropDaughters = cms.string('0'),
    dropTrackRefs = cms.string('1')
)

#recreate tracks and pv for btagging
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
#process.options.allowUnscheduled = cms.untracked.bool(True)


process.ttHTreeMaker = cms.EDAnalyzer('YggdrasilTreeMaker',
    )

process.TFileService = cms.Service("TFileService",
	fileName = cms.string('yggdrasil_treeMaker.root')
)

process.p = cms.Path(
    process.chs
    *process.unpackedTracksAndVertices
    *process.ak4PFJetsCHS
    *process.ak4GenJets
    *process.jetTracksAssociatorAtVertexAK4PFCHS
    *process.impactParameterTagInfosAK4PFCHS
    *process.secondaryVertexTagInfosAK4PFCHS
    *process.combinedSecondaryVertexBJetTagsAK4PFCHS
    *process.patJetChargeAK4PFCHS
    *process.patJetGenJetMatchAK4PFCHS
    *process.patJetPartonMatchAK4PFCHS
    *process.patJetCorrFactorsAK4PFCHS
    *process.patJetPartons
    *process.patJetFlavourAssociationAK4PFCHS
    *process.patJetsAK4PFCHS
    #*process.slimmedJets
    #*process.out
    *process.ttHTreeMaker
)


#process.p = cms.Path(process.ttHTreeMaker)
