#ifndef ttH_LeptonPlusJets_AnalysisCode_BEANeventVars_h
#define ttH_LeptonPlusJets_AnalysisCode_BEANeventVars_h

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

const Int_t kNumSys = 33;

struct eventVars{


  //////////////////////////////////////////////////////////////////////////
  ///  Tree branches/leaves
  //////////////////////////////////////////////////////////////////////////

  explicit eventVars(): passElectronTrigger_(0),passMuonTrigger_(0) { }

  Int_t   passTrigger_;
  Int_t   passElectronTrigger_;
  Int_t   passMuonTrigger_;
  Int_t   leptonType_;
  Int_t   higgsDecayType_;

  Int_t   matchSingleMuTrigger_;
  Int_t   matchSingleElectronTrigger_;

  int run_;
  int lumi_;
  long evt_;

  double top_pt_;
  double antitop_pt_;

  double Q2ScaleUpWgt_;
  double Q2ScaleDownWgt_;

  Int_t   numPVs_;
  Int_t   numSys_;

  Float_t tight_lepton_pt_;
  Float_t tight_lepton_eta_;
  Float_t tight_lepton_phi_;
  Float_t tight_lepton_relIso_;

  Float_t wgt_lumi_;
  Float_t wgt_xs_;
  Float_t wgt_nGen_;
  Float_t wgt_lepSF_;
  Float_t wgt_pu_;
  Float_t wgt_puUp_;
  Float_t wgt_puDown_;
  Float_t wgt_pu_2012AB_;
  Float_t wgt_puUp_2012AB_;
  Float_t wgt_puDown_2012AB_;
  Float_t wgt_pu_2012ABCD_;
  Float_t wgt_puUp_2012ABCD_;
  Float_t wgt_puDown_2012ABCD_;
  Float_t wgt_topPt_;
  Float_t wgt_topPtUp_;
  Float_t wgt_topPtDown_;
  double  wgt_[kNumSys];  
  double  wgt_btagSF_[kNumSys];  
  double  wgt_csvSF_[kNumSys];  

  int ttbb_algo_result_[kNumSys];  
  int ttcc_algo_result_[kNumSys];  

  Float_t numJets_float_[kNumSys];
  Float_t numTags_float_[kNumSys];

  Float_t numJets_PUid_full_loose_float_[kNumSys];
  Float_t numJets_PUid_full_medium_float_[kNumSys];
  Float_t numJets_PUid_full_tight_float_[kNumSys];

  Float_t numJets_PUid_simple_loose_float_[kNumSys];
  Float_t numJets_PUid_simple_medium_float_[kNumSys];
  Float_t numJets_PUid_simple_tight_float_[kNumSys];

  Float_t numTags_PUid_full_loose_float_[kNumSys];
  Float_t numTags_PUid_full_medium_float_[kNumSys];
  Float_t numTags_PUid_full_tight_float_[kNumSys];

  Float_t numTags_PUid_simple_loose_float_[kNumSys];
  Float_t numTags_PUid_simple_medium_float_[kNumSys];
  Float_t numTags_PUid_simple_tight_float_[kNumSys];
  

  int numJets_[kNumSys];
  int numTags_[kNumSys];

  int numJets_PUid_full_loose_[kNumSys];
  int numJets_PUid_full_medium_[kNumSys];
  int numJets_PUid_full_tight_[kNumSys];

  int numJets_PUid_simple_loose_[kNumSys];
  int numJets_PUid_simple_medium_[kNumSys];
  int numJets_PUid_simple_tight_[kNumSys];

  int numTags_PUid_full_loose_[kNumSys];
  int numTags_PUid_full_medium_[kNumSys];
  int numTags_PUid_full_tight_[kNumSys];

  int numTags_PUid_simple_loose_[kNumSys];
  int numTags_PUid_simple_medium_[kNumSys];
  int numTags_PUid_simple_tight_[kNumSys];


