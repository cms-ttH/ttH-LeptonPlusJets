#ifndef ttH_LeptonPlusJets_AnalysisCode_YggdrasilEventVars_h
#define ttH_LeptonPlusJets_AnalysisCode_YggdrasilEventVars_h

//
// Dependencies (#includes)
//
#include <iostream>
#include <vector>
#include "TLorentzVector.h"

#ifdef __MAKECINT__
#pragma link C++ class std::vector< TLorentzVector >+; 
#endif

using namespace std;



typedef std::vector<std::vector<double> > vvdouble;
typedef std::vector<std::vector<std::string> > vvstring;
typedef std::vector<double> vdouble;
typedef std::vector<string> vstring;
typedef std::vector<bool> vbool;
typedef std::vector<int> vint;
typedef std::vector< TLorentzVector > vecTLorentzVector;

//
// Utility Class for Handling Event Variables
//

const Int_t rNumSys = 25;

struct yggdrasilEventVars{


  //////////////////////////////////////////////////////////////////////////
  ///  Tree branches/leaves
  //////////////////////////////////////////////////////////////////////////

  explicit yggdrasilEventVars(): passElectronTrigger_(0),passMuonTrigger_(0) { }

  Int_t   passTrigger_;
  Int_t   passElectronTrigger_;
  Int_t   passMuonTrigger_;
  Int_t   leptonType_;
  Int_t   higgsDecayType_;

  Int_t  PassLJ_;
  Int_t  PassDIL_;

  Int_t  OneMuon_;
  Int_t  OneElectron_;
  Int_t  TwoMuon_;
  Int_t  TwoElectron_;
  Int_t  MuonElectron_;
  double mass_leplep_;
  double dR_leplep_;
  Int_t  oppositeLepCharge_;

  Int_t  PassZmask_;

  Int_t  PassZmaskMET_;

  Int_t   matchSingleMuTrigger_;
  Int_t   matchSingleElectronTrigger_;

  int run_;
  int lumi_;
  long evt_;

  double top_pt_;
  double antitop_pt_;

  double Q2ScaleUpWgt_;
  double Q2ScaleDownWgt_;

  int     numTruePV_;
  int     numGenPV_;

  Int_t   numPVs_;
  Int_t   numSys_;

  Float_t tight_lepton_pt_;
  Float_t tight_lepton_eta_;
  Float_t tight_lepton_phi_;
  Float_t tight_lepton_relIso_;
  Int_t   tight_lepton_isMuon_;
  Int_t   tight_lepton_genId_;
  Int_t   tight_lepton_genParentId_;
  Int_t   tight_lepton_genGrandParentId_;

  
  vvdouble lepton_vect_TLV_;
  vint lepton_trkCharge_;
  vint lepton_isMuon_;
  vint lepton_isTight_;
  vint lepton_isLoose_;
  vint lepton_genId_;
  vint lepton_genParentId_;
  vint lepton_genGrandParentId_;
  vdouble lepton_pt_;
  vdouble lepton_eta_;
  vdouble lepton_phi_;
  vdouble lepton_relIso_;
  vdouble lepton_iso_sumChargedHadronPt_;
  vdouble lepton_iso_sumNeutralHadronEt_;
  vdouble lepton_iso_sumPhotonEt_;
  vdouble lepton_iso_sumPUPt_;
  vdouble lepton_mvaTrigValue_;
  vdouble lepton_scSigmaIEtaIEta_;
  vdouble lepton_full5x5_scSigmaIEtaIEta_;
  vdouble lepton_hadronicOverEm_;
  vdouble lepton_relEcalIso_;
  vdouble lepton_relHcalIso_;
  vdouble lepton_relTrackIso_;
  vdouble lepton_OneOESuperMinusOneOP_;
  vint lepton_numMissingHits_;
  vint lepton_isEB_;
  vint lepton_passHLTId_;
  vint lepton_passConversionVeto_;
  vint lepton_inCrack_;
  vdouble lepton_scEta_;
  vdouble lepton_dEtaSCTrackAtVtx_;
  vdouble lepton_dPhiSCTrackAtVtx_;


  Float_t wgt_lumi_;
  Float_t wgt_xs_;
  Float_t wgt_nGen_;
  Float_t wgt_lepSF_;
  Float_t wgt_pu_;
  Float_t wgt_puUp_;
  Float_t wgt_puDown_;
  Float_t wgt_topPt_;
  Float_t wgt_topPtUp_;
  Float_t wgt_topPtDown_;
  double  wgt_generator_;
  double  wgt_[rNumSys];
  double  wgt_csvSF_[rNumSys];  

  double  wgt_csvSF_LF_[rNumSys];  
  double  wgt_csvSF_HF_[rNumSys];  
  double  wgt_csvSF_CF_[rNumSys];  

