// -*- C++ -*-
//
// Package:    ttH-LeptonPlusJets/L1TriggerRate
// Class:      L1TriggerRate
// 
/**\class L1TriggerRate L1TriggerRate.cc ttH-LeptonPlusJets/AnalysisCode/plugins/L1TriggerRate.cc

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
#include "TH2.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"


//
// class declaration
//

class L1TriggerRate : public edm::EDAnalyzer {
   public:
      explicit L1TriggerRate(const edm::ParameterSet&);
      ~L1TriggerRate();

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

  edm::EDGetTokenT <l1extra::L1EmParticleCollection> l1IsoEmToken;
  edm::EDGetTokenT <l1extra::L1EmParticleCollection> l1NonIsoEmToken;
  edm::EDGetTokenT <l1extra::L1JetParticleCollection> l1CenJetToken;
  edm::EDGetTokenT <l1extra::L1JetParticleCollection> l1ForJetToken;
  edm::EDGetTokenT <l1extra::L1JetParticleCollection> l1TauJetToken;
  edm::EDGetTokenT <l1extra::L1MuonParticleCollection> l1MuToken;
  edm::EDGetTokenT <l1extra::L1EtMissParticleCollection> l1HTMissToken;
  edm::EDGetTokenT <std::vector< PileupSummaryInfo > > puInfoToken;

  int numEvents_;
  int numEvents_PassL1TWithPrescale_;
  int numEvents_L1EG20_;
  int numEvents_L1EG25_;
  int numEvents_L1EG20er_;
  int numEvents_L1EG25er_;
  int numEvents_L1IsoEG30_;
  int numEvents_L1IsoEG30er_;

  int numEvents_L1IsoEG20erTauJet20er_;

  int numEvents_2L1CenJet28_;
  int numEvents_3L1CenJet28_;
  int numEvents_4L1CenJet28_;

  int numEvents_L1EG20_2L1CenJet28_;
  int numEvents_L1EG20_3L1CenJet28_;
  int numEvents_L1EG20_4L1CenJet28_;

  int numEvents_L1EG20er_2L1CenJet28_;
  int numEvents_L1EG20er_3L1CenJet28_;
  int numEvents_L1EG20er_4L1CenJet28_;

  int numEvents_L1EG25_2L1CenJet28_;
  int numEvents_L1EG25_3L1CenJet28_;
  int numEvents_L1EG25_4L1CenJet28_;

  int numEvents_L1EG25er_2L1CenJet28_;
  int numEvents_L1EG25er_3L1CenJet28_;
  int numEvents_L1EG25er_4L1CenJet28_;

  int numEvents_L1EG25er_L1HTT100_;
  int numEvents_L1EG25er_L1HTT110_;
  int numEvents_L1EG25er_L1HTT120_;
  int numEvents_L1EG25er_L1HTT125_;
  int numEvents_L1EG25er_L1HTT130_;
  int numEvents_L1EG25er_L1HTT140_;
  int numEvents_L1EG25er_L1HTT150_;
  int numEvents_L1EG25er_L1HTT160_;

  int numEvents_L1EG25er_3L1CenJet32_28_28_;
  int numEvents_L1EG25er_3L1CenJet32_32_28_;

  int numEvents_L1EG25er_3L1CenJet36_28_28_;
  int numEvents_L1EG25er_3L1CenJet36_32_28_;
  int numEvents_L1EG25er_3L1CenJet36_36_28_;

  int numEvents_L1EG25er_3L1CenJet40_28_28_;
  int numEvents_L1EG25er_3L1CenJet40_32_28_;
  int numEvents_L1EG25er_3L1CenJet40_36_28_;
  int numEvents_L1EG25er_3L1CenJet40_40_28_;



  int numEvents_FailL1TofInterest_L1EG20_2L1CenJet28_;
  int numEvents_FailL1TofInterest_L1EG20_3L1CenJet28_;
  int numEvents_FailL1TofInterest_L1EG20_4L1CenJet28_;

  int numEvents_FailL1TofInterest_L1EG20er_2L1CenJet28_;
  int numEvents_FailL1TofInterest_L1EG20er_3L1CenJet28_;
  int numEvents_FailL1TofInterest_L1EG20er_4L1CenJet28_;

  int numEvents_FailL1TofInterest_L1EG25_2L1CenJet28_;
  int numEvents_FailL1TofInterest_L1EG25_3L1CenJet28_;
  int numEvents_FailL1TofInterest_L1EG25_4L1CenJet28_;

  int numEvents_FailL1TofInterest_L1EG25er_2L1CenJet28_;
  int numEvents_FailL1TofInterest_L1EG25er_3L1CenJet28_;
  int numEvents_FailL1TofInterest_L1EG25er_4L1CenJet28_;

  // 
  int numEvents_FailL1TWithPrescale_L1IsoEG30er_;

  int numEvents_FailL1TWithPrescale_L1EG20_2L1CenJet28_;
  int numEvents_FailL1TWithPrescale_L1EG20_3L1CenJet28_;
  int numEvents_FailL1TWithPrescale_L1EG20_4L1CenJet28_;

  int numEvents_FailL1TWithPrescale_L1EG20er_2L1CenJet28_;
  int numEvents_FailL1TWithPrescale_L1EG20er_3L1CenJet28_;
  int numEvents_FailL1TWithPrescale_L1EG20er_4L1CenJet28_;

  int numEvents_FailL1TWithPrescale_L1EG25_2L1CenJet28_;
  int numEvents_FailL1TWithPrescale_L1EG25_3L1CenJet28_;
  int numEvents_FailL1TWithPrescale_L1EG25_4L1CenJet28_;

  int numEvents_FailL1TWithPrescale_L1EG25er_2L1CenJet28_;
  int numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet28_;
  int numEvents_FailL1TWithPrescale_L1EG25er_4L1CenJet28_;



  int numEvents_FailL1TWithPrescale_L1EG25er_L1HTT100_;
  int numEvents_FailL1TWithPrescale_L1EG25er_L1HTT110_;
  int numEvents_FailL1TWithPrescale_L1EG25er_L1HTT120_;
  int numEvents_FailL1TWithPrescale_L1EG25er_L1HTT125_;
  int numEvents_FailL1TWithPrescale_L1EG25er_L1HTT130_;
  int numEvents_FailL1TWithPrescale_L1EG25er_L1HTT140_;
  int numEvents_FailL1TWithPrescale_L1EG25er_L1HTT150_;
  int numEvents_FailL1TWithPrescale_L1EG25er_L1HTT160_;


  int numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet32_28_28_;
  int numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet32_32_28_;

  int numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_28_28_;
  int numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_32_28_;
  int numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_36_28_;

  int numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_28_28_;
  int numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_32_28_;
  int numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_36_28_;
  int numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_40_28_;

  int numEvents_FailL1TWithNoPrescale_L1IsoEG30er_;



  int numEvents_RemoveEGJetOverlap_L1EG20_2L1CenJet28_;
  int numEvents_RemoveEGJetOverlap_L1EG20_3L1CenJet28_;
  int numEvents_RemoveEGJetOverlap_L1EG20_4L1CenJet28_;

  int numEvents_RemoveEGJetOverlap_L1EG20er_2L1CenJet28_;
  int numEvents_RemoveEGJetOverlap_L1EG20er_3L1CenJet28_;
  int numEvents_RemoveEGJetOverlap_L1EG20er_4L1CenJet28_;

  int numEvents_RemoveEGJetOverlap_L1EG25_2L1CenJet28_;
  int numEvents_RemoveEGJetOverlap_L1EG25_3L1CenJet28_;
  int numEvents_RemoveEGJetOverlap_L1EG25_4L1CenJet28_;

  int numEvents_RemoveEGJetOverlap_L1EG25er_2L1CenJet28_;
  int numEvents_RemoveEGJetOverlap_L1EG25er_3L1CenJet28_;
  int numEvents_RemoveEGJetOverlap_L1EG25er_4L1CenJet28_;



  int numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_2L1CenJet28_;
  int numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_3L1CenJet28_;
  int numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_4L1CenJet28_;

  int numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_2L1CenJet28_;
  int numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_3L1CenJet28_;
  int numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_4L1CenJet28_;

  int numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_2L1CenJet28_;
  int numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_3L1CenJet28_;
  int numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_4L1CenJet28_;

  int numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_2L1CenJet28_;
  int numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_3L1CenJet28_;
  int numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_4L1CenJet28_;



  std::vector<std::string> l1t_of_interest_;

  std::map<std::string, int> l1talgo_cppath_reHLT_;
  std::map<std::string, int> l1talgo_cppath_prescale_reHLT_;
  std::map<std::string, int> l1talgo_cppath_prescale_perevent_reHLT_;
  std::map<std::string, int> l1talgo_cppath_pure_prescale_reHLT_;
  std::map<std::string, int> l1talgo_initial_prescale_reHLT_;
  std::map<std::string, int> l1talgo_prescale_reHLT_;


  edm::Service<TFileService> fs_;


  double minEGEt_;
  double maxEGEt_;
  int numEGEtBins_;

  double min3rdCenJetEt_;
  double max3rdCenJetEt_;
  int num3rdCenJetEtBins_;

  // Declare histograms
  TH1D *h_L1IsoEG_et;
  TH1D *h_L1IsoEGer_et;
  TH1D *h_L1CenJet_et;

  TH1D *h_L1_HTT;
  TH1D *h_L1_HTT_SingleEG25er;

  TH1D *h_L1_HTT_integral;
  TH1D *h_L1_HTT_SingleEG25er_integral;

  TH1D *h_L1_HTT_integral_rate;
  TH1D *h_L1_HTT_SingleEG25er_integral_rate;

  TH1D *h_L1_HTT_pure;
  TH1D *h_L1_HTT_SingleEG25er_pure;

  TH1D *h_L1_HTT_integral_pure;
  TH1D *h_L1_HTT_SingleEG25er_integral_pure;

  TH1D *h_L1_HTT_integral_pure_rate;
  TH1D *h_L1_HTT_SingleEG25er_integral_pure_rate;

  TH1D *h_first_L1CenJet_et;
  TH1D *h_second_L1CenJet_et;
  TH1D *h_third_L1CenJet_et;
  TH1D *h_fourth_L1CenJet_et;

  TH1D *h_first_L1CenJet_et_hasL1EG25er;
  TH1D *h_second_L1CenJet_et_hasL1EG25er;
  TH1D *h_third_L1CenJet_et_hasL1EG25er;
  TH1D *h_fourth_L1CenJet_et_hasL1EG25er;

  TH1D *h_first_L1CenJet_et_has3L1CenJet;
  TH1D *h_second_L1CenJet_et_has3L1CenJet;
  TH1D *h_third_L1CenJet_et_has3L1CenJet;
  TH1D *h_fourth_L1CenJet_et_has3L1CenJet;

  TH1D *h_first_L1CenJet_et_hasL1EG25er_has3L1CenJet;
  TH1D *h_second_L1CenJet_et_hasL1EG25er_has3L1CenJet;
  TH1D *h_third_L1CenJet_et_hasL1EG25er_has3L1CenJet;
  TH1D *h_fourth_L1CenJet_et_hasL1EG25er_has3L1CenJet;


  TH2D* h_L1EGEt_L1CenJet3rdJetEt;
  TH2D* h_L1EGEt_L1CenJet3rdJetEt_rate;

  TH2D* h_L1EGEt_L1CenJet3rdJetEt_pure;
  TH2D* h_L1EGEt_L1CenJet3rdJetEt_pure_rate;


  TH2D* h_L1EG20Et_L1CenJet28Et;

  TH1D* h_deltaR_L1EG20_L1CenJet28;


  TH1D *h_numGenPV;
  TH1D *h_numGenPV_L1EG25erHTT125;
  TH1D *h_numGenPV_L1EG25erHTT125_pure;
  TH1D *h_numGenPV_L1EG25erHTT125_rate;
  TH1D *h_numGenPV_L1EG25erHTT125_pure_rate;

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
L1TriggerRate::L1TriggerRate(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  gtReadoutRecordToken = consumes <L1GlobalTriggerReadoutRecord> (edm::InputTag(std::string("simGtDigis"), std::string(""), std::string("L1TEMULATION")));

  l1IsoEmToken = consumes <l1extra::L1EmParticleCollection> (edm::InputTag(std::string("l1ExtraLayer2"), std::string("Isolated")));
  l1NonIsoEmToken = consumes <l1extra::L1EmParticleCollection> (edm::InputTag(std::string("l1ExtraLayer2"), std::string("NonIsolated")));
  l1CenJetToken = consumes <l1extra::L1JetParticleCollection> (edm::InputTag(std::string("l1ExtraLayer2"), std::string("Central")));
  l1ForJetToken = consumes <l1extra::L1JetParticleCollection> (edm::InputTag(std::string("l1ExtraLayer2"), std::string("Forward")));
  l1TauJetToken = consumes <l1extra::L1JetParticleCollection> (edm::InputTag(std::string("l1ExtraLayer2"), std::string("Tau")));
  l1MuToken = consumes <l1extra::L1MuonParticleCollection> (edm::InputTag(std::string("l1ExtraLayer2"), std::string("")));
  l1HTMissToken = consumes <l1extra::L1EtMissParticleCollection> (edm::InputTag(std::string("l1ExtraLayer2"), std::string("MHT")));

  puInfoToken = consumes <std::vector< PileupSummaryInfo > > (edm::InputTag(std::string("addPileupInfo")));


  h_L1IsoEG_et = fs_->make<TH1D>("h_L1IsoEG_et",";EG E_{T}", 64, 0, 64 );
  h_L1IsoEGer_et = fs_->make<TH1D>("h_L1IsoEGer_et",";EG E_{T}", 64, 0, 64 );

  h_L1CenJet_et = fs_->make<TH1D>("h_L1CenJet_et",";L1CenJet E_{T}", 270, 0, 270 );
  h_first_L1CenJet_et = fs_->make<TH1D>("h_first_L1CenJet_et",";first L1CenJet E_{T}", 270, 0, 270 );
  h_second_L1CenJet_et = fs_->make<TH1D>("h_second_L1CenJet_et",";second L1CenJet E_{T}", 270, 0, 270 );
  h_third_L1CenJet_et = fs_->make<TH1D>("h_third_L1CenJet_et",";third L1CenJet E_{T}", 270, 0, 270 );
  h_fourth_L1CenJet_et = fs_->make<TH1D>("h_fourth_L1CenJet_et",";fourth L1CenJet E_{T}", 270, 0, 270 );

  h_first_L1CenJet_et_hasL1EG25er = fs_->make<TH1D>("h_first_L1CenJet_et_hasL1EG25er",";first L1CenJet E_{T}", 270, 0, 270 );
  h_second_L1CenJet_et_hasL1EG25er = fs_->make<TH1D>("h_second_L1CenJet_et_hasL1EG25er",";second L1CenJet E_{T}", 270, 0, 270 );
  h_third_L1CenJet_et_hasL1EG25er = fs_->make<TH1D>("h_third_L1CenJet_et_hasL1EG25er",";third L1CenJet E_{T}", 270, 0, 270 );
  h_fourth_L1CenJet_et_hasL1EG25er = fs_->make<TH1D>("h_fourth_L1CenJet_et_hasL1EG25er",";fourth L1CenJet E_{T}", 270, 0, 270 );

  h_first_L1CenJet_et_has3L1CenJet = fs_->make<TH1D>("h_first_L1CenJet_et_has3L1CenJet",";first L1CenJet E_{T}", 270, 0, 270 );
  h_second_L1CenJet_et_has3L1CenJet = fs_->make<TH1D>("h_second_L1CenJet_et_has3L1CenJet",";second L1CenJet E_{T}", 270, 0, 270 );
  h_third_L1CenJet_et_has3L1CenJet = fs_->make<TH1D>("h_third_L1CenJet_et_has3L1CenJet",";third L1CenJet E_{T}", 270, 0, 270 );
  h_fourth_L1CenJet_et_has3L1CenJet = fs_->make<TH1D>("h_fourth_L1CenJet_et_has3L1CenJet",";fourth L1CenJet E_{T}", 270, 0, 270 );

  h_first_L1CenJet_et_hasL1EG25er_has3L1CenJet = fs_->make<TH1D>("h_first_L1CenJet_et_hasL1EG25er_has3L1CenJet",";first L1CenJet E_{T}", 270, 0, 270 );
  h_second_L1CenJet_et_hasL1EG25er_has3L1CenJet = fs_->make<TH1D>("h_second_L1CenJet_et_hasL1EG25er_has3L1CenJet",";second L1CenJet E_{T}", 270, 0, 270 );
  h_third_L1CenJet_et_hasL1EG25er_has3L1CenJet = fs_->make<TH1D>("h_third_L1CenJet_et_hasL1EG25er_has3L1CenJet",";third L1CenJet E_{T}", 270, 0, 270 );
  h_fourth_L1CenJet_et_hasL1EG25er_has3L1CenJet = fs_->make<TH1D>("h_fourth_L1CenJet_et_hasL1EG25er_has3L1CenJet",";fourth L1CenJet E_{T}", 270, 0, 270 );


  int numHTbins = 300;
  double HTmax = 300.;
  h_L1_HTT = fs_->make<TH1D>("h_L1_HTT",";L1 H_{T}", numHTbins, 0, HTmax );
  h_L1_HTT_SingleEG25er = fs_->make<TH1D>("h_L1_HTT_SingleEG25er",";L1 H_{T}", numHTbins, 0, HTmax );

  h_L1_HTT_integral = fs_->make<TH1D>("h_L1_HTT_integral",";L1 H_{T} > X", numHTbins, 0, HTmax );
  h_L1_HTT_SingleEG25er_integral = fs_->make<TH1D>("h_L1_HTT_SingleEG25er_integral",";L1 H_{T} > X", numHTbins, 0, HTmax );

  h_L1_HTT_integral_rate = fs_->make<TH1D>("h_L1_HTT_integral_rate",";L1 H_{T} > X", numHTbins, 0, HTmax );
  h_L1_HTT_SingleEG25er_integral_rate = fs_->make<TH1D>("h_L1_HTT_SingleEG25er_integral_rate",";L1 H_{T} > X", numHTbins, 0, HTmax );

  h_L1_HTT_pure = fs_->make<TH1D>("h_L1_HTT_pure",";L1 H_{T}", numHTbins, 0, HTmax );
  h_L1_HTT_SingleEG25er_pure = fs_->make<TH1D>("h_L1_HTT_SingleEG25er_pure",";L1 H_{T}", numHTbins, 0, HTmax );

  h_L1_HTT_integral_pure = fs_->make<TH1D>("h_L1_HTT_integral_pure",";L1 H_{T} > X", numHTbins, 0, HTmax );
  h_L1_HTT_SingleEG25er_integral_pure = fs_->make<TH1D>("h_L1_HTT_SingleEG25er_integral_pure",";L1 H_{T} > X", numHTbins, 0, HTmax );

  h_L1_HTT_integral_pure_rate = fs_->make<TH1D>("h_L1_HTT_integral_pure_rate",";L1 H_{T} > X", numHTbins, 0, HTmax );
  h_L1_HTT_SingleEG25er_integral_pure_rate = fs_->make<TH1D>("h_L1_HTT_SingleEG25er_integral_pure_rate",";L1 H_{T} > X", numHTbins, 0, HTmax );



  minEGEt_ = 18;
  maxEGEt_ = 35;
  numEGEtBins_ = int(maxEGEt_ - minEGEt_ + 0.0001 );

  min3rdCenJetEt_ = 25;
  max3rdCenJetEt_ = 51;
  num3rdCenJetEtBins_ = int(max3rdCenJetEt_ - min3rdCenJetEt_ + 0.0001 );

  h_L1EGEt_L1CenJet3rdJetEt = fs_->make<TH2D>("h_L1EGEt_L1CenJet3rdJetEt",";3^{rd} L1CenJet E_{T} threshold;L1EG E_{T} threshold", num3rdCenJetEtBins_, min3rdCenJetEt_, max3rdCenJetEt_, numEGEtBins_, minEGEt_, maxEGEt_ );
  h_L1EGEt_L1CenJet3rdJetEt_rate = fs_->make<TH2D>("h_L1EGEt_L1CenJet3rdJetEt_rate",";3^{rd} L1CenJet E_{T} threshold;L1EG E_{T} threshold", num3rdCenJetEtBins_, min3rdCenJetEt_, max3rdCenJetEt_, numEGEtBins_, minEGEt_, maxEGEt_ );

  h_L1EGEt_L1CenJet3rdJetEt_pure = fs_->make<TH2D>("h_L1EGEt_L1CenJet3rdJetEt_pure",";3^{rd} L1CenJet E_{T} threshold;L1EG E_{T} threshold", num3rdCenJetEtBins_, min3rdCenJetEt_, max3rdCenJetEt_, numEGEtBins_, minEGEt_, maxEGEt_ );
  h_L1EGEt_L1CenJet3rdJetEt_pure_rate = fs_->make<TH2D>("h_L1EGEt_L1CenJet3rdJetEt_pure_rate",";3^{rd} L1CenJet E_{T} threshold;L1EG E_{T} threshold", num3rdCenJetEtBins_, min3rdCenJetEt_, max3rdCenJetEt_, numEGEtBins_, minEGEt_, maxEGEt_ );


  h_L1EG20Et_L1CenJet28Et = fs_->make<TH2D>("h_L1EG20Et_L1CenJet28Et",";L1CenJet E_{T};L1EG E_{T}", 280, 20, 300, 45, 15, 60 );

  h_deltaR_L1EG20_L1CenJet28 = fs_->make<TH1D>("h_deltaR_L1EG20_L1CenJet28",";#DeltaR(L1EG20,L1CenJet28)", 61, 0, 6.1 );


  h_numGenPV = fs_->make<TH1D>("h_numGenPV",";Number of generated PU", 60, 0, 60 );
  h_numGenPV_L1EG25erHTT125 = fs_->make<TH1D>("h_numGenPV_L1EG25erHTT125",";Number of generated PU", 60, 0, 60 );
  h_numGenPV_L1EG25erHTT125_pure = fs_->make<TH1D>("h_numGenPV_L1EG25erHTT125_pure",";Number of generated PU", 60, 0, 60 );
  h_numGenPV_L1EG25erHTT125_rate = fs_->make<TH1D>("h_numGenPV_L1EG25erHTT125_rate",";Number of generated PU", 60, 0, 60 );
  h_numGenPV_L1EG25erHTT125_pure_rate = fs_->make<TH1D>("h_numGenPV_L1EG25erHTT125_pure_rate",";Number of generated PU", 60, 0, 60 );


  l1t_of_interest_.push_back("L1_SingleEG35er");
  l1t_of_interest_.push_back("L1_SingleIsoEG30er");
  l1t_of_interest_.push_back("L1_SingleJet200");
  l1t_of_interest_.push_back("L1_SingleJet240");
  l1t_of_interest_.push_back("L1_DoubleJetC100");
  l1t_of_interest_.push_back("L1_DoubleJetC112");
  l1t_of_interest_.push_back("L1_DoubleJetC120");
  l1t_of_interest_.push_back("L1_QuadJetC60");
  l1t_of_interest_.push_back("L1_QuadJetC84");
  l1t_of_interest_.push_back("L1_HTT175");
  l1t_of_interest_.push_back("L1_HTT200");
  l1t_of_interest_.push_back("L1_HTT250");


}


L1TriggerRate::~L1TriggerRate()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
L1TriggerRate::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  numEvents_++;

  edm::Handle<L1GlobalTriggerReadoutRecord> gtReadoutRecord;
  iEvent.getByToken(gtReadoutRecordToken, gtReadoutRecord);

  edm::Handle<l1extra::L1EmParticleCollection> l1IsoEms;
  iEvent.getByToken(l1IsoEmToken,l1IsoEms);

  edm::Handle<l1extra::L1EmParticleCollection> l1NonIsoEms;
  iEvent.getByToken(l1NonIsoEmToken,l1NonIsoEms);

  edm::Handle<l1extra::L1JetParticleCollection> l1CenJets;
  iEvent.getByToken(l1CenJetToken,l1CenJets);

  edm::Handle<l1extra::L1JetParticleCollection> l1ForJets;
  iEvent.getByToken(l1ForJetToken,l1ForJets);

  edm::Handle<l1extra::L1JetParticleCollection> l1TauJets;
  iEvent.getByToken(l1TauJetToken,l1TauJets);

  edm::Handle<l1extra::L1MuonParticleCollection> l1Mus;
  iEvent.getByToken(l1MuToken,l1Mus);

  edm::Handle<l1extra::L1EtMissParticleCollection> l1HTs;
  iEvent.getByToken(l1HTMissToken,l1HTs);

  edm::Handle<std::vector< PileupSummaryInfo > > PupInfo;
  iEvent.getByToken(puInfoToken,PupInfo);



  double numGenPV = -1;
  if( (PupInfo.isValid()) ){
    for( std::vector<PileupSummaryInfo>::const_iterator PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI ) {
      int BX = PVI->getBunchCrossing();
      if( BX==0 ) numGenPV = PVI->getPU_NumInteractions();
    }
  }

  h_numGenPV->Fill(numGenPV);


  bool passL1_of_interest = false;
  // Get Trigger and Event Handles
  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  const L1GtTriggerMenu* menu = menuRcd.product();

  bool passL1_with_prescale = false;
  bool passL1_with_prescale_ignoreIsoEG30er = false;

  //bool passL1_noPrescaleL1 = false;
  bool passL1_noPrescaleL1_ignoreIsoEG30er = false;

  bool pass_L1EG25erHTT125 = false;

  for( std::map<std::string, int>::const_iterator iter = l1talgo_cppath_prescale_perevent_reHLT_.begin(); iter != l1talgo_cppath_prescale_perevent_reHLT_.end(); iter++){
    std::string name = iter->first;
    l1talgo_cppath_prescale_perevent_reHLT_[name] = 0;
  }

  if( gtReadoutRecord.isValid() ){
    const DecisionWord& gtDecisionWord = gtReadoutRecord->decisionWord();

    // L1 algos
    for( CItAlgo algo = menu->gtAlgorithmMap().begin(); algo!=menu->gtAlgorithmMap().end(); ++algo) {
      int algoBitNumber = (algo->second).algoBitNumber();

      std::string name = (algo->second).algoName();
      bool accept = gtDecisionWord.at(algoBitNumber);
      int initial_prescale = l1talgo_initial_prescale_reHLT_[name];
      int prescale = l1talgo_prescale_reHLT_[name];

      if( accept ) l1talgo_cppath_reHLT_[name]+=1;

      if( accept && name=="L1_SingleEG25er_HTT125" ) pass_L1EG25erHTT125 = true;

      if( accept ){
	if( initial_prescale>0 ){
	  if( prescale==1 ){
	    l1talgo_cppath_prescale_reHLT_[name]+=1;

	    if( !(name=="L1_IsoEG20er_TauJet20er" ||
		  name=="L1_SingleEG20er_TripleJetC28" ||
		  name=="L1_SingleEG25er_TripleJetC28") ){
	      if( name!="L1_SingleEG25er_HTT125" ) passL1_with_prescale = true;
	      l1talgo_cppath_prescale_perevent_reHLT_[name]+=1;
	      if( name!="L1_SingleIsoEG30er" ) passL1_with_prescale_ignoreIsoEG30er = true;

	      if( initial_prescale==1 ){
		//passL1_noPrescaleL1 = true;
		if( name!="L1_SingleIsoEG30er" ) passL1_noPrescaleL1_ignoreIsoEG30er = true;
	      }
	    }
	  }
	  if( initial_prescale>1 ){
	    if( prescale==1 ) l1talgo_prescale_reHLT_[name] = initial_prescale;
	    else              l1talgo_prescale_reHLT_[name]--;
	  }
	}
      }
      if( false ) std::cout << " =====>  L1T algo: path name = " << (algo->second).algoName() << ",\t prescale = " << initial_prescale << ",\t pass = " << gtDecisionWord.at(algoBitNumber) << std::endl; 


      // if( name=="L1_SingleEG5" ){
      // 	std::cout << "\t Trigger = L1_SingleEG5,\t initial prescale = " << initial_prescale << ",\t current prescale = " << prescale << std::endl;
      // }


      if( accept ){
	for( int iAlgo = 0; iAlgo<int(l1t_of_interest_.size()); iAlgo++ ){
	  if( name == l1t_of_interest_[iAlgo] ){
	    passL1_of_interest = true;
	    break;
	  }
	}
      }
    }
  }

  if( passL1_with_prescale ) numEvents_PassL1TWithPrescale_++;


  for( std::map<std::string, int>::const_iterator iter = l1talgo_cppath_pure_prescale_reHLT_.begin(); iter != l1talgo_cppath_pure_prescale_reHLT_.end(); iter++){
    std::string name = iter->first;

    bool passesAnotherL1T = false;
    for( std::map<std::string, int>::const_iterator iter2 = l1talgo_cppath_prescale_perevent_reHLT_.begin(); iter2 != l1talgo_cppath_prescale_perevent_reHLT_.end(); iter2++){
      std::string name2 = iter2->first;
      if( name2==name ) continue;
      if( iter2->second > 0 && l1talgo_initial_prescale_reHLT_[name2]>0 ){
	passesAnotherL1T = true;
	break;
      }
    }
    if( !passesAnotherL1T && l1talgo_cppath_prescale_perevent_reHLT_[name]>0 ) l1talgo_cppath_pure_prescale_reHLT_[name]+=1;
  }


  /////////
  ///
  /// L1 objects
  ///
  ////////
  bool pass_L1_SingleEG35er = false;
  bool pass_L1_SingleIsoEG30er = false;
  bool pass_L1_SingleJet200 = false;
  bool pass_L1_SingleJet240 = false;
  bool pass_L1_DoubleJetC100 = false;
  bool pass_L1_DoubleJetC112 = false;
  bool pass_L1_DoubleJetC120 = false;
  bool pass_L1_QuadJetC60 = false;
  bool pass_L1_QuadJetC84 = false;



  bool hasL1EG20 = false;
  bool hasL1EG25 = false;
  bool hasL1EG20er = false;
  bool hasL1EG25er = false;
  bool hasL1IsoEG30 = false;
  bool hasL1IsoEG30er = false;

  double maxEGer = -1;

  std::vector<TLorentzVector> l1EG10s;
  std::vector<TLorentzVector> l1EG20s;
  std::vector<TLorentzVector> l1EG25s;
  std::vector<TLorentzVector> l1EG20ers;
  std::vector<TLorentzVector> l1EG25ers;

  TLorentzVector max_L1IsoEGer_p4;

  if( l1IsoEms.isValid() ){
    for( l1extra::L1EmParticleCollection::const_iterator l1IsoEm = l1IsoEms->begin(); l1IsoEm!=l1IsoEms->end(); ++l1IsoEm ){
      double et = l1IsoEm->et();
      double eta = l1IsoEm->eta();


      bool isER = false;
      if( fabs(eta)<=2.17 ) isER = true;

      if( isER ){
	if( et>35 ) pass_L1_SingleEG35er = true;
	if( et>30 ) pass_L1_SingleIsoEG30er = true;
      }

      h_L1IsoEG_et->Fill(et);
      if( isER ) h_L1IsoEGer_et->Fill(et);

      if( et>=20 )         hasL1EG20 = true;
      if( et>=25 )         hasL1EG25 = true;
      if( et>=30 )         hasL1IsoEG30 = true;

      if( isER ){
	if( et>maxEGer ){
	  maxEGer = et;
	  max_L1IsoEGer_p4.SetPxPyPzE(l1IsoEm->px(),l1IsoEm->py(),l1IsoEm->pz(),l1IsoEm->energy());
	}

	if( et>=30 ) hasL1IsoEG30er = true;
	if( et>=20 ) hasL1EG20er = true;
	if( et>=25 ) hasL1EG25er = true;
      }


      TLorentzVector eg;
      eg.SetPxPyPzE(l1IsoEm->px(),l1IsoEm->py(),l1IsoEm->pz(),l1IsoEm->energy());

      if( et>=10 ) l1EG10s.push_back(eg);
      if( et>=20 ) l1EG20s.push_back(eg);
      if( et>=25 ) l1EG25s.push_back(eg);
      if( isER ){
	if( et>=20 ) l1EG20ers.push_back(eg);
	if( et>=25 ) l1EG25ers.push_back(eg);
      }
    }
  }

  if( l1NonIsoEms.isValid() ){
    for( l1extra::L1EmParticleCollection::const_iterator l1NonIsoEm = l1NonIsoEms->begin(); l1NonIsoEm!=l1NonIsoEms->end(); ++l1NonIsoEm ){
      double et = l1NonIsoEm->et();
      double eta = l1NonIsoEm->eta();

      bool isER = false;
      if( fabs(eta)<=2.17 ) isER = true;

      if( isER ){
	if( et>35 ) pass_L1_SingleEG35er = true;
      }

      if( et>=20 ) hasL1EG20 = true;
      if( et>=25 ) hasL1EG25 = true;

      if( isER ){
	if( et>maxEGer ) maxEGer = et;

	if( et>=20 ) hasL1EG20er = true;
	if( et>=25 ) hasL1EG25er = true;
      }

      TLorentzVector eg;
      eg.SetPxPyPzE(l1NonIsoEm->px(),l1NonIsoEm->py(),l1NonIsoEm->pz(),l1NonIsoEm->energy());

      if( et>=10 ) l1EG10s.push_back(eg);
      if( et>=20 ) l1EG20s.push_back(eg);
      if( et>=25 ) l1EG25s.push_back(eg);
      if( isER ){
	if( et>=20 ) l1EG20ers.push_back(eg);
	if( et>=25 ) l1EG25ers.push_back(eg);
      }
    }
  }

  double hasL1IsoEG20erTauJet20er = false;
  if( l1TauJets.isValid() ){
    for( l1extra::L1JetParticleCollection::const_iterator l1Tau = l1TauJets->begin(); l1Tau!=l1TauJets->end(); ++l1Tau ){
      double et = l1Tau->et();
      double eta = l1Tau->eta();

      if( fabs(eta)>2.17 ) continue;
      if( et<20 ) continue;

      for( int iEG = 0; iEG < int(l1EG20ers.size()); iEG++ ){
	if( l1EG20ers[iEG].Pt()<20 ) continue;
	if( fabs(l1EG20ers[iEG].Eta()-eta) < 2 ) continue;
	hasL1IsoEG20erTauJet20er = true;
      }
    }
  }

  passL1_with_prescale = ( passL1_with_prescale || hasL1IsoEG20erTauJet20er );
  passL1_with_prescale_ignoreIsoEG30er = ( passL1_with_prescale_ignoreIsoEG30er || hasL1IsoEG20erTauJet20er );


  double l1ht = -1;
  if( l1HTs.isValid() ){
    for( l1extra::L1EtMissParticleCollection::const_iterator il1HT = l1HTs->begin(); il1HT!=l1HTs->end(); ++il1HT ){
      l1ht = il1HT->etTotal();
    }
  }

  h_L1_HTT->Fill( l1ht );
  if( hasL1EG25er ) h_L1_HTT_SingleEG25er->Fill( l1ht );

  if( !passL1_with_prescale ){
    h_L1_HTT_pure->Fill( l1ht );
    if( hasL1EG25er ) h_L1_HTT_SingleEG25er_pure->Fill( l1ht );
  }


  if( hasL1EG20 ) numEvents_L1EG20_++;
  if( hasL1EG25 ) numEvents_L1EG25_++;
  if( hasL1IsoEG30 ) numEvents_L1IsoEG30_++;
  if( hasL1IsoEG30er ) numEvents_L1IsoEG30er_++;
  if( hasL1IsoEG30er && !passL1_with_prescale_ignoreIsoEG30er ) numEvents_FailL1TWithPrescale_L1IsoEG30er_++;
  if( hasL1IsoEG30er && !passL1_noPrescaleL1_ignoreIsoEG30er )  numEvents_FailL1TWithNoPrescale_L1IsoEG30er_++;
  if( hasL1EG20er ) numEvents_L1EG20er_++;
  if( hasL1EG25er ) numEvents_L1EG25er_++;
  if( hasL1IsoEG20erTauJet20er ) numEvents_L1IsoEG20erTauJet20er_++;


  int numL1CenJets30 = 0;
  int num_L1_JetC100 = 0;
  int num_L1_JetC112 = 0;
  int num_L1_JetC120 = 0;
  int num_L1_JetC60 = 0;
  int num_L1_JetC84 = 0;

  int numL1CenJets30NoOverlapL1EG20 = 0;
  int numL1CenJets30NoOverlapL1EG25 = 0;
  int numL1CenJets30NoOverlapL1EG20er = 0;
  int numL1CenJets30NoOverlapL1EG25er = 0;

  std::vector<double> l1cenjetEt;
  int numL1CenJets = 0;

  double first_l1cenjet_et = -1;
  double second_l1cenjet_et = -1;
  double third_l1cenjet_et = -1;

  if( l1CenJets.isValid() ){
    for( l1extra::L1JetParticleCollection::const_iterator il1CenJet = l1CenJets->begin(); il1CenJet!=l1CenJets->end(); ++il1CenJet ){
      numL1CenJets++;

      double et = il1CenJet->et();

      h_L1CenJet_et->Fill( et );

      if( numL1CenJets==1 ) h_first_L1CenJet_et->Fill( et );
      if( numL1CenJets==2 ) h_second_L1CenJet_et->Fill( et );
      if( numL1CenJets==3 ) h_third_L1CenJet_et->Fill( et );
      if( numL1CenJets==4 ) h_fourth_L1CenJet_et->Fill( et );

      if( numL1CenJets==1 ) first_l1cenjet_et = et;
      if( numL1CenJets==2 ) second_l1cenjet_et = et;
      if( numL1CenJets==3 ) third_l1cenjet_et = et;

      if( hasL1EG25er ){
	if( numL1CenJets==1 ) h_first_L1CenJet_et_hasL1EG25er->Fill( et );
	if( numL1CenJets==2 ) h_second_L1CenJet_et_hasL1EG25er->Fill( et );
	if( numL1CenJets==3 ) h_third_L1CenJet_et_hasL1EG25er->Fill( et );
	if( numL1CenJets==4 ) h_fourth_L1CenJet_et_hasL1EG25er->Fill( et );
      }

      if( l1CenJets->size()>=3 ){
	if( numL1CenJets==1 ) h_first_L1CenJet_et_has3L1CenJet->Fill( et );
	if( numL1CenJets==2 ) h_second_L1CenJet_et_has3L1CenJet->Fill( et );
	if( numL1CenJets==3 ) h_third_L1CenJet_et_has3L1CenJet->Fill( et );
	if( numL1CenJets==4 ) h_fourth_L1CenJet_et_has3L1CenJet->Fill( et );
      }

      if( hasL1EG25er && l1CenJets->size()>=3 ){
	if( numL1CenJets==1 ) h_first_L1CenJet_et_hasL1EG25er_has3L1CenJet->Fill( et );
	if( numL1CenJets==2 ) h_second_L1CenJet_et_hasL1EG25er_has3L1CenJet->Fill( et );
	if( numL1CenJets==3 ) h_third_L1CenJet_et_hasL1EG25er_has3L1CenJet->Fill( et );
	if( numL1CenJets==4 ) h_fourth_L1CenJet_et_hasL1EG25er_has3L1CenJet->Fill( et );
      }


      if( et>=28 ){
	numL1CenJets30++;

	TLorentzVector jet;
	jet.SetPxPyPzE(il1CenJet->px(),il1CenJet->py(),il1CenJet->pz(),il1CenJet->energy());

	bool matchedEG20 = false;
	bool matchedEG25 = false;
	bool matchedEG20er = false;
	bool matchedEG25er = false;

	for( int iEG=0; iEG<int(l1EG20s.size()); iEG++ ){
	  double deltaR = jet.DeltaR(l1EG20s[iEG]);
	  h_deltaR_L1EG20_L1CenJet28->Fill(deltaR);

	  if( deltaR < 0.3 ){
	    h_L1EG20Et_L1CenJet28Et->Fill(jet.Et(),l1EG20s[iEG].Et());
	    matchedEG20 = true;
	    break;
	  }
	}
	for( int iEG=0; iEG<int(l1EG25s.size()); iEG++ ){
	  double deltaR = jet.DeltaR(l1EG25s[iEG]);
	  if( deltaR< 0.3 ){
	    matchedEG25 = true;
	    break;
	  }
	}
	for( int iEG=0; iEG<int(l1EG20ers.size()); iEG++ ){
	  double deltaR = jet.DeltaR(l1EG20ers[iEG]);
	  if( deltaR< 0.3 ){
	    matchedEG20er = true;
	    break;
	  }
	}
	for( int iEG=0; iEG<int(l1EG25ers.size()); iEG++ ){
	  double deltaR = jet.DeltaR(l1EG25ers[iEG]);
	  if( deltaR< 0.3 ){
	    matchedEG25er = true;
	    break;
	  }
	}


	if( !matchedEG20 ) numL1CenJets30NoOverlapL1EG20++;
	if( !matchedEG25 ) numL1CenJets30NoOverlapL1EG25++;
	if( !matchedEG20er ) numL1CenJets30NoOverlapL1EG20er++;
	if( !matchedEG25er ) numL1CenJets30NoOverlapL1EG25er++;
      }
	

      if( et>200 ) pass_L1_SingleJet200 = true;
      if( et>240 ) pass_L1_SingleJet240 = true;

      if( et>100 ) num_L1_JetC100++;
      if( et>112 ) num_L1_JetC112++;
      if( et>120 ) num_L1_JetC120++;

      if( et>60 ) num_L1_JetC60++;
      if( et>84 ) num_L1_JetC84++;

      l1cenjetEt.push_back(et);
    }
  }



  if( num_L1_JetC100>=2 ) pass_L1_DoubleJetC100 = true;
  if( num_L1_JetC112>=2 ) pass_L1_DoubleJetC112 = true;
  if( num_L1_JetC120>=2 ) pass_L1_DoubleJetC120 = true;

  if( num_L1_JetC60>=2 ) pass_L1_QuadJetC60 = true;
  if( num_L1_JetC84>=2 ) pass_L1_QuadJetC84 = true;


  if( numL1CenJets30>=2 ) numEvents_2L1CenJet28_++;
  if( numL1CenJets30>=3 ) numEvents_3L1CenJet28_++;
  if( numL1CenJets30>=4 ) numEvents_4L1CenJet28_++;


  if( numL1CenJets30>=2 && hasL1EG20 ) numEvents_L1EG20_2L1CenJet28_++;
  if( numL1CenJets30>=3 && hasL1EG20 ) numEvents_L1EG20_3L1CenJet28_++;
  if( numL1CenJets30>=4 && hasL1EG20 ) numEvents_L1EG20_4L1CenJet28_++;

  if( numL1CenJets30>=2 && hasL1EG20er ) numEvents_L1EG20er_2L1CenJet28_++;
  if( numL1CenJets30>=3 && hasL1EG20er ) numEvents_L1EG20er_3L1CenJet28_++;
  if( numL1CenJets30>=4 && hasL1EG20er ) numEvents_L1EG20er_4L1CenJet28_++;

  if( numL1CenJets30>=2 && hasL1EG25 ) numEvents_L1EG25_2L1CenJet28_++;
  if( numL1CenJets30>=3 && hasL1EG25 ) numEvents_L1EG25_3L1CenJet28_++;
  if( numL1CenJets30>=4 && hasL1EG25 ) numEvents_L1EG25_4L1CenJet28_++;

  if( numL1CenJets30>=2 && hasL1EG25er ) numEvents_L1EG25er_2L1CenJet28_++;
  if( numL1CenJets30>=3 && hasL1EG25er ) numEvents_L1EG25er_3L1CenJet28_++;
  if( numL1CenJets30>=4 && hasL1EG25er ) numEvents_L1EG25er_4L1CenJet28_++;


  if( hasL1EG25er ){
    if( l1ht > 100 ) numEvents_L1EG25er_L1HTT100_++;
    if( l1ht > 110 ) numEvents_L1EG25er_L1HTT110_++;
    if( l1ht > 120 ) numEvents_L1EG25er_L1HTT120_++;
    if( l1ht > 125 ) numEvents_L1EG25er_L1HTT125_++;
    if( l1ht > 130 ) numEvents_L1EG25er_L1HTT130_++;
    if( l1ht > 140 ) numEvents_L1EG25er_L1HTT140_++;
    if( l1ht > 150 ) numEvents_L1EG25er_L1HTT150_++;
    if( l1ht > 160 ) numEvents_L1EG25er_L1HTT160_++;
  }


  if( hasL1EG25er ){
    if( (first_l1cenjet_et  >= 32) &&
	(second_l1cenjet_et >= 28) &&
	(third_l1cenjet_et  >= 28)  ) numEvents_L1EG25er_3L1CenJet32_28_28_++;
    if( (first_l1cenjet_et  >= 32) &&
	(second_l1cenjet_et >= 32) &&
	(third_l1cenjet_et  >= 28)  ) numEvents_L1EG25er_3L1CenJet32_32_28_++;

    if( (first_l1cenjet_et  >= 36) &&
	(second_l1cenjet_et >= 28) &&
	(third_l1cenjet_et  >= 28)  ) numEvents_L1EG25er_3L1CenJet36_28_28_++;
    if( (first_l1cenjet_et  >= 36) &&
	(second_l1cenjet_et >= 32) &&
	(third_l1cenjet_et  >= 28)  ) numEvents_L1EG25er_3L1CenJet36_32_28_++;
    if( (first_l1cenjet_et  >= 36) &&
	(second_l1cenjet_et >= 36) &&
	(third_l1cenjet_et  >= 28)  ) numEvents_L1EG25er_3L1CenJet36_36_28_++;

    if( (first_l1cenjet_et  >= 40) &&
	(second_l1cenjet_et >= 28) &&
	(third_l1cenjet_et  >= 28)  ) numEvents_L1EG25er_3L1CenJet40_28_28_++;
    if( (first_l1cenjet_et  >= 40) &&
	(second_l1cenjet_et >= 32) &&
	(third_l1cenjet_et  >= 28)  ) numEvents_L1EG25er_3L1CenJet40_32_28_++;
    if( (first_l1cenjet_et  >= 40) &&
	(second_l1cenjet_et >= 36) &&
	(third_l1cenjet_et  >= 28)  ) numEvents_L1EG25er_3L1CenJet40_36_28_++;
    if( (first_l1cenjet_et  >= 40) &&
	(second_l1cenjet_et >= 40) &&
	(third_l1cenjet_et  >= 28)  ) numEvents_L1EG25er_3L1CenJet40_40_28_++;
  }



  if( numL1CenJets30NoOverlapL1EG20>=2 && hasL1EG20 ) numEvents_RemoveEGJetOverlap_L1EG20_2L1CenJet28_++;
  if( numL1CenJets30NoOverlapL1EG20>=3 && hasL1EG20 ) numEvents_RemoveEGJetOverlap_L1EG20_3L1CenJet28_++;
  if( numL1CenJets30NoOverlapL1EG20>=4 && hasL1EG20 ) numEvents_RemoveEGJetOverlap_L1EG20_4L1CenJet28_++;

  if( numL1CenJets30NoOverlapL1EG20er>=2 && hasL1EG20er ) numEvents_RemoveEGJetOverlap_L1EG20er_2L1CenJet28_++;
  if( numL1CenJets30NoOverlapL1EG20er>=3 && hasL1EG20er ) numEvents_RemoveEGJetOverlap_L1EG20er_3L1CenJet28_++;
  if( numL1CenJets30NoOverlapL1EG20er>=4 && hasL1EG20er ) numEvents_RemoveEGJetOverlap_L1EG20er_4L1CenJet28_++;

  if( numL1CenJets30NoOverlapL1EG25>=2 && hasL1EG25 ) numEvents_RemoveEGJetOverlap_L1EG25_2L1CenJet28_++;
  if( numL1CenJets30NoOverlapL1EG25>=3 && hasL1EG25 ) numEvents_RemoveEGJetOverlap_L1EG25_3L1CenJet28_++;
  if( numL1CenJets30NoOverlapL1EG25>=4 && hasL1EG25 ) numEvents_RemoveEGJetOverlap_L1EG25_4L1CenJet28_++;

  if( numL1CenJets30NoOverlapL1EG25er>=2 && hasL1EG25er ) numEvents_RemoveEGJetOverlap_L1EG25er_2L1CenJet28_++;
  if( numL1CenJets30NoOverlapL1EG25er>=3 && hasL1EG25er ) numEvents_RemoveEGJetOverlap_L1EG25er_3L1CenJet28_++;
  if( numL1CenJets30NoOverlapL1EG25er>=4 && hasL1EG25er ) numEvents_RemoveEGJetOverlap_L1EG25er_4L1CenJet28_++;



  passL1_of_interest = (
			pass_L1_SingleEG35er ||
			pass_L1_SingleIsoEG30er ||
			pass_L1_SingleJet200 ||
			pass_L1_SingleJet240 ||
			pass_L1_DoubleJetC100 ||
			pass_L1_DoubleJetC112 ||
			pass_L1_DoubleJetC120 ||
			pass_L1_QuadJetC60 ||
			pass_L1_QuadJetC84
			);


  if( !passL1_of_interest ){
    if( numL1CenJets30>=2 && hasL1EG20 ) numEvents_FailL1TofInterest_L1EG20_2L1CenJet28_++;
    if( numL1CenJets30>=3 && hasL1EG20 ) numEvents_FailL1TofInterest_L1EG20_3L1CenJet28_++;
    if( numL1CenJets30>=4 && hasL1EG20 ) numEvents_FailL1TofInterest_L1EG20_4L1CenJet28_++;

    if( numL1CenJets30>=2 && hasL1EG20er ) numEvents_FailL1TofInterest_L1EG20er_2L1CenJet28_++;
    if( numL1CenJets30>=3 && hasL1EG20er ) numEvents_FailL1TofInterest_L1EG20er_3L1CenJet28_++;
    if( numL1CenJets30>=4 && hasL1EG20er ) numEvents_FailL1TofInterest_L1EG20er_4L1CenJet28_++;

    if( numL1CenJets30>=2 && hasL1EG25 ) numEvents_FailL1TofInterest_L1EG25_2L1CenJet28_++;
    if( numL1CenJets30>=3 && hasL1EG25 ) numEvents_FailL1TofInterest_L1EG25_3L1CenJet28_++;
    if( numL1CenJets30>=4 && hasL1EG25 ) numEvents_FailL1TofInterest_L1EG25_4L1CenJet28_++;

    if( numL1CenJets30>=2 && hasL1EG25er ) numEvents_FailL1TofInterest_L1EG25er_2L1CenJet28_++;
    if( numL1CenJets30>=3 && hasL1EG25er ) numEvents_FailL1TofInterest_L1EG25er_3L1CenJet28_++;
    if( numL1CenJets30>=4 && hasL1EG25er ) numEvents_FailL1TofInterest_L1EG25er_4L1CenJet28_++;
  }

  if( !passL1_with_prescale ){
    if( numL1CenJets30>=2 && hasL1EG20 ) numEvents_FailL1TWithPrescale_L1EG20_2L1CenJet28_++;
    if( numL1CenJets30>=3 && hasL1EG20 ) numEvents_FailL1TWithPrescale_L1EG20_3L1CenJet28_++;
    if( numL1CenJets30>=4 && hasL1EG20 ) numEvents_FailL1TWithPrescale_L1EG20_4L1CenJet28_++;

    if( numL1CenJets30>=2 && hasL1EG20er ) numEvents_FailL1TWithPrescale_L1EG20er_2L1CenJet28_++;
    if( numL1CenJets30>=3 && hasL1EG20er ) numEvents_FailL1TWithPrescale_L1EG20er_3L1CenJet28_++;
    if( numL1CenJets30>=4 && hasL1EG20er ) numEvents_FailL1TWithPrescale_L1EG20er_4L1CenJet28_++;

    if( numL1CenJets30>=2 && hasL1EG25 ) numEvents_FailL1TWithPrescale_L1EG25_2L1CenJet28_++;
    if( numL1CenJets30>=3 && hasL1EG25 ) numEvents_FailL1TWithPrescale_L1EG25_3L1CenJet28_++;
    if( numL1CenJets30>=4 && hasL1EG25 ) numEvents_FailL1TWithPrescale_L1EG25_4L1CenJet28_++;

    if( numL1CenJets30>=2 && hasL1EG25er ) numEvents_FailL1TWithPrescale_L1EG25er_2L1CenJet28_++;
    if( numL1CenJets30>=3 && hasL1EG25er ) numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet28_++;
    if( numL1CenJets30>=4 && hasL1EG25er ) numEvents_FailL1TWithPrescale_L1EG25er_4L1CenJet28_++;


    if( hasL1EG25er ){
      if( l1ht > 100 ) numEvents_FailL1TWithPrescale_L1EG25er_L1HTT100_++;
      if( l1ht > 110 ) numEvents_FailL1TWithPrescale_L1EG25er_L1HTT110_++;
      if( l1ht > 120 ) numEvents_FailL1TWithPrescale_L1EG25er_L1HTT120_++;
      if( l1ht > 125 ) numEvents_FailL1TWithPrescale_L1EG25er_L1HTT125_++;
      if( l1ht > 130 ) numEvents_FailL1TWithPrescale_L1EG25er_L1HTT130_++;
      if( l1ht > 140 ) numEvents_FailL1TWithPrescale_L1EG25er_L1HTT140_++;
      if( l1ht > 150 ) numEvents_FailL1TWithPrescale_L1EG25er_L1HTT150_++;
      if( l1ht > 160 ) numEvents_FailL1TWithPrescale_L1EG25er_L1HTT160_++;
    }

    if( hasL1EG25er ){
      if( (first_l1cenjet_et  >= 32) &&
	  (second_l1cenjet_et >= 28) &&
	  (third_l1cenjet_et  >= 28)  ) numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet32_28_28_++;
      if( (first_l1cenjet_et  >= 32) &&
	  (second_l1cenjet_et >= 32) &&
	  (third_l1cenjet_et  >= 28)  ) numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet32_32_28_++;

      if( (first_l1cenjet_et  >= 36) &&
	  (second_l1cenjet_et >= 28) &&
	  (third_l1cenjet_et  >= 28)  ) numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_28_28_++;
      if( (first_l1cenjet_et  >= 36) &&
	  (second_l1cenjet_et >= 32) &&
	  (third_l1cenjet_et  >= 28)  ) numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_32_28_++;
      if( (first_l1cenjet_et  >= 36) &&
	  (second_l1cenjet_et >= 36) &&
	  (third_l1cenjet_et  >= 28)  ) numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_36_28_++;

      if( (first_l1cenjet_et  >= 40) &&
	  (second_l1cenjet_et >= 28) &&
	  (third_l1cenjet_et  >= 28)  ) numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_28_28_++;
      if( (first_l1cenjet_et  >= 40) &&
	  (second_l1cenjet_et >= 32) &&
	  (third_l1cenjet_et  >= 28)  ) numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_32_28_++;
      if( (first_l1cenjet_et  >= 40) &&
	  (second_l1cenjet_et >= 36) &&
	  (third_l1cenjet_et  >= 28)  ) numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_36_28_++;
      if( (first_l1cenjet_et  >= 40) &&
	  (second_l1cenjet_et >= 40) &&
	  (third_l1cenjet_et  >= 28)  ) numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_40_28_++;
    }


    if( numL1CenJets30NoOverlapL1EG20>=2 && hasL1EG20 ) numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_2L1CenJet28_++;
    if( numL1CenJets30NoOverlapL1EG20>=3 && hasL1EG20 ) numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_3L1CenJet28_++;
    if( numL1CenJets30NoOverlapL1EG20>=4 && hasL1EG20 ) numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_4L1CenJet28_++;

    if( numL1CenJets30NoOverlapL1EG20er>=2 && hasL1EG20er ) numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_2L1CenJet28_++;
    if( numL1CenJets30NoOverlapL1EG20er>=3 && hasL1EG20er ) numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_3L1CenJet28_++;
    if( numL1CenJets30NoOverlapL1EG20er>=4 && hasL1EG20er ) numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_4L1CenJet28_++;

    if( numL1CenJets30NoOverlapL1EG25>=2 && hasL1EG25 ) numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_2L1CenJet28_++;
    if( numL1CenJets30NoOverlapL1EG25>=3 && hasL1EG25 ) numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_3L1CenJet28_++;
    if( numL1CenJets30NoOverlapL1EG25>=4 && hasL1EG25 ) numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_4L1CenJet28_++;

    if( numL1CenJets30NoOverlapL1EG25er>=2 && hasL1EG25er ) numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_2L1CenJet28_++;
    if( numL1CenJets30NoOverlapL1EG25er>=3 && hasL1EG25er ) numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_3L1CenJet28_++;
    if( numL1CenJets30NoOverlapL1EG25er>=4 && hasL1EG25er ) numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_4L1CenJet28_++;

  }

  if( pass_L1EG25erHTT125 ) h_numGenPV_L1EG25erHTT125->Fill(numGenPV);
  if( pass_L1EG25erHTT125 && !passL1_with_prescale ) h_numGenPV_L1EG25erHTT125_pure->Fill(numGenPV);





  int nL1CenJetEt = int( l1cenjetEt.size() );
  Int_t idx[nL1CenJetEt];
  double l1CenJetEtSorted[nL1CenJetEt];
  for( int j=0; j<nL1CenJetEt; j++ ) l1CenJetEtSorted[j] = l1cenjetEt[j];

  TMath::Sort(nL1CenJetEt,l1CenJetEtSorted,idx);

  double ThirdCenJetEt = ( nL1CenJetEt>=3 ) ? l1CenJetEtSorted[idx[2]] : -1;

  if( maxEGer>=minEGEt_ && ThirdCenJetEt>=min3rdCenJetEt_ ){
    for( int iBinX=0; iBinX<h_L1EGEt_L1CenJet3rdJetEt->GetNbinsX(); iBinX++ ){
      for( int iBinY=0; iBinY<h_L1EGEt_L1CenJet3rdJetEt->GetNbinsY(); iBinY++ ){

	double egLow = h_L1EGEt_L1CenJet3rdJetEt->GetYaxis()->GetBinLowEdge(iBinY+1);
	double jetLow = h_L1EGEt_L1CenJet3rdJetEt->GetXaxis()->GetBinLowEdge(iBinX+1);

	if( maxEGer>=egLow && ThirdCenJetEt>=jetLow ) h_L1EGEt_L1CenJet3rdJetEt->Fill(jetLow+0.01,egLow+0.01);
	if( maxEGer>=egLow && ThirdCenJetEt>=jetLow && !passL1_with_prescale ) h_L1EGEt_L1CenJet3rdJetEt_pure->Fill(jetLow+0.01,egLow+0.01);
      }
    }
  }


}


// ------------ method called once each job just before starting event loop  ------------
void 
L1TriggerRate::beginJob()
{

  TH1::SetDefaultSumw2(true);

  numEvents_             = 0;
  numEvents_PassL1TWithPrescale_ = 0;
  numEvents_L1EG20_      = 0;
  numEvents_L1EG25_      = 0;
  numEvents_L1EG20er_    = 0;
  numEvents_L1EG25er_    = 0;
  numEvents_L1IsoEG30_   = 0;
  numEvents_L1IsoEG30er_ = 0;

  numEvents_L1IsoEG20erTauJet20er_ = 0;

  numEvents_FailL1TWithPrescale_L1IsoEG30er_ = 0;
  numEvents_FailL1TWithNoPrescale_L1IsoEG30er_ = 0;

  numEvents_2L1CenJet28_ = 0;
  numEvents_3L1CenJet28_ = 0;
  numEvents_4L1CenJet28_ = 0;

  numEvents_L1EG20_2L1CenJet28_ = 0;
  numEvents_L1EG20_3L1CenJet28_ = 0;
  numEvents_L1EG20_4L1CenJet28_ = 0;

  numEvents_L1EG20er_2L1CenJet28_ = 0;
  numEvents_L1EG20er_3L1CenJet28_ = 0;
  numEvents_L1EG20er_4L1CenJet28_ = 0;

  numEvents_L1EG25_2L1CenJet28_ = 0;
  numEvents_L1EG25_3L1CenJet28_ = 0;
  numEvents_L1EG25_4L1CenJet28_ = 0;

  numEvents_L1EG25er_2L1CenJet28_ = 0;
  numEvents_L1EG25er_3L1CenJet28_ = 0;
  numEvents_L1EG25er_4L1CenJet28_ = 0;

  numEvents_L1EG25er_L1HTT100_ = 0;
  numEvents_L1EG25er_L1HTT110_ = 0;
  numEvents_L1EG25er_L1HTT120_ = 0;
  numEvents_L1EG25er_L1HTT125_ = 0;
  numEvents_L1EG25er_L1HTT130_ = 0;
  numEvents_L1EG25er_L1HTT140_ = 0;
  numEvents_L1EG25er_L1HTT150_ = 0;
  numEvents_L1EG25er_L1HTT160_ = 0;

  numEvents_L1EG25er_3L1CenJet32_28_28_ = 0;
  numEvents_L1EG25er_3L1CenJet32_32_28_ = 0;

  numEvents_L1EG25er_3L1CenJet36_28_28_ = 0;
  numEvents_L1EG25er_3L1CenJet36_32_28_ = 0;
  numEvents_L1EG25er_3L1CenJet36_36_28_ = 0;

  numEvents_L1EG25er_3L1CenJet40_28_28_ = 0;
  numEvents_L1EG25er_3L1CenJet40_32_28_ = 0;
  numEvents_L1EG25er_3L1CenJet40_36_28_ = 0;
  numEvents_L1EG25er_3L1CenJet40_40_28_ = 0;


  numEvents_FailL1TofInterest_L1EG20_2L1CenJet28_ = 0;
  numEvents_FailL1TofInterest_L1EG20_3L1CenJet28_ = 0;
  numEvents_FailL1TofInterest_L1EG20_4L1CenJet28_ = 0;

  numEvents_FailL1TofInterest_L1EG20er_2L1CenJet28_ = 0;
  numEvents_FailL1TofInterest_L1EG20er_3L1CenJet28_ = 0;
  numEvents_FailL1TofInterest_L1EG20er_4L1CenJet28_ = 0;

  numEvents_FailL1TofInterest_L1EG25_2L1CenJet28_ = 0;
  numEvents_FailL1TofInterest_L1EG25_3L1CenJet28_ = 0;
  numEvents_FailL1TofInterest_L1EG25_4L1CenJet28_ = 0;

  numEvents_FailL1TofInterest_L1EG25er_2L1CenJet28_ = 0;
  numEvents_FailL1TofInterest_L1EG25er_3L1CenJet28_ = 0;
  numEvents_FailL1TofInterest_L1EG25er_4L1CenJet28_ = 0;


  numEvents_FailL1TWithPrescale_L1EG20_2L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG20_3L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG20_4L1CenJet28_ = 0;

  numEvents_FailL1TWithPrescale_L1EG20er_2L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG20er_3L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG20er_4L1CenJet28_ = 0;

  numEvents_FailL1TWithPrescale_L1EG25_2L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25_3L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25_4L1CenJet28_ = 0;

  numEvents_FailL1TWithPrescale_L1EG25er_2L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_4L1CenJet28_ = 0;

  numEvents_FailL1TWithPrescale_L1EG25er_L1HTT100_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_L1HTT110_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_L1HTT120_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_L1HTT125_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_L1HTT130_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_L1HTT140_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_L1HTT150_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_L1HTT160_ = 0;

  numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet32_28_28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet32_32_28_ = 0;

  numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_28_28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_32_28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_36_28_ = 0;

  numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_28_28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_32_28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_36_28_ = 0;
  numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_40_28_ = 0;

  numEvents_RemoveEGJetOverlap_L1EG20_2L1CenJet28_ = 0;
  numEvents_RemoveEGJetOverlap_L1EG20_3L1CenJet28_ = 0;
  numEvents_RemoveEGJetOverlap_L1EG20_4L1CenJet28_ = 0;

  numEvents_RemoveEGJetOverlap_L1EG20er_2L1CenJet28_ = 0;
  numEvents_RemoveEGJetOverlap_L1EG20er_3L1CenJet28_ = 0;
  numEvents_RemoveEGJetOverlap_L1EG20er_4L1CenJet28_ = 0;

  numEvents_RemoveEGJetOverlap_L1EG25_2L1CenJet28_ = 0;
  numEvents_RemoveEGJetOverlap_L1EG25_3L1CenJet28_ = 0;
  numEvents_RemoveEGJetOverlap_L1EG25_4L1CenJet28_ = 0;

  numEvents_RemoveEGJetOverlap_L1EG25er_2L1CenJet28_ = 0;
  numEvents_RemoveEGJetOverlap_L1EG25er_3L1CenJet28_ = 0;
  numEvents_RemoveEGJetOverlap_L1EG25er_4L1CenJet28_ = 0;


  numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_2L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_3L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_4L1CenJet28_ = 0;

  numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_2L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_3L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_4L1CenJet28_ = 0;

  numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_2L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_3L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_4L1CenJet28_ = 0;

  numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_2L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_3L1CenJet28_ = 0;
  numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_4L1CenJet28_ = 0;

}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1TriggerRate::endJob() 
{

  double freq = 11426.;
  double nb = 2508;  // number of expected bunched for 25 ns bunch spacing 
  double scale = (freq*nb)/numEvents_;
  scale = scale/1000.; // convert to kHz

  for( int iBin=0; iBin<h_numGenPV_L1EG25erHTT125_rate->GetNbinsX(); iBin++ ){
    double counts_pass = h_numGenPV_L1EG25erHTT125->GetBinContent(iBin+1);
    double counts_pure_pass = h_numGenPV_L1EG25erHTT125_pure->GetBinContent(iBin+1);
    double counts_total = h_numGenPV->GetBinContent(iBin+1);

    double rate = ( counts_total>0 ) ? (freq*nb*counts_pass)/(counts_total*1000) : 0;
    double pure_rate = ( counts_total>0 ) ? (freq*nb*counts_pure_pass)/(counts_total*1000) : 0;
    h_numGenPV_L1EG25erHTT125_rate->SetBinContent(iBin+1,rate);
    h_numGenPV_L1EG25erHTT125_pure_rate->SetBinContent(iBin+1,pure_rate);
  }
  

  for( int iBinX=0; iBinX<h_L1EGEt_L1CenJet3rdJetEt->GetNbinsX(); iBinX++ ){
    for( int iBinY=0; iBinY<h_L1EGEt_L1CenJet3rdJetEt->GetNbinsY(); iBinY++ ){
      double content = h_L1EGEt_L1CenJet3rdJetEt->GetBinContent(iBinX+1,iBinY+1);
      double err = h_L1EGEt_L1CenJet3rdJetEt->GetBinError(iBinX+1,iBinY+1);
      double new_content = scale * content;
      double new_err = scale * err;
      h_L1EGEt_L1CenJet3rdJetEt_rate->SetBinContent(iBinX+1,iBinY+1,new_content);
      h_L1EGEt_L1CenJet3rdJetEt_rate->SetBinError(iBinX+1,iBinY+1,new_err);

      double pure_content = h_L1EGEt_L1CenJet3rdJetEt_pure->GetBinContent(iBinX+1,iBinY+1);
      double pure_err = h_L1EGEt_L1CenJet3rdJetEt_pure->GetBinError(iBinX+1,iBinY+1);
      double new_pure_content = scale * pure_content;
      double new_pure_err = scale * pure_err;

      h_L1EGEt_L1CenJet3rdJetEt_pure_rate->SetBinContent(iBinX+1,iBinY+1,new_pure_content);
      h_L1EGEt_L1CenJet3rdJetEt_pure_rate->SetBinError(iBinX+1,iBinY+1,new_pure_err);
    }
  }

  int numHTbins = h_L1_HTT_integral->GetNbinsX();
  for( int iBin = 0; iBin < numHTbins; iBin++ ){
    double L1_HTT_integral = h_L1_HTT->Integral(iBin+1, numHTbins+1);
    double L1_HTT_integral_err = sqrt(L1_HTT_integral);
    h_L1_HTT_integral->SetBinContent(iBin+1, L1_HTT_integral);
    h_L1_HTT_integral->SetBinError(iBin+1, L1_HTT_integral_err);

    h_L1_HTT_integral_rate->SetBinContent(iBin+1, scale*L1_HTT_integral);
    h_L1_HTT_integral_rate->SetBinError(iBin+1, scale*L1_HTT_integral_err);

    double L1_HTT_integral_pure = h_L1_HTT_pure->Integral(iBin+1, numHTbins+1);
    double L1_HTT_integral_pure_err = sqrt(L1_HTT_integral_pure);
    h_L1_HTT_integral_pure->SetBinContent(iBin+1, L1_HTT_integral_pure);
    h_L1_HTT_integral_pure->SetBinError(iBin+1, L1_HTT_integral_pure_err);

    h_L1_HTT_integral_pure_rate->SetBinContent(iBin+1, scale*L1_HTT_integral_pure);
    h_L1_HTT_integral_pure_rate->SetBinError(iBin+1, scale*L1_HTT_integral_pure_err);

    double L1_HTT_SingleEG25er_integral = h_L1_HTT_SingleEG25er->Integral(iBin+1, numHTbins+1);
    double L1_HTT_SingleEG25er_integral_err = sqrt(L1_HTT_SingleEG25er_integral);
    h_L1_HTT_SingleEG25er_integral->SetBinContent(iBin+1, L1_HTT_SingleEG25er_integral);
    h_L1_HTT_SingleEG25er_integral->SetBinError(iBin+1, L1_HTT_SingleEG25er_integral_err);

    h_L1_HTT_SingleEG25er_integral_rate->SetBinContent(iBin+1, scale*L1_HTT_SingleEG25er_integral);
    h_L1_HTT_SingleEG25er_integral_rate->SetBinError(iBin+1, scale*L1_HTT_SingleEG25er_integral_err);

    double L1_HTT_SingleEG25er_integral_pure = h_L1_HTT_SingleEG25er_pure->Integral(iBin+1, numHTbins+1);
    double L1_HTT_SingleEG25er_integral_pure_err = sqrt(L1_HTT_SingleEG25er_integral_pure);
    h_L1_HTT_SingleEG25er_integral_pure->SetBinContent(iBin+1, L1_HTT_SingleEG25er_integral_pure);
    h_L1_HTT_SingleEG25er_integral_pure->SetBinError(iBin+1, L1_HTT_SingleEG25er_integral_pure_err);

    h_L1_HTT_SingleEG25er_integral_pure_rate->SetBinContent(iBin+1, scale*L1_HTT_SingleEG25er_integral_pure);
    h_L1_HTT_SingleEG25er_integral_pure_rate->SetBinError(iBin+1, scale*L1_HTT_SingleEG25er_integral_pure_err);
  }


  std::cout << "***************************************************************" << std::endl;
  std::cout << "  Summary for L1T Algo: Total number of events = " << numEvents_ << std::endl;
  int bit=0;
  for( std::map<std::string, int>::const_iterator iter = l1talgo_cppath_reHLT_.begin(); iter != l1talgo_cppath_reHLT_.end(); iter++){
    std::string name = iter->first;
    double eff = 100*double(iter->second)/double(numEvents_);
    printf("\t %d,\t %s,\t %d,\t %.1f \n",bit++, name.c_str(), iter->second, eff);
  }  
  std::cout << "***************************************************************" << std::endl;
  std::cout << "  Rates for L1T Algo: Total number of events = " << numEvents_ << std::endl;
  int bit2=0;
  for( std::map<std::string, int>::const_iterator iter = l1talgo_cppath_reHLT_.begin(); iter != l1talgo_cppath_reHLT_.end(); iter++){
    std::string name = iter->first;
    double unprescaled_rate = scale * iter->second;
    double prescaled_rate   = scale * l1talgo_cppath_prescale_reHLT_[name];
    double pure_rate = scale * l1talgo_cppath_pure_prescale_reHLT_[name];
    int prescale = l1talgo_initial_prescale_reHLT_[name];
    printf("\t %d\t %s\t %d\t %.2f\t %.2f\t %.2f \n",bit2++, name.c_str(), prescale, prescaled_rate, pure_rate, unprescaled_rate);
  }  
  std::cout << "***************************************************************" << std::endl;
  std::cout << "  Total number of events = " << numEvents_ << std::endl;
  std::cout << "  Number of events passing L1T (with prescale) = " << numEvents_PassL1TWithPrescale_ << ",\t rate (kHz) = " << scale*numEvents_PassL1TWithPrescale_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20      = " << numEvents_L1EG20_ << ",\t rate (kHz) = " << scale*numEvents_L1EG20_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25      = " << numEvents_L1EG25_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20er    = " << numEvents_L1EG20er_ << ",\t rate (kHz) = " << scale*numEvents_L1EG20er_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er    = " << numEvents_L1EG25er_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_ << std::endl;
  std::cout << "  Number of events with >=1 L1IsoEG30   = " << numEvents_L1IsoEG30_ << ",\t rate (kHz) = " << scale*numEvents_L1IsoEG30_ << std::endl;
  std::cout << "  Number of events with >=1 L1IsoEG30er = " << numEvents_L1IsoEG30er_ << ",\t rate (kHz) = " << scale*numEvents_L1IsoEG30er_  << std::endl;
  std::cout << "  Number of events with >=1 L1IsoEG20erTauJet20er = " << numEvents_L1IsoEG20erTauJet20er_ << ",\t rate (kHz) = " << scale*numEvents_L1IsoEG20erTauJet20er_  << std::endl;

  std::cout << "  Number of events with >=2 L1CenJet28  = " << numEvents_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=3 L1CenJet28  = " << numEvents_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=4 L1CenJet28  = " << numEvents_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_4L1CenJet28_ << std::endl;

  std::cout << "------------------------------------------------------" << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=2 L1CenJet28  = " << numEvents_L1EG20_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG20_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=3 L1CenJet28  = " << numEvents_L1EG20_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG20_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=4 L1CenJet28  = " << numEvents_L1EG20_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG20_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG20er && >=2 L1CenJet28  = " << numEvents_L1EG20er_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG20er_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20er && >=3 L1CenJet28  = " << numEvents_L1EG20er_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG20er_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20er && >=4 L1CenJet28  = " << numEvents_L1EG20er_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG20er_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG25 && >=2 L1CenJet28  = " << numEvents_L1EG25_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25 && >=3 L1CenJet28  = " << numEvents_L1EG25_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25 && >=4 L1CenJet28  = " << numEvents_L1EG25_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG25er && >=2 L1CenJet28  = " << numEvents_L1EG25er_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet28  = " << numEvents_L1EG25er_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=4 L1CenJet28  = " << numEvents_L1EG25er_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_4L1CenJet28_ << std::endl;

  std::cout << "------------------------------------------------------" << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=2 L1CenJet28 (remove overlap with EG) = " << numEvents_RemoveEGJetOverlap_L1EG20_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_RemoveEGJetOverlap_L1EG20_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=3 L1CenJet28 (remove overlap with EG) = " << numEvents_RemoveEGJetOverlap_L1EG20_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_RemoveEGJetOverlap_L1EG20_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=4 L1CenJet28 (remove overlap with EG) = " << numEvents_RemoveEGJetOverlap_L1EG20_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_RemoveEGJetOverlap_L1EG20_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG20er && >=2 L1CenJet28 (remove overlap with EG) = " << numEvents_RemoveEGJetOverlap_L1EG20er_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_RemoveEGJetOverlap_L1EG20er_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20er && >=3 L1CenJet28 (remove overlap with EG) = " << numEvents_RemoveEGJetOverlap_L1EG20er_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_RemoveEGJetOverlap_L1EG20er_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20er && >=4 L1CenJet28 (remove overlap with EG) = " << numEvents_RemoveEGJetOverlap_L1EG20er_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_RemoveEGJetOverlap_L1EG20er_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG25 && >=2 L1CenJet28 (remove overlap with EG) = " << numEvents_RemoveEGJetOverlap_L1EG25_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_RemoveEGJetOverlap_L1EG25_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25 && >=3 L1CenJet28 (remove overlap with EG) = " << numEvents_RemoveEGJetOverlap_L1EG25_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_RemoveEGJetOverlap_L1EG25_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25 && >=4 L1CenJet28 (remove overlap with EG) = " << numEvents_RemoveEGJetOverlap_L1EG25_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_RemoveEGJetOverlap_L1EG25_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG25er && >=2 L1CenJet28 (remove overlap with EG) = " << numEvents_RemoveEGJetOverlap_L1EG25er_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_RemoveEGJetOverlap_L1EG25er_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet28 (remove overlap with EG) = " << numEvents_RemoveEGJetOverlap_L1EG25er_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_RemoveEGJetOverlap_L1EG25er_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=4 L1CenJet28 (remove overlap with EG) = " << numEvents_RemoveEGJetOverlap_L1EG25er_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_RemoveEGJetOverlap_L1EG25er_4L1CenJet28_ << std::endl;
  std::cout << "------------------------------------------------------" << std::endl;
  std::cout << "   If events fail the triggers: ";
  for( int iAlgo = 0; iAlgo<int(l1t_of_interest_.size()); iAlgo++ ){
    std::cout << " " << l1t_of_interest_[iAlgo] << ",";
    if( iAlgo==(int(l1t_of_interest_.size())-1) ) std::cout << " " << l1t_of_interest_[iAlgo] << std::endl;
  }

  std::cout << "  Number of events with >=1 L1EG20 && >=2 L1CenJet28  = " << numEvents_FailL1TofInterest_L1EG20_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_FailL1TofInterest_L1EG20_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=3 L1CenJet28  = " << numEvents_FailL1TofInterest_L1EG20_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_FailL1TofInterest_L1EG20_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=4 L1CenJet28  = " << numEvents_FailL1TofInterest_L1EG20_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_FailL1TofInterest_L1EG20_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG20er && >=2 L1CenJet28  = " << numEvents_FailL1TofInterest_L1EG20er_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_FailL1TofInterest_L1EG20er_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20er && >=3 L1CenJet28  = " << numEvents_FailL1TofInterest_L1EG20er_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_FailL1TofInterest_L1EG20er_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20er && >=4 L1CenJet28  = " << numEvents_FailL1TofInterest_L1EG20er_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_FailL1TofInterest_L1EG20er_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG25 && >=2 L1CenJet28  = " << numEvents_FailL1TofInterest_L1EG25_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_FailL1TofInterest_L1EG25_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25 && >=3 L1CenJet28  = " << numEvents_FailL1TofInterest_L1EG25_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_FailL1TofInterest_L1EG25_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25 && >=4 L1CenJet28  = " << numEvents_FailL1TofInterest_L1EG25_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_FailL1TofInterest_L1EG25_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG25er && >=2 L1CenJet28  = " << numEvents_FailL1TofInterest_L1EG25er_2L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_FailL1TofInterest_L1EG25er_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet28  = " << numEvents_FailL1TofInterest_L1EG25er_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_FailL1TofInterest_L1EG25er_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=4 L1CenJet28  = " << numEvents_FailL1TofInterest_L1EG25er_4L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_FailL1TofInterest_L1EG25er_4L1CenJet28_ << std::endl;

  std::cout << "------------------------------------------------------" << std::endl;
  std::cout << "  Number of events with >=1 L1IsoEG30er = " << numEvents_FailL1TWithPrescale_L1IsoEG30er_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1IsoEG30er_  << std::endl;
  std::cout << "  Number of events with >=1 L1IsoEG30er = " << numEvents_FailL1TWithNoPrescale_L1IsoEG30er_ << ",\t pure rate (kHz) no prescale L1T = " << scale*numEvents_FailL1TWithNoPrescale_L1IsoEG30er_  << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=2 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG20_2L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG20_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=3 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG20_3L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG20_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=4 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG20_4L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG20_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG20er && >=2 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG20er_2L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG20er_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20er && >=3 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG20er_3L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG20er_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20er && >=4 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG20er_4L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG20er_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG25 && >=2 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG25_2L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25 && >=3 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG25_3L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25 && >=4 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG25_4L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG25er && >=2 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG25er_2L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=4 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG25er_4L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_4L1CenJet28_ << std::endl;

  std::cout << "------------------------------------------------------" << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=2 L1CenJet28 (remove overlap with EG) = " << numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_2L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=3 L1CenJet28 (remove overlap with EG) = " << numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_3L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20 && >=4 L1CenJet28 (remove overlap with EG) = " << numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_4L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG20er && >=2 L1CenJet28 (remove overlap with EG) = " << numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_2L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20er && >=3 L1CenJet28 (remove overlap with EG) = " << numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_3L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG20er && >=4 L1CenJet28 (remove overlap with EG) = " << numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_4L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG20er_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG25 && >=2 L1CenJet28 (remove overlap with EG) = " << numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_2L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25 && >=3 L1CenJet28 (remove overlap with EG) = " << numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_3L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25 && >=4 L1CenJet28 (remove overlap with EG) = " << numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_4L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25_4L1CenJet28_ << std::endl;

  std::cout << "  Number of events with >=1 L1EG25er && >=2 L1CenJet28 (remove overlap with EG) = " << numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_2L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_2L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet28 (remove overlap with EG) = " << numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_3L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_3L1CenJet28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=4 L1CenJet28 (remove overlap with EG) = " << numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_4L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_RemoveEGJetOverlap_L1EG25er_4L1CenJet28_ << std::endl;


  std::cout << "------------------------------------------------------" << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet28  = " << numEvents_L1EG25er_3L1CenJet28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_3L1CenJet28_ << std::endl;
  std::cout << "\t ------------- " << std::endl;

  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet32_28_28  = " << numEvents_L1EG25er_3L1CenJet32_28_28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_3L1CenJet32_28_28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet32_32_28  = " << numEvents_L1EG25er_3L1CenJet32_32_28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_3L1CenJet32_32_28_ << std::endl;
  std::cout << "\t ------------- " << std::endl;

  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet36_28_28  = " << numEvents_L1EG25er_3L1CenJet36_28_28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_3L1CenJet36_28_28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet36_32_28  = " << numEvents_L1EG25er_3L1CenJet36_32_28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_3L1CenJet36_32_28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet36_36_28  = " << numEvents_L1EG25er_3L1CenJet36_36_28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_3L1CenJet36_36_28_ << std::endl;
  std::cout << "\t ------------- " << std::endl;

  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet40_28_28  = " << numEvents_L1EG25er_3L1CenJet40_28_28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_3L1CenJet40_28_28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet40_32_28  = " << numEvents_L1EG25er_3L1CenJet40_32_28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_3L1CenJet40_32_28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet40_36_28  = " << numEvents_L1EG25er_3L1CenJet40_36_28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_3L1CenJet40_36_28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet40_40_28  = " << numEvents_L1EG25er_3L1CenJet40_40_28_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_3L1CenJet40_40_28_ << std::endl;

  std::cout << "\t --------------------------------------- " << std::endl;

  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet28  = " << numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet28_ << std::endl;
  std::cout << "\t ------------- " << std::endl;

  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet32_28_28  = " << numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet32_28_28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet32_28_28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet32_32_28  = " << numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet32_32_28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet32_32_28_ << std::endl;
  std::cout << "\t ------------- " << std::endl;

  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet36_28_28  = " << numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_28_28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_28_28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet36_32_28  = " << numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_32_28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_32_28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet36_36_28  = " << numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_36_28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet36_36_28_ << std::endl;
  std::cout << "\t ------------- " << std::endl;

  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet40_28_28  = " << numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_28_28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_28_28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet40_32_28  = " << numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_32_28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_32_28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet40_36_28  = " << numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_36_28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_36_28_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && >=3 L1CenJet40_40_28  = " << numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_40_28_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_3L1CenJet40_40_28_ << std::endl;


  std::cout << "\t --------------------------------------- " << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er    = " << numEvents_L1EG25er_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 100 = " << numEvents_L1EG25er_L1HTT100_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_L1HTT100_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 110 = " << numEvents_L1EG25er_L1HTT110_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_L1HTT110_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 120 = " << numEvents_L1EG25er_L1HTT120_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_L1HTT120_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 125 = " << numEvents_L1EG25er_L1HTT125_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_L1HTT125_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 130 = " << numEvents_L1EG25er_L1HTT130_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_L1HTT130_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 140 = " << numEvents_L1EG25er_L1HTT140_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_L1HTT140_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 150 = " << numEvents_L1EG25er_L1HTT150_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_L1HTT150_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 160 = " << numEvents_L1EG25er_L1HTT160_ << ",\t rate (kHz) = " << scale*numEvents_L1EG25er_L1HTT160_ << std::endl;

  std::cout << "\t ------------- " << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 100 = " << numEvents_FailL1TWithPrescale_L1EG25er_L1HTT100_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_L1HTT100_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 110 = " << numEvents_FailL1TWithPrescale_L1EG25er_L1HTT110_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_L1HTT110_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 120 = " << numEvents_FailL1TWithPrescale_L1EG25er_L1HTT120_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_L1HTT120_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 125 = " << numEvents_FailL1TWithPrescale_L1EG25er_L1HTT125_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_L1HTT125_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 130 = " << numEvents_FailL1TWithPrescale_L1EG25er_L1HTT130_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_L1HTT130_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 140 = " << numEvents_FailL1TWithPrescale_L1EG25er_L1HTT140_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_L1HTT140_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 150 = " << numEvents_FailL1TWithPrescale_L1EG25er_L1HTT150_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_L1HTT150_ << std::endl;
  std::cout << "  Number of events with >=1 L1EG25er && L1 HTT > 160 = " << numEvents_FailL1TWithPrescale_L1EG25er_L1HTT160_ << ",\t pure rate (kHz) = " << scale*numEvents_FailL1TWithPrescale_L1EG25er_L1HTT160_ << std::endl;

  std::cout << "***************************************************************" << std::endl;


}

// ------------ method called when starting to processes a run  ------------
/*
void 
L1TriggerRate::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
void 
L1TriggerRate::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{

  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  const L1GtTriggerMenu* menu = menuRcd.product();

  // L1 algos
  for( CItAlgo algo = menu->gtAlgorithmMap().begin(); algo!=menu->gtAlgorithmMap().end(); ++algo) {
    std::string name = (algo->second).algoName();
    std::cout << "\t L1T : " << name << std::endl;
    l1talgo_cppath_reHLT_[name] = 0;
    l1talgo_cppath_prescale_reHLT_[name] = 0;
    l1talgo_cppath_prescale_perevent_reHLT_[name] = 0;
    l1talgo_cppath_pure_prescale_reHLT_[name] = 0;
  }


  l1talgo_initial_prescale_reHLT_["L1_AlwaysTrue"] = -1;
  l1talgo_initial_prescale_reHLT_["L1_DoubleEG_15_10"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_DoubleEG_20_10_1LegIso"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_DoubleEG_22_10"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_DoubleMu0_Eta1p6_HighQ_WdEta18"] = 2000;
  l1talgo_initial_prescale_reHLT_["L1_DoubleMu0_Eta1p6_HighQ_WdEta18_OS"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_DoubleMu0_HighQ"] = 10000;
  l1talgo_initial_prescale_reHLT_["L1_DoubleMu6_EG6"] = 10000;
  l1talgo_initial_prescale_reHLT_["L1_DoubleMu7_EG7"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_DoubleMu_10_0_HighQ_WdEta18"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_DoubleMu_10_3p5"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_DoubleMu_10_Open"] = 2000;
  l1talgo_initial_prescale_reHLT_["L1_DoubleMu_12_5"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_DoubleS1Jet36_CenFor_NotWdPhi6"] = 10000;
  l1talgo_initial_prescale_reHLT_["L1_DoubleS1Jet36_CenFor_NotWdPhi7"] = 10000;
  l1talgo_initial_prescale_reHLT_["L1_DoubleS1JetC100"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_DoubleS1JetC112"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_DoubleS1JetC120"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_DoubleS1JetC32_WdPhi7_HTT125"] = 100;
  l1talgo_initial_prescale_reHLT_["L1_DoubleS1JetC52"] = 10000;
  l1talgo_initial_prescale_reHLT_["L1_DoubleS1JetC56_ETM60"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_DoubleS1JetC60_ETM60"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_DoubleS1JetC84"] = 700;
  l1talgo_initial_prescale_reHLT_["L1_DoubleTauJet36_Eta2p17"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_DoubleTauJet52er"] = 200;
  l1talgo_initial_prescale_reHLT_["L1_DoubleTauJet68er"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_ETM100"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_ETM30"] = 40000;
  l1talgo_initial_prescale_reHLT_["L1_ETM40"] = 8000;
  l1talgo_initial_prescale_reHLT_["L1_ETM50"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_ETM60"] = 900;
  l1talgo_initial_prescale_reHLT_["L1_ETM60_NotS1Jet52WdPhi2"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_ETM70"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_ETM70_NotS1Jet52WdPhi2"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_HTT125"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_HTT150"] = 800;
  l1talgo_initial_prescale_reHLT_["L1_HTT175"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_HTT200"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_HTT250"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_IsoEG20er_TauJet20er"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_Mu0er_ETM55"] = 100;
  l1talgo_initial_prescale_reHLT_["L1_Mu0er_ETM55_HTT100"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_Mu0er_ETM55_JetC120"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_Mu10er_ETM50"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_Mu14er_ETM30"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_Mu16er_TauJet20er"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_Mu20_EG10"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_Mu20_EG8"] = 10000;
  l1talgo_initial_prescale_reHLT_["L1_Mu3_S1JetC16_WdEtaPhi2"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_Mu3_S1JetC52_WdEtaPhi2"] = 200;
  l1talgo_initial_prescale_reHLT_["L1_Mu3_S1JetC92_WdEtaPhi2"] = 262139;
  l1talgo_initial_prescale_reHLT_["L1_Mu4_EG18"] = 10000;
  l1talgo_initial_prescale_reHLT_["L1_Mu5_DoubleEG5"] = 10000;
  l1talgo_initial_prescale_reHLT_["L1_Mu5_EG20"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_Mu5_IsoEG18"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_Mu6_DoubleEG10"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_Mu6_HTT150"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_QuadMu0_HighQ"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_QuadS1JetC36_TauJet52"] = 20;
  l1talgo_initial_prescale_reHLT_["L1_QuadS1JetC40"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_QuadS1JetC60"] = 1; 
  l1talgo_initial_prescale_reHLT_["L1_QuadS1JetC84"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleEG10"] = 20000;
  l1talgo_initial_prescale_reHLT_["L1_SingleEG20"] = 4000;
  l1talgo_initial_prescale_reHLT_["L1_SingleEG25er_HTT125"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleEG25"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_SingleEG30"] = 700;
  l1talgo_initial_prescale_reHLT_["L1_SingleEG35"] = 500;
  l1talgo_initial_prescale_reHLT_["L1_SingleEG35er"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleEG5"] = 262139;
  l1talgo_initial_prescale_reHLT_["L1_SingleIsoEG18"] = 3000;
  l1talgo_initial_prescale_reHLT_["L1_SingleIsoEG25er"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_SingleIsoEG28er"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_SingleIsoEG30er"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu12"] = 3000;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu14_Eta2p1"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu16"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu16_Eta2p1"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu18er"] = 700;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu20"] = 700;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu20er"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu25"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu25er"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu30"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu30er"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu5"] = 50000;
  l1talgo_initial_prescale_reHLT_["L1_SingleMu6_NotBptxOR"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleMuBeamHalo"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleMuOpen"] = 262139;
  l1talgo_initial_prescale_reHLT_["L1_SingleS1Jet128"] = 1000;
  l1talgo_initial_prescale_reHLT_["L1_SingleS1Jet176"] = 300;
  l1talgo_initial_prescale_reHLT_["L1_SingleS1Jet200"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleS1Jet240"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleS1Jet52"] = 50000;
  l1talgo_initial_prescale_reHLT_["L1_SingleS1Jet68"] = 40000;
  l1talgo_initial_prescale_reHLT_["L1_SingleS1Jet92"] = 4000;
  l1talgo_initial_prescale_reHLT_["L1_SingleS1JetC20_NotBptxOR"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_SingleS1JetC32_NotBptxOR"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_TripleEG10"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_TripleEG_14_10_8"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_TripleMu0_HighQ"] = 100;
  l1talgo_initial_prescale_reHLT_["L1_TripleMu_5_5_3_HighQ"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_TripleS1Jet_92_76_64_VBF"] = 1;
  l1talgo_initial_prescale_reHLT_["L1_ZeroBias"] = -1;


  for( std::map<std::string, int>::const_iterator iter = l1talgo_initial_prescale_reHLT_.begin(); iter != l1talgo_initial_prescale_reHLT_.end(); iter++){
    std::string name = iter->first;
    l1talgo_prescale_reHLT_[name] = iter->second;
  }

}

// ------------ method called when ending the processing of a run  ------------
/*
void 
L1TriggerRate::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
L1TriggerRate::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
L1TriggerRate::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TriggerRate::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TriggerRate);
