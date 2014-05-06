// created 2/18/12 by Geoff Smith
// use this to make tree with own vars for training from yggdrasil vars
// compile in usual way
// 
// modified 5/6/14 by Geoff Smith
// removed extraneous code, uploaded to github 
//

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
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
#include "TVector.h"
#include "TLorentzVector.h"
// list of availble variables:
#include "AnalysisCode/LeptonPlusJets/interface/BEANeventVars.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)

#include "TMVA/Config.h"
#include "TMVA/Tools.h"
// #include "AnalysisCode/LeptonPlusJets/interface/BEANeventVars.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"


#endif


// utilities / type converters:
void convert_jets_to_TLVs(vvdouble jets, vecTLorentzVector &vect_of_jet_TLVs);
void get_lep_TLV(vdouble lep, TLorentzVector &lep_tlv);
void vect_of_tagged_TLVs(vvdouble jets, vdouble jetCSV, vecTLorentzVector &vect_of_btag_TLVs);
void vect_of_untagged_TLVs(vvdouble jets, vdouble jetCSV, vecTLorentzVector &vect_of_nontagged_TLVs);
void get_bs_from_reverse_engineered_bhm(vvdouble jets, vdouble jetCSV, double best_higgs_mass, TLorentzVector &bjet1, TLorentzVector &bjet2); // <-- tempory workaround .. should be depreciated now

// algos:
double w_mass_top(vvdouble jets, vdouble jetCSV);
double had_top_mass(vvdouble jets, vdouble jetCSV);

double ww_mass_nontop(vvdouble jets, vdouble jetCSV);
void nontop_untags(vvdouble jets, vdouble jetCSV, vecTLorentzVector &vect_of_nontagged_TLVs_not_top);
double avg_pt_nontop_untags(vvdouble jets, vdouble jetCSV);

double pt_E_ratio_jets(vvdouble jets);

//void two_tagged_TLVs_from_bhm(TLorentzVector &firstb,TLorentzVector &secondb,int syst=0);
double getBestHiggsMass(TLorentzVector lepton, TLorentzVector met, vecTLorentzVector jets, vdouble btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2);
double getBestHiggsMass2(TLorentzVector lepton, TLorentzVector &met, vecTLorentzVector jets, vdouble btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, TLorentzVector &toplep, TLorentzVector &tophad);
double get_bhm_from_own_algo(double MET, double METphi, TLorentzVector &metv, vdouble lep, vvdouble jets, vdouble csv, double &minChi, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, double &dRbb);
double study_tops_bb_syst (double MET, double METphi, TLorentzVector &metv, vdouble lep, vvdouble jets, vdouble csv, double &minChi, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, double &dRbb, double &testquant1, double &testquant2, double &testquant3, double &testquant4, double &testquant5, double &testquant6, double &testquant7, TLorentzVector &b1, TLorentzVector &b2);

double get_jet_jet_etamax (vvdouble jets);
double get_jet_tag_etamax (vvdouble jets, vdouble jetCSV);
double get_tag_tag_etamax (vvdouble jets, vdouble jetCSV);
double get_median_bb_mass (vvdouble jets, vdouble jetCSV);
double get_avg_abs_deta_jets (vvdouble jets);

double getBestHiggsMass3(vdouble lep, double MET, double METphi, vvdouble jets, vint jet_genId, vint jet_genParentId, int lepton_genParentId, vdouble CSV, vdouble &CSV_sorted, double &leptopmass, double &hadtopmass, double &lepWmass, double &hadWmass,  double &leptop_min_chi2, double &hadtop_min_chi2, double &lepW_min_chi2, double &hadW_min_chi2, double &leptop_lepW_min_chi2, double &higgs_min_chi2, double &total_min_chi2, TLorentzVector &new_MET_TLV, vdouble &vect_of_minchis, double &picked_1_CSV, double &picked_2_CSV, int &picked_1_rank_by_CSV, int &picked_2_rank_by_CSV );
double find_localmin_algo(bool verbose, TLorentzVector lepton, TLorentzVector met, TLorentzVector b_TLV, double &met_pz);
double minchi2_leptop_lepW_given_metpz(TLorentzVector lepton, TLorentzVector met, TLorentzVector b_TLV, double pz);

void get_sorted_phi_wrt_lep_TLVs(vdouble &newPhi_vect, vdouble &newPt_vect, vvdouble jets, vdouble lep, double met, double metphi, bool usemet=true);
void get_vect_of_jet_dRs_sorted_by_dR(vvdouble jets, vdouble &sorted_dRs, vdouble &masses, vdouble CSVvect, vdouble &avg_CSV, vdouble lepton);

double get_response_of_gauss_thing(double eig0, double eig1, double eig2);
double get_response_of_gauss_thing_weighted(double eig0, double eig1, double eig2);

void make_the_tree(TString newfile = "newfile.root", TString af1 = "readfile.root", bool isttjets = true);


// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void maketree()
{
	// input: "yggdrasil"-formatted lepton+jets analysis trees. Uses BEANeventVars class.
	
	TString files = "/settebello/users/puigh/samples/53xOn53x/yggdrasil/v22_newBEANs_noCharmCSVSF/yggdrasil_treeMaker_mc_TTH_Inclusive_M_125_8TeV_53xOn53x*.root";					
	TString files2a = "/settebello/users/puigh/samples/53xOn53x/yggdrasil/v22_newBEANs_noCharmCSVSF/yggdrasil_treeMaker_mc_TTJetsB_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x*.root";
	TString files2b = "/settebello/users/puigh/samples/53xOn53x/yggdrasil/v22_newBEANs_noCharmCSVSF/yggdrasil_treeMaker_mc_TTJetsBB_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x*.root";
	TString files3 = "/settebello/users/puigh/samples/53xOn53x/yggdrasil/v22_newBEANs_noCharmCSVSF/yggdrasil_treeMaker_mc_TTJetsCC_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x*.root";
	TString files4 = "/settebello/users/puigh/samples/53xOn53x/yggdrasil/v22_newBEANs_noCharmCSVSF/yggdrasil_treeMaker_mc_TTJetsLF_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x*.root";
	
	// output: flat trees to use in TMVA training.
	
	make_the_tree("ttH_test_.root",files,false);		cout << "ttH done" << endl;
	make_the_tree("ttb_test.root",files2a,true);	cout << "tt1b done" << endl;
	make_the_tree("ttbb_test.root",files2b,true);	cout << "tt2b done" << endl;
	make_the_tree("ttcc_test.root",files3,true);	cout << "ttcc done" << endl;
	make_the_tree("ttlf_test.root",files4,true);		cout << "ttlf done" << endl;
	
}

