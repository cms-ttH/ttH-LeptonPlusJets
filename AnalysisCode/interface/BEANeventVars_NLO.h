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


typedef std::vector< std::vector< std::vector<double> > > vvvdouble;
typedef std::vector< std::vector<double> > vvdouble;
typedef std::vector< std::vector<std::string> > vvstring;
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

  // Event Variables
  Float_t weight_;


  // Gen Particle TTbar System Variables
  vvdouble genParticle_top_quark_TLV_;
  vdouble genParticle_top_quark_status_;
  vdouble genParticle_top_quark_pdgId_;
  vdouble genParticle_top_quark_mother_pdgId_;
  
  vvdouble genParticle_b_quark_fromTop_TLV_;
  vdouble genParticle_b_quark_fromTop_status_;
  vdouble genParticle_b_quark_fromTop_pdgId_;
  vdouble genParticle_b_quark_fromTop_mother_pdgId_;

  vvdouble genParticle_w_boson_fromTop_TLV_;
  vdouble genParticle_w_boson_fromTop_status_;
  vdouble genParticle_w_boson_fromTop_pdgId_;
  vdouble genParticle_w_boson_fromTop_mother_pdgId_;

  vvdouble genParticle_wDaughter1_fromTop_TLV_;
  vdouble genParticle_wDaughter1_fromTop_status_;
  vdouble genParticle_wDaughter1_fromTop_pdgId_;
  vdouble genParticle_wDaughter1_fromTop_mother_pdgId_;

  vvdouble genParticle_wDaughter2_fromTop_TLV_;
  vdouble genParticle_wDaughter2_fromTop_status_;
  vdouble genParticle_wDaughter2_fromTop_pdgId_;
  vdouble genParticle_wDaughter2_fromTop_mother_pdgId_;

  vvdouble genParticle_topBar_quark_TLV_;
  vdouble genParticle_topBar_quark_status_;
  vdouble genParticle_topBar_quark_pdgId_;
  vdouble genParticle_topBar_quark_mother_pdgId_;

  vvdouble genParticle_b_quark_fromTopBar_TLV_;
  vdouble genParticle_b_quark_fromTopBar_status_;
  vdouble genParticle_b_quark_fromTopBar_pdgId_;
  vdouble genParticle_b_quark_fromTopBar_mother_pdgId_;

  vvdouble genParticle_w_boson_fromTopBar_TLV_;
  vdouble genParticle_w_boson_fromTopBar_status_;
  vdouble genParticle_w_boson_fromTopBar_pdgId_;
  vdouble genParticle_w_boson_fromTopBar_mother_pdgId_;

  vvdouble genParticle_wDaughter1_fromTopBar_TLV_;
  vdouble genParticle_wDaughter1_fromTopBar_status_;
  vdouble genParticle_wDaughter1_fromTopBar_pdgId_;
  vdouble genParticle_wDaughter1_fromTopBar_mother_pdgId_;

  vvdouble genParticle_wDaughter2_fromTopBar_TLV_;
  vdouble genParticle_wDaughter2_fromTopBar_status_;
  vdouble genParticle_wDaughter2_fromTopBar_pdgId_;
  vdouble genParticle_wDaughter2_fromTopBar_mother_pdgId_;

  int genTopIsLeptonic_;
  int genTopBarIsLeptonic_;
 
  int genTopIsTau_;
  int genTopBarIsTau_;
  
  int isHadronicDecay_;
  int isSemiLeptonicDecay_;
  int isDiLeptonicDecay_;

  
  // GenJets matched to ttbar system variables
  vdouble genJet_matchedTo_b_quark_fromTop_TLV_;
  int genJet_matchedTo_b_quark_fromTop_isSelected_;
  
  vdouble genJet_matchedTo_wDaughter1_fromTop_TLV_;
  int genJet_matchedTo_wDaughter1_fromTop_isSelected_;

  vdouble genJet_matchedTo_wDaughter2_fromTop_TLV_;
  int genJet_matchedTo_wDaughter2_fromTop_isSelected_;

  vdouble genJet_matchedTo_b_quark_fromTopBar_TLV_;
  int genJet_matchedTo_b_quark_fromTopBar_isSelected_;
  
  vdouble genJet_matchedTo_wDaughter1_fromTopBar_TLV_;
  int genJet_matchedTo_wDaughter1_fromTopBar_isSelected_;

  vdouble genJet_matchedTo_wDaughter2_fromTopBar_TLV_;
  int genJet_matchedTo_wDaughter2_fromTopBar_isSelected_;


  // Extra B/C Quark Variables
  int nExtraBQuarks_;
  int nExtraCQuarks_;
  
  vvvdouble genParticle_extraBQuark_TLV_;
  vvdouble genParticle_extraBQuark_status_;
  vvdouble genParticle_extraBQuark_pdgId_;
  vvdouble genParticle_extraBQuark_mother_pdgId_;

  vvvdouble genParticle_extraCQuark_TLV_;
  vvdouble genParticle_extraCQuark_status_;
  vvdouble genParticle_extraCQuark_pdgId_;
  vvdouble genParticle_extraCQuark_mother_pdgId_;
  
  int nExtraBJets_;
  int nExtraCJets_;
  
  vvdouble genJet_matchedTo_extraBQuark_TLV_;
  vint genJet_matchedTo_extraBQuark_isSelected_;
  
  vvdouble genJet_matchedTo_extraCQuark_TLV_;
  vint genJet_matchedTo_extraCQuark_isSelected_;


  // TTbar Classification Variables
  int isTTLF_;
  int isTTCC_;
  int isTT1B_;
  int isTT2B_;

  
  // Tight GenParticle Lepton from Selection
  vvdouble genParticle_tightLepton_TLV_;
  vdouble genParticle_tightLepton_status_;
  vdouble genParticle_tightLepton_pdgId_;
  vdouble genParticle_tightLepton_mother_pdgId_;


  // GenJet Variables
  vvdouble genJet_TLV_;
  vint     genJet_isSelected_;
  
  
  // GenParticle
  vvdouble genParticle_TLV_;
  vdouble  genParticle_status_;
  vdouble  genParticle_pdgId_;
  vdouble  genParticle_mother_pdgId_;

    

  void initialize(){

    // Event Variables
    weight_ = 1.0;


    // Gen Particle TTbar System Variables
    genParticle_top_quark_TLV_.clear();
    genParticle_top_quark_status_.clear();
    genParticle_top_quark_pdgId_.clear();
    genParticle_top_quark_mother_pdgId_.clear();
  
    genParticle_b_quark_fromTop_TLV_.clear();
    genParticle_b_quark_fromTop_status_.clear();
    genParticle_b_quark_fromTop_pdgId_.clear();
    genParticle_b_quark_fromTop_mother_pdgId_.clear();

    genParticle_w_boson_fromTop_TLV_.clear();
    genParticle_w_boson_fromTop_status_.clear();
    genParticle_w_boson_fromTop_pdgId_.clear();
    genParticle_w_boson_fromTop_mother_pdgId_.clear();

    genParticle_wDaughter1_fromTop_TLV_.clear();
    genParticle_wDaughter1_fromTop_status_.clear();
    genParticle_wDaughter1_fromTop_pdgId_.clear();
    genParticle_wDaughter1_fromTop_mother_pdgId_.clear();

    genParticle_wDaughter2_fromTop_TLV_.clear();
    genParticle_wDaughter2_fromTop_status_.clear();
    genParticle_wDaughter2_fromTop_pdgId_.clear();
    genParticle_wDaughter2_fromTop_mother_pdgId_.clear();

    genParticle_topBar_quark_TLV_.clear();
    genParticle_topBar_quark_status_.clear();
    genParticle_topBar_quark_pdgId_.clear();
    genParticle_topBar_quark_mother_pdgId_.clear();

    genParticle_b_quark_fromTopBar_TLV_.clear();
    genParticle_b_quark_fromTopBar_status_.clear();
    genParticle_b_quark_fromTopBar_pdgId_.clear();
    genParticle_b_quark_fromTopBar_mother_pdgId_.clear();

    genParticle_w_boson_fromTopBar_TLV_.clear();
    genParticle_w_boson_fromTopBar_status_.clear();
    genParticle_w_boson_fromTopBar_pdgId_.clear();
    genParticle_w_boson_fromTopBar_mother_pdgId_.clear();

    genParticle_wDaughter1_fromTopBar_TLV_.clear();
    genParticle_wDaughter1_fromTopBar_status_.clear();
    genParticle_wDaughter1_fromTopBar_pdgId_.clear();
    genParticle_wDaughter1_fromTopBar_mother_pdgId_.clear();

    genParticle_wDaughter2_fromTopBar_TLV_.clear();
    genParticle_wDaughter2_fromTopBar_status_.clear();
    genParticle_wDaughter2_fromTopBar_pdgId_.clear();
    genParticle_wDaughter2_fromTopBar_mother_pdgId_.clear();

    genTopIsLeptonic_ = 0;
    genTopBarIsLeptonic_ = 0;
 
    genTopIsTau_ = 0;
    genTopBarIsTau_ = 0;
  
    isHadronicDecay_ = 0;
    isSemiLeptonicDecay_ = 0;
    isDiLeptonicDecay_ = 0;

  
    // GenJets matched to ttbar system variables
    genJet_matchedTo_b_quark_fromTop_TLV_.clear();
    genJet_matchedTo_b_quark_fromTop_isSelected_ = 0;
  
    genJet_matchedTo_wDaughter1_fromTop_TLV_.clear();
    genJet_matchedTo_wDaughter1_fromTop_isSelected_ = 0;

    genJet_matchedTo_wDaughter2_fromTop_TLV_.clear();
    genJet_matchedTo_wDaughter2_fromTop_isSelected_ = 0;

    genJet_matchedTo_b_quark_fromTopBar_TLV_.clear();
    genJet_matchedTo_b_quark_fromTopBar_isSelected_ = 0;
  
    genJet_matchedTo_wDaughter1_fromTopBar_TLV_.clear();
    genJet_matchedTo_wDaughter1_fromTopBar_isSelected_ = 0;

    genJet_matchedTo_wDaughter2_fromTopBar_TLV_.clear();
    genJet_matchedTo_wDaughter2_fromTopBar_isSelected_ = 0;


    // Extra B/C Quark Variables
    nExtraBQuarks_ = 0;
    nExtraCQuarks_ = 0;
  
    genParticle_extraBQuark_TLV_.clear();
    genParticle_extraBQuark_status_.clear();
    genParticle_extraBQuark_pdgId_.clear();
    genParticle_extraBQuark_mother_pdgId_.clear();

    genParticle_extraCQuark_TLV_.clear();
    genParticle_extraCQuark_status_.clear();
    genParticle_extraCQuark_pdgId_.clear();
    genParticle_extraCQuark_mother_pdgId_.clear();
  
    nExtraBJets_ = 0;
    nExtraCJets_ = 0;
  
    genJet_matchedTo_extraBQuark_TLV_.clear();
    genJet_matchedTo_extraBQuark_isSelected_.clear();
  
    genJet_matchedTo_extraCQuark_TLV_.clear();
    genJet_matchedTo_extraCQuark_isSelected_.clear();


    // TTbar Classification Variables
    isTTLF_ = 0;
    isTTCC_ = 0;
    isTT1B_ = 0;
    isTT2B_ = 0;
    
  
    // Tight GenParticle Lepton from Selection
    genParticle_tightLepton_TLV_.clear();
    genParticle_tightLepton_status_.clear();
    genParticle_tightLepton_pdgId_.clear();
    genParticle_tightLepton_mother_pdgId_.clear();


    // GenJet Variables
    genJet_TLV_.clear();
    genJet_isSelected_.clear();
  
  
    // GenParticle
    genParticle_TLV_.clear();
    genParticle_status_.clear();
    genParticle_pdgId_.clear();
    genParticle_mother_pdgId_.clear();
    
    
    return;
  }



};


typedef std::vector<BEANeventVars_NLO> vBEANeventVars_NLO;

  
#endif
