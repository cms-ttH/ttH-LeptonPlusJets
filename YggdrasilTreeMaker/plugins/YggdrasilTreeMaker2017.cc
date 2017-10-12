// -*- C++ -*-
//
// Package:    ttH-LeptonPlusJets/YggdrasilTreeMaker
// Class:      YggdrasilTreeMaker
// 
/**\class YggdrasilTreeMaker YggdrasilTreeMaker2017.cc ttH-LeptonPlusJets/YggdrasilTreeMaker/plugins/YggdrasilTreeMaker2017.cc
 Description: [one line class summary]
 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Sean Flowers
//         Created:  Wed, 15 Mar 2017 
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Lepton.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/strbitset.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "FWCore/Common/interface/TriggerNames.h"


#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "JetMETCorrections/JetCorrector/interface/JetCorrector.h"

#include "MiniAOD/MiniAODHelper/interface/MiniAODHelper.h"
#include "MiniAOD/MiniAODHelper/interface/Systematics.h"
#include "MiniAOD/MiniAODHelper/interface/TopTagger.h"
#include "MiniAOD/MiniAODHelper/interface/HiggsTagger.h"
#include "ttH-LeptonPlusJets/AnalysisCode/interface/YggdrasilEventVars.h"

#include "ttH-LeptonPlusJets/YggdrasilTreeMaker/interface/ttHYggdrasilEventSelection.h"
#include "ttH-LeptonPlusJets/YggdrasilTreeMaker/interface/ttHYggdrasilScaleFactors.h"

#include "DataFormats/JetReco/interface/PileupJetIdentifier.h"

class YggdrasilTreeMaker2017 : public edm::EDAnalyzer {
	public:
	  explicit YggdrasilTreeMaker2017(const edm::ParameterSet&);
	  ~YggdrasilTreeMaker2017();
	  
	  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      virtual void beginRun(edm::Run const& iRun,edm::EventSetup const& iSetup) override;
      
      
      
      edm::EDGetTokenT <reco::BeamSpot> beamspotToken;
      edm::EDGetTokenT <reco::VertexCollection> vertexToken;
      
      edm::EDGetTokenT<bool> BadChCandFilterToken;
      edm::EDGetTokenT<bool> BadPFMuonFilterToken;
      edm::EDGetTokenT<bool> badGlobalMuonTaggerMAODToken;
      edm::EDGetTokenT<bool> cloneGlobalMuonTaggerMAODToken;
      
      edm::EDGetTokenT <edm::TriggerResults> triggerResultsToken;
      edm::EDGetTokenT <edm::TriggerResults> filterResultsToken;
      edm::EDGetTokenT <pat::TriggerObjectStandAloneCollection> TriggerObjectStandAloneToken ;
      
      edm::EDGetTokenT <pat::ElectronCollection> electronToken;
      edm::EDGetTokenT <pat::MuonCollection> muonToken;
      edm::EDGetTokenT <pat::JetCollection> jetToken;
      edm::EDGetTokenT <pat::METCollection> metToken;
     
      edm::EDGetTokenT<double> rhoToken;
      edm::EDGetTokenT<reco::GenJetCollection> genJetsToken;
     
      edm::EDGetTokenT <std::vector< PileupSummaryInfo > > puInfoToken;
      
      edm::EDGetTokenT<int> genTtbarIdToken_;
     
     
      HLTConfigProvider hlt_config_;
      HLTConfigProvider filter_config_;
     
      std::map<std::string, int> flt_cppath_;
      std::vector<std::string> flt_filterNames_;
      
      MiniAODHelper miniAODhelper;
      ttHYggdrasilEventSelection selection;
      ttHYggdrasilScaleFactors scalefactors;
     
      bool realData;
      std::string dataEra;
      bool DoSync;
      std::string SyncType;
      bool SkipEvents;
      bool SyncDebug;
      bool doSystematics;
      bool saveOnlySelected;
     
      int numEvents_;
      int badChcount;
      int badpfmucount;
      int badglobalmucount;
      int cloneglobalmucount;
      
      TTree *worldTree;
      yggdrasilEventVars *eve;
     
      
    
};



YggdrasilTreeMaker2017::YggdrasilTreeMaker2017(const edm::ParameterSet& iConfig)
{
   beamspotToken = consumes <reco::BeamSpot> (edm::InputTag(std::string("offlineBeamSpot")));
   vertexToken = consumes <reco::VertexCollection> (edm::InputTag(std::string("offlineSlimmedPrimaryVertices")));
   
   BadChCandFilterToken = consumes<bool>(iConfig.getParameter<edm::InputTag>("BadChargedCandidateFilter"));
   BadPFMuonFilterToken = consumes<bool>(iConfig.getParameter<edm::InputTag>("BadPFMuonFilter"));
   badGlobalMuonTaggerMAODToken = consumes<bool>(iConfig.getParameter<edm::InputTag>("badGlobalMuonTaggerMAOD"));
   cloneGlobalMuonTaggerMAODToken = consumes<bool>(iConfig.getParameter<edm::InputTag>("cloneGlobalMuonTaggerMAOD"));
   
   triggerResultsToken = consumes <edm::TriggerResults> (edm::InputTag(std::string("TriggerResults"), std::string(""), "HLT"));
   filterResultsToken = consumes <edm::TriggerResults> (edm::InputTag(std::string("TriggerResults"), std::string(""), "PAT"));
   TriggerObjectStandAloneToken = consumes <pat::TriggerObjectStandAloneCollection>( edm::InputTag( std::string ( "selectedPatTrigger" ), std::string("") , std::string("RECO") )) ;

   electronToken = consumes<pat::ElectronCollection> (iConfig.getParameter<edm::InputTag>("electronCollection"));
   muonToken = consumes<pat::MuonCollection> (iConfig.getParameter<edm::InputTag>("muonCollection"));
   jetToken  = consumes< pat::JetCollection >(iConfig.getParameter<edm::InputTag>("jetCollection"));
   metToken = consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("METCollection"));
   
   rhoToken = consumes <double> (edm::InputTag(std::string("fixedGridRhoFastjetAll")));
   genJetsToken = consumes< reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("genjetCollection"));
   
   puInfoToken = consumes <std::vector< PileupSummaryInfo > > (edm::InputTag(std::string("slimmedAddPileupInfo")));


   realData = iConfig.getParameter<bool>("realData");
   dataEra = iConfig.getParameter<std::string>("dataEra");
   DoSync = iConfig.getParameter<bool>("DoSync");
   SyncType = iConfig.getParameter<std::string>("SyncType");
   SyncDebug = iConfig.getParameter<bool>("SyncDebug");
   SkipEvents = iConfig.getParameter<bool>("SkipEvents");
   doSystematics = iConfig.getParameter<bool>("doSystematics");
   saveOnlySelected = iConfig.getParameter<bool>("saveOnlySelected");
   
   if(!realData)genTtbarIdToken_ = consumes<int>( edm::InputTag( "categorizeGenTtbar", "genTtbarId","" ) )  ;
   
   analysisType::analysisType iAnalysisType = analysisType::LJ;
   const int sampleID = realData ? -1 : 1;
   miniAODhelper.SetUp(dataEra, sampleID, iAnalysisType, realData);
   
   edm::Service<TFileService> fs_;
   worldTree = fs_->make<TTree>("worldTree", "worldTree");
   eve=0; 
   worldTree->Branch("eve.", "yggdrasilEventVars", &eve, 8000, 1);

   
}
  


YggdrasilTreeMaker2017::~YggdrasilTreeMaker2017()
{  
}



void
YggdrasilTreeMaker2017::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   numEvents_++;

   
   miniAODhelper.UpdateJetCorrectorUncertainties( iSetup );
   miniAODhelper.SetAK8JetCorrectorUncertainty( iSetup );
   
   
   edm::Handle<reco::BeamSpot> bsHandle;
   edm::Handle<reco::VertexCollection> vtxHandle;
   
   edm::Handle<edm::TriggerResults> triggerResults;
   edm::Handle<edm::TriggerResults> filterResults; 
   edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
    
   edm::Handle<pat::ElectronCollection> electrons;
   edm::Handle<pat::MuonCollection> muons;
   edm::Handle<pat::JetCollection> pfjets;
   edm::Handle<pat::METCollection> pfmet;
   
   edm::Handle<double> rhoHandle;
   edm::Handle< reco::GenJetCollection > genjets;
   
   edm::Handle<std::vector< PileupSummaryInfo > > PupInfo;
   
   edm::Handle<int> genTtbarId;
   
   
   
   iEvent.getByToken(beamspotToken,bsHandle);
   iEvent.getByToken(vertexToken,vtxHandle);
   
   iEvent.getByToken(triggerResultsToken, triggerResults);
   iEvent.getByToken(filterResultsToken, filterResults);
   iEvent.getByToken( TriggerObjectStandAloneToken , triggerObjects ) ;
   
   iEvent.getByToken(electronToken,electrons);
   iEvent.getByToken(muonToken,muons);
   iEvent.getByToken( jetToken,pfjets);
   iEvent.getByToken(metToken,pfmet);
  
   iEvent.getByToken(rhoToken,rhoHandle);
   if(!realData)iEvent.getByToken(genJetsToken,genjets);
  
   iEvent.getByToken(puInfoToken,PupInfo);
   
   if(!realData)iEvent.getByToken(genTtbarIdToken_,genTtbarId);
   
   eve->initialize();
  
   
   int run  = iEvent.id().run();
   int lumi = iEvent.luminosityBlock();
   long evt = iEvent.id().event();
  
   
   bool DoThisEvent = true;
   if(SkipEvents){
  	DoThisEvent = false;
   	int skiplist[] = {31647,368651,370674,396382,624978,625032,706089,783185,783846,843469,1523860,1960867,1984818,2321482,2426173,2490426,2490547,2618343,2818409,2894546,2894588,2905966,3178582,3222965,3270603,3482144,3482145,3726213};
  	for(int listn=0;listn<int(sizeof(skiplist)/sizeof(int));listn++){
  		if(evt==skiplist[listn])DoThisEvent=true;
  	}
   }
   if(DoThisEvent){
   
   ///////////////////
   //Event Cleaning//
   //////////////////
   
   //MET Mu Filters
   
    edm::Handle<bool> ifilterbadChCand;
    iEvent.getByToken(BadChCandFilterToken, ifilterbadChCand);
    bool  filterbadChCandidate = *ifilterbadChCand;
    if(filterbadChCandidate)badChcount++;
    
    
    edm::Handle<bool> ifilterbadPFMuon;
    iEvent.getByToken(BadPFMuonFilterToken, ifilterbadPFMuon);
    bool filterbadPFMuon = *ifilterbadPFMuon;
    if(filterbadPFMuon)badpfmucount++;
    
    edm::Handle<bool> ifilterbadglobalmu;
    iEvent.getByToken(badGlobalMuonTaggerMAODToken, ifilterbadglobalmu);
    bool  filterbadglobalmu = *ifilterbadglobalmu;
    if(filterbadglobalmu)badglobalmucount++;
    
    
    edm::Handle<bool> ifiltercloneglobalmu;
    iEvent.getByToken(cloneGlobalMuonTaggerMAODToken, ifiltercloneglobalmu);
    bool filtercloneglobalmu = *ifiltercloneglobalmu;
    if(filtercloneglobalmu)cloneglobalmucount++;
    
    bool pass_MuFilters = false;
    if(filterbadChCandidate && filterbadPFMuon && filterbadglobalmu && filtercloneglobalmu)pass_MuFilters= true;
    
    
   //MET Filters 
    
    

    bool pass_Flag_goodVertices = false;
    bool pass_Flag_globalTightHalo2016Filter = false;
    bool pass_Flag_HBHENoiseFilter = false;
    bool pass_Flag_HBHENoiseIsoFilter = false;
    bool pass_Flag_EcalDeadCellTriggerPrimitiveFilter = false;
    bool pass_Flag_eeBadScFilter = false;


    if( filterResults.isValid() ){
     std::vector<std::string> triggerNames = filter_config_.triggerNames();

     for( unsigned int iPath=0; iPath<triggerNames.size(); iPath++ ){
      std::string pathName = triggerNames[iPath];
      unsigned int hltIndex = filter_config_.triggerIndex(pathName);

      if( hltIndex >= filterResults->size() ) continue;

      int accept = filterResults->accept(hltIndex);
      std::string pathNameNoVer = filter_config_.removeVersion(pathName);

      if( accept ) flt_cppath_[pathNameNoVer]+=1;
      
      if(accept){
      	const unsigned long MatchedAtTheBeginning = 0;
	
	if( pathNameNoVer.find("Flag_goodVertices" ,0) == MatchedAtTheBeginning){pass_Flag_goodVertices = true;}
	if( pathNameNoVer.find("Flag_globalTightHalo2016Filter" ,0) == MatchedAtTheBeginning){pass_Flag_globalTightHalo2016Filter = true;}
	if( pathNameNoVer.find("Flag_HBHENoiseFilter" ,0) == MatchedAtTheBeginning){pass_Flag_HBHENoiseFilter = true;}
	if( pathNameNoVer.find("Flag_HBHENoiseIsoFilter" ,0) == MatchedAtTheBeginning){pass_Flag_HBHENoiseIsoFilter = true;}
	if( pathNameNoVer.find("Flag_EcalDeadCellTriggerPrimitiveFilter" ,0) == MatchedAtTheBeginning){pass_Flag_EcalDeadCellTriggerPrimitiveFilter = true;}
	if( pathNameNoVer.find("Flag_eeBadScFilter",0) == MatchedAtTheBeginning){pass_Flag_eeBadScFilter = true;}
	}
      }
    }
  
    bool pass_METFilters = false;
    if(pass_Flag_goodVertices && pass_Flag_globalTightHalo2016Filter && pass_Flag_HBHENoiseFilter && pass_Flag_HBHENoiseIsoFilter && pass_Flag_EcalDeadCellTriggerPrimitiveFilter){
  	pass_METFilters=true;
    } 
    if(realData)pass_METFilters = pass_METFilters && pass_Flag_eeBadScFilter;
   
    // Primary Verticies
 
    int numpv=0; int iPV=0;
    bool firstGoodPV = false;
  
    miniAODhelper.SetRho(*rhoHandle);
    miniAODhelper.SetJER_SF_Tool(iSetup);
  
    reco::Vertex vertex;
    reco::VertexCollection vtxs = *vtxHandle;
    if( vtxHandle.isValid() ){
     for( reco::VertexCollection::const_iterator vtx = vtxs.begin(); vtx!=vtxs.end(); ++vtx ){
      
      iPV++;
      bool isGood = ( !(vtx->isFake()) &&
		      (vtx->ndof() > 4.0) &&
		      (fabs(vtx->z()) < 24.0) &&
		      (fabs(vtx->position().Rho()) < 2.0) 
		      );
		      
      if( !isGood ) continue;

      if( iPV==1 ){
	firstGoodPV = true;
	vertex = (*vtx);
      }

      numpv++;
     }
    }
  
    if( numpv>0 ) miniAODhelper.SetVertex(vertex);
  double numTruePV = -1;
  double numGenPV = -1;
  if( (PupInfo.isValid()) ){
    for( std::vector<PileupSummaryInfo>::const_iterator PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI ) {
      int BX = PVI->getBunchCrossing();
      if( BX==0 ){
	numTruePV = PVI->getTrueNumInteractions();
	numGenPV  = PVI->getPU_NumInteractions();
      }
    }
  }
   
   /////////////////////
    // TRIGGER STUFF  ///
    ////////////////////
  

    bool passHLT_Ele27_eta2p1_WPTight_Gsf_v = false;
  
    bool passHLT_IsoMu24_v = false;
    bool passHLT_IsoTkMu24_v = false;
 
    bool passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v = false;

    bool passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v = false;
    bool passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v = false;
  
    bool passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v = false;
    bool passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v = false;
    bool passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v = false;
    bool passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v = false;

    bool passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v = false ;
    bool passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v = false ;
 
 
    if( triggerResults.isValid() ){
  
    std::vector<std::string> triggerNames = hlt_config_.triggerNames();

    for( unsigned int iPath=0; iPath<triggerNames.size(); iPath++ ){
      std::string pathName = triggerNames[iPath];
      unsigned int hltIndex = hlt_config_.triggerIndex(pathName);

      if( hltIndex >= triggerResults->size() ) continue;

      int accept = triggerResults->accept(hltIndex);

      if( accept ){
      
      
	const unsigned long MatchedAtTheBegining = 0 ; 

	if( pathName.find( "HLT_IsoMu24_v"        ,0) == MatchedAtTheBegining ){ passHLT_IsoMu24_v = true;}
	if( pathName.find( "HLT_IsoTkMu24_v"      ,0) == MatchedAtTheBegining ){ passHLT_IsoTkMu24_v = true;}
	
	
	
	if( pathName.find( "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v" ,0) == MatchedAtTheBegining ){passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v = true;}

	if( pathName.find( "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v"               ,0) == MatchedAtTheBegining ){ passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v = true;}
	if( pathName.find( "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v"             ,0) == MatchedAtTheBegining ){ passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v = true;}
	
	if( pathName.find( "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v",0) == MatchedAtTheBegining ){ passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v = true;}
	if( pathName.find( "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v" ,0) == MatchedAtTheBegining ){ passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v = true;}
	if( pathName.find( "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",0) == MatchedAtTheBegining ){ passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v = true;}
	if( pathName.find( "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v" ,0) == MatchedAtTheBegining ){ passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v = true;}

	if( pathName.find( "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v"            ,0) == MatchedAtTheBegining ){ passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v	        = true ; }
	if( pathName.find( "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"          ,0) == MatchedAtTheBegining ){ passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v            = true ; }
	

	if( pathName.find( "HLT_Ele27_eta2p1_WPTight_Gsf_v"        ,0) == MatchedAtTheBegining ){ passHLT_Ele27_eta2p1_WPTight_Gsf_v = true;}
	
      }
     }
    } //END triggerResults.isValid

    int ElTrig = passHLT_Ele27_eta2p1_WPTight_Gsf_v;
    int MuTrig = ( passHLT_IsoMu24_v == 1 || passHLT_IsoTkMu24_v == 1 ) ? 1 : 0 ; 
    
    
    // Dilep Trig
    int ElMuTrig = ( passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v ==1 
		     ||
		     passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v == 1 
		     ||
		     passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v ==1 
		     ||
		     passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v == 1 
		     ) ? 1 : 0 ; 
    int MuMuTrig = ( passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v
		     ||
		     passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v 
		     ||
		     passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v
		     ||
		     passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v 
		     ) ? 1 : 0 ;
    int ElElTrig = passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v;
    
  /////Additional Event Information////
  
  int additionalJetEventId = -10;
  if(!realData)additionalJetEventId  = *genTtbarId;
   
  
  
    /////////////////	     
    //// LEPTONS ////
    ////////////////

    std::vector<double> lepton_pt;  
    std::vector<double> lepton_eta;
    std::vector<double> lepton_scEta;
    std::vector<double> lepton_phi;
    std::vector<double> lepton_e;
    std::vector<int>    lepton_trkCharge;
    std::vector<int>    lepton_isMuon;
    std::vector<double> lepton_relIso;
    std::vector<int>    lepton_isTight;
    std::vector<int>    lepton_isLoose;
    std::vector<uint32_t> lepton_seed;
		 
    for( std::vector<pat::Muon>::const_iterator iMu = muons->begin(); iMu != muons->end(); iMu++ ){ 

        TRandom3 rng;
        int32_t seed = iMu->userInt("deterministicSeed");
        rng.SetSeed((uint32_t)seed);
   
    	int trkCharge = -99;
    	if( iMu->muonBestTrack().isAvailable() ) trkCharge = iMu->muonBestTrack()->charge();

    	int isPOGTight = miniAODhelper.passesMuonPOGIdTight(*iMu) ? 1 : 0;
    	int isPOGLoose = 1 ; //this is needed for the consistency of variables with electron.



    // our pre-selections 
    	if( iMu->pt() < 15 ){ continue;}
    	if( fabs( iMu->eta() ) > 2.4 ){ continue;}

    
    	lepton_pt.push_back(iMu->pt());
    	lepton_eta.push_back(iMu->eta());
    	lepton_scEta.push_back(-99);
    	lepton_phi.push_back(iMu->phi());
    	lepton_e.push_back(iMu->energy());
    	lepton_trkCharge.push_back(trkCharge);
    	lepton_isMuon.push_back(1);
    	lepton_relIso.push_back( miniAODhelper.GetMuonRelIso(*iMu, coneSize::R04, corrType::deltaBeta) ) ;
    	lepton_isLoose.push_back(isPOGLoose);
    	lepton_isTight.push_back(isPOGTight);
    	lepton_seed.push_back(seed);
    
     }
  
     for( std::vector<pat::Electron>::const_iterator iEle = electrons->begin(); iEle != electrons->end(); iEle++ ){ 

        TRandom3 rng;
        int32_t seed = iEle->userInt("deterministicSeed");
        rng.SetSeed((uint32_t)seed);

     	int trkCharge = -99;
    	if( iEle->gsfTrack().isAvailable() ) trkCharge = iEle->gsfTrack()->charge();

    	int isPOGTight = miniAODhelper.PassElectron80XId(*iEle ,electronID::electron80XCutBasedM ) ? 1 : 0  ;
    	int isPOGLoose = miniAODhelper.PassElectron80XId(*iEle ,electronID::electron80XCutBasedL ) ? 1 : 0  ;


    // our pre-selections 
    	if( iEle->pt() < 15 ){ continue;}
    	if( fabs( iEle->eta() ) > 2.4 ){ continue;}
    //

    	bool inCrack = false;
    	double scEta = -99;
    	if( iEle->superCluster().isAvailable() ){
     	 inCrack = ( fabs(iEle->superCluster()->position().eta())>=1.4442 && fabs(iEle->superCluster()->position().eta())<=1.5660 );
      	 scEta = iEle->superCluster()->position().eta();
    	}
    
    	if(inCrack)isPOGTight=0;
    
    	lepton_pt.push_back(iEle->pt());
    	lepton_eta.push_back(iEle->eta());
    	lepton_scEta.push_back(scEta);
    	lepton_phi.push_back(iEle->phi());
   	lepton_e.push_back(iEle->energy());
   	lepton_trkCharge.push_back(trkCharge);
   	lepton_isMuon.push_back(0);
   	lepton_relIso.push_back(miniAODhelper.GetElectronRelIso(*iEle, coneSize::R03, corrType::rhoEA,effAreaType::spring16) );
   	lepton_isLoose.push_back(isPOGLoose);
    	lepton_isTight.push_back(isPOGTight);
        lepton_seed.push_back(seed);

     }
     
    ///////////////// 
    //// Jets ////
    /////////////////
  
  const std::vector<pat::Jet> pfJets_ID = miniAODhelper.GetSelectedJets(*pfjets, 0., 9999., jetID::jetLoose, '-' );
  
  // Jet Corrections //
  
  const JetCorrector* corrector = JetCorrector::getJetCorrector( "ak4PFchsL1L2L3", iSetup );
  miniAODhelper.SetJetCorrector(corrector); 
  
  
  std::vector<pat::Jet> rawJets = miniAODhelper.GetUncorrectedJets(pfJets_ID);
  std::vector<pat::Jet> correctedJets_noSys = miniAODhelper.GetCorrectedJets(rawJets, iEvent, iSetup, genjets,Systematics::NA,true,true);
  
  std::vector<pat::Jet> selectedJets_noSys_unsorted = miniAODhelper.GetSelectedJets(correctedJets_noSys, 20., 5.0, jetID::none, '-' );
  std::vector<pat::Jet> selectedJets_noSys_sorted = miniAODhelper.GetSortedByPt(selectedJets_noSys_unsorted);
   
     std::vector<double> jet_pt;
     std::vector<double> jet_eta;
     std::vector<double> jet_phi;
     std::vector<double> jet_m;
     std::vector<double> jet_csvV;
     std::vector<int> jet_flavour;
     std::vector<int> jet_PUID_passWPLooseV;
   
		  // Loop over selected jets
     for( std::vector<pat::Jet>::const_iterator iJet = selectedJets_noSys_sorted.begin(); iJet != selectedJets_noSys_sorted.end(); iJet++ ){ 

      jet_pt  .push_back( iJet -> pt()  );
      jet_phi .push_back( iJet -> phi() );
      jet_eta .push_back( iJet -> eta() );
      jet_m   .push_back( iJet -> mass()   );
      
      
      double myCSV = iJet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
      jet_csvV.push_back(myCSV);
      jet_flavour.push_back(iJet->hadronFlavour());
      
      
      
       //PU jet ID
      if(!iJet->hasUserFloat("pileupJetId:fullDiscriminant") || !iJet->hasUserInt("pileupJetId:fullId")) {
 	std::cout << "no mvaValue or idflag info for the jet ---" << std::endl;
       }
 
      
       int    idflag = iJet->userInt("pileupJetId:fullId");
       bool passPUIDLoose = PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kLoose ); 
 
      if(passPUIDLoose)jet_PUID_passWPLooseV.push_back(99);
      else jet_PUID_passWPLooseV.push_back(88);


     }// end loop over iJet    


    pat::MET correctedMET = pfmet->at(0);
    Float_t MET_pt = correctedMET.pt();
    Float_t MET_phi = correctedMET.phi();
    
    
    
    
    
    /////////////////////
    //// SELECTION //////
    /////////////////////

    selection . SetElTrigger( & ElTrig);
    selection . SetMuTrigger( & MuTrig );
    selection . SetElElTrigger( & ( ElElTrig  ) );
    selection . SetElMuTrigger( & ElMuTrig );
    selection . SetMuMuTrigger( & MuMuTrig );

    selection . SetGoodVtx( & ( firstGoodPV ) );

    selection . SetLeptons( & lepton_pt, 
			    & lepton_eta, 
			    & lepton_scEta, 
			    & lepton_phi,
			    & lepton_e,
			    & lepton_trkCharge, 
			    & lepton_isMuon, 
			    & lepton_relIso,
			    & lepton_isLoose,
			    & lepton_isTight,
			    & lepton_seed );
    selection . SetJets( & jet_pt , 
			 & jet_eta , 
			 & jet_phi, 
			 & jet_m , 
			 & jet_csvV ,
			 & jet_flavour  );

    selection . SetMet( & ( MET_pt ) , &( MET_phi) );

  
  
  selection.doEventSelection();
  std::cout.setf(std::ios::fixed);
  
  //////////////
  //SYNC JUNK//
  //////////////
  
  
  bool selected = true;
  if(!(selection . PassSingleElCh()) && !(selection.PassSingleMuCh()) && !(selection.PassElEl()) && !(selection.PassElMu()) && !(selection.PassMuMu()))selected=false;
  if(selected && pass_METFilters && pass_MuFilters && DoSync){

    std::ofstream csvfile (SyncType+".csv",std::ofstream::app);

    csvfile << run<< "," ;
    csvfile <<lumi << "," ;
    csvfile <<evt << "," ;
    
    //      is_e, is_mu, is_ee, is_emu, is_mumu,
    csvfile <<(selection . PassSingleElCh() ?1:0) << "," ;
    csvfile <<(selection . PassSingleMuCh() ?1:0) << "," ;
    csvfile <<(selection . PassElEl()       ?1:0) << "," ;
    csvfile <<(selection . PassElMu()       ?1:0) << "," ;
    csvfile <<(selection . PassMuMu()       ?1:0) << "," ;
    

	if(selection.PassElEl() || selection.PassElMu() || selection.PassMuMu()){
		csvfile << selection.DLSofterjets().size()<<",";
		csvfile << selection.DLSofterbjets().size()<<",";
	}
	else{
		if(selection.jets().size()>0)    csvfile << selection.jets().size() << "," ;
			else csvfile <<"0,";
		if(selection.bjets().size()>0)    csvfile << selection.bjets().size() << ",";
			else csvfile <<"0,";
	}

    double lepIDSF =  ( realData ? 1 : 
			scalefactors.getTightMuon_IDSF( & selection )
			* 
			scalefactors.getTightElectron_IDSF( & selection )
	);
    double lepISOSF =  ( realData ? 1 : 
			 scalefactors.getTightElectron_RecoSF( & selection ) 
			*
			 scalefactors.getTightMuon_IsoSF( & selection ) 
	);

    if( selection.looseLeptons().size() >=1 ){
      long pdgid = 
	( selection.looseLeptonsIsMuon().at(0) == 1 ? 13 : 11 )
	*
	( selection.looseLeptonsCharge().at(0) > 0 ? -1 : +1 ) ;
      csvfile << std::setprecision(4) << std::fixed
              << selection.looseLeptons().at(0)->Pt()<< "," 
              << selection.looseLeptons().at(0)->Eta()<< ",";
      if(SyncDebug){
        csvfile <<std::setprecision(4)<<std::fixed<<selection.looseLeptons().at(0)->Phi()<<",";
        int cracker=-808;
        for(int i=0;i<int(selection.looseLeptons().size());i++){
      	  if(( fabs(selection.looseLeptonsScEta().at(i))>1.4442 && fabs(selection.looseLeptonsScEta().at(i))<1.5660 ))cracker=888;
      	}
        csvfile <<cracker<<",";	
      }
      csvfile << std::setprecision(4) << std::fixed 
              << selection.looseLeptonsRelIso().at(0)<< ","
              << pdgid << ","
              << lepIDSF << "," 
              << lepISOSF << ","
              << selection.looseLeptonsSeed().at(0) << "," ;
    }else{
      csvfile << "-1,-1,-1,-1,-1,-1,-1," ;
      if(SyncDebug)cout<<"-1,-1";
    }

    if( selection.looseLeptons().size() >=2 ){
      long pdgid = 
	( selection.looseLeptonsIsMuon().at(1) == 1 ? 13 : 11 )
	*
	( selection.looseLeptonsCharge().at(1) > 0 ? -1 : +1 ) ;
      csvfile << std::setprecision(4) << std::fixed
              << selection.looseLeptons().at(1)->Pt()<< "," 
              << selection.looseLeptons().at(1)->Eta()<< "," 
              << selection.looseLeptonsRelIso().at(1)<< ","
              << pdgid << ","
              << lepIDSF << "," 
              << lepISOSF << ","
              << selection.looseLeptonsSeed().at(1) << "," ;
    }else{
      csvfile << "-1,-1,-1,-1,-1,-1,-1," ;
    }
    
    {
   // bool dbg=true;
      bool nJet_ge_one = selection.jets().size() >=1;
      bool nJet_ge_two = selection.jets().size() >=2;
      bool nJet_ge_three = selection.jets().size() >=3;
      bool nJet_ge_four = selection.jets().size() >=4;
    
      double jet1_jesSF = -1;
      double jet1_jesSF_up = -1;
      double jet1_jesSF_down = -1;
      double jet1_jesSF_PileUpDataMC_down = -1;
      double jet1_jesSF_RelativeFSR_up = -1;
      double jet1_jerSF_nominal = -1;
      double jet1_PUJetID = -1;
      double jet1_PUJetIDDiscriminant = -1;
      uint32_t jet1_seed = 999999999;
      double jet2_jesSF = -1;
      double jet2_jesSF_up = -1;
      double jet2_jesSF_down = -1;
      double jet2_jesSF_PileUpDataMC_down = -1;
      double jet2_jesSF_RelativeFSR_up = -1;
      double jet2_jerSF_nominal = -1;
      double jet2_PUJetID = -1;
      double jet2_PUJetIDDiscriminant = -1;
      uint32_t jet2_seed = 999999999;

      const bool  doJES = true;
      const bool  doJER = false;

      std::vector<pat::Jet> jet_JESNOMI =  miniAODhelper.GetCorrectedJets(rawJets, iEvent, iSetup, genjets ,Systematics::NA     , doJES, doJER );
      std::vector<pat::Jet> jet_JESUP   =  miniAODhelper.GetCorrectedJets(rawJets, iEvent, iSetup, genjets ,Systematics::JESup  , doJES, doJER );
      std::vector<pat::Jet> jet_JESDOWN =  miniAODhelper.GetCorrectedJets(rawJets, iEvent, iSetup, genjets ,Systematics::JESdown, doJES, doJER );
      std::vector<pat::Jet> jet_JESPileUpDataMCDOWN   =  miniAODhelper.GetCorrectedJets(rawJets, iEvent, iSetup, genjets ,Systematics::JESPileUpDataMCdown  , doJES, doJER );
      std::vector<pat::Jet> jet_JESRelativeFSRUP =  miniAODhelper.GetCorrectedJets(rawJets, iEvent, iSetup, genjets ,Systematics::JESRelativeFSRup, doJES, doJER );

      std::vector<pat::Jet> jet_JERNOMI =  miniAODhelper.GetCorrectedJets(rawJets, iEvent, iSetup, genjets ,Systematics::NA, false, true );
      

	const double eta1 = selection.jets().at(0)->Eta();
	const double phi1 = selection.jets().at(0)->Phi();
	const double eta2 = selection.jets().at(1)->Eta();
	const double phi2 = selection.jets().at(1)->Phi();
	
	//std::cout<<" RAW JETS["<<rawJets.size()<<"] ,";
	

	// Loop for JEC_nominal
	for( unsigned int idxJet = 0 ; idxJet < rawJets.size(); idxJet ++ ){
	
	//cout<<idxJet<<" ";

	  pat::Jet * iRawJet = & rawJets.at( idxJet );
	  double d_eta1 =       eta1 -  iRawJet->eta();
	  double d_phi1 = fabs( phi1 -  iRawJet->phi() ) ; 
	  d_phi1 = ( d_phi1 < M_PI ) ? d_phi1 : 2 * M_PI - d_phi1 ; 

	  if(  d_eta1*d_eta1 + d_phi1*d_phi1 < 0.01 * 0.01 ){
	  
	 // cout<<" this ";
	   // matching btw Raw and Corrected (physics) jet.
	    jet1_jesSF     = jet_JESNOMI.at( idxJet ).pt() / iRawJet->pt();
	    jet1_jesSF_up   = jet_JESUP  .at( idxJet ).pt() / jet_JESNOMI.at( idxJet ).pt();
	    jet1_jesSF_down = jet_JESDOWN.at( idxJet ).pt() / jet_JESNOMI.at( idxJet ).pt();
            jet1_jesSF_PileUpDataMC_down = jet_JESPileUpDataMCDOWN.at( idxJet ).pt() / jet_JESNOMI.at( idxJet ).pt();
            jet1_jesSF_RelativeFSR_up = jet_JESRelativeFSRUP.at( idxJet ).pt() / jet_JESNOMI.at( idxJet ).pt();
            jet1_jerSF_nominal = jet_JERNOMI.at( idxJet ).pt() / iRawJet->pt();
            jet1_PUJetID = jet_JESNOMI.at( idxJet ).userInt("pileupJetIdUpdated:fullId");
            jet1_PUJetIDDiscriminant = jet_JESNOMI.at( idxJet ).userFloat("pileupJetIdUpdated:fullDiscriminant");
            jet1_seed = jet_JESNOMI.at( idxJet ).userInt("deterministicSeed");
	    
	  }
	  
	  double d_eta2 =       eta2 -  iRawJet->eta();
	  double d_phi2 = fabs( phi2 -  iRawJet->phi() ) ; 
	  d_phi2 = ( d_phi2 < M_PI ) ? d_phi2 : 2 * M_PI - d_phi2 ; 

	  if(  d_eta2*d_eta2 + d_phi2*d_phi2 < 0.01 * 0.01 ){
	  
	 // cout<<" this ";
	   // matching btw Raw and Corrected (physics) jet.
	    jet2_jesSF     = jet_JESNOMI.at( idxJet ).pt() / iRawJet->pt();
	    jet2_jesSF_up   = jet_JESUP  .at( idxJet ).pt() / jet_JESNOMI.at( idxJet ).pt();
	    jet2_jesSF_down = jet_JESDOWN.at( idxJet ).pt() / jet_JESNOMI.at( idxJet ).pt();
            jet2_jesSF_PileUpDataMC_down = jet_JESPileUpDataMCDOWN.at( idxJet ).pt() / jet_JESNOMI.at( idxJet ).pt();
            jet2_jesSF_RelativeFSR_up = jet_JESRelativeFSRUP.at( idxJet ).pt() / jet_JESNOMI.at( idxJet ).pt();
            jet2_jerSF_nominal = jet_JERNOMI.at( idxJet ).pt() / iRawJet->pt();
            jet2_PUJetID = jet_JESNOMI.at( idxJet ).userInt("pileupJetIdUpdated:fullId");
            jet2_PUJetIDDiscriminant = jet_JESNOMI.at( idxJet ).userFloat("pileupJetIdUpdated:fullDiscriminant");
            jet2_seed = jet_JESNOMI.at( idxJet ).userInt("deterministicSeed");
	    
	  }
	}

      
    
    
      csvfile << std::setprecision(4) << std::fixed
              << ( nJet_ge_one ? selection.jets().at(0)->Pt() : -1 )<< "," 
              << ( nJet_ge_one ? selection.jets().at(0)->Eta() : -1)<< "," 
              << ( nJet_ge_one ? selection.jets().at(0)->Phi() : -1)<< "," 
              << ( nJet_ge_one ? jet1_jesSF : -1)<< ","
              << ( nJet_ge_one ? jet1_jesSF_up : -1)<< ","
              << ( nJet_ge_one ? jet1_jesSF_down : -1)<< ","
              << ( nJet_ge_one ? jet1_jesSF_PileUpDataMC_down : -1)<< ","
              << ( nJet_ge_one ? jet1_jesSF_RelativeFSR_up : -1)<< ","
              << ( nJet_ge_one ? jet1_jerSF_nominal : -1)<< ","
              << ( nJet_ge_one ? selection.jetsBdiscriminant().at(0) : -1 )<< "," 
              << ( nJet_ge_one ? jet1_PUJetID : -1)<< ","
              << ( nJet_ge_one ? jet1_PUJetIDDiscriminant : -1)<< ","
              << ( nJet_ge_one ? jet1_seed : -1)<< ","


              << ( nJet_ge_two ? selection.jets().at(1)->Pt() : -1 )<< "," 
              << ( nJet_ge_two ? selection.jets().at(1)->Eta() : -1)<< "," 
              << ( nJet_ge_two ? selection.jets().at(1)->Phi() : -1)<< "," 
              << ( nJet_ge_two ? jet2_jesSF : -1)<< ","
              << ( nJet_ge_two ? jet2_jesSF_up : -1)<< ","
              << ( nJet_ge_two ? jet2_jesSF_down : -1)<< ","
              << ( nJet_ge_one ? jet2_jesSF_PileUpDataMC_down : -1)<< ","
              << ( nJet_ge_one ? jet2_jesSF_RelativeFSR_up : -1)<< ","
              << ( nJet_ge_one ? jet2_jerSF_nominal : -1)<< ","
              << ( nJet_ge_two ? selection.jetsBdiscriminant().at(1) : -1 )<< "," 
              << ( nJet_ge_one ? jet2_PUJetID : -1)<< ","
              << ( nJet_ge_one ? jet2_PUJetIDDiscriminant : -1)<< ","
              << ( nJet_ge_one ? jet2_seed : -1)<< ",";


      if(SyncDebug){
      
        csvfile << std::setprecision(4) << std::fixed
                << ( nJet_ge_three ? selection.jets().at(2)->Pt() : -1 )<< "," 
                << ( nJet_ge_three ? selection.jets().at(2)->Eta() : -1)<< "," 
        				<< ( nJet_ge_three ? selection.jets().at(2)->Phi() : -1)<< "," 
        				<< ( nJet_ge_three ? selection.jetsBdiscriminant().at(2) : -1 )<< "," 

        				<< ( nJet_ge_four ? selection.jets().at(3)->Pt() : -1 )<< "," 
        				<< ( nJet_ge_four ? selection.jets().at(3)->Eta() : -1)<< "," 
        				<< ( nJet_ge_four ? selection.jets().at(3)->Phi() : -1)<< "," 
        				<< ( nJet_ge_four ? selection.jetsBdiscriminant().at(3) : -1 )<< "," ;

        int closeone = -2;
        for(int i=0;i<int(selectedJets_noSys_sorted.size());i++){
        pat::Jet * iJet = & selectedJets_noSys_sorted.at(i);
        if(fabs(iJet->pt()-30.0)<1.5)closeone=87;
        }
        csvfile <<std::setprecision(4)<<std::fixed<<closeone<<",";
        }
      
      
     
      
      
    }
    csvfile << std::setprecision(4) << std::fixed
            << MET_pt << ","
            << MET_phi<< "," ;
    
    
    //MASS OF TOO LLs
    csvfile << selection.DLinvMass << ",";

    if( !realData ){
    csvfile << additionalJetEventId << ","; //ttHFCategory
    csvfile << -1 << ","; //ttHFGenFilterTag
    csvfile << numTruePV << ",";
    csvfile << scalefactors.get_pu_wgt(numTruePV) << "," ;    // PUWeight,
    }else{
    csvfile << 1 << ",";
    csvfile << 1 << ",";
    csvfile << 1 << ",";
    csvfile << 1 << ",";    // PUWeight,
    }

    double csvSF = 1 ;
    double csvSF_lf_up = 1;
    double csvSF_hf_down = 1;
    double csvSF_cErr1_down = 1;
    
    if( !realData ){
      int nom_sys = 0 ;
      int lf_up_sys = 9 ;
      int hf_down_sys = 12 ;
      int cErr1_down_sys = 22;  
      double dummy = - 1 ;
      csvSF = scalefactors.get_csv_wgt( & selection , nom_sys,  dummy , dummy , dummy );
      csvSF_lf_up = scalefactors.get_csv_wgt( & selection , lf_up_sys,  dummy , dummy , dummy );
      csvSF_hf_down = scalefactors.get_csv_wgt( & selection , hf_down_sys,  dummy , dummy , dummy );
      csvSF_cErr1_down = scalefactors.get_csv_wgt( & selection , cErr1_down_sys,  dummy , dummy , dummy );
    }
    csvfile << csvSF <<","
            << csvSF_lf_up <<","
            << csvSF_hf_down <<","
            << csvSF_cErr1_down <<",";
    

    //pdf weight
    csvfile <<1<<",";
    csvfile <<1<<",";
    
    //mem weight
    csvfile << 1 <<",";
    csvfile << 1 <<",";

    double triggerSF =( realData ? 1 :
			scalefactors.get_TrigMuSF( & selection )
			*
			scalefactors.get_TrigElSF( & selection )
			) ;
    csvfile << triggerSF <<",";

    //top PT weight 
    if( !realData ){
      csvfile << eve->weight_topPt_ <<",";
    }else{
      csvfile << 1 <<",";
    }

    //Discriminators
    csvfile << std::setprecision(4) << std::fixed
            << -1 << "," 
            << std::setprecision(4) << -1 << "," 
            << std::setprecision(4) << -1;

    if( false ){
//    csvfile << eve->weight_q2_upup_ <<",";
//    csvfile << eve->weight_q2_downdown_ <<",";
    
//    csvfile << eve-> weight_PDF_NNPDF30NLO_up_ <<",";
//    csvfile << eve-> weight_PDF_NNPDF30NLO_down_<< ",";
    }else{
//    csvfile << 1 <<",";
//    csvfile << 1 <<",";
//    csvfile << 1 <<",";
//    csvfile << 1 <<",";
    }
    

    if(SyncDebug){
  
     //// Outputs Jet Debug Information from "selected_jets" Jet_Pt ,selected_jets_step , jet_csv, dR with lepton, jet_PUID_pass
  
      bool nj1 = jet_pt.size() >=1;
      bool nj2 = jet_pt.size() >=2;
      bool nj3 = jet_pt.size() >=3;
      bool nj4 = jet_pt.size() >=4;
      bool nj5 = jet_pt.size() >=5;
      bool nj6 = jet_pt.size() >=6;
      
      TLorentzVector Telectron;
      Telectron.SetPtEtaPhiE(selection.looseLeptons().at(0)->Pt(),selection.looseLeptons().at(0)->Eta(),selection.looseLeptons().at(0)->Phi(),0);
      TLorentzVector Tjet1;
      TLorentzVector Tjet2;
      TLorentzVector Tjet3;
      TLorentzVector Tjet4;
      TLorentzVector Tjet5;
      TLorentzVector Tjet6;
      double dR1,dR2,dR3,dR4,dR5,dR6;
      if(nj1){
      	Tjet1.SetPtEtaPhiE(jet_pt[0],jet_eta[0],jet_phi[0],0);
	dR1=Tjet1.DeltaR(Telectron);
      }
      if(nj2)Tjet2.SetPtEtaPhiE(jet_pt[1],jet_eta[1],jet_phi[1],0);
      if(nj3)Tjet3.SetPtEtaPhiE(jet_pt[2],jet_eta[2],jet_phi[2],0);
      if(nj4)Tjet4.SetPtEtaPhiE(jet_pt[3],jet_eta[3],jet_phi[3],0);
      if(nj5)Tjet5.SetPtEtaPhiE(jet_pt[4],jet_eta[4],jet_phi[4],0);
      if(nj6)Tjet6.SetPtEtaPhiE(jet_pt[5],jet_eta[5],jet_phi[5],0);
      
      if(nj2)dR2=Tjet2.DeltaR(Telectron);
      if(nj3)dR3=Tjet3.DeltaR(Telectron);
      if(nj4)dR4=Tjet4.DeltaR(Telectron);
      if(nj5)dR5=Tjet5.DeltaR(Telectron);
      if(nj6)dR6=Tjet6.DeltaR(Telectron);
      
      
      csvfile << std::setprecision(4) << "," <<( nj1? jet_pt[0] : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj1 ? selection.selected_jets_step[0] : -1)<< "," ;
      csvfile << std::setprecision(4) << ( nj1 ? jet_csvV[0] : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj1 ? dR1 : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj1 ? jet_PUID_passWPLooseV[0] : -1)<<","; 
      csvfile << std::setprecision(4) << ( nj2 ? jet_pt[1] : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj2 ? selection.selected_jets_step[1] : -1)<< "," ;
      csvfile << std::setprecision(4) << ( nj2 ? jet_csvV[1] : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj1 ? dR2 : -1 )<< "," ;
       csvfile << std::setprecision(4) << ( nj1 ? jet_PUID_passWPLooseV[1] : -1)<<","; 
      csvfile << std::setprecision(4) << ( nj3 ? jet_pt[2] : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj3 ? selection.selected_jets_step[2] : -1)<< "," ;
      csvfile << std::setprecision(4) << ( nj3 ? jet_csvV[2] : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj1 ? dR3 : -1 )<< "," ;
       csvfile << std::setprecision(4) << ( nj1 ? jet_PUID_passWPLooseV[2] : -1)<<","; 
      csvfile << std::setprecision(4) << ( nj4 ? jet_pt[3] : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj4 ? selection.selected_jets_step[3] : -1)<< "," ;
      csvfile << std::setprecision(4) << ( nj4 ? jet_csvV[3] : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj1 ? dR4 : -1 )<< "," ;
       csvfile << std::setprecision(4) << ( nj1 ? jet_PUID_passWPLooseV[3] : -1)<<","; 
      csvfile << std::setprecision(4) << ( nj5 ? jet_pt[4] : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj5 ? selection.selected_jets_step[4] : -1)<< "," ;
      csvfile << std::setprecision(4) << ( nj5 ? jet_csvV[4] : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj1 ? dR5 : -1 )<< "," ;
       csvfile << std::setprecision(4) << ( nj1 ? jet_PUID_passWPLooseV[4] : -1)<<","; 
      csvfile << std::setprecision(4) << ( nj6 ? jet_pt[5] : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj6 ? selection.selected_jets_step[5] : -1)<< "," ;
      csvfile << std::setprecision(4) << ( nj6 ? jet_csvV[5] : -1 )<< "," ;
      csvfile << std::setprecision(4) << ( nj1 ? dR6 : -1 )<< "," ;
       csvfile << std::setprecision(4) << ( nj1 ? jet_PUID_passWPLooseV[5] : -1)<<","; 
      
    
    }
    
    csvfile << std::endl ;

    csvfile.close();
  }//if selected (Sync Stuff)
  
  
  
  ////////////////////
  //Systematics Loop//
  ////////////////////
  
  //I'm doing this here because it makes no sense to loop over the systematics if we aren't even selecting the event.
  //This will save loads of time and space. 
  //If you don't like this format feel free to move it back to the //JETS// section
  
     std::vector<Systematics::Type> systematics;
     std::vector<string> systematics_list;
     systematics_list.push_back("JESup");
     systematics_list.push_back("JESdown");
     systematics_list.push_back("JERup");
     systematics_list.push_back("JERdown");
     
    
  
     for(int i=0;i<int(systematics_list.size());i++)systematics.push_back(Systematics::get(systematics_list[i]));

     
  
     std::vector<std::vector<double>> vjet_pt_sys;
     std::vector<std::vector<double>> vjet_eta_sys;
     std::vector<std::vector<double>> vjet_phi_sys;
     std::vector<std::vector<double>> vjet_m_sys;
     std::vector<std::vector<double>> vjet_csvV_sys;
     std::vector<std::vector<int>>    vjet_flavour_sys;
     std::vector<std::vector<int>>    vjet_PUID_passWPLooseV_sys;
     
     
     if(doSystematics){
     for(int iSys=0; iSys<int(systematics.size());iSys++){
     
  
  
     	std::vector<pat::Jet> correctedJets_Sys = miniAODhelper.GetCorrectedJets(rawJets, iEvent, iSetup, genjets,systematics[iSys],true,true);
  
     	std::vector<pat::Jet> selectedJets_Sys_unsorted = miniAODhelper.GetSelectedJets(correctedJets_Sys, 20., 5.0, jetID::none, '-' );
     	std::vector<pat::Jet> selectedJets_Sys_sorted = miniAODhelper.GetSortedByPt(selectedJets_Sys_unsorted);
   
    	std::vector<double> jet_pt_sys;
     	std::vector<double> jet_eta_sys;
     	std::vector<double> jet_phi_sys;
     	std::vector<double> jet_m_sys;
     	std::vector<double> jet_csvV_sys;
     	std::vector<int> jet_flavour_sys;
     	std::vector<int> jet_PUID_passWPLooseV_sys;
   
		  // Loop over selected jets
     	for( std::vector<pat::Jet>::const_iterator iJet = selectedJets_Sys_sorted.begin(); iJet != selectedJets_Sys_sorted.end(); iJet++ ){ 

      		jet_pt_sys  .push_back( iJet -> pt()  );
      		jet_phi_sys .push_back( iJet -> phi() );
      		jet_eta_sys .push_back( iJet -> eta() );
      		jet_m_sys   .push_back( iJet -> mass()   );
      	
      		double myCSV = iJet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
      		jet_csvV.push_back(myCSV);
      		jet_flavour.push_back(iJet->hadronFlavour());
      
      		int    idflag = iJet->userInt("pileupJetId:fullId");
        	bool passPUIDLoose = PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kLoose ); 
 		if(passPUIDLoose)jet_PUID_passWPLooseV.push_back(99);
        	else jet_PUID_passWPLooseV.push_back(88);
	
	}// end loop over iJet    

/*
    	pat::MET correctedMET = pfmet->at(0);
    	Float_t MET_pt = correctedMET.pt();
    	Float_t MET_phi = correctedMET.phi();
	*/
	vjet_pt_sys.push_back(jet_pt_sys);
	vjet_eta_sys.push_back(jet_eta_sys);
	vjet_phi_sys.push_back(jet_phi_sys);
	vjet_m_sys.push_back(jet_m_sys);
	vjet_csvV_sys.push_back(jet_csvV_sys);
	vjet_flavour_sys.push_back(jet_flavour_sys);
	//vjet_PUID_passWPLooseV_sys(jet_PUID_passWPLooseV_sys);
	
      }// End Systematics Loop
      }
    
    
  
  
  // END SYSTEMATICS
  
  
  ///////////////////////////////////////////
  /////// Fill All Tree Variables Here /////
  //////////////////////////////////////////
  //I'm doing this here because it declutters the code//
  //It's for your own good, I promise//         //To Do: Write this into a seperate .cc plugin :) 
  
  bool FillNtuples = true;
  if(saveOnlySelected && !selected)FillNtuples = false;
  
  if(FillNtuples){ 
  
    eve->run_ = run;
    eve->lumi_ = lumi;
    eve->evt_ = evt;

    eve->numTruePV_ = numTruePV;
    eve->numGenPV_ = numGenPV;

    eve->numPVs_ = numpv;
    eve->numSys_ = rNumSys;
  
    eve->GoodFirstPV_ = firstGoodPV;
    eve->pass_METFilters_ = pass_METFilters;
    eve->pass_MuFilters_ = pass_MuFilters;
  
  
  
  
  /////////////////////
  //TRIGGER VARIABLES//
  /////////////////////
   
    eve->passHLT_IsoMu24_v_ =  ( passHLT_IsoMu24_v) ? 1 : 0;
    eve->passHLT_IsoTkMu24_v_ =  ( passHLT_IsoTkMu24_v) ? 1 : 0;
   
    eve->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v_ = ( passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v ) ? 1 : 0;
    eve->passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v_ = ( passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v ) ? 1 : 0;

    eve->passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v_ = ( passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v ) ? 1 : 0;
    eve->passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v_ = ( passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v ) ? 1 : 0;
    eve->passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_ = ( passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v ) ? 1 : 0;
    eve->passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_ = ( passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v ) ? 1 : 0;
  
    eve->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v_		    = ( passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v	      ) ? 1 : 0 ;
    eve->passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v_           = ( passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v           ) ? 1 : 0 ;

    eve->passHLT_Ele27_eta2p1_WPTight_Gsf_v_ =  ( passHLT_Ele27_eta2p1_WPTight_Gsf_v) ? 1 : 0;
    eve->passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_ =  ( passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v ) ? 1 : 0;
  
  
    eve->additionalJetEventId_ = additionalJetEventId;
  
  
  ////////////////////
  //Lepton Variables//
  ////////////////////
  
    eve->lepton_pt_               = lepton_pt;
    eve->lepton_eta_              = lepton_eta;
    eve->lepton_scEta_            = lepton_scEta;
    eve->lepton_phi_              = lepton_phi;
    eve->lepton_e_                = lepton_e;
    eve->lepton_charge_           = lepton_trkCharge;
    eve->lepton_isMuon_           = lepton_isMuon;
    eve->lepton_relIso_           = lepton_relIso;
    eve->lepton_isTight_          = lepton_isTight;
    eve->lepton_isLoose_          = lepton_isLoose;
    eve->lepton_seed_             = lepton_seed;
  
 
  //These Variables contain systematics so we need to loop
  
  
 
  /////////////////
  //Jet Variables//
  /////////////////
  
    eve->jet_pt_[0]	= jet_pt;
    eve->jet_eta_[0]      = jet_eta;
    eve->jet_phi_[0]	= jet_phi;
    eve->jet_m_[0]	= jet_m;
    eve->jet_combinedInclusiveSecondaryVertexV2BJetTags_[0] = jet_csvV;
    eve->jet_flavour_[0]  = jet_flavour;
  //eve->jet_sysLabel_[0] = "NA";
  /**
  eve->MET_[0] = MET_pt;
  eve->MET_[0] = MET_phi;
  */
    if(doSystematics){
  	for(int i=0;i<int(systematics.size());i++){
		eve->jet_pt_[i+1]	= vjet_pt_sys[i];
  		eve->jet_eta_[i+1]      = vjet_eta_sys[i];
  		eve->jet_phi_[i+1]	= vjet_phi_sys[i];
  		eve->jet_m_[i+1]	= vjet_m_sys[i];
  		eve->jet_combinedInclusiveSecondaryVertexV2BJetTags_[i+1] = vjet_csvV_sys[i];
  		eve->jet_flavour_[i+1]  = vjet_flavour_sys[i];
  		//eve->jet_sysLabel_[i+1] = systematics_list[i];
	}
    }
  
  
    worldTree->Fill();
  
  };//End FillNtuples
  
  
  
  
  
  
  }//end skip events
  
  
  
  
} //End_Analyze