  Float_t first_jet_pt_[kNumSys];
  Float_t min_dr_tagged_jets_[kNumSys];
  Float_t avg_dr_tagged_jets_[kNumSys];
  Float_t aplanarity_[kNumSys];
  Float_t sphericity_[kNumSys];
  Float_t avg_btag_disc_non_btags_[kNumSys];
  Float_t MET_[kNumSys];
  Float_t uMET_[kNumSys];
  Float_t MET_phi_[kNumSys];
  Float_t uMET_phi_[kNumSys];
  Float_t second_jet_pt_[kNumSys];
  Float_t dr_between_lep_and_closest_jet_[kNumSys];
  Float_t h0_[kNumSys];
  Float_t avg_btag_disc_btags_[kNumSys];
  Float_t dev_from_avg_disc_btags_[kNumSys];
  Float_t third_jet_pt_[kNumSys];
  Float_t fourth_jet_pt_[kNumSys];
  Float_t avg_tagged_dijet_mass_[kNumSys];
  Float_t h1_[kNumSys];
  Float_t h2_[kNumSys];
  Float_t lowest_btag_[kNumSys];
  Float_t all_sum_pt_[kNumSys];
  Float_t all_sum_pt_with_met_[kNumSys];
  Float_t avg_untagged_dijet_mass_[kNumSys];
  Float_t closest_tagged_dijet_mass_[kNumSys];
  Float_t h3_[kNumSys];
  Float_t h4_[kNumSys];
  Float_t first_highest_btag_[kNumSys];
  Float_t second_highest_btag_[kNumSys];
  Float_t third_highest_btag_[kNumSys];
  Float_t fourth_highest_btag_[kNumSys];
  Float_t invariant_mass_of_everything_[kNumSys];

  double HT_[kNumSys];
  Float_t MHT_[kNumSys];
  Float_t MHT_phi_[kNumSys];

  Float_t jet_tag_1_pt_[kNumSys];
  Float_t jet_tag_2_pt_[kNumSys];
  Float_t jet_tag_3_pt_[kNumSys];
  Float_t jet_tag_4_pt_[kNumSys];

  Float_t jet_untag_1_pt_[kNumSys];
  Float_t jet_untag_2_pt_[kNumSys];
  Float_t jet_untag_3_pt_[kNumSys];
  Float_t jet_untag_4_pt_[kNumSys];

  double M3_[kNumSys];
  double M3_1tag_[kNumSys];
  double Mlb_[kNumSys];

  double best_higgs_mass_[kNumSys];
  double best_higgs_mass_old_[kNumSys];
  double minChi2_[kNumSys];
  //double minChi2_v2_[kNumSys];
  double dRbb_[kNumSys];
  //double dRbb_v2_[kNumSys];

  double jet_1_chargedHadronEnergyFraction_[kNumSys];
  double jet_1_neutralHadronEnergyFraction_[kNumSys];
  double jet_1_chargedEmEnergyFraction_[kNumSys];
  double jet_1_neutralEmEnergyFraction_[kNumSys];
  double jet_1_chargedMultiplicity_[kNumSys];
  double jet_1_neutralMultiplicity_[kNumSys];
  double jet_1_nconstituents_[kNumSys];

  double jet_2_chargedHadronEnergyFraction_[kNumSys];
  double jet_2_neutralHadronEnergyFraction_[kNumSys];
  double jet_2_chargedEmEnergyFraction_[kNumSys];
  double jet_2_neutralEmEnergyFraction_[kNumSys];
  double jet_2_chargedMultiplicity_[kNumSys];
  double jet_2_neutralMultiplicity_[kNumSys];
  double jet_2_nconstituents_[kNumSys];

  double jet_3_chargedHadronEnergyFraction_[kNumSys];
  double jet_3_neutralHadronEnergyFraction_[kNumSys];
  double jet_3_chargedEmEnergyFraction_[kNumSys];
  double jet_3_neutralEmEnergyFraction_[kNumSys];
  double jet_3_chargedMultiplicity_[kNumSys];
  double jet_3_neutralMultiplicity_[kNumSys];
  double jet_3_nconstituents_[kNumSys];

