#ifndef ttH_LeptonPlusJets_AnalysisCode_TriggerStudyEventVars_h
#define ttH_LeptonPlusJets_AnalysisCode_TriggerStudyEventVars_h

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

struct triggerStudyEventVars{


  //////////////////////////////////////////////////////////////////////////
  ///  Tree branches/leaves
  //////////////////////////////////////////////////////////////////////////

  explicit triggerStudyEventVars() { }

  int numJets_;
  int numTags_;
  int numPVs_;

  int pass_HLT_Ele27_eta2p1_WP85_Gsf_v1_;
  int pass_HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1_;
  int pass_HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet40_v1_;
  int pass_HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1_;

  int pass_HLT_Ele32_eta2p1_WP85_Gsf_v1_;
  int pass_HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1_;
  int pass_HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet40_v1_;
  int pass_HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1_;

  int pass_HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1_;

  double L1HTT_;

  vdouble jet_pt_;
  vdouble jet_eta_;
  vdouble jet_phi_;
  vdouble jet_csv_;
  vint    jet_flavor_;

  vdouble ele_pt_;
  vdouble ele_eta_;
  vdouble ele_phi_;
  vdouble ele_relIso_;
  vdouble ele_scSigmaIEtaIEta_;
  vdouble ele_hadronicOverEm_;
  vdouble ele_relEcalIso_;
  vdouble ele_relHcalIso_;
  vdouble ele_relTrackIso_;
  vdouble ele_OneOESuperMinusOneOP_;
  vint ele_numMissingHits_;
  vint ele_isEB_;
  vint ele_genMatch_;
  vint ele_tightId_;
  vint ele_tightHLTId_;
  vint ele_matchL1Iso_;
  vint ele_matchHLT_;

  vint ele_matchHLT_hltL1sL1EG25erHTT125_;
  vint ele_matchHLT_hltNewEG27EtFilter_;
  vint ele_matchHLT_hltNewEle27WP85ClusterShapeFilter_;
  vint ele_matchHLT_hltNewEle27WP85HEFilter_;
  vint ele_matchHLT_hltNewEle27WP85EcalIsoFilter_;
  vint ele_matchHLT_hltNewEle27WP85HcalIsoFilter_;
  vint ele_matchHLT_hltNewEle27WP85PixelMatchFilter_;
  vint ele_matchHLT_hltNewEle27WP85GsfOneOEMinusOneOPFilter_;
  vint ele_matchHLT_hltNewEle27WP85GsfMissingHitsFilter_;
  vint ele_matchHLT_hltNewEle27WP85GsfDetaFilter_;
  vint ele_matchHLT_hltNewEle27WP85GsfDphiFilter_;
  vint ele_matchHLT_hltNewEle27WP85GsfTrackIsoFilter_;


  vdouble hltL1SingleIsoEG25er_pt_;
  vdouble hltL1SingleIsoEG25er_eta_;
  vdouble hltL1SingleIsoEG25er_phi_;

  vdouble hltEle27WP85Gsf_pt_;
  vdouble hltEle27WP85Gsf_eta_;
  vdouble hltEle27WP85Gsf_phi_;
  vstring hltEle27WP85Gsf_filter_;

  vdouble hltJet30_pt_;
  vdouble hltJet30_eta_;
  vdouble hltJet30_phi_;

  vdouble hltBtagJet30_pt_;
  vdouble hltBtagJet30_eta_;
  vdouble hltBtagJet30_phi_;

  vdouble hltJet40_pt_;
  vdouble hltJet40_eta_;
  vdouble hltJet40_phi_;

  vdouble hltPFHT200_pt_;
  vdouble hltPFHT200_eta_;
  vdouble hltPFHT200_phi_;
  vint    hltPFHT200_id_;

  vdouble L1IsoEG_Stage1_pt_;
  vdouble L1IsoEG_Stage1_eta_;
  vdouble L1IsoEG_Stage1_phi_;

  vdouble L1NonIsoEG_Stage1_pt_;
  vdouble L1NonIsoEG_Stage1_eta_;
  vdouble L1NonIsoEG_Stage1_phi_;