void 
YggdrasilTreeMaker2017::beginJob()
{
  
   numEvents_ = 0; 
   
   badChcount = 0;
   badpfmucount=0;
   badglobalmucount=0;
   cloneglobalmucount=0;

   std::ofstream csvfile (SyncType+".csv",std::ofstream::trunc);
 
  if(DoSync && !SyncDebug)
    csvfile  << "run,lumi,event,is_e,is_mu,is_ee,is_emu,is_mumu,n_jets,n_btags,"
             << "lep1_pt,lep1_eta,lep1_iso,lep1_pdgId,lep1_idSF,lep1_isoSF,lep1_seed,"
             << "lep2_pt,lep2_eta,lep2_iso,lep2_pdgId,lep2_idSF,lep2_isoSF,lep2_seed,"
             << "jet1_pt,jet1_eta,jet1_phi,jet1_jesSF,jet1_jesSF_up,jet1_jesSF_down,jet1_jesSF_PileUpDataMC_down,jet1_jesSF_RelativeFSR_up,jet1_jerSF_nominal,jet1_csv,jet1_PUJetId,jet1_PUJetDiscriminant,jet1_seed,"
             << "jet2_pt,jet2_eta,jet2_phi,jet2_jesSF,jet2_jesSF_up,jet2_jesSF_down,jet2_jesSF_PileUpDataMC_down,jet2_jesSF_RelativeFSR_up,jet2_jerSF_nominal,jet2_csv,jet2_PUJetId,jet2_PUJetDiscriminant,jet2_seed,"
             << "MET_pt,MET_phi,mll,ttHFCategory,ttHFGenFilterTag,n_interactions,puWeight,"
             << "csvSF,csvSF_lf_up,csvSF_hf_down,csvSF_cErr1_down,"
             << "pdf_up,pdf_down,me_up,me_down,triggerSF,top_pt_weight,"
             << "bdt_output,dnn_ttH_output,dnn_ttbb_output"<<endl;

  if(DoSync && SyncDebug) 
    csvfile  << "run,lumi,event,is_e,is_mu,is_ee,is_emu,is_mumu,n_jets,n_btags,"
             << "lep1_pt,lep1_eta,lep1_phi,lep1_crack,lep1_iso,lep1_pdgId,lep1_idSF,lep1_isoSF,lep1_seed,"
             << "lep2_pt,lep2_eta,lep2_iso,lep2_pdgId,lep2_idSF,lep2_isoSF,lep2_seed,"
             << "jet1_pt,jet1_eta,jet1_phi,jet1_jesSF,jet1_jesSF_up,jet1_jesSF_down,jet1_jesSF_PileUpDataMC_down,jet1_jesSF_RelativeFSR_up,jet1_jerSF_nominal,jet1_csv,jet1_PUJetId,jet1_PUJetDiscriminant,jet1_seed,"
             << "jet2_pt,jet2_eta,jet2_phi,jet2_jesSF,jet2_jesSF_up,jet2_jesSF_down,jet2_jesSF_PileUpDataMC_down,jet2_jesSF_RelativeFSR_up,jet2_jerSF_nominal,jet2_csv,jet2_PUJetId,jet2_PUJetDiscriminant,jet2_seed,"
             << "jet3_pt,jet3_eta,jet3_phi,jet3_csv,"
             << "jet4_pt,jet4_eta,jet4_phi,jet4_csv,"
             << "jetClose,MET_pt,MET_phi,mll,ttHFCategory,ttHFGenFilterTag,n_interactions,puWeight,"
             << "csvSF,csvSF_lf_up,csvSF_hf_down,csvSF_cErr1_down,"
             << "pdf_up,pdf_down,me_up,me_down,triggerSF,top_pt_weight,"
             << "bdt_output,dnn_ttH_output,dnn_ttbb_output"<<endl;
 
  csvfile.close();

}

