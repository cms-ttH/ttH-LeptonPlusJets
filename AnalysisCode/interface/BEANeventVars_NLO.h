#ifndef ttH_LeptonPlusJets_AnalysisCode_BEANeventVars_NLO_h
#define ttH_LeptonPlusJets_AnalysisCode_BEANeventVars_NLO_h

//
// Dependencies (#includes)
//
#include <iostream>
#include <vector>
#include "TLorentzVector.h"

#ifdef __MAKECINT__
#pragma link C++ class std::vector< TLorentzVector >+; 
#endif

//*****************************************************************************


using namespace std;


//*****************************************************************************


typedef std::vector<std::vector<double> > vvdouble;
typedef std::vector<std::vector<std::string> > vvstring;
typedef std::vector<double> vdouble;
typedef std::vector<string> vstring;
typedef std::vector<bool> vbool;
typedef std::vector<int> vint;
typedef std::vector< TLorentzVector > vecTLorentzVector;


//*****************************************************************************

//
// Utility Class for Handling Event Variables
//

const Int_t kNumSys_NLO = 1;

struct BEANeventVars_NLO{


  //////////////////////////////////////////////////////////////////////////
  ///  Tree branches/leaves
  //////////////////////////////////////////////////////////////////////////

  explicit BEANeventVars_NLO(): weight_(1.0) { }

  Float_t weight_;

  vint    genJet_isSelected_;
  vdouble genJet_pt_;
  vdouble genJet_eta_;
  vdouble genJet_phi_;
  vdouble genJet_energy_;
  vdouble genJet_pdgId_;
  vdouble genJet_mother_pdgId_;
  
  vdouble genParticle_pt_;
  vdouble genParticle_eta_;
  vdouble genParticle_phi_;
  vdouble genParticle_energy_;
  vdouble genParticle_pdgId_;
  vdouble genParticle_mother_pdgId_;

  Float_t dR_final_intial_gen_top_;
  Float_t dR_final_intial_gen_topBar_;
  Float_t dR_final_intial_gen_b_fromTop_;
  Float_t dR_final_intial_gen_b_fromTopBar_;
  
  Float_t dPtRel_final_intial_gen_top_;
  Float_t dPtRel_final_intial_gen_topBar_;
  Float_t dPtRel_final_intial_gen_b_fromTop_;
  Float_t dPtRel_final_intial_gen_b_fromTopBar_;

  vdouble matched_nonSelected_ttbar_genJet_pt_;
  vdouble matched_nonSelected_ttbar_genJet_eta_;

  Int_t nExtraBQuarks_;
  Int_t nExtraCQuarks_;
  vdouble genParticle_extraBQuarks_pt_;
  vdouble genParticle_extraCQuarks_pt_;
  vdouble genParticle_extraBQuarks_mother_pdgId_;
  vdouble genParticle_extraCQuarks_mother_pdgId_;
 
  Float_t maxPt_gen_extraBQuark_;
  Float_t minPt_gen_extraBQuark_;
  
  Float_t maxPt_gen_extraCQuark_;
  Float_t minPt_gen_extraCQuark_;
  
  Int_t nExtraBJets_;
  Int_t nExtraCJets_;

  

  void initialize(){

    weight_     = 1.0;
  
    genJet_isSelected_.clear();
    genJet_pt_.clear();
    genJet_eta_.clear();
    genJet_phi_.clear();
    genJet_energy_.clear();
    genJet_pdgId_.clear();
    genJet_mother_pdgId_.clear();

    genParticle_pt_.clear();
    genParticle_eta_.clear();
    genParticle_phi_.clear();
    genParticle_energy_.clear();
    genParticle_pdgId_.clear();
    genParticle_mother_pdgId_.clear();

    dR_final_intial_gen_top_          = 0.0;
    dR_final_intial_gen_topBar_       = 0.0;
    dR_final_intial_gen_b_fromTop_    = 0.0;
    dR_final_intial_gen_b_fromTopBar_ = 0.0;
      
    dPtRel_final_intial_gen_top_          = 0.0;
    dPtRel_final_intial_gen_topBar_       = 0.0;
    dPtRel_final_intial_gen_b_fromTop_    = 0.0;
    dPtRel_final_intial_gen_b_fromTopBar_ = 0.0;
      
    matched_nonSelected_ttbar_genJet_pt_.clear();
    matched_nonSelected_ttbar_genJet_eta_.clear();
  
    nExtraBQuarks_ = 0;
    nExtraCQuarks_ = 0;
    genParticle_extraBQuarks_pt_.clear();
    genParticle_extraCQuarks_pt_.clear();
    genParticle_extraBQuarks_mother_pdgId_.clear();
    genParticle_extraCQuarks_mother_pdgId_.clear();
 
    maxPt_gen_extraBQuark_ = 0.0;
    minPt_gen_extraBQuark_ = 0.0;
      
    maxPt_gen_extraCQuark_ = 0.0;
    minPt_gen_extraCQuark_ = 0.0;
      
    nExtraBJets_ = 0;
    nExtraCJets_ = 0;


    return;
  }



};


typedef std::vector<BEANeventVars_NLO> vBEANeventVars_NLO;

  
#endif
