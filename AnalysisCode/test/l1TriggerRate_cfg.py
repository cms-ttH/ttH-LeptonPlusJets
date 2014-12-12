import FWCore.ParameterSet.Config as cms

import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("TTHTrigger")

#inputFileList = 'inputFiles_SimL1Emulator_Stage1_NeutrinoGun_PU40bx25_POSTLS162_V2.txt'

#inputFileList = 'inputFiles_SimL1Emulator1p5_L1EGJet_Stage1_NeutrinoGun_PU40bx25_POSTLS162_V2.txt'

inputFileList = 'inputFiles_SimL1Emulator1p5_L1EGHTT_Stage1_NeutrinoGun_PU40bx25_POSTLS162_V2.txt'

readFiles = cms.untracked.vstring( FileUtils.loadListFromFile(inputFileList) )

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS162_V2::All'

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )


# override the L1 menu from an Xml file
process.l1GtTriggerMenuXml = cms.ESProducer("L1GtTriggerMenuXmlProducer",
  TriggerMenuLuminosity = cms.string('startup'),
  DefXmlFile = cms.string('L1Menu_Collisions2015_25ns_v1_L1T_Scales_20101224_Imp0_0x102f_L1EGHTT.xml'),
  VmeXmlFile = cms.string('')
)
process.L1GtTriggerMenuRcdSource = cms.ESSource("EmptyESSource",
  recordName = cms.string('L1GtTriggerMenuRcd'),
  iovIsRunNotTime = cms.bool(True),
  firstValid = cms.vuint32(1)
)

process.es_prefer_l1GtParameters = cms.ESPrefer('L1GtTriggerMenuXmlProducer','l1GtTriggerMenuXml')


process.source = cms.Source("PoolSource",
                            #fileNames = cms.untracked.vstring(
                            #    'file:/uscms_data/d2/dpuigh/TTH/triggerRun2/Emulator1p5/CMSSW_7_2_0_pre7/src/SimL1Emulator_Stage1_PP_L1EGHTT.root'
                            #    #'/store/user/lpctrig/HATS/SimL1Emulator_Stage1_PP_Neutrino_Pt-2to20_gun_PU40bx25_POSTLS162_V2-v1.root'
                            #)
                            fileNames = readFiles
)

process.l1TriggerRate = cms.EDAnalyzer('L1TriggerRate',
    )

process.TFileService = cms.Service("TFileService",
	fileName = cms.string('l1t_rates_13TeV_histo.root')
)

process.p = cms.Path(process.l1TriggerRate)
