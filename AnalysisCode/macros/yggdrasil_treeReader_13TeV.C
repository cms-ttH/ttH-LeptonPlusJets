#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH3.h"
#include "TH2F.h"
#include "TF1.h"
#include "TF2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "TMath.h"
#include "TRandom3.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TGraphAsymmErrors.h"
#include "TVector.h"
#include "TLorentzVector.h"
#include "Math/Interpolator.h"


#ifdef __MAKECINT__
#pragma link C++ class std::vector< TLorentzVector >+; 
#endif


#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"


#if !defined(__CINT__) && !defined(__MAKECINT__)

//#include "ttH-LeptonPlusJets/AnalysisCode/interface/BEANloader.h"
#include "ttH-LeptonPlusJets/AnalysisCode/interface/YggdrasilEventVars.h"

#endif


//*****************************************************************************
typedef std::vector< TLorentzVector >          vecTLorentzVector;
typedef std::vector<int>                       vint;
typedef std::vector<double>                    vdouble;
typedef std::vector<std::vector<double> >      vvdouble;

// CSV reweighting
TH1D* h_csv_wgt_hf[9][6];
TH1D* c_csv_wgt_hf[9][6];
TH1D* h_csv_wgt_lf[9][4][3];

int getTopSystem(TLorentzVector leptonV, TLorentzVector metV, vecTLorentzVector jetsV, vdouble btagV,
		 double &minChi, TLorentzVector &hadW, TLorentzVector &lepW, TLorentzVector &hadB, TLorentzVector &lepB, TLorentzVector &hadT, TLorentzVector &lepT);


// ------- Geoff Additions: -------
void convert_jets_to_TLVs(vvdouble jets, vecTLorentzVector &vect_of_jet_TLVs);
void vect_of_tagged_TLVs(vvdouble jets, vdouble jetCSV, vecTLorentzVector &vect_of_btag_TLVs);
void vect_of_untagged_TLVs(vvdouble jets, vdouble jetCSV, vecTLorentzVector &vect_of_nontagged_TLVs);

double w_mass_top(vvdouble jets, vdouble jetCSV);
double had_top_mass(vvdouble jets, vdouble jetCSV);

double ww_mass_nontop(vvdouble jets, vdouble jetCSV);
void nontop_untags(vvdouble jets, vdouble jetCSV, vecTLorentzVector &vect_of_nontagged_TLVs_not_top);
double avg_pt_nontop_untags(vvdouble jets, vdouble jetCSV);

double pt_E_ratio_jets(vvdouble jets);

double getBestHiggsMass2(TLorentzVector lepton, TLorentzVector &met, vecTLorentzVector jets, vdouble btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, TLorentzVector &toplep, TLorentzVector &tophad);
double get_bhm_from_own_algo(double MET, double METphi, TLorentzVector &metv, vdouble lep, vvdouble jets, vdouble csv, double &minChi, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, double &dRbb);
double study_tops_bb_syst (double MET, double METphi, TLorentzVector &metv, vdouble lep, vvdouble jets, vdouble csv, double &minChi, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, double &dRbb, double &testquant1, double &testquant2, double &testquant3, double &testquant4, double &testquant5, double &testquant6, double &testquant7, TLorentzVector &b1, TLorentzVector &b2);

double get_jet_jet_etamax (vvdouble jets);
double get_jet_tag_etamax (vvdouble jets, vdouble jetCSV);
double get_tag_tag_etamax (vvdouble jets, vdouble jetCSV);
double get_median_bb_mass (vvdouble jets, vdouble jetCSV);

// double get_csv_wgt(vvdouble jets, vdouble jetCSV, vint jetFlavor, int systematic);

//*****************************************************************************

