// -*- C++ -*-
//
// Package:    ttH-LeptonPlusJets/TTHTriggerAnalyzer
// Class:      TTHTriggerAnalyzer
// 
/**\class TTHTriggerAnalyzer TTHTriggerAnalyzer.cc ttH-LeptonPlusJets/AnalysisCode/plugins/TTHTriggerAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Darren Puigh
//         Created:  Wed, 02 Jul 2014 20:01:00 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

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

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerEvmReadoutRecord.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMapFwd.h"
#include "DataFormats/L1Trigger/interface/L1HFRings.h"
#include "DataFormats/L1Trigger/interface/L1HFRingsFwd.h"

#include "DataFormats/Candidate/interface/VertexCompositePtrCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositePtrCandidateFwd.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TTree.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "MiniAOD/MiniAODHelper/interface/MiniAODHelper.h"

#include "ttH-LeptonPlusJets/AnalysisCode/interface/TriggerStudyEventVars.h"


//
// class declaration
//

class TTHTriggerAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TTHTriggerAnalyzer(const edm::ParameterSet&);
      ~TTHTriggerAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      virtual void beginRun(edm::Run const& iRun,edm::EventSetup const& iSetup) override;
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
  edm::EDGetTokenT<L1GlobalTriggerReadoutRecord> gtReadoutRecordToken;

  edm::EDGetTokenT <edm::TriggerResults> triggerResultsToken;
  edm::EDGetTokenT <trigger::TriggerEvent> triggerEventToken;
  edm::EDGetTokenT <edm::TriggerResults> filterResultsToken;
  edm::EDGetTokenT <pat::TriggerObjectStandAloneCollection> triggerObjectsToken;

  edm::EDGetTokenT <reco::VertexCollection> vertexToken;
  edm::EDGetTokenT <reco::VertexCompositePtrCandidateCollection> secondaryVertexToken;
  edm::EDGetTokenT <pat::ElectronCollection> electronToken;
  edm::EDGetTokenT <pat::MuonCollection> muonToken;
  edm::EDGetTokenT <pat::JetCollection> jetToken;
  edm::EDGetTokenT <pat::METCollection> metToken;

  edm::EDGetTokenT <pat::PackedCandidateCollection> packedpfToken;

  edm::EDGetTokenT <reco::BeamSpot> beamspotToken;
  edm::EDGetTokenT <double> rhoToken;
  edm::EDGetTokenT <reco::GenParticleCollection> mcparicleToken;
  edm::EDGetTokenT <std::vector< PileupSummaryInfo > > puInfoToken;

  edm::EDGetTokenT <l1extra::L1EmParticleCollection> l1IsoEmToken;
  edm::EDGetTokenT <l1extra::L1EmParticleCollection> l1NonIsoEmToken;
  edm::EDGetTokenT <l1extra::L1JetParticleCollection> l1CenJetToken;
  edm::EDGetTokenT <l1extra::L1EtMissParticleCollection> l1HTMissToken;

  HLTConfigProvider hlt_config_;
  HLTConfigProvider filter_config_;

  std::string hltTag;
  std::string filterTag;

  std::string hltPath_;

  std::map<std::string, int> l1talgo_cppath_;

  std::map<std::string, int> hlt_cppath_;
  std::map<std::string, int> flt_cppath_;

  bool verbose_;
  bool dumpHLT_;

  int numEvents_;
  int numHbb;
  int numHdecayToOneParticle;
  int numHdecayToOneGluon;
  int numHdecayToOnePhoton;
  int numHdecayToOneZBoson;
  int numHdecayToOneWBoson;


  int insample_;

  std::vector<std::string> hlt_triggerNames_;
  std::vector<std::string> flt_filterNames_;

  edm::Service<TFileService> fs_;

  TTree *m_ttree;
  triggerStudyEventVars *eve;


  double mySample_xSec_;
  double mySample_nGen_;
  double intLumi_;

  double ptmax;
  int NptBins;

  double jetptmax;
  int NjetptBins;

  int NjetMin;
  int NjetMax;
  int NjetBins;
  int NtagMin;
  int NtagMax;
  int NtagBins;
  int NpuMin;
  int NpuMax;
  int NpuBins;


  // Declare histograms
  TH1D *h_hlt;
  TH1D *h_flt;

  TH1D* h_numJet;
  TH1D* h_numTag;
  TH1D* h_numPV;
  TH1D* h_numTruePV;

  TH1D* h_numSecVtx;

  TH1D* h_H_pt;
  TH1D* h_H_eta;
  TH1D* h_H_mass;
  TH1D* h_H_Mbb;
  TH1D* h_H_DeltaRbb;
  TH1D* h_H_maxDeltaRbH;
  TH2D* h_H_pt_DeltaRbb;
  TH2D* h_H_pt_maxDeltaRbH;

  TH1D* h_W_pt;
  TH1D* h_W_eta;
  TH1D* h_W_mass;

  TH1D* h_top_pt;
  TH1D* h_top_eta;
  TH1D* h_top_mass;
  TH1D* h_top_Mbqq;
  TH1D* h_top_Mbbarqq;
  TH1D* h_top_maxDeltaRjj;
  TH1D* h_top_maxDeltaRjt;
  TH2D* h_top_pt_maxDeltaRjj;
  TH2D* h_top_pt_maxDeltaRjt;

  TH1D* h_electron_selection;
  TH1D* h_muon_selection;

  TH2D* h_Wdecay;
  TH1D* h_Hdecay;

  TH1D* h_category_yield;
  TH1D* h_category_yield_1l;
  TH1D* h_category_yield_1e;
  TH1D* h_category_yield_1m;
  TH1D* h_category_yield_1l_noiso;
  TH1D* h_category_yield_1e_noiso;
  TH1D* h_category_yield_1m_noiso;

  MiniAODHelper miniAODhelper;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TTHTriggerAnalyzer::TTHTriggerAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  verbose_ = false;
  dumpHLT_ = false;

  hltTag = iConfig.getUntrackedParameter("HLTsource",std::string("reHLT"));
  filterTag = "PAT";

  hltPath_ = "HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1";

  gtReadoutRecordToken = consumes <L1GlobalTriggerReadoutRecord> (edm::InputTag(std::string("simGtDigis"), std::string(""), std::string("reHLT")));

  triggerResultsToken = consumes <edm::TriggerResults> (edm::InputTag(std::string("TriggerResults"), std::string(""), std::string("reHLT")));
  triggerEventToken = consumes <trigger::TriggerEvent> (edm::InputTag(std::string("hltTriggerSummaryAOD"), std::string(""), std::string("reHLT")));
  filterResultsToken = consumes <edm::TriggerResults> (edm::InputTag(std::string("TriggerResults"), std::string(""), filterTag));
  triggerObjectsToken = consumes <pat::TriggerObjectStandAloneCollection> (edm::InputTag(std::string("selectedPatTrigger")));


  vertexToken = consumes <reco::VertexCollection> (edm::InputTag(std::string("offlineSlimmedPrimaryVertices")));
  secondaryVertexToken = consumes <reco::VertexCompositePtrCandidateCollection> (edm::InputTag(std::string("slimmedSecondaryVertices")));
  electronToken = consumes <pat::ElectronCollection> (edm::InputTag(std::string("slimmedElectrons")));
  muonToken = consumes <pat::MuonCollection> (edm::InputTag(std::string("slimmedMuons")));
  jetToken = consumes <pat::JetCollection> (edm::InputTag(std::string("slimmedJets")));
  metToken = consumes <pat::METCollection> (edm::InputTag(std::string("slimmedMETs")));

  packedpfToken = consumes <pat::PackedCandidateCollection> (edm::InputTag(std::string("packedPFCandidates")));

  beamspotToken = consumes <reco::BeamSpot> (edm::InputTag(std::string("offlineBeamSpot")));
  rhoToken = consumes <double> (edm::InputTag(std::string("fixedGridRhoAll")));
  mcparicleToken = consumes <reco::GenParticleCollection> (edm::InputTag(std::string("prunedGenParticles")));
  puInfoToken = consumes <std::vector< PileupSummaryInfo > > (edm::InputTag(std::string("addPileupInfo")));

  l1IsoEmToken = consumes <l1extra::L1EmParticleCollection> (edm::InputTag(std::string("hltL1extraParticles"), std::string("Isolated")));
  l1NonIsoEmToken = consumes <l1extra::L1EmParticleCollection> (edm::InputTag(std::string("hltL1extraParticles"), std::string("NonIsolated")));
  l1CenJetToken = consumes <l1extra::L1JetParticleCollection> (edm::InputTag(std::string("hltL1extraParticles"), std::string("Central")));
  l1HTMissToken = consumes <l1extra::L1EtMissParticleCollection> (edm::InputTag(std::string("hltL1extraParticles"), std::string("MHT")));



  ptmax = 500.;
  NptBins = int( ptmax/1. + 0.0001 );

  jetptmax = 1000.;
  NjetptBins = int( ptmax/1. + 0.0001 );

  NjetMin = 0;
  NjetMax = 15;
  NjetBins = NjetMax - NjetMin + 1;
  NtagMin = 0;
  NtagMax = 8;
  NtagBins = NtagMax - NtagMin + 1;
  NpuMin = 0;
  NpuMax = 50;
  NpuBins = NpuMax - NpuMin + 1;

  // book the TTree
  m_ttree = fs_->make<TTree>("triggerTree", "triggerTree");
  eve=0; 
  m_ttree->Branch("eve.", "triggerStudyEventVars", &eve, 8000, 1);


  h_numJet = fs_->make<TH1D>("h_numJet", ";Number of Jets", NjetBins, NjetMin-0.5, NjetMax+0.5 );
  h_numTag = fs_->make<TH1D>("h_numTag", ";Number of Tags", NtagBins, NtagMin-0.5, NtagMax+0.5 );
  h_numPV  = fs_->make<TH1D>("h_numPV", ";Number of Good Vertices", NpuBins, NpuMin-0.5, NpuMax+0.5 );
  h_numTruePV  = fs_->make<TH1D>("h_numTruePV", ";Number of True PU Vertices", NpuBins, NpuMin-0.5, NpuMax+0.5 );

  h_numSecVtx = fs_->make<TH1D>("h_numSecVtx", ";Number of Secondary Vertices", 6, 6-0.5, 6+0.5 );

  h_H_pt = fs_->make<TH1D>("h_H_pt",";H p_{T}", NjetptBins, 0, jetptmax );
  h_H_eta = fs_->make<TH1D>("h_H_eta",";H #eta", 70, -3.5, 3.5 );
  h_H_mass = fs_->make<TH1D>("h_H_mass",";H mass", 1000, 100, 200 );
  h_H_Mbb = fs_->make<TH1D>("h_H_Mbb",";mass(b,b)", 1000, 100, 200 );
  h_H_DeltaRbb = fs_->make<TH1D>("h_H_DeltaRbb",";#DeltaR(b,b)", 100, 0, 6.3 );
  h_H_maxDeltaRbH = fs_->make<TH1D>("h_H_maxDeltaRbH",";max #DeltaR(b,H)", 100, 0, 6.3 );
  h_H_pt_DeltaRbb = fs_->make<TH2D>("h_H_pt_DeltaRbb",";H p_{T};#DeltaR(b,b)", NjetptBins, 0, jetptmax, 100, 0, 6.3 );
  h_H_pt_maxDeltaRbH = fs_->make<TH2D>("h_H_pt_maxDeltaRbH",";H p_{T};max #DeltaR(b,H)", NjetptBins, 0, jetptmax, 100, 0, 6.3 );

  h_W_pt = fs_->make<TH1D>("h_W_pt",";W p_{T}", NjetptBins, 0, jetptmax );
  h_W_eta = fs_->make<TH1D>("h_W_eta",";W #eta", 70, -3.5, 3.5 );
  h_W_mass = fs_->make<TH1D>("h_W_mass",";W mass", 200, 0, 200 );

  h_top_pt = fs_->make<TH1D>("h_top_pt",";top p_{T}", NjetptBins, 0, jetptmax );
  h_top_eta = fs_->make<TH1D>("h_top_eta",";top #eta", 70, -3.5, 3.5 );
  h_top_mass = fs_->make<TH1D>("h_top_mass",";top mass", 100, 100, 200 );
  h_top_Mbqq = fs_->make<TH1D>("h_top_Mbqq",";mass(b,q,q)", 100, 100, 200 );
  h_top_Mbbarqq = fs_->make<TH1D>("h_top_Mbbarqq",";mass(bbar,q,q)", 100, 100, 200 );
  h_top_maxDeltaRjj = fs_->make<TH1D>("h_top_maxDeltaRjj",";max #DeltaR(jet,jet)", 100, 0, 6.3 );
  h_top_maxDeltaRjt = fs_->make<TH1D>("h_top_maxDeltaRjt",";max #DeltaR(jet,top)", 100, 0, 6.3 );
  h_top_pt_maxDeltaRjj = fs_->make<TH2D>("h_top_pt_maxDeltaRjj",";top p_{T};max #DeltaR(jet,jet)", NjetptBins, 0, jetptmax, 100, 0, 6.3 );
  h_top_pt_maxDeltaRjt = fs_->make<TH2D>("h_top_pt_maxDeltaRjt",";top p_{T};max #DeltaR(jet,top)", NjetptBins, 0, jetptmax, 100, 0, 6.3 );


  h_electron_selection = fs_->make<TH1D>("h_electron_selection",";electron cut", 7, 0 , 7 );
  h_muon_selection = fs_->make<TH1D>("h_muon_selection",";muon cut", 10, 0 , 10 );

  h_electron_selection->GetXaxis()->SetBinLabel(1,"All");
  h_electron_selection->GetXaxis()->SetBinLabel(2,"MC match");
  h_electron_selection->GetXaxis()->SetBinLabel(3,"p_{T}>30, |#eta|<2.5");
  h_electron_selection->GetXaxis()->SetBinLabel(4,"No exp inner trk hits");
  h_electron_selection->GetXaxis()->SetBinLabel(5,"Not conversion");
  h_electron_selection->GetXaxis()->SetBinLabel(6,"d0, dZ");
  h_electron_selection->GetXaxis()->SetBinLabel(7,"relIso < 0.1");

  h_muon_selection->GetXaxis()->SetBinLabel(1,"All");
  h_muon_selection->GetXaxis()->SetBinLabel(2,"MC match");
  h_muon_selection->GetXaxis()->SetBinLabel(3,"p_{T}>30, |#eta|<2.1");
  h_muon_selection->GetXaxis()->SetBinLabel(4,"Global Muon");
  h_muon_selection->GetXaxis()->SetBinLabel(5,"#Chi^{2}, validMuonHit");
  h_muon_selection->GetXaxis()->SetBinLabel(6,"validPixelHit");
  h_muon_selection->GetXaxis()->SetBinLabel(7,"matched stations");
  h_muon_selection->GetXaxis()->SetBinLabel(8,"trk layers w/meas");
  h_muon_selection->GetXaxis()->SetBinLabel(9,"d0, dZ");
  h_muon_selection->GetXaxis()->SetBinLabel(10,"relIso < 0.12");


  std::vector<TString> particle_names;
  particle_names.push_back("d");
  particle_names.push_back("u");
  particle_names.push_back("s");
  particle_names.push_back("c");
  particle_names.push_back("b");
  particle_names.push_back("t");
  particle_names.push_back("b'");
  particle_names.push_back("t'");
  particle_names.push_back("");
  particle_names.push_back("");
  particle_names.push_back("e");
  particle_names.push_back("#nu_{e}");
  particle_names.push_back("#mu");
  particle_names.push_back("#nu_{#mu}");
  particle_names.push_back("#tau");
  particle_names.push_back("#nu_{#tau}");

  int numParticles = int( particle_names.size() );

  h_Wdecay = fs_->make<TH2D>("h_Wdecay",";particle 1; particle 2", numParticles, 0, numParticles, numParticles, 0, numParticles );
  for( int iPar=0; iPar<numParticles; iPar++ ){
    h_Wdecay->GetYaxis()->SetBinLabel(iPar+1,particle_names[iPar]);
    h_Wdecay->GetXaxis()->SetBinLabel(iPar+1,particle_names[iPar]);
  }


  std::vector<std::string> Hdecay_names;
  Hdecay_names.push_back("b#bar{b}");
  Hdecay_names.push_back("WW");
  Hdecay_names.push_back("#tau#tau");
  Hdecay_names.push_back("gg");
  Hdecay_names.push_back("#gamma#gamma");
  Hdecay_names.push_back("ZZ");
  Hdecay_names.push_back("c#bar{c}");
  Hdecay_names.push_back("Z#gamma");
  Hdecay_names.push_back("#mu#mu");
  Hdecay_names.push_back("ee");
  Hdecay_names.push_back("s#bar{s}");
  Hdecay_names.push_back("u#bar{u}");
  Hdecay_names.push_back("d#bar{d}");
  Hdecay_names.push_back("Other");


  int numHdecays = int(Hdecay_names.size());

  h_Hdecay = fs_->make<TH1D>("h_Hdecay",";H decay", numHdecays, 0, numHdecays );

  for( int i=0; i<numHdecays; i++ ) h_Hdecay->GetXaxis()->SetBinLabel(i+1,Hdecay_names[i].c_str());



  std::vector<std::string> cat_labels;
  cat_labels.push_back("incl4j2t");
  cat_labels.push_back("4j2t");
  cat_labels.push_back("5j2t");
  cat_labels.push_back("6j2t");
  cat_labels.push_back("4j3t");
  cat_labels.push_back("5j3t");
  cat_labels.push_back("6j3t");
  cat_labels.push_back("4j4t");
  cat_labels.push_back("5j4t");
  cat_labels.push_back("6j4t");

  int NumCat = int(cat_labels.size());

  h_category_yield = fs_->make<TH1D>("h_category_yield", ";category", NumCat, 0, NumCat );
  h_category_yield_1l = fs_->make<TH1D>("h_category_yield_1l", ";category", NumCat, 0, NumCat );
  h_category_yield_1e = fs_->make<TH1D>("h_category_yield_1e", ";category", NumCat, 0, NumCat );
  h_category_yield_1m = fs_->make<TH1D>("h_category_yield_1m", ";category", NumCat, 0, NumCat );
  h_category_yield_1l_noiso = fs_->make<TH1D>("h_category_yield_1l_noiso", ";category", NumCat, 0, NumCat );
  h_category_yield_1e_noiso = fs_->make<TH1D>("h_category_yield_1e_noiso", ";category", NumCat, 0, NumCat );
  h_category_yield_1m_noiso = fs_->make<TH1D>("h_category_yield_1m_noiso", ";category", NumCat, 0, NumCat );

  for( int iCat=0; iCat<NumCat; iCat++ ){
    h_category_yield->GetXaxis()->SetBinLabel(iCat+1,cat_labels[iCat].c_str());
    h_category_yield_1l->GetXaxis()->SetBinLabel(iCat+1,cat_labels[iCat].c_str());
    h_category_yield_1e->GetXaxis()->SetBinLabel(iCat+1,cat_labels[iCat].c_str());
    h_category_yield_1m->GetXaxis()->SetBinLabel(iCat+1,cat_labels[iCat].c_str());
    h_category_yield_1l_noiso->GetXaxis()->SetBinLabel(iCat+1,cat_labels[iCat].c_str());
    h_category_yield_1e_noiso->GetXaxis()->SetBinLabel(iCat+1,cat_labels[iCat].c_str());
    h_category_yield_1m_noiso->GetXaxis()->SetBinLabel(iCat+1,cat_labels[iCat].c_str());
  }



  std::string era = "2012_53x";
  insample_ = 2500;

  mySample_xSec_ = 689.1;
  mySample_nGen_ = 25474122;
  intLumi_ = 20000;

  analysisType::analysisType iAnalysisType = analysisType::LJ;
  bool isData = true;

  miniAODhelper.SetUp(era, insample_, iAnalysisType, isData);

}


TTHTriggerAnalyzer::~TTHTriggerAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TTHTriggerAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  numEvents_++;


  double minTightLeptonPt = 10.;


  h_hlt->Fill(0.,1);
  h_flt->Fill(0.,1);

  edm::Handle<L1GlobalTriggerReadoutRecord> gtReadoutRecord;
  iEvent.getByToken(gtReadoutRecordToken, gtReadoutRecord);

  edm::Handle<edm::TriggerResults> triggerResults;
  iEvent.getByToken(triggerResultsToken, triggerResults);

  edm::Handle<trigger::TriggerEvent> triggerEventAOD;
  iEvent.getByToken(triggerEventToken, triggerEventAOD);

  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
  iEvent.getByToken(triggerObjectsToken, triggerObjects);

  // Get Trigger and Event Handles
  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  const L1GtTriggerMenu* menu = menuRcd.product();

  if( gtReadoutRecord.isValid() ){
    const DecisionWord& gtDecisionWord = gtReadoutRecord->decisionWord();

    // L1 algos
    for( CItAlgo algo = menu->gtAlgorithmMap().begin(); algo!=menu->gtAlgorithmMap().end(); ++algo) {
      int algoBitNumber = (algo->second).algoBitNumber();

      std::string name = (algo->second).algoName();
      bool accept = gtDecisionWord.at(algoBitNumber);
      int prescaleFactor = -1;

      if( accept ) l1talgo_cppath_[name]+=1;
      if( verbose_ ) std::cout << " =====>  L1T algo: path name = " << (algo->second).algoName() << ",\t prescale = " << prescaleFactor << ",\t pass = " << gtDecisionWord.at(algoBitNumber) << std::endl; 
    }
  }

  std::vector<TLorentzVector> triggerObjects_L1SingleIsoEG;
  std::vector<TLorentzVector> triggerObjects_hltEle;

  std::vector<TLorentzVector> triggerObjects_hltJet30;
  std::vector<TLorentzVector> triggerObjects_hltBtagJet30;
  std::vector<TLorentzVector> triggerObjects_hltJet40;

  std::vector<TLorentzVector> triggerObjects_hltPFHT200;

  std::vector<std::string> vec_hltEle27WP85Gsf_filter;

  std::vector<int> vec_hltPFHT200_id;

  if( triggerEventAOD.isValid() ){
    std::vector<trigger::size_type> filtKeys;
    std::vector<std::string> filtKeys_string;
    for(unsigned int i=0; i<triggerEventAOD->sizeFilters(); i++) {
      const edm::InputTag filterTag = triggerEventAOD->filterTag(i);
      const std::string filt = filterTag.encode();

      const trigger::size_type filtIndex = triggerEventAOD->filterIndex(filterTag);
      const std::vector<trigger::size_type>& theseKeys = triggerEventAOD->filterKeys(filtIndex);
      int numKeys = theseKeys.size();
      for(int i=0; i<numKeys; i++) {
	filtKeys.push_back(theseKeys.at(i));
	filtKeys_string.push_back(filt);
      }
    }

    const trigger::TriggerObjectCollection& hltObjects = triggerEventAOD->getObjects();
    for(size_t i=0; i<filtKeys.size(); i++) {
      size_t key = filtKeys.at(i);
      const trigger::TriggerObject obj = hltObjects[key];

      TLorentzVector obj_TLV;
      obj_TLV.SetPxPyPzE( obj.px(), obj.py(), obj.pz(), obj.energy());

      if( filtKeys_string[i]=="hltL1sL1SingleIsoEG25er::reHLT" ) triggerObjects_L1SingleIsoEG.push_back(obj_TLV);

      if( filtKeys_string[i]=="hltL1sL1EG25erHTT125::reHLT" ||
	  filtKeys_string[i]=="hltNewEG27EtFilter::reHLT" ||
	  filtKeys_string[i]=="hltNewEle27WP85ClusterShapeFilter::reHLT" ||
	  filtKeys_string[i]=="hltNewEle27WP85HEFilter::reHLT" ||
	  filtKeys_string[i]=="hltNewEle27WP85EcalIsoFilter::reHLT" ||
	  filtKeys_string[i]=="hltNewEle27WP85HcalIsoFilter::reHLT" ||
	  filtKeys_string[i]=="hltNewEle27WP85PixelMatchFilter::reHLT" ||
	  filtKeys_string[i]=="hltNewEle27WP85GsfOneOEMinusOneOPFilter::reHLT" ||
	  filtKeys_string[i]=="hltNewEle27WP85GsfMissingHitsFilter::reHLT" ||
	  filtKeys_string[i]=="hltNewEle27WP85GsfDetaFilter::reHLT" ||
	  filtKeys_string[i]=="hltNewEle27WP85GsfDphiFilter::reHLT" ||
	  filtKeys_string[i]=="hltNewEle27WP85GsfTrackIsoFilter::reHLT" ){
	triggerObjects_hltEle.push_back(obj_TLV);
	vec_hltEle27WP85Gsf_filter.push_back(filtKeys_string[i]);
      }

      if( filtKeys_string[i]=="hltPFHT200::reHLT" ){
	triggerObjects_hltPFHT200.push_back(obj_TLV);
	vec_hltPFHT200_id.push_back(obj.id());
      }

      if( filtKeys_string[i]=="hltJetFilterSingleTopEle27::reHLT" ) triggerObjects_hltJet30.push_back(obj_TLV);
      if( filtKeys_string[i]=="hltCSVFilterSingleTopEle27::reHLT" ) triggerObjects_hltBtagJet30.push_back(obj_TLV);
      if( filtKeys_string[i]=="hltEle27TriCentralPFJet40EleCleaned::reHLT" ) triggerObjects_hltJet40.push_back(obj_TLV);

      if( verbose_ ) std::cout << " ===> HLT: filter = " << filtKeys_string[i] << ",\t id = " << obj.id() << ",\t pt = " << obj.pt() << ",\t eta = " << obj.eta() << ",\t phi = " << obj.phi() << std::endl;
    }
  }


  bool passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_v1 = false;
  bool passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1 = false;
  bool passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet40_v1 = false;
  bool passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1 = false;

  bool passesHLT_HLT_Ele32_eta2p1_WP85_Gsf_v1 = false;
  bool passesHLT_HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1 = false;
  bool passesHLT_HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet40_v1 = false;
  bool passesHLT_HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1 = false;

  bool passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1 = false;

  if( triggerResults.isValid() ){
    std::vector<std::string> triggerNames = hlt_config_.triggerNames();

    for( unsigned int iPath=0; iPath<triggerNames.size(); iPath++ ){
      std::string pathName = triggerNames[iPath];
      unsigned int hltIndex = hlt_config_.triggerIndex(pathName);

      if( hltIndex >= triggerResults->size() ) continue;

      int accept = triggerResults->accept(hltIndex);
      int prescale = -1;//hlt_config_.prescaleValue(iEvent, iSetup, pathName);

      if( verbose_ && dumpHLT_ ) std::cout << " =====>  HLT: path name = " << pathName << ",\t prescale = " << prescale << ",\t pass = " << accept << std::endl; 

      unsigned int triggerEventSize = 0;
      if( triggerEventAOD.isValid() ) triggerEventSize = triggerEventAOD->sizeFilters();

      const std::vector<std::string>& moduleLabels = hlt_config_.moduleLabels(pathName);
      int NumModules = int( moduleLabels.size() );

      std::string prefix("hltPre");

      for( int iMod=0; iMod<NumModules; iMod++ ){
	std::string moduleType = hlt_config_.moduleType(moduleLabels[iMod]);
	std::string moduleEDMType = hlt_config_.moduleEDMType(moduleLabels[iMod]);
	if( !(moduleEDMType == "EDFilter") ) continue;
	if( moduleType.find("Selector")!= std::string::npos ) continue;
	if( moduleType == "HLTTriggerTypeFilter" || 
	    moduleType == "HLTBool" ||
	    moduleType == "PrimaryVertexObjectFilter" ||
	    moduleType == "JetVertexChecker" ||
	    moduleType == "HLTRHemisphere" ||
	    moduleType == "DetectorStateFilter" ) continue;

	if( moduleLabels[iMod].compare(0, prefix.length(), prefix) == 0 ) continue;

	edm::InputTag moduleWhoseResultsWeWant(moduleLabels[iMod],
					       "",
					       "reHLT");

	unsigned int idx_module_trg = 0;
	if( triggerEventAOD.isValid() ) idx_module_trg = triggerEventAOD->filterIndex(moduleWhoseResultsWeWant);
	if( !(idx_module_trg < triggerEventSize) ) continue;
      }

      std::string pathNameNoVer = hlt_config_.removeVersion(pathName);

      if( accept ) hlt_cppath_[pathNameNoVer]+=1;

      if( pathName=="HLT_Ele27_eta2p1_WP85_Gsf_v1" && accept )                         passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_v1 = true;
      if( pathName=="HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1" && accept )  passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1 = true;
      if( pathName=="HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet40_v1" && accept )       passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet40_v1 = true;
      if( pathName=="HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1" && accept ) passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1 = true;

      if( pathName=="HLT_Ele32_eta2p1_WP85_Gsf_v1" && accept )                         passesHLT_HLT_Ele32_eta2p1_WP85_Gsf_v1 = true;
      if( pathName=="HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1" && accept )  passesHLT_HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1 = true;
      if( pathName=="HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet40_v1" && accept )       passesHLT_HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet40_v1 = true;
      if( pathName=="HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1" && accept ) passesHLT_HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1 = true;

      if( pathName=="HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1" && accept )                   passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1 = true;

      if( accept ){
	TAxis * axis = h_hlt->GetXaxis();
	if( !axis ) continue;
	int bin_num = axis->FindBin(pathNameNoVer.c_str());
	int bn = bin_num - 1;
	h_hlt->Fill(bn, 1);
      }
    }
  }
  else{
    std::cout << "Trigger results not valid for tag " << "reHLT" << std::endl;
  }



  std::vector<double> vec_hltL1SingleIsoEG25er_pt;
  std::vector<double> vec_hltL1SingleIsoEG25er_eta;
  std::vector<double> vec_hltL1SingleIsoEG25er_phi;
  for( int iL1TObj = 0; iL1TObj < int(triggerObjects_L1SingleIsoEG.size()); iL1TObj++ ){ 
    vec_hltL1SingleIsoEG25er_pt.push_back(triggerObjects_L1SingleIsoEG[iL1TObj].Pt());
    vec_hltL1SingleIsoEG25er_eta.push_back(triggerObjects_L1SingleIsoEG[iL1TObj].Eta());
    vec_hltL1SingleIsoEG25er_phi.push_back(triggerObjects_L1SingleIsoEG[iL1TObj].Phi());
  }
  eve->hltL1SingleIsoEG25er_pt_  = vec_hltL1SingleIsoEG25er_pt;
  eve->hltL1SingleIsoEG25er_eta_ = vec_hltL1SingleIsoEG25er_eta;
  eve->hltL1SingleIsoEG25er_phi_ = vec_hltL1SingleIsoEG25er_phi;

  std::vector<double> vec_hltEle27WP85Gsf_pt;
  std::vector<double> vec_hltEle27WP85Gsf_eta;
  std::vector<double> vec_hltEle27WP85Gsf_phi;
  for( int iHLTObj = 0; iHLTObj < int(triggerObjects_hltEle.size()); iHLTObj++ ){ 
    vec_hltEle27WP85Gsf_pt.push_back(triggerObjects_hltEle[iHLTObj].Pt());
    vec_hltEle27WP85Gsf_eta.push_back(triggerObjects_hltEle[iHLTObj].Eta());
    vec_hltEle27WP85Gsf_phi.push_back(triggerObjects_hltEle[iHLTObj].Phi());
  }
  eve->hltEle27WP85Gsf_pt_  = vec_hltEle27WP85Gsf_pt;
  eve->hltEle27WP85Gsf_eta_ = vec_hltEle27WP85Gsf_eta;
  eve->hltEle27WP85Gsf_phi_ = vec_hltEle27WP85Gsf_phi;
  eve->hltEle27WP85Gsf_filter_ = vec_hltEle27WP85Gsf_filter;

  std::vector<double> vec_hltJet30_pt;
  std::vector<double> vec_hltJet30_eta;
  std::vector<double> vec_hltJet30_phi;
  for( int iHLTObj = 0; iHLTObj < int(triggerObjects_hltJet30.size()); iHLTObj++ ){ 
    vec_hltJet30_pt.push_back(triggerObjects_hltJet30[iHLTObj].Pt());
    vec_hltJet30_eta.push_back(triggerObjects_hltJet30[iHLTObj].Eta());
    vec_hltJet30_phi.push_back(triggerObjects_hltJet30[iHLTObj].Phi());
  }
  eve->hltJet30_pt_  = vec_hltJet30_pt;
  eve->hltJet30_eta_ = vec_hltJet30_eta;
  eve->hltJet30_phi_ = vec_hltJet30_phi;


  std::vector<double> vec_hltBtagJet30_pt;
  std::vector<double> vec_hltBtagJet30_eta;
  std::vector<double> vec_hltBtagJet30_phi;
  for( int iHLTObj = 0; iHLTObj < int(triggerObjects_hltBtagJet30.size()); iHLTObj++ ){ 
    vec_hltBtagJet30_pt.push_back(triggerObjects_hltBtagJet30[iHLTObj].Pt());
    vec_hltBtagJet30_eta.push_back(triggerObjects_hltBtagJet30[iHLTObj].Eta());
    vec_hltBtagJet30_phi.push_back(triggerObjects_hltBtagJet30[iHLTObj].Phi());
  }
  eve->hltBtagJet30_pt_  = vec_hltBtagJet30_pt;
  eve->hltBtagJet30_eta_ = vec_hltBtagJet30_eta;
  eve->hltBtagJet30_phi_ = vec_hltBtagJet30_phi;


  std::vector<double> vec_hltJet40_pt;
  std::vector<double> vec_hltJet40_eta;
  std::vector<double> vec_hltJet40_phi;
  for( int iHLTObj = 0; iHLTObj < int(triggerObjects_hltJet40.size()); iHLTObj++ ){ 
    vec_hltJet40_pt.push_back(triggerObjects_hltJet40[iHLTObj].Pt());
    vec_hltJet40_eta.push_back(triggerObjects_hltJet40[iHLTObj].Eta());
    vec_hltJet40_phi.push_back(triggerObjects_hltJet40[iHLTObj].Phi());
  }
  eve->hltJet40_pt_  = vec_hltJet40_pt;
  eve->hltJet40_eta_ = vec_hltJet40_eta;
  eve->hltJet40_phi_ = vec_hltJet40_phi;


  std::vector<double> vec_hltPFHT200_pt;
  std::vector<double> vec_hltPFHT200_eta;
  std::vector<double> vec_hltPFHT200_phi;
  for( int iHLTObj = 0; iHLTObj < int(triggerObjects_hltPFHT200.size()); iHLTObj++ ){ 
    vec_hltPFHT200_pt.push_back(triggerObjects_hltPFHT200[iHLTObj].Pt());
    vec_hltPFHT200_eta.push_back(triggerObjects_hltPFHT200[iHLTObj].Eta());
    vec_hltPFHT200_phi.push_back(triggerObjects_hltPFHT200[iHLTObj].Phi());
  }
  eve->hltPFHT200_pt_  = vec_hltPFHT200_pt;
  eve->hltPFHT200_eta_ = vec_hltPFHT200_eta;
  eve->hltPFHT200_phi_ = vec_hltPFHT200_phi;
  eve->hltPFHT200_id_  = vec_hltPFHT200_id;





  eve->pass_HLT_Ele27_eta2p1_WP85_Gsf_v1_                         = (passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_v1) ? 1 : 0;
  eve->pass_HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1_  = (passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1) ? 1 : 0;
  eve->pass_HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet40_v1_       = (passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet40_v1) ? 1 : 0;
  eve->pass_HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1_ = (passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1) ? 1 : 0;

  eve->pass_HLT_Ele32_eta2p1_WP85_Gsf_v1_                         = (passesHLT_HLT_Ele32_eta2p1_WP85_Gsf_v1) ? 1 : 0;
  eve->pass_HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1_  = (passesHLT_HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1) ? 1 : 0;
  eve->pass_HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet40_v1_       = (passesHLT_HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet40_v1) ? 1 : 0;
  eve->pass_HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1_ = (passesHLT_HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1) ? 1 : 0;

  eve->pass_HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1_                   = (passesHLT_HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1) ? 1 : 0;


  edm::Handle<edm::TriggerResults> filterResults;
  iEvent.getByToken(filterResultsToken, filterResults);

  if( filterResults.isValid() ){
    std::vector<std::string> triggerNames = filter_config_.triggerNames();

    for( unsigned int iPath=0; iPath<triggerNames.size(); iPath++ ){
      std::string pathName = triggerNames[iPath];
      unsigned int hltIndex = filter_config_.triggerIndex(pathName);

      if( hltIndex >= filterResults->size() ) continue;

      int accept = filterResults->accept(hltIndex);
      int prescale = -1;//filter_config_.prescaleValue(iEvent, iSetup, pathName);

      if( verbose_ && dumpHLT_ ) std::cout << " =====>  Filter: path name = " << pathName << ",\t prescale = " << prescale << ",\t pass = " << accept << std::endl; 

      std::string pathNameNoVer = filter_config_.removeVersion(pathName);

      if( accept ) flt_cppath_[pathNameNoVer]+=1;

      if( accept ){
	TAxis * axis = h_flt->GetXaxis();
	if( !axis ) continue;
	int bin_num = axis->FindBin(pathNameNoVer.c_str());
	int bn = bin_num - 1;
	h_flt->Fill(bn, 1);
      }
    }
  }
  else{
    std::cout << "Trigger results not valid for tag " << filterTag << std::endl;
  }



  edm::Handle<reco::VertexCollection> vtxHandle;
  iEvent.getByToken(vertexToken,vtxHandle);
  reco::VertexCollection vtxs = *vtxHandle;

  edm::Handle<reco::VertexCompositePtrCandidateCollection> secondaryVtxs;
  iEvent.getByToken(secondaryVertexToken,secondaryVtxs);

  edm::Handle<pat::ElectronCollection> electrons;
  iEvent.getByToken(electronToken,electrons);

  edm::Handle<pat::MuonCollection> muons;
  iEvent.getByToken(muonToken,muons);

  edm::Handle<pat::JetCollection> pfjets;
  iEvent.getByToken(jetToken,pfjets);

  edm::Handle<pat::METCollection> pfmet;
  iEvent.getByToken(metToken,pfmet);

  edm::Handle<pat::PackedCandidateCollection> packedPFcands;
  iEvent.getByToken(packedpfToken,packedPFcands);


  edm::Handle<reco::BeamSpot> bsHandle;
  iEvent.getByToken(beamspotToken,bsHandle);

  edm::Handle<reco::GenParticleCollection> mcparticles;
  iEvent.getByToken(mcparicleToken,mcparticles);

  edm::Handle<double> rhoHandle;
  iEvent.getByToken(rhoToken,rhoHandle);
  //double rho = *rhoHandle;

  edm::Handle<std::vector< PileupSummaryInfo > > PupInfo;
  iEvent.getByToken(puInfoToken,PupInfo);


  edm::Handle<l1extra::L1EmParticleCollection> l1IsoEms;
  iEvent.getByToken(l1IsoEmToken,l1IsoEms);

  edm::Handle<l1extra::L1EmParticleCollection> l1NonIsoEms;
  iEvent.getByToken(l1NonIsoEmToken,l1NonIsoEms);

  edm::Handle<l1extra::L1JetParticleCollection> l1CenJets;
  iEvent.getByToken(l1CenJetToken,l1CenJets);

  edm::Handle<l1extra::L1EtMissParticleCollection> l1HTs;
  iEvent.getByToken(l1HTMissToken,l1HTs);



  math::XYZPoint beamSpotPosition;
  beamSpotPosition.SetCoordinates(0,0,0);
  double BSx=0,BSy=0,BSz=0;
  if( (bsHandle.isValid()) ){
    reco::BeamSpot bs = *bsHandle;
    BSx = bs.x0();
    BSy = bs.y0();
    BSz = bs.z0();
    beamSpotPosition = bsHandle->position();
  }


  if( verbose_ ) printf("\t BeamSpot: x = %.2f,\t y = %.2f,\t z = %.2f \n", BSx, BSy, BSz );

  int numpv=0;
  reco::Vertex vertex;
  if( vtxHandle.isValid() ){
    for( reco::VertexCollection::const_iterator vtx = vtxs.begin(); vtx!=vtxs.end(); ++vtx ){
      bool isGood = ( !(vtx->isFake()) &&
		      (vtx->ndof() >= 4.0) &&
		      (abs(vtx->z()) <= 24.0) &&
		      (abs(vtx->position().Rho()) <= 2.0) 
		      );

      if( !isGood ) continue;

      if( numpv==0 ) vertex = (*vtx);

      numpv++;
    }
  }


  if( verbose_ ) printf("\t Event PV: x = %.3f,\t y = %.3f,\t z = %.3f \n", vertex.x(), vertex.y(), vertex.z());
  int numSecVtx = 0;
  for( reco::VertexCompositePtrCandidateCollection::const_iterator iVtx = secondaryVtxs->begin(); iVtx!=secondaryVtxs->end(); ++iVtx ){
    numSecVtx++;

    int numDgt = iVtx->numberOfDaughters();

    double dX = iVtx->vx() - vertex.x();
    double dY = iVtx->vy() - vertex.y();
    double dZ = iVtx->vz() - vertex.z();

    double dist = sqrt( dX*dX + dY*dY + dZ*dZ );

    if( verbose_ ) {
      printf("\t\t Secondary vtx %d: vx = %.3f,\t vy = %.3f,\t vz = %.3f,\t mass = %.1f,\t dist = %.2f,\t numDgt = %d \n", 
	     int(iVtx - secondaryVtxs->begin()), iVtx->vx(), iVtx->vy(), iVtx->vz(), iVtx->mass(), dist, numDgt);

      for( int iDau=0; iDau<numDgt; iDau++ ){
	printf("\t\t\t daughter %d: pdgId = %d,\t pt = %.1f,\t eta = %.2f,\t phi = %.2f \n", 
	       iDau, iVtx->daughter(iDau)->pdgId(), iVtx->daughter(iDau)->pt(), iVtx->daughter(iDau)->eta(), iVtx->daughter(iDau)->phi());
      }
    }
  }

  if( numpv>0 ) miniAODhelper.SetVertex(vertex);

  double numTruePV = -1;
  if( (PupInfo.isValid()) ){
    for( std::vector<PileupSummaryInfo>::const_iterator PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI ) {
      int BX = PVI->getBunchCrossing();
      if( BX==0 ) numTruePV = PVI->getTrueNumInteractions();
    }
  }


  const JetCorrector* corrector = JetCorrector::getJetCorrector( "ak4PFchsL1L2L3", iSetup );   //Get the jet corrector from the event setup

  miniAODhelper.SetJetCorrector(corrector);


  double wgt = 1;

  double xSec = mySample_xSec_;
  double nGen = mySample_nGen_;

  wgt = intLumi_ * xSec / nGen;


  eve->numPVs_ = numpv;


  /////////
  ///
  /// L1 objects
  ///
  ////////
  std::vector<double> vec_L1IsoEG_Stage1_pt;
  std::vector<double> vec_L1IsoEG_Stage1_eta;
  std::vector<double> vec_L1IsoEG_Stage1_phi;
  if( l1IsoEms.isValid() ){
    for( l1extra::L1EmParticleCollection::const_iterator l1IsoEm = l1IsoEms->begin(); l1IsoEm!=l1IsoEms->end(); ++l1IsoEm ){
      vec_L1IsoEG_Stage1_pt.push_back(l1IsoEm->pt());
      vec_L1IsoEG_Stage1_eta.push_back(l1IsoEm->eta());
      vec_L1IsoEG_Stage1_phi.push_back(l1IsoEm->phi());

      if( l1IsoEm->pt()<25 ) continue;
      if( fabs( l1IsoEm->eta() ) > 2.12 ) continue;

      if( verbose_ ){
	printf("L1EmParticleCollection  %d:\t\t pt = %.1f,\t eta = %.2f,\t phi = %.2f \n",
	       int( l1IsoEm - l1IsoEms->begin() ), l1IsoEm->pt(), l1IsoEm->eta(), l1IsoEm->phi() );
      }
    }
  }
  eve->L1IsoEG_Stage1_pt_  = vec_L1IsoEG_Stage1_pt;
  eve->L1IsoEG_Stage1_eta_ = vec_L1IsoEG_Stage1_eta;
  eve->L1IsoEG_Stage1_phi_ = vec_L1IsoEG_Stage1_phi;



  std::vector<double> vec_L1NonIsoEG_Stage1_pt;
  std::vector<double> vec_L1NonIsoEG_Stage1_eta;
  std::vector<double> vec_L1NonIsoEG_Stage1_phi;
  if( l1NonIsoEms.isValid() ){
    for( l1extra::L1EmParticleCollection::const_iterator l1NonIsoEm = l1NonIsoEms->begin(); l1NonIsoEm!=l1NonIsoEms->end(); ++l1NonIsoEm ){
      vec_L1NonIsoEG_Stage1_pt.push_back(l1NonIsoEm->pt());
      vec_L1NonIsoEG_Stage1_eta.push_back(l1NonIsoEm->eta());
      vec_L1NonIsoEG_Stage1_phi.push_back(l1NonIsoEm->phi());

      if( l1NonIsoEm->pt()<25 ) continue;
      if( fabs( l1NonIsoEm->eta() ) > 2.12 ) continue;

      if( verbose_ ){
	printf("L1EmParticleCollection  %d:\t\t pt = %.1f,\t eta = %.2f,\t phi = %.2f \n",
	       int( l1NonIsoEm - l1NonIsoEms->begin() ), l1NonIsoEm->pt(), l1NonIsoEm->eta(), l1NonIsoEm->phi() );
      }
    }
  }
  eve->L1NonIsoEG_Stage1_pt_  = vec_L1NonIsoEG_Stage1_pt;
  eve->L1NonIsoEG_Stage1_eta_ = vec_L1NonIsoEG_Stage1_eta;
  eve->L1NonIsoEG_Stage1_phi_ = vec_L1NonIsoEG_Stage1_phi;



  std::vector<double> vec_L1CenJet_Stage1_pt;
  std::vector<double> vec_L1CenJet_Stage1_eta;
  std::vector<double> vec_L1CenJet_Stage1_phi;
  if( l1CenJets.isValid() ){
    for( l1extra::L1JetParticleCollection::const_iterator l1CenJet = l1CenJets->begin(); l1CenJet!=l1CenJets->end(); ++l1CenJet ){
      vec_L1CenJet_Stage1_pt.push_back(l1CenJet->pt());
      vec_L1CenJet_Stage1_eta.push_back(l1CenJet->eta());
      vec_L1CenJet_Stage1_phi.push_back(l1CenJet->phi());
    }
  }
  eve->L1CenJet_Stage1_pt_  = vec_L1CenJet_Stage1_pt;
  eve->L1CenJet_Stage1_eta_ = vec_L1CenJet_Stage1_eta;
  eve->L1CenJet_Stage1_phi_ = vec_L1CenJet_Stage1_phi;



  for( int iL1Obj = 0; iL1Obj < int(triggerObjects_L1SingleIsoEG.size()); iL1Obj++ ){
    TLorentzVector l1obj = triggerObjects_L1SingleIsoEG[iL1Obj];
    if( l1obj.Pt()<25 ) continue;
    if( fabs( l1obj.Eta() ) > 2.12 ) continue;
    if( verbose_ ){
      printf("triggerObjectsHLT_L1SingleIsoEG %d:\t pt = %.1f,\t eta = %.2f,\t phi = %.2f \n",
	     iL1Obj, l1obj.Pt(), l1obj.Eta(), l1obj.Phi() );
    }
  }


  if( l1HTs.isValid() ){
    for( l1extra::L1EtMissParticleCollection::const_iterator il1HT = l1HTs->begin(); il1HT!=l1HTs->end(); ++il1HT ){
      eve->L1HTT_ = il1HT->etTotal();
    }
  }


  /////////
  ///
  /// Electrons
  ///
  ////////
  std::vector<pat::Electron> selectedElectrons_tight = miniAODhelper.GetSelectedElectrons( *electrons, minTightLeptonPt, electronID::electronTight );
  std::vector<pat::Electron> selectedElectrons_loose = miniAODhelper.GetSelectedElectrons( *electrons, 10., electronID::electronLoose );

  int numTightElectrons = int(selectedElectrons_tight.size());
  int numLooseElectrons = int(selectedElectrons_loose.size());// - numTightElectrons;


  /////////
  ///
  /// Muons
  ///
  ////////
  std::vector<pat::Muon> selectedMuons_tight = miniAODhelper.GetSelectedMuons( *muons, minTightLeptonPt, muonID::muonTight );
  std::vector<pat::Muon> selectedMuons_loose = miniAODhelper.GetSelectedMuons( *muons, 10., muonID::muonLoose );

  int numTightMuons = int(selectedMuons_tight.size());
  int numLooseMuons = int(selectedMuons_loose.size());// - numTightMuons;


  // Pass one tight lepton, zero loose lepton cuts
  bool oneGoodLepton = ( (numTightMuons+numTightElectrons)==1 &&
			 (numLooseMuons+numLooseElectrons)==1 );


  int numNoIsoMu=0;
  for( std::vector<pat::Muon>::const_iterator pfmu = muons->begin(); pfmu!=muons->end(); ++pfmu ){
    int ncut = 0;
    h_muon_selection->Fill(0.5+ncut++, 1);

    if( (pfmu->genLepton()) ){
      if( abs(pfmu->genLepton()->pdgId())==13 ) h_muon_selection->Fill(0.5+ncut++, 1);
      else continue;
    }
    else continue;

    if( pfmu->pt()>30 && fabs(pfmu->eta())<2.1 ) h_muon_selection->Fill(0.5+ncut++, 1);
    else continue;

    if( (pfmu->isGlobalMuon()) ) h_muon_selection->Fill(0.5+ncut++, 1);
    else continue;

    if( pfmu->globalTrack().isAvailable() ){
      if( (pfmu->globalTrack()->normalizedChi2() < 10.) &&
	  (pfmu->globalTrack()->hitPattern().numberOfValidMuonHits() > 0) ) h_muon_selection->Fill(0.5+ncut++, 1);
      else continue;
    }
    else continue;

    if( pfmu->innerTrack().isAvailable() ){
      if( (pfmu->innerTrack()->hitPattern().numberOfValidPixelHits() > 0) ) h_muon_selection->Fill(0.5+ncut++, 1);
      else continue;
    }
    else continue;

    if( (pfmu->numberOfMatchedStations() > 1) ) h_muon_selection->Fill(0.5+ncut++, 1);
    else continue;

    if( pfmu->track().isAvailable() ){
      if( (pfmu->track()->hitPattern().trackerLayersWithMeasurement() > 5) ) h_muon_selection->Fill(0.5+ncut++, 1);
      else continue;
    }
    else continue;

    if( pfmu->muonBestTrack().isAvailable() ){
      if( (fabs(pfmu->muonBestTrack()->dxy(vertex.position())) < 0.2) &&
	  (fabs(pfmu->muonBestTrack()->dz(vertex.position())) < 0.5) ) h_muon_selection->Fill(0.5+ncut++, 1);
      else continue;
    }
    else continue;

    numNoIsoMu++;

    if( miniAODhelper.GetMuonRelIso(*pfmu) < 0.120 ) h_muon_selection->Fill(0.5+ncut++, 1);
    else continue;
  }


  int numNoIsoEle=0;
  for( std::vector<pat::Electron>::const_iterator pfele = electrons->begin(); pfele!=electrons->end(); ++pfele ){
    int ncut = 0;
    h_electron_selection->Fill(0.5+ncut++, 1);

    if( (pfele->genLepton()) ){
      if( abs(pfele->genLepton()->pdgId())==11 ) h_electron_selection->Fill(0.5+ncut++, 1);
      else continue;
    }
    else continue;

    bool inCrack = false;
    if( pfele->superCluster().isAvailable() )
      inCrack = ( fabs(pfele->superCluster()->position().eta())>1.4442 && fabs(pfele->superCluster()->position().eta())<1.5660 );

    if( pfele->pt()>minTightLeptonPt && fabs(pfele->eta())<2.5 && !inCrack ) h_electron_selection->Fill(0.5+ncut++, 1);
    else continue;

    if( pfele->gsfTrack().isAvailable() ){
      if( pfele->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS)<=0 ) h_electron_selection->Fill(0.5+ncut++, 1);
      else continue;
    }
    else continue;

    if( pfele->passConversionVeto() ) h_electron_selection->Fill(0.5+ncut++, 1);
    else continue;

    if( pfele->gsfTrack().isAvailable() ){
      if( (fabs(pfele->gsfTrack()->dxy(vertex.position())) < 0.02) &&
	  (fabs(pfele->gsfTrack()->dz(vertex.position())) < 1.) ) h_electron_selection->Fill(0.5+ncut++, 1);
      else continue;
    }
    else continue;

    numNoIsoEle++;

    if( miniAODhelper.GetElectronRelIso(*pfele) < 0.100 ) h_electron_selection->Fill(0.5+ncut++, 1);
    else continue;
  }




  std::vector<pat::Jet> rawJets = miniAODhelper.GetUncorrectedJets(*pfjets);
  std::vector<pat::Jet> jetsNoMu = miniAODhelper.RemoveOverlaps(selectedMuons_loose, rawJets);
  std::vector<pat::Jet> jetsNoEle = miniAODhelper.RemoveOverlaps(selectedElectrons_loose, jetsNoMu);
  std::vector<pat::Jet> correctedJets = miniAODhelper.GetCorrectedJets(jetsNoEle, iEvent, iSetup);
  std::vector<pat::Jet> selectedJets_unsorted = miniAODhelper.GetSelectedJets(correctedJets, 30., 2.4, jetID::jetLoose, '-' );
  std::vector<pat::Jet> selectedJets_tag_unsorted = miniAODhelper.GetSelectedJets( correctedJets, 30., 2.4, jetID::jetLoose, 'M' );

  // Sort jet collections by pT
  std::vector<pat::Jet> selectedJets       = miniAODhelper.GetSortedByPt( selectedJets_unsorted );
  std::vector<pat::Jet> selectedJets_tag   = miniAODhelper.GetSortedByPt( selectedJets_tag_unsorted );


  // Get numJets, numTags
  int numJet = int( selectedJets.size() );
  int numTag = int( selectedJets_tag.size() );

  eve->numJets_ = numJet;
  eve->numTags_ = numTag;
         
  // Get Corrected MET
  pat::MET correctedMET = pfmet->front();//miniAODhelper.GetCorrectedMET( pfmets.at(0), pfJets_forMET, iSysType );


  //// Edit below here
  if( mcparticles.isValid() && false ){

    bool isHbb = false;

    TLorentzVector vecT(0,0,0,0);
    TLorentzVector vecTbar(0,0,0,0);
    TLorentzVector vecB(0,0,0,0);
    TLorentzVector vecBbar(0,0,0,0);
    TLorentzVector vecWp(0,0,0,0);
    TLorentzVector vecWm(0,0,0,0);
    TLorentzVector vecWpdau0(0,0,0,0);
    TLorentzVector vecWpdau1(0,0,0,0);
    TLorentzVector vecWmdau0(0,0,0,0);
    TLorentzVector vecWmdau1(0,0,0,0);
    TLorentzVector vecH(0,0,0,0);
    TLorentzVector vecHB(0,0,0,0);
    TLorentzVector vecHBbar(0,0,0,0);

    int numT = 0, numW = 0, numH = 0;

    for( size_t k = 0; k < mcparticles->size(); k++ ){
      const reco::Candidate & mcParticle = (*mcparticles)[k];

      int status = mcParticle.status();
      int pdgId  = mcParticle.pdgId();
      int absId  = abs( pdgId );
      int numdgt = mcParticle.numberOfDaughters();

      double pt     = mcParticle.pt();
      double eta    = mcParticle.eta();
      double phi    = mcParticle.phi();
      double energy = mcParticle.energy();
      double mass   = mcParticle.mass();

      int motherId = ( mcParticle.numberOfMothers()>0 ) ? mcParticle.mother(0)->pdgId() : -99;

      if( motherId==pdgId ){
	if( mcParticle.mother(0)->numberOfMothers()>0 ){
	  motherId = mcParticle.mother(0)->mother(0)->pdgId();
	  if( motherId==pdgId ){
	    if( mcParticle.mother(0)->mother(0)->numberOfMothers()>0 ){
	      motherId = mcParticle.mother(0)->mother(0)->mother(0)->pdgId();
	      if( motherId==pdgId ){
		if( mcParticle.mother(0)->mother(0)->mother(0)->numberOfMothers()>0 ){
		  motherId = mcParticle.mother(0)->mother(0)->mother(0)->mother(0)->pdgId();
		  if( motherId==pdgId ){
		    if( mcParticle.mother(0)->mother(0)->mother(0)->mother(0)->numberOfMothers()>0 ){
		      motherId = mcParticle.mother(0)->mother(0)->mother(0)->mother(0)->mother(0)->pdgId();
		    }
		  }
		}
	      }
	    }
	  }
	}
      }

      // if( absId==24 && numdgt==1 ){
      // printf(" iPar = %d,\t pdgId = %d,\t status = %d,\t numDgt = %d,\t pt = %.1f,\t eta = %.2f,\t phi = %.2f,\t mass = %.1f\n",
      //  	 int(k), pdgId, status, numdgt, pt, eta, phi, mass);
      // }

      // only concern yourself with status==3 particles
      if( !(absId==25 || absId==24 || absId==6) ) continue;

      if( numdgt<2 ) continue;

      if( insample_==8125 ){
	if( status!=3 ) continue;

	if( absId==6  )  numT++;
	if( absId==25 ) numH++;
	if( absId==24 && abs(motherId)==6 )  numW++;
      }
      if( insample_==2500 || insample_==9125 ){
	if( absId==6 && !(status==62) ) continue;
	if( absId==25 && !(status==62) ) continue;
	if( absId==24 && !(abs(motherId)==6) ) continue;

	if( absId==6 && status==62 )  numT++;
	if( absId==24 && abs(motherId)==6 )  numW++;
	if( absId==25 && status==62 ) numH++;
      }




      int d0=-99, d1=-99;
      if( numdgt>1 ){
	    
	int ind0 = 0;
	int ind1 = 1;

	if( numdgt>2 ){
	  if( mcParticle.daughter(0)->pdgId()==pdgId ){
	    ind0 = 1;
	    ind1 = 2;
	  }
	  if( mcParticle.daughter(1)->pdgId()==pdgId ){
	    ind0 = 0;
	    ind1 = 2;
	  }
	}

	d0 = mcParticle.daughter(ind0)->pdgId();
	d1 = mcParticle.daughter(ind1)->pdgId();

	// double daughter0Theta = kinem::theta(mcParticle.daughter(0)->eta());
	// double daughter0E = ( daughter0Theta!=0 ) ? mcParticle.daughter(0)->et()/sin( daughter0Theta ) : 0;
	double daughter0PT = mcParticle.daughter(ind0)->pt();
	double daughter0Eta = mcParticle.daughter(ind0)->eta();
	double daughter0Phi = mcParticle.daughter(ind0)->phi();
	double daughter0E   = mcParticle.daughter(ind0)->energy();


	// double daughter1Theta = kinem::theta(mcParticle.daughter(1)->eta());
	// double daughter1E = ( daughter1Theta!=0 ) ? mcParticle.daughter(1)->et()/sin( daughter1Theta ) : 0;
	double daughter1PT = mcParticle.daughter(ind1)->pt();
	double daughter1Eta = mcParticle.daughter(ind1)->eta();
	double daughter1Phi = mcParticle.daughter(ind1)->phi();
	double daughter1E   = mcParticle.daughter(ind1)->energy();

	if( pdgId==6 ){
	  vecT.SetPtEtaPhiE( pt, eta, phi, energy );
	  if( d0==5 )   vecB.SetPtEtaPhiE( daughter0PT, daughter0Eta, daughter0Phi, daughter0E );
	  if( d0==-5 )  vecBbar.SetPtEtaPhiE( daughter0PT, daughter0Eta, daughter0Phi, daughter0E );
	  if( d0==24 )  vecWp.SetPtEtaPhiE( daughter0PT, daughter0Eta, daughter0Phi, daughter0E );
	  if( d0==-24 ) vecWp.SetPtEtaPhiE( daughter0PT, daughter0Eta, daughter0Phi, daughter0E );
	  if( d1==5 )   vecB.SetPtEtaPhiE( daughter1PT, daughter1Eta, daughter1Phi, daughter1E );
	  if( d1==-5 )  vecBbar.SetPtEtaPhiE( daughter1PT, daughter1Eta, daughter1Phi, daughter1E );
	  if( d1==24 )  vecWp.SetPtEtaPhiE( daughter1PT, daughter1Eta, daughter1Phi, daughter1E );
	  if( d1==-24 ) vecWp.SetPtEtaPhiE( daughter1PT, daughter1Eta, daughter1Phi, daughter1E );
	}
	else if( pdgId==-6 ){
	  vecTbar.SetPtEtaPhiE( pt, eta, phi, energy );
	  if( d0==5 )   vecB.SetPtEtaPhiE( daughter0PT, daughter0Eta, daughter0Phi, daughter0E );
	  if( d0==-5 )  vecBbar.SetPtEtaPhiE( daughter0PT, daughter0Eta, daughter0Phi, daughter0E );
	  if( d0==24 )  vecWp.SetPtEtaPhiE( daughter0PT, daughter0Eta, daughter0Phi, daughter0E );
	  if( d0==-24 ) vecWp.SetPtEtaPhiE( daughter0PT, daughter0Eta, daughter0Phi, daughter0E );
	  if( d1==5 )   vecB.SetPtEtaPhiE( daughter1PT, daughter1Eta, daughter1Phi, daughter1E );
	  if( d1==-5 )  vecBbar.SetPtEtaPhiE( daughter1PT, daughter1Eta, daughter1Phi, daughter1E );
	  if( d1==24 )  vecWp.SetPtEtaPhiE( daughter1PT, daughter1Eta, daughter1Phi, daughter1E );
	  if( d1==-24 ) vecWp.SetPtEtaPhiE( daughter1PT, daughter1Eta, daughter1Phi, daughter1E );
	}
	else if( pdgId==24 && abs(d0)<6 && abs(d1)<6 ){
	  vecWpdau0.SetPtEtaPhiE( daughter0PT, daughter0Eta, daughter0Phi, daughter0E );
	  vecWpdau1.SetPtEtaPhiE( daughter1PT, daughter1Eta, daughter1Phi, daughter1E );
	}
	else if( pdgId==-24 && abs(d0)<6 && abs(d1)<6 ){
	  vecWmdau0.SetPtEtaPhiE( daughter0PT, daughter0Eta, daughter0Phi, daughter0E );
	  vecWmdau1.SetPtEtaPhiE( daughter1PT, daughter1Eta, daughter1Phi, daughter1E );
	}
	else if( pdgId==25 && abs(d0)==5 && abs(d1)==5 ){
	  vecH.SetPtEtaPhiE( pt, eta, phi, energy );
	  vecHB.SetPtEtaPhiE( daughter0PT, daughter0Eta, daughter0Phi, daughter0E );
	  vecHBbar.SetPtEtaPhiE( daughter1PT, daughter1Eta, daughter1Phi, daughter1E );
	}
      }


      int dau0 = abs(d0)-1;
      int dau1 = abs(d1)-1;
      if( absId==25 ){
	h_H_pt->Fill(std::min(pt,jetptmax)-0.0001,wgt);
	h_H_eta->Fill(eta,wgt);
	h_H_mass->Fill(std::min(mass,200.)-0.0001,wgt);
      }
      else if( absId==24 ){
	if( d0!=-99 && d1!=-99 ) h_Wdecay->Fill(dau0,dau1);
	h_W_pt->Fill(std::min(pt,jetptmax)-0.0001,wgt);
	h_W_eta->Fill(eta,wgt);
	h_W_mass->Fill(std::min(mass,200.)-0.0001,wgt);
      }
      else if( absId==6 ){
	h_top_pt->Fill(std::min(pt,jetptmax)-0.0001,wgt);
	h_top_eta->Fill(eta,wgt);
	h_top_mass->Fill(std::min(mass,200.)-0.0001,wgt);
      }


      if( absId==25 ){

	d0 = abs(d0);
	d1 = abs(d1);

	int binHdecay = -1;
	if( d0==5 && d1==5 )        binHdecay=0.5; // bb
	else if( d0==24 && d1==24 ) binHdecay=1.5; // WW
	else if( d0==15 && d1==15 ) binHdecay=2.5; // tau-tau
	else if( d0==21 && d1==21 ) binHdecay=3.5; // gg
	else if( d0==22 && d1==22 ) binHdecay=4.5; // gam-gam
	else if( d0==23 && d1==23 ) binHdecay=5.5; // ZZ
	else if( d0==4  && d1==4  ) binHdecay=6.5; // cc
	else if( (d0==23 && d1==22) || (d1==23 && d0==22) ) binHdecay=7.5; // Z-gam
	else if( d0==13 && d1==13 ) binHdecay=8.5; // mu-mu
	else if( d0==11 && d1==11 ) binHdecay=9.5; // e-e
	else if( d0==3 && d1==3 )   binHdecay=10.5; // ss
	else if( d0==2 && d1==2 )   binHdecay=11.5; // uu
	else if( d0==1 && d1==1 )   binHdecay=12.5; // dd
	else {                      binHdecay=13.5;
	  std::cout << "   Higgs decay:\t d0 = " << d0 << ", d1 = " << d1 << std::endl;
	}

	if( d0==5 && d1==5 ) isHbb = true;

	h_Hdecay->Fill(binHdecay);
      }

    }


    if( vecB.E()>0 && vecWpdau0.E()>0 && vecWpdau1.E()>0 ){
      TLorentzVector mytop = vecB + vecWpdau0 + vecWpdau1;
      h_top_Mbqq->Fill(mytop.M(),wgt);

      double maxDeltaRjt = std::max( vecB.DeltaR(mytop), std::max( vecWpdau0.DeltaR(mytop), vecWpdau1.DeltaR(mytop) ) );
      double maxDeltaRjj = std::max( vecWpdau0.DeltaR(vecWpdau1), std::max( vecB.DeltaR(vecWpdau0), vecB.DeltaR(vecWpdau1) ) );
      h_top_pt_maxDeltaRjj->Fill( mytop.Pt(), maxDeltaRjj, wgt );
      h_top_pt_maxDeltaRjt->Fill( mytop.Pt(), maxDeltaRjt, wgt );

      h_top_maxDeltaRjj->Fill( maxDeltaRjj, wgt );
      h_top_maxDeltaRjt->Fill( maxDeltaRjt, wgt );
    }

    if( vecBbar.E()>0 && vecWmdau0.E()>0 && vecWmdau1.E()>0 ){
      TLorentzVector mytop = vecBbar + vecWmdau0 + vecWmdau1;
      h_top_Mbbarqq->Fill(mytop.M(),wgt);

      double maxDeltaRjt = std::max( vecBbar.DeltaR(mytop), std::max( vecWmdau0.DeltaR(mytop), vecWmdau1.DeltaR(mytop) ) );
      double maxDeltaRjj = std::max( vecWmdau0.DeltaR(vecWmdau1), std::max( vecBbar.DeltaR(vecWmdau0), vecBbar.DeltaR(vecWmdau1) ) );
      h_top_pt_maxDeltaRjj->Fill( mytop.Pt(), maxDeltaRjj, wgt );
      h_top_pt_maxDeltaRjt->Fill( mytop.Pt(), maxDeltaRjt, wgt );
    }


    if( vecH.E() >0 && vecHB.E()>0 && vecHBbar.E()>0 ){
      h_H_pt_DeltaRbb->Fill(vecH.Pt(), vecHB.DeltaR(vecHBbar), wgt);
      TLorentzVector higgs = vecHB + vecHBbar;
      h_H_Mbb->Fill(higgs.M(), wgt);

      double maxDeltaRbH = std::max( vecHB.DeltaR(vecH), vecHBbar.DeltaR(vecH) );
      h_H_pt_maxDeltaRbH->Fill(vecH.Pt(), maxDeltaRbH, wgt);

      h_H_DeltaRbb->Fill(vecHB.DeltaR(vecHBbar), wgt);
      h_H_maxDeltaRbH->Fill(maxDeltaRbH, wgt);
    }


    bool dumpGen = false;
    if( insample_==2500 && !(numT==2 && numW==2) ) dumpGen = true;
    if( (insample_==8125 || insample_==9125) && !(numT==2 && numW>=2 && numH==1) ) dumpGen = true;

    if( dumpGen && false ){
      printf("\t Failed event: numT = %d, numW = %d, numH = %d \n", numT, numW, numH);
      for( size_t k = 0; k < mcparticles->size(); k++ ){
	const reco::Candidate & mcParticle = (*mcparticles)[k];

	int status = mcParticle.status();
	int pdgId  = mcParticle.pdgId();
	int numdgt = mcParticle.numberOfDaughters();
	int nummom = mcParticle.numberOfMothers();

	double pt     = mcParticle.pt();
	double eta    = mcParticle.eta();
	double phi    = mcParticle.phi();
	double mass   = mcParticle.mass();

	printf(" iPar = %d,\t pdgId = %d,\t status = %d,\t numDgt = %d,\t numMom = %d,\t pt = %.1f,\t eta = %.2f,\t phi = %.2f,\t mass = %.1f",
	       int(k), pdgId, status, numdgt, nummom, pt, eta, phi, mass);

	if( nummom>0 ) std::cout << ", momId = " << mcParticle.mother(0)->pdgId();
	      
	for( int i=0; i<numdgt; i++ ) std::cout << ", dau" << i << " = " << mcParticle.daughter(i)->pdgId();

	std::cout << " " << std::endl;

	if( pdgId==25 && status==62 && numdgt==1 ){
	  numHdecayToOneParticle++;
	  if( mcParticle.daughter(0)->pdgId()==21 ) numHdecayToOneGluon++;
	  if( mcParticle.daughter(0)->pdgId()==22 ) numHdecayToOnePhoton++;
	  if( mcParticle.daughter(0)->pdgId()==23 ) numHdecayToOneZBoson++;
	  if( mcParticle.daughter(0)->pdgId()==24 ) numHdecayToOneWBoson++;
	}
      }
    }

    if( (insample_==9125 || insample_==8125) && !isHbb ) return;

    if( isHbb ) numHbb++;
  }// end loop over MC info


  h_numJet->Fill(std::min(numJet,NjetMax),wgt);
  h_numTag->Fill(std::min(numTag,NtagMax),wgt);
  h_numPV->Fill(std::min(numpv,NpuMax),wgt);
  h_numTruePV->Fill(std::min(numTruePV,double(NpuMax)),wgt);

  h_numSecVtx->Fill(numSecVtx,wgt);


  // Loop over jets
  std::vector<double> vec_jet_pt;
  std::vector<double> vec_jet_eta;
  std::vector<double> vec_jet_phi;
  std::vector<double> vec_jet_csv;
  std::vector<int>    vec_jet_flavor;

  for( std::vector<pat::Jet>::const_iterator iJet = selectedJets.begin(); iJet != selectedJets.end(); iJet++ ){ 
    double csv = iJet->bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags");
    if( csv>-5 && csv<-0.5 ) csv = -0.2;
    if( csv<-5 )             csv = -0.4;

    vec_jet_pt.push_back(iJet->pt());
    vec_jet_eta.push_back(iJet->eta());
    vec_jet_phi.push_back(iJet->phi());
    vec_jet_csv.push_back(iJet->bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags"));
    vec_jet_flavor.push_back(iJet->partonFlavour());
  } // end loop on jets

  eve->jet_pt_  = vec_jet_pt;
  eve->jet_eta_ = vec_jet_eta;
  eve->jet_phi_ = vec_jet_phi;
  eve->jet_csv_ = vec_jet_csv;
  eve->jet_flavor_ = vec_jet_flavor;




  std::vector<double> vec_ele_pt;
  std::vector<double> vec_ele_eta;
  std::vector<double> vec_ele_phi;
  std::vector<double> vec_ele_relIso;
  std::vector<double> vec_ele_scSigmaIEtaIEta;
  std::vector<double> vec_ele_hadronicOverEm;
  std::vector<double> vec_ele_relEcalIso;
  std::vector<double> vec_ele_relHcalIso;
  std::vector<double> vec_ele_relTrackIso;
  std::vector<double> vec_ele_OneOESuperMinusOneOP;
  std::vector<int> vec_ele_numMissingHits;
  std::vector<int> vec_ele_isEB;
  std::vector<int> vec_ele_genMatch;
  std::vector<int> vec_ele_tightId;
  std::vector<int> vec_ele_tightHLTId;
  std::vector<int> vec_ele_matchL1Iso;
  std::vector<int> vec_ele_matchHLT;

  std::vector<int> vec_ele_matchHLT_hltL1sL1EG25erHTT125;
  std::vector<int> vec_ele_matchHLT_hltNewEG27EtFilter;
  std::vector<int> vec_ele_matchHLT_hltNewEle27WP85ClusterShapeFilter;
  std::vector<int> vec_ele_matchHLT_hltNewEle27WP85HEFilter;
  std::vector<int> vec_ele_matchHLT_hltNewEle27WP85EcalIsoFilter;
  std::vector<int> vec_ele_matchHLT_hltNewEle27WP85HcalIsoFilter;
  std::vector<int> vec_ele_matchHLT_hltNewEle27WP85PixelMatchFilter;
  std::vector<int> vec_ele_matchHLT_hltNewEle27WP85GsfOneOEMinusOneOPFilter;
  std::vector<int> vec_ele_matchHLT_hltNewEle27WP85GsfMissingHitsFilter;
  std::vector<int> vec_ele_matchHLT_hltNewEle27WP85GsfDetaFilter;
  std::vector<int> vec_ele_matchHLT_hltNewEle27WP85GsfDphiFilter;
  std::vector<int> vec_ele_matchHLT_hltNewEle27WP85GsfTrackIsoFilter;

  for( std::vector<pat::Electron>::const_iterator pfele = electrons->begin(); pfele!=electrons->end(); ++pfele ){

    bool inCrack = false;
    if( pfele->superCluster().isAvailable() )
      inCrack = ( fabs(pfele->superCluster()->position().eta())>1.4442 && fabs(pfele->superCluster()->position().eta())<1.5660 );

    if( !(pfele->pt()>10. && fabs(pfele->eta())<2.5 && !inCrack) ) continue;

    vec_ele_pt.push_back(pfele->pt());
    vec_ele_eta.push_back(pfele->eta());
    vec_ele_phi.push_back(pfele->phi());
    vec_ele_relIso.push_back(miniAODhelper.GetElectronRelIso(*pfele));

    vec_ele_scSigmaIEtaIEta.push_back( pfele->scSigmaIEtaIEta() );
    vec_ele_hadronicOverEm.push_back( pfele->hadronicOverEm() );

    int genMatch = 0;
    if( (pfele->genLepton()) ){
      if( abs(pfele->genLepton()->pdgId())==11 ) genMatch = 1;
    }
    vec_ele_genMatch.push_back(genMatch);


    bool no_exp_inner_trkr_hits = false;
    if( pfele->gsfTrack().isAvailable() ){
      if( pfele->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS)<=0 ) no_exp_inner_trkr_hits = true;
    }

    bool notConv = ( pfele->passConversionVeto() ) ? true : false;

    bool d02 = false;
    bool dZ  = false;
    if( pfele->gsfTrack().isAvailable() ){
      if( (fabs(pfele->gsfTrack()->dxy(vertex.position())) < 0.02) ) d02 = true;
      if( (fabs(pfele->gsfTrack()->dz(vertex.position())) < 1.) )    dZ  = true;
    }

    bool passesId = ( d02 && dZ && notConv && no_exp_inner_trkr_hits );
    int tightId = ( passesId ) ? 1 : 0;

    vec_ele_tightId.push_back( tightId );


    bool passHLTId = false;
    double OneOESuperMinusOneOP = -1;
    int numMissingHits = 99;
    int isEB = -1;
    double relEcalIso = 99, relHcalIso = 99, relTrackIso = 99;
    if( (pfele->superCluster().isAvailable()) && (pfele->gsfTrack().isAvailable()) ){
      double SCenergy = pfele->superCluster()->energy();
      double SCeta = pfele->superCluster()->position().eta();
      double absSCeta = fabs( SCeta );
      double SCet = SCenergy * sin (2*atan(exp(-SCeta))); 

      double TrkP = pfele->gsfTrack()->p();
      numMissingHits = pfele->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS);

      OneOESuperMinusOneOP = fabs( 1/SCenergy - 1/TrkP );

      relEcalIso = pfele->ecalIso()/SCet;
      relHcalIso = pfele->hcalIso()/SCet;
      relTrackIso = pfele->trackIso()/SCet;

      if( absSCeta < 1.479 ){
	isEB = 1;
	passHLTId = ( pfele->scSigmaIEtaIEta() <= 0.011 &&
		      pfele->hadronicOverEm() <= 0.15 &&
		      pfele->ecalIso()/SCet <= 0.16 &&
		      pfele->hcalIso()/SCet <= 0.20 &&
		      pfele->trackIso()/SCet <= 0.05 &&
		      OneOESuperMinusOneOP <= 0.012 &&
		      numMissingHits <= 999
		      );
      }
      else{
	isEB = 0;
	passHLTId = ( pfele->scSigmaIEtaIEta() <= 0.033 &&
		      pfele->hadronicOverEm() <= 0.20 &&
		      pfele->ecalIso()/SCet <= 0.12 &&
		      pfele->hcalIso()/SCet <= 0.30 &&
		      pfele->trackIso()/SCet <= 0.05 &&
		      OneOESuperMinusOneOP <= 0.009 &&
		      numMissingHits <= 1
		      );
      }
    }

    int tightHLTId = ( passHLTId ) ? 1 : 0;
    vec_ele_tightHLTId.push_back( tightHLTId );

    vec_ele_isEB.push_back( isEB );

    vec_ele_OneOESuperMinusOneOP.push_back( OneOESuperMinusOneOP );
    vec_ele_numMissingHits.push_back( numMissingHits );

    vec_ele_relEcalIso.push_back( relEcalIso );
    vec_ele_relHcalIso.push_back( relHcalIso );
    vec_ele_relTrackIso.push_back( relTrackIso );


    bool matchesL1Iso = false;
    for( int iL1Obj = 0; iL1Obj < int(triggerObjects_L1SingleIsoEG.size()); iL1Obj++ ){ 
      double deltaR = reco::deltaR( pfele->eta(), pfele->phi(), triggerObjects_L1SingleIsoEG[iL1Obj].Eta(), triggerObjects_L1SingleIsoEG[iL1Obj].Phi() );

      if( triggerObjects_L1SingleIsoEG[iL1Obj].Pt()<25 ) continue;
      if( fabs( triggerObjects_L1SingleIsoEG[iL1Obj].Eta() ) > 2.12 ) continue;

      if( deltaR<0.5 ) matchesL1Iso = true;
    }

    bool matchesHLT = false;
    bool matchesHLT_hltL1sL1EG25erHTT125 = false;
    bool matchesHLT_hltNewEG27EtFilter = false;
    bool matchesHLT_hltNewEle27WP85ClusterShapeFilter = false;
    bool matchesHLT_hltNewEle27WP85HEFilter = false;
    bool matchesHLT_hltNewEle27WP85EcalIsoFilter = false;
    bool matchesHLT_hltNewEle27WP85HcalIsoFilter = false;
    bool matchesHLT_hltNewEle27WP85PixelMatchFilter = false;
    bool matchesHLT_hltNewEle27WP85GsfOneOEMinusOneOPFilter = false;
    bool matchesHLT_hltNewEle27WP85GsfMissingHitsFilter = false;
    bool matchesHLT_hltNewEle27WP85GsfDetaFilter = false;
    bool matchesHLT_hltNewEle27WP85GsfDphiFilter = false;
    bool matchesHLT_hltNewEle27WP85GsfTrackIsoFilter = false;

    for( int iHLTObj = 0; iHLTObj < int(triggerObjects_hltEle.size()); iHLTObj++ ){ 
      std::string name = vec_hltEle27WP85Gsf_filter[iHLTObj];
      double deltaR = reco::deltaR( pfele->eta(), pfele->phi(), triggerObjects_hltEle[iHLTObj].Eta(), triggerObjects_hltEle[iHLTObj].Phi() );
      if( deltaR<0.5 ){
	if( name=="hltL1sL1EG25erHTT125::reHLT" ) matchesHLT_hltL1sL1EG25erHTT125 = true;
	if( name=="hltNewEG27EtFilter::reHLT" ) matchesHLT_hltNewEG27EtFilter = true;
	if( name=="hltNewEle27WP85ClusterShapeFilter::reHLT" ) matchesHLT_hltNewEle27WP85ClusterShapeFilter = true;
	if( name=="hltNewEle27WP85HEFilter::reHLT" ) matchesHLT_hltNewEle27WP85HEFilter = true;
	if( name=="hltNewEle27WP85EcalIsoFilter::reHLT" ) matchesHLT_hltNewEle27WP85EcalIsoFilter = true;
	if( name=="hltNewEle27WP85HcalIsoFilter::reHLT" ) matchesHLT_hltNewEle27WP85HcalIsoFilter = true;
	if( name=="hltNewEle27WP85PixelMatchFilter::reHLT" ) matchesHLT_hltNewEle27WP85PixelMatchFilter = true;
	if( name=="hltNewEle27WP85GsfOneOEMinusOneOPFilter::reHLT" ) matchesHLT_hltNewEle27WP85GsfOneOEMinusOneOPFilter = true;
	if( name=="hltNewEle27WP85GsfMissingHitsFilter::reHLT" ) matchesHLT_hltNewEle27WP85GsfMissingHitsFilter = true;
	if( name=="hltNewEle27WP85GsfDetaFilter::reHLT" ) matchesHLT_hltNewEle27WP85GsfDetaFilter = true;
	if( name=="hltNewEle27WP85GsfDphiFilter::reHLT" ) matchesHLT_hltNewEle27WP85GsfDphiFilter = true;
	if( name=="hltNewEle27WP85GsfTrackIsoFilter::reHLT" ) matchesHLT_hltNewEle27WP85GsfTrackIsoFilter = true;

	if( name=="hltNewEle27WP85GsfTrackIsoFilter::reHLT" ) matchesHLT = true;
      }
    }

    vec_ele_matchL1Iso.push_back( ((matchesL1Iso) ? 1 : 0) );
    vec_ele_matchHLT.push_back( ((matchesHLT) ? 1 : 0) );

    vec_ele_matchHLT_hltL1sL1EG25erHTT125.push_back( ((matchesHLT_hltL1sL1EG25erHTT125) ? 1 : 0) );
    vec_ele_matchHLT_hltNewEG27EtFilter.push_back( ((matchesHLT_hltNewEG27EtFilter) ? 1 : 0) );
    vec_ele_matchHLT_hltNewEle27WP85ClusterShapeFilter.push_back( ((matchesHLT_hltNewEle27WP85ClusterShapeFilter) ? 1 : 0) );
    vec_ele_matchHLT_hltNewEle27WP85HEFilter.push_back( ((matchesHLT_hltNewEle27WP85HEFilter) ? 1 : 0) );
    vec_ele_matchHLT_hltNewEle27WP85EcalIsoFilter.push_back( ((matchesHLT_hltNewEle27WP85EcalIsoFilter) ? 1 : 0) );
    vec_ele_matchHLT_hltNewEle27WP85HcalIsoFilter.push_back( ((matchesHLT_hltNewEle27WP85HcalIsoFilter) ? 1 : 0) );
    vec_ele_matchHLT_hltNewEle27WP85PixelMatchFilter.push_back( ((matchesHLT_hltNewEle27WP85PixelMatchFilter) ? 1 : 0) );
    vec_ele_matchHLT_hltNewEle27WP85GsfOneOEMinusOneOPFilter.push_back( ((matchesHLT_hltNewEle27WP85GsfOneOEMinusOneOPFilter) ? 1 : 0) );
    vec_ele_matchHLT_hltNewEle27WP85GsfMissingHitsFilter.push_back( ((matchesHLT_hltNewEle27WP85GsfMissingHitsFilter) ? 1 : 0) );
    vec_ele_matchHLT_hltNewEle27WP85GsfDetaFilter.push_back( ((matchesHLT_hltNewEle27WP85GsfDetaFilter) ? 1 : 0) );
    vec_ele_matchHLT_hltNewEle27WP85GsfDphiFilter.push_back( ((matchesHLT_hltNewEle27WP85GsfDphiFilter) ? 1 : 0) );
    vec_ele_matchHLT_hltNewEle27WP85GsfTrackIsoFilter.push_back( ((matchesHLT_hltNewEle27WP85GsfTrackIsoFilter) ? 1 : 0) );
  }

  eve->ele_pt_ = vec_ele_pt;
  eve->ele_eta_ = vec_ele_eta;
  eve->ele_phi_ = vec_ele_phi;
  eve->ele_relIso_ = vec_ele_relIso;
  eve->ele_scSigmaIEtaIEta_ = vec_ele_scSigmaIEtaIEta;
  eve->ele_hadronicOverEm_ = vec_ele_hadronicOverEm;
  eve->ele_relEcalIso_ = vec_ele_relEcalIso;
  eve->ele_relHcalIso_ = vec_ele_relHcalIso;
  eve->ele_relTrackIso_ = vec_ele_relTrackIso;
  eve->ele_OneOESuperMinusOneOP_ = vec_ele_OneOESuperMinusOneOP;
  eve->ele_numMissingHits_ = vec_ele_numMissingHits;
  eve->ele_isEB_ = vec_ele_isEB;
  eve->ele_genMatch_ = vec_ele_genMatch;
  eve->ele_tightId_ = vec_ele_tightId;
  eve->ele_tightHLTId_ = vec_ele_tightHLTId;
  eve->ele_matchL1Iso_ = vec_ele_matchL1Iso;
  eve->ele_matchHLT_ = vec_ele_matchHLT;

  eve->ele_matchHLT_hltL1sL1EG25erHTT125_ = vec_ele_matchHLT_hltL1sL1EG25erHTT125;
  eve->ele_matchHLT_hltNewEG27EtFilter_ = vec_ele_matchHLT_hltNewEG27EtFilter;
  eve->ele_matchHLT_hltNewEle27WP85ClusterShapeFilter_ = vec_ele_matchHLT_hltNewEle27WP85ClusterShapeFilter;
  eve->ele_matchHLT_hltNewEle27WP85HEFilter_ = vec_ele_matchHLT_hltNewEle27WP85HEFilter;
  eve->ele_matchHLT_hltNewEle27WP85EcalIsoFilter_ = vec_ele_matchHLT_hltNewEle27WP85EcalIsoFilter;
  eve->ele_matchHLT_hltNewEle27WP85HcalIsoFilter_ = vec_ele_matchHLT_hltNewEle27WP85HcalIsoFilter;
  eve->ele_matchHLT_hltNewEle27WP85PixelMatchFilter_ = vec_ele_matchHLT_hltNewEle27WP85PixelMatchFilter;
  eve->ele_matchHLT_hltNewEle27WP85GsfOneOEMinusOneOPFilter_ = vec_ele_matchHLT_hltNewEle27WP85GsfOneOEMinusOneOPFilter;
  eve->ele_matchHLT_hltNewEle27WP85GsfMissingHitsFilter_ = vec_ele_matchHLT_hltNewEle27WP85GsfMissingHitsFilter;
  eve->ele_matchHLT_hltNewEle27WP85GsfDetaFilter_ = vec_ele_matchHLT_hltNewEle27WP85GsfDetaFilter;
  eve->ele_matchHLT_hltNewEle27WP85GsfDphiFilter_ = vec_ele_matchHLT_hltNewEle27WP85GsfDphiFilter;
  eve->ele_matchHLT_hltNewEle27WP85GsfTrackIsoFilter_ = vec_ele_matchHLT_hltNewEle27WP85GsfTrackIsoFilter;


  m_ttree->Fill();

  // Require at least 4 regular jets and tags
  if( !(numJet>=4 && numTag>=2) ) return;

  int this_category = -1;
  if( numJet==4 && numTag==2) this_category=1;
  if( numJet==5 && numTag==2) this_category=2;
  if( numJet>=6 && numTag==2) this_category=3;	
  if( numJet==4 && numTag==3) this_category=4;
  if( numJet==5 && numTag==3) this_category=5;
  if( numJet>=6 && numTag==3) this_category=6;
  if( numJet==4 && numTag>=4) this_category=7;
  if( numJet==5 && numTag>=4) this_category=8;
  if( numJet>=6 && numTag>=4) this_category=9;


  h_category_yield->Fill(0.5,wgt);
  h_category_yield->Fill(this_category,wgt);

  if( (numNoIsoMu+numNoIsoEle)>0 ){
    h_category_yield_1l_noiso->Fill(0.5,wgt);
    h_category_yield_1l_noiso->Fill(this_category,wgt);

    if( numTightElectrons==1 ){
      h_category_yield_1e_noiso->Fill(0.5,wgt);
      h_category_yield_1e_noiso->Fill(this_category,wgt);
    } 
    else if( numTightMuons==1 ){
      h_category_yield_1m_noiso->Fill(0.5,wgt);
      h_category_yield_1m_noiso->Fill(this_category,wgt);
    } 
  }

  // Require event has one good lepton
  if( !oneGoodLepton ) return;

  h_category_yield_1l->Fill(0.5,wgt);
  h_category_yield_1l->Fill(this_category,wgt);

  if( numTightElectrons==1 ){
    h_category_yield_1e->Fill(0.5,wgt);
    h_category_yield_1e->Fill(this_category,wgt);
  } 
  else if( numTightMuons==1 ){
    h_category_yield_1m->Fill(0.5,wgt);
    h_category_yield_1m->Fill(this_category,wgt);
  } 


}


// ------------ method called once each job just before starting event loop  ------------
void 
TTHTriggerAnalyzer::beginJob()
{

  TH1::SetDefaultSumw2(true);

  numEvents_         = 0;
  numHbb = 0;
  numHdecayToOneParticle = 0;
  numHdecayToOneGluon = 0;
  numHdecayToOnePhoton = 0;
  numHdecayToOneZBoson = 0;
  numHdecayToOneWBoson = 0;

}

// ------------ method called once each job just after ending the event loop  ------------
void 
TTHTriggerAnalyzer::endJob() 
{
  // report on triggers fired
  if( dumpHLT_ || true ){
    std::cout << "***************************************************************" << std::endl;
    std::cout << "  Summary for L1T Algo: Total number of events = " << numEvents_ << std::endl;
    int bit=0;
    for( std::map<std::string, int>::const_iterator iter = l1talgo_cppath_.begin(); iter != l1talgo_cppath_.end(); iter++){
      std::string name = iter->first;
      double eff = 100*double(iter->second)/double(numEvents_);
      printf("\t %d,\t %s,\t %d,\t %.1f \n",bit++, name.c_str(), iter->second, eff);
    }
    std::cout << "***************************************************************" << std::endl;
    std::cout << "      Trigger menu = " << hlt_config_.tableName() << std::endl;
    std::cout << "  Summary for HLT: Total number of events = " << numEvents_ << std::endl;
    for( std::map<std::string, int>::const_iterator iter = hlt_cppath_.begin(); iter != hlt_cppath_.end(); iter++){
      std::string name = iter->first;
      double eff = 100*double(iter->second)/double(numEvents_);
      printf("\t %s,\t %d,\t %.1f \n",name.c_str(), iter->second, eff);
    }
    std::cout << "***************************************************************" << std::endl;
    std::cout << "  Summary for Filters: Total number of events = " << numEvents_ << std::endl;
    for( std::map<std::string, int>::const_iterator iter = flt_cppath_.begin(); iter != flt_cppath_.end(); iter++){
      std::string name = iter->first;
      double eff = 100*double(iter->second)/double(numEvents_);
      printf("\t %s,\t %d,\t %.1f \n",name.c_str(), iter->second, eff);
    }
    std::cout << "***************************************************************" << std::endl;
  }

  std::cout << "***************************************************************" << std::endl;
  std::cout << "  Total number of events = " << numEvents_ << std::endl;
  std::cout << "***************************************************************" << std::endl;


}

// ------------ method called when starting to processes a run  ------------
/*
void 
TTHTriggerAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
void 
TTHTriggerAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{

  bool hltchanged = true;
  if (!hlt_config_.init(iRun, iSetup, "reHLT", hltchanged)) {
    std::cout << "Warning, didn't find trigger process HLT,\t" << "reHLT" << std::endl;
    return;
  }
  bool filterchanged = true;
  if (!filter_config_.init(iRun, iSetup, filterTag, filterchanged)) {
    std::cout << "Warning, didn't find filter process HLT,\t" << filterTag << std::endl;
    return;
  }

  // Zero out map
  std::vector<std::string> triggerNames = hlt_config_.triggerNames();
  std::vector<std::string> filterNames  = filter_config_.triggerNames();

  hlt_triggerNames_.clear();
  flt_filterNames_.clear();

  hlt_triggerNames_.push_back("All");
  std::string prefix = "HLT_";
  for( unsigned int iPath=0; iPath<triggerNames.size(); iPath++ ){
    std::string name = triggerNames[iPath];
    std::string pathNameNoVer = hlt_config_.removeVersion(name);
    hlt_cppath_[pathNameNoVer] = 0;
    if( name.compare(0, prefix.length(), prefix) == 0 ) hlt_triggerNames_.push_back(pathNameNoVer);
  }

  flt_filterNames_.push_back("All");
  for( unsigned int iPath=0; iPath<filterNames.size(); iPath++ ){
    std::string name = filterNames[iPath];
    std::string pathNameNoVer = filter_config_.removeVersion(name);
    flt_cppath_[pathNameNoVer] = 0;
    flt_filterNames_.push_back(pathNameNoVer);
  }

  unsigned int numHLT = hlt_triggerNames_.size();
  unsigned int numFLT = flt_filterNames_.size();

  h_hlt = fs_->make<TH1D>("h_hlt",";HLT path", numHLT , 0 , numHLT );
  h_flt = fs_->make<TH1D>("h_flt",";Filter path", numFLT , 0 , numFLT );

  for( unsigned int iPath=0; iPath<numHLT; iPath++ ){
    std::string pathNameNoVer = hlt_triggerNames_[iPath];
    int jPath = iPath+1;
    if( h_hlt ){
      TAxis * axis = h_hlt->GetXaxis();
      if( axis ) axis->SetBinLabel(jPath, pathNameNoVer.c_str());
    }
  }

  for( unsigned int iPath=0; iPath<numFLT; iPath++ ){
    std::string pathNameNoVer = flt_filterNames_[iPath];
    int jPath = iPath+1;
    if( h_flt ){
      TAxis * axis = h_flt->GetXaxis();
      if( axis ) axis->SetBinLabel(jPath, pathNameNoVer.c_str());
    }
  }

  // Get Trigger and Event Handles
  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  const L1GtTriggerMenu* menu = menuRcd.product();

  // L1 algos
  for( CItAlgo algo = menu->gtAlgorithmMap().begin(); algo!=menu->gtAlgorithmMap().end(); ++algo) {
    std::string name = (algo->second).algoName();
    l1talgo_cppath_[name] = 0;
  }

}

// ------------ method called when ending the processing of a run  ------------
/*
void 
TTHTriggerAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
TTHTriggerAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
TTHTriggerAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TTHTriggerAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TTHTriggerAnalyzer);