  int ttbb_algo_result_[rNumSys];  
  int ttcc_algo_result_[rNumSys];  

  Float_t numJets_float_[rNumSys];
  Float_t numTags_float_[rNumSys];

  int numJets_[rNumSys];
  int numTags_[rNumSys];


  Float_t first_jet_pt_[rNumSys];
  Float_t min_dr_tagged_jets_[rNumSys];
  Float_t avg_dr_tagged_jets_[rNumSys];
  Float_t aplanarity_[rNumSys];
  Float_t sphericity_[rNumSys];
  Float_t avg_btag_disc_non_btags_[rNumSys];
  Float_t MET_[rNumSys];
  Float_t uMET_[rNumSys];
  Float_t MET_phi_[rNumSys];
  Float_t uMET_phi_[rNumSys];
  Float_t second_jet_pt_[rNumSys];
  Float_t dr_between_lep_and_closest_jet_[rNumSys];
  Float_t h0_[rNumSys];
  Float_t avg_btag_disc_btags_[rNumSys];
  Float_t dev_from_avg_disc_btags_[rNumSys];
  Float_t third_jet_pt_[rNumSys];
  Float_t fourth_jet_pt_[rNumSys];
  Float_t avg_tagged_dijet_mass_[rNumSys];
  Float_t h1_[rNumSys];
  Float_t h2_[rNumSys];
  Float_t lowest_btag_[rNumSys];
  Float_t all_sum_pt_[rNumSys];
  Float_t all_sum_pt_with_met_[rNumSys];
  Float_t avg_untagged_dijet_mass_[rNumSys];
  Float_t closest_tagged_dijet_mass_[rNumSys];
  Float_t h3_[rNumSys];
  Float_t h4_[rNumSys];
  Float_t first_highest_btag_[rNumSys];
  Float_t second_highest_btag_[rNumSys];
  Float_t third_highest_btag_[rNumSys];
  Float_t fourth_highest_btag_[rNumSys];
  Float_t invariant_mass_of_everything_[rNumSys];

  double HT_[rNumSys];
  Float_t MHT_[rNumSys];
  Float_t MHT_phi_[rNumSys];

  double M3_[rNumSys];
  double M3_1tag_[rNumSys];
  double Mlb_[rNumSys];

  double best_higgs_mass_[rNumSys];
  double minChi2_[rNumSys];
  double dRbb_[rNumSys];

  double minChi2_bjet1_px_[rNumSys];
  double minChi2_bjet1_py_[rNumSys];
  double minChi2_bjet1_pz_[rNumSys];
  double minChi2_bjet1_E_[rNumSys];

  double minChi2_bjet2_px_[rNumSys];
  double minChi2_bjet2_py_[rNumSys];
  double minChi2_bjet2_pz_[rNumSys];
  double minChi2_bjet2_E_[rNumSys];

  double tagged_dijet_mass_closest_to_125_[rNumSys];
  
  vvdouble jet_vect_TLV_[rNumSys];
  vdouble jet_CSV_[rNumSys];
  vdouble jet_combinedMVABJetTags_[rNumSys];
  vdouble jet_combinedInclusiveSecondaryVertexV2BJetTags_[rNumSys];
  vdouble jet_vtxMass_[rNumSys];
  vdouble jet_vtxNtracks_[rNumSys];
  vdouble jet_vtx3DVal_[rNumSys];
  vdouble jet_vtx3DSig_[rNumSys];
  vint jet_genId_[rNumSys];
  vint jet_flavour_[rNumSys];
  vint jet_genParentId_[rNumSys];
  vint jet_genGrandParentId_[rNumSys];

  int IsTauTauLeptonEvent_[rNumSys];


  // loose jets
  Float_t numJets_loose_float_[rNumSys];
  Float_t numTags_loose_float_[rNumSys];

  vvdouble jet_loose_vect_TLV_[rNumSys];
  vdouble jet_loose_CSV_[rNumSys];
  vdouble jet_loose_combinedMVABJetTags_[rNumSys];
  vdouble jet_loose_combinedInclusiveSecondaryVertexV2BJetTags_[rNumSys];
  vdouble jet_loose_vtxMass_[rNumSys];
  vdouble jet_loose_vtxNtracks_[rNumSys];
  vdouble jet_loose_vtx3DVal_[rNumSys];
  vdouble jet_loose_vtx3DSig_[rNumSys];  
  vdouble jet_loose_pileupJetId_fullDiscriminant_[rNumSys];
  vint jet_loose_flavour_[rNumSys];

  // temp loose jets
  vvdouble jet_temp_loose_vect_TLV_[rNumSys];
  vdouble jet_temp_loose_CSV_[rNumSys];
  vint jet_temp_loose_flavour_[rNumSys];

