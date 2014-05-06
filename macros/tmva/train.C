// Created by Geoff Smith 2/13/13
// 

#include <cstdlib>
#include <iostream> 
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <fstream>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TH1I.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TVector.h"
#include "TLorentzVector.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)

#include "TMVA/Config.h"
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/DataSetInfo.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "AnalysisCode/LeptonPlusJets/interface/BEANeventVars.h"

#endif


void lj_train( TString myMethodList = "", std::string back_files = "path/to/yggdrasil/wildcarded_files*.root", std::string sig_files = "path/to/yggdrasil/wildcarded_files*.root", int cat_switch=0, bool equal_evts_by_wgt = false, TString tname="worldTree", bool two_bkds = false, std::string second_bkd = "path/to/yggdrasil/wildcarded_files*.root");
std::string selection(int cat_switch=0);


/// --------------------------------------------------------

   
void train(int cat)
{

	// load your signal and background trees to be used in training:

	string sig_files = "../ttH_test_.root"; // signal
	string back_files ="../tt*_test.root"; // inclusive ttjets background
	//string back_files ="../ttb*_test.root"; // tt1b+tt2b only


	// true: USE ALL AVAILABLE EVENTS in each category (half for training, half for testing), but weight them so that background is normalized to signal.
	// 	This causes the MLP method to treat signal and background events equally during training, but not CFMlpANN for some reason.
	// false: MINIMUM NUMBER OF EVENTS ARE DISCARDED so that the number of signal and background events are the same in each category. Forces any MVA 
	// 	method to treat signal and background equally during training.
	bool equal_evts_by_wgt = false;
	

	// choose methods:
	TString methodlist = "BDT,CFMlpANN"; // ,CFMlpANN,MLP,Likelihood,SVM,HMatrix"; // ,SVM"; // "MLP,CFMlpANN,TMlpANN,BDT" , HMatrix

	// run training:
	lj_train(methodlist, back_files, sig_files, cat, equal_evts_by_wgt,"tree_of_knowledge");

}



