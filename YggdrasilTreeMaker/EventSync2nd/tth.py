import FWCore.ParameterSet.Config as cms



isPUPPI=False
#--> if isPUPPI=true, change the code too.


#isMC=True
isMC=True

isTTBARMC=False

genjetInputTag = cms.InputTag("slimmedGenJets","","")
#genjetInputTag = cms.InputTag("ak4GenJetsReproduced","","")
#genjetInputTag = cms.InputTag("ak4GenJetsWithChargedLepFromTop","","")



enableJECFromLocalDB=True


# Switch to perform lumi-mask inside this python script.
# It is preferable to set this option False as default,
#   because, when we submit job to grid, we also set the file too.
ForDebugAndEventSync_EnableLumiMaskByHand=False


process = cms.Process("MAOD")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

#### caution: use the correct global tag for MC or Data 
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

# Update global tag based on : https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions?rev=568
if isMC:
    process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_miniAODv2_v1'
else :
    process.GlobalTag.globaltag = '80X_dataRun2_Prompt_ICHEP16JEC_v0'




# Load the producer for MVA IDs. Make sure it is also added to the sequence!
process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.options.allowUnscheduled = cms.untracked.bool(True)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )



if enableJECFromLocalDB :
    print "JEC is applied with LOCAL DB."

    import sys
    import os.path
   

    if not isMC :
        process.GlobalTag.toGet.append(
            cms.PSet(
                connect = cms.string('sqlite:///'+os.environ.get('CMSSW_BASE')+'/src/ttH-LeptonPlusJets/YggdrasilTreeMaker/data/Spring16_25nsV6_DATA.db'),
                record = cms.string('JetCorrectionsRecord'),
                tag    = cms.string('JetCorrectorParametersCollection_Spring16_25nsV6_DATA_AK4PFchs'),
                label  = cms.untracked.string('AK4PFchs')
                )
            )
        process.GlobalTag.toGet.append(
            cms.PSet(
                connect = cms.string('sqlite:///'+os.environ.get('CMSSW_BASE')+'/src/ttH-LeptonPlusJets/YggdrasilTreeMaker/data/Spring16_25nsV6_DATA.db'),
                record = cms.string('JetCorrectionsRecord'),
                tag    = cms.string('JetCorrectorParametersCollection_Spring16_25nsV6_DATA_AK8PFchs'),
                label  = cms.untracked.string('AK8PFchs')
                )
            )
    else:
        process.GlobalTag.toGet.append(
            cms.PSet(
                connect = cms.string('sqlite:///'+os.environ.get('CMSSW_BASE')+'/src/ttH-LeptonPlusJets/YggdrasilTreeMaker/data/Spring16_25nsV6_MC.db'),
                record = cms.string('JetCorrectionsRecord'),
                tag    = cms.string('JetCorrectorParametersCollection_Spring16_25nsV6_MC_AK4PFchs'),
                label  = cms.untracked.string('AK4PFchs')
                )
            )
        process.GlobalTag.toGet.append(
            cms.PSet(
                connect = cms.string('sqlite:///'+os.environ.get('CMSSW_BASE')+'/src/ttH-LeptonPlusJets/YggdrasilTreeMaker/data/Spring16_25nsV6_MC.db'),
                record = cms.string('JetCorrectionsRecord'),
                tag    = cms.string('JetCorrectorParametersCollection_Spring16_25nsV6_MC_AK8PFchs'),
                label  = cms.untracked.string('AK8PFchs')
                )
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
if not isMC :
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

if not isMC :
 process.ak8PFchsL1L2L3.correctors.append('ak8PFchsResidual') # add residual JEC for data



process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
'file:///uscmst1b_scratch/lpc1/3DayLifetime/satoshi//tth/0089CC67-6338-E611-947D-0025904C4E2A.root'
            )
)


###############
### GenJet production from ChargedLeptonVetoedGenParticles

if not isMC :
    if ForDebugAndEventSync_EnableLumiMaskByHand :
        import sys
        import os.path
        import FWCore.PythonUtilities.LumiList as Lumilist
        process.source.lumisToProcess = LumiList.LumiList(filename =
                                                          os.environ.get('CMSSW_BASE')+'/src/ttH-LeptonPlusJets/YggdrasilTreeMaker/data/Cert_271036-275783_13TeV_PromptReco_Collisions16_JSON.txt'
                                                          ).getVLuminosityBlockRange()



if isMC :
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
    
    
    process.GenParticleWithoutChargedLeptonFropTop = cms.EDProducer('GenParticleTopOriginChargedleptonFilter',
                                                                    PackedGenParticle = cms.InputTag( "packedGenParticles" )
                                                                    , PrunedGenParticle = cms.InputTag( "prunedGenParticles" )
                                                                    )
    
    process.myGenParticlesWithChargedLeptonFromTopForJet = process.myGenParticlesForJets . clone(
        src = cms.InputTag("GenParticleWithoutChargedLeptonFropTop","TopOriginChargedleptonFilteredGenParticle","")
        )
    process.ak4GenJetsWithChargedLepFromTop = ak4GenJets.clone(
        src = cms.InputTag( 'myGenParticlesWithChargedLeptonFromTopForJet' ),
        rParam = cms.double(0.4),
        jetAlgorithm = cms.string("AntiKt")
        )

    if isTTBARMC :
        process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")
        process.initSubset.src=cms.InputTag("prunedGenParticles")
        process.decaySubset.src=cms.InputTag("prunedGenParticles")
        process.decaySubset.fillMode = cms.string("kStable")
          ## define fill mode. The following modes are available:
          ## 'kStable' : status 2 equivalents (after parton shower) are
          ##             calculated and saved (as status 2 particles)
          ## 'kME'     : status 3 particles (from matrix element, before
          ##             parton shower) are saved (as status 3 particles)


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
    


if isMC :
    if isPUPPI :
        process.ttHTreeMaker = cms.EDAnalyzer('YggdrasilTreeMaker',
                                              genjet =  genjetInputTag,
                                              inputfiletype   =  cms.string("TTbarMC") if isTTBARMC else cms.string("MC") ,
                                              jetPU = cms.string( "PUPPI" )
                                          )
    else:
        process.ttHTreeMaker = cms.EDAnalyzer('YggdrasilTreeMaker',
                                              genjet =  genjetInputTag,
                                              inputfiletype   =  cms.string("TTbarMC") if isTTBARMC else cms.string("MC") ,
                                          jetPU = cms.string( "CHS" )
                                          )

else :
    if isPUPPI :
        process.ttHTreeMaker = cms.EDAnalyzer('YggdrasilTreeMaker',
                                              genjet =  genjetInputTag,
                                          inputfiletype    =  cms.string("data"),
                                          jetPU = cms.string( "PUPPI" )
                                          )
    else:
        process.ttHTreeMaker = cms.EDAnalyzer('YggdrasilTreeMaker',
                                              genjet =  genjetInputTag,
                                          inputfiletype    =  cms.string("data"),
                                          jetPU = cms.string( "CHS" )
                                          )
        
    
process.TFileService = cms.Service("TFileService",
	fileName = cms.string('yggdrasil_treeMaker.root')
)

if isMC : 
    process.p = cms.Path(
        process.GenParticleWithoutChargedLeptonFropTop * process.myGenParticlesWithChargedLeptonFromTopForJet * process.ak4GenJetsWithChargedLepFromTop *  
        process.electronMVAValueMapProducer * process.ttHTreeMaker)
else :
    process.p = cms.Path(
        process.electronMVAValueMapProducer * process.ttHTreeMaker)