void yggdrasil_treeReader_13TeV(  int insample=1, int maxNentries=-1, int Njobs=1, int jobN=1 ) {

  int lepSel = 2;
  int hdecay = -1;
  // for( int iSys=0; iSys<9; iSys++ ){
  //   for( int iPt=0; iPt<5; iPt++ ) h_csv_wgt_hf[iSys][iPt] = NULL;
  //   for( int iPt=0; iPt<3; iPt++ ){
  //     for( int iEta=0; iEta<3; iEta++ )h_csv_wgt_lf[iSys][iPt][iEta] = NULL;
  //   }
  // }
  // for( int iSys=0; iSys<5; iSys++ ){
  //   for( int iPt=0; iPt<5; iPt++ ) c_csv_wgt_hf[iSys][iPt] = NULL;
  // }

  // TFile* f_CSVwgt_HF = new TFile ((string(getenv("CMSSW_BASE")) + "/src/BEAN/BEANmaker/data/csv_rwt_hf_IT_sean_2014_07_27.root").c_str());
  // TFile* f_CSVwgt_LF = new TFile ((string(getenv("CMSSW_BASE")) + "/src/BEAN/BEANmaker/data/csv_rwt_lf_IT_sean_2014_07_27.root").c_str());


  // // CSV reweighting
  // for( int iSys=0; iSys<9; iSys++ ){
  //   TString syst_csv_suffix_hf = "final";
  //   TString syst_csv_suffix_c = "final";
  //   TString syst_csv_suffix_lf = "final";
    
  //   switch( iSys ){
  //   case 0:
  //     // this is the nominal case
  //     break;
  //   case 1:
  //     // JESUp
  //     syst_csv_suffix_hf = "final_JESUp"; syst_csv_suffix_lf = "final_JESUp";
  //     syst_csv_suffix_c  = "final_cErr1Up";
  //     break;
  //   case 2:
  //     // JESDown
  //     syst_csv_suffix_hf = "final_JESDown"; syst_csv_suffix_lf = "final_JESDown";
  //     syst_csv_suffix_c  = "final_cErr1Down";
  //     break;
  //   case 3:
  //     // purity up
  //     syst_csv_suffix_hf = "final_LFUp"; syst_csv_suffix_lf = "final_HFUp";
  //     syst_csv_suffix_c  = "final_cErr2Up";
  //     break;
  //   case 4:
  //     // purity down
  //     syst_csv_suffix_hf = "final_LFDown"; syst_csv_suffix_lf = "final_HFDown";
  //     syst_csv_suffix_c  = "final_cErr2Down";
  //     break;
  //   case 5:
  //     // stats1 up
  //     syst_csv_suffix_hf = "final_Stats1Up"; syst_csv_suffix_lf = "final_Stats1Up";
  //     break;
  //   case 6:
  //     // stats1 down
  //     syst_csv_suffix_hf = "final_Stats1Down"; syst_csv_suffix_lf = "final_Stats1Down";
  //     break;
  //   case 7:
  //     // stats2 up
  //     syst_csv_suffix_hf = "final_Stats2Up"; syst_csv_suffix_lf = "final_Stats2Up";
  //     break;
  //   case 8:
  //     // stats2 down
  //     syst_csv_suffix_hf = "final_Stats2Down"; syst_csv_suffix_lf = "final_Stats2Down";
  //     break;
  //   }

  //   for( int iPt=0; iPt<6; iPt++ ) h_csv_wgt_hf[iSys][iPt] = (TH1D*)f_CSVwgt_HF->Get( Form("csv_ratio_Pt%i_Eta0_%s",iPt,syst_csv_suffix_hf.Data()) );

  //   if( iSys<5 ){
  //     for( int iPt=0; iPt<6; iPt++ ) c_csv_wgt_hf[iSys][iPt] = (TH1D*)f_CSVwgt_HF->Get( Form("c_csv_ratio_Pt%i_Eta0_%s",iPt,syst_csv_suffix_c.Data()) );
  //   }
    
  //   for( int iPt=0; iPt<4; iPt++ ){
  //     for( int iEta=0; iEta<3; iEta++ )h_csv_wgt_lf[iSys][iPt][iEta] = (TH1D*)f_CSVwgt_LF->Get( Form("csv_ratio_Pt%i_Eta%i_%s",iPt,iEta,syst_csv_suffix_lf.Data()) );
  //   }
  // }


  std::cout << "   ===> load the root files! " << std::endl;

  std::string sampleName = "TTJets_MSDecaysCKM_central_Tune4C_13TeV_madgraph_PU20bx25_POSTLS170_V5_v1";

  std::string sampleType = ( insample>=0 ) ? "mc" : "data";
  std::string str_jobN;
  std::stringstream stream;
  stream << jobN;
  str_jobN = stream.str();

  std::string leptonType;
  if( lepSel==0 )      leptonType = "ele_sel";
  else if( lepSel==1 ) leptonType = "mu_sel";
  else if( lepSel==2 ) leptonType = "lep_sel";


  std::string treefilename = "/uscms_data/d2/dpuigh/TTH/miniAOD/CMSSW_7_0_7_patch1/src/ttH-LeptonPlusJets/AnalysisCode/yggdrasil_treeMaker_mc_TTJets_MSDecaysCKM_central_Tune4C_13TeV_madgraph_PU20bx25_POSTLS170_V5_v1.root";


  std::string use_sampleName = sampleName;
  bool doFilterDecay = false;
  if( (use_sampleName.find("TTH")!=std::string::npos) && hdecay>=0 ){
    doFilterDecay = true;

    std::string s_inclusive = "Inclusive";

    std::cout << "\t Original sample name = " << use_sampleName << std::endl;
    //size_t index = use_sampleName.find(s_inclusive, index);
    size_t index = use_sampleName.find(s_inclusive);

    std::string s_new = "";
    switch( hdecay ){
    case 0: s_new = "hbb"; break;
    case 1: s_new = "hcc"; break;
    case 2: s_new = "hww"; break;
    case 3: s_new = "hzz"; break;
    case 4: s_new = "htt"; break;
    case 5: s_new = "hgg"; break;
    case 6: s_new = "hjj"; break;
    case 7: s_new = "hzg"; break;
    default: s_new = s_inclusive; break;
    }

    if( index!=string::npos ) use_sampleName.replace(index, s_inclusive.size(), s_new);
    std::cout << "\t New sample name = " << use_sampleName << std::endl;
  }

  std::string s_end = "_histo_" + str_jobN + ".root";
  //if( doFilterDecay && Njobs==1 ) s_end = "_histo.root";
  if( Njobs==1 ) s_end = "_histo.root";


  std::string histofilename = "/uscms_data/d2/dpuigh/TTH/miniAOD/CMSSW_7_0_7_patch1/src/ttH-LeptonPlusJets/AnalysisCode/yggdrasil_treeReader_13TeV_" + sampleType + "_" + use_sampleName + "_" + leptonType + s_end;
  //std::string histofilename = "/data/users/puigh/yggdrasil/Output/HistoFiles/treeReader/sean_yggdrasil_loose_consistent/yggdrasil_treeReader_" + sampleType + "_" + use_sampleName + "_" + leptonType + s_end;

  std::cout << "  treefilename  = " << treefilename.c_str() << std::endl;
  std::cout << "  histofilename = " << histofilename.c_str() << std::endl;

  TChain *chain = new TChain("worldTree");
  chain->Add(treefilename.c_str());



  //////////////////////////////////////////////////////////////////////////
  ///  Reader information
  //////////////////////////////////////////////////////////////////////////


  /*
  std::vector<std::string> weight_dirv3;
  weight_dirv3.push_back("423/");
  weight_dirv3.push_back("523/");
  weight_dirv3.push_back("623/");
  weight_dirv3.push_back("433/");
  weight_dirv3.push_back("533/");
  weight_dirv3.push_back("633/");
  weight_dirv3.push_back("443/");
  weight_dirv3.push_back("543/");
  weight_dirv3.push_back("643/");
  weight_dirv3.push_back("423/");
  weight_dirv3.push_back("523/");
  weight_dirv3.push_back("623/");
  */

  std::vector<std::string> cat_labels;
  cat_labels.push_back("4j2t");
  cat_labels.push_back("5j2t");
  cat_labels.push_back("6j2t");
  cat_labels.push_back("4j3t");
  cat_labels.push_back("5j3t");
  cat_labels.push_back("6j3t");
  cat_labels.push_back("4j4t");
  cat_labels.push_back("5j4t");
  cat_labels.push_back("6j4t");
  cat_labels.push_back("4j1t");
  cat_labels.push_back("5j1t");
  cat_labels.push_back("6j1t");
  cat_labels.push_back("Incl");

  int NumCat = int(cat_labels.size());


  /*
  TMVA::Reader *reader_ttH_ttbb_8TeV_CFMLP = new TMVA::Reader("!Color:Silent");
  TMVA::Reader *reader_ttH_ttbb_best15_8TeV_BDT = new TMVA::Reader("!Color:Silent");
  TMVA::Reader *reader_ttH_ttbb_6j3t_8TeV_BDT = new TMVA::Reader("!Color:Silent");
  TMVA::Reader *reader_ttH_ttbb_5j4t_8TeV_BDT = new TMVA::Reader("!Color:Silent");
  TMVA::Reader *reader_ttH_ttbb_best10_8TeV_BDT = new TMVA::Reader("!Color:Silent");
  TMVA::Reader *reader_final10v16_8TeV_BDT[NumCat];
  TMVA::Reader *reader_ttbb_ttH_8TeV_CFMLP[NumCat];
  TMVA::Reader *reader_ttbb_ttH_8TeV_BDT[NumCat];
  TMVA::Reader *reader_ttbb_ttlf_8TeV_CFMLP[NumCat];
  TMVA::Reader *reader_ttbb_ttlf_8TeV_BDT[NumCat];
  for( int c=0; c<NumCat; c++ ){
    reader_final10v16_8TeV_BDT[c] = new TMVA::Reader("!Color:Silent");
    reader_ttbb_ttH_8TeV_CFMLP[c] = new TMVA::Reader("!Color:Silent");
    reader_ttbb_ttH_8TeV_BDT[c] = new TMVA::Reader("!Color:Silent");
    reader_ttbb_ttlf_8TeV_CFMLP[c] = new TMVA::Reader("!Color:Silent");
    reader_ttbb_ttlf_8TeV_BDT[c] = new TMVA::Reader("!Color:Silent");
  }
  */

  Float_t numJets_float; // due to a bug the reader only takes floats
  Float_t numTags_float;

  Float_t tight_lepton_pt;
  Float_t tight_lepton_phi;
  Float_t first_jet_pt;
  Float_t min_dr_tagged_jets;
  Float_t avg_dr_tagged_jets;
  Float_t aplanarity;
  Float_t sphericity;
  Float_t avg_btag_disc_non_btags;
  Float_t MET;
  Float_t second_jet_pt;
  Float_t dr_between_lep_and_closest_jet;
  Float_t h0;
  Float_t avg_btag_disc_btags;
  Float_t dev_from_avg_disc_btags;
  Float_t third_jet_pt;
  Float_t fourth_jet_pt;
  Float_t avg_tagged_dijet_mass;
  Float_t h1;
  Float_t h2;
  Float_t lowest_btag;
  Float_t all_sum_pt_with_met;
  Float_t all_sum_pt_incl_met;
  Float_t all_sum_pt;
  Float_t avg_untagged_dijet_mass;
  Float_t closest_tagged_dijet_mass;
  Float_t h3;
  Float_t h4;
  Float_t first_highest_btag;
  Float_t second_highest_btag;
  Float_t third_highest_btag;
  Float_t fourth_highest_btag;
  //Float_t dijet_mass_of_everything;

  Float_t MHT;
  Float_t MHT_phi;

  Float_t invariant_mass_of_everything;

  Float_t MT_mht;

  Float_t HT;

  Float_t M3;
  Float_t M3_1tag;
  Float_t Mlb;

  Float_t best_higgs_mass;
  Float_t minChi2;
  Float_t dRbb;

  Float_t tagged_dijet_mass_closest_to_125;

  // Float_t ttbb_tth_ANN_output = 0.;
  // Float_t ttbb_tth_ANN_output_best15 = 0.;
  // Float_t ttbb_tth_ANN_output_best10 = 0.;
  // Float_t ttbb_tth_MVA_output_5j4t = 0.;
  // Float_t ttbb_tth_MVA_output_6j3t = 0.;

  Float_t bhmv2 = 0.;
  Float_t maxeta_jet_jet = 0.;			 
  Float_t maxeta_jet_tag = 0.;			 
  Float_t maxeta_tag_tag = 0.;			 
  Float_t abs_dEta_leptop_bb = 0.;		 
  Float_t abs_dEta_hadtop_bb = 0.;		 
  Float_t dEta_fn = 0.;					 
  Float_t abs_bb_eta = 0.;				 
  Float_t angle_tops_bb = 0.;			 
  Float_t median_bb_mass = 0.;			 
  Float_t pt_all_jets_over_E_all_jets = 0.;

  //Float_t MHT_float = 0.;




  /////////////


  /////////////
  /////////////
  /////////////
  /*
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets);		 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "sphericity", &sphericity);				 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "avg_untagged_dijet_mass", &avg_untagged_dijet_mass);	 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "h3", &h3);						 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "second_highest_btag", &second_highest_btag);		 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "best_higgs_mass", &best_higgs_mass);			 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "dRbb", &dRbb);
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag);			 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag);			 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "abs(dEta_leptop_bb)", &abs_dEta_leptop_bb);		 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "abs(dEta_hadtop_bb)", &abs_dEta_hadtop_bb);		 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "dEta_fn", &dEta_fn);			 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "angle_tops_bb", &angle_tops_bb);			 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "median_bb_mass", &median_bb_mass);			 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets);
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "aplanarity", &aplanarity);				 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "second_jet_pt", &second_jet_pt);			 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "h1", &h1);						 
  reader_ttH_ttbb_8TeV_CFMLP->AddVariable( "MHT", &MHT_float);	 



  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets);		 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "sphericity", &sphericity);				 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );	 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "closest_tagged_dijet_mass", &closest_tagged_dijet_mass );	 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "h3", &h3);						 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "invariant_mass_of_everything", &invariant_mass_of_everything );		 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "best_higgs_mass", &best_higgs_mass);			 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "dRbb", &dRbb);
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag);			 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag);			 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "abs(dEta_leptop_bb)", &abs_dEta_leptop_bb);		 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "abs(dEta_hadtop_bb)", &abs_dEta_hadtop_bb);		 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "dEta_fn", &dEta_fn);			 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "median_bb_mass", &median_bb_mass);			 
  reader_ttH_ttbb_best15_8TeV_BDT->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets);

  reader_ttH_ttbb_best10_8TeV_BDT->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets);
  reader_ttH_ttbb_best10_8TeV_BDT->AddVariable( "closest_tagged_dijet_mass", &closest_tagged_dijet_mass );	 
  reader_ttH_ttbb_best10_8TeV_BDT->AddVariable( "h3", &h3);
  reader_ttH_ttbb_best10_8TeV_BDT->AddVariable( "best_higgs_mass", &best_higgs_mass);			 
  reader_ttH_ttbb_best10_8TeV_BDT->AddVariable( "dRbb", &dRbb);
  reader_ttH_ttbb_best10_8TeV_BDT->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag);
  reader_ttH_ttbb_best10_8TeV_BDT->AddVariable( "abs(dEta_leptop_bb)", &abs_dEta_leptop_bb);		 
  reader_ttH_ttbb_best10_8TeV_BDT->AddVariable( "abs(dEta_hadtop_bb)", &abs_dEta_hadtop_bb);		 
  reader_ttH_ttbb_best10_8TeV_BDT->AddVariable( "dEta_fn", &dEta_fn);
  reader_ttH_ttbb_best10_8TeV_BDT->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets);



		 
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "sphericity", &sphericity);				 
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "h3", &h3 );	 
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "maxeta_jet_jet", &maxeta_jet_jet );	 
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag);						 
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag);						 
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "abs(dEta_leptop_bb)", &abs_dEta_leptop_bb);		 
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "abs(dEta_hadtop_bb)", &abs_dEta_hadtop_bb);		 
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "dEta_fn", &dEta_fn);			 
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "M3", &M3);			 
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "min_dr_tagged_jets", &min_dr_tagged_jets);			 
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets);
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "aplanarity", &aplanarity);
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "h1", &h1);
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "tagged_dijet_mass_closest_to_125", &tagged_dijet_mass_closest_to_125);
  reader_ttH_ttbb_6j3t_8TeV_BDT->AddVariable( "third_jet_pt", &third_jet_pt);


  //here
  reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets);		 
  reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag);				 
  reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "h1", &h1 );	 
  //reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "h2", &h2 );	 
  reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "h3", &h3 );	 
  reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );	 
  //reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "HT", &HT);						 
  //reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag);
  reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "tagged_dijet_mass_closest_to_125", &tagged_dijet_mass_closest_to_125);
  reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "MET", &MET);
  reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "aplanarity", &aplanarity);
  reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets);
  reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "fourth_highest_btag", &fourth_highest_btag);
  //reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "fourth_jet_pt", &fourth_jet_pt);
  //reader_ttH_ttbb_5j4t_8TeV_BDT->AddVariable( "closest_tagged_dijet_mass", &closest_tagged_dijet_mass);



  /////////////


  /////////////

  // 4j2t
  reader_ttbb_ttH_8TeV_CFMLP[0]->AddVariable( "avg_untagged_dijet_mass", &avg_untagged_dijet_mass );
  reader_ttbb_ttH_8TeV_CFMLP[0]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[0]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[0]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[0]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[0]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_CFMLP[0]->AddVariable( "HT", &HT );
  reader_ttbb_ttH_8TeV_CFMLP[0]->AddVariable( "MHT", &MHT );
  reader_ttbb_ttH_8TeV_CFMLP[0]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttH_8TeV_CFMLP[0]->AddVariable( "third_highest_btag", &third_highest_btag );

  // 5j2t
  reader_ttbb_ttH_8TeV_CFMLP[1]->AddVariable( "avg_btag_disc_non_btags", &avg_btag_disc_non_btags );
  reader_ttbb_ttH_8TeV_CFMLP[1]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[1]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[1]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[1]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[1]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_CFMLP[1]->AddVariable( "HT", &HT );
  reader_ttbb_ttH_8TeV_CFMLP[1]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[1]->AddVariable( "Mlb", &Mlb );
  reader_ttbb_ttH_8TeV_CFMLP[1]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );


  // 6j2t
  reader_ttbb_ttH_8TeV_CFMLP[2]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_CFMLP[2]->AddVariable( "HT", &HT );
  reader_ttbb_ttH_8TeV_CFMLP[2]->AddVariable( "sphericity", &sphericity );
  reader_ttbb_ttH_8TeV_CFMLP[2]->AddVariable( "dr_between_lep_and_closest_jet", &dr_between_lep_and_closest_jet );
  reader_ttbb_ttH_8TeV_CFMLP[2]->AddVariable( "h2", &h2 );
  reader_ttbb_ttH_8TeV_CFMLP[2]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[2]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[2]->AddVariable( "maxeta_jet_jet", &maxeta_jet_jet );
  reader_ttbb_ttH_8TeV_CFMLP[2]->AddVariable( "Mlb", &Mlb );
  reader_ttbb_ttH_8TeV_CFMLP[2]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );

  // 4j3t
  reader_ttbb_ttH_8TeV_CFMLP[3]->AddVariable( "MET", &MET );
  reader_ttbb_ttH_8TeV_CFMLP[3]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[3]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[3]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[3]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[3]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_CFMLP[3]->AddVariable( "HT", &HT );
  reader_ttbb_ttH_8TeV_CFMLP[3]->AddVariable( "MHT", &MHT );
  reader_ttbb_ttH_8TeV_CFMLP[3]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[3]->AddVariable( "M3", &M3 );

  // 5j3t
  reader_ttbb_ttH_8TeV_CFMLP[4]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[4]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[4]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[4]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[4]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_CFMLP[4]->AddVariable( "HT", &HT );
  reader_ttbb_ttH_8TeV_CFMLP[4]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttH_8TeV_CFMLP[4]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[4]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[4]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );

  // 6j3t
  reader_ttbb_ttH_8TeV_CFMLP[5]->AddVariable( "h0", &h0 );
  reader_ttbb_ttH_8TeV_CFMLP[5]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_CFMLP[5]->AddVariable( "sphericity", &sphericity );
  reader_ttbb_ttH_8TeV_CFMLP[5]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttH_8TeV_CFMLP[5]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[5]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[5]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[5]->AddVariable( "maxeta_jet_jet", &maxeta_jet_jet );
  reader_ttbb_ttH_8TeV_CFMLP[5]->AddVariable( "first_highest_btag", &first_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[5]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  // reader_ttbb_ttH_8TeV_CFMLP[5]->AddVariable( "best_higgs_mass", &best_higgs_mass );

  // 4j4t
  reader_ttbb_ttH_8TeV_CFMLP[6]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[6]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[6]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttH_8TeV_CFMLP[6]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_CFMLP[6]->AddVariable( "HT", &HT );
  reader_ttbb_ttH_8TeV_CFMLP[6]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttH_8TeV_CFMLP[6]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[6]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[6]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[6]->AddVariable( "M3", &M3 );

  // 5j4t
  reader_ttbb_ttH_8TeV_CFMLP[7]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_CFMLP[7]->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets );
  reader_ttbb_ttH_8TeV_CFMLP[7]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttH_8TeV_CFMLP[7]->AddVariable( "dev_from_avg_disc_btags", &dev_from_avg_disc_btags );
  reader_ttbb_ttH_8TeV_CFMLP[7]->AddVariable( "lowest_btag", &lowest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[7]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[7]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[7]->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag );
  reader_ttbb_ttH_8TeV_CFMLP[7]->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag );
  reader_ttbb_ttH_8TeV_CFMLP[7]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  // reader_ttbb_ttH_8TeV_CFMLP[7]->AddVariable( "best_higgs_mass", &best_higgs_mass );

  // 6j4t
  reader_ttbb_ttH_8TeV_CFMLP[8]->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets );
  reader_ttbb_ttH_8TeV_CFMLP[8]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttH_8TeV_CFMLP[8]->AddVariable( "closest_tagged_dijet_mass", &closest_tagged_dijet_mass );
  reader_ttbb_ttH_8TeV_CFMLP[8]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_CFMLP[8]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  //reader_ttbb_ttH_8TeV_CFMLP[8]->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag );
  reader_ttbb_ttH_8TeV_CFMLP[8]->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag );
  //reader_ttbb_ttH_8TeV_CFMLP[8]->AddVariable( "median_bb_mass", &median_bb_mass );
  reader_ttbb_ttH_8TeV_CFMLP[8]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  reader_ttbb_ttH_8TeV_CFMLP[8]->AddVariable( "ttbb_tth_ANN_output", &ttbb_tth_ANN_output );
  reader_ttbb_ttH_8TeV_CFMLP[8]->AddVariable( "best_higgs_mass", &best_higgs_mass );
  reader_ttbb_ttH_8TeV_CFMLP[8]->AddVariable( "dEta_fn", &dEta_fn );

  /////////////

  /////////////
  /////////////



  /////////////

  // 4j2t
  reader_ttbb_ttH_8TeV_BDT[0]->AddVariable( "avg_untagged_dijet_mass", &avg_untagged_dijet_mass );
  reader_ttbb_ttH_8TeV_BDT[0]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[0]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[0]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[0]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[0]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_BDT[0]->AddVariable( "HT", &HT );
  reader_ttbb_ttH_8TeV_BDT[0]->AddVariable( "MHT", &MHT );
  reader_ttbb_ttH_8TeV_BDT[0]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttH_8TeV_BDT[0]->AddVariable( "third_highest_btag", &third_highest_btag );

  // 5j2t
  reader_ttbb_ttH_8TeV_BDT[1]->AddVariable( "avg_btag_disc_non_btags", &avg_btag_disc_non_btags );
  reader_ttbb_ttH_8TeV_BDT[1]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[1]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[1]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[1]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[1]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_BDT[1]->AddVariable( "HT", &HT );
  reader_ttbb_ttH_8TeV_BDT[1]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[1]->AddVariable( "Mlb", &Mlb );
  reader_ttbb_ttH_8TeV_BDT[1]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );


  // 6j2t
  reader_ttbb_ttH_8TeV_BDT[2]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_BDT[2]->AddVariable( "HT", &HT );
  reader_ttbb_ttH_8TeV_BDT[2]->AddVariable( "sphericity", &sphericity );
  reader_ttbb_ttH_8TeV_BDT[2]->AddVariable( "dr_between_lep_and_closest_jet", &dr_between_lep_and_closest_jet );
  reader_ttbb_ttH_8TeV_BDT[2]->AddVariable( "h2", &h2 );
  reader_ttbb_ttH_8TeV_BDT[2]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[2]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[2]->AddVariable( "maxeta_jet_jet", &maxeta_jet_jet );
  reader_ttbb_ttH_8TeV_BDT[2]->AddVariable( "Mlb", &Mlb );
  reader_ttbb_ttH_8TeV_BDT[2]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );

  // 4j3t
  reader_ttbb_ttH_8TeV_BDT[3]->AddVariable( "MET", &MET );
  reader_ttbb_ttH_8TeV_BDT[3]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[3]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[3]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[3]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[3]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_BDT[3]->AddVariable( "HT", &HT );
  reader_ttbb_ttH_8TeV_BDT[3]->AddVariable( "MHT", &MHT );
  reader_ttbb_ttH_8TeV_BDT[3]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[3]->AddVariable( "M3", &M3 );

  // 5j3t
  reader_ttbb_ttH_8TeV_BDT[4]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[4]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[4]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[4]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[4]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_BDT[4]->AddVariable( "HT", &HT );
  reader_ttbb_ttH_8TeV_BDT[4]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttH_8TeV_BDT[4]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[4]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[4]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );

  // 6j3t
  reader_ttbb_ttH_8TeV_BDT[5]->AddVariable( "h0", &h0 );
  reader_ttbb_ttH_8TeV_BDT[5]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_BDT[5]->AddVariable( "sphericity", &sphericity );
  reader_ttbb_ttH_8TeV_BDT[5]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttH_8TeV_BDT[5]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[5]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[5]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[5]->AddVariable( "maxeta_jet_jet", &maxeta_jet_jet );
  reader_ttbb_ttH_8TeV_BDT[5]->AddVariable( "first_highest_btag", &first_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[5]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  //reader_ttbb_ttH_8TeV_BDT[5]->AddVariable( "best_higgs_mass", &best_higgs_mass );

  // 4j4t
  reader_ttbb_ttH_8TeV_BDT[6]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[6]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[6]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttH_8TeV_BDT[6]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_BDT[6]->AddVariable( "HT", &HT );
  reader_ttbb_ttH_8TeV_BDT[6]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttH_8TeV_BDT[6]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[6]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[6]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[6]->AddVariable( "M3", &M3 );

  // 5j4t
  reader_ttbb_ttH_8TeV_BDT[7]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttH_8TeV_BDT[7]->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets );
  reader_ttbb_ttH_8TeV_BDT[7]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttH_8TeV_BDT[7]->AddVariable( "dev_from_avg_disc_btags", &dev_from_avg_disc_btags );
  reader_ttbb_ttH_8TeV_BDT[7]->AddVariable( "lowest_btag", &lowest_btag );
  reader_ttbb_ttH_8TeV_BDT[7]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[7]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[7]->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag );
  reader_ttbb_ttH_8TeV_BDT[7]->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag );
  reader_ttbb_ttH_8TeV_BDT[7]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  //reader_ttbb_ttH_8TeV_BDT[7]->AddVariable( "best_higgs_mass", &best_higgs_mass );

  // 6j4t
  reader_ttbb_ttH_8TeV_BDT[8]->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets );
  reader_ttbb_ttH_8TeV_BDT[8]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttH_8TeV_BDT[8]->AddVariable( "closest_tagged_dijet_mass", &closest_tagged_dijet_mass );
  reader_ttbb_ttH_8TeV_BDT[8]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttH_8TeV_BDT[8]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  //reader_ttbb_ttH_8TeV_BDT[8]->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag );
  reader_ttbb_ttH_8TeV_BDT[8]->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag );
  //reader_ttbb_ttH_8TeV_BDT[8]->AddVariable( "median_bb_mass", &median_bb_mass );
  reader_ttbb_ttH_8TeV_BDT[8]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  reader_ttbb_ttH_8TeV_BDT[8]->AddVariable( "ttbb_tth_ANN_output", &ttbb_tth_ANN_output );
  reader_ttbb_ttH_8TeV_BDT[8]->AddVariable( "best_higgs_mass", &best_higgs_mass );
  reader_ttbb_ttH_8TeV_BDT[8]->AddVariable( "dEta_fn", &dEta_fn );

  /////////////



  /////////////

  // 4j2t
  reader_ttbb_ttlf_8TeV_CFMLP[0]->AddVariable( "avg_untagged_dijet_mass", &avg_untagged_dijet_mass );
  reader_ttbb_ttlf_8TeV_CFMLP[0]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[0]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[0]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[0]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[0]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_CFMLP[0]->AddVariable( "HT", &HT );
  reader_ttbb_ttlf_8TeV_CFMLP[0]->AddVariable( "MHT", &MHT );
  reader_ttbb_ttlf_8TeV_CFMLP[0]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttlf_8TeV_CFMLP[0]->AddVariable( "third_highest_btag", &third_highest_btag );

  // 5j2t
  reader_ttbb_ttlf_8TeV_CFMLP[1]->AddVariable( "avg_btag_disc_non_btags", &avg_btag_disc_non_btags );
  reader_ttbb_ttlf_8TeV_CFMLP[1]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[1]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[1]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[1]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[1]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_CFMLP[1]->AddVariable( "HT", &HT );
  reader_ttbb_ttlf_8TeV_CFMLP[1]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[1]->AddVariable( "Mlb", &Mlb );
  reader_ttbb_ttlf_8TeV_CFMLP[1]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );


  // 6j2t
  reader_ttbb_ttlf_8TeV_CFMLP[2]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_CFMLP[2]->AddVariable( "HT", &HT );
  reader_ttbb_ttlf_8TeV_CFMLP[2]->AddVariable( "sphericity", &sphericity );
  reader_ttbb_ttlf_8TeV_CFMLP[2]->AddVariable( "dr_between_lep_and_closest_jet", &dr_between_lep_and_closest_jet );
  reader_ttbb_ttlf_8TeV_CFMLP[2]->AddVariable( "h2", &h2 );
  reader_ttbb_ttlf_8TeV_CFMLP[2]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[2]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[2]->AddVariable( "maxeta_jet_jet", &maxeta_jet_jet );
  reader_ttbb_ttlf_8TeV_CFMLP[2]->AddVariable( "Mlb", &Mlb );
  reader_ttbb_ttlf_8TeV_CFMLP[2]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );

  // 4j3t
  reader_ttbb_ttlf_8TeV_CFMLP[3]->AddVariable( "MET", &MET );
  reader_ttbb_ttlf_8TeV_CFMLP[3]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[3]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[3]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[3]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[3]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_CFMLP[3]->AddVariable( "HT", &HT );
  reader_ttbb_ttlf_8TeV_CFMLP[3]->AddVariable( "MHT", &MHT );
  reader_ttbb_ttlf_8TeV_CFMLP[3]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[3]->AddVariable( "M3", &M3 );

  // 5j3t
  reader_ttbb_ttlf_8TeV_CFMLP[4]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[4]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[4]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[4]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[4]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_CFMLP[4]->AddVariable( "HT", &HT );
  reader_ttbb_ttlf_8TeV_CFMLP[4]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttlf_8TeV_CFMLP[4]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[4]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[4]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );

  // 6j3t
  reader_ttbb_ttlf_8TeV_CFMLP[5]->AddVariable( "h0", &h0 );
  reader_ttbb_ttlf_8TeV_CFMLP[5]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_CFMLP[5]->AddVariable( "sphericity", &sphericity );
  reader_ttbb_ttlf_8TeV_CFMLP[5]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttlf_8TeV_CFMLP[5]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[5]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[5]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[5]->AddVariable( "maxeta_jet_jet", &maxeta_jet_jet );
  reader_ttbb_ttlf_8TeV_CFMLP[5]->AddVariable( "first_highest_btag", &first_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[5]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  //reader_ttbb_ttlf_8TeV_CFMLP[5]->AddVariable( "best_higgs_mass", &best_higgs_mass );

  // 4j4t
  reader_ttbb_ttlf_8TeV_CFMLP[6]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[6]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[6]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttlf_8TeV_CFMLP[6]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_CFMLP[6]->AddVariable( "HT", &HT );
  reader_ttbb_ttlf_8TeV_CFMLP[6]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttlf_8TeV_CFMLP[6]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[6]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[6]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[6]->AddVariable( "M3", &M3 );

  // 5j4t
  reader_ttbb_ttlf_8TeV_CFMLP[7]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_CFMLP[7]->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets );
  reader_ttbb_ttlf_8TeV_CFMLP[7]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttlf_8TeV_CFMLP[7]->AddVariable( "dev_from_avg_disc_btags", &dev_from_avg_disc_btags );
  reader_ttbb_ttlf_8TeV_CFMLP[7]->AddVariable( "lowest_btag", &lowest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[7]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[7]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[7]->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag );
  reader_ttbb_ttlf_8TeV_CFMLP[7]->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag );
  reader_ttbb_ttlf_8TeV_CFMLP[7]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  //reader_ttbb_ttlf_8TeV_CFMLP[7]->AddVariable( "best_higgs_mass", &best_higgs_mass );

  // 6j4t
  reader_ttbb_ttlf_8TeV_CFMLP[8]->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets );
  reader_ttbb_ttlf_8TeV_CFMLP[8]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttlf_8TeV_CFMLP[8]->AddVariable( "closest_tagged_dijet_mass", &closest_tagged_dijet_mass );
  reader_ttbb_ttlf_8TeV_CFMLP[8]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_CFMLP[8]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  //reader_ttbb_ttlf_8TeV_CFMLP[8]->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag );
  reader_ttbb_ttlf_8TeV_CFMLP[8]->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag );
  //reader_ttbb_ttlf_8TeV_CFMLP[8]->AddVariable( "median_bb_mass", &median_bb_mass );
  reader_ttbb_ttlf_8TeV_CFMLP[8]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  reader_ttbb_ttlf_8TeV_CFMLP[8]->AddVariable( "ttbb_tth_ANN_output", &ttbb_tth_ANN_output_best10 );
  reader_ttbb_ttlf_8TeV_CFMLP[8]->AddVariable( "best_higgs_mass", &best_higgs_mass );
  reader_ttbb_ttlf_8TeV_CFMLP[8]->AddVariable( "dEta_fn", &dEta_fn );

  /////////////


  /////////////

  // 4j2t
  reader_ttbb_ttlf_8TeV_BDT[0]->AddVariable( "avg_untagged_dijet_mass", &avg_untagged_dijet_mass );
  reader_ttbb_ttlf_8TeV_BDT[0]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[0]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[0]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[0]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[0]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_BDT[0]->AddVariable( "HT", &HT );
  reader_ttbb_ttlf_8TeV_BDT[0]->AddVariable( "MHT", &MHT );
  reader_ttbb_ttlf_8TeV_BDT[0]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttlf_8TeV_BDT[0]->AddVariable( "third_highest_btag", &third_highest_btag );

  // 5j2t
  reader_ttbb_ttlf_8TeV_BDT[1]->AddVariable( "avg_btag_disc_non_btags", &avg_btag_disc_non_btags );
  reader_ttbb_ttlf_8TeV_BDT[1]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[1]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[1]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[1]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[1]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_BDT[1]->AddVariable( "HT", &HT );
  reader_ttbb_ttlf_8TeV_BDT[1]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[1]->AddVariable( "Mlb", &Mlb );
  reader_ttbb_ttlf_8TeV_BDT[1]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );


  // 6j2t
  reader_ttbb_ttlf_8TeV_BDT[2]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_BDT[2]->AddVariable( "HT", &HT );
  reader_ttbb_ttlf_8TeV_BDT[2]->AddVariable( "sphericity", &sphericity );
  reader_ttbb_ttlf_8TeV_BDT[2]->AddVariable( "dr_between_lep_and_closest_jet", &dr_between_lep_and_closest_jet );
  reader_ttbb_ttlf_8TeV_BDT[2]->AddVariable( "h2", &h2 );
  reader_ttbb_ttlf_8TeV_BDT[2]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[2]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[2]->AddVariable( "maxeta_jet_jet", &maxeta_jet_jet );
  reader_ttbb_ttlf_8TeV_BDT[2]->AddVariable( "Mlb", &Mlb );
  reader_ttbb_ttlf_8TeV_BDT[2]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );

  // 4j3t
  reader_ttbb_ttlf_8TeV_BDT[3]->AddVariable( "MET", &MET );
  reader_ttbb_ttlf_8TeV_BDT[3]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[3]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[3]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[3]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[3]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_BDT[3]->AddVariable( "HT", &HT );
  reader_ttbb_ttlf_8TeV_BDT[3]->AddVariable( "MHT", &MHT );
  reader_ttbb_ttlf_8TeV_BDT[3]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[3]->AddVariable( "M3", &M3 );

  // 5j3t
  reader_ttbb_ttlf_8TeV_BDT[4]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[4]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[4]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[4]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[4]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_BDT[4]->AddVariable( "HT", &HT );
  reader_ttbb_ttlf_8TeV_BDT[4]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttlf_8TeV_BDT[4]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[4]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[4]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );

  // 6j3t
  reader_ttbb_ttlf_8TeV_BDT[5]->AddVariable( "h0", &h0 );
  reader_ttbb_ttlf_8TeV_BDT[5]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_BDT[5]->AddVariable( "sphericity", &sphericity );
  reader_ttbb_ttlf_8TeV_BDT[5]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttlf_8TeV_BDT[5]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[5]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[5]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[5]->AddVariable( "maxeta_jet_jet", &maxeta_jet_jet );
  reader_ttbb_ttlf_8TeV_BDT[5]->AddVariable( "first_highest_btag", &first_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[5]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  //reader_ttbb_ttlf_8TeV_BDT[5]->AddVariable( "best_higgs_mass", &best_higgs_mass );

  // 4j4t
  reader_ttbb_ttlf_8TeV_BDT[6]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[6]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[6]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_ttbb_ttlf_8TeV_BDT[6]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_BDT[6]->AddVariable( "HT", &HT );
  reader_ttbb_ttlf_8TeV_BDT[6]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttlf_8TeV_BDT[6]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[6]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[6]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[6]->AddVariable( "M3", &M3 );

  // 5j4t
  reader_ttbb_ttlf_8TeV_BDT[7]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_ttbb_ttlf_8TeV_BDT[7]->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets );
  reader_ttbb_ttlf_8TeV_BDT[7]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttlf_8TeV_BDT[7]->AddVariable( "dev_from_avg_disc_btags", &dev_from_avg_disc_btags );
  reader_ttbb_ttlf_8TeV_BDT[7]->AddVariable( "lowest_btag", &lowest_btag );
  reader_ttbb_ttlf_8TeV_BDT[7]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[7]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[7]->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag );
  reader_ttbb_ttlf_8TeV_BDT[7]->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag );
  reader_ttbb_ttlf_8TeV_BDT[7]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  //reader_ttbb_ttlf_8TeV_BDT[7]->AddVariable( "best_higgs_mass", &best_higgs_mass );

  // 6j4t
  reader_ttbb_ttlf_8TeV_BDT[8]->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets );
  reader_ttbb_ttlf_8TeV_BDT[8]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_ttbb_ttlf_8TeV_BDT[8]->AddVariable( "closest_tagged_dijet_mass", &closest_tagged_dijet_mass );
  reader_ttbb_ttlf_8TeV_BDT[8]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_ttbb_ttlf_8TeV_BDT[8]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  //reader_ttbb_ttlf_8TeV_BDT[8]->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag );
  reader_ttbb_ttlf_8TeV_BDT[8]->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag );
  //reader_ttbb_ttlf_8TeV_BDT[8]->AddVariable( "median_bb_mass", &median_bb_mass );
  reader_ttbb_ttlf_8TeV_BDT[8]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  reader_ttbb_ttlf_8TeV_BDT[8]->AddVariable( "ttbb_tth_ANN_output", &ttbb_tth_ANN_output_best10 );
  reader_ttbb_ttlf_8TeV_BDT[8]->AddVariable( "best_higgs_mass", &best_higgs_mass );
  reader_ttbb_ttlf_8TeV_BDT[8]->AddVariable( "dEta_fn", &dEta_fn );

  /////////////



  /////////////

  // 4j2t
  reader_final10v16_8TeV_BDT[0]->AddVariable( "avg_untagged_dijet_mass", &avg_untagged_dijet_mass );
  reader_final10v16_8TeV_BDT[0]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_final10v16_8TeV_BDT[0]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_final10v16_8TeV_BDT[0]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_final10v16_8TeV_BDT[0]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_final10v16_8TeV_BDT[0]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_final10v16_8TeV_BDT[0]->AddVariable( "HT", &HT );
  reader_final10v16_8TeV_BDT[0]->AddVariable( "MHT", &MHT );
  reader_final10v16_8TeV_BDT[0]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_final10v16_8TeV_BDT[0]->AddVariable( "third_highest_btag", &third_highest_btag );

  // 5j2t
  reader_final10v16_8TeV_BDT[1]->AddVariable( "avg_btag_disc_non_btags", &avg_btag_disc_non_btags );
  reader_final10v16_8TeV_BDT[1]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_final10v16_8TeV_BDT[1]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_final10v16_8TeV_BDT[1]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_final10v16_8TeV_BDT[1]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_final10v16_8TeV_BDT[1]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_final10v16_8TeV_BDT[1]->AddVariable( "HT", &HT );
  reader_final10v16_8TeV_BDT[1]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_final10v16_8TeV_BDT[1]->AddVariable( "Mlb", &Mlb );
  reader_final10v16_8TeV_BDT[1]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );


  // 6j2t
  reader_final10v16_8TeV_BDT[2]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_final10v16_8TeV_BDT[2]->AddVariable( "HT", &HT );
  reader_final10v16_8TeV_BDT[2]->AddVariable( "sphericity", &sphericity );
  reader_final10v16_8TeV_BDT[2]->AddVariable( "dr_between_lep_and_closest_jet", &dr_between_lep_and_closest_jet );
  reader_final10v16_8TeV_BDT[2]->AddVariable( "h2", &h2 );
  reader_final10v16_8TeV_BDT[2]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_final10v16_8TeV_BDT[2]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_final10v16_8TeV_BDT[2]->AddVariable( "maxeta_jet_jet", &maxeta_jet_jet );
  reader_final10v16_8TeV_BDT[2]->AddVariable( "Mlb", &Mlb );
  reader_final10v16_8TeV_BDT[2]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );

  // 4j3t
  reader_final10v16_8TeV_BDT[3]->AddVariable( "MET", &MET );
  reader_final10v16_8TeV_BDT[3]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_final10v16_8TeV_BDT[3]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_final10v16_8TeV_BDT[3]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_final10v16_8TeV_BDT[3]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_final10v16_8TeV_BDT[3]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_final10v16_8TeV_BDT[3]->AddVariable( "HT", &HT );
  reader_final10v16_8TeV_BDT[3]->AddVariable( "MHT", &MHT );
  reader_final10v16_8TeV_BDT[3]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_final10v16_8TeV_BDT[3]->AddVariable( "M3", &M3 );

  // 5j3t
  reader_final10v16_8TeV_BDT[4]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_final10v16_8TeV_BDT[4]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_final10v16_8TeV_BDT[4]->AddVariable( "third_jet_pt", &third_jet_pt );
  reader_final10v16_8TeV_BDT[4]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_final10v16_8TeV_BDT[4]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_final10v16_8TeV_BDT[4]->AddVariable( "HT", &HT );
  reader_final10v16_8TeV_BDT[4]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_final10v16_8TeV_BDT[4]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_final10v16_8TeV_BDT[4]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_final10v16_8TeV_BDT[4]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );

  // 6j3t
  reader_final10v16_8TeV_BDT[5]->AddVariable( "h0", &h0 );
  reader_final10v16_8TeV_BDT[5]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_final10v16_8TeV_BDT[5]->AddVariable( "sphericity", &sphericity );
  reader_final10v16_8TeV_BDT[5]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_final10v16_8TeV_BDT[5]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_final10v16_8TeV_BDT[5]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_final10v16_8TeV_BDT[5]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_final10v16_8TeV_BDT[5]->AddVariable( "maxeta_jet_jet", &maxeta_jet_jet );
  reader_final10v16_8TeV_BDT[5]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  reader_final10v16_8TeV_BDT[5]->AddVariable( "ttbb_tth_MVA_output_6j3t", &ttbb_tth_MVA_output_6j3t );

  // 4j4t
  reader_final10v16_8TeV_BDT[6]->AddVariable( "first_jet_pt", &first_jet_pt );
  reader_final10v16_8TeV_BDT[6]->AddVariable( "second_jet_pt", &second_jet_pt );
  reader_final10v16_8TeV_BDT[6]->AddVariable( "fourth_jet_pt", &fourth_jet_pt );
  reader_final10v16_8TeV_BDT[6]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_final10v16_8TeV_BDT[6]->AddVariable( "HT", &HT );
  reader_final10v16_8TeV_BDT[6]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_final10v16_8TeV_BDT[6]->AddVariable( "second_highest_btag", &second_highest_btag );
  reader_final10v16_8TeV_BDT[6]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_final10v16_8TeV_BDT[6]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_final10v16_8TeV_BDT[6]->AddVariable( "M3", &M3 );

  // 5j4t
  reader_final10v16_8TeV_BDT[7]->AddVariable( "all_sum_pt_with_met", &all_sum_pt_with_met );
  reader_final10v16_8TeV_BDT[7]->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets );
  reader_final10v16_8TeV_BDT[7]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_final10v16_8TeV_BDT[7]->AddVariable( "dev_from_avg_disc_btags", &dev_from_avg_disc_btags );
  reader_final10v16_8TeV_BDT[7]->AddVariable( "lowest_btag", &lowest_btag );
  reader_final10v16_8TeV_BDT[7]->AddVariable( "second_highest_btag", &second_highest_btag );//new
  reader_final10v16_8TeV_BDT[7]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_final10v16_8TeV_BDT[7]->AddVariable( "maxeta_jet_tag", &maxeta_jet_tag );
  //reader_final10v16_8TeV_BDT[7]->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag );
  reader_final10v16_8TeV_BDT[7]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  reader_final10v16_8TeV_BDT[7]->AddVariable( "ttbb_tth_MVA_output_5j4t", &ttbb_tth_MVA_output_5j4t );

  // 6j4t
  reader_final10v16_8TeV_BDT[8]->AddVariable( "avg_dr_tagged_jets", &avg_dr_tagged_jets );
  reader_final10v16_8TeV_BDT[8]->AddVariable( "avg_btag_disc_btags", &avg_btag_disc_btags );
  reader_final10v16_8TeV_BDT[8]->AddVariable( "closest_tagged_dijet_mass", &closest_tagged_dijet_mass );
  reader_final10v16_8TeV_BDT[8]->AddVariable( "third_highest_btag", &third_highest_btag );
  reader_final10v16_8TeV_BDT[8]->AddVariable( "fourth_highest_btag", &fourth_highest_btag );
  reader_final10v16_8TeV_BDT[8]->AddVariable( "maxeta_tag_tag", &maxeta_tag_tag );
  reader_final10v16_8TeV_BDT[8]->AddVariable( "pt_all_jets_over_E_all_jets", &pt_all_jets_over_E_all_jets );
  reader_final10v16_8TeV_BDT[8]->AddVariable( "ttbb_tth_ANN_output", &ttbb_tth_ANN_output_best15 );
  reader_final10v16_8TeV_BDT[8]->AddVariable( "best_higgs_mass", &best_higgs_mass );
  reader_final10v16_8TeV_BDT[8]->AddVariable( "dEta_fn", &dEta_fn );
  */
  /////////////


  /////////////



  /*
  std::string weightdir_prefix = "/home/gsmith/charlie_projection/CMSSW_5_3_11/src/ttH-LeptonPlusJets/AnalysisCode/"; // pointing to weights area



  std::string prefix = "TMVAClassification";
  std::string dir_tth_ttbb_8TeV = weightdir_prefix+"weights/TMVAweights_20130524_8TeV_train_tth_ttbb/";
  std::string dir_tth_ttbb_best15_8TeV = weightdir_prefix+"weights/TMVAweights_20130528_8TeV_train_tth_ttbb_best15/";
  std::string dir_tth_ttbb_best10_8TeV = weightdir_prefix+"weights/TMVAweights_20130524_8TeV_train_tth_ttbb_best10/";
  std::string dir_final10v16_8TeV_BDT = weightdir_prefix+"weights/TMVAweights_20130626_8TeV_train_with_TTH_TTBB_best15_BDT_tth_ttbb/";

  std::string dir_ttbb_ttH_8TeV = weightdir_prefix+"weights/TMVAweights_20130524_8TeV_train_ttbb_ttH/";
  std::string dir_ttbb_ttlf_8TeV = weightdir_prefix+"weights/TMVAweights_20130524_8TeV_train_ttbb_ttlf/";

  std::string dir_tth_ttbb_5j4t_6j3t_8TeV = weightdir_prefix+"weights/TMVAweights_20130624_8TeV_train_tth_ttbb_6j3t_5j4t/";



  for( int c=0; c<NumCat-3; c++ ){
    reader_final10v16_8TeV_BDT[c]->BookMVA("BDT method", dir_final10v16_8TeV_BDT + "weights/" + weight_dirv3[c] + prefix + "_BDT.weights.xml" );
    reader_ttbb_ttH_8TeV_CFMLP[c]->BookMVA("CFMlpANN method", dir_ttbb_ttH_8TeV + "weights/" + weight_dirv3[c] + prefix + "_CFMlpANN.weights.xml" );
    reader_ttbb_ttH_8TeV_BDT[c]->BookMVA("BDT method", dir_ttbb_ttH_8TeV + "weights/" + weight_dirv3[c] + prefix + "_BDT.weights.xml" );
    reader_ttbb_ttlf_8TeV_CFMLP[c]->BookMVA("CFMlpANN method", dir_ttbb_ttlf_8TeV + "weights/" + weight_dirv3[c] + prefix + "_CFMlpANN.weights.xml" );
    reader_ttbb_ttlf_8TeV_BDT[c]->BookMVA("BDT method", dir_ttbb_ttlf_8TeV + "weights/" + weight_dirv3[c] + prefix + "_BDT.weights.xml" );
  }

  reader_ttH_ttbb_8TeV_CFMLP->BookMVA("CFMlpANN method", dir_tth_ttbb_8TeV + "weights/640202/" + prefix + "_CFMlpANN.weights.xml" );
  reader_ttH_ttbb_best15_8TeV_BDT->BookMVA("BDT method", dir_tth_ttbb_best15_8TeV + "weights/640203/" + prefix + "_BDT.weights.xml" );
  reader_ttH_ttbb_best10_8TeV_BDT->BookMVA("BDT method", dir_tth_ttbb_best10_8TeV + "weights/640203/" + prefix + "_BDT.weights.xml" );

  reader_ttH_ttbb_6j3t_8TeV_BDT->BookMVA("BDT method", dir_tth_ttbb_5j4t_6j3t_8TeV + "weights/630000/" + prefix + "_BDT.weights.xml" );
  reader_ttH_ttbb_5j4t_8TeV_BDT->BookMVA("BDT method", dir_tth_ttbb_5j4t_6j3t_8TeV + "weights/540000/" + prefix + "_BDT.weights.xml" );


  double min_reader_final10v16_8TeV_BDT[NumCat];
  double max_reader_final10v16_8TeV_BDT[NumCat];


  double min_reader_ttH_ttbb_8TeV_CFMLP=-1;
  double max_reader_ttH_ttbb_8TeV_CFMLP=-1;

  double min_reader_ttH_ttbb_best15_8TeV_BDT=-1;
  double max_reader_ttH_ttbb_best15_8TeV_BDT=-1;

  double min_reader_ttH_ttbb_6j3t_8TeV_BDT=-1;
  double max_reader_ttH_ttbb_6j3t_8TeV_BDT=-1;

  double min_reader_ttH_ttbb_5j4t_8TeV_BDT=-1;
  double max_reader_ttH_ttbb_5j4t_8TeV_BDT=-1;

  double min_reader_ttH_ttbb_best10_8TeV_BDT;
  double max_reader_ttH_ttbb_best10_8TeV_BDT;

  double min_reader_ttbb_ttH_8TeV_CFMLP[NumCat];
  double max_reader_ttbb_ttH_8TeV_CFMLP[NumCat];

  double min_reader_ttbb_ttH_8TeV_BDT[NumCat];
  double max_reader_ttbb_ttH_8TeV_BDT[NumCat];

  double min_reader_ttbb_ttlf_8TeV_CFMLP[NumCat];
  double max_reader_ttbb_ttlf_8TeV_CFMLP[NumCat];

  double min_reader_ttbb_ttlf_8TeV_BDT[NumCat];
  double max_reader_ttbb_ttlf_8TeV_BDT[NumCat];


  double min_reader_best_old_ttbb_ttH_8TeV_CFMLP[NumCat];
  double max_reader_best_old_ttbb_ttH_8TeV_CFMLP[NumCat];

  double min_reader_best_ttbb_ttH_8TeV_CFMLP[NumCat];
  double max_reader_best_ttbb_ttH_8TeV_CFMLP[NumCat];



  for( int c=0; c<NumCat-3; c++ ){

    TFile *f_reader_final10v16_8TeV_BDT = new TFile(std::string(dir_final10v16_8TeV_BDT + "TMVA_trained_" + weight_dirv3[c].substr(0,weight_dirv3[c].size()-1) + ".root").c_str());

    TH1F *rep_hist_reader_final10v16_8TeV_bdt = (TH1F*)f_reader_final10v16_8TeV_BDT->Get("Method_BDT/BDT/MVA_BDT_S")->Clone();

    min_reader_final10v16_8TeV_BDT[c] = rep_hist_reader_final10v16_8TeV_bdt->GetBinLowEdge(1);
    max_reader_final10v16_8TeV_BDT[c] = rep_hist_reader_final10v16_8TeV_bdt->GetXaxis()->GetBinUpEdge(rep_hist_reader_final10v16_8TeV_bdt->GetNbinsX());


    ///////



    TFile *f_reader_ttbb_ttH_8TeV = new TFile(std::string(dir_ttbb_ttH_8TeV + "TMVA_trained_" + weight_dirv3[c].substr(0,weight_dirv3[c].size()-1) + ".root").c_str());

    TH1F *rep_hist_reader_ttbb_ttH_8TeV_cfmlp = (TH1F*)f_reader_ttbb_ttH_8TeV->Get("Method_CFMlpANN/CFMlpANN/MVA_CFMlpANN_S")->Clone();
    TH1F *rep_hist_reader_ttbb_ttH_8TeV_bdt = (TH1F*)f_reader_ttbb_ttH_8TeV->Get("Method_BDT/BDT/MVA_BDT_S")->Clone();

    min_reader_ttbb_ttH_8TeV_CFMLP[c] = rep_hist_reader_ttbb_ttH_8TeV_cfmlp->GetBinLowEdge(1);
    max_reader_ttbb_ttH_8TeV_CFMLP[c] = rep_hist_reader_ttbb_ttH_8TeV_cfmlp->GetXaxis()->GetBinUpEdge(rep_hist_reader_ttbb_ttH_8TeV_cfmlp->GetNbinsX());

    min_reader_ttbb_ttH_8TeV_BDT[c] = rep_hist_reader_ttbb_ttH_8TeV_bdt->GetBinLowEdge(1);
    max_reader_ttbb_ttH_8TeV_BDT[c] = rep_hist_reader_ttbb_ttH_8TeV_bdt->GetXaxis()->GetBinUpEdge(rep_hist_reader_ttbb_ttH_8TeV_bdt->GetNbinsX());



    TFile *f_reader_ttbb_ttlf_8TeV = new TFile(std::string(dir_ttbb_ttlf_8TeV + "TMVA_trained_" + weight_dirv3[c].substr(0,weight_dirv3[c].size()-1) + ".root").c_str());

    TH1F *rep_hist_reader_ttbb_ttlf_8TeV_cfmlp = (TH1F*)f_reader_ttbb_ttlf_8TeV->Get("Method_CFMlpANN/CFMlpANN/MVA_CFMlpANN_S")->Clone();
    TH1F *rep_hist_reader_ttbb_ttlf_8TeV_bdt = (TH1F*)f_reader_ttbb_ttlf_8TeV->Get("Method_BDT/BDT/MVA_BDT_S")->Clone();

    min_reader_ttbb_ttlf_8TeV_CFMLP[c] = rep_hist_reader_ttbb_ttlf_8TeV_cfmlp->GetBinLowEdge(1);
    max_reader_ttbb_ttlf_8TeV_CFMLP[c] = rep_hist_reader_ttbb_ttlf_8TeV_cfmlp->GetXaxis()->GetBinUpEdge(rep_hist_reader_ttbb_ttlf_8TeV_cfmlp->GetNbinsX());

    min_reader_ttbb_ttlf_8TeV_BDT[c] = rep_hist_reader_ttbb_ttlf_8TeV_bdt->GetBinLowEdge(1);
    max_reader_ttbb_ttlf_8TeV_BDT[c] = rep_hist_reader_ttbb_ttlf_8TeV_bdt->GetXaxis()->GetBinUpEdge(rep_hist_reader_ttbb_ttlf_8TeV_bdt->GetNbinsX());


    delete f_reader_final10v16_8TeV_BDT;
    delete f_reader_ttbb_ttH_8TeV;
    delete f_reader_ttbb_ttlf_8TeV;
  }




  TFile *f_reader_ttH_ttbb_8TeV = new TFile(std::string(dir_tth_ttbb_8TeV + "TMVA_trained_640202.root").c_str());
  TH1F *rep_hist_reader_ttH_ttbb_8TeV_cfmlp = (TH1F*)f_reader_ttH_ttbb_8TeV->Get("Method_CFMlpANN/CFMlpANN/MVA_CFMlpANN_S")->Clone();

  min_reader_ttH_ttbb_8TeV_CFMLP = rep_hist_reader_ttH_ttbb_8TeV_cfmlp->GetBinLowEdge(1);
  max_reader_ttH_ttbb_8TeV_CFMLP = rep_hist_reader_ttH_ttbb_8TeV_cfmlp->GetXaxis()->GetBinUpEdge(rep_hist_reader_ttH_ttbb_8TeV_cfmlp->GetNbinsX());

  delete f_reader_ttH_ttbb_8TeV;


  TFile *f_reader_ttH_ttbb_best15_8TeV = new TFile(std::string(dir_tth_ttbb_best15_8TeV + "TMVA_trained_640203.root").c_str());
  TH1F *rep_hist_reader_ttH_ttbb_best15_8TeV_cfmlp = (TH1F*)f_reader_ttH_ttbb_best15_8TeV->Get("Method_BDT/BDT/MVA_BDT_S")->Clone();

  min_reader_ttH_ttbb_best15_8TeV_BDT = rep_hist_reader_ttH_ttbb_best15_8TeV_cfmlp->GetBinLowEdge(1);
  max_reader_ttH_ttbb_best15_8TeV_BDT = rep_hist_reader_ttH_ttbb_best15_8TeV_cfmlp->GetXaxis()->GetBinUpEdge(rep_hist_reader_ttH_ttbb_best15_8TeV_cfmlp->GetNbinsX());

  delete f_reader_ttH_ttbb_best15_8TeV;


  TFile *f_reader_ttH_ttbb_best10_8TeV = new TFile(std::string(dir_tth_ttbb_best10_8TeV + "TMVA_trained_640203.root").c_str());
  TH1F *rep_hist_reader_ttH_ttbb_best10_8TeV_cfmlp = (TH1F*)f_reader_ttH_ttbb_best10_8TeV->Get("Method_BDT/BDT/MVA_BDT_S")->Clone();

  min_reader_ttH_ttbb_best10_8TeV_BDT = rep_hist_reader_ttH_ttbb_best10_8TeV_cfmlp->GetBinLowEdge(1);
  max_reader_ttH_ttbb_best10_8TeV_BDT = rep_hist_reader_ttH_ttbb_best10_8TeV_cfmlp->GetXaxis()->GetBinUpEdge(rep_hist_reader_ttH_ttbb_best10_8TeV_cfmlp->GetNbinsX());

  delete f_reader_ttH_ttbb_best10_8TeV;



  //6j3t
  TFile *f_reader_ttH_ttbb_6j3t_8TeV = new TFile(std::string(dir_tth_ttbb_5j4t_6j3t_8TeV + "TMVA_trained_630000.root").c_str());
  TH1F *rep_hist_reader_ttH_ttbb_6j3t_8TeV_bdt = (TH1F*)f_reader_ttH_ttbb_6j3t_8TeV->Get("Method_BDT/BDT/MVA_BDT_S")->Clone();

  min_reader_ttH_ttbb_6j3t_8TeV_BDT = rep_hist_reader_ttH_ttbb_6j3t_8TeV_bdt->GetBinLowEdge(1);
  max_reader_ttH_ttbb_6j3t_8TeV_BDT = rep_hist_reader_ttH_ttbb_6j3t_8TeV_bdt->GetXaxis()->GetBinUpEdge(rep_hist_reader_ttH_ttbb_6j3t_8TeV_bdt->GetNbinsX());

  delete f_reader_ttH_ttbb_6j3t_8TeV;


  //5j4t
  TFile *f_reader_ttH_ttbb_5j4t_8TeV = new TFile(std::string(dir_tth_ttbb_5j4t_6j3t_8TeV + "TMVA_trained_540000.root").c_str());
  TH1F *rep_hist_reader_ttH_ttbb_5j4t_8TeV_bdt = (TH1F*)f_reader_ttH_ttbb_5j4t_8TeV->Get("Method_BDT/BDT/MVA_BDT_S")->Clone();

  min_reader_ttH_ttbb_5j4t_8TeV_BDT = rep_hist_reader_ttH_ttbb_5j4t_8TeV_bdt->GetBinLowEdge(1);
  max_reader_ttH_ttbb_5j4t_8TeV_BDT = rep_hist_reader_ttH_ttbb_5j4t_8TeV_bdt->GetXaxis()->GetBinUpEdge(rep_hist_reader_ttH_ttbb_5j4t_8TeV_bdt->GetNbinsX());

  delete f_reader_ttH_ttbb_5j4t_8TeV;



  for( int c=0; c<NumCat; c++ ){ 
    min_reader_best_old_ttbb_ttH_8TeV_CFMLP[c] = min_reader_ttbb_ttH_8TeV_CFMLP[c];
    max_reader_best_old_ttbb_ttH_8TeV_CFMLP[c] = max_reader_ttbb_ttH_8TeV_CFMLP[c];

    min_reader_best_ttbb_ttH_8TeV_CFMLP[c] = min_reader_ttbb_ttH_8TeV_CFMLP[c];
    max_reader_best_ttbb_ttH_8TeV_CFMLP[c] = max_reader_ttbb_ttH_8TeV_CFMLP[c];

    if( (cat_labels[c].find("6j4t")!=std::string::npos) ){
      min_reader_best_old_ttbb_ttH_8TeV_CFMLP[c] = min_reader_ttH_ttbb_8TeV_CFMLP;
      max_reader_best_old_ttbb_ttH_8TeV_CFMLP[c] = max_reader_ttH_ttbb_8TeV_CFMLP;

      min_reader_best_ttbb_ttH_8TeV_CFMLP[c] = min_reader_ttH_ttbb_best15_8TeV_BDT;
      max_reader_best_ttbb_ttH_8TeV_CFMLP[c] = max_reader_ttH_ttbb_best15_8TeV_BDT;
    }
    else if( (cat_labels[c].find("5j4t")!=std::string::npos) ){
      min_reader_best_ttbb_ttH_8TeV_CFMLP[c] = min_reader_ttH_ttbb_5j4t_8TeV_BDT;
      max_reader_best_ttbb_ttH_8TeV_CFMLP[c] = max_reader_ttH_ttbb_5j4t_8TeV_BDT;
    }
    else if( (cat_labels[c].find("6j3t")!=std::string::npos) ){
      min_reader_best_ttbb_ttH_8TeV_CFMLP[c] = min_reader_ttH_ttbb_6j3t_8TeV_BDT;
      max_reader_best_ttbb_ttH_8TeV_CFMLP[c] = max_reader_ttH_ttbb_6j3t_8TeV_BDT;
    }

  }
  */


  //////////////////////////////////////////////////////////////////////////
  ///  Tree branches/leaves
  //////////////////////////////////////////////////////////////////////////

  yggdrasilEventVars *eve=0;
  chain->SetBranchAddress("eve.", &eve );

  //////////////////////////////////////////////////////////////////////////
  ///  Histogram making
  //////////////////////////////////////////////////////////////////////////


  TFile histofile(histofilename.c_str(),"recreate");
  histofile.cd();


  double NpvMin = 0-0.5;
  double NpvMax = 50+0.5;
  int NpvBins = int( NpvMax - NpvMin + 0.0001 );

  int NjetMin_full = 4;
  int NjetMax_full = 10;
  int NjetBins_full = NjetMax_full - NjetMin_full + 1;

  int NjetMin = 4;
  int NjetMax = 6;
  int NjetBins = NjetMax - NjetMin + 1;
  int NtagMin = 2;
  int NtagMax = 4;
  int NtagBins = NtagMax - NtagMin + 1;

  int NtagMin_full = 2;
  int NtagMax_full = 7;
  int NtagBins_full = NtagMax_full - NtagMin_full + 1;


  int NjetMin_loose = 0;
  int NjetMax_loose = 5;
  int NjetBins_loose = NjetMax_loose - NjetMin_loose + 1;

  int NtagMin_loose = 0;
  int NtagMax_loose = 2;
  int NtagBins_loose = NtagMax_loose - NtagMin_loose + 1;

  double metmax   = 500.;
  double lepPtMax = 300.;
  double jetptmax = 500.;
  double htmax    = 2000.;
  double massmax  = 500.;
  double m3max    = 1000.;
  double massmax_sumTop = 2000.;

  int NmetBins   = int( metmax/20. + 0.0001 );
  int NlepPtBins = int( lepPtMax/10. + 0.0001 );
  int NjetptBins = int( jetptmax/10. + 0.0001 );
  int NhtBins    = int( htmax/50. + 0.0001 );
  int NmassBins  = int( massmax/10. + 0.0001 );
  int Nm3Bins    = int( m3max/10. + 0.0001 );
  int NmassBins_sumTop  = int( massmax/10. + 0.0001 );


  // double max_jet_leadCandDistFromPV = 4.;
  // double max_jet_minPVDist = 0.1;

  // int Nbins_CFMlpANN = 20;

  int NcsvBins = 44;


  std::vector<std::string> sys_cat_labels;
  sys_cat_labels.push_back("");               //0
  sys_cat_labels.push_back("_lepIdSFUp");     //1
  sys_cat_labels.push_back("_lepIdSFDown");   //2
  sys_cat_labels.push_back("_PUUp");          //3
  sys_cat_labels.push_back("_PUDown");        //4
  sys_cat_labels.push_back("_JERUp");         //5
  sys_cat_labels.push_back("_JERDown");       //6
  sys_cat_labels.push_back("_JESUp");         //7
  sys_cat_labels.push_back("_JESDown");       //8
  sys_cat_labels.push_back("_CSVLFUp");       //9
  sys_cat_labels.push_back("_CSVLFDown");     //10
  sys_cat_labels.push_back("_CSVHFUp");       //11
  sys_cat_labels.push_back("_CSVHFDown");     //12
  sys_cat_labels.push_back("_CSVHFStats1Up");     //13
  sys_cat_labels.push_back("_CSVHFStats1Down");   //14
  sys_cat_labels.push_back("_CSVHFStats2Up");     //15
  sys_cat_labels.push_back("_CSVHFStats2Down");   //16
  sys_cat_labels.push_back("_CSVLFStats1Up");     //17
  sys_cat_labels.push_back("_CSVLFStats1Down");   //18
  sys_cat_labels.push_back("_CSVLFStats2Up");     //19
  sys_cat_labels.push_back("_CSVLFStats2Down");   //20
  // sys_cat_labels.push_back("_CSVCErr1Up");     //29
  // sys_cat_labels.push_back("_CSVCErr1Down");   //30
  // sys_cat_labels.push_back("_CSVCErr2Up");     //31
  // sys_cat_labels.push_back("_CSVCErr2Down");   //32
  sys_cat_labels.push_back("_Q2scaleUp");     //21
  sys_cat_labels.push_back("_Q2scaleDown");   //22
  sys_cat_labels.push_back("_topPtcorrUp");   //23
  sys_cat_labels.push_back("_topPtcorrDown"); //24


  int NumSysCat = int(sys_cat_labels.size());


  //////////////////////////////////////////////////////////////////////////
  ///  Histograms
  //////////////////////////////////////////////////////////////////////////



  TH1::SetDefaultSumw2();

  //
  // Event level information
  //
  TH1D* h_wgt_lepSF[NumSysCat];
  TH1D* h_wgt_PU[NumSysCat];
  TH1D* h_wgt_csvSF[NumSysCat];

  TH1D* h_category_yield[NumSysCat];
  TH1D* h_category_yield_noCSV[NumSysCat];
  TH1D* h_category_yield_newCSV[NumSysCat];
  TH1D* h_category_yield_looseNewCSV[NumSysCat];
  TH1D* h_category_yield_looseNewCSV_nomCSV[NumSysCat];
  TH1D* h_category_yield_looseNewCSV_newCSV[NumSysCat];

  TH1D* h_numPV_cat[NumCat][NumSysCat];
  TH1D* h_numPV[NumSysCat];
  TH1D* h_numJet[NumSysCat];
  TH1D* h_numTag[NumSysCat];
  TH2D* h_numJet_numTag[NumSysCat];

  TH1D* h_NumBmatch[NumCat][NumSysCat];

  TH1D* h_numLooseJet[NumSysCat];
  TH1D* h_numLooseTag[NumSysCat];


  // ttH, Higgs specific histograms
  TH1D* h_higgsDecayType[NumCat][NumSysCat];
  TH2D* h_category_yield_higgsDecayType[NumSysCat];



  //
  // Lepton plots
  //
  TH1D* h_lepton_pt[NumCat][NumSysCat];
  TH1D* h_lepton_phi[NumCat][NumSysCat];
  TH1D* h_lepton_eta[NumCat][NumSysCat];
  TH1D* h_lepton_relIso[NumCat][NumSysCat];


  // 
  // Jet plots
  //

  // all jets combined
  TH1D* h_jet_pt[NumCat][NumSysCat];
  TH1D* h_jet_pt_noCSV[NumCat][NumSysCat];
  TH1D* h_jet_pt_newCSV[NumCat][NumSysCat];
  TH1D* h_jet_pt_looseNewCSV[NumCat][NumSysCat];
  TH1D* h_jet_pt_looseNewCSV_nomCSV[NumCat][NumSysCat];
  TH1D* h_jet_pt_looseNewCSV_newCSV[NumCat][NumSysCat];

  TH1D* h_jet_eta[NumCat][NumSysCat];
  TH1D* h_jet_eta_noCSV[NumCat][NumSysCat];
  TH1D* h_jet_eta_newCSV[NumCat][NumSysCat];
  TH1D* h_jet_eta_looseNewCSV[NumCat][NumSysCat];
  TH1D* h_jet_eta_looseNewCSV_nomCSV[NumCat][NumSysCat];
  TH1D* h_jet_eta_looseNewCSV_newCSV[NumCat][NumSysCat];

  TH1D* h_jet_csv[NumCat][NumSysCat];
  TH1D* h_jet_csv_noCSV[NumCat][NumSysCat];
  TH1D* h_jet_csv_newCSV[NumCat][NumSysCat];
  TH1D* h_jet_csv_looseNewCSV[NumCat][NumSysCat];
  TH1D* h_jet_csv_looseNewCSV_nomCSV[NumCat][NumSysCat];
  TH1D* h_jet_csv_looseNewCSV_newCSV[NumCat][NumSysCat];

  TH1D* h_jet_loose_pt[NumCat][NumSysCat];
  TH1D* h_jet_loose_pt_noCSV[NumCat][NumSysCat];
  TH1D* h_jet_loose_pt_newCSV[NumCat][NumSysCat];
  TH1D* h_jet_loose_pt_looseNewCSV[NumCat][NumSysCat];
  TH1D* h_jet_loose_pt_looseNewCSV_nomCSV[NumCat][NumSysCat];
  TH1D* h_jet_loose_pt_looseNewCSV_newCSV[NumCat][NumSysCat];

  TH1D* h_jet_loose_eta[NumCat][NumSysCat];
  TH1D* h_jet_loose_eta_noCSV[NumCat][NumSysCat];
  TH1D* h_jet_loose_eta_newCSV[NumCat][NumSysCat];
  TH1D* h_jet_loose_eta_looseNewCSV[NumCat][NumSysCat];
  TH1D* h_jet_loose_eta_looseNewCSV_nomCSV[NumCat][NumSysCat];
  TH1D* h_jet_loose_eta_looseNewCSV_newCSV[NumCat][NumSysCat];

  TH1D* h_jet_loose_csv[NumCat][NumSysCat];
  TH1D* h_jet_loose_csv_noCSV[NumCat][NumSysCat];
  TH1D* h_jet_loose_csv_newCSV[NumCat][NumSysCat];
  TH1D* h_jet_loose_csv_looseNewCSV[NumCat][NumSysCat];
  TH1D* h_jet_loose_csv_looseNewCSV_nomCSV[NumCat][NumSysCat];
  TH1D* h_jet_loose_csv_looseNewCSV_newCSV[NumCat][NumSysCat];


  TH1D* h_tag_jet_pt[NumCat][NumSysCat];
  TH1D* h_untag_jet_pt[NumCat][NumSysCat];

  TH1D* h_jet_pt_noTopPt[NumCat][NumSysCat];
  TH1D* h_jet_pt_noTopPt_noCSV[NumCat][NumSysCat];

  TH1D* h_jet_csv_noTopPt[NumCat][NumSysCat];
  TH1D* h_jet_csv_noTopPt_noCSV[NumCat][NumSysCat];


  TH1D* h_maxDEta_jet_aveJetEta[NumCat][NumSysCat];
  TH1D* h_maxDEta_tag_aveJetEta[NumCat][NumSysCat];
  TH1D* h_maxDEta_tag_aveTagEta[NumCat][NumSysCat];

  TH1D* h_jet_aveJetEta[NumCat][NumSysCat];
  TH1D* h_jet_aveTagEta[NumCat][NumSysCat];

  // single jet specific plots

  TH1D* h_jet_pt_1_noCSV[NumCat][NumSysCat];
  TH1D* h_jet_pt_1_noTopPt[NumCat][NumSysCat];
  TH1D* h_jet_pt_1_noTopPt_noCSV[NumCat][NumSysCat];

  TH1D* h_jet_pt_1[NumCat][NumSysCat];
  TH1D* h_jet_pt_2[NumCat][NumSysCat];
  TH1D* h_jet_pt_3[NumCat][NumSysCat];
  TH1D* h_jet_pt_4[NumCat][NumSysCat];

  TH1D* h_jet_eta_1[NumCat][NumSysCat];
  TH1D* h_jet_eta_2[NumCat][NumSysCat];
  TH1D* h_jet_eta_3[NumCat][NumSysCat];
  TH1D* h_jet_eta_4[NumCat][NumSysCat];

  TH1D* h_jet_tag_pt_1[NumCat][NumSysCat];
  TH1D* h_jet_tag_pt_2[NumCat][NumSysCat];
  TH1D* h_jet_tag_pt_3[NumCat][NumSysCat];
  TH1D* h_jet_tag_pt_4[NumCat][NumSysCat];

  TH1D* h_jet_untag_pt_1[NumCat][NumSysCat];
  TH1D* h_jet_untag_pt_2[NumCat][NumSysCat];
  TH1D* h_jet_untag_pt_3[NumCat][NumSysCat];
  TH1D* h_jet_untag_pt_4[NumCat][NumSysCat];

  TH1D* h_jet_csv_1[NumCat][NumSysCat];
  TH1D* h_jet_csv_2[NumCat][NumSysCat];
  TH1D* h_jet_csv_3[NumCat][NumSysCat];
  TH1D* h_jet_csv_4[NumCat][NumSysCat];





  //
  // Info from multiple objects
  //
  TH1D* h_HT[NumCat][NumSysCat];
  TH1D* h_all_sum_pt[NumCat][NumSysCat];
  TH1D* h_all_sum_pt_with_met[NumCat][NumSysCat];
  TH1D* h_invariant_mass_of_everything[NumCat][NumSysCat];

  TH1D* h_M3[NumCat][NumSysCat];
  TH1D* h_M3_1tag[NumCat][NumSysCat];
  TH1D* h_Mlb[NumCat][NumSysCat];

  TH1D* h_met_pt[NumCat][NumSysCat];
  TH1D* h_met_phi[NumCat][NumSysCat];
  TH1D* h_mht_pt[NumCat][NumSysCat];
  TH1D* h_mht_phi[NumCat][NumSysCat];

  TH1D* h_transverse_mass_met[NumCat][NumSysCat];
  TH2D* h_transverse_mass_met_vs_met[NumCat][NumSysCat];
  TH1D* h_transverse_mass_mht[NumCat][NumSysCat];
  TH2D* h_transverse_mass_mht_vs_mht[NumCat][NumSysCat];

  TH1D* h_min_dR_lep_jet[NumCat][NumSysCat];
  TH1D* h_min_dR_tag_tag[NumCat][NumSysCat];
  TH1D* h_ave_dR_tag_tag[NumCat][NumSysCat];

  TH1D* h_ave_mass_tag_tag[NumCat][NumSysCat];
  TH1D* h_ave_mass_untag_untag[NumCat][NumSysCat];


  TH1D* h_leptonicW_mass[NumCat][NumSysCat];
  TH1D* h_hadronicW_mass[NumCat][NumSysCat];
  TH1D* h_leptonicT_mass[NumCat][NumSysCat];
  TH1D* h_hadronicT_mass[NumCat][NumSysCat];

  TH1D* h_hadronicW_pT[NumCat][NumSysCat];
  TH2D* h_hadronicW_pT_mass[NumCat][NumSysCat];

  TH1D* h_leptonicT_pT[NumCat][NumSysCat];
  TH2D* h_leptonicT_pT_mass[NumCat][NumSysCat];
  TH1D* h_hadronicT_pT[NumCat][NumSysCat];
  TH2D* h_hadronicT_pT_mass[NumCat][NumSysCat];

  TH1D* h_lepT_hadT_DeltaR[NumCat][NumSysCat];
  TH1D* h_lepT_hadT_Angle[NumCat][NumSysCat];


  TH1D* h_sumTop_pT[NumCat][NumSysCat];
  TH1D* h_sumTop_mass[NumCat][NumSysCat];
  TH2D* h_sumTop_pT_mass[NumCat][NumSysCat];

  TH1D* h_minChi2_getTopSystem[NumCat][NumSysCat];

  TH2D* h_leptonicW_mass_minChi2_getTopSystem[NumCat][NumSysCat];
  TH2D* h_hadronicW_mass_minChi2_getTopSystem[NumCat][NumSysCat];
  TH2D* h_leptonicT_mass_minChi2_getTopSystem[NumCat][NumSysCat];
  TH2D* h_hadronicT_mass_minChi2_getTopSystem[NumCat][NumSysCat];




  TH1D* h_aplanarity[NumCat][NumSysCat];
  TH1D* h_sphericity[NumCat][NumSysCat];
  TH1D* h_avg_btag_disc_non_btags[NumCat][NumSysCat];
  TH1D* h_avg_btag_disc_btags[NumCat][NumSysCat];
  TH1D* h_dev_from_avg_disc_btags[NumCat][NumSysCat];
  TH1D* h_lowest_btag[NumCat][NumSysCat];
  TH1D* h_closest_tagged_dijet_mass[NumCat][NumSysCat];

  TH1D* h_h0[NumCat][NumSysCat];
  TH1D* h_h1[NumCat][NumSysCat];
  TH1D* h_h2[NumCat][NumSysCat];
  TH1D* h_h3[NumCat][NumSysCat];
  TH1D* h_h4[NumCat][NumSysCat];

  
  // 
  // Best Higgs Mass and related variables
  //
  TH1D* h_best_higgs_mass[NumCat][NumSysCat];
  TH1D* h_minChi2[NumCat][NumSysCat];
  TH1D* h_dRbb[NumCat][NumSysCat];

  TH1D* h_bhmv2[NumCat][NumSysCat];
  TH1D* h_maxeta_jet_jet[NumCat][NumSysCat];       
  TH1D* h_maxeta_jet_tag[NumCat][NumSysCat];       
  TH1D* h_maxeta_tag_tag[NumCat][NumSysCat];       
  TH1D* h_abs_dEta_leptop_bb[NumCat][NumSysCat];   
  TH1D* h_abs_dEta_hadtop_bb[NumCat][NumSysCat];   
  TH1D* h_dEta_fn[NumCat][NumSysCat];	      
  TH1D* h_abs_bb_eta[NumCat][NumSysCat];	      
  TH1D* h_angle_tops_bb[NumCat][NumSysCat];        
  TH1D* h_median_bb_mass[NumCat][NumSysCat];       
  TH1D* h_pt_all_jets_over_E_all_jets[NumCat][NumSysCat];

  TH1D* h_tagged_dijet_mass_closest_to_125[NumCat][NumSysCat];


  //
  // MVA discriminant plots
  //
  /*
  TH1D* h_disc_ttH_ttbb_8TeV_CFMlpANN[NumCat][NumSysCat];
  TH1D* h_disc_ttH_ttbb_best15_8TeV_CFMlpANN_BDT[NumCat][NumSysCat];
  TH1D* h_disc_ttH_ttbb_best10_8TeV_CFMlpANN_BDT[NumCat][NumSysCat];
  TH1D* h_disc_ttH_ttbb_5j4t_8TeV_CFMlpANN_BDT[NumCat][NumSysCat];
  TH1D* h_disc_ttH_ttbb_6j3t_8TeV_CFMlpANN_BDT[NumCat][NumSysCat];

  TH1D* h_disc_ttbb_ttH_8TeV_CFMlpANN[NumCat][NumSysCat];
  TH1D* h_disc_ttbb_ttH_8TeV_CFMlpANN_BDT[NumCat][NumSysCat];
  TH1D* h_disc_ttbb_ttlf_8TeV_CFMlpANN[NumCat][NumSysCat];
  TH1D* h_disc_ttbb_ttlf_8TeV_CFMlpANN_BDT[NumCat][NumSysCat];

  TH2D* h_disc_ttbb_ttH_ttbb_ttlf_8TeV_CFMlpANN[NumCat][NumSysCat];
  TH2D* h_disc_ttbb_ttH_ttbb_ttlf_8TeV_CFMlpANN_BDT[NumCat][NumSysCat];
  TH2D* h_disc_old_ttH_ttbb_ttbb_ttlf_8TeV_CFMlpANN[NumCat][NumSysCat];
  TH2D* h_disc_ttH_ttbb_ttbb_ttlf_8TeV_CFMlpANN[NumCat][NumSysCat];

  TH1D* h_disc_final10v16_8TeV_CFMlpANN_BDT[NumCat][NumSysCat];

  TH1D* h_ttbb_algo_result[NumCat][NumSysCat];
  TH1D* h_ttcc_algo_result[NumCat][NumSysCat];


  // for( int c=0; c<NumCat; c++ ){ 
  //   double width_final10v1_8TeV_CFMLP = max_reader_final10v1_8TeV_CFMLP[c] - min_reader_final10v1_8TeV_CFMLP[c];
  //   max_reader_final10v1_8TeV_CFMLP[c] += 0.1*width_final10v1_8TeV_CFMLP;
  //   min_reader_final10v1_8TeV_CFMLP[c] -= 0.1*width_final10v1_8TeV_CFMLP;
  // }
  */


  for( int b=0; b<NumSysCat; b++ ){

    std::string short_suffix = sys_cat_labels[b];

    h_wgt_lepSF[b] = new TH1D((std::string("h_wgt_lepSF" + short_suffix)).c_str(), ";lepton scale factor", 100, 0.5, 1.5 );
    h_wgt_PU[b]    = new TH1D((std::string("h_wgt_PU" + short_suffix)).c_str(), ";PU weight", 200, 0., 2.0 );
    h_wgt_csvSF[b]    = new TH1D((std::string("h_wgt_csvSF" + short_suffix)).c_str(), ";CSV SF weight", 200, 0., 2.0 );

    h_category_yield[b] = new TH1D((std::string("h_category_yield" + short_suffix)).c_str(), ";category", NumCat, 0, NumCat );
    h_category_yield_noCSV[b] = new TH1D((std::string("h_category_yield_noCSV" + short_suffix)).c_str(), ";category", NumCat, 0, NumCat );
    h_category_yield_newCSV[b] = new TH1D((std::string("h_category_yield_newCSV" + short_suffix)).c_str(), ";category", NumCat, 0, NumCat );
    h_category_yield_looseNewCSV[b] = new TH1D((std::string("h_category_yield_looseNewCSV" + short_suffix)).c_str(), ";category", NumCat, 0, NumCat );
    h_category_yield_looseNewCSV_nomCSV[b] = new TH1D((std::string("h_category_yield_looseNewCSV_nomCSV" + short_suffix)).c_str(), ";category", NumCat, 0, NumCat );
    h_category_yield_looseNewCSV_newCSV[b] = new TH1D((std::string("h_category_yield_looseNewCSV_newCSV" + short_suffix)).c_str(), ";category", NumCat, 0, NumCat );

    h_category_yield_higgsDecayType[b] = new TH2D((std::string("h_category_yield_higgsDecayType" + short_suffix)).c_str(), ";category;Higgs decay", NumCat, 0, NumCat, 5, 0, 5 );


    h_numPV[b]  = new TH1D((std::string("h_numPV" + short_suffix)).c_str(), ";Number of Vertices", NpvBins, NpvMin, NpvMax );
    h_numJet[b] = new TH1D((std::string("h_numJet" + short_suffix)).c_str(), ";Number of Jets", NjetBins_full, NjetMin_full-0.5, NjetMax_full+0.5 );
    h_numTag[b] = new TH1D((std::string("h_numTag" + short_suffix)).c_str(), ";Number of Tags", NtagBins_full, NtagMin_full-0.5, NtagMax_full+0.5 );
    h_numJet_numTag[b] = new TH2D((std::string("h_numJet_numTag" + short_suffix)).c_str(), ";Number of Jets;Number of Tags", NjetBins, NjetMin-0.5, NjetMax+0.5, NtagBins, NtagMin-0.5, NtagMax+0.5 );


    h_numLooseJet[b] = new TH1D((std::string("h_numLooseJet" + short_suffix)).c_str(), ";Number of Loose Jets", NjetBins_loose, NjetMin_loose-0.5, NjetMax_loose+0.5 );
    h_numLooseTag[b] = new TH1D((std::string("h_numLooseTag" + short_suffix)).c_str(), ";Number of Loose Tags", NtagBins_loose, NtagMin_loose-0.5, NtagMax_loose+0.5 );



    for( int c=0; c<NumCat; c++ ){ 
      std::string suffix = "_" + cat_labels[c];
      std::string cat_suffix = "_" + cat_labels[c];
      suffix += sys_cat_labels[b];


      h_NumBmatch[c][b] = new TH1D((std::string("h_NumBmatch" + suffix)).c_str(), ";Number of b matched jets", 8, 0-0.5, 7+0.5 );

      h_higgsDecayType[c][b] = new TH1D((std::string("h_higgsDecayType" + suffix)).c_str(), ";Higgs decay", 5, 0, 5 );

      h_numPV_cat[c][b]  = new TH1D((std::string("h_numPV_cat" + suffix)).c_str(), ";Number of Vertices", NpvBins, NpvMin, NpvMax );

      h_lepton_pt[c][b] = new TH1D((std::string("h_lepton_pt" + suffix)).c_str(),";lepton p_{T}", NlepPtBins, 0, lepPtMax );
      h_lepton_phi[c][b] = new TH1D((std::string("h_lepton_phi" + suffix)).c_str(),";lepton #phi", 34, -3.4, 3.4 );
      h_lepton_eta[c][b] = new TH1D((std::string("h_lepton_eta" + suffix)).c_str(),";lepton #eta", 25, -2.5, 2.5 );
      h_lepton_relIso[c][b] = new TH1D((std::string("h_lepton_relIso" + suffix)).c_str(),";lepton relative isolation", 60, 0., 0.13 );


      h_jet_eta_1[c][b] = new TH1D((std::string("h_jet_eta_1" + suffix)).c_str(),";jet 1 #eta", 25, -2.5, 2.5 );
      h_jet_eta_2[c][b] = new TH1D((std::string("h_jet_eta_2" + suffix)).c_str(),";jet 2 #eta", 25, -2.5, 2.5 );
      h_jet_eta_3[c][b] = new TH1D((std::string("h_jet_eta_3" + suffix)).c_str(),";jet 3 #eta", 25, -2.5, 2.5 );
      h_jet_eta_4[c][b] = new TH1D((std::string("h_jet_eta_4" + suffix)).c_str(),";jet 4 #eta", 25, -2.5, 2.5 );

      h_jet_pt[c][b] = new TH1D((std::string("h_jet_pt" + suffix)).c_str(),";jet p_{T}", NjetptBins, 0, jetptmax );
      h_jet_pt_noCSV[c][b] = new TH1D((std::string("h_jet_pt_noCSV" + suffix)).c_str(),";jet p_{T}", NjetptBins, 0, jetptmax );
      h_jet_pt_newCSV[c][b] = new TH1D((std::string("h_jet_pt_newCSV" + suffix)).c_str(),";jet p_{T}", NjetptBins, 0, jetptmax );
      h_jet_pt_looseNewCSV[c][b] = new TH1D((std::string("h_jet_pt_looseNewCSV" + suffix)).c_str(),";jet p_{T}", NjetptBins, 0, jetptmax );
      h_jet_pt_looseNewCSV_nomCSV[c][b] = new TH1D((std::string("h_jet_pt_looseNewCSV_nomCSV" + suffix)).c_str(),";jet p_{T}", NjetptBins, 0, jetptmax );
      h_jet_pt_looseNewCSV_newCSV[c][b] = new TH1D((std::string("h_jet_pt_looseNewCSV_newCSV" + suffix)).c_str(),";jet p_{T}", NjetptBins, 0, jetptmax );

      h_jet_eta[c][b] = new TH1D((std::string("h_jet_eta" + suffix)).c_str(),";jet #eta", 25, -2.5, 2.5 );
      h_jet_eta_noCSV[c][b] = new TH1D((std::string("h_jet_eta_noCSV" + suffix)).c_str(),";jet #eta", 25, -2.5, 2.5 );
      h_jet_eta_newCSV[c][b] = new TH1D((std::string("h_jet_eta_newCSV" + suffix)).c_str(),";jet #eta", 25, -2.5, 2.5 );
      h_jet_eta_looseNewCSV[c][b] = new TH1D((std::string("h_jet_eta_looseNewCSV" + suffix)).c_str(),";jet #eta", 25, -2.5, 2.5 );
      h_jet_eta_looseNewCSV_nomCSV[c][b] = new TH1D((std::string("h_jet_eta_looseNewCSV_nomCSV" + suffix)).c_str(),";jet #eta", 25, -2.5, 2.5 );
      h_jet_eta_looseNewCSV_newCSV[c][b] = new TH1D((std::string("h_jet_eta_looseNewCSV_newCSV" + suffix)).c_str(),";jet #eta", 25, -2.5, 2.5 );

      h_jet_csv[c][b] = new TH1D((std::string("h_jet_csv" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );
      h_jet_csv_noCSV[c][b] = new TH1D((std::string("h_jet_csv_noCSV" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );
      h_jet_csv_newCSV[c][b] = new TH1D((std::string("h_jet_csv_newCSV" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );
      h_jet_csv_looseNewCSV[c][b] = new TH1D((std::string("h_jet_csv_looseNewCSV" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );
      h_jet_csv_looseNewCSV_nomCSV[c][b] = new TH1D((std::string("h_jet_csv_looseNewCSV_nomCSV" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );
      h_jet_csv_looseNewCSV_newCSV[c][b] = new TH1D((std::string("h_jet_csv_looseNewCSV_newCSV" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );

      h_jet_loose_pt[c][b] = new TH1D((std::string("h_jet_loose_pt" + suffix)).c_str(),";jet p_{T}", 20, 20, 30 );
      h_jet_loose_pt_noCSV[c][b] = new TH1D((std::string("h_jet_loose_pt_noCSV" + suffix)).c_str(),";jet p_{T}", 20, 20, 30 );
      h_jet_loose_pt_newCSV[c][b] = new TH1D((std::string("h_jet_loose_pt_newCSV" + suffix)).c_str(),";jet p_{T}", 20, 20, 30 );
      h_jet_loose_pt_looseNewCSV[c][b] = new TH1D((std::string("h_jet_loose_pt_looseNewCSV" + suffix)).c_str(),";jet p_{T}", 20, 20, 30 );
      h_jet_loose_pt_looseNewCSV_nomCSV[c][b] = new TH1D((std::string("h_jet_loose_pt_looseNewCSV_nomCSV" + suffix)).c_str(),";jet p_{T}", 20, 20, 30 );
      h_jet_loose_pt_looseNewCSV_newCSV[c][b] = new TH1D((std::string("h_jet_loose_pt_looseNewCSV_newCSV" + suffix)).c_str(),";jet p_{T}", 20, 20, 30 );

      h_jet_loose_eta[c][b] = new TH1D((std::string("h_jet_loose_eta" + suffix)).c_str(),";jet #eta", 25, -2.5, 2.5 );
      h_jet_loose_eta_noCSV[c][b] = new TH1D((std::string("h_jet_loose_eta_noCSV" + suffix)).c_str(),";jet #eta", 25, -2.5, 2.5 );
      h_jet_loose_eta_newCSV[c][b] = new TH1D((std::string("h_jet_loose_eta_newCSV" + suffix)).c_str(),";jet #eta", 25, -2.5, 2.5 );
      h_jet_loose_eta_looseNewCSV[c][b] = new TH1D((std::string("h_jet_loose_eta_looseNewCSV" + suffix)).c_str(),";jet #eta", 25, -2.5, 2.5 );
      h_jet_loose_eta_looseNewCSV_nomCSV[c][b] = new TH1D((std::string("h_jet_loose_eta_looseNewCSV_nomCSV" + suffix)).c_str(),";jet #eta", 25, -2.5, 2.5 );
      h_jet_loose_eta_looseNewCSV_newCSV[c][b] = new TH1D((std::string("h_jet_loose_eta_looseNewCSV_newCSV" + suffix)).c_str(),";jet #eta", 25, -2.5, 2.5 );

      h_jet_loose_csv[c][b] = new TH1D((std::string("h_jet_loose_csv" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );
      h_jet_loose_csv_noCSV[c][b] = new TH1D((std::string("h_jet_loose_csv_noCSV" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );
      h_jet_loose_csv_newCSV[c][b] = new TH1D((std::string("h_jet_loose_csv_newCSV" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );
      h_jet_loose_csv_looseNewCSV[c][b] = new TH1D((std::string("h_jet_loose_csv_looseNewCSV" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );
      h_jet_loose_csv_looseNewCSV_nomCSV[c][b] = new TH1D((std::string("h_jet_loose_csv_looseNewCSV_nomCSV" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );
      h_jet_loose_csv_looseNewCSV_newCSV[c][b] = new TH1D((std::string("h_jet_loose_csv_looseNewCSV_newCSV" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );


      h_tag_jet_pt[c][b] = new TH1D((std::string("h_tag_jet_pt" + suffix)).c_str(),";tagged jet p_{T}", NjetptBins, 0, jetptmax );
      h_untag_jet_pt[c][b] = new TH1D((std::string("h_untag_jet_pt" + suffix)).c_str(),";untagged jet p_{T}", NjetptBins, 0, jetptmax );


      h_jet_csv_noTopPt[c][b] = new TH1D((std::string("h_jet_csv_noTopPt" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );
      h_jet_csv_noTopPt_noCSV[c][b] = new TH1D((std::string("h_jet_csv_noTopPt_noCSV" + suffix)).c_str(),";jet CSV", NcsvBins, -1.1, 1.1 );

      h_jet_pt_noTopPt[c][b] = new TH1D((std::string("h_jet_pt_noTopPt" + suffix)).c_str(),";jet p_{T}", NjetptBins, 0, jetptmax );
      h_jet_pt_noTopPt_noCSV[c][b] = new TH1D((std::string("h_jet_pt_noTopPt_noCSV" + suffix)).c_str(),";jet p_{T}", NjetptBins, 0, jetptmax );

      h_jet_pt_1_noCSV[c][b] = new TH1D((std::string("h_jet_pt_1_noCSV" + suffix)).c_str(),";jet 1 p_{T}", NjetptBins, 0, jetptmax );
      h_jet_pt_1_noTopPt[c][b] = new TH1D((std::string("h_jet_pt_1_noTopPt" + suffix)).c_str(),";jet 1 p_{T}", NjetptBins, 0, jetptmax );
      h_jet_pt_1_noTopPt_noCSV[c][b] = new TH1D((std::string("h_jet_pt_1_noTopPt_noCSV" + suffix)).c_str(),";jet 1 p_{T}", NjetptBins, 0, jetptmax );


      h_jet_pt_1[c][b] = new TH1D((std::string("h_jet_pt_1" + suffix)).c_str(),";jet 1 p_{T}", NjetptBins, 0, jetptmax );
      h_jet_pt_2[c][b] = new TH1D((std::string("h_jet_pt_2" + suffix)).c_str(),";jet 2 p_{T}", NjetptBins, 0, jetptmax );
      h_jet_pt_3[c][b] = new TH1D((std::string("h_jet_pt_3" + suffix)).c_str(),";jet 3 p_{T}", NjetptBins, 0, jetptmax );
      h_jet_pt_4[c][b] = new TH1D((std::string("h_jet_pt_4" + suffix)).c_str(),";jet 4 p_{T}", NjetptBins, 0, jetptmax );


      h_jet_tag_pt_1[c][b] = new TH1D((std::string("h_jet_tag_pt_1" + suffix)).c_str(),";tag jet 1 p_{T}", NjetptBins, 0, jetptmax );
      h_jet_tag_pt_2[c][b] = new TH1D((std::string("h_jet_tag_pt_2" + suffix)).c_str(),";tag jet 2 p_{T}", NjetptBins, 0, jetptmax );
      h_jet_tag_pt_3[c][b] = new TH1D((std::string("h_jet_tag_pt_3" + suffix)).c_str(),";tag jet 3 p_{T}", NjetptBins, 0, jetptmax );
      h_jet_tag_pt_4[c][b] = new TH1D((std::string("h_jet_tag_pt_4" + suffix)).c_str(),";tag jet 4 p_{T}", NjetptBins, 0, jetptmax );


      h_jet_untag_pt_1[c][b] = new TH1D((std::string("h_jet_untag_pt_1" + suffix)).c_str(),";untag jet 1 p_{T}", NjetptBins, 0, jetptmax );
      h_jet_untag_pt_2[c][b] = new TH1D((std::string("h_jet_untag_pt_2" + suffix)).c_str(),";untag jet 2 p_{T}", NjetptBins, 0, jetptmax );
      h_jet_untag_pt_3[c][b] = new TH1D((std::string("h_jet_untag_pt_3" + suffix)).c_str(),";untag jet 3 p_{T}", NjetptBins, 0, jetptmax );
      h_jet_untag_pt_4[c][b] = new TH1D((std::string("h_jet_untag_pt_4" + suffix)).c_str(),";untag jet 4 p_{T}", NjetptBins, 0, jetptmax );




      h_jet_csv_1[c][b] = new TH1D((std::string("h_jet_csv_1" + suffix)).c_str(),";jet 1 CSV", NcsvBins, -1.1, 1.1 );
      h_jet_csv_2[c][b] = new TH1D((std::string("h_jet_csv_2" + suffix)).c_str(),";jet 2 CSV", NcsvBins, -1.1, 1.1 );
      h_jet_csv_3[c][b] = new TH1D((std::string("h_jet_csv_3" + suffix)).c_str(),";jet 3 CSV", NcsvBins, -1.1, 1.1 );
      h_jet_csv_4[c][b] = new TH1D((std::string("h_jet_csv_4" + suffix)).c_str(),";jet 4 CSV", NcsvBins, -1.1, 1.1 );



      h_maxDEta_jet_aveJetEta[c][b] = new TH1D((std::string("h_maxDEta_jet_aveJetEta" + suffix)).c_str(),";max #Delta#eta(jet,ave jet #eta)", 50, 0., 6. );
      h_maxDEta_tag_aveJetEta[c][b] = new TH1D((std::string("h_maxDEta_tag_aveJetEta" + suffix)).c_str(),";max #Delta#eta(tag,ave jet #eta)", 50, 0., 6. );
      h_maxDEta_tag_aveTagEta[c][b] = new TH1D((std::string("h_maxDEta_tag_aveTagEta" + suffix)).c_str(),";max #Delta#eta(tag,ave tag #eta)", 50, 0., 6. );


      h_jet_aveJetEta[c][b] = new TH1D((std::string("h_jet_aveJetEta" + suffix)).c_str(),";ave jet #eta", 50, -2.5, 2.5 );
      h_jet_aveTagEta[c][b] = new TH1D((std::string("h_jet_aveTagEta" + suffix)).c_str(),";ave tag #eta", 50, -2.5, 2.5 );


      h_met_pt[c][b]  = new TH1D((std::string("h_met_pt" + suffix)).c_str(),";MET p_{T}", NmetBins, 0, metmax );
      h_met_phi[c][b] = new TH1D((std::string("h_met_phi" + suffix)).c_str(),";MET #phi", 34, -3.4, 3.4 );
      h_mht_pt[c][b]  = new TH1D((std::string("h_mht_pt" + suffix)).c_str(),";MHT p_{T}", NmetBins, 0, metmax );
      h_mht_phi[c][b] = new TH1D((std::string("h_mht_phi" + suffix)).c_str(),";MHT #phi", 34, -3.4, 3.4 );
      h_all_sum_pt[c][b] = new TH1D((std::string("h_all_sum_pt" + suffix)).c_str(),";sum p_{T} (jets,#mu,ele)", NhtBins, 0, htmax );
      h_all_sum_pt_with_met[c][b] = new TH1D((std::string("h_all_sum_pt_with_met" + suffix)).c_str(),";sum p_{T} (jets,lepton,MET)", NhtBins, 0, htmax );
      h_invariant_mass_of_everything[c][b] = new TH1D((std::string("h_invariant_mass_of_everything" + suffix)).c_str(),";M_{inv} (jets,lepton,MET)", NhtBins, 0, htmax );
      h_transverse_mass_met[c][b] = new TH1D((std::string("h_transverse_mass_met" + suffix)).c_str(),";M_{T} (lepton,MET)", NmetBins, 0, metmax );
      h_transverse_mass_met_vs_met[c][b] = new TH2D((std::string("h_transverse_mass_met_vs_met" + suffix)).c_str(),";M_{T} (lepton,MET);MET", NmetBins, 0, metmax, NmetBins, 0, metmax );
      h_transverse_mass_mht[c][b] = new TH1D((std::string("h_transverse_mass_mht" + suffix)).c_str(),";M_{T} (lepton,MHT)", NmetBins, 0, metmax );
      h_transverse_mass_mht_vs_mht[c][b] = new TH2D((std::string("h_transverse_mass_mht_vs_mht" + suffix)).c_str(),";M_{T} (lepton,MHT);MHT", NmetBins, 0, metmax, NmetBins, 0, metmax );


      h_HT[c][b] = new TH1D((std::string("h_HT" + suffix)).c_str(),";H_{T} (jets)", NhtBins, 0, htmax );

      h_min_dR_lep_jet[c][b] = new TH1D((std::string("h_min_dR_lep_jet" + suffix)).c_str(),";min #DeltaR(lep,jet)", 30, 0, 6. );
      h_min_dR_tag_tag[c][b] = new TH1D((std::string("h_min_dR_tag_tag" + suffix)).c_str(),";min #DeltaR(tag,tag)", 30, 0, 6. );
      h_ave_dR_tag_tag[c][b] = new TH1D((std::string("h_ave_dR_tag_tag" + suffix)).c_str(),";ave #DeltaR(tag,tag)", 30, 0, 6. );

      h_ave_mass_tag_tag[c][b] = new TH1D((std::string("h_ave_mass_tag_tag" + suffix)).c_str(),";ave mass(tag,tag)", NmassBins, 0, massmax );
      h_ave_mass_untag_untag[c][b] = new TH1D((std::string("h_ave_mass_untag_untag" + suffix)).c_str(),";ave mass(untag,untag)", NmassBins, 0, massmax );
      h_M3[c][b] = new TH1D((std::string("h_M3" + suffix)).c_str(),";M3", Nm3Bins, 0, m3max );
      h_M3_1tag[c][b] = new TH1D((std::string("h_M3_1tag" + suffix)).c_str(),";M3 (1 tag)", Nm3Bins, 0, m3max );
      h_Mlb[c][b] = new TH1D((std::string("h_Mlb" + suffix)).c_str(),";M(lep,tag)", NmassBins, 0, massmax );

      h_best_higgs_mass[c][b] = new TH1D((std::string("h_best_higgs_mass" + suffix)).c_str(),";best Higgs mass", NmassBins, 0, massmax );
      h_minChi2[c][b] = new TH1D((std::string("h_minChi2" + suffix)).c_str(),";minChi2", 500, 0, 500 );
      h_dRbb[c][b] = new TH1D((std::string("h_dRbb" + suffix)).c_str(),";best #DeltaR(b,b)", 30, 0, 6. );

      h_tagged_dijet_mass_closest_to_125[c][b] = new TH1D((std::string("h_tagged_dijet_mass_closest_to_125" + suffix)).c_str(),";tagged dijet mass closest to 125", NmassBins, 0, massmax );



      h_leptonicW_mass[c][b] = new TH1D((std::string("h_leptonicW_mass" + suffix)).c_str(),";lep W mass (minChi2)", NmassBins, 0, massmax );
      h_hadronicW_mass[c][b] = new TH1D((std::string("h_hadronicW_mass" + suffix)).c_str(),";had W mass (minChi2)", NmassBins, 0, massmax );
      h_leptonicT_mass[c][b] = new TH1D((std::string("h_leptonicT_mass" + suffix)).c_str(),";lep top mass (minChi2)", NmassBins, 0, massmax );
      h_hadronicT_mass[c][b] = new TH1D((std::string("h_hadronicT_mass" + suffix)).c_str(),";had top mass (minChi2)", NmassBins, 0, massmax );

      h_hadronicW_pT[c][b] = new TH1D((std::string("h_hadronicW_pT" + suffix)).c_str(),";had W mass (minChi2)", NjetptBins, 0, jetptmax );
      h_hadronicW_pT_mass[c][b] = new TH2D((std::string("h_hadronicW_pT_mass" + suffix)).c_str(),";had W mass (minChi2)", NjetptBins, 0, jetptmax, NmassBins, 0, massmax );

      h_leptonicT_pT[c][b] = new TH1D((std::string("h_leptonicT_pT" + suffix)).c_str(),";lep top p_{T} (minChi2)", NjetptBins, 0, jetptmax );
      h_leptonicT_pT_mass[c][b] = new TH2D((std::string("h_leptonicT_pT_mass" + suffix)).c_str(),";lep top p_{T} (minChi2);lep top mass (minChi2)", NjetptBins, 0, jetptmax, NmassBins, 0, massmax );
      h_hadronicT_pT[c][b] = new TH1D((std::string("h_hadronicT_pT" + suffix)).c_str(),";had top p_{T} (minChi2)", NjetptBins, 0, jetptmax );
      h_hadronicT_pT_mass[c][b] = new TH2D((std::string("h_hadronicT_pT_mass" + suffix)).c_str(),";had top p_{T} (minChi2);had top mass (minChi2)", NjetptBins, 0, jetptmax, NmassBins, 0, massmax );

      h_lepT_hadT_DeltaR[c][b] = new TH1D((std::string("h_lepT_hadT_DeltaR" + suffix)).c_str(),";DeltaR(lepT,hadT)", 35, 0, 7.0 );
      h_lepT_hadT_Angle[c][b]  = new TH1D((std::string("h_lepT_hadT_Angle" + suffix)).c_str(), ";Angle(lepT,hadT)",  30, 0, 3.5 );


      h_sumTop_pT[c][b] = new TH1D((std::string("h_sumTop_pT" + suffix)).c_str(),";had + lep top p_{T} (minChi2)", NjetptBins, 0, jetptmax );
      h_sumTop_mass[c][b] = new TH1D((std::string("h_sumTop_mass" + suffix)).c_str(),";had + lep top mass (minChi2)", NmassBins_sumTop, 0, massmax_sumTop );
      h_sumTop_pT_mass[c][b] = new TH2D((std::string("h_sumTop_pT_mass" + suffix)).c_str(),";had + lep top mass (minChi2)", NjetptBins, 0, jetptmax, NmassBins_sumTop, 0, massmax_sumTop );

      h_minChi2_getTopSystem[c][b] = new TH1D((std::string("h_minChi2_getTopSystem" + suffix)).c_str(),";minChi2 (getTopSystem)", 50, 0, 500 );


      h_leptonicW_mass_minChi2_getTopSystem[c][b] = new TH2D((std::string("h_leptonicW_mass_minChi2_getTopSystem" + suffix)).c_str(),";lep W mass (minChi2)", NmassBins, 0, massmax, 500, 0, 500 );
      h_hadronicW_mass_minChi2_getTopSystem[c][b] = new TH2D((std::string("h_hadronicW_mass_minChi2_getTopSystem" + suffix)).c_str(),";had W mass (minChi2)", NmassBins, 0, massmax, 500, 0, 500 );
      h_leptonicT_mass_minChi2_getTopSystem[c][b] = new TH2D((std::string("h_leptonicT_mass_minChi2_getTopSystem" + suffix)).c_str(),";lep top mass (minChi2)", NmassBins, 0, massmax, 500, 0, 500 );
      h_hadronicT_mass_minChi2_getTopSystem[c][b] = new TH2D((std::string("h_hadronicT_mass_minChi2_getTopSystem" + suffix)).c_str(),";had top mass (minChi2)", NmassBins, 0, massmax, 500, 0, 500 );



      h_aplanarity[c][b] = new TH1D((std::string("h_aplanarity" + suffix)).c_str(),";aplanarity", 25, 0, 0.5 );
      h_sphericity[c][b] = new TH1D((std::string("h_sphericity" + suffix)).c_str(),";sphericity", 25, 0, 1.0 );
      h_avg_btag_disc_non_btags[c][b] = new TH1D((std::string("h_avg_btag_disc_non_btags" + suffix)).c_str(),";ave btag disc non btags", 230, -10.5, 1.0 );
      h_avg_btag_disc_btags[c][b] = new TH1D((std::string("h_avg_btag_disc_btags" + suffix)).c_str(),";ave btag disc btags", 100, -0.01, 1.01 );
      h_dev_from_avg_disc_btags[c][b] = new TH1D((std::string("h_dev_from_avg_disc_btags" + suffix)).c_str(),";dev from avg disc btags", 25, -0.0001, 0.04 );
      h_lowest_btag[c][b] = new TH1D((std::string("h_lowest_btag" + suffix)).c_str(),";lowest btag", 20, 0.65, 1.001 );
      h_closest_tagged_dijet_mass[c][b] = new TH1D((std::string("h_closest_tagged_dijet_mass" + suffix)).c_str(),";mass of closest tagged jets", NmassBins, 0, massmax );

      h_h0[c][b] = new TH1D((std::string("h_h0" + suffix)).c_str(),";h0", 25, 0, 0.5 );
      h_h1[c][b] = new TH1D((std::string("h_h1" + suffix)).c_str(),";h1", 25, -0.3, 0.5 );
      h_h2[c][b] = new TH1D((std::string("h_h2" + suffix)).c_str(),";h2", 25, -0.2, 0.5 );
      h_h3[c][b] = new TH1D((std::string("h_h3" + suffix)).c_str(),";h3", 25, -0.2, 1.1 );
      h_h4[c][b] = new TH1D((std::string("h_h4" + suffix)).c_str(),";h4", 25, -0.2, 0.3 );


      h_bhmv2[c][b] = new TH1D((std::string("h_bhmv2" + suffix)).c_str(),";best Higgs mass (v2)",20, 0, massmax );
      h_maxeta_jet_jet[c][b] = new TH1D((std::string("h_maxeta_jet_jet" + suffix)).c_str(),";max #Delta#eta(jet,ave jet #eta)", 50, 0., 6. );
      h_maxeta_jet_tag[c][b] = new TH1D((std::string("h_maxeta_jet_tag" + suffix)).c_str(),";max #Delta#eta(tag,ave jet #eta)", 50, 0., 6. );
      h_maxeta_tag_tag[c][b] = new TH1D((std::string("h_maxeta_tag_tag" + suffix)).c_str(),";max #Delta#eta(tag,ave tag #eta)", 50, 0., 6. );
      h_abs_dEta_leptop_bb[c][b] = new TH1D((std::string("h_abs_dEta_leptop_bb" + suffix)).c_str(),";abs(#Delta#eta(leptonic top, bb))", 20, 0., 6. );
      h_abs_dEta_hadtop_bb[c][b] = new TH1D((std::string("h_abs_dEta_hadtop_bb" + suffix)).c_str(),";abs(#Delta#eta(hadronic top, bb))", 20, 0., 6. );
      h_dEta_fn[c][b] = new TH1D((std::string("h_dEta_fn" + suffix)).c_str(),";product(#Delta#eta(leptonic top, bb),#Delta#eta(hadronic top, bb))", 20, 0., 6.);
      h_abs_bb_eta[c][b] = new TH1D((std::string("h_abs_bb_eta" + suffix)).c_str(),";abs(bb#eta)", 20, 0., 6.);
      h_angle_tops_bb[c][b] = new TH1D((std::string("h_angle_tops_bb" + suffix)).c_str(),";3D angle (tops, bb)", 20, 0., 3.5);
      h_median_bb_mass[c][b] = new TH1D((std::string("h_median_bb_mass" + suffix)).c_str(),";median inv. mass, all btag pairs",20, 0, massmax );
      h_pt_all_jets_over_E_all_jets[c][b] = new TH1D((std::string("h_pt_all_jets_over_E_all_jets" + suffix)).c_str(),";(#Sigma jet p_{T})/(#Sigma jet E)",50,0.,1.);

      /*
      h_disc_ttH_ttbb_8TeV_CFMlpANN[c][b] = new TH1D((std::string("h_disc_ttH_ttbb_8TeV_CFMlpANN" + suffix)).c_str(),";CFMlpANN response", Nbins_CFMlpANN, min_reader_ttH_ttbb_8TeV_CFMLP, max_reader_ttH_ttbb_8TeV_CFMLP );
      h_disc_ttH_ttbb_best15_8TeV_CFMlpANN_BDT[c][b] = new TH1D((std::string("h_disc_ttH_ttbb_best15_8TeV_CFMlpANN_BDT" + suffix)).c_str(),";BDT response", Nbins_CFMlpANN, min_reader_ttH_ttbb_best15_8TeV_BDT, max_reader_ttH_ttbb_best15_8TeV_BDT );
      h_disc_ttH_ttbb_best10_8TeV_CFMlpANN_BDT[c][b] = new TH1D((std::string("h_disc_ttH_ttbb_best10_8TeV_CFMlpANN_BDT" + suffix)).c_str(),";BDT response", Nbins_CFMlpANN, min_reader_ttH_ttbb_best10_8TeV_BDT, max_reader_ttH_ttbb_best10_8TeV_BDT );

      h_disc_ttH_ttbb_6j3t_8TeV_CFMlpANN_BDT[c][b] = new TH1D((std::string("h_disc_ttH_ttbb_6j3t_8TeV_CFMlpANN_BDT" + suffix)).c_str(),";BDT response", Nbins_CFMlpANN, min_reader_ttH_ttbb_6j3t_8TeV_BDT, max_reader_ttH_ttbb_6j3t_8TeV_BDT );
      h_disc_ttH_ttbb_5j4t_8TeV_CFMlpANN_BDT[c][b] = new TH1D((std::string("h_disc_ttH_ttbb_5j4t_8TeV_CFMlpANN_BDT" + suffix)).c_str(),";BDT response", Nbins_CFMlpANN, min_reader_ttH_ttbb_5j4t_8TeV_BDT, max_reader_ttH_ttbb_5j4t_8TeV_BDT );


      h_disc_ttbb_ttH_8TeV_CFMlpANN[c][b] = new TH1D((std::string("h_disc_ttbb_ttH_8TeV_CFMlpANN" + suffix)).c_str(),";CFMlpANN response", Nbins_CFMlpANN, min_reader_ttbb_ttH_8TeV_CFMLP[c], max_reader_ttbb_ttH_8TeV_CFMLP[c] );
      h_disc_ttbb_ttH_8TeV_CFMlpANN_BDT[c][b] = new TH1D((std::string("h_disc_ttbb_ttH_8TeV_CFMlpANN_BDT" + suffix)).c_str(),";BDT response", Nbins_CFMlpANN, min_reader_ttbb_ttH_8TeV_BDT[c], max_reader_ttbb_ttH_8TeV_BDT[c] );

      h_disc_ttbb_ttlf_8TeV_CFMlpANN[c][b] = new TH1D((std::string("h_disc_ttbb_ttlf_8TeV_CFMlpANN" + suffix)).c_str(),";CFMlpANN response", Nbins_CFMlpANN, min_reader_ttbb_ttlf_8TeV_CFMLP[c], max_reader_ttbb_ttlf_8TeV_CFMLP[c] );
      h_disc_ttbb_ttlf_8TeV_CFMlpANN_BDT[c][b] = new TH1D((std::string("h_disc_ttbb_ttlf_8TeV_CFMlpANN_BDT" + suffix)).c_str(),";BDT response", Nbins_CFMlpANN, min_reader_ttbb_ttlf_8TeV_BDT[c], max_reader_ttbb_ttlf_8TeV_BDT[c] );


      h_disc_ttbb_ttH_ttbb_ttlf_8TeV_CFMlpANN[c][b] = new TH2D((std::string("h_disc_ttbb_ttH_ttbb_ttlf_8TeV_CFMlpANN" + suffix)).c_str(),";CFMlpANN response (ttbb/ttlf);CFMlpANN response (ttbb/ttH)", Nbins_CFMlpANN, min_reader_ttbb_ttlf_8TeV_CFMLP[c], max_reader_ttbb_ttlf_8TeV_CFMLP[c], Nbins_CFMlpANN, min_reader_ttbb_ttH_8TeV_CFMLP[c], max_reader_ttbb_ttH_8TeV_CFMLP[c] );
      h_disc_ttbb_ttH_ttbb_ttlf_8TeV_CFMlpANN_BDT[c][b] = new TH2D((std::string("h_disc_ttbb_ttH_ttbb_ttlf_8TeV_CFMlpANN_BDT" + suffix)).c_str(),";BDT response (ttbb/ttlf);BDT response (ttbb/ttH)", Nbins_CFMlpANN, min_reader_ttbb_ttlf_8TeV_BDT[c], max_reader_ttbb_ttlf_8TeV_BDT[c], Nbins_CFMlpANN, min_reader_ttbb_ttH_8TeV_BDT[c], max_reader_ttbb_ttH_8TeV_BDT[c] );


      h_disc_old_ttH_ttbb_ttbb_ttlf_8TeV_CFMlpANN[c][b] = new TH2D((std::string("h_disc_old_ttH_ttbb_ttbb_ttlf_8TeV_CFMlpANN" + suffix)).c_str(),";CFMlpANN response (ttbb/ttlf);CFMlpANN response (ttbb/ttH)", Nbins_CFMlpANN, min_reader_ttbb_ttlf_8TeV_CFMLP[c], max_reader_ttbb_ttlf_8TeV_CFMLP[c], Nbins_CFMlpANN, min_reader_best_old_ttbb_ttH_8TeV_CFMLP[c], max_reader_best_old_ttbb_ttH_8TeV_CFMLP[c] );

      h_disc_ttH_ttbb_ttbb_ttlf_8TeV_CFMlpANN[c][b] = new TH2D((std::string("h_disc_ttH_ttbb_ttbb_ttlf_8TeV_CFMlpANN" + suffix)).c_str(),";CFMlpANN response (ttbb/ttlf);CFMlpANN response (ttbb/ttH)", Nbins_CFMlpANN, min_reader_ttbb_ttlf_8TeV_CFMLP[c], max_reader_ttbb_ttlf_8TeV_CFMLP[c], Nbins_CFMlpANN, min_reader_best_ttbb_ttH_8TeV_CFMLP[c], max_reader_best_ttbb_ttH_8TeV_CFMLP[c] );

      h_disc_final10v16_8TeV_CFMlpANN_BDT[c][b] = new TH1D((std::string("h_disc_final10v16_8TeV_CFMlpANN_BDT" + suffix)).c_str(),";BDT response", Nbins_CFMlpANN, min_reader_final10v16_8TeV_BDT[c], max_reader_final10v16_8TeV_BDT[c] );


      h_ttbb_algo_result[c][b] = new TH1D((std::string("h_ttbb_algo_result" + suffix)).c_str(),";tt+bb algo result", 5, 0, 5 );
      h_ttcc_algo_result[c][b] = new TH1D((std::string("h_ttcc_algo_result" + suffix)).c_str(),";tt+cc algo result", 5, 0, 5 );
      */
    } // end loop over categories

  } // end loop over systematics


  for( int b=0; b<NumSysCat; b++ ){
    for( int c=0; c<NumCat; c++ ){
      h_higgsDecayType[c][b]->GetXaxis()->SetBinLabel(1,"bb");
      h_higgsDecayType[c][b]->GetXaxis()->SetBinLabel(2,"WW");
      h_higgsDecayType[c][b]->GetXaxis()->SetBinLabel(3,"ZZ");
      h_higgsDecayType[c][b]->GetXaxis()->SetBinLabel(4,"#tau#tau");
      h_higgsDecayType[c][b]->GetXaxis()->SetBinLabel(5,"Other");

      // h_ttbb_algo_result[c][b]->GetXaxis()->SetBinLabel(1,"no match");
      // h_ttbb_algo_result[c][b]->GetXaxis()->SetBinLabel(2,"1 b");
      // h_ttbb_algo_result[c][b]->GetXaxis()->SetBinLabel(3,"2 b matched to same jet");
      // h_ttbb_algo_result[c][b]->GetXaxis()->SetBinLabel(4,"2 b matched to diff jets");
      // h_ttbb_algo_result[c][b]->GetXaxis()->SetBinLabel(5,">2 b");

      // h_ttcc_algo_result[c][b]->GetXaxis()->SetBinLabel(1,"no match");
      // h_ttcc_algo_result[c][b]->GetXaxis()->SetBinLabel(2,"1 c");
      // h_ttcc_algo_result[c][b]->GetXaxis()->SetBinLabel(3,"2 c matched to same jet");
      // h_ttcc_algo_result[c][b]->GetXaxis()->SetBinLabel(4,"2 c matched to diff jets");
      // h_ttcc_algo_result[c][b]->GetXaxis()->SetBinLabel(5,">2 c");
    }
  }

  for( int b=0; b<NumSysCat; b++ ){
    for( int c=0; c<NumCat; c++ ){
      h_category_yield[b]->GetXaxis()->SetBinLabel(c+1,cat_labels[c].c_str());

      h_category_yield_noCSV[b]->GetXaxis()->SetBinLabel(c+1,cat_labels[c].c_str());
      h_category_yield_newCSV[b]->GetXaxis()->SetBinLabel(c+1,cat_labels[c].c_str());
      h_category_yield_looseNewCSV[b]->GetXaxis()->SetBinLabel(c+1,cat_labels[c].c_str());
      h_category_yield_looseNewCSV_nomCSV[b]->GetXaxis()->SetBinLabel(c+1,cat_labels[c].c_str());
      h_category_yield_looseNewCSV_newCSV[b]->GetXaxis()->SetBinLabel(c+1,cat_labels[c].c_str());

      h_category_yield_higgsDecayType[b]->GetXaxis()->SetBinLabel(c+1,cat_labels[c].c_str());
      h_category_yield_higgsDecayType[b]->GetYaxis()->SetBinLabel(1,"bb");
      h_category_yield_higgsDecayType[b]->GetYaxis()->SetBinLabel(2,"WW");
      h_category_yield_higgsDecayType[b]->GetYaxis()->SetBinLabel(3,"ZZ");
      h_category_yield_higgsDecayType[b]->GetYaxis()->SetBinLabel(4,"#tau#tau");
      h_category_yield_higgsDecayType[b]->GetYaxis()->SetBinLabel(5,"Other");
    }
  }


  //////////////////////////////////////////////////////////////////////////
  ///  Weights
  //////////////////////////////////////////////////////////////////////////
 

  int nentries = chain->GetEntries();
  std::cout << "\n\t Number of entries = " << nentries << std::endl;
  std::cout << "\t Max number of entries = " << maxNentries << std::endl;
  std::cout << "\n" << std::endl;

  int use_nentries = std::max( maxNentries, nentries);

  int NeventsPerJob = int( double(use_nentries)/double(Njobs) + 0.000001 ) + 1;

  int firstEvent = (jobN-1)*NeventsPerJob + 1;
  int lastEvent  = firstEvent + NeventsPerJob;
  if( jobN==Njobs ) lastEvent = -1;
  if( jobN==1 ) firstEvent = 0;

  std::cout << "========  Starting Event Loop  ========" << std::endl;
  for (Long64_t ievt=0; ievt<chain->GetEntries();ievt++) {    //Long64_t

    if( ievt<firstEvent ) continue;
    if( ievt==lastEvent ) break;

    if( ievt==1 )        std::cout << "     Event " << ievt << std::endl;
    if( ievt%10000==0 && ievt!=1 ) std::cout << "           " << ievt << "\t" 
					     << int(double(ievt-firstEvent)/double(NeventsPerJob)*100) << "% done" << std::endl;

    //if( ievt==(maxNentries+1) ) break;
    if( ievt==(maxNentries+1) && ievt!=0 ) break;

    chain->GetEntry(ievt);

    bool passEvent = false;
    if( lepSel==1 && eve->leptonType_==1 )       passEvent = ( eve->passMuonTrigger_==1 && eve->matchSingleMuTrigger_==1 );
    else if( lepSel==0 && eve->leptonType_==0 )  passEvent = ( eve->passElectronTrigger_==1 && eve->matchSingleElectronTrigger_==1 );
    else if( lepSel==2 )                         passEvent = ( ( eve->passMuonTrigger_==1 && eve->matchSingleMuTrigger_==1 ) || ( eve->passElectronTrigger_==1 && eve->matchSingleElectronTrigger_==1 ) );


    //if( !passEvent ) continue;

    // Filter by Higgs decay
    if( doFilterDecay && insample>=8000 && insample<=9145 ){
      if( eve->higgsDecayType_ != hdecay ) continue;
    }

    int numPVs = eve->numPVs_;

    int run = eve->run_;
    int lumi = eve->lumi_;
    long evt = eve->evt_;

    // // Drop problematic W+3 jets event for now
    // if( insample==2403 && lepSel!=0 && evt==90273686 ) continue;



    // iSys used for plot filling
    int iSys=-1;
    for(int treeSys=0; treeSys<rNumSys; treeSys++){
      // if( treeSys>=1 && treeSys<=10 ) continue;
      iSys++;

      numJets_float = eve->numJets_float_[treeSys];
      numTags_float = eve->numTags_float_[treeSys];

      // convert floats to ints for nJets, nTags
      int numJets = int( numJets_float + 0.001 );
      int numTags = int( numTags_float + 0.001 );

      int njet = ( numJets>NjetMax ) ? NjetMax : numJets;
      int ntag = ( numTags>NtagMax ) ? NtagMax : numTags;
      int njet_full = ( numJets>NjetMax_full ) ? NjetMax_full : numJets;
      int ntag_full = ( numTags>NtagMax_full ) ? NtagMax_full : numTags;

      int numJets_loose = int( eve->numJets_loose_float_[treeSys] + 0.001 );
      int numTags_loose = int( eve->numTags_loose_float_[treeSys] + 0.001 );

      int njet_loose = ( numJets_loose>NjetMax_loose ) ? NjetMax_loose : numJets_loose;
      int ntag_loose = ( numTags_loose>NtagMax_loose ) ? NtagMax_loose : numTags_loose;


      // 
      // Associate the variables with the MVA reader with the vars in the tree
      //
      tight_lepton_pt           = eve->tight_lepton_pt_;
      tight_lepton_phi          = eve->tight_lepton_phi_;
      first_jet_pt              = eve->first_jet_pt_[treeSys];
      min_dr_tagged_jets        = eve->min_dr_tagged_jets_[treeSys];
      avg_dr_tagged_jets        = eve->avg_dr_tagged_jets_[treeSys];
      aplanarity                = eve->aplanarity_[treeSys];
      sphericity                = eve->sphericity_[treeSys];
      MET                       = eve->MET_[treeSys];
      second_jet_pt             = eve->second_jet_pt_[treeSys];
      avg_btag_disc_btags       = eve->avg_btag_disc_btags_[treeSys]; 
      dev_from_avg_disc_btags   = eve->dev_from_avg_disc_btags_[treeSys];
      third_jet_pt              = eve->third_jet_pt_[treeSys];
      fourth_jet_pt             = eve->fourth_jet_pt_[treeSys];
      avg_tagged_dijet_mass     = eve->avg_tagged_dijet_mass_[treeSys];
      lowest_btag               = eve->lowest_btag_[treeSys];
      all_sum_pt_incl_met       = eve->all_sum_pt_with_met_[treeSys];
      avg_untagged_dijet_mass   = eve->avg_untagged_dijet_mass_[treeSys];
      closest_tagged_dijet_mass = eve->closest_tagged_dijet_mass_[treeSys];
      first_highest_btag        = eve->first_highest_btag_[treeSys];
      second_highest_btag       = eve->second_highest_btag_[treeSys];
      third_highest_btag        = eve->third_highest_btag_[treeSys];
      fourth_highest_btag       = eve->fourth_highest_btag_[treeSys];
      best_higgs_mass           = eve->best_higgs_mass_[treeSys];
      invariant_mass_of_everything   = eve->invariant_mass_of_everything_[treeSys];
      dr_between_lep_and_closest_jet = eve->dr_between_lep_and_closest_jet_[treeSys];

      tagged_dijet_mass_closest_to_125 = eve->tagged_dijet_mass_closest_to_125_[treeSys];

      MHT     = eve->MHT_[treeSys];
      MHT_phi = eve->MHT_phi_[treeSys];


      h0 = eve->h0_[treeSys];
      h1 = eve->h1_[treeSys];
      h2 = eve->h2_[treeSys];
      h3 = eve->h3_[treeSys];
      h4 = eve->h4_[treeSys];

      minChi2 = eve->minChi2_[treeSys];
      dRbb = eve->dRbb_[treeSys];
      avg_btag_disc_non_btags = eve->avg_btag_disc_non_btags_[treeSys];
      all_sum_pt = eve->all_sum_pt_[treeSys];

      all_sum_pt_with_met = all_sum_pt_incl_met;

      Mlb     = eve->Mlb_[treeSys];
      M3      = eve->M3_[treeSys];
      M3_1tag = eve->M3_1tag_[treeSys];

      double MET_phi = eve->MET_phi_[treeSys];
      double tight_lepton_eta = eve->tight_lepton_eta_;


      // Require >= 3 jets with pT > 40 GeV
      //if( third_jet_pt<40. ) continue; // DP change thresholds for now

      // Require event not be selected by tau analysis
      //if( eve->IsTauTauLeptonEvent_[treeSys]==1 ) continue;


      double wgt = eve->wgt_[treeSys];
      double wgt_nomCSV = eve->wgt_csvSF_[treeSys];
      double wgt_noCSV = ( eve->wgt_csvSF_[treeSys]>=0. ) ? eve->wgt_[treeSys]/eve->wgt_csvSF_[treeSys] : eve->wgt_[treeSys];
      double wgt_noTopPt = ( eve->wgt_topPt_>=0. ) ? eve->wgt_[treeSys]/eve->wgt_topPt_ : eve->wgt_[treeSys];

      if( (sys_cat_labels[iSys].find("_topPtcorrUp")!=std::string::npos) )        wgt_noTopPt *= eve->wgt_topPt_/eve->wgt_topPtUp_;
      else if( (sys_cat_labels[iSys].find("_topPtcorrDown")!=std::string::npos) ) wgt_noTopPt *= eve->wgt_topPt_/eve->wgt_topPtDown_;

      double wgt_noTopPt_noCSV = ( eve->wgt_csvSF_[iSys]>=0. ) ? wgt_noTopPt/eve->wgt_csvSF_[iSys] : wgt_noTopPt;



      int this_category = -1;
      if( numJets==4 && numTags==2) this_category=0;
      if( numJets==5 && numTags==2) this_category=1;
      if( numJets>=6 && numTags==2) this_category=2;	
      if( numJets==4 && numTags==3) this_category=3;
      if( numJets==5 && numTags==3) this_category=4;
      if( numJets>=6 && numTags==3) this_category=5;
      if( numJets==4 && numTags>=4) this_category=6;
      if( numJets==5 && numTags>=4) this_category=7;
      if( numJets>=6 && numTags>=4) this_category=8;
      if( numJets==4 && numTags==1) this_category=9;
      if( numJets==5 && numTags==1) this_category=10;
      if( numJets>=6 && numTags==1) this_category=11;	
      if( numJets>=0 && numTags>=0) this_category=12;	

      // int this_BDT_category = this_category;
      // if( this_BDT_category>=9 ) this_BDT_category -= 9;

      double MT_met = sqrt( 2 * tight_lepton_pt * MET * (1-cos(tight_lepton_phi - MET_phi)) );
      MT_mht = sqrt( 2 * tight_lepton_pt * MHT * (1-cos(tight_lepton_phi - MHT_phi)) );

      /////////////



      TLorentzVector newmet, b1, b2;
	
      double chi2lepW, chi2leptop, chi2hadW, chi2hadtop;
      double mass_lepW, mass_leptop, mass_hadW, mass_hadtop, drbb_dummy, minChi;
      double bbleptopeta, bbhadtopeta, dphifn, bbeta, avgetatops, detafn, anglbbtops;
      double bhm_v2=-1;			
      double etamax_jj;
      double etamax_jt;
      double etamax_tt;		

      if( this_category>-1 && this_category<9 ){   
	bhm_v2 = study_tops_bb_syst(MET, MET_phi, newmet, eve->lepton_TLV_[treeSys], eve->jet_vect_TLV_[treeSys], 
				    eve->jet_CSV_[treeSys], minChi, chi2lepW, chi2leptop, chi2hadW, chi2hadtop, mass_lepW, mass_leptop, mass_hadW, mass_hadtop, 
				    drbb_dummy, bbleptopeta, bbhadtopeta, dphifn, bbeta, avgetatops, detafn, anglbbtops, b1, b2);		

	etamax_jj = get_jet_jet_etamax (eve->jet_vect_TLV_[treeSys]);
	etamax_jt = get_jet_tag_etamax (eve->jet_vect_TLV_[treeSys],eve->jet_CSV_[treeSys]);
	etamax_tt = get_tag_tag_etamax (eve->jet_vect_TLV_[treeSys],eve->jet_CSV_[treeSys]);		

	bhmv2 = bhm_v2;
	maxeta_jet_jet = etamax_jj;       
	maxeta_jet_tag = etamax_jt;       
	maxeta_tag_tag = etamax_tt;       
	abs_dEta_leptop_bb = abs(bbleptopeta);   
	abs_dEta_hadtop_bb = abs(bbhadtopeta);   
	dEta_fn = detafn;	      
	abs_bb_eta = abs(bbeta);	      
	angle_tops_bb = anglbbtops;        
	median_bb_mass = get_median_bb_mass(eve->jet_vect_TLV_[treeSys],eve->jet_CSV_[treeSys]);       
	pt_all_jets_over_E_all_jets = pt_E_ratio_jets(eve->jet_vect_TLV_[treeSys]);

	/*
	ttbb_tth_ANN_output = reader_ttH_ttbb_8TeV_CFMLP->EvaluateMVA( "CFMlpANN method" );

	ttbb_tth_ANN_output_best15 = reader_ttH_ttbb_best15_8TeV_BDT->EvaluateMVA( "BDT method" );
	ttbb_tth_ANN_output_best10 = reader_ttH_ttbb_best10_8TeV_BDT->EvaluateMVA( "BDT method" );


	ttbb_tth_MVA_output_5j4t = reader_ttH_ttbb_5j4t_8TeV_BDT->EvaluateMVA( "BDT method" );
	ttbb_tth_MVA_output_6j3t = reader_ttH_ttbb_6j3t_8TeV_BDT->EvaluateMVA( "BDT method" );
	*/
      }



      vvdouble jet_vect_TLV = eve->jet_vect_TLV_[treeSys];
      vdouble jet_CSV = eve->jet_CSV_[treeSys];
      vint    jet_flavour = eve->jet_flavour_[treeSys];
      vint    jet_genId = eve->jet_genId_[treeSys];

      vvdouble jet_loose_vect_TLV = eve->jet_loose_vect_TLV_[treeSys];
      vdouble  jet_loose_CSV = eve->jet_loose_CSV_[treeSys];
      vint     jet_loose_flavour = eve->jet_loose_flavour_[treeSys];

      double newCSVwgt = 1;//( insample<0 ) ? 1 : get_csv_wgt(jet_vect_TLV,jet_CSV,jet_flavour,treeSys);
      double newCSVwgt_loose = 1;//( insample<0 ) ? 1 : get_csv_wgt(jet_loose_vect_TLV,jet_loose_CSV,jet_loose_flavour,treeSys);

      double wgt_newCSV = ( insample<0 ) ? 1 : wgt_noCSV*newCSVwgt;
      double wgt_looseNewCSV = ( insample<0 ) ? 1 : wgt_noCSV*newCSVwgt_loose;
      double wgt_looseNewCSV_nomCSV = ( insample<0 ) ? 1 : wgt_noCSV*newCSVwgt_loose*wgt_nomCSV;
      double wgt_looseNewCSV_newCSV = ( insample<0 ) ? 1 : wgt_noCSV*newCSVwgt_loose*newCSVwgt;


      h_wgt_lepSF[iSys]->Fill(eve->wgt_lepSF_);
      h_wgt_PU[iSys]->Fill(eve->wgt_pu_);
      h_wgt_csvSF[iSys]->Fill(eve->wgt_csvSF_[treeSys]);


      h_category_yield[iSys]->Fill(this_category,eve->wgt_[treeSys]);

      h_category_yield_noCSV[iSys]->Fill(this_category,wgt_noCSV);
      h_category_yield_newCSV[iSys]->Fill(this_category,wgt_newCSV);
      h_category_yield_looseNewCSV[iSys]->Fill(this_category,wgt_looseNewCSV);
      h_category_yield_looseNewCSV_nomCSV[iSys]->Fill(this_category,wgt_looseNewCSV_nomCSV);
      h_category_yield_looseNewCSV_newCSV[iSys]->Fill(this_category,wgt_looseNewCSV_newCSV);


      h_category_yield_higgsDecayType[iSys]->Fill(this_category,eve->higgsDecayType_,wgt);



      h_numLooseJet[iSys]->Fill(njet_loose,wgt);
      h_numLooseTag[iSys]->Fill(ntag_loose,wgt);

      if( numJets>=4 && numTags>=2 ){
	h_numPV[iSys]->Fill(numPVs,wgt);
	h_numTag[iSys]->Fill(ntag_full,wgt);
	h_numJet[iSys]->Fill(njet_full,wgt);
	h_numJet_numTag[iSys]->Fill(njet,ntag,wgt);
      }


      HT = eve->HT_[treeSys];


      /////////////////////////////////////////
      ////
      ////
      ////  Loop over categories
      ////
      ////
      /////////////////////////////////////////


      for( int category=0; category<NumCat; category++ ){
	if( this_category!=category ) continue;

	h_higgsDecayType[category][iSys]->Fill(eve->higgsDecayType_,wgt);

	h_met_pt[category][iSys]->Fill(std::min(double(MET),metmax)-0.00001,wgt);
	h_met_phi[category][iSys]->Fill(MET_phi,wgt);

	h_mht_pt[category][iSys]->Fill(std::min(double(MHT),metmax)-0.00001,wgt);
	h_mht_phi[category][iSys]->Fill(MHT_phi,wgt);

	h_transverse_mass_met[category][iSys]->Fill(std::min(double(MT_met),metmax)-0.00001,wgt);
	h_transverse_mass_met_vs_met[category][iSys]->Fill(std::min(double(MT_met),metmax)-0.00001,std::min(double(MET),metmax)-0.00001,wgt);

	h_transverse_mass_mht[category][iSys]->Fill(std::min(double(MT_mht),metmax)-0.00001,wgt);
	h_transverse_mass_mht_vs_mht[category][iSys]->Fill(std::min(double(MT_mht),metmax)-0.00001,std::min(double(MHT),metmax)-0.00001,wgt);

	h_numPV_cat[category][iSys]->Fill(numPVs,wgt);

	h_lepton_pt[category][iSys]->Fill(std::min(double(tight_lepton_pt),lepPtMax)-0.00001,wgt);
	h_lepton_eta[category][iSys]->Fill(tight_lepton_eta,wgt);
	h_lepton_phi[category][iSys]->Fill(tight_lepton_phi,wgt);
	h_lepton_relIso[category][iSys]->Fill(eve->tight_lepton_relIso_,wgt);

	// bool isMinNPV = ( numPVs>=0  && numPVs<=12 );
	// bool isMidNPV = ( numPVs>=13 && numPVs<=17 );


	h_min_dR_lep_jet[category][iSys]->Fill(dr_between_lep_and_closest_jet,wgt);
	h_all_sum_pt[category][iSys]->Fill(std::min(double(all_sum_pt),htmax)-0.00001,wgt);
	h_all_sum_pt_with_met[category][iSys]->Fill(std::min(double(all_sum_pt_with_met),htmax)-0.00001,wgt);
	h_invariant_mass_of_everything[category][iSys]->Fill(std::min(double(invariant_mass_of_everything+MET),htmax)-0.00001,wgt);

	h_HT[category][iSys]->Fill(std::min(double(HT),htmax)-0.00001,wgt);


	double sumJetEta = 0;
	double sumTagEta = 0;
	double cntJetEta = 0;
	double cntTagEta = 0;

	int myTags=0, myUnTags=0;
	int myBmatch=0;
	vecTLorentzVector jetsV;
	for( int iJet=0; iJet<int(jet_vect_TLV.size()); iJet++ ){
	  TLorentzVector myJet;
	  myJet.SetPxPyPzE( jet_vect_TLV[iJet][0], jet_vect_TLV[iJet][1], jet_vect_TLV[iJet][2], jet_vect_TLV[iJet][3] );

	  jetsV.push_back(myJet);
	  
	  double myCSV = jet_CSV[iJet];
	  double myJetPT = myJet.Pt();
	  int myFlavor = jet_flavour[iJet];

	  if( abs(myFlavor)==5 ) myBmatch++;

	  h_jet_eta[category][iSys]->Fill(myJet.Eta(),wgt);
	  h_jet_eta_noCSV[category][iSys]->Fill(myJet.Eta(),wgt_noCSV);
	  h_jet_eta_newCSV[category][iSys]->Fill(myJet.Eta(),wgt_newCSV);
	  h_jet_eta_looseNewCSV[category][iSys]->Fill(myJet.Eta(),wgt_looseNewCSV);
	  h_jet_eta_looseNewCSV_nomCSV[category][iSys]->Fill(myJet.Eta(),wgt_looseNewCSV_nomCSV);
	  h_jet_eta_looseNewCSV_newCSV[category][iSys]->Fill(myJet.Eta(),wgt_looseNewCSV_newCSV);

	  h_jet_csv[category][iSys]->Fill(myCSV,wgt);
	  h_jet_csv_noCSV[category][iSys]->Fill(myCSV,wgt_noCSV);
	  h_jet_csv_newCSV[category][iSys]->Fill(myCSV,wgt_newCSV);
	  h_jet_csv_looseNewCSV[category][iSys]->Fill(myCSV,wgt_looseNewCSV);
	  h_jet_csv_looseNewCSV_nomCSV[category][iSys]->Fill(myCSV,wgt_looseNewCSV_nomCSV);
	  h_jet_csv_looseNewCSV_newCSV[category][iSys]->Fill(myCSV,wgt_looseNewCSV_newCSV);

	  h_jet_csv_noTopPt[category][iSys]->Fill(myCSV,wgt_noTopPt);
	  h_jet_csv_noTopPt_noCSV[category][iSys]->Fill(myCSV,wgt_noTopPt_noCSV);


	  h_jet_pt[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	  h_jet_pt_noCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_noCSV);
	  h_jet_pt_newCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_newCSV);
	  h_jet_pt_looseNewCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_looseNewCSV);
	  h_jet_pt_looseNewCSV_nomCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_looseNewCSV_nomCSV);
	  h_jet_pt_looseNewCSV_newCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_looseNewCSV_newCSV);


	  h_jet_pt_noTopPt[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_noTopPt);
	  h_jet_pt_noTopPt_noCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_noTopPt_noCSV);

	  if( myCSV>0.679 ) h_tag_jet_pt[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	  else              h_untag_jet_pt[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);


	  sumJetEta += myJet.Eta();
	  cntJetEta += 1.;

	  if( myCSV>0.679 ){
	    sumTagEta += myJet.Eta();
	    cntTagEta += 1.;
	  }

	  if( iJet==0 ){
	    h_jet_eta_1[category][iSys]->Fill(myJet.Eta(),wgt);
	    h_jet_pt_1[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);

	    h_jet_pt_1_noCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_noCSV);
	    h_jet_pt_1_noTopPt[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_noTopPt);
	    h_jet_pt_1_noTopPt_noCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_noTopPt_noCSV);
	  }
	  else if( iJet==1 ){
	    h_jet_eta_2[category][iSys]->Fill(myJet.Eta(),wgt);
	    h_jet_pt_2[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	  }
	  else if( iJet==2 ){
	    h_jet_eta_3[category][iSys]->Fill(myJet.Eta(),wgt);
	    h_jet_pt_3[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	  }
	  else if( iJet==3 ){
	    h_jet_eta_4[category][iSys]->Fill(myJet.Eta(),wgt);
	    h_jet_pt_4[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	  }

	  if( myCSV>0.679 ){
	    if( myTags==0 )      h_jet_tag_pt_1[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	    else if( myTags==1 ) h_jet_tag_pt_2[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	    else if( myTags==2 ) h_jet_tag_pt_3[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	    else if( myTags==3 ) h_jet_tag_pt_4[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	    myTags++;
	  }
	  else {
	    if( myUnTags==0 )      h_jet_untag_pt_1[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	    else if( myUnTags==1 ) h_jet_untag_pt_2[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	    else if( myUnTags==2 ) h_jet_untag_pt_3[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	    else if( myUnTags==3 ) h_jet_untag_pt_4[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	    myUnTags++;
	  }          

	} // end loop over jets


	for( int iJet=0; iJet<int(jet_loose_vect_TLV.size()); iJet++ ){
	  TLorentzVector myJet;
	  myJet.SetPxPyPzE( jet_loose_vect_TLV[iJet][0], jet_loose_vect_TLV[iJet][1], jet_loose_vect_TLV[iJet][2], jet_loose_vect_TLV[iJet][3] );
	  
	  double myCSV = jet_loose_CSV[iJet];
	  double myJetPT = myJet.Pt();

	  h_jet_loose_eta[category][iSys]->Fill(myJet.Eta(),wgt);
	  h_jet_loose_eta_noCSV[category][iSys]->Fill(myJet.Eta(),wgt_noCSV);
	  h_jet_loose_eta_newCSV[category][iSys]->Fill(myJet.Eta(),wgt_newCSV);
	  h_jet_loose_eta_looseNewCSV[category][iSys]->Fill(myJet.Eta(),wgt_looseNewCSV);
	  h_jet_loose_eta_looseNewCSV_nomCSV[category][iSys]->Fill(myJet.Eta(),wgt_looseNewCSV_nomCSV);
	  h_jet_loose_eta_looseNewCSV_newCSV[category][iSys]->Fill(myJet.Eta(),wgt_looseNewCSV_newCSV);

	  h_jet_loose_csv[category][iSys]->Fill(myCSV,wgt);
	  h_jet_loose_csv_noCSV[category][iSys]->Fill(myCSV,wgt_noCSV);
	  h_jet_loose_csv_newCSV[category][iSys]->Fill(myCSV,wgt_newCSV);
	  h_jet_loose_csv_looseNewCSV[category][iSys]->Fill(myCSV,wgt_looseNewCSV);
	  h_jet_loose_csv_looseNewCSV_nomCSV[category][iSys]->Fill(myCSV,wgt_looseNewCSV_nomCSV);
	  h_jet_loose_csv_looseNewCSV_newCSV[category][iSys]->Fill(myCSV,wgt_looseNewCSV_newCSV);

	  h_jet_loose_pt[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt);
	  h_jet_loose_pt_noCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_noCSV);
	  h_jet_loose_pt_newCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_newCSV);
	  h_jet_loose_pt_looseNewCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_looseNewCSV);
	  h_jet_loose_pt_looseNewCSV_nomCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_looseNewCSV_nomCSV);
	  h_jet_loose_pt_looseNewCSV_newCSV[category][iSys]->Fill(std::min(myJetPT,jetptmax)-0.00001,wgt_looseNewCSV_newCSV);
	} //end loop over loose jets

	h_NumBmatch[category][iSys]->Fill(myBmatch,wgt);

	double aveJetEta = ( cntJetEta>0 ) ? sumJetEta/cntJetEta : -999;
	double aveTagEta = ( cntTagEta>0 ) ? sumTagEta/cntTagEta : -999;

	double maxDEta_jet_aveJetEta = -1;
	double maxDEta_tag_aveJetEta = -1;
	double maxDEta_tag_aveTagEta = -1;

	for( int iJet=0; iJet<int(jet_vect_TLV.size()); iJet++ ){
	  TLorentzVector myJet;
	  myJet.SetPxPyPzE( jet_vect_TLV[iJet][0], jet_vect_TLV[iJet][1], jet_vect_TLV[iJet][2], jet_vect_TLV[iJet][3] );

	  double myCSV = jet_CSV[iJet];
	  double myJetEta = myJet.Eta();

	  maxDEta_jet_aveJetEta = std::max( maxDEta_jet_aveJetEta, fabs(myJetEta - aveJetEta) );
	  if( myCSV>0.679 ){
	    maxDEta_tag_aveJetEta = std::max( maxDEta_tag_aveJetEta, fabs(myJetEta - aveJetEta) );
	    maxDEta_tag_aveTagEta = std::max( maxDEta_tag_aveTagEta, fabs(myJetEta - aveTagEta) );
	  }
	}

	h_maxDEta_jet_aveJetEta[category][iSys]->Fill(maxDEta_jet_aveJetEta,wgt);
	h_maxDEta_tag_aveJetEta[category][iSys]->Fill(maxDEta_tag_aveJetEta,wgt);
	h_maxDEta_tag_aveTagEta[category][iSys]->Fill(maxDEta_tag_aveTagEta,wgt);

	h_jet_aveJetEta[category][iSys]->Fill(aveJetEta,wgt);
	h_jet_aveTagEta[category][iSys]->Fill(aveTagEta,wgt);


	/*
	double CFMlpANN_ttbb_tth_8TeV = ttbb_tth_ANN_output;
	CFMlpANN_ttbb_tth_8TeV = std::min( CFMlpANN_ttbb_tth_8TeV, max_reader_ttH_ttbb_8TeV_CFMLP-0.0001 );
	CFMlpANN_ttbb_tth_8TeV = std::max( CFMlpANN_ttbb_tth_8TeV, min_reader_ttH_ttbb_8TeV_CFMLP+0.0001 );
	h_disc_ttH_ttbb_8TeV_CFMlpANN[category][iSys]->Fill(CFMlpANN_ttbb_tth_8TeV,wgt);

	double BDT_ttbb_tth_best15_8TeV = ttbb_tth_ANN_output_best15;
	BDT_ttbb_tth_best15_8TeV = std::min( BDT_ttbb_tth_best15_8TeV, max_reader_ttH_ttbb_best15_8TeV_BDT-0.0001 );
	BDT_ttbb_tth_best15_8TeV = std::max( BDT_ttbb_tth_best15_8TeV, min_reader_ttH_ttbb_best15_8TeV_BDT+0.0001 );
	h_disc_ttH_ttbb_best15_8TeV_CFMlpANN_BDT[category][iSys]->Fill(BDT_ttbb_tth_best15_8TeV,wgt);

	double BDT_ttbb_tth_best10_8TeV = ttbb_tth_ANN_output_best10;
	BDT_ttbb_tth_best10_8TeV = std::min( BDT_ttbb_tth_best10_8TeV, max_reader_ttH_ttbb_best10_8TeV_BDT-0.0001 );
	BDT_ttbb_tth_best10_8TeV = std::max( BDT_ttbb_tth_best10_8TeV, min_reader_ttH_ttbb_best10_8TeV_BDT+0.0001 );
	h_disc_ttH_ttbb_best10_8TeV_CFMlpANN_BDT[category][iSys]->Fill(BDT_ttbb_tth_best10_8TeV,wgt);


	double BDT_ttbb_tth_6j3t_8TeV = ttbb_tth_MVA_output_6j3t;
	BDT_ttbb_tth_6j3t_8TeV = std::min( BDT_ttbb_tth_6j3t_8TeV, max_reader_ttH_ttbb_6j3t_8TeV_BDT-0.0001 );
	BDT_ttbb_tth_6j3t_8TeV = std::max( BDT_ttbb_tth_6j3t_8TeV, min_reader_ttH_ttbb_6j3t_8TeV_BDT+0.0001 );
	h_disc_ttH_ttbb_6j3t_8TeV_CFMlpANN_BDT[category][iSys]->Fill(BDT_ttbb_tth_6j3t_8TeV,wgt);

	double BDT_ttbb_tth_5j4t_8TeV = ttbb_tth_MVA_output_5j4t;
	BDT_ttbb_tth_5j4t_8TeV = std::min( BDT_ttbb_tth_5j4t_8TeV, max_reader_ttH_ttbb_5j4t_8TeV_BDT-0.0001 );
	BDT_ttbb_tth_5j4t_8TeV = std::max( BDT_ttbb_tth_5j4t_8TeV, min_reader_ttH_ttbb_5j4t_8TeV_BDT+0.0001 );
	h_disc_ttH_ttbb_5j4t_8TeV_CFMlpANN_BDT[category][iSys]->Fill(BDT_ttbb_tth_5j4t_8TeV,wgt);



	double CFMlpANN_ttbb_ttH_8TeV = reader_ttbb_ttH_8TeV_CFMLP[this_BDT_category]->EvaluateMVA( "CFMlpANN method" );
	CFMlpANN_ttbb_ttH_8TeV = std::min( CFMlpANN_ttbb_ttH_8TeV, max_reader_ttbb_ttH_8TeV_CFMLP[category]-0.0001 );
	CFMlpANN_ttbb_ttH_8TeV = std::max( CFMlpANN_ttbb_ttH_8TeV, min_reader_ttbb_ttH_8TeV_CFMLP[category]+0.0001 );
	h_disc_ttbb_ttH_8TeV_CFMlpANN[category][iSys]->Fill(CFMlpANN_ttbb_ttH_8TeV,wgt);

	double BDT_ttbb_ttH_8TeV = reader_ttbb_ttH_8TeV_BDT[this_BDT_category]->EvaluateMVA( "BDT method" );
	BDT_ttbb_ttH_8TeV = std::min( BDT_ttbb_ttH_8TeV, max_reader_ttbb_ttH_8TeV_BDT[category]-0.0001 );
	BDT_ttbb_ttH_8TeV = std::max( BDT_ttbb_ttH_8TeV, min_reader_ttbb_ttH_8TeV_BDT[category]+0.0001 );
	h_disc_ttbb_ttH_8TeV_CFMlpANN_BDT[category][iSys]->Fill(BDT_ttbb_ttH_8TeV,wgt);

	double CFMlpANN_ttbb_ttlf_8TeV = reader_ttbb_ttlf_8TeV_CFMLP[this_BDT_category]->EvaluateMVA( "CFMlpANN method" );
	CFMlpANN_ttbb_ttlf_8TeV = std::min( CFMlpANN_ttbb_ttlf_8TeV, max_reader_ttbb_ttlf_8TeV_CFMLP[category]-0.0001 );
	CFMlpANN_ttbb_ttlf_8TeV = std::max( CFMlpANN_ttbb_ttlf_8TeV, min_reader_ttbb_ttlf_8TeV_CFMLP[category]+0.0001 );
	h_disc_ttbb_ttlf_8TeV_CFMlpANN[category][iSys]->Fill(CFMlpANN_ttbb_ttlf_8TeV,wgt);

	double BDT_ttbb_ttlf_8TeV = reader_ttbb_ttlf_8TeV_BDT[this_BDT_category]->EvaluateMVA( "BDT method" );
	BDT_ttbb_ttlf_8TeV = std::min( BDT_ttbb_ttlf_8TeV, max_reader_ttbb_ttlf_8TeV_BDT[category]-0.0001 );
	BDT_ttbb_ttlf_8TeV = std::max( BDT_ttbb_ttlf_8TeV, min_reader_ttbb_ttlf_8TeV_BDT[category]+0.0001 );
	h_disc_ttbb_ttlf_8TeV_CFMlpANN_BDT[category][iSys]->Fill(BDT_ttbb_ttlf_8TeV,wgt);

	h_disc_ttbb_ttH_ttbb_ttlf_8TeV_CFMlpANN_BDT[category][iSys]->Fill(BDT_ttbb_ttlf_8TeV,BDT_ttbb_ttH_8TeV,wgt);
	h_disc_ttbb_ttH_ttbb_ttlf_8TeV_CFMlpANN[category][iSys]->Fill(CFMlpANN_ttbb_ttlf_8TeV,CFMlpANN_ttbb_ttH_8TeV,wgt);

	double CFMlpANN_ttH_ttbb_8TeV = ttbb_tth_ANN_output;
	CFMlpANN_ttH_ttbb_8TeV = std::min( CFMlpANN_ttH_ttbb_8TeV, max_reader_ttbb_ttH_8TeV_CFMLP[category]-0.0001 );
	CFMlpANN_ttH_ttbb_8TeV = std::max( CFMlpANN_ttH_ttbb_8TeV, min_reader_ttbb_ttH_8TeV_CFMLP[category]+0.0001 );

	// here
	double use_ttbb_ttH_8TeV = CFMlpANN_ttbb_ttH_8TeV;
	if( this_category==8 )      use_ttbb_ttH_8TeV = ttbb_tth_ANN_output_best15;
	else if( this_category==5 ) use_ttbb_ttH_8TeV = ttbb_tth_MVA_output_6j3t;
	else if( this_category==7 ) use_ttbb_ttH_8TeV = ttbb_tth_MVA_output_5j4t;
	double MVA_ttbb_ttH_8TeV = use_ttbb_ttH_8TeV;
	MVA_ttbb_ttH_8TeV = std::min( MVA_ttbb_ttH_8TeV, max_reader_best_ttbb_ttH_8TeV_CFMLP[category]-0.0001 );
	MVA_ttbb_ttH_8TeV = std::max( MVA_ttbb_ttH_8TeV, min_reader_best_ttbb_ttH_8TeV_CFMLP[category]+0.0001 );
	h_disc_ttH_ttbb_ttbb_ttlf_8TeV_CFMlpANN[category][iSys]->Fill(CFMlpANN_ttbb_ttlf_8TeV,MVA_ttbb_ttH_8TeV,wgt);

	// here
	double use_old_ttbb_ttH_8TeV = CFMlpANN_ttbb_ttH_8TeV;
	if( this_category==8 ) use_old_ttbb_ttH_8TeV = CFMlpANN_ttbb_tth_8TeV;
	h_disc_old_ttH_ttbb_ttbb_ttlf_8TeV_CFMlpANN[category][iSys]->Fill(CFMlpANN_ttbb_ttlf_8TeV,use_old_ttbb_ttH_8TeV,wgt);


	/////
	double BDT_final10v16_8TeV = reader_final10v16_8TeV_BDT[this_BDT_category]->EvaluateMVA( "BDT method" );
	BDT_final10v16_8TeV = std::min( BDT_final10v16_8TeV, max_reader_final10v16_8TeV_BDT[category]-0.0001 );
	BDT_final10v16_8TeV = std::max( BDT_final10v16_8TeV, min_reader_final10v16_8TeV_BDT[category]+0.0001 );
	h_disc_final10v16_8TeV_CFMlpANN_BDT[category][iSys]->Fill(BDT_final10v16_8TeV,wgt);
	*/


	/*
	int ttbb_result = 0, ttcc_result = 0;
	if( false ){
	  ttbb_result = eve->ttbb_algo_result_[iSys];
	  ttcc_result = eve->ttcc_algo_result_[iSys];
	}

	h_ttbb_algo_result[category][iSys]->Fill(ttbb_result,wgt);
	h_ttcc_algo_result[category][iSys]->Fill(ttcc_result,wgt);
	*/


	h_jet_csv_1[category][iSys]->Fill(first_highest_btag,wgt);
	h_jet_csv_2[category][iSys]->Fill(second_highest_btag,wgt);
	h_jet_csv_3[category][iSys]->Fill(third_highest_btag,wgt);
	h_jet_csv_4[category][iSys]->Fill(fourth_highest_btag,wgt);

	h_min_dR_tag_tag[category][iSys]->Fill(min_dr_tagged_jets,wgt);
	h_ave_dR_tag_tag[category][iSys]->Fill(avg_dr_tagged_jets,wgt);
	h_ave_mass_tag_tag[category][iSys]->Fill(std::min(double(avg_tagged_dijet_mass),massmax)-0.00001,wgt);
	h_ave_mass_untag_untag[category][iSys]->Fill(std::min(double(avg_untagged_dijet_mass),massmax)-0.00001,wgt);

	h_best_higgs_mass[category][iSys]->Fill(std::min(double(best_higgs_mass),massmax)-0.00001,wgt);
	h_minChi2[category][iSys]->Fill(minChi2,wgt);
	h_dRbb[category][iSys]->Fill(dRbb,wgt);

	h_tagged_dijet_mass_closest_to_125[category][iSys]->Fill(std::min(double(tagged_dijet_mass_closest_to_125),massmax)-0.00001,wgt);


	TLorentzVector mydummyguy;
	mydummyguy.SetPxPyPzE(1,1,1,5);
	TLorentzVector lepW = mydummyguy;
	TLorentzVector hadW = mydummyguy;
	TLorentzVector lepB = mydummyguy;
	TLorentzVector hadB = mydummyguy;
	TLorentzVector lepT = mydummyguy;
	TLorentzVector hadT = mydummyguy;
	double minChi2_getTopSystem = 100000000;

	vdouble lepton_vect = eve->lepton_TLV_[treeSys];
	TLorentzVector leptonV;
	leptonV.SetPxPyPzE( lepton_vect[0], lepton_vect[1], lepton_vect[2], lepton_vect[3] );

	TLorentzVector metV;
	metV.SetPxPyPzE( MET*cos(MET_phi), MET*sin(MET_phi), 0, MET );


	int result_getTopSystem = 0;

	TLorentzVector sumT;
	sumT.SetPxPyPzE( 1, 1, 0, 3 );

	if( this_category<9 ){
	  result_getTopSystem = getTopSystem( leptonV, metV, jetsV, jet_CSV, minChi2_getTopSystem, hadW, lepW, hadB, lepB, hadT, lepT );

	  TLorentzVector sumT = hadT + lepT;

	  if( result_getTopSystem<0 ) std::cout << " ERROR !! result_getTopSystem = " << result_getTopSystem << std::endl;
	}



	h_leptonicW_mass[category][iSys]->Fill(std::min(lepW.M(),massmax)-0.00001,wgt);
	h_hadronicW_mass[category][iSys]->Fill(std::min(hadW.M(),massmax)-0.00001,wgt);
	h_leptonicT_mass[category][iSys]->Fill(std::min(lepT.M(),massmax)-0.00001,wgt);
	h_hadronicT_mass[category][iSys]->Fill(std::min(hadT.M(),massmax)-0.00001,wgt);

	h_hadronicW_pT[category][iSys]->Fill(std::min(hadW.Pt(),massmax)-0.00001,wgt);
	h_hadronicW_pT_mass[category][iSys]->Fill(std::min(hadW.Pt(),massmax)-0.00001,std::min(hadW.M(),massmax)-0.00001,wgt);

	h_leptonicT_pT[category][iSys]->Fill(std::min(lepT.Pt(),massmax)-0.00001,wgt);
	h_leptonicT_pT_mass[category][iSys]->Fill(std::min(lepT.Pt(),massmax)-0.00001,std::min(lepT.M(),massmax)-0.00001,wgt);
	h_hadronicT_pT[category][iSys]->Fill(std::min(hadT.Pt(),massmax)-0.00001,wgt);
	h_hadronicT_pT_mass[category][iSys]->Fill(std::min(hadT.Pt(),massmax)-0.00001,std::min(hadT.M(),massmax)-0.00001,wgt);

	h_lepT_hadT_DeltaR[category][iSys]->Fill(std::min(lepT.DeltaR(hadT),7.0)-0.00001,wgt);
	h_lepT_hadT_Angle[category][iSys]->Fill(lepT.Angle(hadT.Vect()),wgt);

	h_sumTop_pT[category][iSys]->Fill(std::min(sumT.Pt(),jetptmax)-0.00001,wgt);
	h_sumTop_mass[category][iSys]->Fill(std::min(sumT.M(),massmax_sumTop)-0.00001,wgt);
	h_sumTop_pT_mass[category][iSys]->Fill(std::min(sumT.Pt(),jetptmax)-0.00001,std::min(sumT.M(),massmax_sumTop)-0.00001,wgt);

	h_minChi2_getTopSystem[category][iSys]->Fill(minChi2_getTopSystem,wgt);

	h_leptonicW_mass_minChi2_getTopSystem[category][iSys]->Fill(std::min(lepW.M(),massmax)-0.00001,minChi2_getTopSystem,wgt);
	h_hadronicW_mass_minChi2_getTopSystem[category][iSys]->Fill(std::min(hadW.M(),massmax)-0.00001,minChi2_getTopSystem,wgt);
	h_leptonicT_mass_minChi2_getTopSystem[category][iSys]->Fill(std::min(lepT.M(),massmax)-0.00001,minChi2_getTopSystem,wgt);
	h_hadronicT_mass_minChi2_getTopSystem[category][iSys]->Fill(std::min(hadT.M(),massmax)-0.00001,minChi2_getTopSystem,wgt);




	h_M3[category][iSys]->Fill(std::min(double(M3),m3max)-0.00001,wgt);
	h_M3_1tag[category][iSys]->Fill(std::min(double(M3_1tag),m3max)-0.00001,wgt);
	h_Mlb[category][iSys]->Fill(std::min(double(Mlb),massmax)-0.00001,wgt);

	h_aplanarity[category][iSys]->Fill(aplanarity,wgt);
	h_sphericity[category][iSys]->Fill(sphericity,wgt);
	h_avg_btag_disc_non_btags[category][iSys]->Fill(avg_btag_disc_non_btags,wgt);
	h_avg_btag_disc_btags[category][iSys]->Fill(avg_btag_disc_btags,wgt);
	h_dev_from_avg_disc_btags[category][iSys]->Fill(dev_from_avg_disc_btags,wgt);
	h_lowest_btag[category][iSys]->Fill(lowest_btag,wgt);
	h_closest_tagged_dijet_mass[category][iSys]->Fill(closest_tagged_dijet_mass,wgt);


	h_h0[category][iSys]->Fill(h0,wgt);
	h_h1[category][iSys]->Fill(h1,wgt);
	h_h2[category][iSys]->Fill(h2,wgt);
	h_h3[category][iSys]->Fill(h3,wgt);
	h_h4[category][iSys]->Fill(h4,wgt);




	h_bhmv2[category][iSys]->Fill(std::min(double(bhmv2),massmax)-0.00001,wgt);
	h_maxeta_jet_jet[category][iSys]->Fill(maxeta_jet_jet,wgt);
	h_maxeta_jet_tag[category][iSys]->Fill(maxeta_jet_tag,wgt);
	h_maxeta_tag_tag[category][iSys]->Fill(maxeta_tag_tag,wgt);
	h_abs_dEta_leptop_bb[category][iSys]->Fill(abs_dEta_leptop_bb,wgt);
	h_abs_dEta_hadtop_bb[category][iSys]->Fill(abs_dEta_hadtop_bb,wgt);
	h_dEta_fn[category][iSys]->Fill(dEta_fn,wgt);
	h_abs_bb_eta[category][iSys]->Fill(abs_bb_eta,wgt);
	h_angle_tops_bb[category][iSys]->Fill(angle_tops_bb,wgt);
	h_median_bb_mass[category][iSys]->Fill(std::min(double(median_bb_mass),massmax)-0.00001,wgt);
	h_pt_all_jets_over_E_all_jets[category][iSys]->Fill(pt_all_jets_over_E_all_jets,wgt);



 

// 	if( numJets==4 && numTags==4 && iSys==0 && insample<0 ){
// 	  printf("FLAG %d %d %ld %.1f %+.2f %+.2f %.1f %.1f %.1f %.1f \n", run, lumi, evt,
// 		 tight_lepton_pt,tight_lepton_eta,tight_lepton_phi,
// 		 first_jet_pt, second_jet_pt, third_jet_pt, fourth_jet_pt);
// 	}


	// if( numJets>=6 && numTags>=4 && BDT_final10v16_8TeV>0.5 && iSys==0 && insample<0 ){
	if( numJets>=6 && numTags>=4 && iSys==0 && insample<0 ){
	  std::cout << " *************************************************************** " << std::endl;
	  if( lepSel==1 )     std::cout << " \t   muon   + jets event " << std::endl;
	  else if( lepSel==0) std::cout << " \t electron + jets event " << std::endl;
	  else if( lepSel==2) std::cout << " \t  lepton  + jets event " << std::endl;
	  std::cout << "\t grab " << run << ":" << lumi << ":" << evt << std::endl;
	  std::cout << "  run = " << run << ",\t lumi = " << lumi << ",\t event = " << evt << std::endl;
	  std::cout << "  number of jets = " << numJets << ",\t number of tags = " << numTags << std::endl;
	  // std::cout << "  BDT output (v16 8TeV) = " << BDT_final10v16_8TeV << std::endl;
	  std::cout << "  lepton: pt = " << tight_lepton_pt << ",\t eta = " << tight_lepton_eta << ",\t phi = " << tight_lepton_phi << std::endl;
	  std::cout << "  min DR (lep,jet) = " << dr_between_lep_and_closest_jet << std::endl;
	  std::cout << "  first:  jet pt = " << first_jet_pt  << ",\t jet csv = " << first_highest_btag   << std::endl;
	  std::cout << "  second: jet pt = " << second_jet_pt << ",\t jet csv = " << second_highest_btag  << std::endl;
	  std::cout << "  third:  jet pt = " << third_jet_pt  << ",\t jet csv = " << third_highest_btag  << std::endl;
	  std::cout << "  fourth: jet pt = " << fourth_jet_pt << ",\t jet csv = " << fourth_highest_btag << std::endl;
	  std::cout << "  lowest csv     = " << lowest_btag << std::endl;
	  std::cout << "  ave CSV (tags) = " << avg_btag_disc_btags << ",\t (untags) = " << avg_btag_disc_non_btags << std::endl;
	  std::cout << "  dev from ave CSV (tags) = " << dev_from_avg_disc_btags << std::endl;
	  std::cout << "  min DR (tag,tag) = " << min_dr_tagged_jets << ",\t ave DR (tag,tag) = " << avg_dr_tagged_jets << std::endl;
	  std::cout << "  MET:    pt = " << MET << ",\t phi = " << MET_phi << std::endl;
	  std::cout << "  MHT:    pt = " << MHT << ",\t phi = " << MHT_phi << std::endl;
	  std::cout << "  Transverse mass (MET) = " << MT_met << ",\t (MHT) = " << MT_mht << std::endl;
	  std::cout << "  Mlb = " << Mlb << ",\t M3 (1 tag) = " << M3_1tag << ",\t M3 = " << M3 << std::endl;
	  std::cout << "  aplanarity = " << aplanarity << ",\t sphericity = " << sphericity << std::endl;
	  std::cout << "  closest mass (tag,tag) = " << closest_tagged_dijet_mass << ",\t ave mass (tag,tag) = " << avg_tagged_dijet_mass << ",\t (untag,untag) = " << avg_untagged_dijet_mass << std::endl;
	  std::cout << "  sum pt (lepton,jets,met) = " << all_sum_pt_with_met << ",\t sum pt (lepton,jets) = " << all_sum_pt << std::endl;
	  std::cout << "  mass (lepton,jets,met) = " << invariant_mass_of_everything << std::endl;
	  std::cout << "  best_higgs_mass = " << best_higgs_mass << std::endl;
	  printf("  getTopSystem mass: lepW = %.1f,\t hadW = %.1f,\t lepT = %.1f,\t hadT = %.1f \n",
		 lepW.M(), hadW.M(), lepT.M(), hadT.M() );
	  printf("  BHM v2 = %.1f,\t mass: lepW = %.1f,\t leptop = %.1f,\t hadW = %.1f,\t hadtop = %.1f \n", 
		 bhm_v2, mass_lepW, mass_leptop, mass_hadW, mass_hadtop );
	  std::cout << " *************************************************************** " << std::endl;
	}
      } // end loop over categories


    } // end loop over systematics
  } // end loop over events

  std::cout << " Done! " << std::endl;

  histofile.Write();
  histofile.Close();

}



int getTopSystem(TLorentzVector lepton, TLorentzVector met, vecTLorentzVector jets, vdouble btag,
		 double &minChi, TLorentzVector &hadW, TLorentzVector &lepW, TLorentzVector &hadB, TLorentzVector &lepB, TLorentzVector &hadT, TLorentzVector &lepT){

  
  int nJets = int(jets.size());

  double chi_top_lep=10000;
  double chi_top_had=10000;
  //double chi_W_lep=10000; //isn't really used
  double chi_W_had=10000;

  minChi = 1000000;
  double btagCut = 0.679;
  double W_mass = 80.4;
  double top_mass = 172.5;

  // from Darren 2/22/2013
  double sigma_hadW   = 10.51;
  double sigma_hadTop = 17.97;
  double sigma_lepTop = 10.02;

  double metPz[2];
  double chi=999999;


  int nBtags = 0;
  for(int i=0;i<nJets;i++){
    if(btag[i]>btagCut) nBtags++;
  }

  if( !(nJets>=4 && nBtags>=2) ) return -1;

  int nUntags = nJets-nBtags;

  double lowest_btag = 99.;
  double second_lowest_btag = 999.;
  int ind_lowest_btag = 999;
  int ind_second_lowest_btag = 999;

  if( nUntags<2 ){
    for(int i=0;i<nJets;i++){
      if( btag[i]<lowest_btag ){
	second_lowest_btag = lowest_btag;
	ind_second_lowest_btag = ind_lowest_btag;
	
	lowest_btag = btag[i];
	ind_lowest_btag = i;
      }
      else if( btag[i]<second_lowest_btag ){
	second_lowest_btag = btag[i];
	ind_second_lowest_btag = i;
      }
    }
  }


  // First get the neutrino z
  double energyLep = lepton.E();
  double a = (W_mass*W_mass/(2.0*energyLep)) + (lepton.Px()*met.Px() + lepton.Py()*met.Py())/energyLep;
  double radical = (2.0*lepton.Pz()*a/energyLep)*(2.0*lepton.Pz()*a/energyLep);
  radical = radical - 4.0*(1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep))*(met.Px()*met.Px() + met.Py()*met.Py()- a*a);
  if (radical < 0.0) radical = 0.0;
  metPz[0] = (lepton.Pz()*a/energyLep) + 0.5*sqrt(radical);
  metPz[0] = metPz[0] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));
  metPz[1] = (lepton.Pz()*a/energyLep) - 0.5*sqrt(radical);
  metPz[1] = metPz[1] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));


  // Loop over all jets, both Pz, calcaulte chi-square
  TLorentzVector metNew;
  for( int ipznu=0; ipznu<2; ipznu++ ){
    metNew.SetXYZM(met.Px(),met.Py(),metPz[ipznu],0.0); //neutrino has mass 0
    //with b-tag info
    if( nJets>=4 && nBtags>=2 ){
      vecTLorentzVector not_b_tagged,b_tagged;
      //fill not_b_tagged and b_tagged
      for( int i=0;i<nJets;i++ ){
	if( btag[i]>btagCut && i!=ind_second_lowest_btag && i!=ind_lowest_btag) b_tagged.push_back(jets[i]);
	else not_b_tagged.push_back(jets[i]);
      }
      //first make possible t_lep's with b-tagged jets (includes making W_lep)
      for( int i=0; i<int(b_tagged.size()); i++ ){
	TLorentzVector W_lep=metNew+lepton; //used for histogram drawing only
	TLorentzVector top_lep=metNew+lepton+b_tagged.at(i);
	chi_top_lep=pow((top_lep.M()-top_mass)/sigma_lepTop,2);
	//next make possible W_had's with not b-tagged jets
	for( int j=0; j<int(not_b_tagged.size()); j++ ){
	  for( int k=0; k<int(not_b_tagged.size()); k++ ){
	    if( j!=k ){
	      TLorentzVector W_had=not_b_tagged.at(j)+not_b_tagged.at(k);
	      chi_W_had=pow((W_had.M()-W_mass)/sigma_hadW,2);
	      //now make possible top_had's (using the W_had + some b-tagged jet)
	      for( int l=0; l<int(b_tagged.size()); l++ ){
		if( l!=i ){
		  TLorentzVector top_had=W_had+b_tagged.at(l);
		  chi_top_had=pow((top_had.M()-top_mass)/sigma_hadTop,2);
		  chi=chi_top_lep+chi_W_had+chi_top_had;
		  //accept the lowest chi
		  if( chi<minChi ){
		    minChi=chi;
		    hadW = W_had;
		    lepW = W_lep;
		    hadB = b_tagged.at(l);
		    lepB = b_tagged.at(i);
		    hadT = top_had;
		    lepT = top_lep;
		  } // end if chi<minChi
		}
	      }
	    }
	  }
	}
      }
    }
  }

  return 1;
}


/// algos / supporting functions:



double get_bhm_from_own_algo(double MET, double METphi, TLorentzVector &metv, vdouble lep, vvdouble jets, vdouble csv, double &minChi, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, double &dRbb)
{
	metv.SetPtEtaPhiE(MET,0.,METphi,MET);
	TLorentzVector lepton;
	lepton.SetPxPyPzE(lep[0],lep[1],lep[2],lep[3]);
	vecTLorentzVector jet_TLVs;	
	convert_jets_to_TLVs(jets, jet_TLVs);
		
	//double minChi;
	//double dRbb;
	TLorentzVector bjet1;
	TLorentzVector bjet2;
	
	TLorentzVector test1;
	TLorentzVector test2;
	
	double bhm = getBestHiggsMass2(lepton, metv, jet_TLVs, csv, minChi, dRbb, bjet1, bjet2, chi2lepW, chi2leptop, chi2hadW, chi2hadtop, mass_lepW, mass_leptop, mass_hadW, mass_hadtop, test1, test2); // Jon T. version 2

	return bhm;
}


double getBestHiggsMass2(TLorentzVector lepton, TLorentzVector &met, vecTLorentzVector jets, vdouble btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, TLorentzVector &toplep, TLorentzVector &tophad)
{

  int nJets = int(jets.size());
  double pfmet_px=met.Px(), pfmet_py=met.Py();
  double chi_top_lep=10000;
  double chi_top_had=10000;
  //double chi_W_lep=10000; //isn't really used
  double chi_W_had=10000;

  minChi = 1000000;
  dRbb = 1000000;
  double btagCut = 0.679;
  double W_mass = 80.0;
  double top_mass = 172.5;
  //double H_mass=120.0;

  // updated 8/22/2012 from J. Timcheck
  //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttH
  double sigma_hadW   = 12.77;
  double sigma_hadTop = 18.9;
  //double sigma_lepTop = 32.91;
  double sigma_lepTop = 18.9;

  // //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttH
  // double sigma_hadW   = 12.59;
  // double sigma_hadTop = 19.9;
  // double sigma_lepTop = 39.05;

  //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttJets
  /*double sigma_hadW		= 12.72,
    sigma_hadTop	= 18.12,
    sigma_lepTop	= 38.72;
  */
  
  /// more initializitions
  
  bjet1.SetPxPyPzE(1.,1.,1.,2.);
  bjet2.SetPxPyPzE(1.,1.,1.,2.);
//  chi2lepW = 0.;
//  chi2leptop = 0.;
//  chi2hadtop = 0.;
  mass_lepW = 0.;
  mass_leptop = 0.;
  mass_hadW = 0.;
  mass_hadtop = 0.;
  toplep.SetPxPyPzE(1.,1.,1.,2.);
  tophad.SetPxPyPzE(1.,1.,1.,2.);
  
  
  double metPz[2];
  double chi=999999;

  //stuff to find:
  double higgs_mass_high_energy=0;

  int nBtags = 0;
  for(int i=0;i<nJets;i++){
    if(btag[i]>btagCut) nBtags++;
  }

  int nUntags = nJets-nBtags;

  double lowest_btag = 99.;
  double second_lowest_btag = 999.;
  int ind_lowest_btag = 999;
  int ind_second_lowest_btag = 999;

  vdouble btag_sorted = btag;
  //int ind_fourth_highest = 999;

  if( nJets>=6 && nBtags>=4 ){
    
    if( nUntags<2 ){
      for(int i=0;i<nJets;i++){
	if( btag[i]<lowest_btag ){
	  second_lowest_btag = lowest_btag;
	  ind_second_lowest_btag = ind_lowest_btag;

	  lowest_btag = btag[i];
	  ind_lowest_btag = i;
	}
	else if( btag[i]<second_lowest_btag ){
	  second_lowest_btag = btag[i];
	  ind_second_lowest_btag = i;
	}
      }
    }
    /*
    if( nBtags==3 )
    {
	sort(btag_sorted.begin(),btag_sorted.end());
	double fourth_highest_csv = btag_sorted[nJets-4];
	
	for (int f=0; f<nJets; f++)
	{
		if (btag[f]==fourth_highest_csv) ind_fourth_highest = f;
	}

    }
    */
  }

    //Handle 6j3t.
  int ind_promoted_btag = 999;

  if( nJets>=6 && nBtags==3 ){
    for(int i=0;i<nJets;i++){
      int rank = 0;
      for(int j=0;j<nJets;j++){
	if( btag[j] > btag[i] ){
	  rank++;
	}
      }
      if( rank == 3 ) ind_promoted_btag = i;
    }
  }


  // First get the neutrino z
  double energyLep = lepton.E();
  double a = (W_mass*W_mass/(2.0*energyLep)) + (lepton.Px()*met.Px() + lepton.Py()*met.Py())/energyLep;
  double radical = (2.0*lepton.Pz()*a/energyLep)*(2.0*lepton.Pz()*a/energyLep);
  radical = radical - 4.0*(1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep))*(met.Px()*met.Px() + met.Py()*met.Py()- a*a);
  
  bool imaginary = false;

if (radical < 0.0)
{
	imaginary=true;
}
if(imaginary)
{
	radical=-1.0;
	double value=.001;
	while(true)
	{
		met.SetPxPyPzE(pfmet_px,pfmet_py,0.0,sqrt(pow(pfmet_px,2)+pow(pfmet_py,2))); //neutrino mass 0, pt = sqrt(px^2+py^2)
//			energyLep = lepton.E();
		a = (W_mass*W_mass/(2.0*energyLep)) + (lepton.Px()*met.Px() + lepton.Py()*met.Py())/energyLep;
		radical = (2.0*lepton.Pz()*a/energyLep)*(2.0*lepton.Pz()*a/energyLep);
		radical = radical - 4.0*(1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep))*(met.Px()*met.Px() + met.Py()*met.Py()- a*a);
		if(radical>=0)
			break;
		pfmet_px-=pfmet_px*value;
		pfmet_py-=pfmet_py*value;
	}
}


  metPz[0] = (lepton.Pz()*a/energyLep) + 0.5*sqrt(radical);
  metPz[0] = metPz[0] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));
  metPz[1] = (lepton.Pz()*a/energyLep) - 0.5*sqrt(radical);
  metPz[1] = metPz[1] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));



  // Loop over all jets, both Pz, calcaulte chi-square
  TLorentzVector metNew;
  for( int ipznu=0; ipznu<2; ipznu++ ){
    metNew.SetXYZM(met.Px(),met.Py(),metPz[ipznu],0.0); //neutrino has mass 0
    //with b-tag info
    if(( nJets>=6 && nBtags>=4 )||( nJets>=6 && nBtags==3 )){
      vecTLorentzVector not_b_tagged,b_tagged;
      //fill not_b_tagged and b_tagged
      for( int i=0;i<nJets;i++ ){
      
        //if (nBtags>=4)
	//{
		if( (btag[i]>btagCut && i!=ind_second_lowest_btag && i!=ind_lowest_btag) || (i==ind_promoted_btag) ) b_tagged.push_back(jets[i]);
		else not_b_tagged.push_back(jets[i]);
	//}
	/*
	if (nBtags==3)
	{
      		if( btag[i]>btagCut || i==ind_fourth_highest) b_tagged.push_back(jets[i]);
		else not_b_tagged.push_back(jets[i]);
      	}
 	*/
      
      }
      //first make possible t_lep's with b-tagged jets (includes making W_lep)
      for( int i=0; i<int(b_tagged.size()); i++ ){
	TLorentzVector W_lep=metNew+lepton; //used for histogram drawing only
	TLorentzVector top_lep=metNew+lepton+b_tagged.at(i);
	chi_top_lep=pow((top_lep.M()-top_mass)/sigma_lepTop,2);
	//next make possible W_had's with not b-tagged jets
	for( int j=0; j<int(not_b_tagged.size()); j++ ){
	  for( int k=0; k<int(not_b_tagged.size()); k++ ){
	    if( j!=k ){
	      TLorentzVector W_had=not_b_tagged.at(j)+not_b_tagged.at(k);
	      chi_W_had=pow((W_had.M()-W_mass)/sigma_hadW,2);
	      //now make possible top_had's (using the W_had + some b-tagged jet)
	      for( int l=0; l<int(b_tagged.size()); l++ ){
		if( l!=i ){
		  TLorentzVector top_had=W_had+b_tagged.at(l);
		  chi_top_had=pow((top_had.M()-top_mass)/sigma_hadTop,2);
		  chi=chi_top_lep+chi_W_had+chi_top_had;
		  //accept the lowest chi
		  if( chi<minChi ){
		    minChi=chi;
		    //pick the other two b's that have the highest et (energy in transverse plane) as higgs mass constituents
		    TLorentzVector H2;
		    int numH2Constituents=0;
		    
		    TLorentzVector bBest[2];
		    
		    for( int m=0; m<int(b_tagged.size()); m++ ){
		      if( m!=i && m!=l && numH2Constituents<2 ){
			bBest[numH2Constituents] = b_tagged.at(m);
			numH2Constituents++;
			H2+=b_tagged.at(m);
		      }
		    }
		    dRbb = bBest[0].DeltaR( bBest[1] );
		    higgs_mass_high_energy=H2.M();
		    bjet1 = bBest[0];
		    bjet2 = bBest[1];
		    
		    mass_lepW = W_mass;
		    mass_leptop = top_lep.M();
		    mass_hadW = W_had.M();
		    mass_hadtop = top_had.M();
		    toplep = top_lep;
		    tophad = top_had;
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
chi2lepW = 0.;
chi2leptop = chi_top_lep;
chi2hadtop = chi_top_had;
chi2hadW = chi_W_had;



  
  return higgs_mass_high_energy;

}



double get_jet_jet_etamax (vvdouble jets)
{
	vecTLorentzVector thejets;
	convert_jets_to_TLVs(jets, thejets);
	
	int count=0;
	double avgval=0.;
	
	for (int i=0; i<int(thejets.size()-1); i++)
	{
	  for (int j=i+1; j<int(thejets.size()); j++)
		{
				
				avgval += thejets[i].Eta()-thejets[j].Eta();
				count++;
		}
	}
	
	avgval /= count;
	
	double imax = 0.;
	double etamax=0.;
	
	for (int k=0; k<int(thejets.size()); k++)
	{
		imax = abs(thejets[k].Eta()-avgval);
		
		if(imax>etamax)
		{
			etamax = imax;
		}
	}

	return etamax;
}

double get_jet_tag_etamax (vvdouble jets, vdouble jetCSV)
{
	vecTLorentzVector thejets;
	convert_jets_to_TLVs(jets, thejets);
	
	int count=0;
	double avgval=0.;
	
	for (int i=0; i<int(thejets.size()-1); i++)
	{
	  for (int j=i+1; j<int(thejets.size()); j++)
		{
				
				avgval += thejets[i].Eta()-thejets[j].Eta();
				count++;
		}
	}
	
	avgval /= count;
	
	double imax = 0.;
	double etamax=0.;
	
	
	vecTLorentzVector thetags;
	vect_of_tagged_TLVs(jets, jetCSV, thetags);
	
	
	for (int k=0; k<int(thetags.size()); k++)
	{
		imax = abs(thetags[k].Eta()-avgval);
		
		if(imax>etamax)
		{
			etamax = imax;
		}
	}

	return etamax;
}


double get_tag_tag_etamax (vvdouble jets, vdouble jetCSV)
{

	vecTLorentzVector thetags;
	vect_of_tagged_TLVs(jets, jetCSV, thetags);
		
	int count=0;
	double avgval=0.;
	
	for (int i=0; i<int(thetags.size()-1); i++)
	{
	  for (int j=i+1; j<int(thetags.size()); j++)
		{
				
				avgval += thetags[i].Eta()-thetags[j].Eta();
				count++;
		}
	}
	
	avgval /= count;
	
	double imax = 0.;
	double etamax=0.;
	
	
	for (int k=0; k<int(thetags.size()); k++)
	{
		imax = abs(thetags[k].Eta()-avgval);
		
		if(imax>etamax)
		{
			etamax = imax;
		}
	}

	return etamax;
}




void vect_of_tagged_TLVs(vvdouble jets, vdouble jetCSV, vecTLorentzVector &vect_of_btag_TLVs)
{
	TLorentzVector tagged_jet;
	
	int nJets = jets.size();
	double btagCut = 0.679;
	
	for(int i=0;i<nJets;i++)
	{
		if (jetCSV[i]>btagCut)
		{
		
			tagged_jet.SetPxPyPzE(jets[i][0],jets[i][1],jets[i][2],jets[i][3]);
			vect_of_btag_TLVs.push_back(tagged_jet);
		}
	}
}

void vect_of_untagged_TLVs(vvdouble jets, vdouble jetCSV, vecTLorentzVector &vect_of_nontagged_TLVs)
{
	TLorentzVector untagged_jet;
	
	int nJets = jets.size();
	double btagCut = 0.679;
	
	for(int i=0;i<nJets;i++)
	{
		if (!(jetCSV[i]>btagCut))
		{
			untagged_jet.SetPxPyPzE(jets[i][0],jets[i][1],jets[i][2],jets[i][3]);
			vect_of_nontagged_TLVs.push_back(untagged_jet);
		}
	}
}

void convert_jets_to_TLVs(vvdouble jets, vecTLorentzVector &vect_of_jet_TLVs)
{
	TLorentzVector jet;	
	int nJets = jets.size();
	
	for(int i=0;i<nJets;i++)
	{
		jet.SetPxPyPzE(jets[i][0],jets[i][1],jets[i][2],jets[i][3]);
		vect_of_jet_TLVs.push_back(jet);
	}
}

double w_mass_top(vvdouble jets, vdouble jetCSV)
{
	double mass = -1.;
	
	vecTLorentzVector vect_of_btags;
	vecTLorentzVector vect_of_untags;
	
	vect_of_tagged_TLVs(jets,jetCSV,vect_of_btags);
	vect_of_untagged_TLVs(jets,jetCSV,vect_of_untags);

	double chi=999999.;
	double chi_top_had=10000.;
	double chi_W_had=10000.;
	
	double minChi = 1000000.;
	
	double W_mass = 80.0;
	double top_mass = 172.5;
	//double H_mass=120.0;

	double sigma_hadW = 12.59, sigma_hadTop = 19.9;

	// double sigma_hadW 	= 12.59,
	// 	sigma_hadTop 	= 19.9,
	// 	sigma_lepTop	= 39.05;
			 
	//double sigma_H		= 15.79;

	
	int ntags = vect_of_btags.size();
	int untags = vect_of_untags.size();


	for(int j=0;j<untags;j++)
	{
		for(int k=0;k<untags;k++)
		{
			if(j!=k)
			{
				TLorentzVector W_had = vect_of_untags.at(j)+vect_of_untags.at(k);
				chi_W_had = pow((W_had.M()-W_mass)/sigma_hadW,2);

				for(int l=0;l<ntags;l++)
				{
					TLorentzVector top_had = W_had + vect_of_btags.at(l);
					chi_top_had = pow((top_had.M()-top_mass)/sigma_hadTop,2);
					chi = chi_W_had + chi_top_had;
					
					if(chi<minChi)
					{
						minChi = chi;
						mass = W_had.M();
					}
				}
			}
		}
	}
	
	return mass;	
}
double had_top_mass(vvdouble jets, vdouble jetCSV)
{
	double mass = -1.;
	
	vecTLorentzVector vect_of_btags;
	vecTLorentzVector vect_of_untags;
	
	vect_of_tagged_TLVs(jets,jetCSV,vect_of_btags);
	vect_of_untagged_TLVs(jets,jetCSV,vect_of_untags);

	double chi=999999.;
	double chi_top_had=10000.;
	double chi_W_had=10000.;
	
	double minChi = 1000000.;
	
	double W_mass = 80.0;
	double top_mass = 172.5;
	//double H_mass=120.0;

	double sigma_hadW = 12.59, sigma_hadTop = 19.9;

	// double sigma_hadW 	= 12.59,
	// 	sigma_hadTop 	= 19.9,
	// 	sigma_lepTop	= 39.05;
			 
	//double sigma_H		= 15.79;

	
	int ntags = vect_of_btags.size();
	int untags = vect_of_untags.size();


	for(int j=0;j<untags;j++)
	{
		for(int k=0;k<untags;k++)
		{
			if(j!=k)
			{
				TLorentzVector W_had = vect_of_untags.at(j)+vect_of_untags.at(k);
				chi_W_had = pow((W_had.M()-W_mass)/sigma_hadW,2);

				for(int l=0;l<ntags;l++)
				{
					TLorentzVector top_had = W_had + vect_of_btags.at(l);
					chi_top_had = pow((top_had.M()-top_mass)/sigma_hadTop,2);
					chi = chi_W_had + chi_top_had;
					
					if(chi<minChi)
					{
						minChi = chi;
						mass = top_had.M();
					}
				}
			}
		}
	}
	
	return mass;	
}

double ww_mass_nontop(vvdouble jets, vdouble jetCSV)
{

	double mass = -1.;
	
	vecTLorentzVector vect_of_btags;
	vecTLorentzVector vect_of_untags;
	
	vect_of_tagged_TLVs(jets,jetCSV,vect_of_btags);
	vect_of_untagged_TLVs(jets,jetCSV,vect_of_untags);

	double chi=999999.;
	double chi_top_had=10000.;
	double chi_W_had=10000.;
	
	double minChi = 1000000.;
	
	double W_mass = 80.0;
	double top_mass = 172.5;
	//double H_mass=120.0;

	double sigma_hadW = 12.59, sigma_hadTop = 19.9;

	// double sigma_hadW 	= 12.59,
	// 	sigma_hadTop 	= 19.9,
	// 	sigma_lepTop	= 39.05;
			 
	//double sigma_H		= 15.79;

	
	int ntags = vect_of_btags.size();
	int untags = vect_of_untags.size();

	TLorentzVector mydummyguy;
	mydummyguy.SetPxPyPzE(0,0,0,0);

	for(int j=0;j<untags;j++)
	{
		for(int k=0;k<untags;k++)
		{
			if(j!=k)
			{
				TLorentzVector W_had = vect_of_untags.at(j)+vect_of_untags.at(k);
				chi_W_had = pow((W_had.M()-W_mass)/sigma_hadW,2);

				for(int l=0;l<ntags;l++)
				{
					TLorentzVector top_had = W_had + vect_of_btags.at(l);
					chi_top_had = pow((top_had.M()-top_mass)/sigma_hadTop,2);
					chi = chi_W_had + chi_top_had;
					
					if(chi<minChi)
					{
						minChi = chi;
						TLorentzVector H2 = mydummyguy;
						
						for(int m=0; m<untags; m++)
						{
							if((m!=j) && (m!=k))
							{												
								H2 += vect_of_untags.at(m);
							}
						}

						mass = H2.M();
					}
				}
			}
		}
	}
	
	return mass;
}

void nontop_untags(vvdouble jets, vdouble jetCSV, vecTLorentzVector &vect_of_nontagged_TLVs_not_top)
{

  //double mass = -1.;
	
	vecTLorentzVector vect_of_btags;
	vecTLorentzVector vect_of_untags;
	
	vect_of_tagged_TLVs(jets,jetCSV,vect_of_btags);
	vect_of_untagged_TLVs(jets,jetCSV,vect_of_untags);

	double chi=999999.;
	double chi_top_had=10000.;
	double chi_W_had=10000.;
	
	double minChi = 1000000.;
	
	double W_mass = 80.0;
	double top_mass = 172.5;
	//double H_mass=120.0;

	double sigma_hadW = 12.59, sigma_hadTop = 19.9;

	// double sigma_hadW 	= 12.59,
	// 	sigma_hadTop 	= 19.9,
	// 	sigma_lepTop	= 39.05;
			 
	//double sigma_H		= 15.79;

	
	int ntags = vect_of_btags.size();
	int untags = vect_of_untags.size();


	for(int j=0;j<untags;j++)
	{
		for(int k=0;k<untags;k++)
		{
			if(j!=k)
			{
				TLorentzVector W_had = vect_of_untags.at(j)+vect_of_untags.at(k);
				chi_W_had = pow((W_had.M()-W_mass)/sigma_hadW,2);

				for(int l=0;l<ntags;l++)
				{
					TLorentzVector top_had = W_had + vect_of_btags.at(l);
					chi_top_had = pow((top_had.M()-top_mass)/sigma_hadTop,2);
					chi = chi_W_had + chi_top_had;
					
					if(chi<minChi)
					{
						minChi = chi;
						vecTLorentzVector result;
						
						for(int m=0; m<untags; m++)
						{
							if((m!=j) && (m!=k))
							{												
								result.push_back(vect_of_untags.at(m));
							}
						}
						
						
						vect_of_nontagged_TLVs_not_top = result;
					}
				}
			}
		}
	}
	
}

double avg_pt_nontop_untags(vvdouble jets, vdouble jetCSV)
{
	double avgpt = 0.;
	vecTLorentzVector vTLV_untags;
	nontop_untags(jets, jetCSV, vTLV_untags);	
	int asdf = vTLV_untags.size();

	for (int i=0; i<asdf; i++)
	{
		avgpt += vTLV_untags[i].Perp();
	}
	
	avgpt = avgpt / asdf;	
	return avgpt;
}

double pt_E_ratio_jets(vvdouble jets)
{
	double ratio = 0.;
	double ptsum = 0.;
	double Esum = 0.;
	
	vecTLorentzVector jetvect;
	convert_jets_to_TLVs(jets,jetvect);
	
	for (int i=0; i<int(jetvect.size()); i++)
	{
		ptsum += jetvect[i].Pt();
		Esum += jetvect[i].E();
	}
	
	ratio = ptsum / Esum;
	
	return ratio;
}

double study_tops_bb_syst (double MET, double METphi, TLorentzVector &metv, vdouble lep, vvdouble jets, vdouble csv, double &minChi, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, double &dRbb, double &testquant1, double &testquant2, double &testquant3, double &testquant4, double &testquant5, double &testquant6, double &testquant7, TLorentzVector &b1, TLorentzVector &b2)
{
	// cout<< "in study_tops_bb_syst" << endl;
	
	double pi = 3.14;
	
	metv.SetPtEtaPhiE(MET,0.,METphi,MET);
	
	// cout<< metv.Pt() << endl;
	
	TLorentzVector lepton;
	
	
	lepton.SetPxPyPzE(lep[0],lep[1],lep[2],lep[3]);
	
	// cout<< lepton.Pt() << endl;
	
	vecTLorentzVector jet_TLVs;	
	
	
	convert_jets_to_TLVs(jets, jet_TLVs);
	
	
	// cout<< jet_TLVs[0].Pt() << endl;
		
	//double minChi;
	//double dRbb;
	TLorentzVector bjet1;
	TLorentzVector bjet2;
	TLorentzVector leptop;
	TLorentzVector hadtop;
	
	// cout<< "before bhm" << endl;
	
	double bhm = getBestHiggsMass2(lepton, metv, jet_TLVs, csv, minChi, dRbb, bjet1, bjet2, chi2lepW, chi2leptop, chi2hadW, chi2hadtop, mass_lepW, mass_leptop, mass_hadW, mass_hadtop, leptop, hadtop); // Jon T. version 2

	
	b1 = bjet1;
	b2 = bjet2;
	
	TLorentzVector bsyst = bjet1+bjet2;
	TLorentzVector topsyst = leptop+hadtop;
	
	double dphihad = bsyst.DeltaPhi(hadtop);
	double dphilep = bsyst.DeltaPhi(leptop);
	
	
	testquant1 = bsyst.Eta() - leptop.Eta();	
	
	// cout<< testquant1 << endl;
	
	testquant2 = bsyst.Eta() - hadtop.Eta();
	
	// cout<< testquant2 << endl;
	
	testquant3 = fabs((dphilep - pi)*(dphilep + pi)) + pow(dphihad,2);
	testquant3 = sqrt(testquant3 / (2.0*pow(pi,2)));		
	
	// cout<< testquant3 << endl;
	
	testquant4 = bsyst.Eta();
	
	// cout<< testquant4 << endl;
	
	testquant5 = (hadtop.Eta() + leptop.Eta())/2;
	
	// cout<< testquant5 << endl;
		
	testquant6 = sqrt(abs((bsyst.Eta() - leptop.Eta())*(bsyst.Eta() - hadtop.Eta())));
	
	// cout<< testquant6 << endl;
	
	testquant7 = bsyst.Angle(topsyst.Vect());
	
	// cout<< testquant7 << endl;
	
	return bhm;
}

double get_median_bb_mass (vvdouble jets, vdouble jetCSV)
{

	// all btags
	vecTLorentzVector all_btags;
	TLorentzVector bb;

	vect_of_tagged_TLVs(jets, jetCSV, all_btags);

	int bbcount = 0;
	vector<double> median_vect;
	double median_mass = 0.;


	for (int asdf=0; asdf<int(all_btags.size()-1); asdf++)
	{
	  for (int j=asdf+1; j<int(all_btags.size()); j++)
		{	

			bb = all_btags[asdf]+all_btags[j];

			median_vect.push_back(bb.M());

			bbcount++;

		}
	}

	float vectpos = (float)median_vect.size();

	vectpos = floor(vectpos/2)-1; // all these are even -> gets lower one

	sort(median_vect.begin(),median_vect.end());

	median_mass = median_vect[vectpos+1]; // gets upper one


	return median_mass;

}


/*
double get_csv_wgt( vvdouble jets, vdouble jetCSV, vint jetFlavor, int iSys ){

  int iSysHF = 0;
  switch(iSys){
  case 11: iSysHF=1; break;
  case 12: iSysHF=2; break;
  case 17: iSysHF=3; break;
  case 18: iSysHF=4; break;
  case 21: iSysHF=5; break;
  case 22: iSysHF=6; break;
  case 25: iSysHF=7; break;
  case 26: iSysHF=8; break;
  default : iSysHF = 0; break;
  }

  int iSysC = 0;
  switch(iSys){
  case 29: iSysC=1; break;
  case 30: iSysC=2; break;
  case 31: iSysC=3; break;
  case 32: iSysC=4; break;
  default : iSysC = 0; break;
  }

  int iSysLF = 0;
  switch(iSys){
  case 11: iSysLF=1; break;
  case 12: iSysLF=2; break;
  case 19: iSysLF=3; break;
  case 20: iSysLF=4; break;
  case 23: iSysLF=5; break;
  case 24: iSysLF=6; break;
  case 27: iSysLF=7; break;
  case 28: iSysLF=8; break;
  default : iSysLF = 0; break;
  }

  double csvWgthf = 1.;
  double csvWgtC  = 1.;
  double csvWgtlf = 1.;


  for( int iJet=0; iJet<int(jets.size()); iJet++ ){
    TLorentzVector myJet;
    myJet.SetPxPyPzE( jets[iJet][0], jets[iJet][1], jets[iJet][2], jets[iJet][3] );
	  
    double csv = jetCSV[iJet];
    double jetPt = myJet.Pt();
    double jetAbsEta = fabs( myJet.Eta() );
    int flavor = jetFlavor[iJet];

    int iPt = -1; int iEta = -1;
    if (jetPt >=19.99 && jetPt<30) iPt = 0;
    else if (jetPt >=30 && jetPt<40) iPt = 1;
    else if (jetPt >=40 && jetPt<60) iPt = 2;
    else if (jetPt >=60 && jetPt<100) iPt = 3;
    else if (jetPt >=100 && jetPt<160) iPt = 4;
    else if (jetPt >=160 && jetPt<10000) iPt = 5;

    if (jetAbsEta >=0 &&  jetAbsEta<0.8 ) iEta = 0;
    else if ( jetAbsEta>=0.8 && jetAbsEta<1.6 )  iEta = 1;
    else if ( jetAbsEta>=1.6 && jetAbsEta<2.41 ) iEta = 2;

    if (iPt < 0 || iEta < 0) std::cout << "Error, couldn't find Pt, Eta bins for this b-flavor jet, jetPt = " << jetPt << ", jetAbsEta = " << jetAbsEta << std::endl;

    if (abs(flavor) == 5 ){
      int useCSVBin = (csv>=0.) ? h_csv_wgt_hf[iSysHF][iPt]->FindBin(csv) : 1;
      double iCSVWgtHF = h_csv_wgt_hf[iSysHF][iPt]->GetBinContent(useCSVBin);
      if( iCSVWgtHF!=0 ) csvWgthf *= iCSVWgtHF;

      // if( iSysHF==0 ) printf(" iJet,\t flavor=%d,\t pt=%.1f,\t eta=%.2f,\t csv=%.3f,\t wgt=%.2f \n",
      // 			     flavor, jetPt, iJet->eta, csv, iCSVWgtHF );
    }
    else if( abs(flavor) == 4 ){
      int useCSVBin = (csv>=0.) ? c_csv_wgt_hf[iSysC][iPt]->FindBin(csv) : 1;
      double iCSVWgtC = c_csv_wgt_hf[iSysC][iPt]->GetBinContent(useCSVBin);
      if( iCSVWgtC!=0 ) csvWgtC *= iCSVWgtC;
      // if( iSysC==0 ) printf(" iJet,\t flavor=%d,\t pt=%.1f,\t eta=%.2f,\t csv=%.3f,\t wgt=%.2f \n",
      //      flavor, jetPt, iJet->eta, csv, iCSVWgtC );
    }
    else {
      if (iPt >=3) iPt=3;       /// [30-40], [40-60] and [60-10000] only 3 Pt bins for lf
      int useCSVBin = (csv>=0.) ? h_csv_wgt_lf[iSysLF][iPt][iEta]->FindBin(csv) : 1;
      double iCSVWgtLF = h_csv_wgt_lf[iSysLF][iPt][iEta]->GetBinContent(useCSVBin);
      if( iCSVWgtLF!=0 ) csvWgtlf *= iCSVWgtLF;

      // if( iSysLF==0 ) printf(" iJet,\t flavor=%d,\t pt=%.1f,\t eta=%.2f,\t csv=%.3f,\t wgt=%.2f \n",
      // 			     flavor, jetPt, iJet->eta, csv, iCSVWgtLF );
    }
  }

  double csvWgtTotal = csvWgthf * csvWgtC * csvWgtlf;

  return csvWgtTotal;
}
*/