  // no cc loose jets
  vvdouble jet_nocc_loose_vect_TLV_[rNumSys];
  vdouble jet_nocc_loose_CSV_[rNumSys];
  vdouble jet_nocc_loose_combinedMVABJetTags_[rNumSys];
  vdouble jet_nocc_loose_combinedInclusiveSecondaryVertexV2BJetTags_[rNumSys];
  vint jet_nocc_loose_flavour_[rNumSys];


  void initialize();

};


typedef std::vector<yggdrasilEventVars> vyggdrasilEventVars;


void yggdrasilEventVars::initialize(){

  passTrigger_         = 0;
  passElectronTrigger_ = 0;
  passMuonTrigger_     = 0;
  leptonType_          = 0;
  higgsDecayType_      = -1;


  OneMuon_ = -99;
  OneElectron_ = -99;
  TwoMuon_ = -99;
  TwoElectron_ = -99;
  MuonElectron_ = -99;
  mass_leplep_ = -99;
  dR_leplep_ = -99;
  oppositeLepCharge_ = -99;

  PassZmask_ = -99;

  matchSingleMuTrigger_       = 0;
  matchSingleElectronTrigger_ = 0;

  run_  = -99;
  lumi_ = -99;
  evt_ = -99;

  top_pt_     = -99.9;
  antitop_pt_ = -99.9;

  Q2ScaleUpWgt_   = -99.9;
  Q2ScaleDownWgt_ = -99.9;

  numTruePV_ = -99;
  numGenPV_ = -99;

  numPVs_ = -99;
  numSys_ = -99;

  tight_lepton_pt_  = -99.9;
  tight_lepton_eta_ = -99.9;
  tight_lepton_phi_ = -99.9;
  tight_lepton_relIso_ = -99.9;
  tight_lepton_isMuon_ = -99;

  tight_lepton_genId_ = -99;
  tight_lepton_genParentId_ = -99;
  tight_lepton_genGrandParentId_ = -99;

  lepton_vect_TLV_.clear();
  lepton_trkCharge_.clear();
  lepton_isMuon_.clear();
  lepton_isTight_.clear();
  lepton_isLoose_.clear();
  lepton_genId_.clear();
  lepton_genParentId_.clear();
  lepton_genGrandParentId_.clear();
  lepton_pt_.clear();
  lepton_eta_.clear();
  lepton_phi_.clear();
  lepton_relIso_.clear();
  lepton_iso_sumChargedHadronPt_.clear();
  lepton_iso_sumNeutralHadronEt_.clear();
  lepton_iso_sumPhotonEt_.clear();
  lepton_iso_sumPUPt_.clear();
  lepton_mvaTrigValue_.clear();
  lepton_scSigmaIEtaIEta_.clear();
  lepton_full5x5_scSigmaIEtaIEta_.clear();
  lepton_hadronicOverEm_.clear();
  lepton_relEcalIso_.clear();
  lepton_relHcalIso_.clear();
  lepton_relTrackIso_.clear();
  lepton_OneOESuperMinusOneOP_.clear();
  lepton_numMissingHits_.clear();
  lepton_isEB_.clear();
  lepton_passHLTId_.clear();
  lepton_passConversionVeto_.clear();
  lepton_inCrack_.clear();
  lepton_scEta_.clear();
  lepton_dEtaSCTrackAtVtx_.clear();
  lepton_dPhiSCTrackAtVtx_.clear();

  wgt_generator_        = -99.9;
  wgt_lumi_             = -99.9;
  wgt_xs_               = -99.9;
  wgt_nGen_             = -99.9;
  wgt_lepSF_            = -99.9;
  wgt_pu_               = -99.9;
  wgt_puUp_             = -99.9;
  wgt_puDown_           = -99.9;
  wgt_topPt_            = -99.9;
  wgt_topPtUp_          = -99.9;
  wgt_topPtDown_        = -99.9;
 
  for(int iSys=0; iSys<rNumSys; iSys++){

    wgt_[iSys] = -99.9;  
    wgt_csvSF_[iSys] = -99.9;  
    wgt_csvSF_LF_[iSys] = -99.9;  
    wgt_csvSF_HF_[iSys] = -99.9;   

    ttbb_algo_result_[iSys] = -99;  
    ttcc_algo_result_[iSys] = -99;  

    numJets_float_[iSys] = -99.9;
    numTags_float_[iSys] = -99.9;

    numJets_[iSys] = -99;
    numTags_[iSys] = -99;

    numJets_loose_float_[iSys] = -99.9;
    numTags_loose_float_[iSys] = -99.9;


    first_jet_pt_[iSys]                   = -99.9;
    min_dr_tagged_jets_[iSys]             = -99.9;
    avg_dr_tagged_jets_[iSys]             = -99.9;
    aplanarity_[iSys]                     = -99.9;
    sphericity_[iSys]                     = -99.9;
    avg_btag_disc_non_btags_[iSys]        = -99.9;
    MET_[iSys]                            = -99.9;
    uMET_[iSys]                           = -99.9;
    MET_phi_[iSys]                        = -99.9;
    uMET_phi_[iSys]                       = -99.9;
    second_jet_pt_[iSys]                  = -99.9;
    dr_between_lep_and_closest_jet_[iSys] = -99.9;
    h0_[iSys]                             = -99.9;
    avg_btag_disc_btags_[iSys]            = -99.9;
    dev_from_avg_disc_btags_[iSys]        = -99.9;
    third_jet_pt_[iSys]                   = -99.9;
    fourth_jet_pt_[iSys]                  = -99.9;
    avg_tagged_dijet_mass_[iSys]          = -99.9;
    h1_[iSys]                             = -99.9;
    h2_[iSys]                             = -99.9;
    lowest_btag_[iSys]                    = -99.9;
    all_sum_pt_[iSys]                     = -99.9;
    all_sum_pt_with_met_[iSys]            = -99.9;
    avg_untagged_dijet_mass_[iSys]        = -99.9;
    closest_tagged_dijet_mass_[iSys]      = -99.9;
    h3_[iSys]                             = -99.9;
    h4_[iSys]                             = -99.9;
    first_highest_btag_[iSys]             = -99.9;
    second_highest_btag_[iSys]            = -99.9;
    third_highest_btag_[iSys]             = -99.9;
    fourth_highest_btag_[iSys]            = -99.9;
    invariant_mass_of_everything_[iSys]   = -99.9;

    HT_[iSys]       = -99.9;
    MHT_[iSys]     = -99.9;
    MHT_phi_[iSys] = -99.9;

    M3_[iSys]      = -99.9;
    M3_1tag_[iSys] = -99.9;
    Mlb_[iSys]     = -99.9;

    best_higgs_mass_[iSys]    = -99.9;
    minChi2_[iSys]            = 99.9e9;
    dRbb_[iSys]               = -99.9;

    minChi2_bjet1_px_[iSys] = 99.9e9;
    minChi2_bjet1_py_[iSys] = 99.9e9;
    minChi2_bjet1_pz_[iSys] = 99.9e9;
    minChi2_bjet1_E_[iSys]  = 99.9e9;

    minChi2_bjet2_px_[iSys] = 99.9e9;
    minChi2_bjet2_py_[iSys] = 99.9e9;
    minChi2_bjet2_pz_[iSys] = 99.9e9;
    minChi2_bjet2_E_[iSys]  = 99.9e9;

    tagged_dijet_mass_closest_to_125_[iSys] = -99.9;
  
    jet_vect_TLV_[iSys].clear();
    jet_CSV_[iSys].clear();
    jet_combinedMVABJetTags_[iSys].clear();
    jet_combinedInclusiveSecondaryVertexV2BJetTags_[iSys].clear();
    jet_vtxMass_[iSys].clear();
    jet_vtxNtracks_[iSys].clear();
    jet_vtx3DVal_[iSys].clear();
    jet_vtx3DSig_[iSys].clear();
    jet_flavour_[iSys].clear();
    jet_genId_[iSys].clear();
    jet_genParentId_[iSys].clear();
    jet_genGrandParentId_[iSys].clear();

    jet_loose_vect_TLV_[iSys].clear();
    jet_loose_CSV_[iSys].clear();
    jet_loose_combinedMVABJetTags_[iSys].clear();
    jet_loose_combinedInclusiveSecondaryVertexV2BJetTags_[iSys].clear();
    jet_loose_vtxMass_[iSys].clear();
    jet_loose_vtxNtracks_[iSys].clear();
    jet_loose_vtx3DVal_[iSys].clear();
    jet_loose_vtx3DSig_[iSys].clear();
    jet_loose_flavour_[iSys].clear();

    jet_temp_loose_vect_TLV_[iSys].clear();
    jet_temp_loose_CSV_[iSys].clear();
    jet_temp_loose_flavour_[iSys].clear();

    jet_nocc_loose_vect_TLV_[iSys].clear();
    jet_nocc_loose_CSV_[iSys].clear();
    jet_nocc_loose_combinedMVABJetTags_[iSys].clear();
    jet_nocc_loose_combinedInclusiveSecondaryVertexV2BJetTags_[iSys].clear();
    jet_nocc_loose_flavour_[iSys].clear();

    IsTauTauLeptonEvent_[iSys] = -99;
  }


  return;
}

  

#endif