void make_the_tree(TString newfile, TString af1, bool isttjets)
{


	// load yggdrasil tree

	TString tname = "worldTree";
	TChain ch1(tname);
	ch1.Add(af1);

	eventVars *eve1=0;
	ch1.SetBranchAddress( "eve.", &eve1 );
	
	
	// open new file + declarations	

	TFile *histfile = new TFile(newfile,"RECREATE","testfile");
	
	TTree* tree_of_knowledge = new TTree("tree_of_knowledge", "tree_of_knowledge");
	
	TMVA::Reader *reader_6j4t = new TMVA::Reader("!Color");
	TMVA::Reader *reader_5j4t = new TMVA::Reader("!Color");
	TMVA::Reader *reader_6j3t = new TMVA::Reader("!Color");
	
	
	int is_three_tag_newtree = 0;
	
	int numJets_newtree = 0;
	int numTags_newtree = 0;
	
	double weight_newtree = 0.;
	
	double tight_lepton_pt_newtree = 0.;
	
	double avg_dr_tagged_jets_newtree = 0.;
	double sphericity_newtree = 0.;
	double dr_between_lep_and_closest_jet_newtree = 0.;
	double avg_btag_disc_btags_newtree = 0.;
	double dev_from_avg_disc_btags_newtree = 0.;
	double h2_newtree = 0.;
	double lowest_btag_newtree = 0.;
	double avg_untagged_dijet_mass_newtree = 0.;
	double closest_tagged_dijet_mass_newtree = 0.;
	double h3_newtree = 0.;
	double h4_newtree = 0.;
	double second_highest_btag_newtree = 0.;
	double third_highest_btag_newtree = 0.;
	double fourth_highest_btag_newtree = 0.;
	
	double avg_btag_disc_non_btags_newtree = 0.;
	double tagged_dijet_mass_closest_to_125_newtree = 0.;
	
	
	double invariant_mass_of_everything_newtree = 0.;
	double best_higgs_mass_newtree = 0.;
	double minChi2_newtree = 0.;
	double dRbb_newtree = 0.;

	double bhmv2_newtree = 0.;
	double maxeta_jet_jet_newtree = 0.;
	double maxeta_jet_tag_newtree = 0.;
	double maxeta_tag_tag_newtree = 0.;
	double dEta_leptop_bb_newtree = 0.;
	double dEta_hadtop_bb_newtree = 0.;
	double dPhi_fn_newtree = 0.;
	double dEta_fn_newtree = 0.;
	double avg_eta_tops_newtree = 0.;
	double bb_eta_newtree = 0.;
	double angle_tops_bb_newtree = 0.;
	
	double dRbb_bhmv2_newtree = 0.;
//	double bb_eta_newtree = 0.;
	double b1_eta_newtree = 0.;
	double b2_eta_newtree = 0.;
	double b1_pt_newtree = 0.;
	double b2_pt_newtree = 0.;
	
	double median_bb_mass_newtree = 0.;
	double min_dr_tagged_jets_newtree = 0.;
	
	double pt_all_jets_over_E_all_jets_newtree = 0.;
	
	
	double aplanarity_newtree = 0.;
	double MET_newtree = 0.;
	double MET_phi_newtree = 0.;
	double first_jet_pt_newtree = 0.;
	double second_jet_pt_newtree = 0.;
	double third_jet_pt_newtree = 0.;
	double fourth_jet_pt_newtree = 0.;
	double h0_newtree = 0.;
	double h1_newtree = 0.;
	double all_sum_pt_with_met_newtree = 0.;
	double HT_newtree = 0.;
	double MHT_newtree = 0.;
	double MHT_phi_newtree = 0.;
	
	double jet_tag_1_pt_newtree = 0.;
	double jet_tag_2_pt_newtree = 0.;
	double jet_tag_3_pt_newtree = 0.;
	double jet_tag_4_pt_newtree = 0.;

	double avg_tagged_dijet_mass_newtree = 0.;
	double tight_lepton_phi_newtree = 0.;
	double M3_newtree = 0.;
	double M3_1tag_newtree = 0.;
	double Mlb_newtree = 0.;
	double first_highest_btag_newtree = 0.;
	
	double avg_abs_deta_jets_newtree = 0.;
	
	double ttbb_tth_MVA_output_6j4t_newtree = 0.;
	double ttbb_tth_MVA_output_5j4t_newtree = 0.;
	double ttbb_tth_MVA_output_6j3t_newtree = 0.;

	double bhmv3_newtree = 0.;

	float var1 = 0.;
	float var2 = 0.;
	float var3 = 0.;
	float var4 = 0.;
	float var5 = 0.;
	float var6 = 0.;
	float var7 = 0.;
	float var8 = 0.;
	float var9 = 0.;
	float var10= 0.;
	float var11= 0.;
	float var12= 0.;
	float var13= 0.;
	float var14= 0.;
	float var15= 0.;
	float var16= 0.;
	float var17= 0.;
	float var18= 0.;
	float var19= 0.;
	float var20= 0.;
	float var21= 0.;
	float var22= 0.;
	float var23= 0.;
	
	float var24= 0.;
	float var25= 0.;
	float var26= 0.;
	float var27= 0.;
	float var28= 0.;
	float var29= 0.;
	float var30= 0.;
	float var31= 0.;
	float var32= 0.;
	float var33= 0.;
	float var34= 0.;
	float var35= 0.;
	float var36= 0.;
	float var37= 0.;
	float var38= 0.;
	float var39= 0.;
	float var40= 0.;
	

	
	// add vars to readers:
		
	// 6j4t ttbb/ttH reader:
									
	reader_6j4t->AddVariable( "avg_dr_tagged_jets", &var1);		
	reader_6j4t->AddVariable( "sphericity", &var2);			
	reader_6j4t->AddVariable( "avg_btag_disc_btags", &var25);		
	reader_6j4t->AddVariable( "closest_tagged_dijet_mass", &var22);	
	reader_6j4t->AddVariable( "h3", &var4);				
	reader_6j4t->AddVariable( "invariant_mass_of_everything", &var23);	
	reader_6j4t->AddVariable( "best_higgs_mass", &var6); 		
	reader_6j4t->AddVariable( "dRbb", &var7);  				
	reader_6j4t->AddVariable( "maxeta_jet_tag", &var9);			
	reader_6j4t->AddVariable( "maxeta_tag_tag", &var10);			
	reader_6j4t->AddVariable( "abs(dEta_leptop_bb)", &var11);		
	reader_6j4t->AddVariable( "abs(dEta_hadtop_bb)", &var12);		
	reader_6j4t->AddVariable( "dEta_fn", &var13); 			
	reader_6j4t->AddVariable( "median_bb_mass", &var16);			
	reader_6j4t->AddVariable( "pt_all_jets_over_E_all_jets", &var17);	
	
	// location of 6j4t ttbb/ttH weights file: 
	reader_6j4t->BookMVA( "BDT method", "tmva/lj_weights/640203/TMVAClassification_BDT.weights.xml");
	

	// 5j4t ttbb/ttH reader:
	
	reader_5j4t->AddVariable( "avg_dr_tagged_jets", &var1);
	reader_5j4t->AddVariable( "maxeta_tag_tag", &var10);
	reader_5j4t->AddVariable( "h1", &var20);
	reader_5j4t->AddVariable( "h3", &var4);
	reader_5j4t->AddVariable( "all_sum_pt_with_met", &var24);
	reader_5j4t->AddVariable( "tagged_dijet_mass_closest_to_125", &var35);
	reader_5j4t->AddVariable( "MET", &var37);
	reader_5j4t->AddVariable( "aplanarity", &var18);
	reader_5j4t->AddVariable( "pt_all_jets_over_E_all_jets", &var17);
	reader_5j4t->AddVariable( "fourth_highest_btag", &var30);
	
	// location of 5j4t ttbb/ttH weights file: 
	reader_5j4t->BookMVA( "BDT method", "tmva/lj_weights/540000/TMVAClassification_BDT.weights.xml");
	
	
	
	// 6j3t ttbb/ttH reader:

	reader_6j3t->AddVariable( "sphericity", &var2);
	reader_6j3t->AddVariable( "h3", &var4);
	reader_6j3t->AddVariable( "maxeta_jet_jet", &var31);
	reader_6j3t->AddVariable( "maxeta_jet_tag", &var9);
	reader_6j3t->AddVariable( "maxeta_tag_tag", &var10);
	reader_6j3t->AddVariable( "abs(dEta_leptop_bb)", &var11);
	reader_6j3t->AddVariable( "abs(dEta_hadtop_bb)", &var12);
	reader_6j3t->AddVariable( "dEta_fn", &var13);
	reader_6j3t->AddVariable( "M3", &var33);
	reader_6j3t->AddVariable( "min_dr_tagged_jets", &var34);
	reader_6j3t->AddVariable( "pt_all_jets_over_E_all_jets", &var17);
	reader_6j3t->AddVariable( "aplanarity", &var18);
	reader_6j3t->AddVariable( "h1", &var20);
	reader_6j3t->AddVariable( "tagged_dijet_mass_closest_to_125", &var35);
	reader_6j3t->AddVariable( "third_jet_pt", &var40);
	
	// location of 6j3t ttbb/ttH weights file: 
	reader_6j3t->BookMVA( "BDT method", "tmva/lj_weights/630000/TMVAClassification_BDT.weights.xml");




	// add branches to new tree:
	
	tree_of_knowledge->Branch("numJets", &numJets_newtree, "numJets/I");
	tree_of_knowledge->Branch("numTags", &numTags_newtree, "numTags/I");
	
	tree_of_knowledge->Branch("weight", &weight_newtree, "weight/D");
	
	tree_of_knowledge->Branch("tight_lepton_pt", &tight_lepton_pt_newtree, "tight_lepton_pt/D");
	tree_of_knowledge->Branch("aplanarity", &aplanarity_newtree,"aplanarity/D");
	tree_of_knowledge->Branch("MET", &MET_newtree,"MET/D");
	tree_of_knowledge->Branch("MET_phi", &MET_phi_newtree,"MET_phi/D");
	tree_of_knowledge->Branch("first_jet_pt", &first_jet_pt_newtree,"first_jet_pt/D");
	tree_of_knowledge->Branch("second_jet_pt",&second_jet_pt_newtree,"second_jet_pt/D");
	tree_of_knowledge->Branch("third_jet_pt",&third_jet_pt_newtree,"third_jet_pt/D");
	tree_of_knowledge->Branch("fourth_jet_pt",&fourth_jet_pt_newtree,"fourth_jet_pt/D");
	tree_of_knowledge->Branch("h0",&h0_newtree,"h0/D");
	tree_of_knowledge->Branch("h1",&h1_newtree,"h1/D");
	tree_of_knowledge->Branch("all_sum_pt_with_met",&all_sum_pt_with_met_newtree,"all_sum_pt_with_met/D");
	tree_of_knowledge->Branch("HT",&HT_newtree,"HT/D");
	tree_of_knowledge->Branch("MHT",&MHT_newtree,"MHT/D");
	tree_of_knowledge->Branch("MHT_phi",&MHT_phi_newtree,"MHT_phi/D");
	tree_of_knowledge->Branch("jet_tag_1_pt",&jet_tag_1_pt_newtree,"jet_tag_1_pt/D");
	tree_of_knowledge->Branch("jet_tag_2_pt",&jet_tag_2_pt_newtree,"jet_tag_2_pt/D");
	tree_of_knowledge->Branch("jet_tag_3_pt",&jet_tag_3_pt_newtree,"jet_tag_3_pt/D");
	tree_of_knowledge->Branch("jet_tag_4_pt",&jet_tag_4_pt_newtree,"jet_tag_4_pt/D");	
	
	tree_of_knowledge->Branch("avg_dr_tagged_jets", &avg_dr_tagged_jets_newtree, "avg_dr_tagged_jets/D");
	tree_of_knowledge->Branch("sphericity", &sphericity_newtree, "sphericity/D");
	tree_of_knowledge->Branch("dr_between_lep_and_closest_jet", &dr_between_lep_and_closest_jet_newtree, "dr_between_lep_and_closest_jet/D");
	tree_of_knowledge->Branch("avg_btag_disc_btags", &avg_btag_disc_btags_newtree, "avg_btag_disc_btags/D");
	tree_of_knowledge->Branch("dev_from_avg_disc_btags", &dev_from_avg_disc_btags_newtree, "dev_from_avg_disc_btags/D");
	tree_of_knowledge->Branch("h2", &h2_newtree, "h2/D");
	tree_of_knowledge->Branch("lowest_btag", &lowest_btag_newtree, "lowest_btag/D");
	tree_of_knowledge->Branch("avg_untagged_dijet_mass", &avg_untagged_dijet_mass_newtree, "avg_untagged_dijet_mass/D");
	tree_of_knowledge->Branch("closest_tagged_dijet_mass", &closest_tagged_dijet_mass_newtree, "closest_tagged_dijet_mass/D");
	tree_of_knowledge->Branch("h3", &h3_newtree, "h3/D");
	tree_of_knowledge->Branch("h4", &h4_newtree, "h4/D");
	tree_of_knowledge->Branch("second_highest_btag", &second_highest_btag_newtree, "second_highest_btag/D");
	tree_of_knowledge->Branch("third_highest_btag", &third_highest_btag_newtree, "third_highest_btag/D");
	tree_of_knowledge->Branch("fourth_highest_btag", &fourth_highest_btag_newtree, "fourth_highest_btag/D");
	tree_of_knowledge->Branch("invariant_mass_of_everything", &invariant_mass_of_everything_newtree, "invariant_mass_of_everything/D");
	tree_of_knowledge->Branch("best_higgs_mass", &best_higgs_mass_newtree, "best_higgs_mass/D");
	tree_of_knowledge->Branch("minChi2", &minChi2_newtree, "minChi2/D");
	tree_of_knowledge->Branch("dRbb", &dRbb_newtree, "dRbb/D");
	tree_of_knowledge->Branch("bhmv2", &bhmv2_newtree, "bhmv2/D");
	tree_of_knowledge->Branch("maxeta_jet_jet", &maxeta_jet_jet_newtree, "maxeta_jet_jet/D");
	tree_of_knowledge->Branch("maxeta_jet_tag", &maxeta_jet_tag_newtree, "maxeta_jet_tag/D");
	tree_of_knowledge->Branch("maxeta_tag_tag", &maxeta_tag_tag_newtree, "maxeta_tag_tag/D");
	tree_of_knowledge->Branch("avg_abs_deta_jets", &avg_abs_deta_jets_newtree, "avg_abs_deta_jets/D");
	tree_of_knowledge->Branch("dEta_leptop_bb", &dEta_leptop_bb_newtree, "dEta_leptop_bb/D");
	tree_of_knowledge->Branch("dEta_hadtop_bb", &dEta_hadtop_bb_newtree, "dEta_hadtop_bb/D");
	tree_of_knowledge->Branch("dPhi_fn", &dPhi_fn_newtree, "dPhi_fn/D");
	tree_of_knowledge->Branch("dEta_fn", &dEta_fn_newtree, "dEta_fn/D");
	tree_of_knowledge->Branch("avg_eta_tops", &avg_eta_tops_newtree, "avg_eta_tops/D");
	tree_of_knowledge->Branch("bb_eta", &bb_eta_newtree, "bb_eta/D");
	tree_of_knowledge->Branch("angle_tops_bb", &angle_tops_bb_newtree, "angle_tops_bb/D");

	tree_of_knowledge->Branch("avg_btag_disc_non_btags", &avg_btag_disc_non_btags_newtree, "avg_btag_disc_non_btags/D");
	tree_of_knowledge->Branch("tagged_dijet_mass_closest_to_125", &tagged_dijet_mass_closest_to_125_newtree, "tagged_dijet_mass_closest_to_125/D");
	tree_of_knowledge->Branch("avg_tagged_dijet_mass", &avg_tagged_dijet_mass_newtree, "avg_tagged_dijet_mass/D");
	tree_of_knowledge->Branch("tight_lepton_phi", &tight_lepton_phi_newtree, "tight_lepton_phi/D");
	tree_of_knowledge->Branch("M3", &M3_newtree, "M3/D");
	tree_of_knowledge->Branch("M3_1tag", &M3_1tag_newtree, "M3_1tag/D");
	tree_of_knowledge->Branch("Mlb", &Mlb_newtree, "Mlb/D");
	tree_of_knowledge->Branch("first_highest_btag", &first_highest_btag_newtree, "first_highest_btag/D");
	
	tree_of_knowledge->Branch("dRbb_bhmv2", &dRbb_bhmv2_newtree, "dRbb_bhmv2/D");
	tree_of_knowledge->Branch("b1_eta_bhmv2",&b1_eta_newtree, "b1_eta_bhmv2/D");
	tree_of_knowledge->Branch("b2_eta_bhmv2",&b2_eta_newtree, "b2_eta_bhmv2/D");
	tree_of_knowledge->Branch("b1_pt_bhmv2",&b1_pt_newtree, "b1_pt_bhmv2/D");
	tree_of_knowledge->Branch("b2_pt_bhmv2",&b2_pt_newtree, "b2_pt_bhmv2/D");
	
	tree_of_knowledge->Branch("median_bb_mass",&median_bb_mass_newtree, "median_bb_mass/D");
	tree_of_knowledge->Branch("min_dr_tagged_jets",&min_dr_tagged_jets_newtree, "min_dr_tagged_jets/D");
	
	tree_of_knowledge->Branch("pt_all_jets_over_E_all_jets",&pt_all_jets_over_E_all_jets_newtree, "pt_all_jets_over_E_all_jets/D");
	
	tree_of_knowledge->Branch("ttbb_tth_MVA_output_6j4t", &ttbb_tth_MVA_output_6j4t_newtree, "ttbb_tth_MVA_output_6j4t/D");
	tree_of_knowledge->Branch("ttbb_tth_MVA_output_5j4t", &ttbb_tth_MVA_output_5j4t_newtree, "ttbb_tth_MVA_output_5j4t/D");
	tree_of_knowledge->Branch("ttbb_tth_MVA_output_6j3t", &ttbb_tth_MVA_output_6j3t_newtree, "ttbb_tth_MVA_output_6j3t/D");
	
	tree_of_knowledge->Branch("bhmv3", &bhmv3_newtree, "bhmv3/D");
	
		
											    											       
	int count = 0;									       
	int syst = 0; // no systematics!
	
	
	for (Int_t i=0;i<ch1.GetEntries();i++)
	{
		// get entry from input tree:
		ch1.GetEntry(i);
				
		// choose subset of events:
		
		bool jetsel = eve1->numJets_float_[syst]>=4; // (choose smaller subset if desired)
		bool tagsel = eve1->numTags_float_[syst]>=2; // (choose smaller subset if desired)
		
		bool is_sel_cat = jetsel&&tagsel;
		if (!is_sel_cat) continue;
		
		//bool pass_ele_trigger = eve1->passElectronTrigger_ && eve1->matchSingleElectronTrigger_; // selection used in ana
		//bool pass_mu_trigger = eve1->passMuonTrigger_ && eve1->matchSingleMuTrigger_; // selection used in ana
		bool pass_ele_trigger = eve1->passElectronTrigger_; // more stats for training
		bool pass_mu_trigger = eve1->passMuonTrigger_; // more stats for training
		
		if (eve1->leptonType_==0) if (!pass_ele_trigger) continue;
		if (eve1->leptonType_==1) if (!pass_mu_trigger) continue;
		//if (eve1->IsTauTauLeptonEvent_[syst]==1) continue; // selection used in ana // comment out for more training stats
		
		///////////////////
		
				
		// associate variables from input tree to new tree branches:
		
		numJets_newtree = eve1->numJets_float_[syst];
		numTags_newtree = eve1->numTags_float_[syst];
		
		weight_newtree = eve1->wgt_[syst];

		tight_lepton_pt_newtree = eve1->tight_lepton_pt_;
		
		aplanarity_newtree = eve1->aplanarity_[syst];
		MET_newtree = eve1->MET_[syst];
		MET_phi_newtree = eve1->MET_phi_[syst];
		first_jet_pt_newtree = eve1->first_jet_pt_[syst];
		second_jet_pt_newtree = eve1->second_jet_pt_[syst];
		third_jet_pt_newtree = eve1->third_jet_pt_[syst];
		fourth_jet_pt_newtree = eve1->fourth_jet_pt_[syst];
		h0_newtree = eve1->h0_[syst];
		h1_newtree = eve1->h1_[syst];
		all_sum_pt_with_met_newtree = eve1->all_sum_pt_with_met_[syst];
		HT_newtree = eve1->HT_[syst];
		MHT_newtree = eve1->MHT_[syst];
		MHT_phi_newtree = eve1->MHT_phi_[syst];

		jet_tag_1_pt_newtree = eve1->jet_tag_1_pt_[syst];
		jet_tag_2_pt_newtree = eve1->jet_tag_2_pt_[syst];
		jet_tag_3_pt_newtree = eve1->jet_tag_3_pt_[syst];
		jet_tag_4_pt_newtree = eve1->jet_tag_4_pt_[syst];


		avg_dr_tagged_jets_newtree = eve1->avg_dr_tagged_jets_[syst];
		sphericity_newtree = eve1->sphericity_[syst];
		dr_between_lep_and_closest_jet_newtree = eve1->dr_between_lep_and_closest_jet_[syst];
		avg_btag_disc_btags_newtree = eve1->avg_btag_disc_btags_[syst];
		dev_from_avg_disc_btags_newtree = eve1->dev_from_avg_disc_btags_[syst];
		h2_newtree = eve1->h2_[syst];
		lowest_btag_newtree = eve1->lowest_btag_[syst];
		avg_untagged_dijet_mass_newtree = eve1->avg_untagged_dijet_mass_[syst];
		closest_tagged_dijet_mass_newtree = eve1->closest_tagged_dijet_mass_[syst];
		h3_newtree = eve1->h3_[syst];
		h4_newtree = eve1->h4_[syst];
		second_highest_btag_newtree = eve1->second_highest_btag_[syst];
		third_highest_btag_newtree = eve1->third_highest_btag_[syst];
		fourth_highest_btag_newtree = eve1->fourth_highest_btag_[syst];
		invariant_mass_of_everything_newtree = eve1->invariant_mass_of_everything_[syst];
		best_higgs_mass_newtree = eve1->best_higgs_mass_[syst];
		minChi2_newtree = eve1->minChi2_[syst];
		dRbb_newtree = eve1->dRbb_[syst];
		min_dr_tagged_jets_newtree = eve1->min_dr_tagged_jets_[syst];

		avg_btag_disc_non_btags_newtree = eve1->avg_btag_disc_non_btags_[syst];
		tagged_dijet_mass_closest_to_125_newtree = eve1->tagged_dijet_mass_closest_to_125_[syst];

		avg_tagged_dijet_mass_newtree = eve1->avg_tagged_dijet_mass_[syst];
		tight_lepton_phi_newtree = eve1->tight_lepton_phi_;
		M3_newtree = eve1->M3_[syst];
		M3_1tag_newtree = eve1->M3_1tag_[syst];
		Mlb_newtree = eve1->Mlb_[syst];
		first_highest_btag_newtree = eve1->first_highest_btag_[syst];

		
		// get additional variables:
		
		// --- bhmv2 ---
		TLorentzVector newmet; // might want to compare to old met...
		double chi2lepW, chi2leptop, chi2hadW, chi2hadtop;
		double mass_lepW, mass_leptop, mass_hadW, mass_hadtop;
		double drbb_dummy;
		double minChi;
		double testquant1, testquant2, testquant3, testquant4, testquant5, testquant6, testquant7;
		TLorentzVector b1;
		TLorentzVector b2;
		double bhm = study_tops_bb_syst(eve1->MET_[syst], eve1->MET_phi_[syst], newmet, eve1->lepton_TLV_[syst], eve1->jet_vect_TLV_[syst], eve1->jet_CSV_[syst], minChi, chi2lepW, chi2leptop, chi2hadW, chi2hadtop, mass_lepW, mass_leptop, mass_hadW, mass_hadtop, drbb_dummy, testquant1, testquant2, testquant3, testquant4, testquant5, testquant6, testquant7, b1, b2);
		double etamax_v1 = get_jet_jet_etamax (eve1->jet_vect_TLV_[syst]);
		double etamax_v2 = get_jet_tag_etamax (eve1->jet_vect_TLV_[syst],eve1->jet_CSV_[syst]);
		double etamax_v3 = get_tag_tag_etamax (eve1->jet_vect_TLV_[syst],eve1->jet_CSV_[syst]);		
		
		bhmv2_newtree = bhm;


		// --- bhmv3 ---
		double leptopmass = 0.;
		double hadtopmass = 0.;
		double lepWmass = 0.;
		double hadWmass = 0.;
		double leptop_min_chi2;
		double hadtop_min_chi2;
		double lepW_min_chi2;
		double hadW_min_chi2;
		double leptop_lepW_min_chi2;
		double higgs_min_chi2;
		double total_min_chi2;
		double CSV1;
		double CSV2;
		int CSVrank1, CSVrank2;
		vdouble sortedCSVs;
		TLorentzVector new_MET;
		vdouble theminchis;
		theminchis.clear();
		double mass = getBestHiggsMass3(eve1->lepton_TLV_[syst], eve1->MET_[syst], eve1->MET_phi_[syst], eve1->jet_vect_TLV_[syst], eve1->jet_genId_[syst], eve1->jet_genParentId_[syst], eve1->lepton_genParentId_[syst], eve1->jet_CSV_[syst], sortedCSVs, leptopmass, hadtopmass, lepWmass, hadWmass, leptop_min_chi2, hadtop_min_chi2, lepW_min_chi2, hadW_min_chi2, leptop_lepW_min_chi2, higgs_min_chi2, total_min_chi2, new_MET, theminchis, CSV1, CSV2, CSVrank1, CSVrank2);
		
		bhmv3_newtree = mass;

		maxeta_jet_jet_newtree = etamax_v1;
		maxeta_jet_tag_newtree = etamax_v2;
		maxeta_tag_tag_newtree = etamax_v3;

		dEta_leptop_bb_newtree = testquant1;
		dEta_hadtop_bb_newtree = testquant2;

		dPhi_fn_newtree = testquant3;
		dEta_fn_newtree = testquant6;

		avg_eta_tops_newtree = testquant5;

		bb_eta_newtree = testquant4;

		angle_tops_bb_newtree = testquant7;
				
		dRbb_bhmv2_newtree = drbb_dummy;
		b1_eta_newtree = b1.Eta();
		b2_eta_newtree = b2.Eta();
		b1_pt_newtree = b1.Perp();
		b2_pt_newtree = b2.Perp();
		
		median_bb_mass_newtree = get_median_bb_mass(eve1->jet_vect_TLV_[syst],eve1->jet_CSV_[syst]);		
		
		if (eve1->numTags_float_[syst]>3) is_three_tag_newtree = 0;
		if (eve1->numTags_float_[syst]==3) is_three_tag_newtree = 1;
		
		pt_all_jets_over_E_all_jets_newtree = pt_E_ratio_jets(eve1->jet_vect_TLV_[syst]);
		
		avg_abs_deta_jets_newtree = get_avg_abs_deta_jets(eve1->jet_vect_TLV_[syst]);
		
		
		/// ------ reader vars -------
		
		var1 = avg_dr_tagged_jets_newtree;		
		var2 = sphericity_newtree;			
		var3 = avg_untagged_dijet_mass_newtree;		
		var4 = h3_newtree;		  		
		var5 = second_highest_btag_newtree;		
		var6 = best_higgs_mass_newtree;
		var7 = dRbb_newtree;
		var8 = bhmv2_newtree;		  
		var9 = maxeta_jet_tag_newtree;			
		var10 = maxeta_tag_tag_newtree;			
		var11 = abs(dEta_leptop_bb_newtree);
		var12 = abs(dEta_hadtop_bb_newtree);
		var13 = dEta_fn_newtree;
		var15 = angle_tops_bb_newtree;
		var16 = median_bb_mass_newtree;			
		var17 = pt_all_jets_over_E_all_jets_newtree;	
		var18 = aplanarity_newtree;			
		var19 = second_jet_pt_newtree;			
		var20 = h1_newtree;				
		var21 = MHT_newtree;				
		var22 = closest_tagged_dijet_mass_newtree;
		var23 = invariant_mass_of_everything_newtree;
		var24 = all_sum_pt_with_met_newtree;
		var25 = avg_btag_disc_btags_newtree;
		var26 = dev_from_avg_disc_btags_newtree;
		var27 = lowest_btag_newtree;
		var28 = third_highest_btag_newtree;
		var29 = h0_newtree;
		var30 = fourth_highest_btag_newtree;
		var31 = maxeta_jet_jet_newtree;
		var32 = first_highest_btag_newtree;
		var33 = M3_newtree;
		var34 = min_dr_tagged_jets_newtree;
		var35 = tagged_dijet_mass_closest_to_125_newtree;
		var36 = HT_newtree;
		var37 = MET_newtree;
		var38 = fourth_jet_pt_newtree;
		var39 = h2_newtree;
		var40 = third_jet_pt_newtree;
		
		/// ---------------------------
		
		
		// evaluate ttbb/ttH BDTs to save as variables in new tree:
		
		ttbb_tth_MVA_output_6j4t_newtree = reader_6j4t->EvaluateMVA( "BDT method" );
		ttbb_tth_MVA_output_5j4t_newtree = reader_5j4t->EvaluateMVA( "BDT method" );
		ttbb_tth_MVA_output_6j3t_newtree = reader_6j3t->EvaluateMVA( "BDT method" );
		
		
		// add entry to new tree:

		tree_of_knowledge->Fill();
		
		count++;
	}
	   
   	histfile->Write();
   	histfile->Close();

	// garbage collection
	delete reader_6j4t;
	delete reader_6j3t;
	delete reader_5j4t;
}