  vdouble L1CenJet_Stage1_pt_;
  vdouble L1CenJet_Stage1_eta_;
  vdouble L1CenJet_Stage1_phi_;


  void initialize();

};


typedef std::vector<triggerStudyEventVars> vtriggerStudyEventVars;


void triggerStudyEventVars::initialize(){

  numJets_ = -99;
  numTags_ = -99;
  numPVs_  = -99;

  pass_HLT_Ele27_eta2p1_WP85_Gsf_v1_ = -99;
  pass_HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1_ = -99;
  pass_HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet40_v1_ = -99;
  pass_HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1_ = -99;

  pass_HLT_Ele32_eta2p1_WP85_Gsf_v1_ = -99;
  pass_HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1_ = -99;
  pass_HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet40_v1_ = -99;
  pass_HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1_ = -99;

  pass_HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1_ = -99;

  L1HTT_ = -1;

  jet_pt_.clear();
  jet_eta_.clear();
  jet_phi_.clear();
  jet_csv_.clear();
  jet_flavor_.clear();

  ele_pt_.clear();
  ele_eta_.clear();
  ele_phi_.clear();
  ele_relIso_.clear();
  ele_scSigmaIEtaIEta_.clear();
  ele_hadronicOverEm_.clear();
  ele_relEcalIso_.clear();
  ele_relHcalIso_.clear();
  ele_relTrackIso_.clear();
  ele_OneOESuperMinusOneOP_.clear();
  ele_numMissingHits_.clear();
  ele_isEB_.clear();
  ele_genMatch_.clear();
  ele_tightId_.clear();
  ele_tightHLTId_.clear();
  ele_matchL1Iso_.clear();
  ele_matchHLT_.clear();

  ele_matchHLT_hltL1sL1EG25erHTT125_.clear();
  ele_matchHLT_hltNewEG27EtFilter_.clear();
  ele_matchHLT_hltNewEle27WP85ClusterShapeFilter_.clear();
  ele_matchHLT_hltNewEle27WP85HEFilter_.clear();
  ele_matchHLT_hltNewEle27WP85EcalIsoFilter_.clear();
  ele_matchHLT_hltNewEle27WP85HcalIsoFilter_.clear();
  ele_matchHLT_hltNewEle27WP85PixelMatchFilter_.clear();
  ele_matchHLT_hltNewEle27WP85GsfOneOEMinusOneOPFilter_.clear();
  ele_matchHLT_hltNewEle27WP85GsfMissingHitsFilter_.clear();
  ele_matchHLT_hltNewEle27WP85GsfDetaFilter_.clear();
  ele_matchHLT_hltNewEle27WP85GsfDphiFilter_.clear();
  ele_matchHLT_hltNewEle27WP85GsfTrackIsoFilter_.clear();


  hltL1SingleIsoEG25er_pt_.clear();
  hltL1SingleIsoEG25er_eta_.clear();
  hltL1SingleIsoEG25er_phi_.clear();

  hltEle27WP85Gsf_pt_.clear();
  hltEle27WP85Gsf_eta_.clear();
  hltEle27WP85Gsf_phi_.clear();
  hltEle27WP85Gsf_filter_.clear();

  hltJet30_pt_.clear();
  hltJet30_eta_.clear();
  hltJet30_phi_.clear();

  hltBtagJet30_pt_.clear();
  hltBtagJet30_eta_.clear();
  hltBtagJet30_phi_.clear();

  hltJet40_pt_.clear();
  hltJet40_eta_.clear();
  hltJet40_phi_.clear();

  hltPFHT200_pt_.clear();
  hltPFHT200_eta_.clear();
  hltPFHT200_phi_.clear();
  hltPFHT200_id_.clear();


  L1IsoEG_Stage1_pt_.clear();
  L1IsoEG_Stage1_eta_.clear();
  L1IsoEG_Stage1_phi_.clear();

  L1NonIsoEG_Stage1_pt_.clear();
  L1NonIsoEG_Stage1_eta_.clear();
  L1NonIsoEG_Stage1_phi_.clear();

  L1CenJet_Stage1_pt_.clear();
  L1CenJet_Stage1_eta_.clear();
  L1CenJet_Stage1_phi_.clear();

  return;
}

  

#endif