  double jet_4_chargedHadronEnergyFraction_[kNumSys];
  double jet_4_neutralHadronEnergyFraction_[kNumSys];
  double jet_4_chargedEmEnergyFraction_[kNumSys];
  double jet_4_neutralEmEnergyFraction_[kNumSys];
  double jet_4_chargedMultiplicity_[kNumSys];
  double jet_4_neutralMultiplicity_[kNumSys];
  double jet_4_nconstituents_[kNumSys];

  double minChi2_bjet1_px_[kNumSys];
  double minChi2_bjet1_py_[kNumSys];
  double minChi2_bjet1_pz_[kNumSys];
  double minChi2_bjet1_E_[kNumSys];

  double minChi2_bjet2_px_[kNumSys];
  double minChi2_bjet2_py_[kNumSys];
  double minChi2_bjet2_pz_[kNumSys];
  double minChi2_bjet2_E_[kNumSys];

  double tagged_dijet_mass_closest_to_125_[kNumSys];
  
  
  vdouble lepton_TLV_[kNumSys];
  int lepton_genId_[kNumSys];
  int lepton_genParentId_[kNumSys];
  int lepton_genGrandParentId_[kNumSys];
  // vdouble MET_from_bhm_TLV_[kNumSys]; // leave out for now
  vvdouble jet_vect_TLV_[kNumSys];
  vdouble jet_CSV_[kNumSys];
  vint jet_genId_[kNumSys];
  vint jet_flavour_[kNumSys];
  vint jet_genParentId_[kNumSys];
  vint jet_genGrandParentId_[kNumSys];
  // vstring jet_assignment_from_bhm_[kNumSys]; // leave out for now


  vint    jet_PUid_full_loose_[kNumSys];
  vint    jet_PUid_full_medium_[kNumSys];
  vint    jet_PUid_full_tight_[kNumSys];
 
  vint    jet_PUid_simple_loose_[kNumSys];
  vint    jet_PUid_simple_medium_[kNumSys];
  vint    jet_PUid_simple_tight_[kNumSys];

  vdouble jet_PUmva_full_[kNumSys];
  vdouble jet_PUmva_simple_[kNumSys];

  vdouble jet_dR2Mean_[kNumSys];
  vdouble jet_dRMean_[kNumSys];
  vdouble jet_frac01_[kNumSys];
  vdouble jet_frac02_[kNumSys];
  vdouble jet_frac03_[kNumSys];
  vdouble jet_beta_[kNumSys];
  vdouble jet_betaStar_[kNumSys];
  vdouble jet_leadCandDistFromPV_[kNumSys];
  vdouble jet_minPVDist_[kNumSys];

  int IsTauTauLeptonEvent_[kNumSys];


  // loose jets
  Float_t numJets_loose_float_[kNumSys];
  Float_t numTags_loose_float_[kNumSys];

  vvdouble jet_loose_vect_TLV_[kNumSys];
  vdouble jet_loose_CSV_[kNumSys];
  vint jet_loose_flavour_[kNumSys];


  void initialize();

};


typedef std::vector<eventVars> veventVars;