// end of make_the_tree

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




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


void get_bs_from_reverse_engineered_bhm(vvdouble jets, vdouble jetCSV, double best_higgs_mass, TLorentzVector &bjet1, TLorentzVector &bjet2)
{

	// this is temporary due to accidentally leaving out some info in trees
	
	double testmass = 0.;
	
	vecTLorentzVector btag_TLV_vect;
		
	vect_of_tagged_TLVs(jets, jetCSV, btag_TLV_vect);
	
	for (int i=0; i<btag_TLV_vect.size(); i++)
	{
		for (int j=0; j<btag_TLV_vect.size(); j++)
		{
			if (i!=j)
			{
				TLorentzVector tempvect = btag_TLV_vect[i]+btag_TLV_vect[j];
				testmass = tempvect.M();
				if (testmass==best_higgs_mass)
				{
					bjet1 = btag_TLV_vect[i];
					bjet2 = btag_TLV_vect[j];
					break;
				}
			}
		}		
		if (testmass==best_higgs_mass) break;
	}
}



double getBestHiggsMass(TLorentzVector lepton, TLorentzVector met, vecTLorentzVector jets, vdouble btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2)
{

  int nJets = int(jets.size());

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
  double sigma_lepTop = 32.91;

  // //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttH
  // double sigma_hadW   = 12.59;
  // double sigma_hadTop = 19.9;
  // double sigma_lepTop = 39.05;

  //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttJets
  /*double sigma_hadW		= 12.72,
    sigma_hadTop	= 18.12,
    sigma_lepTop	= 38.72;
  */

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
    if( nJets>=6 && nBtags>=4 ){
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
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  return higgs_mass_high_energy;
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
  int ind_fourth_highest = 999;

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








/*
void two_tagged_TLVs_from_bhm(TLorentzVector &firstb,TLorentzVector &secondb,int syst=0)
{
	firstb.SetPxPyPzE(eve1->minChi2_bjet1_px_[syst],eve1->minChi2_bjet1_py_[syst],eve1->minChi2_bjet1_pz_[syst],eve1->minChi2_bjet1_E_[syst]);
	secondb.SetPxPyPzE(eve1->minChi2_bjet2_px_[syst],eve1->minChi2_bjet2_py_[syst],eve1->minChi2_bjet2_pz_[syst],eve1->minChi2_bjet2_E_[syst]);
	
}
*/

double get_jet_jet_etamax (vvdouble jets)
{
	vecTLorentzVector thejets;
	convert_jets_to_TLVs(jets, thejets);
	
	int count=0;
	double avgval=0.;
	
	for (int i=0; i<(thejets.size()-1); i++)
	{
		for (int j=i+1; j<thejets.size(); j++)
		{
				
				avgval += thejets[i].Eta()-thejets[j].Eta();
				count++;
		}
	}
	
	avgval /= count;
	
	double imax = 0.;
	double etamax=0.;
	
	for (int k=0; k<thejets.size(); k++)
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
	
	for (int i=0; i<(thejets.size()-1); i++)
	{
		for (int j=i+1; j<thejets.size(); j++)
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
	
	
	for (int k=0; k<thetags.size(); k++)
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
	
	for (int i=0; i<(thetags.size()-1); i++)
	{
		for (int j=i+1; j<thetags.size(); j++)
		{
				
				avgval += thetags[i].Eta()-thetags[j].Eta();
				count++;
		}
	}
	
	avgval /= count;
	
	double imax = 0.;
	double etamax=0.;
	
	
	for (int k=0; k<thetags.size(); k++)
	{
		imax = abs(thetags[k].Eta()-avgval);
		
		if(imax>etamax)
		{
			etamax = imax;
		}
	}

	return etamax;

}


double get_avg_abs_deta_jets (vvdouble jets)
{
	
	vecTLorentzVector thejets;
	convert_jets_to_TLVs(jets, thejets);
	
	int count=0;
	double avgval=0.;
	
	for (int i=0; i<(thejets.size()-1); i++)
	{
		for (int j=i+1; j<thejets.size(); j++)
		{
				
			avgval += fabs(thejets[i].Eta()-thejets[j].Eta());
			count++;
		}
	}
	
	if (count) avgval /= count;
	
	return avgval;
	
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
void get_lep_TLV(vdouble lep, TLorentzVector &lep_tlv)
{
	lep_tlv.SetPxPyPzE(lep[0],lep[1],lep[2],lep[3]);
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
	double H_mass=120.0;

	double sigma_hadW 	= 12.59,
		sigma_hadTop 	= 19.9,
		sigma_lepTop	= 39.05;
			 
	double sigma_H		= 15.79;
	
	
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
	double H_mass=120.0;

	double sigma_hadW 	= 12.59,
		sigma_hadTop 	= 19.9,
		sigma_lepTop	= 39.05;
			 
	double sigma_H		= 15.79;
	
	
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
	double H_mass=120.0;

	double sigma_hadW 	= 12.59,
		sigma_hadTop 	= 19.9,
		sigma_lepTop	= 39.05;
			 
	double sigma_H		= 15.79;
	
	
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
	double H_mass=120.0;

	double sigma_hadW 	= 12.59,
		sigma_hadTop 	= 19.9,
		sigma_lepTop	= 39.05;
			 
	double sigma_H		= 15.79;
	
	
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
	
	for (int i=0; i<jetvect.size(); i++)
	{
		ptsum += jetvect[i].Pt();
		Esum += jetvect[i].E();
	}
	
	ratio = ptsum / Esum;
	
	return ratio;
}

double study_tops_bb_syst (double MET, double METphi, TLorentzVector &metv, vdouble lep, vvdouble jets, vdouble csv, double &minChi, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, double &dRbb, double &testquant1, double &testquant2, double &testquant3, double &testquant4, double &testquant5, double &testquant6, double &testquant7, TLorentzVector &b1, TLorentzVector &b2)
{
	double pi = 3.14;
	
	metv.SetPtEtaPhiE(MET,0.,METphi,MET);
	TLorentzVector lepton;
	lepton.SetPxPyPzE(lep[0],lep[1],lep[2],lep[3]);
	vecTLorentzVector jet_TLVs;	
	convert_jets_to_TLVs(jets, jet_TLVs);
		
	//double minChi;
	//double dRbb;
	TLorentzVector bjet1;
	TLorentzVector bjet2;
	TLorentzVector leptop;
	TLorentzVector hadtop;
	
	
	double bhm = getBestHiggsMass2(lepton, metv, jet_TLVs, csv, minChi, dRbb, bjet1, bjet2, chi2lepW, chi2leptop, chi2hadW, chi2hadtop, mass_lepW, mass_leptop, mass_hadW, mass_hadtop, leptop, hadtop); // Jon T. version 2

	
	b1 = bjet1;
	b2 = bjet2;
	
	TLorentzVector bsyst = bjet1+bjet2;
	TLorentzVector topsyst = leptop+hadtop;
	
	//testquant1 = bsyst.Angle(leptop.Vect());
	//testquant2 = bsyst.Angle(hadtop.Vect());
	
	//testquant1 = bsyst.DeltaPhi(leptop);
	//testquant2 = bsyst.DeltaPhi(hadtop);
	
	//testquant1 = bsyst.Eta() - leptop.Eta();
	//testquant2 = bsyst.Eta() - hadtop.Eta();
	
	//testquant1 = bsyst.Eta() - hadtop.Eta();
	//testquant2 = bsyst.DeltaPhi(hadtop);
	
	
	//testquant2 = bsyst.Eta();
	
	//testquant1 = topsyst.Phi();
	//testquant2 = bsyst.Phi();
	
	
	//testquant2 = bsyst.Angle(hadtop.Vect());
	
	
	//dphi, deta stuff	
	
	
	
	testquant1 = bsyst.Eta() - leptop.Eta();	
	testquant2 = bsyst.Eta() - hadtop.Eta();
	
	
	double dphihad = bsyst.DeltaPhi(hadtop);
	double dphilep = bsyst.DeltaPhi(leptop);
	testquant3 = fabs((dphilep - pi)*(dphilep + pi)) + pow(dphihad,2);
	testquant3 = sqrt(testquant3 / (2.0*pow(pi,2)));
		
	
	testquant4 = bsyst.Eta();
	
	testquant5 = (hadtop.Eta() + leptop.Eta())/2;
	
	
	testquant6 = sqrt(abs((bsyst.Eta() - leptop.Eta())*(bsyst.Eta() - hadtop.Eta())));
	
	
	testquant7 = bsyst.Angle(topsyst.Vect());
	
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


	for (int asdf=0; asdf<(all_btags.size()-1); asdf++)
	{
		for (int j=asdf+1; j<all_btags.size(); j++)
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
double find_localmin_algo(bool verbose, TLorentzVector lepton, TLorentzVector met, TLorentzVector b_TLV, double &met_pz)
{

	// 8/7/13, Geoff Smith
	//
	// Algo to find local min using divide-by-two method.
	// Tested with quadratic functions 
	// and is only meant for quadratic functions. But, will probably work to 
	// find local min for (reasonably well-behaved) arbitrary functions, 
	// given that initial parameters are set appropriately.
	
	
	double leftpoint;
	double rightpoint;
	double midpoint;
	
	// initial parameters:
	double left_bound_guess = -10000.;	// <-- neutrino pz left bound guess -- should be > -10 TeV!	
	double right_bound_guess = 10000.;	// <-- neutrino pz right bound guess -- should be < 10 TeV!
	double tolerance = 0.01;		// <-- how many digits you want the neutrino pz to converge to (GeV)
	bool tolerance_reached = false;
	double increment = tolerance / 10.;
	
	
	
	leftpoint = left_bound_guess;
	rightpoint = right_bound_guess;
	midpoint = leftpoint + (right_bound_guess - left_bound_guess) / 2.;
	
	double initial_left_value = minchi2_leptop_lepW_given_metpz(lepton,met,b_TLV,leftpoint);
	double initial_right_value = minchi2_leptop_lepW_given_metpz(lepton,met,b_TLV,rightpoint);
	
	if (verbose) cout << "initial left bound: (" << leftpoint << "," << initial_left_value << ")" << endl;
	if (verbose) cout << "initial right bound: (" << rightpoint << "," << initial_right_value << ")" << endl;
	

	
	double leftslope = minchi2_leptop_lepW_given_metpz(lepton,met,b_TLV,leftpoint + increment) - minchi2_leptop_lepW_given_metpz(lepton,met,b_TLV,leftpoint - increment); // dy
	leftslope = leftslope / (2*increment); // dx
		
	double rightslope = minchi2_leptop_lepW_given_metpz(lepton,met,b_TLV,rightpoint + increment) - minchi2_leptop_lepW_given_metpz(lepton,met,b_TLV,rightpoint - increment); // dy
	rightslope = rightslope / (2*increment); // dx
	
	bool pos_true_leftslope = leftslope > 0.;
	bool pos_true_rightslope = rightslope > 0.;
	
	if (pos_true_leftslope==pos_true_rightslope)
	{
		cout << "try picking different bounds (either no minimum in range, or fn is not quadratic.)" << endl;
		return -99.;
	}
		
	if ( (pos_true_leftslope==true)&&(pos_true_rightslope==false) )
	{
		cout << "assuming this is a quadratic fn., it has no local min. try a different fn." << endl;
		return -99.;
	}
	
	long int debug_count = 0;
	
	
	while (!tolerance_reached)
	{
		double midslope = minchi2_leptop_lepW_given_metpz(lepton,met,b_TLV,midpoint + increment) - minchi2_leptop_lepW_given_metpz(lepton,met,b_TLV,midpoint - increment); // dy
		midslope = midslope / (2*increment); // dx
		
		bool pos_true_midslope = midslope > 0.;
		
		if (pos_true_midslope) rightpoint = midpoint;
		
		if (!pos_true_midslope) leftpoint = midpoint;
	
		midpoint = leftpoint + (rightpoint - leftpoint) / 2.;       //  <---  divide-by-2 method
	
		if ( (rightpoint-leftpoint) < tolerance ) tolerance_reached = true;
		
		debug_count++;
		if (debug_count%1000==0&&verbose) cout << leftpoint << "  " << midpoint << "  " << rightpoint << "  " << debug_count << endl;  // <--- if everything ok, usually <<1000 cycles in this loop
		//if (verbose) cout << leftpoint << "  " << midpoint << "  " << rightpoint << "  " << debug_count << endl;  // <--- if everything ok, usually <<1000 cycles in this loop	
	}
	
	double final_min_value = minchi2_leptop_lepW_given_metpz(lepton,met,b_TLV,midpoint);
	
	if (verbose) cout << "found minimum at " << midpoint << ". Value of function at that point is: " << final_min_value << endl; 
	if (verbose) cout << debug_count << endl;
	
	met_pz = midpoint;
	
	return final_min_value;
	
}

double minchi2_leptop_lepW_given_metpz(TLorentzVector lepton, TLorentzVector met, TLorentzVector b_TLV, double pz)
{
	//double minchi2_leptop_lepW = -1.;
	
	// set parameters:
	double target_wmass = 81.;
	double target_topmass = 174.;
	double sigma_w = 12.;
	double sigma_top = 17.;
	
	TLorentzVector top;
	TLorentzVector W;
	
	// calculate test leptonic W, top:	
	double met_E_sq = met.E()*met.E();
	met_E_sq = met_E_sq + (pz*pz);
	double met_E = sqrt(met_E_sq);
	met.SetPz(pz);
	met.SetE(met_E);
	W = lepton + met;
	top = W + b_TLV;
	double wmass = W.M();
	double topmass = top.M();
	
	// get minchi2:
	double chi2_W = ( (wmass - target_wmass) / sigma_w )*( (wmass - target_wmass) / sigma_w );
	double chi2_top = ( (topmass - target_topmass) / sigma_top )*( (topmass - target_topmass) / sigma_top );
	double minchi2_leptop_lepW = chi2_W + chi2_top; // chi2_W +
	return minchi2_leptop_lepW;
}
double getBestHiggsMass3(vdouble lep, double MET, double METphi, vvdouble jets, vint jet_genId, vint jet_genParentId, int lepton_genParentId, vdouble CSV, vdouble &CSV_sorted, double &leptopmass, double &hadtopmass, double &lepWmass, double &hadWmass,  double &leptop_min_chi2, double &hadtop_min_chi2, double &lepW_min_chi2, double &hadW_min_chi2, double &leptop_lepW_min_chi2, double &higgs_min_chi2, double &total_min_chi2, TLorentzVector &new_MET_TLV, vdouble &vect_of_minchis, double &picked_1_CSV, double &picked_2_CSV, int &picked_1_rank_by_CSV, int &picked_2_rank_by_CSV )
{
	
	// See "minchi2_leptop_lepW_given_metpz" for lep top, lep W parameters.
	// rest of parameters:
	const double csv_medium = 0.679;
	const double target_hadtop_mass = 176.;
	const double hadtop_sigma = 19.;
	const double target_hadW_mass = 81.; // 84
	const double hadW_sigma = 17.; // 12
	const double target_higgs_mass = 110.; // 118 // 122
	const double higgs_sigma = 10.; // 17.
	
	const double higgs_dr_avg = 1.5;
	const double higgs_dr_sigma = 0.67;
	
	
	double best_higgs_mass_v3 = 0.;
	
	const int njets = CSV.size();
	
	bool used_jets[njets];
	for (int i=0; i<njets; i++) used_jets[i] = false;
	
	// roster of objects:
	TLorentzVector lepton;
	TLorentzVector metv;
	TLorentzVector hadW;
	TLorentzVector lepW;
	TLorentzVector hadtop;
	TLorentzVector leptop;
	TLorentzVector higgs;
	TLorentzVector hadtop_b;
	TLorentzVector leptop_b;
	TLorentzVector hadW_j1;
	TLorentzVector hadW_j2;
	TLorentzVector higgs_b1;
	TLorentzVector higgs_b2;
	
	// separate tagged, untagged collections:
	vecTLorentzVector untagged_TLVs;
	vecTLorentzVector tagged_TLVs;
	vecTLorentzVector jet_TLVs;
	
	// set initial TLVs of lepton, jets, met:
	metv.SetPtEtaPhiE(MET,0.,METphi,MET);
	lepton.SetPxPyPzE(lep[0],lep[1],lep[2],lep[3]);		
	vect_of_tagged_TLVs(jets, CSV, tagged_TLVs);
	vect_of_untagged_TLVs(jets, CSV, untagged_TLVs);
	convert_jets_to_TLVs(jets, jet_TLVs);
	
	const int n_untags = untagged_TLVs.size();
	const int n_tags = tagged_TLVs.size();
	//const int njets = jet_TLVs.size();
	
	leptop_min_chi2 = 99999.;
	hadtop_min_chi2 = 99999.;
	lepW_min_chi2 = 99999.;
	hadW_min_chi2 = 99999.;
	leptop_lepW_min_chi2 = 99999.;
	total_min_chi2 = 999999.;
	//total_min_chi2 = 0.;
	
	// some more leptop params:
	double new_met_pz = -999;
	double new_met_pz_chosen = -9999.;
	bool cout_algo = false;
	
	//cout << n_untags << "  " << n_tags << endl;
	
	
	
	// "simple" algo:
	/*
	for(int i=0;i<(n_untags-1);i++)
	{
		for (int j=(i+1);j<n_untags;j++)
		{
			for (int k=0;k<n_tags;k++)
			{
				TLorentzVector hadW_j1_i = untagged_TLVs[i];
				TLorentzVector hadW_j2_j = untagged_TLVs[j];
				TLorentzVector hadW_ij = hadW_j1_i + hadW_j2_j;
				TLorentzVector hadtop_k = hadW_ij + tagged_TLVs[k];
					
				double hadW_chi2 = ((hadW_ij.M()-target_hadW_mass) / hadW_sigma)*((hadW_ij.M()-target_hadW_mass) / hadW_sigma);
				double hadtop_chi2 = ((hadtop_k.M()-target_hadtop_mass) / hadtop_sigma)*((hadtop_k.M()-target_hadtop_mass) / hadtop_sigma);
				
				//cout << "i = " << i << ", j = " << j << ", k = " << k << endl;
					
				for (int l=0;l<n_tags;l++)
				{
					if (l!=k)
					{
						TLorentzVector leptop_b_l = tagged_TLVs[l];
						double leptop_lepW_chi2 = find_localmin_algo(cout_algo, lepton, metv, leptop_b_l, new_met_pz);
						double total_chi2 = 2*leptop_lepW_chi2 + hadtop_chi2 + hadW_chi2;
						
						//cout << "l = " << l << endl;
						
						bool abool = (leptop_lepW_chi2<2.)&&(hadtop_chi2<2.)&&(hadW_chi2<2.);
						
						if (total_chi2<total_min_chi2) // &&abool)
						{
							total_min_chi2 = total_chi2;
							
							bool found_first = false;
							
							for (int m=0; m<(n_tags-1); m++)
							{
								for (int n=(m+1); n<n_tags; n++)
								{
									
									//cout << "m = " << m << ", n = " << n << endl;
									
									
									if ( (m!=l)&&(n!=l)&&(m!=k)&&(n!=k)&&(!found_first) )
									{
										higgs_b1 = tagged_TLVs[m];
										higgs_b2 = tagged_TLVs[n];
										higgs = higgs_b1 + higgs_b2;
										best_higgs_mass_v3 = higgs.M();
										
										found_first = true;
										
										//hadW_j1 = hadW_j1_i;
										//hadW_j2 = hadW_j2_j;
										//hadW = hadW_ij;
										//hadtop_b = 
										//leptop_b = 
										//hadtop = 
											
										//leptop_min_chi2
										hadtop_min_chi2 = hadtop_chi2;
										//lepW_min_chi2 = 
										hadW_min_chi2 = hadW_chi2;
										leptop_lepW_min_chi2 = leptop_lepW_chi2;
									
									}
								}
							}
						}
					}
				}
			}
		}
	}
	*/
	
	int counter_3terms = 0;
	
	// algo # 2:
	
	//count how many make it (3 terms)
	
	picked_1_rank_by_CSV = -1;
	picked_2_rank_by_CSV = -1;
	
	
	
	CSV_sorted = CSV;
	sort(CSV_sorted.begin(),CSV_sorted.end()); // <-ascending    , std::greater<int>()); // descending
	int index_to_jet_CSV_sorted[njets];
	
	for (int i=0; i<njets; i++)
	{
		for (int j=0; j<njets; j++)
		{
			if ( (CSV_sorted[i]==CSV[j])&&(!used_jets[j]) )
			{
				index_to_jet_CSV_sorted[i] = j;
				
				//if (picked_1_rank_by_CSV>=0&&picked_2_rank_by_CSV<0&&(abs(jet_genParentId[j])==25)) picked_2_rank_by_CSV = njets-i;
				//if (picked_1_rank_by_CSV<0&&picked_2_rank_by_CSV<0&&(abs(jet_genParentId[j])==25)) picked_1_rank_by_CSV = njets-i;
				
				used_jets[j] = true;
				break;
			}
			
		}
	}
	
	int check1=0;
	int check2=0;
	
	for (int check=0; check<njets; check++)
	{
		check1 = check1 + check;
		check2 = check2 + index_to_jet_CSV_sorted[check];
	}
	
	if (check1!=check2)
	{
		cout << " You are an idiot." << endl;
		cout << "  " << endl;
		for (int blah=0; blah<njets; blah++) cout << CSV_sorted[blah] << endl;
		cout << "  " << endl;
		return -99;
	}
	
	
	//////////////// FIX THIS !!! ////////////////
	
	/*
	//for (int i=0;i<(njets-1);i++)
	//{
		//for (int j=(i+1);j<njets;j++)
		//{
			
			
			//TLorentzVector higgs_ij = jet_TLVs[i] + jet_TLVs[j];
			
			//for (int k=0; k<(njets-1); k++)
			//{
				//if ((k!=i)&&(k!=j))
				//{
					//for (int l=(k+1); l<njets; l++)
					//{
						//if ((l!=i)&&(l!=j))
						//{
							
							
							//used_jets[index_to_jet_CSV_sorted[0]] = true; 					 
							//used_jets[index_to_jet_CSV_sorted[1]] = true; 					 
							
							
							TLorentzVector hadW_j1_i = jet_TLVs[index_to_jet_CSV_sorted[0]];
							TLorentzVector hadW_j2_j = jet_TLVs[index_to_jet_CSV_sorted[1]];
							TLorentzVector hadW_ij = hadW_j1_i + hadW_j2_j;
							
							for (int m=2; m<njets; m++)
							{

								TLorentzVector hadtop_k = hadW_ij + jet_TLVs[index_to_jet_CSV_sorted[m]];

								for (int n=2; n<njets; n++)
								{
									if (n!=m)
									{

										TLorentzVector leptop_b_l = jet_TLVs[index_to_jet_CSV_sorted[n]];

										for (int i=2;i<(njets-1);i++)
										{
											for (int j=i+1;j<njets;j++)
											{
										
												if ((i!=m)&&(i!=n)&&(j!=m)&&(j!=n))
												{
										
													TLorentzVector higgs_ij = jet_TLVs[index_to_jet_CSV_sorted[i]] + jet_TLVs[index_to_jet_CSV_sorted[j]];
										
													double leptop_lepW_chi2 = find_localmin_algo(cout_algo, lepton, metv, leptop_b_l, new_met_pz);
													double hadW_chi2 = ((hadW_ij.M()-target_hadW_mass) / hadW_sigma)*((hadW_ij.M()-target_hadW_mass) / hadW_sigma);
													double hadtop_chi2 = ((hadtop_k.M()-target_hadtop_mass) / hadtop_sigma)*((hadtop_k.M()-target_hadtop_mass) / hadtop_sigma);
													double higgs_chi2 = ( (higgs_ij.M()-target_higgs_mass) / higgs_sigma)*( (higgs_ij.M()-target_higgs_mass) / higgs_sigma);
													double total_chi2 = leptop_lepW_chi2 + hadtop_chi2 + hadW_chi2 + higgs_chi2;

													int num_terms_lt1 = 0;

													if (leptop_lepW_chi2<2) num_terms_lt1++;
													if (hadtop_chi2<2) num_terms_lt1++;
													//if (hadW_chi2<2) num_terms_lt1++;
													if (higgs_chi2<2) num_terms_lt1++;

													//counter_3terms += num_terms_lt1;
													if (num_terms_lt1>=2) counter_3terms++;
												}
											}
										}
									}
								}
							}
						//}
					//}
				//}
			//}
		//}
	//}
	
	*/							
		
		
		
		
		
		
	
	// algo #3
	
	TLorentzVector hadW_j1_i = jet_TLVs[index_to_jet_CSV_sorted[0]];
	TLorentzVector hadW_j2_j = jet_TLVs[index_to_jet_CSV_sorted[1]];
	TLorentzVector hadW_ij = hadW_j1_i + hadW_j2_j;
	
	double working_hadW_mass = hadW_ij.M();
	double hadW_initial_chi2 = ((working_hadW_mass-target_hadW_mass) / hadW_sigma)*((working_hadW_mass-target_hadW_mass) / hadW_sigma);
	double new_hadW_energy = 0.;
	
	if (hadW_initial_chi2>2.)
	{
		
		new_hadW_energy = target_hadW_mass*target_hadW_mass;
		new_hadW_energy -= working_hadW_mass*working_hadW_mass;
		new_hadW_energy += (hadW_ij.E())*(hadW_ij.E());
		new_hadW_energy = sqrt(new_hadW_energy);
		
		hadW_ij.SetE(new_hadW_energy);
	}
	

	int max_chi2s = 0;
	int num_chi2s_minchi = 0;


	for (int m=2; m<njets; m++)
	{

		TLorentzVector hadtop_k = hadW_ij + jet_TLVs[index_to_jet_CSV_sorted[m]];

		for (int n=2; n<njets; n++)
		{
			if (n!=m)
			{

				TLorentzVector leptop_b_l = jet_TLVs[index_to_jet_CSV_sorted[n]];
				
				
				double leptop_lepW_chi2 = find_localmin_algo(cout_algo, lepton, metv, leptop_b_l, new_met_pz);
				double hadW_chi2 = ((hadW_ij.M()-target_hadW_mass) / hadW_sigma)*((hadW_ij.M()-target_hadW_mass) / hadW_sigma);
				double hadtop_chi2 = ((hadtop_k.M()-target_hadtop_mass) / hadtop_sigma)*((hadtop_k.M()-target_hadtop_mass) / hadtop_sigma);
				
				//double total_chi2 = leptop_lepW_chi2 + hadW_chi2 + hadtop_chi2; // + higgs_chi2;

				//if (total_chi2<total_min_chi2)
				//{
				
				
					for (int i=2;i<(njets-1);i++)
					{
						for (int j=i+1;j<njets;j++)
						{

							bool happened = false;

							if ((i!=m)&&(i!=n)&&(j!=m)&&(j!=n)) // &&(!happened))  // <- remember, it's in ascending CSV order!
							{
								happened = true;
								
								double csvi = CSV[index_to_jet_CSV_sorted[i]];
								double csvj = CSV[index_to_jet_CSV_sorted[j]];
								double csv_chi2 = 0.5*( (1.0-csvi) / csv_medium )*( (1.0-csvi) / csv_medium );
								csv_chi2 += 0.5*( (1.0-csvj) / csv_medium )*( (1.0-csvj) / csv_medium );
								
								
								TLorentzVector higgs_ij = jet_TLVs[index_to_jet_CSV_sorted[i]] + jet_TLVs[index_to_jet_CSV_sorted[j]];
								double higgsmass = higgs_ij.M();
								double higgs_chi2 = ( (higgsmass-target_higgs_mass) / higgs_sigma)*( (higgsmass-target_higgs_mass) / higgs_sigma);
								
								//if (higgsmass<85||higgsmass>155) higgs_chi2 = 0.; // 125: (higgsmass<105||higgsmass>145) // (higgsmass<100||higgsmass>140) // (higgsmass<85||higgsmass>155) // 165
								
								//if (higgsmass>100&&higgsmass<125) higgs_chi2 = (1./25.)*higgsmass - 4.;
								//if (higgsmass>125&&higgsmass<150) higgs_chi2 = 6.-(1./25.)*higgsmass;
								
								//higgs_chi2 = higgs_chi2*((higgs_ij.M()-50.) / 50.);
								//higgs_chi2 = higgs_chi2*((higgs_ij.M()-50.) / 50.);
								//higgs_chi2 = higgs_chi2*((400.-higgs_ij.M()) / 250.);
								//higgs_chi2 = higgs_chi2*((400.-higgs_ij.M()) / 250.);
								//higgs_chi2 = higgs_chi2*((200.-higgs_ij.M()) / 50.);
								//higgs_chi2 = higgs_chi2*((200.-higgs_ij.M()) / 50.);
								//higgs_chi2 = fabs(higgs_chi2);
								//higgs_chi2 = pow(higgs_chi2,0.333333);
								
								double higg_dR = jet_TLVs[index_to_jet_CSV_sorted[i]].DeltaR(jet_TLVs[index_to_jet_CSV_sorted[j]]);
								double dr_chi2 = ( (higgs_dr_avg - higg_dR) / higgs_dr_sigma )*( (higgs_dr_avg - higg_dR) / higgs_dr_sigma );
								
								double total_chi2 = leptop_lepW_chi2 + hadtop_chi2 + dr_chi2; //  + higgs_chi2; // hadW_chi2 // + 2*csv_chi2     0.89*leptop_lepW_chi2 + 0.67*hadtop_chi2

								int num_terms_lt1 = 0;

								if (leptop_lepW_chi2<2) num_terms_lt1++;
								if (hadtop_chi2<2) num_terms_lt1++;
								//if (hadW_chi2<1) num_terms_lt1++;
								//if (higgs_chi2<1) num_terms_lt1++;

								//counter_3terms += num_terms_lt1;
								if (num_terms_lt1==1) counter_3terms++;



								//if (max_chi2s==num_terms_lt1)
								//{
									//if (total_chi2>total_min_chi2) // &&((njets-i)<4)&&((njets-j)<4)) // &&num_terms_lt1==2)
									if (total_chi2<total_min_chi2) //&&((njets-i)<4)&&((njets-j)<4))
									{
										total_min_chi2 = total_chi2;
										
										higgs_min_chi2 = higgs_chi2;
										hadtop_min_chi2 = hadtop_chi2;
										hadW_min_chi2 = hadW_chi2;
										leptop_lepW_min_chi2 = leptop_lepW_chi2;
										hadWmass = hadW_ij.M();
										hadtopmass = hadtop_k.M();
										
										picked_1_CSV = CSV[index_to_jet_CSV_sorted[i]];
										picked_2_CSV = CSV[index_to_jet_CSV_sorted[j]];
										
										best_higgs_mass_v3 = higgsmass;
										
										picked_1_rank_by_CSV = njets-i;
										picked_2_rank_by_CSV = njets-j;
										
										new_met_pz_chosen = new_met_pz;
										leptop_b = leptop_b_l;
										
									}
								//}
								
								vect_of_minchis.push_back(total_chi2);

								
								//if (max_chi2s<num_terms_lt1)
								//{
								//	max_chi2s = num_terms_lt1;

								//	best_higgs_mass_v3 = higgs_ij.M();

								//	total_min_chi2 = total_chi2;

								//}
								

							}
						}
					}
				//}
			}
		}
	}	
	
	
	
	
	// algo #4:
	/*
	TLorentzVector fourandfive;
	fourandfive = jet_TLVs[index_to_jet_CSV_sorted[4]] + jet_TLVs[index_to_jet_CSV_sorted[5]]; // <- supposedly bin with most higgs jets
	best_higgs_mass_v3 = fourandfive.M();
	
	picked_1_CSV = -10.;
	picked_2_CSV = -10.;
	
	if (picked_1_rank_by_CSV>-1) picked_1_CSV = CSV[index_to_jet_CSV_sorted[picked_1_rank_by_CSV]];
	if (picked_2_rank_by_CSV>-1) picked_2_CSV = CSV[index_to_jet_CSV_sorted[picked_2_rank_by_CSV]];
	*/
										
	sort(vect_of_minchis.begin(),vect_of_minchis.end());
	
	
	//leptonic stuff:
	TLorentzVector the_chosen_new_METv = metv;
	double met_E_sq = MET*MET;
	met_E_sq = met_E_sq + (new_met_pz_chosen*new_met_pz_chosen);
	double met_E = sqrt(met_E_sq);
	the_chosen_new_METv.SetPz(new_met_pz_chosen);
	the_chosen_new_METv.SetE(met_E);
	new_MET_TLV = the_chosen_new_METv;

	lepW = lepton + the_chosen_new_METv;
	lepWmass = lepW.M();
	leptop = leptop_b + lepW;
	leptopmass = leptop.M();
	
	lepW_min_chi2 = ((81.-lepWmass) / 12.)*((81.-lepWmass) / 12.);
	
	
//	if (best_higgs_mass_v3==0)
//	{
//		cout << njets << "  " << n_tags << "  " << n_untags << "  " << CSV_sorted[1] << endl;
//	}

	return best_higgs_mass_v3;
	//return (double)counter_3terms;
	//return (double)(max_chi2s-num_chi2s_minchi);
	//return total_min_chi2;
}

void get_sorted_phi_wrt_lep_TLVs(vdouble &newPhi_vect, vdouble &newPt_vect, vvdouble jets, vdouble lep, double met, double metphi, bool usemet)
{
	
	double mpi = acos(-1.);
	
	vecTLorentzVector thejets;
	TLorentzVector thelep;
	convert_jets_to_TLVs(jets,thejets);
	get_lep_TLV(lep,thelep);
	
	//vecTLorentzVector tosort;
	vdouble tosort;
	
	TLorentzVector metv;
	metv.SetPtEtaPhiE(met,0.,metphi,met);
	
	if (usemet) thejets.push_back(metv);
	
	double thelepPhi = thelep.Phi();
	
	for (int i=0; i<thejets.size(); i++)
	{
		double newphi = thejets[i].Phi() - thelepPhi;
		
		if (newphi<0.) newphi = newphi + 2.0*mpi;
		
		//TLorentzVector tempjet = thejets[i];
		//tempjet.SetPhi(newphi);
		//tosort.push_back(tempjet);
		
		tosort.push_back(newphi);
	}
	
	double prev_small_one = -1.;
	
	
	for (int i=0; i<thejets.size(); i++)
	{
		
		double larger_phi_subset = 999.;
		double theone_phi;
		double theone_pt;
		
		for (int j=0; j<thejets.size(); j++)
		{
			if ( (tosort[j] < larger_phi_subset) && (tosort[j] > prev_small_one) )
			{
				theone_phi = tosort[j];
				theone_pt = thejets[j].Pt();

				larger_phi_subset = tosort[j];
				
			}
		}
		
		prev_small_one = theone_phi;
		
		newPhi_vect.push_back(theone_phi);
		newPt_vect.push_back(theone_pt);
	}
		
}

void get_vect_of_jet_dRs_sorted_by_dR(vvdouble jets, vdouble &sorted_dRs, vdouble &masses, vdouble CSVvect, vdouble &avg_CSV, vdouble lepton)
{
	vecTLorentzVector thejets;
	//convert_jets_to_TLVs(jets,thejets);
	vect_of_tagged_TLVs(jets,CSVvect,thejets);
	
	TLorentzVector thelep;
	get_lep_TLV(lepton, thelep);
	
	//vdouble jetdRs_to_sort;
	
	vdouble masses_temp;
	vdouble avg_CSV_temp;
	
	
	///////////////////////////////////////////////////////////
	bool uselep = false;	///// <- treat lepton as a jet! ///
	///////////////////////////////////////////////////////////
	
	if (uselep) thejets.push_back(thelep);
	
	
	
	for (int i=0; i<(thejets.size()-1); i++)
	{
		for (int j=i+1; j<thejets.size(); j++)
		{
			double dr = thejets[i].DeltaR(thejets[j]);
			//sorted_dRs.push_back(dr);
			
			TLorentzVector dijet = thejets[i] + thejets[j];
			masses_temp.push_back(dijet.M());
			//masses_temp.push_back(fabs(dijet.M()-120.));
			sorted_dRs.push_back(fabs(dijet.M()-120.));
			
			double avgCSV = (CSVvect[i] + CSVvect[j]) / 2.0;
			avg_CSV_temp.push_back(avgCSV);
		}
	}
	
	vdouble orig_dRs = sorted_dRs;
	
	sort(sorted_dRs.begin(),sorted_dRs.end());
	
	
	for (int i=0; i<sorted_dRs.size(); i++)
	{
		for (int j=0; j<sorted_dRs.size(); j++)
		{
			if (orig_dRs[j]==sorted_dRs[i])
			{
				masses.push_back(masses_temp[j]);
				avg_CSV.push_back(avg_CSV_temp[j]);
			}
		}
	}
	
	//sort(masses_temp.begin(),masses_temp.end());
	//sorted_dRs = masses_temp;
	

}
double get_response_of_gauss_thing(double eig0, double eig1, double eig2)
{
	// unweighted, ==6 jets, >=4 tags PCA transform (ttH evts only)
	// using drjets, corresponding masses as vars (30 vars)
	//
	// distributions of first three (PCA-space) eigenvalues (signal):
	//
	/*
	double sig0_mean = -1.94849e-06;
	double sig0_std_dev = 3.29963;
	double sig1_mean = 8.37015e-06;
	double sig1_std_dev = 1.99889;
	double sig2_mean = 1.63447e-05;
	double sig2_std_dev = 1.56886;
	
	//
	// distributions of first three (PCA-space) eigenvalues (background):
	//
	
	double back0_mean = 1.39724;
	double back0_std_dev = 3.71283;
	double back1_mean = -0.745377;
	double back1_std_dev = 2.19241;
	double back2_mean = 0.484393;
	double back2_std_dev = 1.78847;
	*/
	
	
	// unweighted, ==5 jets, >=4 tags PCA transform (ttH evts only)
	// using drjets, corresponding masses as vars (30 vars)
	//
	// distributions of first three (PCA-space) eigenvalues (signal):
	//
	
	double sig0_mean = -4.15564e-06;
	double sig0_std_dev = 2.57834;
	double sig1_mean = 3.79183e-07;
	double sig1_std_dev = 1.82045;
	double sig2_mean = -7.7393e-06;
	double sig2_std_dev = 1.41361;
	
	//
	// distributions of first three (PCA-space) eigenvalues (background):
	//
	
	double back0_mean = 0.726818;
	double back0_std_dev = 3.07422;
	double back1_mean = -0.808324;
	double back1_std_dev = 1.84863;
	double back2_mean = 0.494084;
	double back2_std_dev = 1.57644;
	
	
	double term_sig0 = (eig0 - sig0_mean) / sig0_std_dev;
	double term_sig1 = (eig1 - sig1_mean) / sig1_std_dev;
	double term_sig2 = (eig2 - sig2_mean) / sig2_std_dev;
	double term_sig = term_sig0*term_sig0 + term_sig1*term_sig1 + term_sig2*term_sig2;
	
	double term_back0 = (eig0 - back0_mean) / back0_std_dev;
	double term_back1 = (eig1 - back1_mean) / back1_std_dev;
	double term_back2 = (eig2 - back2_mean) / back2_std_dev;
	double term_back = term_back0*term_back0 + term_back1*term_back1 + term_back2*term_back2;
		
	double response = exp(-term_sig) - exp(-term_back);
	response = response / (exp(-term_sig) + exp(-term_back));
	
	return response;
}
double get_response_of_gauss_thing_weighted(double eig0, double eig1, double eig2)
{
	// weighted, ==5 jets, >=4 tags PCA transform (ttH evts only)
	// using drjets, corresponding masses as vars (30 vars)
	//
	// distributions of first three (PCA-space) eigenvalues (signal):
	//
	
	double sig0_mean = -0.0543917;
	double sig0_std_dev = 2.5613;
	double sig1_mean = -0.0623917;
	double sig1_std_dev = 1.77857;
	double sig2_mean = 0.00916637;
	double sig2_std_dev = 1.40257;
	
	//
	// distributions of first three (PCA-space) eigenvalues (background):
	//
	
	double back0_mean = 0.760083;
	double back0_std_dev = 3.08059;
	double back1_mean = -0.969262;
	double back1_std_dev = 1.83468;
	double back2_mean = 0.561686;
	double back2_std_dev = 1.55989;
	
	
	
	
	double term_sig0 = (eig0 - sig0_mean) / sig0_std_dev;
	double term_sig1 = (eig1 - sig1_mean) / sig1_std_dev;
	double term_sig2 = (eig2 - sig2_mean) / sig2_std_dev;
	double term_sig = term_sig0*term_sig0 + term_sig1*term_sig1 + term_sig2*term_sig2;
	
	double term_back0 = (eig0 - back0_mean) / back0_std_dev;
	double term_back1 = (eig1 - back1_mean) / back1_std_dev;
	double term_back2 = (eig2 - back2_mean) / back2_std_dev;
	double term_back = term_back0*term_back0 + term_back1*term_back1 + term_back2*term_back2;
		
	double response = exp(-term_sig) - exp(-term_back);
	response = response / (exp(-term_sig) + exp(-term_back));
	
	return response;
}