void lj_train( TString myMethodList, std::string back_files, std::string sig_files, int cat_switch, bool equal_evts_by_wgt, TString tname, bool two_bkds, std::string second_bkd)
{


	// this loads the library
	TMVA::Tools::Instance();

	//---------------------------------------------------------------
	// default MVA methods to be trained + tested
	std::map<std::string,int> Use;


	// ---
	Use["MLP"]		= 1; // this is the recommended ANN
	//Use["MLPBFGS"]	= 1; // recommended ANN with optional training method
	Use["CFMlpANN"]		= 1; 
	Use["TMlpANN"]		= 1;
	Use["BDT"]		= 1;
	Use["SVM"]		= 1;
	Use["PDEFoam"]		= 1;
	Use["Likelihood"]	= 1;
	Use["HMatrix"]	= 1;
	// ---------------------------------------------------------------

	std::cout << std::endl;
	std::cout << "==> Start TMVAClassification" << std::endl;

	if (myMethodList != "")
	{
		for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

		std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
		for (UInt_t i=0; i<mlist.size(); i++)
		{
	 		std::string regMethod(mlist[i]);

	 		if (Use.find(regMethod) == Use.end())
			{
	    			std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
	    			for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
	    			std::cout << std::endl;
	    			return;
	 		}
	 		Use[regMethod] = 1;
		}
	}


	//// --------inputs--------------


	TString fname1 = sig_files;   // <-- input filenames
	TString fname2 = back_files;
	TString fname3 = second_bkd;

	cout << " " << endl;
	cout << "Opening TChains... " << endl;	
		
	TChain signal(tname);
	TChain background(tname);
	TChain background2(tname);

	signal.Add(fname1);
	cout << "Opened " << fname1 << endl;
	background.Add(fname2);
	cout << "Opened " << fname2 << endl;	
	
	if (two_bkds)
	{
		background2.Add(fname3);
		cout << "Opened " << fname3 << endl;
	}
	
	string cut = selection(cat_switch); // can be muons or electrons

	int signum_nocut = signal.GetEntries();
	int backnum_nocut = background.GetEntries();
	int backnum_nocut2;
	if (two_bkds) backnum_nocut2 = background2.GetEntries();
	
	cout << "signal events before selection: " << signum_nocut << endl;
	cout << "background events before selection: " << backnum_nocut << endl;
	if (two_bkds) cout << "background events (second background) before selection: " << backnum_nocut2 << endl;
	
	int signum = signal.GetEntries(cut.c_str());
	int backnum = background.GetEntries(cut.c_str());
	int backnum2;
	if (two_bkds) backnum2 = background2.GetEntries(cut.c_str());
	
	cout << "will select events with cut: " << cut << endl;
	cout << "signal events after cut: " << signum << endl;
	cout << "background events after cut: " << backnum << endl;
	if (two_bkds) cout << "additional events from second background after cut: " << backnum2 << endl;
	cout << " " << endl;
		
	Int_t smallest_amount = signum;	
	if (smallest_amount>backnum) smallest_amount = backnum; // if two backgrounds, assuming first is smallest!
	if (two_bkds&&(smallest_amount>backnum2)) smallest_amount = backnum2; // not anymore
	
	
	TString label;
	std::stringstream label_ss;
	label_ss << cat_switch;
	label = label_ss.str();
	
	
	// Create new root output files.
	TString outfileName1( "TMVA_trained_" + label + ".root" );   //  <------------- output files
	TFile* outputFile1 = TFile::Open( outfileName1, "RECREATE" );

	// book factory:
	TMVA::Factory *factory1;
	if (two_bkds) factory1 = new TMVA::Factory( "TMVAMulticlass", outputFile1, "V:!Silent:!Color:!DrawProgressBar:Transformations=I,N:AnalysisType=multiclass" );
	if (!two_bkds) factory1 = new TMVA::Factory( "TMVAClassification", outputFile1, "V:!Silent:!Color:!DrawProgressBar:Transformations=I,N" );  /// Transformations=I;D;P;G,D
		
	
	TMVA::gConfig().GetIONames().fWeightFileDir = "weights/" + label;



	// select variables:	
	switch(cat_switch)
	{
					
			
		case 433:	// best sep, mod
			
			
			//factory1->AddVariable( "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", 'F');  // const			
			//factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			//factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');
			
			//factory1->AddVariable( "tight_lepton_pt", "tight_lepton_pt", "tight_lepton_pt", 'F');
			//factory1->AddVariable( "aplanarity", "aplanarity", "aplanarity", 'F');
			factory1->AddVariable( "MET", "MET", "MET", 'F'); 					// rm for 8 best sep										
			//factory1->AddVariable( "MET_phi", "MET_phi", "MET_phi", 'F');
			factory1->AddVariable( "first_jet_pt", "first_jet_pt", "first_jet_pt", 'F'); 
			factory1->AddVariable( "second_jet_pt", "second_jet_pt", "second_jet_pt", 'F'); 
			factory1->AddVariable( "third_jet_pt", "third_jet_pt", "third_jet_pt", 'F'); // rm for 5 best sep
			factory1->AddVariable( "fourth_jet_pt", "fourth_jet_pt", "fourth_jet_pt", 'F'); 	// rm for 8 best sep
			//factory1->AddVariable( "h0", "h0", "h0", 'F');
			//factory1->AddVariable( "h1", "h1", "h1", 'F');
			////factory1->AddVariable( "all_sum_pt", "all_sum_pt", "all_sum_pt", 'F');
			factory1->AddVariable( "all_sum_pt_with_met", "all_sum_pt_with_met", "all_sum_pt_with_met", 'F');
			factory1->AddVariable( "HT", "HT", "HT", 'F');
			factory1->AddVariable( "MHT", "MHT", "MHT", 'F');	// rm for 5 best sep										
			//factory1->AddVariable( "MHT_phi", "MHT_phi", "MHT_phi", 'F');
			////factory1->AddVariable( "uMET", "", "", 'F');
			////factory1->AddVariable( "uMET_phi", "", "", 'F');
			//factory1->AddVariable( "avg_dr_tagged_jets", "avg_dr_tagged_jets", "avg_dr_tagged_jets", 'F');
			//factory1->AddVariable( "sphericity", "sphericity", "sphericity", 'F');
			//factory1->AddVariable( "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", 'F');
			//factory1->AddVariable( "avg_btag_disc_btags", "avg_btag_disc_btags", "avg_btag_disc_btags", 'F');					
			//factory1->AddVariable( "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", 'F');
			//factory1->AddVariable( "h2", "h2", "h2", 'F');
			//factory1->AddVariable( "lowest_btag", "lowest_btag", "lowest_btag", 'F');
			//factory1->AddVariable( "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F');			
			//factory1->AddVariable( "h3", "h3", "h3", 'F');
			//factory1->AddVariable( "h4", "h4", "h4", 'F');
			//factory1->AddVariable( "second_highest_btag", "second_highest_btag", "second_highest_btag", 'F');
			factory1->AddVariable( "third_highest_btag", "third_highest_btag", "third_highest_btag", 'F'); // rm for 5 best sep
			//factory1->AddVariable( "fourth_highest_btag", "fourth_highest_btag", "fourth_highest_btag", 'F');
			//factory1->AddVariable( "invariant_mass_of_everything", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F');		
			
			//factory1->AddVariable( "maxeta_jet_jet", "maxeta_jet_jet", "maxeta_jet_jet", 'F' );
			//factory1->AddVariable( "maxeta_jet_tag", "maxeta_jet_tag", "maxeta_jet_tag", 'F' );
			//factory1->AddVariable( "maxeta_tag_tag", "maxeta_tag_tag", "maxeta_tag_tag", 'F' );
			
			//factory1->AddVariable( "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", 'F');
			factory1->AddVariable( "M3", "M3", "M3", 'F');
			////factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			//factory1->AddVariable( "Mlb", "Mlb", "Mlb", 'F');
			//factory1->AddVariable( "first_highest_btag", "first_highest_btag", "first_highest_btag", 'F');
			// //factory1->AddVariable( "dRbb_bhmv2", "dRbb_bhmv2", "dRbb_bhmv2", 'F');
			//factory1->AddVariable( "median_bb_mass", "median_bb_mass", "median_bb_mass", 'F' );
			//factory1->AddVariable( "min_dr_tagged_jets", "min_dr_tagged_jets", "min_dr_tagged_jets", 'F');
			//factory1->AddVariable( "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", 'F');
			//factory1->AddVariable( "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", 'F');
			//factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');				
			//factory1->AddVariable( "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", 'F');	
			////factory1->AddVariable( "minChi2_v2", "minChi2_v2", "minChi2_v2", 'F');
			
			//factory1->AddVariable( "avg_abs_deta_jets", "avg_abs_deta_jets", "avg_abs_deta_jets", 'F');
			
			break;
			
			
		case 523: 
			
			//factory1->AddVariable( "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", 'F');  // const			
			//factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');	// rm for 5 best sep					// <--
			
			//factory1->AddVariable( "tight_lepton_pt", "tight_lepton_pt", "tight_lepton_pt", 'F');
			//factory1->AddVariable( "aplanarity", "aplanarity", "aplanarity", 'F');
			//factory1->AddVariable( "MET", "MET", "MET", 'F');
			//factory1->AddVariable( "MET_phi", "MET_phi", "MET_phi", 'F');
			factory1->AddVariable( "first_jet_pt", "first_jet_pt", "first_jet_pt", 'F');
			factory1->AddVariable( "second_jet_pt", "second_jet_pt", "second_jet_pt", 'F');
			factory1->AddVariable( "third_jet_pt", "third_jet_pt", "third_jet_pt", 'F'); // rm for 5 best sep
			factory1->AddVariable( "fourth_jet_pt", "fourth_jet_pt", "fourth_jet_pt", 'F');	// rm 8 best sep									// <--
			//factory1->AddVariable( "h0", "h0", "h0", 'F');
			//factory1->AddVariable( "h1", "h1", "h1", 'F');
			////factory1->AddVariable( "all_sum_pt", "all_sum_pt", "all_sum_pt", 'F');
			factory1->AddVariable( "all_sum_pt_with_met", "all_sum_pt_with_met", "all_sum_pt_with_met", 'F');							// <--
			factory1->AddVariable( "HT", "HT", "HT", 'F');														// <--
			//factory1->AddVariable( "MHT", "MHT", "MHT", 'F');
			//factory1->AddVariable( "MHT_phi", "MHT_phi", "MHT_phi", 'F');
			////factory1->AddVariable( "uMET", "", "", 'F');
			////factory1->AddVariable( "uMET_phi", "", "", 'F');
			//factory1->AddVariable( "avg_dr_tagged_jets", "avg_dr_tagged_jets", "avg_dr_tagged_jets", 'F');
			//factory1->AddVariable( "sphericity", "sphericity", "sphericity", 'F');
			//factory1->AddVariable( "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", 'F');
			//factory1->AddVariable( "avg_btag_disc_btags", "avg_btag_disc_btags", "avg_btag_disc_btags", 'F');
			//factory1->AddVariable( "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", 'F');
			//factory1->AddVariable( "h2", "h2", "h2", 'F');
			//factory1->AddVariable( "lowest_btag", "lowest_btag", "lowest_btag", 'F');
			//factory1->AddVariable( "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F');			
			//factory1->AddVariable( "h3", "h3", "h3", 'F');
			//factory1->AddVariable( "h4", "h4", "h4", 'F');
			//factory1->AddVariable( "second_highest_btag", "second_highest_btag", "second_highest_btag", 'F');
			factory1->AddVariable( "third_highest_btag", "third_highest_btag", "third_highest_btag", 'F');								// <--
			//factory1->AddVariable( "fourth_highest_btag", "fourth_highest_btag", "fourth_highest_btag", 'F');
			//factory1->AddVariable( "invariant_mass_of_everything", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F');
			
			//factory1->AddVariable( "maxeta_jet_jet", "maxeta_jet_jet", "maxeta_jet_jet", 'F' );
			//factory1->AddVariable( "maxeta_jet_tag", "maxeta_jet_tag", "maxeta_jet_tag", 'F' );
			//factory1->AddVariable( "maxeta_tag_tag", "maxeta_tag_tag", "maxeta_tag_tag", 'F' );
			
			//factory1->AddVariable( "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", 'F');
			//factory1->AddVariable( "M3", "M3", "M3", 'F');
			//factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			factory1->AddVariable( "Mlb", "Mlb", "Mlb", 'F');	// rm for 5 best sep												// <--
			//factory1->AddVariable( "first_highest_btag", "first_highest_btag", "first_highest_btag", 'F');
			// //factory1->AddVariable( "dRbb_bhmv2", "dRbb_bhmv2", "dRbb_bhmv2", 'F');
			//factory1->AddVariable( "median_bb_mass", "median_bb_mass", "median_bb_mass", 'F' );
			//factory1->AddVariable( "min_dr_tagged_jets", "min_dr_tagged_jets", "min_dr_tagged_jets", 'F');
			factory1->AddVariable( "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", 'F');	// rm 8 best sep			// <--
			//factory1->AddVariable( "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", 'F');
			////factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');				
			//factory1->AddVariable( "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", 'F');	
			////factory1->AddVariable( "minChi2_v2", "minChi2_v2", "minChi2_v2", 'F');
			factory1->AddVariable( "avg_abs_deta_jets", "avg_abs_deta_jets", "avg_abs_deta_jets", 'F');
			break;
			
			
		case 533:  
			
			
			//factory1->AddVariable( "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", 'F');  // const			
			//factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			//factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');
			
			//factory1->AddVariable( "tight_lepton_pt", "tight_lepton_pt", "tight_lepton_pt", 'F');
			//factory1->AddVariable( "aplanarity", "aplanarity", "aplanarity", 'F');
			//factory1->AddVariable( "MET", "MET", "MET", 'F');
			//factory1->AddVariable( "MET_phi", "MET_phi", "MET_phi", 'F');
			factory1->AddVariable( "first_jet_pt", "first_jet_pt", "first_jet_pt", 'F'); // rm for 5 best sep
			factory1->AddVariable( "second_jet_pt", "second_jet_pt", "second_jet_pt", 'F');
			factory1->AddVariable( "third_jet_pt", "third_jet_pt", "third_jet_pt", 'F'); // rm for 5 best sep
			factory1->AddVariable( "fourth_jet_pt", "fourth_jet_pt", "fourth_jet_pt", 'F'); // rm for 5 best sep
			//factory1->AddVariable( "h0", "h0", "h0", 'F');
			//factory1->AddVariable( "h1", "h1", "h1", 'F');
			////factory1->AddVariable( "all_sum_pt", "all_sum_pt", "all_sum_pt", 'F');
			factory1->AddVariable( "all_sum_pt_with_met", "all_sum_pt_with_met", "all_sum_pt_with_met", 'F');
			factory1->AddVariable( "HT", "HT", "HT", 'F');
			//factory1->AddVariable( "MHT", "MHT", "MHT", 'F');
			//factory1->AddVariable( "MHT_phi", "MHT_phi", "MHT_phi", 'F');
			////factory1->AddVariable( "uMET", "", "", 'F');
			////factory1->AddVariable( "uMET_phi", "", "", 'F');
			//factory1->AddVariable( "avg_dr_tagged_jets", "avg_dr_tagged_jets", "avg_dr_tagged_jets", 'F');
			//factory1->AddVariable( "sphericity", "sphericity", "sphericity", 'F');
			//factory1->AddVariable( "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", 'F');
			factory1->AddVariable( "avg_btag_disc_btags", "avg_btag_disc_btags", "avg_btag_disc_btags", 'F');
			//factory1->AddVariable( "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", 'F');
			//factory1->AddVariable( "h2", "h2", "h2", 'F');
			//factory1->AddVariable( "lowest_btag", "lowest_btag", "lowest_btag", 'F');
			//factory1->AddVariable( "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F');			
			//factory1->AddVariable( "h3", "h3", "h3", 'F');
			//factory1->AddVariable( "h4", "h4", "h4", 'F');
			//factory1->AddVariable( "second_highest_btag", "second_highest_btag", "second_highest_btag", 'F');
			factory1->AddVariable( "third_highest_btag", "third_highest_btag", "third_highest_btag", 'F');
			factory1->AddVariable( "fourth_highest_btag", "fourth_highest_btag", "fourth_highest_btag", 'F'); // rm 8 best sep
			//factory1->AddVariable( "invariant_mass_of_everything", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F');
			
			//factory1->AddVariable( "maxeta_jet_jet", "maxeta_jet_jet", "maxeta_jet_jet", 'F' );
			//factory1->AddVariable( "maxeta_jet_tag", "maxeta_jet_tag", "maxeta_jet_tag", 'F' );
			//factory1->AddVariable( "maxeta_tag_tag", "maxeta_tag_tag", "maxeta_tag_tag", 'F' );
			
			//factory1->AddVariable( "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", 'F');
			//factory1->AddVariable( "M3", "M3", "M3", 'F');
			////factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			//factory1->AddVariable( "Mlb", "Mlb", "Mlb", 'F');
			//factory1->AddVariable( "first_highest_btag", "first_highest_btag", "first_highest_btag", 'F');
			// //factory1->AddVariable( "dRbb_bhmv2", "dRbb_bhmv2", "dRbb_bhmv2", 'F');
			//factory1->AddVariable( "median_bb_mass", "median_bb_mass", "median_bb_mass", 'F' );
			//factory1->AddVariable( "min_dr_tagged_jets", "min_dr_tagged_jets", "min_dr_tagged_jets", 'F');
			factory1->AddVariable( "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", 'F'); // rm 8 best sep
			//factory1->AddVariable( "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", 'F');
			////factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');				
			//factory1->AddVariable( "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", 'F');	
			////factory1->AddVariable( "minChi2_v2", "minChi2_v2", "minChi2_v2", 'F');
			
			//factory1->AddVariable( "avg_abs_deta_jets", "avg_abs_deta_jets", "avg_abs_deta_jets", 'F');
			
			break;
			
			
		case 623: 
			
			//factory1->AddVariable( "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", 'F');  // const			
			//factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			//factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');
			
			//factory1->AddVariable( "tight_lepton_pt", "tight_lepton_pt", "tight_lepton_pt", 'F');
			//factory1->AddVariable( "aplanarity", "aplanarity", "aplanarity", 'F');
			//factory1->AddVariable( "MET", "MET", "MET", 'F');
			//factory1->AddVariable( "MET_phi", "MET_phi", "MET_phi", 'F');
			//factory1->AddVariable( "first_jet_pt", "first_jet_pt", "first_jet_pt", 'F');
			//factory1->AddVariable( "second_jet_pt", "second_jet_pt", "second_jet_pt", 'F');
			//factory1->AddVariable( "third_jet_pt", "third_jet_pt", "third_jet_pt", 'F');
			//factory1->AddVariable( "fourth_jet_pt", "fourth_jet_pt", "fourth_jet_pt", 'F');
			//factory1->AddVariable( "h0", "h0", "h0", 'F');
			//factory1->AddVariable( "h1", "h1", "h1", 'F');
			////factory1->AddVariable( "all_sum_pt", "all_sum_pt", "all_sum_pt", 'F');
			factory1->AddVariable( "all_sum_pt_with_met", "all_sum_pt_with_met", "all_sum_pt_with_met", 'F');
			factory1->AddVariable( "HT", "HT", "HT", 'F'); // rm 5 best sep
			//factory1->AddVariable( "MHT", "MHT", "MHT", 'F');
			//factory1->AddVariable( "MHT_phi", "MHT_phi", "MHT_phi", 'F');
			////factory1->AddVariable( "uMET", "", "", 'F');
			////factory1->AddVariable( "uMET_phi", "", "", 'F');
			//factory1->AddVariable( "avg_dr_tagged_jets", "avg_dr_tagged_jets", "avg_dr_tagged_jets", 'F');
			factory1->AddVariable( "sphericity", "sphericity", "sphericity", 'F');
			factory1->AddVariable( "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", 'F'); // rm 8 best sep
			//factory1->AddVariable( "avg_btag_disc_btags", "avg_btag_disc_btags", "avg_btag_disc_btags", 'F');
			//factory1->AddVariable( "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", 'F');
			factory1->AddVariable( "h2", "h2", "h2", 'F'); // rm 8 best sep
			//factory1->AddVariable( "lowest_btag", "lowest_btag", "lowest_btag", 'F');
			//factory1->AddVariable( "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F');			
			//factory1->AddVariable( "h3", "h3", "h3", 'F');
			//factory1->AddVariable( "h4", "h4", "h4", 'F');
			//factory1->AddVariable( "second_highest_btag", "second_highest_btag", "second_highest_btag", 'F');
			factory1->AddVariable( "third_highest_btag", "third_highest_btag", "third_highest_btag", 'F');
			factory1->AddVariable( "fourth_highest_btag", "fourth_highest_btag", "fourth_highest_btag", 'F'); // rm 5 best sep
			//factory1->AddVariable( "invariant_mass_of_everything", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F');
			
			factory1->AddVariable( "maxeta_jet_jet", "maxeta_jet_jet", "maxeta_jet_jet", 'F' ); // rm 5 best sep
			//factory1->AddVariable( "maxeta_jet_tag", "maxeta_jet_tag", "maxeta_jet_tag", 'F' );
			//factory1->AddVariable( "maxeta_tag_tag", "maxeta_tag_tag", "maxeta_tag_tag", 'F' );
			
			//factory1->AddVariable( "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", 'F');
			//factory1->AddVariable( "M3", "M3", "M3", 'F');
			////factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			factory1->AddVariable( "Mlb", "Mlb", "Mlb", 'F');
			//factory1->AddVariable( "first_highest_btag", "first_highest_btag", "first_highest_btag", 'F');
			// //factory1->AddVariable( "dRbb_bhmv2", "dRbb_bhmv2", "dRbb_bhmv2", 'F');
			//factory1->AddVariable( "median_bb_mass", "median_bb_mass", "median_bb_mass", 'F' );
			//factory1->AddVariable( "min_dr_tagged_jets", "min_dr_tagged_jets", "min_dr_tagged_jets", 'F');
			factory1->AddVariable( "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", 'F');
			//factory1->AddVariable( "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", 'F');
			////factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');				
			//factory1->AddVariable( "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", 'F');	
			////factory1->AddVariable( "minChi2_v2", "minChi2_v2", "minChi2_v2", 'F');
			//factory1->AddVariable( "avg_abs_deta_jets", "avg_abs_deta_jets", "avg_abs_deta_jets", 'F');
			break;
			
			
		case 633:
			
			
			//factory1->AddVariable( "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", 'F');  // const			
			//factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			//factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');
			
			//factory1->AddVariable( "tight_lepton_pt", "tight_lepton_pt", "tight_lepton_pt", 'F');
			//factory1->AddVariable( "aplanarity", "aplanarity", "aplanarity", 'F');
			//factory1->AddVariable( "MET", "MET", "MET", 'F');
			//factory1->AddVariable( "MET_phi", "MET_phi", "MET_phi", 'F');
			//factory1->AddVariable( "first_jet_pt", "first_jet_pt", "first_jet_pt", 'F');
			//factory1->AddVariable( "second_jet_pt", "second_jet_pt", "second_jet_pt", 'F');
			//factory1->AddVariable( "third_jet_pt", "third_jet_pt", "third_jet_pt", 'F');
			//factory1->AddVariable( "fourth_jet_pt", "fourth_jet_pt", "fourth_jet_pt", 'F');
			factory1->AddVariable( "h0", "h0", "h0", 'F');	// rm 8 best sep							// add
			//factory1->AddVariable( "h1", "h1", "h1", 'F');
			////factory1->AddVariable( "all_sum_pt", "all_sum_pt", "all_sum_pt", 'F');
			factory1->AddVariable( "all_sum_pt_with_met", "all_sum_pt_with_met", "all_sum_pt_with_met", 'F');  // rm 5 best sep	// add
			//factory1->AddVariable( "HT", "HT", "HT", 'F');
			//factory1->AddVariable( "MHT", "MHT", "MHT", 'F');
			//factory1->AddVariable( "MHT_phi", "MHT_phi", "MHT_phi", 'F');
			////factory1->AddVariable( "uMET", "", "", 'F');
			////factory1->AddVariable( "uMET_phi", "", "", 'F');
			//factory1->AddVariable( "avg_dr_tagged_jets", "avg_dr_tagged_jets", "avg_dr_tagged_jets", 'F');						// rm
			factory1->AddVariable( "sphericity", "sphericity", "sphericity", 'F');
			//factory1->AddVariable( "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", 'F');
			factory1->AddVariable( "avg_btag_disc_btags", "avg_btag_disc_btags", "avg_btag_disc_btags", 'F');
			//factory1->AddVariable( "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", 'F');					// rm
			//factory1->AddVariable( "h2", "h2", "h2", 'F');
			//factory1->AddVariable( "lowest_btag", "lowest_btag", "lowest_btag", 'F');
			//factory1->AddVariable( "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F');			
			//factory1->AddVariable( "h3", "h3", "h3", 'F');
			//factory1->AddVariable( "h4", "h4", "h4", 'F');
			factory1->AddVariable( "second_highest_btag", "second_highest_btag", "second_highest_btag", 'F');
			factory1->AddVariable( "third_highest_btag", "third_highest_btag", "third_highest_btag", 'F');
			factory1->AddVariable( "fourth_highest_btag", "fourth_highest_btag", "fourth_highest_btag", 'F'); // rm 5 best sep
			//factory1->AddVariable( "invariant_mass_of_everything", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F');
			
			factory1->AddVariable( "maxeta_jet_jet", "maxeta_jet_jet", "maxeta_jet_jet", 'F' ); // rm 5 best sep
			//factory1->AddVariable( "maxeta_jet_tag", "maxeta_jet_tag", "maxeta_jet_tag", 'F' );
			//factory1->AddVariable( "maxeta_tag_tag", "maxeta_tag_tag", "maxeta_tag_tag", 'F' );
			
			//factory1->AddVariable( "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", 'F');
			//factory1->AddVariable( "M3", "M3", "M3", 'F');
			////factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			//factory1->AddVariable( "Mlb", "Mlb", "Mlb", 'F');
			//factory1->AddVariable( "first_highest_btag", "first_highest_btag", "first_highest_btag", 'F'); // rm 8 best sep // rm 6/21
			// //factory1->AddVariable( "dRbb_bhmv2", "dRbb_bhmv2", "dRbb_bhmv2", 'F');
			//factory1->AddVariable( "median_bb_mass", "median_bb_mass", "median_bb_mass", 'F' );
			//factory1->AddVariable( "min_dr_tagged_jets", "min_dr_tagged_jets", "min_dr_tagged_jets", 'F');
			factory1->AddVariable( "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", 'F');
			//factory1->AddVariable( "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", 'F');						// <- temp 6/6/13
			////factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');				
			//factory1->AddVariable( "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", 'F');	
			////factory1->AddVariable( "minChi2_v2", "minChi2_v2", "minChi2_v2", 'F');
			//factory1->AddVariable( "best_higgs_mass","best_higgs_mass","best_higgs_mass",'F'); // rm to make 10 best sep
			
			factory1->AddVariable( "ttbb_tth_MVA_output_6j3t","ttbb_tth_MVA_output_6j3t","ttbb_tth_MVA_output_6j3t", 'F');
			//factory1->AddVariable( "avg_abs_deta_jets", "avg_abs_deta_jets", "avg_abs_deta_jets", 'F');
			break;
			
			
		case 643: 
			
			
			//factory1->AddVariable( "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", 'F');  // const
			//factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			//factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');
			
			//factory1->AddVariable( "tight_lepton_pt", "tight_lepton_pt", "tight_lepton_pt", 'F');
			//factory1->AddVariable( "aplanarity", "aplanarity", "aplanarity", 'F');
			//factory1->AddVariable( "MET", "MET", "MET", 'F');
			//factory1->AddVariable( "MET_phi", "MET_phi", "MET_phi", 'F');
			//factory1->AddVariable( "first_jet_pt", "first_jet_pt", "first_jet_pt", 'F');
			//factory1->AddVariable( "second_jet_pt", "second_jet_pt", "second_jet_pt", 'F');
			//factory1->AddVariable( "third_jet_pt", "third_jet_pt", "third_jet_pt", 'F');
			//factory1->AddVariable( "fourth_jet_pt", "fourth_jet_pt", "fourth_jet_pt", 'F');
			//factory1->AddVariable( "h0", "h0", "h0", 'F');
			//factory1->AddVariable( "h1", "h1", "h1", 'F');
			////factory1->AddVariable( "all_sum_pt", "all_sum_pt", "all_sum_pt", 'F');
			//factory1->AddVariable( "all_sum_pt_with_met", "all_sum_pt_with_met", "all_sum_pt_with_met", 'F');
			//factory1->AddVariable( "HT", "HT", "HT", 'F');
			//factory1->AddVariable( "MHT", "MHT", "MHT", 'F');
			//factory1->AddVariable( "MHT_phi", "MHT_phi", "MHT_phi", 'F');
			////factory1->AddVariable( "uMET", "", "", 'F');
			////factory1->AddVariable( "uMET_phi", "", "", 'F');
			factory1->AddVariable( "avg_dr_tagged_jets", "avg_dr_tagged_jets", "avg_dr_tagged_jets", 'F');
			//factory1->AddVariable( "sphericity", "sphericity", "sphericity", 'F');
			//factory1->AddVariable( "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", 'F');
			factory1->AddVariable( "avg_btag_disc_btags", "avg_btag_disc_btags", "avg_btag_disc_btags", 'F');
			//factory1->AddVariable( "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", 'F');						// test1
			//factory1->AddVariable( "h2", "h2", "h2", 'F');
			//factory1->AddVariable( "lowest_btag", "lowest_btag", "lowest_btag", 'F');								// rm
			factory1->AddVariable( "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F'); // rm to make 8 best sep		
			//factory1->AddVariable( "h3", "h3", "h3", 'F');
			//factory1->AddVariable( "h4", "h4", "h4", 'F');
			//factory1->AddVariable( "second_highest_btag", "second_highest_btag", "second_highest_btag", 'F');
			factory1->AddVariable( "third_highest_btag", "third_highest_btag", "third_highest_btag", 'F');
			factory1->AddVariable( "fourth_highest_btag", "fourth_highest_btag", "fourth_highest_btag", 'F');
			//factory1->AddVariable( "invariant_mass_of_everything", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F');
			
			//factory1->AddVariable( "maxeta_jet_jet", "maxeta_jet_jet", "maxeta_jet_jet", 'F' );
			//factory1->AddVariable( "maxeta_jet_tag", "maxeta_jet_tag", "maxeta_jet_tag", 'F' ); // rm 5 best sep
			factory1->AddVariable( "maxeta_tag_tag", "maxeta_tag_tag", "maxeta_tag_tag", 'F' ); // rm 5 best sep
			
			//factory1->AddVariable( "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", 'F');
			//factory1->AddVariable( "M3", "M3", "M3", 'F');
			////factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			//factory1->AddVariable( "Mlb", "Mlb", "Mlb", 'F');
			//factory1->AddVariable( "first_highest_btag", "first_highest_btag", "first_highest_btag", 'F');
			// //factory1->AddVariable( "dRbb_bhmv2", "dRbb_bhmv2", "dRbb_bhmv2", 'F');
			//factory1->AddVariable( "median_bb_mass", "median_bb_mass", "median_bb_mass", 'F' ); // rm to make 10 best sep
			//factory1->AddVariable( "min_dr_tagged_jets", "min_dr_tagged_jets", "min_dr_tagged_jets", 'F');
			factory1->AddVariable( "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", 'F');
			factory1->AddVariable( "ttbb_tth_MVA_output_6j4t", "ttbb_tth_MVA_output_6j4t", "ttbb_tth_MVA_output_6j4t", 'F');			// <-- in: v1; out: v1.2
			////factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');				
			//factory1->AddVariable( "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", 'F');		// test2	
			////factory1->AddVariable( "minChi2_v2", "minChi2_v2", "minChi2_v2", 'F');
			factory1->AddVariable( "best_higgs_mass","best_higgs_mass","best_higgs_mass",'F'); // rm to make 8 best sep
			factory1->AddVariable( "dEta_fn", "dEta_fn", "dEta_fn", 'F' ); // rm 5 best sep
			
			
			
			//factory1->AddVariable( "avg_abs_deta_jets", "avg_abs_deta_jets", "avg_abs_deta_jets", 'F');
			//factory1->AddVariable( "min(abs(bhmv3-140),400)", "bhmv3", "bhmv3", 'F');		
			//factory1->AddVariable( "pca_var2",  "pca_var2",  "pca_var2",  'F');
			//factory1->AddVariable( "eig0var", "eig0var", "eig0var", 'F');
			//factory1->AddVariable( "eigs_0_1_2_gauss", "eigs_0_1_2_gauss", "eigs_0_1_2_gauss", 'F');
			
			break;
			
			
		case 423:
			
			factory1->AddVariable( "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", 'F'); 
			//factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');
			
			//factory1->AddVariable( "tight_lepton_pt", "tight_lepton_pt", "tight_lepton_pt", 'F');
			//factory1->AddVariable( "aplanarity", "aplanarity", "aplanarity", 'F');
			//factory1->AddVariable( "MET", "MET", "MET", 'F');
			//factory1->AddVariable( "MET_phi", "MET_phi", "MET_phi", 'F');
			factory1->AddVariable( "first_jet_pt", "first_jet_pt", "first_jet_pt", 'F');
			factory1->AddVariable( "second_jet_pt", "second_jet_pt", "second_jet_pt", 'F');
			factory1->AddVariable( "third_jet_pt", "third_jet_pt", "third_jet_pt", 'F');  // rm 5 best sep
			factory1->AddVariable( "fourth_jet_pt", "fourth_jet_pt", "fourth_jet_pt", 'F');  // rm 5 best sep									// add
			//factory1->AddVariable( "h0", "h0", "h0", 'F');
			//factory1->AddVariable( "h1", "h1", "h1", 'F');
			////factory1->AddVariable( "all_sum_pt", "all_sum_pt", "all_sum_pt", 'F');
			factory1->AddVariable( "all_sum_pt_with_met", "all_sum_pt_with_met", "all_sum_pt_with_met", 'F');
			factory1->AddVariable( "HT", "HT", "HT", 'F');
			factory1->AddVariable( "MHT", "MHT", "MHT", 'F');  // rm 5 best sep
			//factory1->AddVariable( "MHT_phi", "MHT_phi", "MHT_phi", 'F');
			////factory1->AddVariable( "uMET", "", "", 'F');
			////factory1->AddVariable( "uMET_phi", "", "", 'F');
			//factory1->AddVariable( "avg_dr_tagged_jets", "avg_dr_tagged_jets", "avg_dr_tagged_jets", 'F');
			//factory1->AddVariable( "sphericity", "sphericity", "sphericity", 'F');
			//factory1->AddVariable( "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", 'F');
			factory1->AddVariable( "avg_btag_disc_btags", "avg_btag_disc_btags", "avg_btag_disc_btags", 'F');	// rm 8 best sep					// add
			//factory1->AddVariable( "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", 'F');
			//factory1->AddVariable( "h2", "h2", "h2", 'F');
			//factory1->AddVariable( "lowest_btag", "lowest_btag", "lowest_btag", 'F');
			//factory1->AddVariable( "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F');			
			//factory1->AddVariable( "h3", "h3", "h3", 'F');
			//factory1->AddVariable( "h4", "h4", "h4", 'F');
			//factory1->AddVariable( "second_highest_btag", "second_highest_btag", "second_highest_btag", 'F');
			factory1->AddVariable( "third_highest_btag", "third_highest_btag", "third_highest_btag", 'F');	// rm 8 best sep						// add
			//factory1->AddVariable( "fourth_highest_btag", "fourth_highest_btag", "fourth_highest_btag", 'F');
			//factory1->AddVariable( "invariant_mass_of_everything", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F');			// rm
			
			//factory1->AddVariable( "maxeta_jet_jet", "maxeta_jet_jet", "maxeta_jet_jet", 'F' );
			//factory1->AddVariable( "maxeta_jet_tag", "maxeta_jet_tag", "maxeta_jet_tag", 'F' );
			//factory1->AddVariable( "maxeta_tag_tag", "maxeta_tag_tag", "maxeta_tag_tag", 'F' );
			
			//factory1->AddVariable( "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", 'F');
			//factory1->AddVariable( "M3", "M3", "M3", 'F');												// rm
			//factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');											// rm
			//factory1->AddVariable( "Mlb", "Mlb", "Mlb", 'F');
			//factory1->AddVariable( "first_highest_btag", "first_highest_btag", "first_highest_btag", 'F');
			// //factory1->AddVariable( "dRbb_bhmv2", "dRbb_bhmv2", "dRbb_bhmv2", 'F');
			//factory1->AddVariable( "median_bb_mass", "median_bb_mass", "median_bb_mass", 'F' );
			//factory1->AddVariable( "min_dr_tagged_jets", "min_dr_tagged_jets", "min_dr_tagged_jets", 'F');
			//factory1->AddVariable( "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", 'F');
			//factory1->AddVariable( "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", 'F');
			////factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');				
			//factory1->AddVariable( "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", 'F');	
			////factory1->AddVariable( "minChi2_v2", "minChi2_v2", "minChi2_v2", 'F');
			factory1->AddVariable( "avg_abs_deta_jets", "avg_abs_deta_jets", "avg_abs_deta_jets", 'F');
			break;
			
		case 543:
			
			//factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');
			
			
			//factory1->AddVariable( "tight_lepton_pt", "tight_lepton_pt", "tight_lepton_pt", 'F');
			//factory1->AddVariable( "aplanarity", "aplanarity", "aplanarity", 'F');
			//factory1->AddVariable( "MET", "MET", "MET", 'F');
			//factory1->AddVariable( "MET_phi", "MET_phi", "MET_phi", 'F');
			//factory1->AddVariable( "first_jet_pt", "first_jet_pt", "first_jet_pt", 'F');
			//factory1->AddVariable( "second_jet_pt", "second_jet_pt", "second_jet_pt", 'F');
			//factory1->AddVariable( "third_jet_pt", "third_jet_pt", "third_jet_pt", 'F');
			//factory1->AddVariable( "fourth_jet_pt", "fourth_jet_pt", "fourth_jet_pt", 'F');
			//factory1->AddVariable( "h0", "h0", "h0", 'F');
			//factory1->AddVariable( "h1", "h1", "h1", 'F');
			////factory1->AddVariable( "all_sum_pt", "all_sum_pt", "all_sum_pt", 'F');
			factory1->AddVariable( "all_sum_pt_with_met", "all_sum_pt_with_met", "all_sum_pt_with_met", 'F'); // rm 5 best sep
			//factory1->AddVariable( "HT", "HT", "HT", 'F');
			//factory1->AddVariable( "MHT", "MHT", "MHT", 'F');
			//factory1->AddVariable( "MHT_phi", "MHT_phi", "MHT_phi", 'F');
			////factory1->AddVariable( "uMET", "", "", 'F');
			////factory1->AddVariable( "uMET_phi", "", "", 'F');
			factory1->AddVariable( "avg_dr_tagged_jets", "avg_dr_tagged_jets", "avg_dr_tagged_jets", 'F');
			//factory1->AddVariable( "sphericity", "sphericity", "sphericity", 'F');
			//factory1->AddVariable( "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", 'F');
			factory1->AddVariable( "avg_btag_disc_btags", "avg_btag_disc_btags", "avg_btag_disc_btags", 'F');
			factory1->AddVariable( "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", 'F'); // rm 5 best sep
			//factory1->AddVariable( "h2", "h2", "h2", 'F');
			factory1->AddVariable( "lowest_btag", "lowest_btag", "lowest_btag", 'F');
			//factory1->AddVariable( "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F');			
			//factory1->AddVariable( "h3", "h3", "h3", 'F');
			//factory1->AddVariable( "h4", "h4", "h4", 'F');
			factory1->AddVariable( "second_highest_btag", "second_highest_btag", "second_highest_btag", 'F'); // rm 6/21
			factory1->AddVariable( "third_highest_btag", "third_highest_btag", "third_highest_btag", 'F');
			//factory1->AddVariable( "fourth_highest_btag", "fourth_highest_btag", "fourth_highest_btag", 'F');
			//factory1->AddVariable( "invariant_mass_of_everything", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F');
			
			//factory1->AddVariable( "maxeta_jet_jet", "maxeta_jet_jet", "maxeta_jet_jet", 'F' );
			factory1->AddVariable( "maxeta_jet_tag", "maxeta_jet_tag", "maxeta_jet_tag", 'F' ); // rm 5 best sep
			//factory1->AddVariable( "maxeta_tag_tag", "maxeta_tag_tag", "maxeta_tag_tag", 'F' ); // rm 8 best sep
			
			//factory1->AddVariable( "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", 'F');
			//factory1->AddVariable( "M3", "M3", "M3", 'F');
			////factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			//factory1->AddVariable( "Mlb", "Mlb", "Mlb", 'F');
			//factory1->AddVariable( "first_highest_btag", "first_highest_btag", "first_highest_btag", 'F');
			// //factory1->AddVariable( "dRbb_bhmv2", "dRbb_bhmv2", "dRbb_bhmv2", 'F');
			//factory1->AddVariable( "median_bb_mass", "median_bb_mass", "median_bb_mass", 'F' );
			//factory1->AddVariable( "min_dr_tagged_jets", "min_dr_tagged_jets", "min_dr_tagged_jets", 'F');
			factory1->AddVariable( "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", 'F'); // rm 8 best sep
			//factory1->AddVariable( "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", 'F');
			////factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');				
			//factory1->AddVariable( "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", 'F');	
			////factory1->AddVariable( "minChi2_v2", "minChi2_v2", "minChi2_v2", 'F');
			//factory1->AddVariable( "best_higgs_mass","best_higgs_mass","best_higgs_mass",'F'); // rm to make 10 best sep
			
			factory1->AddVariable( "ttbb_tth_MVA_output_5j4t","ttbb_tth_MVA_output_5j4t","ttbb_tth_MVA_output_5j4t", 'F');
			
			//factory1->AddVariable( "avg_abs_deta_jets", "avg_abs_deta_jets", "avg_abs_deta_jets", 'F'); // p-exp tests
			//factory1->AddVariable( "eigs_0_1_2_gauss_weighted","eigs_0_1_2_gauss_weighted","eigs_0_1_2_gauss_weighted", 'F'); // p-exp tests
			
			break;
		
		
		case 443:
			//factory1->AddVariable( "tight_lepton_pt", "tight_lepton_pt", "tight_lepton_pt", 'F');
			//factory1->AddVariable( "aplanarity", "aplanarity", "aplanarity", 'F');
			//factory1->AddVariable( "MET", "MET", "MET", 'F');
			//factory1->AddVariable( "MET_phi", "MET_phi", "MET_phi", 'F');
			factory1->AddVariable( "first_jet_pt", "first_jet_pt", "first_jet_pt", 'F'); // rm 5 best sep
			factory1->AddVariable( "second_jet_pt", "second_jet_pt", "second_jet_pt", 'F'); // rm 5 best sep
			//factory1->AddVariable( "third_jet_pt", "third_jet_pt", "third_jet_pt", 'F');
			factory1->AddVariable( "fourth_jet_pt", "fourth_jet_pt", "fourth_jet_pt", 'F'); // rm 8 best sep
			//factory1->AddVariable( "h0", "h0", "h0", 'F');
			//factory1->AddVariable( "h1", "h1", "h1", 'F');
			////factory1->AddVariable( "all_sum_pt", "all_sum_pt", "all_sum_pt", 'F');
			factory1->AddVariable( "all_sum_pt_with_met", "all_sum_pt_with_met", "all_sum_pt_with_met", 'F');
			factory1->AddVariable( "HT", "HT", "HT", 'F');
			//factory1->AddVariable( "MHT", "MHT", "MHT", 'F');
			//factory1->AddVariable( "MHT_phi", "MHT_phi", "MHT_phi", 'F');
			////factory1->AddVariable( "uMET", "", "", 'F');
			////factory1->AddVariable( "uMET_phi", "", "", 'F');
			//factory1->AddVariable( "avg_dr_tagged_jets", "avg_dr_tagged_jets", "avg_dr_tagged_jets", 'F');
			//factory1->AddVariable( "sphericity", "sphericity", "sphericity", 'F');
			//factory1->AddVariable( "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", 'F');
			factory1->AddVariable( "avg_btag_disc_btags", "avg_btag_disc_btags", "avg_btag_disc_btags", 'F');
			//factory1->AddVariable( "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", 'F');
			//factory1->AddVariable( "h2", "h2", "h2", 'F');
			//factory1->AddVariable( "lowest_btag", "lowest_btag", "lowest_btag", 'F');
			//factory1->AddVariable( "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F');			
			//factory1->AddVariable( "h3", "h3", "h3", 'F');
			//factory1->AddVariable( "h4", "h4", "h4", 'F');
			factory1->AddVariable( "second_highest_btag", "second_highest_btag", "second_highest_btag", 'F'); // rm 5 best sep
			factory1->AddVariable( "third_highest_btag", "third_highest_btag", "third_highest_btag", 'F');
			factory1->AddVariable( "fourth_highest_btag", "fourth_highest_btag", "fourth_highest_btag", 'F');
			//factory1->AddVariable( "invariant_mass_of_everything", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F');
			
			//factory1->AddVariable( "maxeta_jet_jet", "maxeta_jet_jet", "maxeta_jet_jet", 'F' );
			//factory1->AddVariable( "maxeta_jet_tag", "maxeta_jet_tag", "maxeta_jet_tag", 'F' );
			//factory1->AddVariable( "maxeta_tag_tag", "maxeta_tag_tag", "maxeta_tag_tag", 'F' );
			
			//factory1->AddVariable( "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", "avg_tagged_dijet_mass", 'F');
			factory1->AddVariable( "M3", "M3", "M3", 'F'); // rm 8 best sep
			////factory1->AddVariable( "M3_1tag", "M3_1tag", "M3_1tag", 'F');
			//factory1->AddVariable( "Mlb", "Mlb", "Mlb", 'F');
			//factory1->AddVariable( "first_highest_btag", "first_highest_btag", "first_highest_btag", 'F');
			// //factory1->AddVariable( "dRbb_bhmv2", "dRbb_bhmv2", "dRbb_bhmv2", 'F');
			//factory1->AddVariable( "median_bb_mass", "median_bb_mass", "median_bb_mass", 'F' );
			//factory1->AddVariable( "min_dr_tagged_jets", "min_dr_tagged_jets", "min_dr_tagged_jets", 'F');
			//factory1->AddVariable( "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", 'F');
			//factory1->AddVariable( "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", "ttbb_tth_ANN_output", 'F');
			////factory1->AddVariable( "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", "avg_btag_disc_non_btags", 'F');				
			////factory1->AddVariable( "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", 'F');	
			//factory1->AddVariable( "minChi2_v2", "minChi2_v2", "minChi2_v2", 'F');
			
			//factory1->AddVariable( "avg_abs_deta_jets", "avg_abs_deta_jets", "avg_abs_deta_jets", 'F');
			
			break;

			
			
			
		case 640203: // 6402 pruned 4	// removing poorly modeled vars. Then successively removing vars based on separation of samples with new ttbb splitting	
			
			factory1->AddVariable( "avg_dr_tagged_jets", "avg_dr_tagged_jets", "avg_dr_tagged_jets", 'F' );						// modeling = fine	// top 10 sep
			factory1->AddVariable( "sphericity", "sphericity", "sphericity", 'F' );									// modeling = fine	// top 15 sep
//			factory1->AddVariable( "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", 'F' );	// modeling = fine	
			factory1->AddVariable( "avg_btag_disc_btags", "avg_btag_disc_btags", "avg_btag_disc_btags", 'F' );					// modeling = fine	// swapped into top 15 sep 5/27
//			factory1->AddVariable( "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", 'F' );				// modeling = fine	
//			factory1->AddVariable( "h2", "h2", "h2", 'F' );												// modeling = ok	
//			factory1->AddVariable( "lowest_btag","lowest_btag","lowest_btag", 'F');									// modeling = fine	
//			factory1->AddVariable( "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", 'F' );				// modeling = fine	
		//	factory1->AddVariable( "min(closest_tagged_dijet_mass,200)", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F' );		
			factory1->AddVariable( "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F' );			// modeling = fine	// top 10 imp	// rm for 8 best sep
			factory1->AddVariable( "h3", "h3", "h3", 'F' );												// modeling = fine	// top 10 sep	// rm for 5 best sep
//			factory1->AddVariable( "second_highest_btag","second_highest_btag","second_highest_btag", 'F');						// modeling = fine	
		//	factory1->AddVariable( "min(invariant_mass_of_everything,2000)", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F' );	
			factory1->AddVariable( "invariant_mass_of_everything", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F' );		// modeling = fine	// top 15 sep
		//	factory1->AddVariable( "min(best_higgs_mass,500)", "best_higgs_mass", "best_higgs_mass", 'F' );								
			factory1->AddVariable( "best_higgs_mass", "best_higgs_mass", "best_higgs_mass", 'F' );							// modeling = fine	// top 10 sep
//			factory1->AddVariable( "log(minChi2)", "log_minChi2", "log_minChi2", 'F' );								// modeling = ?		
		
		
			factory1->AddVariable( "dRbb", "dRbb", "dRbb", 'F' );											// modeling = fine	// top 10 imp	// rm for 5 best sep
		//	factory1->AddVariable( "min(bhmv2,500)", "bhmv2", "bhmv2", 'F' );		
		//	factory1->AddVariable( "bhmv2", "bhmv2", "bhmv2", 'F' );										// modeling = fine	// leaving out
//			factory1->AddVariable( "dRbb_bhmv2", "dRbb_bhmv2", "dRbb_bhmv2", 'F' );									// modeling = prob. fine	
		//	factory1->AddVariable( "thing := (bhmv2-125)/(bhmv2+125)", "thing", "thing", 'F' );
//			factory1->AddVariable( "maxeta_jet_jet", "maxeta_jet_jet", "maxeta_jet_jet", 'F' );							// modeling = fine
			factory1->AddVariable( "maxeta_jet_tag", "maxeta_jet_tag", "maxeta_jet_tag", 'F' );							// modeling = ok	// top 10 sep
			factory1->AddVariable( "maxeta_tag_tag", "maxeta_tag_tag", "maxeta_tag_tag", 'F' );							// modeling = ok	// top 11 sep
			factory1->AddVariable( "abs(dEta_leptop_bb)", "abs_dEta_leptop_bb", "abs_dEta_leptop_bb", 'F' );					// modeling = fine	// top 10 sep	// rm for 8 best sep
			factory1->AddVariable( "abs(dEta_hadtop_bb)", "abs_dEta_hadtop_bb", "abs_dEta_hadtop_bb", 'F' );					// modeling = fine	// top 10 sep
//			factory1->AddVariable( "dPhi_fn", "dPhi_fn", "dPhi_fn", 'F' );										// modeling = ?		
			factory1->AddVariable( "dEta_fn", "dEta_fn", "dEta_fn", 'F' );										// modeling = fine	// top 10 sep	// rm for 5 best sep
//			factory1->AddVariable( "avg_eta_tops", "avg_eta_tops", "avg_eta_tops", 'F' );								// modeling = ?
		//	factory1->AddVariable( "abs(bb_eta)", "abs_bb_eta", "abs_bb_eta", 'F' );								// modeling = not fine
//			factory1->AddVariable( "angle_tops_bb", "angle_tops_bb", "angle_tops_bb", 'F' ); 							// modeling = fine	// top 15 sep 	// swapped out of top 15 sep 5/27 (modeling)
			
		//	factory1->AddVariable( "min(median_bb_mass,400)", "median_bb_mass", "median_bb_mass", 'F' );
			factory1->AddVariable( "median_bb_mass", "median_bb_mass", "median_bb_mass", 'F' );							// modeling = fine	// top 15 sep
			
			
//			factory1->AddVariable( "min_dr_tagged_jets", "min_dr_tagged_jets", "min_dr_tagged_jets", 'F');						// modeling = fine
			
			// added 3/7:
			factory1->AddVariable( "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", 'F');		// modeling = fine	// top 10 sep
			
			// added 3/8:
//			factory1->AddVariable( "aplanarity","aplanarity","aplanarity",'F');									// modeling = fine
//			factory1->AddVariable( "MET","MET","MET",'F');												// modeling = ok
//			factory1->AddVariable( "MET_phi","MET_phi","MET_phi",'F');										// modeling = fine
//			factory1->AddVariable( "first_jet_pt","first_jet_pt","first_jet_pt",'F');								// modeling = fine
//			factory1->AddVariable( "second_jet_pt","second_jet_pt","second_jet_pt",'F');								// modeling = fine
//			factory1->AddVariable( "third_jet_pt","third_jet_pt","third_jet_pt",'F');								// modeling = fine
//			factory1->AddVariable( "fourth_jet_pt","fourth_jet_pt","fourth_jet_pt",'F');								// modeling = fine
//			factory1->AddVariable( "h0","h0","h0",'F');												// modeling = ok
//			factory1->AddVariable( "h1","h1","h1",'F');												// modeling = questionable
//			factory1->AddVariable( "all_sum_pt_with_met","all_sum_pt_with_met","all_sum_pt_with_met",'F');						// modeling = fine
//			factory1->AddVariable( "HT","HT","HT",'F');												// modeling = fine
//			factory1->AddVariable( "MHT","MHT","MHT",'F');												// modeling = fine
//			factory1->AddVariable( "MHT_phi","MHT_phi","MHT_phi",'F');										// modeling = fine
			
			// added 4/22:
		//	factory1->AddVariable( "ttbb_tth_ANN_output","ttbb_tth_ANN_output","ttbb_tth_ANN_output", 'F' );
		//	factory1->AddVariable( "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", 'F'); // modeling = not fine
			
			// added 9/20:
			//factory1->AddVariable( "avg_abs_deta_jets", "avg_abs_deta_jets", "avg_abs_deta_jets", 'F');
			//factory1->AddVariable( "eigs_0_1_2_gauss", "eigs_0_1_2_gauss", "eigs_0_1_2_gauss", 'F');
			
			
			break;
			
		
		case 630000:
			
			//factory1->AddVariable( "numJets","numJets","numJets",'F');
			//factory1->AddVariable( "avg_dr_tagged_jets","avg_dr_tagged_jets","avg_dr_tagged_jets",'F'); //  not great // rm 6/21
			factory1->AddVariable( "sphericity","sphericity","sphericity",'F'); // ok
			factory1->AddVariable( "h3","h3","h3",'F'); // ok
			factory1->AddVariable( "maxeta_jet_jet","maxeta_jet_jet","maxeta_jet_jet",'F'); // ok
			factory1->AddVariable( "maxeta_jet_tag","maxeta_jet_tag","maxeta_jet_tag",'F'); // ok
			factory1->AddVariable( "maxeta_tag_tag","maxeta_tag_tag","maxeta_tag_tag",'F'); // ok
			factory1->AddVariable( "abs(dEta_leptop_bb)","abs_dEta_leptop_bb","abs_dEta_leptop_bb",'F'); // ok
			factory1->AddVariable( "abs(dEta_hadtop_bb)","abs_dEta_hadtop_bb","abs_dEta_hadtop_bb",'F'); // ok
			factory1->AddVariable( "dEta_fn","dEta_fn","dEta_fn",'F'); //ok
			factory1->AddVariable( "M3","M3","M3",'F'); // ok
			factory1->AddVariable( "min_dr_tagged_jets","min_dr_tagged_jets","min_dr_tagged_jets",'F'); // ok
			factory1->AddVariable( "pt_all_jets_over_E_all_jets","pt_all_jets_over_E_all_jets","pt_all_jets_over_E_all_jets",'F'); // ok
			factory1->AddVariable( "aplanarity","aplanarity","aplanarity",'F'); // ok
			factory1->AddVariable( "h1","h1","h1",'F'); // ok
			factory1->AddVariable( "tagged_dijet_mass_closest_to_125","tagged_dijet_mass_closest_to_125","tagged_dijet_mass_closest_to_125",'F'); // ok
			
			factory1->AddVariable( "third_jet_pt","third_jet_pt","third_jet_pt",'F'); // added 6/21

			break;
			
			
		case 540000:
			
			factory1->AddVariable( "avg_dr_tagged_jets","avg_dr_tagged_jets","avg_dr_tagged_jets",'F');  // ok
			factory1->AddVariable( "maxeta_tag_tag","maxeta_tag_tag","maxeta_tag_tag",'F');  // ok
			factory1->AddVariable( "h1","h1","h1",'F');  //  ?
			
			//factory1->AddVariable( "h2","h2","h2",'F');  //  added 6/21
			
			factory1->AddVariable( "h3","h3","h3",'F');  //  ok
			factory1->AddVariable( "all_sum_pt_with_met","all_sum_pt_with_met","all_sum_pt_with_met",'F');  // ok
			//factory1->AddVariable( "HT","HT","HT",'F'); // ok
			//factory1->AddVariable( "maxeta_jet_tag","maxeta_jet_tag","maxeta_jet_tag",'F'); // ok
			factory1->AddVariable( "tagged_dijet_mass_closest_to_125","tagged_dijet_mass_closest_to_125","tagged_dijet_mass_closest_to_125",'F'); // rm (grumble) // ha -- don't remove
			factory1->AddVariable( "MET","MET","MET",'F'); // ok
			factory1->AddVariable( "aplanarity","aplanarity","aplanarity",'F'); // ok
			factory1->AddVariable( "pt_all_jets_over_E_all_jets","pt_all_jets_over_E_all_jets","pt_all_jets_over_E_all_jets",'F'); // ok
			factory1->AddVariable( "fourth_highest_btag","fourth_highest_btag","fourth_highest_btag",'F'); //
			//factory1->AddVariable( "fourth_jet_pt","fourth_jet_pt","fourth_jet_pt",'F'); //
			
			//factory1->AddVariable( "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F' ); // added 6/21
			//factory1->AddVariable( "eigs_0_1_2_gauss_weighted","eigs_0_1_2_gauss_weighted","eigs_0_1_2_gauss_weighted", 'F');
			
			//factory1->AddVariable( "pca_var1","pca_var1","pca_var1", 'F');
			//factory1->AddVariable( "pca_var2","pca_var2","pca_var2", 'F');
			//factory1->AddVariable( "pca_var3","pca_var3","pca_var3", 'F');
			
			break;
			
			
			
		case 643203: // superset of 640203 and 643 variables
			
			
			factory1->AddVariable( "avg_dr_tagged_jets", "avg_dr_tagged_jets", "avg_dr_tagged_jets", 'F' );						// modeling = fine	// top 10 sep
			factory1->AddVariable( "sphericity", "sphericity", "sphericity", 'F' );									// modeling = fine	// top 15 sep
//			factory1->AddVariable( "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", "dr_between_lep_and_closest_jet", 'F' );	// modeling = fine	
			factory1->AddVariable( "avg_btag_disc_btags", "avg_btag_disc_btags", "avg_btag_disc_btags", 'F' );					// modeling = fine	// swapped into top 15 sep 5/27
//			factory1->AddVariable( "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", "dev_from_avg_disc_btags", 'F' );				// modeling = fine	
//			factory1->AddVariable( "h2", "h2", "h2", 'F' );												// modeling = ok	
//			factory1->AddVariable( "lowest_btag","lowest_btag","lowest_btag", 'F');									// modeling = fine	
//			factory1->AddVariable( "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", "avg_untagged_dijet_mass", 'F' );				// modeling = fine	
		//	factory1->AddVariable( "min(closest_tagged_dijet_mass,200)", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F' );		
			factory1->AddVariable( "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", "closest_tagged_dijet_mass", 'F' );			// modeling = fine	// top 10 imp	// rm for 8 best sep
			factory1->AddVariable( "h3", "h3", "h3", 'F' );												// modeling = fine	// top 10 sep	// rm for 5 best sep
//			factory1->AddVariable( "second_highest_btag","second_highest_btag","second_highest_btag", 'F');						// modeling = fine	
		//	factory1->AddVariable( "min(invariant_mass_of_everything,2000)", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F' );	
			factory1->AddVariable( "invariant_mass_of_everything", "invariant_mass_of_everything", "invariant_mass_of_everything", 'F' );		// modeling = fine	// top 15 sep
		//	factory1->AddVariable( "min(best_higgs_mass,500)", "best_higgs_mass", "best_higgs_mass", 'F' );								
			factory1->AddVariable( "best_higgs_mass", "best_higgs_mass", "best_higgs_mass", 'F' );							// modeling = fine	// top 10 sep
//			factory1->AddVariable( "log(minChi2)", "log_minChi2", "log_minChi2", 'F' );								// modeling = ?		
		
		
			factory1->AddVariable( "dRbb", "dRbb", "dRbb", 'F' );											// modeling = fine	// top 10 imp	// rm for 5 best sep
		//	factory1->AddVariable( "min(bhmv2,500)", "bhmv2", "bhmv2", 'F' );		
		//	factory1->AddVariable( "bhmv2", "bhmv2", "bhmv2", 'F' );										// modeling = fine	// leaving out
//			factory1->AddVariable( "dRbb_bhmv2", "dRbb_bhmv2", "dRbb_bhmv2", 'F' );									// modeling = prob. fine	
		//	factory1->AddVariable( "thing := (bhmv2-125)/(bhmv2+125)", "thing", "thing", 'F' );
//			factory1->AddVariable( "maxeta_jet_jet", "maxeta_jet_jet", "maxeta_jet_jet", 'F' );							// modeling = fine
			factory1->AddVariable( "maxeta_jet_tag", "maxeta_jet_tag", "maxeta_jet_tag", 'F' );							// modeling = ok	// top 10 sep
			factory1->AddVariable( "maxeta_tag_tag", "maxeta_tag_tag", "maxeta_tag_tag", 'F' );							// modeling = ok	// top 11 sep
			factory1->AddVariable( "abs(dEta_leptop_bb)", "abs_dEta_leptop_bb", "abs_dEta_leptop_bb", 'F' );					// modeling = fine	// top 10 sep	// rm for 8 best sep
			factory1->AddVariable( "abs(dEta_hadtop_bb)", "abs_dEta_hadtop_bb", "abs_dEta_hadtop_bb", 'F' );					// modeling = fine	// top 10 sep
//			factory1->AddVariable( "dPhi_fn", "dPhi_fn", "dPhi_fn", 'F' );										// modeling = ?		
			factory1->AddVariable( "dEta_fn", "dEta_fn", "dEta_fn", 'F' );										// modeling = fine	// top 10 sep	// rm for 5 best sep
//			factory1->AddVariable( "avg_eta_tops", "avg_eta_tops", "avg_eta_tops", 'F' );								// modeling = ?
		//	factory1->AddVariable( "abs(bb_eta)", "abs_bb_eta", "abs_bb_eta", 'F' );								// modeling = not fine
//			factory1->AddVariable( "angle_tops_bb", "angle_tops_bb", "angle_tops_bb", 'F' ); 							// modeling = fine	// top 15 sep 	// swapped out of top 15 sep 5/27 (modeling)
			
		//	factory1->AddVariable( "min(median_bb_mass,400)", "median_bb_mass", "median_bb_mass", 'F' );
			factory1->AddVariable( "median_bb_mass", "median_bb_mass", "median_bb_mass", 'F' );							// modeling = fine	// top 15 sep
			
			
//			factory1->AddVariable( "min_dr_tagged_jets", "min_dr_tagged_jets", "min_dr_tagged_jets", 'F');						// modeling = fine
			
			// added 3/7:
			factory1->AddVariable( "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", "pt_all_jets_over_E_all_jets", 'F');		// modeling = fine	// top 10 sep
			
			// added 3/8:
//			factory1->AddVariable( "aplanarity","aplanarity","aplanarity",'F');									// modeling = fine
//			factory1->AddVariable( "MET","MET","MET",'F');												// modeling = ok
//			factory1->AddVariable( "MET_phi","MET_phi","MET_phi",'F');										// modeling = fine
//			factory1->AddVariable( "first_jet_pt","first_jet_pt","first_jet_pt",'F');								// modeling = fine
//			factory1->AddVariable( "second_jet_pt","second_jet_pt","second_jet_pt",'F');								// modeling = fine
//			factory1->AddVariable( "third_jet_pt","third_jet_pt","third_jet_pt",'F');								// modeling = fine
//			factory1->AddVariable( "fourth_jet_pt","fourth_jet_pt","fourth_jet_pt",'F');								// modeling = fine
//			factory1->AddVariable( "h0","h0","h0",'F');												// modeling = ok
//			factory1->AddVariable( "h1","h1","h1",'F');												// modeling = questionable
//			factory1->AddVariable( "all_sum_pt_with_met","all_sum_pt_with_met","all_sum_pt_with_met",'F');						// modeling = fine
//			factory1->AddVariable( "HT","HT","HT",'F');												// modeling = fine
//			factory1->AddVariable( "MHT","MHT","MHT",'F');												// modeling = fine
//			factory1->AddVariable( "MHT_phi","MHT_phi","MHT_phi",'F');										// modeling = fine
			
			// added 4/22:
		//	factory1->AddVariable( "ttbb_tth_ANN_output","ttbb_tth_ANN_output","ttbb_tth_ANN_output", 'F' );
		//	factory1->AddVariable( "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", "tagged_dijet_mass_closest_to_125", 'F'); // modeling = not fine
			
			
		//	factory1->AddVariable( "third_highest_btag", "third_highest_btag", "third_highest_btag", 'F');
		//	factory1->AddVariable( "fourth_highest_btag", "fourth_highest_btag", "fourth_highest_btag", 'F');
			
			break;	
	
	}


	TTree *new_tree_sig = (TTree*)signal.Clone();
	TTree *new_tree_back = (TTree*)background.Clone();
	TTree *new_tree_back2;
	
	int scaled_num = floor( backnum_nocut2 * (double)backnum / (double)backnum2 );
	int debug_count = 0;
	
	// added following line after commenting out lines below ...
	new_tree_back2 = (TTree*)background2.Clone();
	
	
	
	const TCut thecut = cut.c_str();	

	// global event weights per tree --> set to 1 here. Choice to weight/not weight controlled via "equal_evts_by_wgt" bool)
	Double_t signalWeight     = 1.0;
	Double_t backgroundWeight = 1.0;


	///------ now enter samples into factory: -------

	if (!two_bkds)
	{
		factory1->AddSignalTree    ( new_tree_sig, signalWeight );
		factory1->AddBackgroundTree( new_tree_back, backgroundWeight );
	}
	
	if (two_bkds)
	{
		gROOT->cd( outfileName1+TString(":/") );
		factory1->AddSignalTree	( new_tree_sig, signalWeight );
		factory1->AddTree	( new_tree_back, "bg0", backgroundWeight );
		factory1->AddTree	( new_tree_back2, "bg1", backgroundWeight );
	}
	
	if (equal_evts_by_wgt)
	{
		if (!two_bkds) factory1->PrepareTrainingAndTestTree( thecut, thecut, "nTrain_signal=0:nTrain_Background=0:SplitMode=Random:SplitSeed=0:NormMode=EqualNumEvents:V" );
		if (two_bkds) factory1->PrepareTrainingAndTestTree( thecut, "SplitMode=Random:SplitSeed=0:NormMode=EqualNumEvents:V" );
	}
	if (!equal_evts_by_wgt) // <- right now code below isn't set up for >1 background && (!equal_evts_by_wgt).
	{		
				
		Int_t half_num = smallest_amount / 2;  // half for training, half for testing.		
		Int_t half_num_scale_sig = half_num*( (Double_t)signum_nocut / (Double_t)signum ); 
		Int_t half_num_scale_back = half_num*( (Double_t)backnum_nocut / (Double_t)backnum );
		
		/*
		if (two_bkds)
		{
			half_num = smallest_amount;
			half_num_scale_sig = half_num*( (Double_t)signum_nocut / (Double_t)signum );
			half_num_scale_back = half_num*( (Double_t)(backnum_nocut+scaled_num) / (Double_t)(2*backnum) );
		}
		*/
		
		factory1->PrepareTrainingAndTestTree(thecut,half_num_scale_sig,half_num_scale_back,half_num_scale_sig,half_num_scale_back,":SplitMode=Random:SplitSeed=0:NormMode=NumEvents:V");
	}
	


	////----------------------------------------------


	bool cfmlpbool = true;
	if (equal_evts_by_wgt) cfmlpbool = !((signum>200000)||(backnum>200000));


	// Book methods:
	
	// TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
	if (Use["MLP"])
	{
		factory1->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=1000:HiddenLayers=N,N-1:TestRate=5:EpochMonitoring" ); // N,N-1  // 34 // 32   /// N+10,6 "H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=500:HiddenLayers=N,N-1:TestRate=5:EpochMonitoring"
	}

	// CF(Clermont-Ferrand)ANN
	if (Use["CFMlpANN"]&&cfmlpbool)// &&max_Events_<200000)
	{
		factory1->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=2000:HiddenLayers=N,N-1"  ); // N,N-1  // 34 // N+10,6
	}

	// Tmlp(Root)ANN
	if (Use["TMlpANN"])
	{
		factory1->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N:LearningMethod=BFGS:ValidationFraction=0.3"  );
	}

	if (Use["BDT"])  // BDT configuration from KIT group
	{
		//factory1->BookMethod( TMVA::Types::kBDT, "BDT","!H:!V:NTrees=250:BoostType=Grad:Shrinkage=0.1:UseBaggedGrad=kFalse::MaxDepth=4:nCuts=-1:NNodesMax=15:NegWeightTreatment=IgnoreNegWeights" ); // Shrinkage=0.5:UseBaggedGrad=kFalse::MaxDepth=4:nCuts=-1:NNodesMax=15: // test
		
		factory1->BookMethod( TMVA::Types::kBDT, "BDT","!H:!V:NTrees=100:BoostType=Grad:Shrinkage=0.1:UseBaggedGrad=kFalse::nCuts=20:NNodesMax=5:NegWeightTreatment=IgnoreNegWeights" ); // <- from Hannes
		
		//originally: "BDT", "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning"     // test
	}
	

   	// Support Vector Machine
   	if (Use["SVM"])
	{
      		factory1->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );
	}
	
	// N-dimensional likelihood (self-adaptive binning)

	if (Use["PDEFoam"])
	{
		factory1->BookMethod( TMVA::Types::kPDEFoam, "PDEFoam", "H:V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.2:nActiveCells=100:nSampl=5:nBin=5:Nmin=50:Kernel=None" );
	}
	
	if (Use["Likelihood"])
      	{
		factory1->BookMethod( TMVA::Types::kLikelihood, "Likelihood", "H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );
	}
			   
	if (Use["PDERS"])
      	{
		factory1->BookMethod( TMVA::Types::kPDERS, "PDERS", "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );	
	}
	
	if (Use["HMatrix"])
      	{
		factory1->BookMethod( TMVA::Types::kHMatrix, "HMatrix", "!H:!V" );
	}	   

	// ---- Now you can tell the factory to train, test, and evaluate the MVAs

	// Train MVAs using the set of training events
	factory1->TrainAllMethods();

	// ---- Evaluate all MVAs using the set of test events
	factory1->TestAllMethods();

	// ----- Evaluate and compare performance of all configured MVAs
	factory1->EvaluateAllMethods();


	outputFile1->Close();
	std::cout << "  " << std::endl;
	std::cout << "==> Wrote root file: " << outputFile1->GetName() << std::endl;
	std::cout << "  " << std::endl;


	// end factory

	// --------------------------------------------------------------



	std::cout << "  " << std::endl;
	std::cout << "==> TMVAClassification is done!" << std::endl;      


}


std::string selection(int cat_switch) //, bool isMuon)
{
	string sel_string = "(((eve.leptonType_==0)&&(eve.passElectronTrigger_))||((eve.leptonType_==1)&&(eve.passMuonTrigger_)))";
	
//	if (isMuon) sel_string = "(eve.leptonType_==1)&&(eve.passMuonTrigger_)";
	
	
	switch(cat_switch)
	{
		// nominal:
		case 42: sel_string = sel_string + "&&(eve.numJets_float_[0]==4)&&(eve.numTags_float_[0]==2)"; break;	// yggdrasil
		case 43: sel_string = sel_string + "&&(eve.numJets_float_[0]==4)&&(eve.numTags_float_[0]==3)"; break;	// yggdrasil
		case 44: sel_string = sel_string + "&&(eve.numJets_float_[0]==4)&&(eve.numTags_float_[0]==4)"; break;	// yggdrasil
		case 52: sel_string = sel_string + "&&(eve.numJets_float_[0]==5)&&(eve.numTags_float_[0]==2)"; break;	// yggdrasil
		case 53: sel_string = sel_string + "&&(eve.numJets_float_[0]==5)&&(eve.numTags_float_[0]==3)"; break;	// yggdrasil
		case 54: sel_string = sel_string + "&&(eve.numJets_float_[0]==5)&&(eve.numTags_float_[0]>=4)"; break;	// yggdrasil
		case 62: sel_string = sel_string + "&&(eve.numJets_float_[0]>=6)&&(eve.numTags_float_[0]==2)"; break;	// yggdrasil
		case 63: sel_string = sel_string + "&&(eve.numJets_float_[0]>=6)&&(eve.numTags_float_[0]==3)"; break;	// yggdrasil
		case 64: sel_string = sel_string + "&&(eve.numJets_float_[0]>=6)&&(eve.numTags_float_[0]>=4)"; break;	// yggdrasil
		
		// test with >=6 >=3:
		case 6364: sel_string = ""; break;									// custom trees
		case 636400: sel_string = ""; break;									// custom trees
		
		// test 64:
		case 6400: sel_string = ""; break;									// custom trees
		case 640000: sel_string = ""; break;
		case 6401: sel_string = ""; break;									// custom trees
		case 6402: sel_string = "(numJets>=6)&&(numTags>=4)"; break;						// custom trees
		case 640200: sel_string = ""; break;
		case 640201: sel_string = "(numJets>=6)&&(numTags>=4)"; break;
		case 640202: sel_string = "(numJets>=6)&&(numTags>=4)"; break;
		case 640203: sel_string = "(numJets>=6)&&(numTags>=4)"; break;
		
		case 640204: sel_string = "(numJets==6)&&(numTags>=4)"; break;
		
		case 6403: sel_string = ""; break;									// custom trees
		case 6404: sel_string = ""; break;									// custom trees
		case 6405: sel_string = ""; break;									// custom trees

		// test 54:
		case 540000: sel_string = "(numJets==5)&&(numTags>=4)"; break;
		
		// test 63:
		case 630000: sel_string = "(numJets>=6)&&(numTags==3)"; break;

											// custom trees
//		case 72: sel_string = sel_string + "&&(eve.numJets_float_[0]==7)&&(eve.numTags_float_[0]==2)"; break;
//		case 82: sel_string = sel_string + "&&(eve.numJets_float_[0]==8)&&(eve.numTags_float_[syst]==2); break;

		// for same vars in all cats, v1, v2, v3 of nominal cats:
		case 420: case 421: case 422: case 423: sel_string = "(numJets==4)&&(numTags==2)"; break;  // custom trees
		case 430: case 431: case 432: case 433: sel_string = "(numJets==4)&&(numTags==3)"; break;  // custom trees
		case 440: case 441: case 442: case 443: sel_string = "(numJets==4)&&(numTags==4)"; break;  // custom trees
		case 520: case 521: case 522: case 523: sel_string = "(numJets==5)&&(numTags==2)"; break;  // custom trees
		case 530: case 531: case 532: case 533: sel_string = "(numJets==5)&&(numTags==3)"; break;  // custom trees
		case 540: case 541: case 542: case 543: sel_string = "(numJets==5)&&(numTags>=4)"; break;  // custom trees
		case 620: case 621: case 622: case 623: sel_string = "(numJets>=6)&&(numTags==2)"; break;  // custom trees
		case 630: case 631: case 632: case 633: sel_string = "(numJets>=6)&&(numTags==3)"; break;  // custom trees
		case 640: case 641: case 642: case 643: sel_string = "(numJets>=6)&&(numTags>=4)"; break;  // custom trees
		
		case 644: sel_string = "(numJets==6)&&(numTags>=4)"; break;
		
		// adding maxjeteta vars:
		case 4201: sel_string = "(numJets==4)&&(numTags==2)"; break;	// custom trees
		case 4301: sel_string = "(numJets==4)&&(numTags==3)"; break;	// custom trees
		case 4401: sel_string = "(numJets==4)&&(numTags==4)"; break;	// custom trees
		case 5201: sel_string = "(numJets==5)&&(numTags==2)"; break;	// custom trees
		case 5301: sel_string = "(numJets==5)&&(numTags==3)"; break;	// custom trees
		case 5401: sel_string = "(numJets==5)&&(numTags>=4)"; break;  	// custom trees
		case 6201: sel_string = "(numJets>=6)&&(numTags==2)"; break;  	// custom trees
		case 6301: sel_string = "(numJets>=6)&&(numTags==3)"; break;	// custom trees
		case 6406: sel_string = "(numJets>=6)&&(numTags>=4)"; break;	// custom trees
		
		// superset of 643 and 640203:
		case 643203: sel_string = "(numJets>=6)&&(numTags>=4)"; break;	// custom trees

		default: break;
	}

	return sel_string;
}