void eventVars::initialize(){

  passTrigger_         = 0;
  passElectronTrigger_ = 0;
  passMuonTrigger_     = 0;
  leptonType_          = 0;
  higgsDecayType_      = -1;

  matchSingleMuTrigger_       = 0;
  matchSingleElectronTrigger_ = 0;

  run_  = -99;
  lumi_ = -99;
  evt_ = -99;

  top_pt_     = -99.9;
  antitop_pt_ = -99.9;

  Q2ScaleUpWgt_   = -99.9;
  Q2ScaleDownWgt_ = -99.9;

  numPVs_ = -99;
  numSys_ = -99;

  tight_lepton_pt_  = -99.9;
  tight_lepton_eta_ = -99.9;
  tight_lepton_phi_ = -99.9;
  tight_lepton_relIso_ = -99.9;

  wgt_lumi_             = -99.9;
  wgt_xs_               = -99.9;
  wgt_nGen_             = -99.9;
  wgt_lepSF_            = -99.9;
  wgt_pu_               = -99.9;
  wgt_puUp_             = -99.9;
  wgt_puDown_           = -99.9;
  wgt_pu_2012AB_        = -99.9;
  wgt_puUp_2012AB_      = -99.9;
  wgt_puDown_2012AB_    = -99.9;
  wgt_pu_2012ABCD_      = -99.9;
  wgt_puUp_2012ABCD_    = -99.9;
  wgt_puDown_2012ABCD_  = -99.9;;
  wgt_topPt_            = -99.9;
  wgt_topPtUp_          = -99.9;
  wgt_topPtDown_        = -99.9;
 
  for(int iSys=0; iSys<kNumSys; iSys++){

    wgt_[iSys] = -99.9;  
    wgt_btagSF_[iSys] = -99.9;  
    wgt_csvSF_[iSys] = -99.9;  


    ttbb_algo_result_[iSys] = -99;  
    ttcc_algo_result_[iSys] = -99;  

    
    numJets_float_[iSys] = -99.9;
    numTags_float_[iSys] = -99.9;

    numJets_PUid_full_loose_float_[iSys]  = -99.9;
    numJets_PUid_full_medium_float_[iSys] = -99.9;
    numJets_PUid_full_tight_float_[iSys]  = -99.9;

    numJets_PUid_simple_loose_float_[iSys]  = -99.9;
    numJets_PUid_simple_medium_float_[iSys] = -99.9;
    numJets_PUid_simple_tight_float_[iSys]  = -99.9;

    numTags_PUid_full_loose_float_[iSys]  = -99.9;
    numTags_PUid_full_medium_float_[iSys] = -99.9;
    numTags_PUid_full_tight_float_[iSys]  = -99.9;

    numTags_PUid_simple_loose_float_[iSys]  = -99.9;
    numTags_PUid_simple_medium_float_[iSys] = -99.9;
    numTags_PUid_simple_tight_float_[iSys]  = -99.9;
    

    numJets_[iSys] = -99;
    numTags_[iSys] = -99;

    numJets_PUid_full_loose_[iSys]  = -99;
    numJets_PUid_full_medium_[iSys] = -99;
    numJets_PUid_full_tight_[iSys]  = -99;

    numJets_PUid_simple_loose_[iSys]  = -99;
    numJets_PUid_simple_medium_[iSys] = -99;
    numJets_PUid_simple_tight_[iSys]  = -99;

    numTags_PUid_full_loose_[iSys]  = -99;
    numTags_PUid_full_medium_[iSys] = -99;
    numTags_PUid_full_tight_[iSys]  = -99;

    numTags_PUid_simple_loose_[iSys]  = -99;
    numTags_PUid_simple_medium_[iSys] = -99;
    numTags_PUid_simple_tight_[iSys]  = -99;


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

    jet_tag_1_pt_[iSys] = -99.9;
    jet_tag_2_pt_[iSys] = -99.9;
    jet_tag_3_pt_[iSys] = -99.9;
    jet_tag_4_pt_[iSys] = -99.9;

    jet_untag_1_pt_[iSys] = -99.9;
    jet_untag_2_pt_[iSys] = -99.9;
    jet_untag_3_pt_[iSys] = -99.9;
    jet_untag_4_pt_[iSys] = -99.9;

    M3_[iSys]      = -99.9;
    M3_1tag_[iSys] = -99.9;
    Mlb_[iSys]     = -99.9;

    best_higgs_mass_[iSys]    = -99.9;
    best_higgs_mass_old_[iSys] = -99.9;
    minChi2_[iSys]            = 99.9e9;
    //minChi2_v2_[iSys]         = 99.9e9;
    dRbb_[iSys]               = -99.9;
    //dRbb_v2_[iSys]            = -99.9;

    jet_1_chargedHadronEnergyFraction_[iSys] = -99.9;
    jet_1_neutralHadronEnergyFraction_[iSys] = -99.9;
    jet_1_chargedEmEnergyFraction_[iSys]     = -99.9;
    jet_1_neutralEmEnergyFraction_[iSys]     = -99.9;
    jet_1_chargedMultiplicity_[iSys]         = -99.9;
    jet_1_neutralMultiplicity_[iSys]         = -99.9;
    jet_1_nconstituents_[iSys]               = -99.9;

    jet_2_chargedHadronEnergyFraction_[iSys] = -99.9;
    jet_2_neutralHadronEnergyFraction_[iSys] = -99.9;
    jet_2_chargedEmEnergyFraction_[iSys]     = -99.9;
    jet_2_neutralEmEnergyFraction_[iSys]     = -99.9;
    jet_2_chargedMultiplicity_[iSys]         = -99.9;
    jet_2_neutralMultiplicity_[iSys]         = -99.9;
    jet_2_nconstituents_[iSys]               = -99.9;

    jet_3_chargedHadronEnergyFraction_[iSys] = -99.9;
    jet_3_neutralHadronEnergyFraction_[iSys] = -99.9;
    jet_3_chargedEmEnergyFraction_[iSys]     = -99.9;
    jet_3_neutralEmEnergyFraction_[iSys]     = -99.9;
    jet_3_chargedMultiplicity_[iSys]         = -99.9;
    jet_3_neutralMultiplicity_[iSys]         = -99.9;
    jet_3_nconstituents_[iSys]               = -99.9;

    jet_4_chargedHadronEnergyFraction_[iSys] = -99.9;
    jet_4_neutralHadronEnergyFraction_[iSys] = -99.9;
    jet_4_chargedEmEnergyFraction_[iSys]     = -99.9;
    jet_4_neutralEmEnergyFraction_[iSys]     = -99.9;
    jet_4_chargedMultiplicity_[iSys]         = -99.9;
    jet_4_neutralMultiplicity_[iSys]         = -99.9;
    jet_4_nconstituents_[iSys]               = -99.9;

    minChi2_bjet1_px_[iSys] = 99.9e9;
    minChi2_bjet1_py_[iSys] = 99.9e9;
    minChi2_bjet1_pz_[iSys] = 99.9e9;
    minChi2_bjet1_E_[iSys]  = 99.9e9;

    minChi2_bjet2_px_[iSys] = 99.9e9;
    minChi2_bjet2_py_[iSys] = 99.9e9;
    minChi2_bjet2_pz_[iSys] = 99.9e9;
    minChi2_bjet2_E_[iSys]  = 99.9e9;

    tagged_dijet_mass_closest_to_125_[iSys] = -99.9;
  
  
    lepton_TLV_[iSys].clear();
    lepton_genId_[iSys]             = -99;
    lepton_genParentId_[iSys]       = -99;
    lepton_genGrandParentId_[iSys]  = -99;
    // MET_from_bhm_TLV_[iSys].clear(); // leave out for now
    jet_vect_TLV_[iSys].clear();
    jet_CSV_[iSys].clear();
    jet_flavour_[iSys].clear();
    jet_genId_[iSys].clear();
    jet_genParentId_[iSys].clear();
    jet_genGrandParentId_[iSys].clear();
    // jet_assignment_from_bhm_[iSys].clear(); // leave out for now


    jet_PUid_full_loose_[iSys].clear();
    jet_PUid_full_medium_[iSys].clear();
    jet_PUid_full_tight_[iSys].clear();
 
    jet_PUid_simple_loose_[iSys].clear();
    jet_PUid_simple_medium_[iSys].clear();
    jet_PUid_simple_tight_[iSys].clear();

    jet_PUmva_full_[iSys].clear();
    jet_PUmva_simple_[iSys].clear();

    jet_dR2Mean_[iSys].clear();
    jet_dRMean_[iSys].clear();
    jet_frac01_[iSys].clear();
    jet_frac02_[iSys].clear();
    jet_frac03_[iSys].clear();
    jet_beta_[iSys].clear();
    jet_betaStar_[iSys].clear();
    jet_leadCandDistFromPV_[iSys].clear();
    jet_minPVDist_[iSys].clear();

    IsTauTauLeptonEvent_[iSys] = -99;
  }


  return;
}

  

#endif