// ------------ method called once each job just after ending the event loop  ------------
void 
YggdrasilTreeMaker2017::endJob() 
{

  if(!DoSync){
    std::cout << "***************************************************************" << std::endl;
    std::cout << "  Summary for Filters: Total number of events = " << numEvents_ << std::endl;
    
    std::cout<<"this stuff "<<std::endl;
    std::cout<<"badChcount: "<<badChcount<<std::endl;
    std::cout<<"badpfmucount: "<<badpfmucount<<std::endl;
    std::cout<<"badglobalmucount: "<<badglobalmucount<<std::endl;
    std::cout<<"cloneglobalmucount: "<<cloneglobalmucount<<std::endl;
    for( std::map<std::string, int>::const_iterator iter = flt_cppath_.begin(); iter != flt_cppath_.end(); iter++){
      std::string name = iter->first;
      double eff = 100*double(iter->second)/double(numEvents_);
      printf("\t %s,\t %d,\t %.1f \n",name.c_str(), iter->second, eff);
    }
    std::cout << "***************************************************************" << std::endl;
   }
}

// ------------ method called when starting to processes a run  ------------

void 
YggdrasilTreeMaker2017::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{

    

    bool hltchanged = true;
  if (!hlt_config_.init(iRun, iSetup, "HLT", hltchanged)) {
    std::cout << "Warning, didn't find trigger process HLT,\t" << "HLT" << std::endl;
    return;
  }
   bool filterchanged = true;
  if (!filter_config_.init(iRun, iSetup, "PAT", filterchanged)) {
    std::cout << "Warning, didn't find filter process HLT,\t" << "RECO" << std::endl;
    return;
  }
  
  
  
  std::vector<std::string> filterNames  = filter_config_.triggerNames();

 
  flt_filterNames_.clear();


  flt_filterNames_.push_back("All");
  for( unsigned int iPath=0; iPath<filterNames.size(); iPath++ ){
    std::string name = filterNames[iPath];
    std::string pathNameNoVer = filter_config_.removeVersion(name);
    flt_cppath_[pathNameNoVer] = 0;
    flt_filterNames_.push_back(pathNameNoVer);
  }
  
  
 
}



// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
YggdrasilTreeMaker2017::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


DEFINE_FWK_MODULE(YggdrasilTreeMaker2017);   


