// -*- C++ -*-
//
// Package:    ttH-LeptonPlusJets/TTHMiniAODAnalyzer
// Class:      TTHMiniAODAnalyzer
// 
/**\class TTHMiniAODAnalyzer TTHMiniAODAnalyzer.cc ttH-LeptonPlusJets/AnalysisCode/plugins/TTHMiniAODAnalyzer.cc

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

#include "DataFormats/Candidate/interface/VertexCompositePtrCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositePtrCandidateFwd.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "MiniAOD/MiniAODHelper/interface/MiniAODHelper.h"

//
// class declaration
//

class TTHMiniAODAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TTHMiniAODAnalyzer(const edm::ParameterSet&);
      ~TTHMiniAODAnalyzer();

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
  edm::EDGetTokenT <edm::TriggerResults> triggerResultsToken;
  edm::EDGetTokenT <edm::TriggerResults> filterResultsToken;

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

  HLTConfigProvider hlt_config_;
  HLTConfigProvider filter_config_;

  std::string hltTag;
  std::string filterTag;

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

  TH1D* h_jet_pt;
  TH1D* h_jet_eta;
  TH1D* h_jet_phi;
  TH1D* h_jet_mass;
  TH1D* h_jet_csv;
  TH1D* h_jet_csv_b;
  TH1D* h_jet_csv_c;
  TH1D* h_jet_csv_l;
  TH1D* h_jet_csv_o;
  TH1D* h_jet_secVtx_mass;
  TH1D* h_jet_secVtx_mass_b;
  TH1D* h_jet_secVtx_mass_c;
  TH1D* h_jet_secVtx_mass_l;
  TH1D* h_jet_secVtx_mass_o;
  TH1D* h_jet_secVtx_Ntracks;
  TH1D* h_jet_secVtx_Ntracks_b;
  TH1D* h_jet_secVtx_Ntracks_c;
  TH1D* h_jet_secVtx_Ntracks_l;
  TH1D* h_jet_secVtx_Ntracks_o;
  TH1D* h_jet_secVtx_3DVal;
  TH1D* h_jet_secVtx_3DVal_b;
  TH1D* h_jet_secVtx_3DVal_c;
  TH1D* h_jet_secVtx_3DVal_l;
  TH1D* h_jet_secVtx_3DVal_o;
  TH1D* h_jet_secVtx_3DSig;
  TH1D* h_jet_secVtx_3DSig_b;
  TH1D* h_jet_secVtx_3DSig_c;
  TH1D* h_jet_secVtx_3DSig_l;
  TH1D* h_jet_secVtx_3DSig_o;

  TH1D* h_jet_CSVT_secVtx_mass;
  TH1D* h_jet_CSVT_secVtx_mass_b;
  TH1D* h_jet_CSVT_secVtx_mass_c;
  TH1D* h_jet_CSVT_secVtx_mass_l;
  TH1D* h_jet_CSVT_secVtx_mass_o;
  TH1D* h_jet_CSVT_secVtx_Ntracks;
  TH1D* h_jet_CSVT_secVtx_Ntracks_b;
  TH1D* h_jet_CSVT_secVtx_Ntracks_c;
  TH1D* h_jet_CSVT_secVtx_Ntracks_l;
  TH1D* h_jet_CSVT_secVtx_Ntracks_o;
  TH1D* h_jet_CSVT_secVtx_3DVal;
  TH1D* h_jet_CSVT_secVtx_3DVal_b;
  TH1D* h_jet_CSVT_secVtx_3DVal_c;
  TH1D* h_jet_CSVT_secVtx_3DVal_l;
  TH1D* h_jet_CSVT_secVtx_3DVal_o;
  TH1D* h_jet_CSVT_secVtx_3DSig;
  TH1D* h_jet_CSVT_secVtx_3DSig_b;
  TH1D* h_jet_CSVT_secVtx_3DSig_c;
  TH1D* h_jet_CSVT_secVtx_3DSig_l;
  TH1D* h_jet_CSVT_secVtx_3DSig_o;

  TH1D* h_jet_chargedHadronEnergyFraction;
  TH1D* h_jet_neutralHadronEnergyFraction;
  TH1D* h_jet_chargedEmEnergyFraction;
  TH1D* h_jet_neutralEmEnergyFraction;
  TH1D* h_jet_chargedMultiplicity;
  TH1D* h_jet_neutralMultiplicity;
  TH1D* h_jet_numberOfDaughters;

  TH1D* h_numJet_nocc;

  TH1D* h_jet_nocc_pt;
  TH1D* h_jet_nocc_eta;
  TH1D* h_jet_nocc_phi;
  TH1D* h_jet_nocc_mass;
  TH1D* h_jet_nocc_csv;
  TH1D* h_jet_nocc_csv_b;
  TH1D* h_jet_nocc_csv_c;
  TH1D* h_jet_nocc_csv_l;
  TH1D* h_jet_nocc_csv_o;

  TH1D* h_jet_nocc_chargedHadronEnergyFraction;
  TH1D* h_jet_nocc_neutralHadronEnergyFraction;
  TH1D* h_jet_nocc_chargedEmEnergyFraction;
  TH1D* h_jet_nocc_neutralEmEnergyFraction;
  TH1D* h_jet_nocc_chargedMultiplicity;
  TH1D* h_jet_nocc_neutralMultiplicity;
  TH1D* h_jet_nocc_numberOfDaughters;


  TH1D* h_ele_tight_pt;
  TH1D* h_ele_tight_eta;
  TH1D* h_ele_tight_phi;
  TH1D* h_ele_tight_iso;
  TH1D* h_ele_tight_chHadIso;
  TH1D* h_ele_tight_neHadIso;
  TH1D* h_ele_tight_photonIso;

  TH1D* h_mu_tight_pt;
  TH1D* h_mu_tight_eta;
  TH1D* h_mu_tight_phi;
  TH1D* h_mu_tight_iso;
  TH1D* h_mu_tight_chHadIso;
  TH1D* h_mu_tight_neHadIso;
  TH1D* h_mu_tight_photonIso;
  TH1D* h_mu_tight_sumPUPtIso;

  TH1D* h_ele_noiso_pt;
  TH1D* h_ele_noiso_eta;
  TH1D* h_ele_noiso_phi;
  TH1D* h_ele_noiso_iso;
  TH1D* h_ele_noiso_chHadIso;
  TH1D* h_ele_noiso_neHadIso;
  TH1D* h_ele_noiso_photonIso;

  TH1D* h_mu_noiso_pt;
  TH1D* h_mu_noiso_eta;
  TH1D* h_mu_noiso_phi;
  TH1D* h_mu_noiso_iso;
  TH1D* h_mu_noiso_chHadIso;
  TH1D* h_mu_noiso_neHadIso;
  TH1D* h_mu_noiso_photonIso;
  TH1D* h_mu_noiso_sumPUPtIso;

  TH1D* h_met_pt;
  TH1D* h_met_phi;

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

  TH1D* h_dR_tight_ele_jet;
  TH1D* h_dR_tight_mu_jet;


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

  TH1D* h_pfcand_energy;
  TH1D* h_pfcand_pt;
  TH1D* h_pfcand_eta;
  TH1D* h_pfcand_phi;
  TH1D* h_pfcand_fromPV;
  TH1D* h_pfcand_pdgId;

  TH1D* h_pfcand_chargedHadronEnergy;
  TH1D* h_pfcand_neutralHadronEnergy;
  TH1D* h_pfcand_photonEnergy;
  TH1D* h_pfcand_electronEnergy;
  TH1D* h_pfcand_muonEnergy;
  TH1D* h_pfcand_HFHadronEnergy;
  TH1D* h_pfcand_HFEMEnergy;
  TH1D* h_pfcand_chargedHadronMultiplicity;
  TH1D* h_pfcand_neutralHadronMultiplicity;
  TH1D* h_pfcand_photonMultiplicity;
  TH1D* h_pfcand_electronMultiplicity;
  TH1D* h_pfcand_muonMultiplicity;
  TH1D* h_pfcand_HFHadronMultiplicity;
  TH1D* h_pfcand_HFEMMultiplicity;

  TH1D* h_pfcand_chargedEmEnergy;
  TH1D* h_pfcand_neutralEmEnergy;
  TH1D* h_pfcand_chargedMuEnergy;
  TH1D* h_pfcand_chargedMultiplicity;
  TH1D* h_pfcand_neutralMultiplicity;


  TH1D* h_pfcand_pt0to2_chargedHadronEnergy;
  TH1D* h_pfcand_pt0to2_neutralHadronEnergy;
  TH1D* h_pfcand_pt0to2_photonEnergy;
  TH1D* h_pfcand_pt0to2_chargedHadronMultiplicity;
  TH1D* h_pfcand_pt0to2_neutralHadronMultiplicity;
  TH1D* h_pfcand_pt0to2_photonMultiplicity;

  TH1D* h_pfcand_pt2toInf_chargedHadronEnergy;
  TH1D* h_pfcand_pt2toInf_neutralHadronEnergy;
  TH1D* h_pfcand_pt2toInf_photonEnergy;
  TH1D* h_pfcand_pt2toInf_chargedHadronMultiplicity;
  TH1D* h_pfcand_pt2toInf_neutralHadronMultiplicity;
  TH1D* h_pfcand_pt2toInf_photonMultiplicity;

  TH1D* h_pfcand_pt5toInf_chargedHadronEnergy;
  TH1D* h_pfcand_pt5toInf_neutralHadronEnergy;
  TH1D* h_pfcand_pt5toInf_photonEnergy;
  TH1D* h_pfcand_pt5toInf_chargedHadronMultiplicity;
  TH1D* h_pfcand_pt5toInf_neutralHadronMultiplicity;
  TH1D* h_pfcand_pt5toInf_photonMultiplicity;

  TH1D* h_pfcand_eta0to1p5_chargedHadronEnergy;
  TH1D* h_pfcand_eta0to1p5_neutralHadronEnergy;
  TH1D* h_pfcand_eta0to1p5_photonEnergy;
  TH1D* h_pfcand_eta0to1p5_chargedHadronMultiplicity;
  TH1D* h_pfcand_eta0to1p5_neutralHadronMultiplicity;
  TH1D* h_pfcand_eta0to1p5_photonMultiplicity;

  TH1D* h_pfcand_eta1p5to2p5_chargedHadronEnergy;
  TH1D* h_pfcand_eta1p5to2p5_neutralHadronEnergy;
  TH1D* h_pfcand_eta1p5to2p5_photonEnergy;
  TH1D* h_pfcand_eta1p5to2p5_chargedHadronMultiplicity;
  TH1D* h_pfcand_eta1p5to2p5_neutralHadronMultiplicity;
  TH1D* h_pfcand_eta1p5to2p5_photonMultiplicity;

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
TTHMiniAODAnalyzer::TTHMiniAODAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  verbose_ = false;
  dumpHLT_ = false;

  hltTag = "HLT";
  filterTag = "PAT";

  triggerResultsToken = consumes <edm::TriggerResults> (edm::InputTag(std::string("TriggerResults"), std::string(""), hltTag));
  filterResultsToken = consumes <edm::TriggerResults> (edm::InputTag(std::string("TriggerResults"), std::string(""), filterTag));

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


  h_numJet = fs_->make<TH1D>("h_numJet", ";Number of Jets", NjetBins, NjetMin-0.5, NjetMax+0.5 );
  h_numTag = fs_->make<TH1D>("h_numTag", ";Number of Tags", NtagBins, NtagMin-0.5, NtagMax+0.5 );
  h_numPV  = fs_->make<TH1D>("h_numPV", ";Number of Good Vertices", NpuBins, NpuMin-0.5, NpuMax+0.5 );
  h_numTruePV  = fs_->make<TH1D>("h_numTruePV", ";Number of True PU Vertices", NpuBins, NpuMin-0.5, NpuMax+0.5 );

  h_numSecVtx = fs_->make<TH1D>("h_numSecVtx", ";Number of Secondary Vertices", 6, 6-0.5, 6+0.5 );


  h_jet_pt = fs_->make<TH1D>("h_jet_pt",";jet p_{T}", NjetptBins, 0, jetptmax );
  h_jet_eta = fs_->make<TH1D>("h_jet_eta",";jet #eta", 25, -2.5, 2.5 );
  h_jet_phi = fs_->make<TH1D>("h_jet_phi",";jet #phi", 34, -3.4, 3.4 );
  h_jet_mass = fs_->make<TH1D>("h_jet_mass",";jet mass", 30, 0, 300 );
  h_jet_csv = fs_->make<TH1D>("h_jet_csv",";jet CSV output", 1600, -0.5, 1.1 );
  h_jet_csv_b = fs_->make<TH1D>("h_jet_csv_b",";b jet CSV output", 1600, -0.5, 1.1 );
  h_jet_csv_c = fs_->make<TH1D>("h_jet_csv_c",";c jet CSV output", 1600, -0.5, 1.1 );
  h_jet_csv_l = fs_->make<TH1D>("h_jet_csv_l",";udsg jet CSV output", 1600, -0.5, 1.1 );
  h_jet_csv_o = fs_->make<TH1D>("h_jet_csv_o",";other jet CSV output", 1600, -0.5, 1.1 );

  h_jet_secVtx_mass = fs_->make<TH1D>("h_jet_secVtx_mass",";jet secondary vertex mass", 80, 0., 8.0 );
  h_jet_secVtx_mass_b = fs_->make<TH1D>("h_jet_secVtx_mass_b",";b jet secondary vertex mass", 80, 0., 8.0 );
  h_jet_secVtx_mass_c = fs_->make<TH1D>("h_jet_secVtx_mass_c",";c jet secondary vertex mass", 80, 0., 8.0 );
  h_jet_secVtx_mass_l = fs_->make<TH1D>("h_jet_secVtx_mass_l",";l jet secondary vertex mass", 80, 0., 8.0 );
  h_jet_secVtx_mass_o = fs_->make<TH1D>("h_jet_secVtx_mass_o",";o jet secondary vertex mass", 80, 0., 8.0 );

  h_jet_secVtx_Ntracks = fs_->make<TH1D>("h_jet_secVtx_Ntracks",";jet secondary vertex number of tracks", 14, -0.5, 13.5 );
  h_jet_secVtx_Ntracks_b = fs_->make<TH1D>("h_jet_secVtx_Ntracks_b",";b jet secondary vertex number of tracks", 14, -0.5, 13.5 );
  h_jet_secVtx_Ntracks_c = fs_->make<TH1D>("h_jet_secVtx_Ntracks_c",";c jet secondary vertex number of tracks", 14, -0.5, 13.5 );
  h_jet_secVtx_Ntracks_l = fs_->make<TH1D>("h_jet_secVtx_Ntracks_l",";l jet secondary vertex number of tracks", 14, -0.5, 13.5 );
  h_jet_secVtx_Ntracks_o = fs_->make<TH1D>("h_jet_secVtx_Ntracks_o",";o jet secondary vertex number of tracks", 14, -0.5, 13.5 );

  h_jet_secVtx_3DVal = fs_->make<TH1D>("h_jet_secVtx_3DVal",";jet secondary vertex 3D value", 141, -0.01, 14.0 );
  h_jet_secVtx_3DVal_b = fs_->make<TH1D>("h_jet_secVtx_3DVal_b",";b jet secondary vertex 3D value", 141, -0.01, 14.0 );
  h_jet_secVtx_3DVal_c = fs_->make<TH1D>("h_jet_secVtx_3DVal_c",";c jet secondary vertex 3D value", 141, -0.01, 14.0 );
  h_jet_secVtx_3DVal_l = fs_->make<TH1D>("h_jet_secVtx_3DVal_l",";l jet secondary vertex 3D value", 141, -0.01, 14.0 );
  h_jet_secVtx_3DVal_o = fs_->make<TH1D>("h_jet_secVtx_3DVal_o",";o jet secondary vertex 3D value", 141, -0.01, 14.0 );

  h_jet_secVtx_3DSig = fs_->make<TH1D>("h_jet_secVtx_3DSig",";jet secondary vertex 3D significance", 401, -0.01, 400 );
  h_jet_secVtx_3DSig_b = fs_->make<TH1D>("h_jet_secVtx_3DSig_b",";b jet secondary vertex 3D significance", 401, -0.01, 400 );
  h_jet_secVtx_3DSig_c = fs_->make<TH1D>("h_jet_secVtx_3DSig_c",";c jet secondary vertex 3D significance", 401, -0.01, 400 );
  h_jet_secVtx_3DSig_l = fs_->make<TH1D>("h_jet_secVtx_3DSig_l",";l jet secondary vertex 3D significance", 401, -0.01, 400 );
  h_jet_secVtx_3DSig_o = fs_->make<TH1D>("h_jet_secVtx_3DSig_o",";o jet secondary vertex 3D significance", 401, -0.01, 400 );

  h_jet_CSVT_secVtx_mass = fs_->make<TH1D>("h_jet_CSVT_secVtx_mass",";jet secondary vertex mass", 80, 0., 8.0 );
  h_jet_CSVT_secVtx_mass_b = fs_->make<TH1D>("h_jet_CSVT_secVtx_mass_b",";b jet secondary vertex mass", 80, 0., 8.0 );
  h_jet_CSVT_secVtx_mass_c = fs_->make<TH1D>("h_jet_CSVT_secVtx_mass_c",";c jet secondary vertex mass", 80, 0., 8.0 );
  h_jet_CSVT_secVtx_mass_l = fs_->make<TH1D>("h_jet_CSVT_secVtx_mass_l",";l jet secondary vertex mass", 80, 0., 8.0 );
  h_jet_CSVT_secVtx_mass_o = fs_->make<TH1D>("h_jet_CSVT_secVtx_mass_o",";o jet secondary vertex mass", 80, 0., 8.0 );

  h_jet_CSVT_secVtx_Ntracks = fs_->make<TH1D>("h_jet_CSVT_secVtx_Ntracks",";jet secondary vertex number of tracks", 14, -0.5, 13.5 );
  h_jet_CSVT_secVtx_Ntracks_b = fs_->make<TH1D>("h_jet_CSVT_secVtx_Ntracks_b",";b jet secondary vertex number of tracks", 14, -0.5, 13.5 );
  h_jet_CSVT_secVtx_Ntracks_c = fs_->make<TH1D>("h_jet_CSVT_secVtx_Ntracks_c",";c jet secondary vertex number of tracks", 14, -0.5, 13.5 );
  h_jet_CSVT_secVtx_Ntracks_l = fs_->make<TH1D>("h_jet_CSVT_secVtx_Ntracks_l",";l jet secondary vertex number of tracks", 14, -0.5, 13.5 );
  h_jet_CSVT_secVtx_Ntracks_o = fs_->make<TH1D>("h_jet_CSVT_secVtx_Ntracks_o",";o jet secondary vertex number of tracks", 14, -0.5, 13.5 );

  h_jet_CSVT_secVtx_3DVal = fs_->make<TH1D>("h_jet_CSVT_secVtx_3DVal",";jet secondary vertex 3D value", 141, -0.01, 14.0 );
  h_jet_CSVT_secVtx_3DVal_b = fs_->make<TH1D>("h_jet_CSVT_secVtx_3DVal_b",";b jet secondary vertex 3D value", 141, -0.01, 14.0 );
  h_jet_CSVT_secVtx_3DVal_c = fs_->make<TH1D>("h_jet_CSVT_secVtx_3DVal_c",";c jet secondary vertex 3D value", 141, -0.01, 14.0 );
  h_jet_CSVT_secVtx_3DVal_l = fs_->make<TH1D>("h_jet_CSVT_secVtx_3DVal_l",";l jet secondary vertex 3D value", 141, -0.01, 14.0 );
  h_jet_CSVT_secVtx_3DVal_o = fs_->make<TH1D>("h_jet_CSVT_secVtx_3DVal_o",";o jet secondary vertex 3D value", 141, -0.01, 14.0 );

  h_jet_CSVT_secVtx_3DSig = fs_->make<TH1D>("h_jet_CSVT_secVtx_3DSig",";jet secondary vertex 3D significance", 401, -0.01, 400 );
  h_jet_CSVT_secVtx_3DSig_b = fs_->make<TH1D>("h_jet_CSVT_secVtx_3DSig_b",";b jet secondary vertex 3D significance", 401, -0.01, 400 );
  h_jet_CSVT_secVtx_3DSig_c = fs_->make<TH1D>("h_jet_CSVT_secVtx_3DSig_c",";c jet secondary vertex 3D significance", 401, -0.01, 400 );
  h_jet_CSVT_secVtx_3DSig_l = fs_->make<TH1D>("h_jet_CSVT_secVtx_3DSig_l",";l jet secondary vertex 3D significance", 401, -0.01, 400 );
  h_jet_CSVT_secVtx_3DSig_o = fs_->make<TH1D>("h_jet_CSVT_secVtx_3DSig_o",";o jet secondary vertex 3D significance", 401, -0.01, 400 );


  h_jet_chargedHadronEnergyFraction = fs_->make<TH1D>("h_jet_chargedHadronEnergyFraction",";jet chargedHadronEnergyFraction", 100, 0, 1 );
  h_jet_neutralHadronEnergyFraction = fs_->make<TH1D>("h_jet_neutralHadronEnergyFraction",";jet neutralHadronEnergyFraction", 100, 0, 1 );
  h_jet_chargedEmEnergyFraction = fs_->make<TH1D>("h_jet_chargedEmEnergyFraction",";jet chargedEmEnergyFraction", 100, 0, 1 );
  h_jet_neutralEmEnergyFraction = fs_->make<TH1D>("h_jet_neutralEmEnergyFraction",";jet neutralEmEnergyFraction", 100, 0, 1 );
  h_jet_chargedMultiplicity = fs_->make<TH1D>("h_jet_chargedMultiplicity",";jet chargedMultiplicity", 100, 0, 100 );
  h_jet_neutralMultiplicity = fs_->make<TH1D>("h_jet_neutralMultiplicity",";jet neutralMultiplicity", 100, 0, 100 );
  h_jet_numberOfDaughters = fs_->make<TH1D>("h_jet_numberOfDaughters",";jet numberOfDaughters", 100, 0, 100 );


  h_numJet_nocc = fs_->make<TH1D>("h_numJet_nocc", ";Number of Jets", NjetBins, NjetMin-0.5, NjetMax+0.5 );

  h_jet_nocc_pt = fs_->make<TH1D>("h_jet_nocc_pt",";jet p_{T}", NjetptBins, 0, jetptmax );
  h_jet_nocc_eta = fs_->make<TH1D>("h_jet_nocc_eta",";jet #eta", 25, -2.5, 2.5 );
  h_jet_nocc_phi = fs_->make<TH1D>("h_jet_nocc_phi",";jet #phi", 34, -3.4, 3.4 );
  h_jet_nocc_mass = fs_->make<TH1D>("h_jet_nocc_mass",";jet mass", 30, 0, 300 );
  h_jet_nocc_csv = fs_->make<TH1D>("h_jet_nocc_csv",";jet CSV output", 1600, -0.5, 1.1 );
  h_jet_nocc_csv_b = fs_->make<TH1D>("h_jet_nocc_csv_b",";b jet CSV output", 1600, -0.5, 1.1 );
  h_jet_nocc_csv_c = fs_->make<TH1D>("h_jet_nocc_csv_c",";c jet CSV output", 1600, -0.5, 1.1 );
  h_jet_nocc_csv_l = fs_->make<TH1D>("h_jet_nocc_csv_l",";udsg jet CSV output", 1600, -0.5, 1.1 );
  h_jet_nocc_csv_o = fs_->make<TH1D>("h_jet_nocc_csv_o",";other jet CSV output", 1600, -0.5, 1.1 );

  h_jet_nocc_chargedHadronEnergyFraction = fs_->make<TH1D>("h_jet_nocc_chargedHadronEnergyFraction",";jet chargedHadronEnergyFraction", 100, 0, 1 );
  h_jet_nocc_neutralHadronEnergyFraction = fs_->make<TH1D>("h_jet_nocc_neutralHadronEnergyFraction",";jet neutralHadronEnergyFraction", 100, 0, 1 );
  h_jet_nocc_chargedEmEnergyFraction = fs_->make<TH1D>("h_jet_nocc_chargedEmEnergyFraction",";jet chargedEmEnergyFraction", 100, 0, 1 );
  h_jet_nocc_neutralEmEnergyFraction = fs_->make<TH1D>("h_jet_nocc_neutralEmEnergyFraction",";jet neutralEmEnergyFraction", 100, 0, 1 );
  h_jet_nocc_chargedMultiplicity = fs_->make<TH1D>("h_jet_nocc_chargedMultiplicity",";jet chargedMultiplicity", 100, 0, 100 );
  h_jet_nocc_neutralMultiplicity = fs_->make<TH1D>("h_jet_nocc_neutralMultiplicity",";jet neutralMultiplicity", 100, 0, 100 );
  h_jet_nocc_numberOfDaughters = fs_->make<TH1D>("h_jet_nocc_numberOfDaughters",";jet numberOfDaughters", 100, 0, 100 );


  h_ele_tight_pt  = fs_->make<TH1D>("h_ele_tight_pt",";tight ele p_{T}", NptBins, 0, ptmax );
  h_ele_tight_eta = fs_->make<TH1D>("h_ele_tight_eta",";tight ele #eta", 25, -2.5, 2.5 );
  h_ele_tight_phi = fs_->make<TH1D>("h_ele_tight_phi",";tight ele #phi", 34, -3.4, 3.4 );
  h_ele_tight_iso = fs_->make<TH1D>("h_ele_tight_iso",";tight ele relative isolation", 220, -0.01, 0.21 );
  h_ele_tight_chHadIso = fs_->make<TH1D>("h_ele_tight_chHadIso",";tight ele charged hadron isolation", 200, -0.01, 10. );
  h_ele_tight_neHadIso = fs_->make<TH1D>("h_ele_tight_neHadIso",";tight ele neutral hadron isolation", 200, -0.01, 20. );
  h_ele_tight_photonIso = fs_->make<TH1D>("h_ele_tight_photonIso",";tight ele photon isolation", 200, -0.01, 20. );

  h_mu_tight_pt = fs_->make<TH1D>("h_mu_tight_pt",";tight mu p_{T}", NptBins, 0, ptmax );
  h_mu_tight_eta = fs_->make<TH1D>("h_mu_tight_eta",";tight mu #eta", 25, -2.5, 2.5 );
  h_mu_tight_phi = fs_->make<TH1D>("h_mu_tight_phi",";tight mu #phi", 34, -3.4, 3.4 );
  h_mu_tight_iso = fs_->make<TH1D>("h_mu_tight_iso",";tight mu relative isolation", 220, -0.01, 0.21 );
  h_mu_tight_chHadIso = fs_->make<TH1D>("h_mu_tight_chHadIso",";tight mu charged hadron isolation", 200, -0.01, 10. );
  h_mu_tight_neHadIso = fs_->make<TH1D>("h_mu_tight_neHadIso",";tight mu neutral hadron isolation", 200, -0.01, 20. );
  h_mu_tight_photonIso = fs_->make<TH1D>("h_mu_tight_photonIso",";tight mu photon isolation", 200, -0.01, 20. );
  h_mu_tight_sumPUPtIso = fs_->make<TH1D>("h_mu_tight_sumPUPtIso",";tight mu sum PU pT", 200, -0.01, 20. );

  h_ele_noiso_pt  = fs_->make<TH1D>("h_ele_noiso_pt",";ele p_{T}", NptBins, 0, ptmax );
  h_ele_noiso_eta = fs_->make<TH1D>("h_ele_noiso_eta",";ele #eta", 25, -2.5, 2.5 );
  h_ele_noiso_phi = fs_->make<TH1D>("h_ele_noiso_phi",";ele #phi", 34, -3.4, 3.4 );
  h_ele_noiso_iso = fs_->make<TH1D>("h_ele_noiso_iso",";ele relative isolation", 220, -0.01, 0.21 );
  h_ele_noiso_chHadIso = fs_->make<TH1D>("h_ele_noiso_chHadIso",";ele charged hadron isolation", 200, -0.01, 10. );
  h_ele_noiso_neHadIso = fs_->make<TH1D>("h_ele_noiso_neHadIso",";ele neutral hadron isolation", 200, -0.01, 20. );
  h_ele_noiso_photonIso = fs_->make<TH1D>("h_ele_noiso_photonIso",";ele photon isolation", 200, -0.01, 20. );

  h_mu_noiso_pt = fs_->make<TH1D>("h_mu_noiso_pt",";mu p_{T}", NptBins, 0, ptmax );
  h_mu_noiso_eta = fs_->make<TH1D>("h_mu_noiso_eta",";mu #eta", 25, -2.5, 2.5 );
  h_mu_noiso_phi = fs_->make<TH1D>("h_mu_noiso_phi",";mu #phi", 34, -3.4, 3.4 );
  h_mu_noiso_iso = fs_->make<TH1D>("h_mu_noiso_iso",";mu relative isolation", 220, -0.01, 0.21 );
  h_mu_noiso_chHadIso = fs_->make<TH1D>("h_mu_noiso_chHadIso",";mu charged hadron isolation", 200, -0.01, 10. );
  h_mu_noiso_neHadIso = fs_->make<TH1D>("h_mu_noiso_neHadIso",";mu neutral hadron isolation", 200, -0.01, 20. );
  h_mu_noiso_photonIso = fs_->make<TH1D>("h_mu_noiso_photonIso",";mu photon isolation", 200, -0.01, 20. );
  h_mu_noiso_sumPUPtIso = fs_->make<TH1D>("h_mu_noiso_sumPUPtIso",";mu sum PU pT", 200, -0.01, 20. );

  h_met_pt = fs_->make<TH1D>("h_met_pt",";met p_{T}", NptBins, 0, ptmax );
  h_met_phi = fs_->make<TH1D>("h_met_phi",";met #phi", 34, -3.4, 3.4 );

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

  h_dR_tight_ele_jet = fs_->make<TH1D>("h_dR_tight_ele_jet",";#DeltaR(tight e,jet)", 100, 0, 6.3 );
  h_dR_tight_mu_jet = fs_->make<TH1D>("h_dR_tight_mu_jet",";#DeltaR(tight #mu,jet)", 100, 0, 6.3 );


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


  h_pfcand_energy = fs_->make<TH1D>("h_pfcand_energy",";PFCand energy", 1000, 0, 1000 );
  h_pfcand_pt = fs_->make<TH1D>("h_pfcand_pt",";PFCand p_{T}", 500, 0, 500 );
  h_pfcand_eta = fs_->make<TH1D>("h_pfcand_eta",";PFCand #eta", 70, -3.5, 3.5 );
  h_pfcand_phi = fs_->make<TH1D>("h_pfcand_phi",";PFCand #phi", 30, -3.0, 3.0 );
  h_pfcand_fromPV = fs_->make<TH1D>("h_pfcand_fromPV",";PFCand fromPV", 4, 0, 4 );
  h_pfcand_pdgId = fs_->make<TH1D>("h_pfcand_pdgId",";PFCand pdgId", 7, 0, 7 );

  h_pfcand_pdgId->GetXaxis()->SetBinLabel(1,"Charged hadron");
  h_pfcand_pdgId->GetXaxis()->SetBinLabel(2,"Neutral hadron");
  h_pfcand_pdgId->GetXaxis()->SetBinLabel(3,"Photon");
  h_pfcand_pdgId->GetXaxis()->SetBinLabel(4,"Electron");
  h_pfcand_pdgId->GetXaxis()->SetBinLabel(5,"Muon");
  h_pfcand_pdgId->GetXaxis()->SetBinLabel(6,"Hadron in HF");
  h_pfcand_pdgId->GetXaxis()->SetBinLabel(7,"EM in HF");


  h_pfcand_chargedHadronEnergy = fs_->make<TH1D>("h_pfcand_chargedHadronEnergy",";PFCand chargedHadronEnergy", 400, 0, 4000 );
  h_pfcand_neutralHadronEnergy = fs_->make<TH1D>("h_pfcand_neutralHadronEnergy",";PFCand neutralHadronEnergy", 400, 0, 4000 );
  h_pfcand_photonEnergy = fs_->make<TH1D>("h_pfcand_photonEnergy",";PFCand photonEnergy", 400, 0, 4000 );
  h_pfcand_electronEnergy = fs_->make<TH1D>("h_pfcand_electronEnergy",";PFCand electronEnergy", 500, 0, 500 );
  h_pfcand_muonEnergy = fs_->make<TH1D>("h_pfcand_muonEnergy",";PFCand muonEnergy", 500, 0, 500 );
  h_pfcand_HFHadronEnergy = fs_->make<TH1D>("h_pfcand_HFHadronEnergy",";PFCand HFHadronEnergy", 200, 0, 20000 );
  h_pfcand_HFEMEnergy = fs_->make<TH1D>("h_pfcand_HFEMEnergy",";PFCand HFEMEnergy", 200, 0, 2000 );
  h_pfcand_chargedHadronMultiplicity = fs_->make<TH1D>("h_pfcand_chargedHadronMultiplicity",";PFCand chargedHadronMultiplicity", 2000, 0, 2000 );
  h_pfcand_neutralHadronMultiplicity = fs_->make<TH1D>("h_pfcand_neutralHadronMultiplicity",";PFCand neutralHadronMultiplicity", 300, 0, 300 );
  h_pfcand_photonMultiplicity = fs_->make<TH1D>("h_pfcand_photonMultiplicity",";PFCand photonMultiplicity", 2000, 0, 2000 );
  h_pfcand_electronMultiplicity = fs_->make<TH1D>("h_pfcand_electronMultiplicity",";PFCand electronMultiplicity", 15, 0, 15 );
  h_pfcand_muonMultiplicity = fs_->make<TH1D>("h_pfcand_muonMultiplicity",";PFCand muonMultiplicity", 15, 0, 15 );
  h_pfcand_HFHadronMultiplicity = fs_->make<TH1D>("h_pfcand_HFHadronMultiplicity",";PFCand HFHadronMultiplicity", 1000, 0, 1000 );
  h_pfcand_HFEMMultiplicity = fs_->make<TH1D>("h_pfcand_HFEMMultiplicity",";PFCand HFEMMultiplicity", 1000, 0, 1000 );

  h_pfcand_chargedEmEnergy = fs_->make<TH1D>("h_pfcand_chargedEmEnergy",";PFCand chargedEmEnergy", 500, 0, 500 );
  h_pfcand_neutralEmEnergy = fs_->make<TH1D>("h_pfcand_neutralEmEnergy",";PFCand neutralEmEnergy", 400, 0, 4000 );
  h_pfcand_chargedMuEnergy = fs_->make<TH1D>("h_pfcand_chargedMuEnergy",";PFCand chargedMuEnergy", 500, 0, 500 );
  h_pfcand_chargedMultiplicity = fs_->make<TH1D>("h_pfcand_chargedMultiplicity",";PFCand chargedMultiplicity", 2000, 0, 2000 );
  h_pfcand_neutralMultiplicity = fs_->make<TH1D>("h_pfcand_neutralMultiplicity",";PFCand neutralMultiplicity", 3000, 0, 3000 );


  h_pfcand_pt0to2_chargedHadronEnergy = fs_->make<TH1D>("h_pfcand_pt0to2_chargedHadronEnergy",";PFCand chargedHadronEnergy (p_{T} < 2 GeV)", 400, 0, 4000 );
  h_pfcand_pt0to2_neutralHadronEnergy = fs_->make<TH1D>("h_pfcand_pt0to2_neutralHadronEnergy",";PFCand neutralHadronEnergy (p_{T} < 2 GeV)", 400, 0, 4000 );
  h_pfcand_pt0to2_photonEnergy = fs_->make<TH1D>("h_pfcand_pt0to2_photonEnergy",";PFCand photonEnergy (p_{T} < 2 GeV)", 400, 0, 4000 );
  h_pfcand_pt0to2_chargedHadronMultiplicity = fs_->make<TH1D>("h_pfcand_pt0to2_chargedHadronMultiplicity",";PFCand chargedHadronMultiplicity (p_{T} < 2 GeV)", 2000, 0, 2000 );
  h_pfcand_pt0to2_neutralHadronMultiplicity = fs_->make<TH1D>("h_pfcand_pt0to2_neutralHadronMultiplicity",";PFCand neutralHadronMultiplicity (p_{T} < 2 GeV)", 300, 0, 300 );
  h_pfcand_pt0to2_photonMultiplicity = fs_->make<TH1D>("h_pfcand_pt0to2_photonMultiplicity",";PFCand photonMultiplicity (p_{T} < 2 GeV)", 2000, 0, 2000 );

  h_pfcand_pt2toInf_chargedHadronEnergy = fs_->make<TH1D>("h_pfcand_pt2toInf_chargedHadronEnergy",";PFCand chargedHadronEnergy (p_{T} > 2 GeV)", 400, 0, 4000 );
  h_pfcand_pt2toInf_neutralHadronEnergy = fs_->make<TH1D>("h_pfcand_pt2toInf_neutralHadronEnergy",";PFCand neutralHadronEnergy (p_{T} > 2 GeV)", 400, 0, 4000 );
  h_pfcand_pt2toInf_photonEnergy = fs_->make<TH1D>("h_pfcand_pt2toInf_photonEnergy",";PFCand photonEnergy (p_{T} > 2 GeV)", 400, 0, 4000 );
  h_pfcand_pt2toInf_chargedHadronMultiplicity = fs_->make<TH1D>("h_pfcand_pt2toInf_chargedHadronMultiplicity",";PFCand chargedHadronMultiplicity (p_{T} > 2 GeV)", 2000, 0, 2000 );
  h_pfcand_pt2toInf_neutralHadronMultiplicity = fs_->make<TH1D>("h_pfcand_pt2toInf_neutralHadronMultiplicity",";PFCand neutralHadronMultiplicity (p_{T} > 2 GeV)", 300, 0, 300 );
  h_pfcand_pt2toInf_photonMultiplicity = fs_->make<TH1D>("h_pfcand_pt2toInf_photonMultiplicity",";PFCand photonMultiplicity (p_{T} > 2 GeV)", 2000, 0, 2000 );

  h_pfcand_pt5toInf_chargedHadronEnergy = fs_->make<TH1D>("h_pfcand_pt5toInf_chargedHadronEnergy",";PFCand chargedHadronEnergy (p_{T} > 5 GeV)", 2000, 0, 2000 );
  h_pfcand_pt5toInf_neutralHadronEnergy = fs_->make<TH1D>("h_pfcand_pt5toInf_neutralHadronEnergy",";PFCand neutralHadronEnergy (p_{T} > 5 GeV)", 1000, 0, 1000 );
  h_pfcand_pt5toInf_photonEnergy = fs_->make<TH1D>("h_pfcand_pt5toInf_photonEnergy",";PFCand photonEnergy (p_{T} > 5 GeV)", 2000, 0, 2000 );
  h_pfcand_pt5toInf_chargedHadronMultiplicity = fs_->make<TH1D>("h_pfcand_pt5toInf_chargedHadronMultiplicity",";PFCand chargedHadronMultiplicity (p_{T} > 5 GeV)", 100, 0, 100 );
  h_pfcand_pt5toInf_neutralHadronMultiplicity = fs_->make<TH1D>("h_pfcand_pt5toInf_neutralHadronMultiplicity",";PFCand neutralHadronMultiplicity (p_{T} > 5 GeV)", 50, 0, 50 );
  h_pfcand_pt5toInf_photonMultiplicity = fs_->make<TH1D>("h_pfcand_pt5toInf_photonMultiplicity",";PFCand photonMultiplicity (p_{T} > 5 GeV)", 50, 0, 50 );


  h_pfcand_eta0to1p5_chargedHadronEnergy = fs_->make<TH1D>("h_pfcand_eta0to1p5_chargedHadronEnergy",";PFCand chargedHadronEnergy (|#eta| < 1.5)", 400, 0, 4000 );
  h_pfcand_eta0to1p5_neutralHadronEnergy = fs_->make<TH1D>("h_pfcand_eta0to1p5_neutralHadronEnergy",";PFCand neutralHadronEnergy (|#eta| < 1.5)", 400, 0, 4000 );
  h_pfcand_eta0to1p5_photonEnergy = fs_->make<TH1D>("h_pfcand_eta0to1p5_photonEnergy",";PFCand photonEnergy (|#eta| < 1.5)", 400, 0, 4000 );
  h_pfcand_eta0to1p5_chargedHadronMultiplicity = fs_->make<TH1D>("h_pfcand_eta0to1p5_chargedHadronMultiplicity",";PFCand chargedHadronMultiplicity (|#eta| < 1.5)", 2000, 0, 2000 );
  h_pfcand_eta0to1p5_neutralHadronMultiplicity = fs_->make<TH1D>("h_pfcand_eta0to1p5_neutralHadronMultiplicity",";PFCand neutralHadronMultiplicity (|#eta| < 1.5)", 300, 0, 300 );
  h_pfcand_eta0to1p5_photonMultiplicity = fs_->make<TH1D>("h_pfcand_eta0to1p5_photonMultiplicity",";PFCand photonMultiplicity (|#eta| < 1.5)", 2000, 0, 2000 );

  h_pfcand_eta1p5to2p5_chargedHadronEnergy = fs_->make<TH1D>("h_pfcand_eta1p5to2p5_chargedHadronEnergy",";PFCand chargedHadronEnergy (1.5 < |#eta| < 2.5)", 400, 0, 4000 );
  h_pfcand_eta1p5to2p5_neutralHadronEnergy = fs_->make<TH1D>("h_pfcand_eta1p5to2p5_neutralHadronEnergy",";PFCand neutralHadronEnergy (1.5 < |#eta| < 2.5)", 400, 0, 4000 );
  h_pfcand_eta1p5to2p5_photonEnergy = fs_->make<TH1D>("h_pfcand_eta1p5to2p5_photonEnergy",";PFCand photonEnergy (1.5 < |#eta| < 2.5)", 400, 0, 4000 );
  h_pfcand_eta1p5to2p5_chargedHadronMultiplicity = fs_->make<TH1D>("h_pfcand_eta1p5to2p5_chargedHadronMultiplicity",";PFCand chargedHadronMultiplicity (1.5 < |#eta| < 2.5)", 2000, 0, 2000 );
  h_pfcand_eta1p5to2p5_neutralHadronMultiplicity = fs_->make<TH1D>("h_pfcand_eta1p5to2p5_neutralHadronMultiplicity",";PFCand neutralHadronMultiplicity (1.5 < |#eta| < 2.5)", 300, 0, 300 );
  h_pfcand_eta1p5to2p5_photonMultiplicity = fs_->make<TH1D>("h_pfcand_eta1p5to2p5_photonMultiplicity",";PFCand photonMultiplicity (1.5 < |#eta| < 2.5)", 2000, 0, 2000 );


  std::string era = "2012_53x";
  insample_ = 2500;

  mySample_xSec_ = 689.1;
  mySample_nGen_ = 25474122;
  intLumi_ = 20000;

  analysisType::analysisType iAnalysisType = analysisType::LJ;
  bool isData = true;

  miniAODhelper.SetUp(era, insample_, iAnalysisType, isData);

}


TTHMiniAODAnalyzer::~TTHMiniAODAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TTHMiniAODAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  numEvents_++;


  double minTightLeptonPt = 30.;


  h_hlt->Fill(0.,1);
  h_flt->Fill(0.,1);

  edm::Handle<edm::TriggerResults> triggerResults;
  iEvent.getByToken(triggerResultsToken, triggerResults);

  if( triggerResults.isValid() ){
    std::vector<std::string> triggerNames = hlt_config_.triggerNames();

    for( unsigned int iPath=0; iPath<triggerNames.size(); iPath++ ){
      std::string pathName = triggerNames[iPath];
      unsigned int hltIndex = hlt_config_.triggerIndex(pathName);

      if( hltIndex >= triggerResults->size() ) continue;

      int accept = triggerResults->accept(hltIndex);
      int prescale = -1;//hlt_config_.prescaleValue(iEvent, iSetup, pathName);

      if( verbose_ && dumpHLT_ ) std::cout << " =====>  HLT: path name = " << pathName << ",\t prescale = " << prescale << ",\t pass = " << accept << std::endl; 

      std::string pathNameNoVer = hlt_config_.removeVersion(pathName);

      if( accept ) hlt_cppath_[pathNameNoVer]+=1;

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
    std::cout << "Trigger results not valid for tag " << hltTag << std::endl;
  }


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



  /////////
  ///
  /// Packed PF Candidates
  ///
  ////////
  float chargedHadronEnergy=0.;
  float neutralHadronEnergy=0.;
  float photonEnergy=0.;
  float electronEnergy=0.;
  float muonEnergy=0.;
  float HFHadronEnergy=0.;
  float HFEMEnergy=0.;
  int   chargedHadronMultiplicity=0;
  int   neutralHadronMultiplicity=0;
  int   photonMultiplicity=0;
  int   electronMultiplicity=0;
  int   muonMultiplicity=0;
  int   HFHadronMultiplicity=0;
  int   HFEMMultiplicity=0;

  float chargedEmEnergy=0.;
  float neutralEmEnergy=0.;
  float chargedMuEnergy=0.;
  int   chargedMultiplicity=0;
  int   neutralMultiplicity=0;

  float chargedHadronEnergy_pt0to2=0.;
  float neutralHadronEnergy_pt0to2=0.;
  float photonEnergy_pt0to2=0.;
  int   chargedHadronMultiplicity_pt0to2=0;
  int   neutralHadronMultiplicity_pt0to2=0;
  int   photonMultiplicity_pt0to2=0;

  float chargedHadronEnergy_pt2toInf=0.;
  float neutralHadronEnergy_pt2toInf=0.;
  float photonEnergy_pt2toInf=0.;
  int   chargedHadronMultiplicity_pt2toInf=0;
  int   neutralHadronMultiplicity_pt2toInf=0;
  int   photonMultiplicity_pt2toInf=0;

  float chargedHadronEnergy_pt5toInf=0.;
  float neutralHadronEnergy_pt5toInf=0.;
  float photonEnergy_pt5toInf=0.;
  int   chargedHadronMultiplicity_pt5toInf=0;
  int   neutralHadronMultiplicity_pt5toInf=0;
  int   photonMultiplicity_pt5toInf=0;

  float chargedHadronEnergy_eta0to1p5=0.;
  float neutralHadronEnergy_eta0to1p5=0.;
  float photonEnergy_eta0to1p5=0.;
  int   chargedHadronMultiplicity_eta0to1p5=0;
  int   neutralHadronMultiplicity_eta0to1p5=0;
  int   photonMultiplicity_eta0to1p5=0;

  float chargedHadronEnergy_eta1p5to2p5=0.;
  float neutralHadronEnergy_eta1p5to2p5=0.;
  float photonEnergy_eta1p5to2p5=0.;
  int   chargedHadronMultiplicity_eta1p5to2p5=0;
  int   neutralHadronMultiplicity_eta1p5to2p5=0;
  int   photonMultiplicity_eta1p5to2p5=0;


  for( std::vector<pat::PackedCandidate>::const_iterator pfCand = packedPFcands->begin(); pfCand!=packedPFcands->end(); ++pfCand ){

    if( pfCand->pt()<0.2 ) continue;
    if( fabs(pfCand->eta())>2.5 ) continue;
    if( !(pfCand->fromPV()>=1) ) continue;
    // printf(" PFcand %d:\t pdgId = %d,\t fromPV = %d,\t pt = %.1f,\t eta = %.2f,\t phi = %.2f \n",
    // 	   int( pfCand - packedPFcands->begin() ), pfCand->pdgId(), pfCand->fromPV(), pfCand->pt(), pfCand->eta(), pfCand->phi() );


    int convertedId = -1;
    switch( std::abs(pfCand->pdgId()) ){
    case 211: //PFCandidate::h:       // charged hadron
      convertedId = 0;
      break;
    case 130: //PFCandidate::h0 :    // neutral hadron
      convertedId = 1;
      break;
    case 22: //PFCandidate::gamma:   // photon
      convertedId = 2;
      break;
    case 11: // PFCandidate::e:       // electron 
      convertedId = 3;
      break;
    case 13: //PFCandidate::mu:      // muon
      convertedId = 4;
      break;
    case 1: // PFCandidate::h_HF :    // hadron in HF
      convertedId = 5;
      break;
    case 2: //PFCandidate::egamma_HF :    // electromagnetic in HF
      convertedId = 6;
      break;
    default:
      std::cout << " !! pdgId ignored!! pdgId = " << pfCand->pdgId() << std::endl;
      break;
    }

    h_pfcand_energy->Fill(pfCand->energy(),wgt);
    h_pfcand_pt->Fill(pfCand->pt(),wgt);
    h_pfcand_eta->Fill(pfCand->eta(),wgt);
    h_pfcand_phi->Fill(pfCand->phi(),wgt);
    h_pfcand_fromPV->Fill(pfCand->fromPV(),wgt);

    h_pfcand_pdgId->Fill(convertedId,wgt);
 
    float temp_chargedHadronEnergy=0.;
    float temp_neutralHadronEnergy=0.;
    float temp_photonEnergy=0.;
    float temp_electronEnergy=0.;
    float temp_muonEnergy=0.;
    float temp_HFHadronEnergy=0.;
    float temp_HFEMEnergy=0.;
    int   temp_chargedHadronMultiplicity=0;
    int   temp_neutralHadronMultiplicity=0;
    int   temp_photonMultiplicity=0;
    int   temp_electronMultiplicity=0;
    int   temp_muonMultiplicity=0;
    int   temp_HFHadronMultiplicity=0;
    int   temp_HFEMMultiplicity=0;

    float temp_chargedEmEnergy=0.;
    float temp_neutralEmEnergy=0.;
    float temp_chargedMuEnergy=0.;
    int   temp_chargedMultiplicity=0;
    int   temp_neutralMultiplicity=0;

    switch (std::abs(pfCand->pdgId())) {
    case 211: //PFCandidate::h:       // charged hadron
      temp_chargedHadronEnergy += pfCand->energy();
      temp_chargedHadronMultiplicity++;
      temp_chargedMultiplicity++;
      break;

    case 130: //PFCandidate::h0 :    // neutral hadron
      temp_neutralHadronEnergy += pfCand->energy();
      temp_neutralHadronMultiplicity++;
      temp_neutralMultiplicity++;
      break;

    case 22: //PFCandidate::gamma:   // photon
      temp_photonEnergy += pfCand->energy();
      temp_photonMultiplicity++;
      temp_neutralEmEnergy += pfCand->energy();
      temp_neutralMultiplicity++;
      break;

    case 11: // PFCandidate::e:       // electron 
      temp_electronEnergy += pfCand->energy();
      temp_electronMultiplicity++;
      temp_chargedEmEnergy += pfCand->energy(); 
      temp_chargedMultiplicity++;
      break;

    case 13: //PFCandidate::mu:      // muon
      temp_muonEnergy += pfCand->energy();
      temp_muonMultiplicity++;
      temp_chargedMuEnergy += pfCand->energy();
      temp_chargedMultiplicity++;
      break;

    case 1: // PFCandidate::h_HF :    // hadron in HF
      temp_HFHadronEnergy += pfCand->energy();
      temp_HFHadronMultiplicity++;
      temp_neutralMultiplicity++;
      break;

    case 2: //PFCandidate::egamma_HF :    // electromagnetic in HF
      temp_HFEMEnergy += pfCand->energy();
      temp_HFEMMultiplicity++;
      temp_neutralEmEnergy += pfCand->energy();
      temp_neutralMultiplicity++;
      break;
	
    default:
      std::cout << " !! pdgId ignored!! pdgId = " << pfCand->pdgId() << std::endl;
      break;
    }

    chargedHadronEnergy += temp_chargedHadronEnergy;
    neutralHadronEnergy += temp_neutralHadronEnergy;
    photonEnergy += temp_photonEnergy;
    electronEnergy += temp_electronEnergy;
    muonEnergy += temp_muonEnergy;
    HFHadronEnergy += temp_HFHadronEnergy;
    HFEMEnergy += temp_HFEMEnergy;
    chargedHadronMultiplicity += temp_chargedHadronMultiplicity;
    neutralHadronMultiplicity += temp_neutralHadronMultiplicity;
    photonMultiplicity += temp_photonMultiplicity;
    electronMultiplicity += temp_electronMultiplicity;
    muonMultiplicity += temp_muonMultiplicity;
    HFHadronMultiplicity += temp_HFHadronMultiplicity;
    HFEMMultiplicity += temp_HFEMMultiplicity;

    chargedEmEnergy += temp_chargedEmEnergy;
    neutralEmEnergy += temp_neutralEmEnergy;
    chargedMuEnergy += temp_chargedMuEnergy;
    chargedMultiplicity += temp_chargedMultiplicity;
    neutralMultiplicity += temp_neutralMultiplicity;


    if( pfCand->pt()<2. ){
      chargedHadronEnergy_pt0to2 += temp_chargedHadronEnergy;
      neutralHadronEnergy_pt0to2 += temp_neutralHadronEnergy;
      photonEnergy_pt0to2 += temp_photonEnergy;
      chargedHadronMultiplicity_pt0to2 += temp_chargedHadronMultiplicity;
      neutralHadronMultiplicity_pt0to2 += temp_neutralHadronMultiplicity;
      photonMultiplicity_pt0to2 += temp_photonMultiplicity;
    }
    else{
      chargedHadronEnergy_pt2toInf += temp_chargedHadronEnergy;
      neutralHadronEnergy_pt2toInf += temp_neutralHadronEnergy;
      photonEnergy_pt2toInf += temp_photonEnergy;
      chargedHadronMultiplicity_pt2toInf += temp_chargedHadronMultiplicity;
      neutralHadronMultiplicity_pt2toInf += temp_neutralHadronMultiplicity;
      photonMultiplicity_pt2toInf += temp_photonMultiplicity;
    }

    if( pfCand->pt()>5. ){
      chargedHadronEnergy_pt5toInf += temp_chargedHadronEnergy;
      neutralHadronEnergy_pt5toInf += temp_neutralHadronEnergy;
      photonEnergy_pt5toInf += temp_photonEnergy;
      chargedHadronMultiplicity_pt5toInf += temp_chargedHadronMultiplicity;
      neutralHadronMultiplicity_pt5toInf += temp_neutralHadronMultiplicity;
      photonMultiplicity_pt5toInf += temp_photonMultiplicity;
    }

    if( fabs(pfCand->eta())<1.5 ){
      chargedHadronEnergy_eta0to1p5 += temp_chargedHadronEnergy;
      neutralHadronEnergy_eta0to1p5 += temp_neutralHadronEnergy;
      photonEnergy_eta0to1p5 += temp_photonEnergy;
      chargedHadronMultiplicity_eta0to1p5 += temp_chargedHadronMultiplicity;
      neutralHadronMultiplicity_eta0to1p5 += temp_neutralHadronMultiplicity;
      photonMultiplicity_eta0to1p5 += temp_photonMultiplicity;
    }
    else if( fabs(pfCand->eta())<2.5 ){
      chargedHadronEnergy_eta1p5to2p5 += temp_chargedHadronEnergy;
      neutralHadronEnergy_eta1p5to2p5 += temp_neutralHadronEnergy;
      photonEnergy_eta1p5to2p5 += temp_photonEnergy;
      chargedHadronMultiplicity_eta1p5to2p5 += temp_chargedHadronMultiplicity;
      neutralHadronMultiplicity_eta1p5to2p5 += temp_neutralHadronMultiplicity;
      photonMultiplicity_eta1p5to2p5 += temp_photonMultiplicity;
    }


  }



  h_pfcand_chargedHadronEnergy->Fill(chargedHadronEnergy,wgt);
  h_pfcand_neutralHadronEnergy->Fill(neutralHadronEnergy,wgt);
  h_pfcand_photonEnergy->Fill(photonEnergy,wgt);
  h_pfcand_electronEnergy->Fill(electronEnergy,wgt);
  h_pfcand_muonEnergy->Fill(muonEnergy,wgt);
  h_pfcand_HFHadronEnergy->Fill(HFHadronEnergy,wgt);
  h_pfcand_HFEMEnergy->Fill(HFEMEnergy,wgt);
  h_pfcand_chargedHadronMultiplicity->Fill(chargedHadronMultiplicity,wgt);
  h_pfcand_neutralHadronMultiplicity->Fill(neutralHadronMultiplicity,wgt);
  h_pfcand_photonMultiplicity->Fill(photonMultiplicity,wgt);
  h_pfcand_electronMultiplicity->Fill(electronMultiplicity,wgt);
  h_pfcand_muonMultiplicity->Fill(muonMultiplicity,wgt);
  h_pfcand_HFHadronMultiplicity->Fill(HFHadronMultiplicity,wgt);
  h_pfcand_HFEMMultiplicity->Fill(HFEMMultiplicity,wgt);

  h_pfcand_chargedEmEnergy->Fill(chargedEmEnergy,wgt);
  h_pfcand_neutralEmEnergy->Fill(neutralEmEnergy,wgt);
  h_pfcand_chargedMuEnergy->Fill(chargedMuEnergy,wgt);
  h_pfcand_chargedMultiplicity->Fill(chargedMultiplicity,wgt);
  h_pfcand_neutralMultiplicity->Fill(neutralMultiplicity,wgt);


  h_pfcand_pt0to2_chargedHadronEnergy->Fill(chargedHadronEnergy_pt0to2,wgt);
  h_pfcand_pt0to2_neutralHadronEnergy->Fill(neutralHadronEnergy_pt0to2,wgt);
  h_pfcand_pt0to2_photonEnergy->Fill(photonEnergy_pt0to2,wgt);
  h_pfcand_pt0to2_chargedHadronMultiplicity->Fill(chargedHadronMultiplicity_pt0to2,wgt);
  h_pfcand_pt0to2_neutralHadronMultiplicity->Fill(neutralHadronMultiplicity_pt0to2,wgt);
  h_pfcand_pt0to2_photonMultiplicity->Fill(photonMultiplicity_pt0to2,wgt);

  h_pfcand_pt2toInf_chargedHadronEnergy->Fill(chargedHadronEnergy_pt2toInf,wgt);
  h_pfcand_pt2toInf_neutralHadronEnergy->Fill(neutralHadronEnergy_pt2toInf,wgt);
  h_pfcand_pt2toInf_photonEnergy->Fill(photonEnergy_pt2toInf,wgt);
  h_pfcand_pt2toInf_chargedHadronMultiplicity->Fill(chargedHadronMultiplicity_pt2toInf,wgt);
  h_pfcand_pt2toInf_neutralHadronMultiplicity->Fill(neutralHadronMultiplicity_pt2toInf,wgt);
  h_pfcand_pt2toInf_photonMultiplicity->Fill(photonMultiplicity_pt2toInf,wgt);

  h_pfcand_pt5toInf_chargedHadronEnergy->Fill(chargedHadronEnergy_pt5toInf,wgt);
  h_pfcand_pt5toInf_neutralHadronEnergy->Fill(neutralHadronEnergy_pt5toInf,wgt);
  h_pfcand_pt5toInf_photonEnergy->Fill(photonEnergy_pt5toInf,wgt);
  h_pfcand_pt5toInf_chargedHadronMultiplicity->Fill(chargedHadronMultiplicity_pt5toInf,wgt);
  h_pfcand_pt5toInf_neutralHadronMultiplicity->Fill(neutralHadronMultiplicity_pt5toInf,wgt);
  h_pfcand_pt5toInf_photonMultiplicity->Fill(photonMultiplicity_pt5toInf,wgt);

  h_pfcand_eta0to1p5_chargedHadronEnergy->Fill(chargedHadronEnergy_eta0to1p5,wgt);
  h_pfcand_eta0to1p5_neutralHadronEnergy->Fill(neutralHadronEnergy_eta0to1p5,wgt);
  h_pfcand_eta0to1p5_photonEnergy->Fill(photonEnergy_eta0to1p5,wgt);
  h_pfcand_eta0to1p5_chargedHadronMultiplicity->Fill(chargedHadronMultiplicity_eta0to1p5,wgt);
  h_pfcand_eta0to1p5_neutralHadronMultiplicity->Fill(neutralHadronMultiplicity_eta0to1p5,wgt);
  h_pfcand_eta0to1p5_photonMultiplicity->Fill(photonMultiplicity_eta0to1p5,wgt);

  h_pfcand_eta1p5to2p5_chargedHadronEnergy->Fill(chargedHadronEnergy_eta1p5to2p5,wgt);
  h_pfcand_eta1p5to2p5_neutralHadronEnergy->Fill(neutralHadronEnergy_eta1p5to2p5,wgt);
  h_pfcand_eta1p5to2p5_photonEnergy->Fill(photonEnergy_eta1p5to2p5,wgt);
  h_pfcand_eta1p5to2p5_chargedHadronMultiplicity->Fill(chargedHadronMultiplicity_eta1p5to2p5,wgt);
  h_pfcand_eta1p5to2p5_neutralHadronMultiplicity->Fill(neutralHadronMultiplicity_eta1p5to2p5,wgt);
  h_pfcand_eta1p5to2p5_photonMultiplicity->Fill(photonMultiplicity_eta1p5to2p5,wgt);

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

    h_mu_noiso_pt->Fill(std::min(double(pfmu->pt()),ptmax)-0.0001,wgt);
    h_mu_noiso_eta->Fill(pfmu->eta(),wgt);
    h_mu_noiso_phi->Fill(pfmu->phi(),wgt);
    h_mu_noiso_iso->Fill(miniAODhelper.GetMuonRelIso(*pfmu),wgt);

    h_mu_noiso_chHadIso->Fill(pfmu->pfIsolationR04().sumChargedHadronPt,wgt);
    h_mu_noiso_neHadIso->Fill(pfmu->pfIsolationR04().sumNeutralHadronEt,wgt);
    h_mu_noiso_photonIso->Fill(pfmu->pfIsolationR04().sumPhotonEt,wgt);
    h_mu_noiso_sumPUPtIso->Fill(pfmu->pfIsolationR04().sumPUPt,wgt);

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

    if( pfele->pt()>30 && fabs(pfele->eta())<2.5 && !inCrack ) h_electron_selection->Fill(0.5+ncut++, 1);
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

    h_ele_noiso_pt->Fill(std::min(double(pfele->pt()),ptmax)-0.0001,wgt);
    h_ele_noiso_eta->Fill(pfele->eta(),wgt);
    h_ele_noiso_phi->Fill(pfele->phi(),wgt);
    h_ele_noiso_iso->Fill(miniAODhelper.GetElectronRelIso(*pfele),wgt);
    h_ele_noiso_chHadIso->Fill(pfele->chargedHadronIso(),wgt);
    h_ele_noiso_neHadIso->Fill(pfele->neutralHadronIso(),wgt);
    h_ele_noiso_photonIso->Fill(pfele->photonIso(),wgt);

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

  std::vector<pat::Jet> selectedJets_nocc_unsorted = miniAODhelper.GetSelectedJets(*pfjets, 30., 2.4, jetID::jetLoose, '-' );

  // Sort jet collections by pT
  std::vector<pat::Jet> selectedJets       = miniAODhelper.GetSortedByPt( selectedJets_unsorted );
  std::vector<pat::Jet> selectedJets_tag   = miniAODhelper.GetSortedByPt( selectedJets_tag_unsorted );

  std::vector<pat::Jet> selectedJets_nocc = miniAODhelper.GetSortedByPt( selectedJets_nocc_unsorted );


  // Get numJets, numTags
  int numJet = int( selectedJets.size() );
  int numTag = int( selectedJets_tag.size() );

  int numJet_nocc = int( selectedJets_nocc.size() );

         
  // Get Corrected MET
  pat::MET correctedMET = pfmet->front();//miniAODhelper.GetCorrectedMET( pfmets.at(0), pfJets_forMET, iSysType );


  //// Edit below here
  if( mcparticles.isValid() ){

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

    if( dumpGen ){
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

  h_met_pt->Fill(std::min(double(correctedMET.pt()),jetptmax)-0.0001,wgt);
  h_met_phi->Fill(correctedMET.phi(),wgt);

  h_numJet_nocc->Fill(std::min(numJet_nocc,NjetMax),wgt);


  // Loop over jets
  for( std::vector<pat::Jet>::const_iterator iJet = selectedJets.begin(); iJet != selectedJets.end(); iJet++ ){ 
    double csv = iJet->bDiscriminator("combinedSecondaryVertexBJetTags");
    if( csv>-5 && csv<-0.5 ) csv = -0.2;
    if( csv<-5 )             csv = -0.4;

    int flavour = abs( iJet->partonFlavour() );

    h_jet_pt->Fill(std::min(double(iJet->pt()),jetptmax)-0.0001,wgt);
    h_jet_eta->Fill(iJet->eta(),wgt);
    h_jet_phi->Fill(iJet->phi(),wgt);
    h_jet_mass->Fill(std::min(double(iJet->mass()),jetptmax)-0.0001,wgt);

    h_jet_chargedHadronEnergyFraction->Fill(iJet->chargedHadronEnergyFraction(),wgt);
    h_jet_neutralHadronEnergyFraction->Fill(iJet->neutralHadronEnergyFraction(),wgt);
    h_jet_chargedEmEnergyFraction->Fill(iJet->chargedEmEnergyFraction(),wgt);
    h_jet_neutralEmEnergyFraction->Fill(iJet->neutralEmEnergyFraction(),wgt);
    h_jet_chargedMultiplicity->Fill(iJet->chargedMultiplicity(),wgt);
    h_jet_neutralMultiplicity->Fill(iJet->neutralMultiplicity(),wgt);
    h_jet_numberOfDaughters->Fill(iJet->numberOfDaughters(),wgt);

    double jet_secVtx_mass = iJet->userFloat("vtxMass");
    double jet_secVtx_Ntracks = iJet->userFloat("vtxNtracks");
    double jet_secVtx_3DVal = iJet->userFloat("vtx3DVal");
    double jet_secVtx_3DSig = iJet->userFloat("vtx3DSig");


    h_jet_csv->Fill(csv,wgt);
    h_jet_secVtx_mass->Fill(jet_secVtx_mass,wgt);
    h_jet_secVtx_Ntracks->Fill(jet_secVtx_Ntracks,wgt);
    h_jet_secVtx_3DVal->Fill(jet_secVtx_3DVal,wgt);
    h_jet_secVtx_3DSig->Fill(jet_secVtx_3DSig,wgt);
    if( flavour==5 ){
      h_jet_csv_b->Fill(csv,wgt);
      h_jet_secVtx_mass_b->Fill(jet_secVtx_mass,wgt);
      h_jet_secVtx_Ntracks_b->Fill(jet_secVtx_Ntracks,wgt);
      h_jet_secVtx_3DVal_b->Fill(jet_secVtx_3DVal,wgt);
      h_jet_secVtx_3DSig_b->Fill(jet_secVtx_3DSig,wgt);
    }
    else if( flavour==4 ){
      h_jet_csv_c->Fill(csv,wgt);
      h_jet_secVtx_mass_c->Fill(jet_secVtx_mass,wgt);
      h_jet_secVtx_Ntracks_c->Fill(jet_secVtx_Ntracks,wgt);
      h_jet_secVtx_3DVal_c->Fill(jet_secVtx_3DVal,wgt);
      h_jet_secVtx_3DSig_c->Fill(jet_secVtx_3DSig,wgt);
    }
    else if( (flavour<=3 && flavour!=0) || flavour==21 ){
      h_jet_csv_l->Fill(csv,wgt);
      h_jet_secVtx_mass_l->Fill(jet_secVtx_mass,wgt);
      h_jet_secVtx_Ntracks_l->Fill(jet_secVtx_Ntracks,wgt);
      h_jet_secVtx_3DVal_l->Fill(jet_secVtx_3DVal,wgt);
      h_jet_secVtx_3DSig_l->Fill(jet_secVtx_3DSig,wgt);
    }
    else{
      h_jet_csv_o->Fill(csv,wgt);
      h_jet_secVtx_mass_o->Fill(jet_secVtx_mass,wgt);
      h_jet_secVtx_Ntracks_o->Fill(jet_secVtx_Ntracks,wgt);
      h_jet_secVtx_3DVal_o->Fill(jet_secVtx_3DVal,wgt);
      h_jet_secVtx_3DSig_o->Fill(jet_secVtx_3DSig,wgt);
    }


    if( csv > 0.898 ){
      h_jet_CSVT_secVtx_mass->Fill(jet_secVtx_mass,wgt);
      h_jet_CSVT_secVtx_Ntracks->Fill(jet_secVtx_Ntracks,wgt);
      h_jet_CSVT_secVtx_3DVal->Fill(jet_secVtx_3DVal,wgt);
      h_jet_CSVT_secVtx_3DSig->Fill(jet_secVtx_3DSig,wgt);
      if( flavour==5 ){
	h_jet_CSVT_secVtx_mass_b->Fill(jet_secVtx_mass,wgt);
	h_jet_CSVT_secVtx_Ntracks_b->Fill(jet_secVtx_Ntracks,wgt);
	h_jet_CSVT_secVtx_3DVal_b->Fill(jet_secVtx_3DVal,wgt);
	h_jet_CSVT_secVtx_3DSig_b->Fill(jet_secVtx_3DSig,wgt);
      }
      else if( flavour==4 ){
	h_jet_CSVT_secVtx_mass_c->Fill(jet_secVtx_mass,wgt);
	h_jet_CSVT_secVtx_Ntracks_c->Fill(jet_secVtx_Ntracks,wgt);
	h_jet_CSVT_secVtx_3DVal_c->Fill(jet_secVtx_3DVal,wgt);
	h_jet_CSVT_secVtx_3DSig_c->Fill(jet_secVtx_3DSig,wgt);
      }
      else if( (flavour<=3 && flavour!=0) || flavour==21 ){
	h_jet_CSVT_secVtx_mass_l->Fill(jet_secVtx_mass,wgt);
	h_jet_CSVT_secVtx_Ntracks_l->Fill(jet_secVtx_Ntracks,wgt);
	h_jet_CSVT_secVtx_3DVal_l->Fill(jet_secVtx_3DVal,wgt);
	h_jet_CSVT_secVtx_3DSig_l->Fill(jet_secVtx_3DSig,wgt);
      }
      else{
	h_jet_CSVT_secVtx_mass_o->Fill(jet_secVtx_mass,wgt);
	h_jet_CSVT_secVtx_Ntracks_o->Fill(jet_secVtx_Ntracks,wgt);
	h_jet_CSVT_secVtx_3DVal_o->Fill(jet_secVtx_3DVal,wgt);
	h_jet_CSVT_secVtx_3DSig_o->Fill(jet_secVtx_3DSig,wgt);
      }
    }

  } // end loop on jets




  // Loop over jets with no cc
  for( std::vector<pat::Jet>::const_iterator iJet = selectedJets_nocc.begin(); iJet != selectedJets_nocc.end(); iJet++ ){ 
    double csv = iJet->bDiscriminator("combinedSecondaryVertexBJetTags");
    if( csv>-5 && csv<-0.5 ) csv = -0.2;
    if( csv<-5 )             csv = -0.4;

    int flavour = abs( iJet->partonFlavour() );

    h_jet_nocc_pt->Fill(std::min(double(iJet->pt()),jetptmax)-0.0001,wgt);
    h_jet_nocc_eta->Fill(iJet->eta(),wgt);
    h_jet_nocc_phi->Fill(iJet->phi(),wgt);
    h_jet_nocc_mass->Fill(std::min(double(iJet->mass()),jetptmax)-0.0001,wgt);

    h_jet_nocc_chargedHadronEnergyFraction->Fill(iJet->chargedHadronEnergyFraction(),wgt);
    h_jet_nocc_neutralHadronEnergyFraction->Fill(iJet->neutralHadronEnergyFraction(),wgt);
    h_jet_nocc_chargedEmEnergyFraction->Fill(iJet->chargedEmEnergyFraction(),wgt);
    h_jet_nocc_neutralEmEnergyFraction->Fill(iJet->neutralEmEnergyFraction(),wgt);
    h_jet_nocc_chargedMultiplicity->Fill(iJet->chargedMultiplicity(),wgt);
    h_jet_nocc_neutralMultiplicity->Fill(iJet->neutralMultiplicity(),wgt);
    h_jet_nocc_numberOfDaughters->Fill(iJet->numberOfDaughters(),wgt);

    h_jet_nocc_csv->Fill(csv,wgt);
    if( flavour==5 ){
      h_jet_nocc_csv_b->Fill(csv,wgt);
    }
    else if( flavour==4 ){
      h_jet_nocc_csv_c->Fill(csv,wgt);
    }
    else if( (flavour<=3 && flavour!=0) || flavour==21 ){
      h_jet_nocc_csv_l->Fill(csv,wgt);
    }
    else{
      h_jet_nocc_csv_o->Fill(csv,wgt);
    }

  } // end loop on jets with no cc



  // Loop over tight muons
  for( std::vector<pat::Muon>::const_iterator iMu = selectedMuons_tight.begin(); iMu != selectedMuons_tight.end(); iMu++ ){ 
    h_mu_tight_pt->Fill(std::min(double(iMu->pt()),ptmax)-0.0001,wgt);
    h_mu_tight_eta->Fill(iMu->eta(),wgt);
    h_mu_tight_phi->Fill(iMu->phi(),wgt);
    h_mu_tight_iso->Fill(miniAODhelper.GetMuonRelIso(*iMu),wgt);

    h_mu_tight_chHadIso->Fill(iMu->pfIsolationR04().sumChargedHadronPt,wgt);
    h_mu_tight_neHadIso->Fill(iMu->pfIsolationR04().sumNeutralHadronEt,wgt);
    h_mu_tight_photonIso->Fill(iMu->pfIsolationR04().sumPhotonEt,wgt);
    h_mu_tight_sumPUPtIso->Fill(iMu->pfIsolationR04().sumPUPt,wgt);

    for( std::vector<pat::Jet>::const_iterator iJet = selectedJets.begin(); iJet != selectedJets.end(); iJet++ ){ 
      double deltaR = reco::deltaR( iMu->eta(), iMu->phi(), iJet->eta(), iJet->phi() );
      h_dR_tight_mu_jet->Fill(deltaR,wgt);
    }
  } // end loop on muons

  // Loop over tight electrons
  for( std::vector<pat::Electron>::const_iterator iEle = selectedElectrons_tight.begin(); iEle != selectedElectrons_tight.end(); iEle++ ){ 
    h_ele_tight_pt->Fill(std::min(double(iEle->pt()),ptmax)-0.0001,wgt);
    h_ele_tight_eta->Fill(iEle->eta(),wgt);
    h_ele_tight_phi->Fill(iEle->phi(),wgt);
    h_ele_tight_iso->Fill(miniAODhelper.GetElectronRelIso(*iEle),wgt);
    h_ele_tight_chHadIso->Fill(iEle->chargedHadronIso(),wgt);
    h_ele_tight_neHadIso->Fill(iEle->neutralHadronIso(),wgt);
    h_ele_tight_photonIso->Fill(iEle->photonIso(),wgt);

    for( std::vector<pat::Jet>::const_iterator iJet = selectedJets.begin(); iJet != selectedJets.end(); iJet++ ){ 
      double deltaR = reco::deltaR( iEle->eta(), iEle->phi(), iJet->eta(), iJet->phi() );
      h_dR_tight_ele_jet->Fill(deltaR,wgt);
    }
  } // end loop on electrons


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
TTHMiniAODAnalyzer::beginJob()
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
TTHMiniAODAnalyzer::endJob() 
{
  // report on triggers fired
  if( dumpHLT_ ){
    std::cout << "***************************************************************" << std::endl;
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
TTHMiniAODAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
void 
TTHMiniAODAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{

  bool hltchanged = true;
  if (!hlt_config_.init(iRun, iSetup, hltTag, hltchanged)) {
    std::cout << "Warning, didn't find trigger process HLT,\t" << hltTag << std::endl;
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

}

// ------------ method called when ending the processing of a run  ------------
/*
void 
TTHMiniAODAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
TTHMiniAODAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
TTHMiniAODAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TTHMiniAODAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TTHMiniAODAnalyzer);
