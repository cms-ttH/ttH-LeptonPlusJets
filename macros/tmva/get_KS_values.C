// Created by Geoff 5/18/13
//
// print KS test values:
// root -b -q 'get_KS_values.C+'


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

void get_KS(const char* file1 = "trained_file1.root");

void get_KS_values()
{

	cout << "printing KS values ... " << endl;
	cout << " " << endl;

	string dir = "";
	
	string filenamebase = "TMVA_trained_";
	
	
	string training1 = dir + filenamebase + "423" + ".root";
	string training2 = dir + filenamebase + "433" + ".root";
	string training3 = dir + filenamebase + "443" + ".root";
	string training4 = dir + filenamebase + "523" + ".root";
	string training5 = dir + filenamebase + "533" + ".root";
	string training6 = dir + filenamebase + "543" + ".root";
	string training7 = dir + filenamebase + "623" + ".root";
	string training8 = dir + filenamebase + "633" + ".root";
	string training9 = dir + filenamebase + "643" + ".root";
	string training10= dir + filenamebase + "640203" + ".root";
	string training11= dir + filenamebase + "540000" + ".root";
	string training12= dir + filenamebase + "630000" + ".root";


	cout << "KS values for " << training1 << ": " << endl;
	get_KS(training1.c_str());
	cout << "KS values for " << training2 << ": " << endl;
	get_KS(training2.c_str());
	cout << "KS values for " << training3 << ": " << endl;
	get_KS(training3.c_str());
	cout << "KS values for " << training4 << ": " << endl;
	get_KS(training4.c_str());
	cout << "KS values for " << training5 << ": " << endl;
	get_KS(training5.c_str());
	cout << "KS values for " << training6 << ": " << endl;
	get_KS(training6.c_str());
	cout << "KS values for " << training7 << ": " << endl;
	get_KS(training7.c_str());
	cout << "KS values for " << training8 << ": " << endl;
	get_KS(training8.c_str());
	cout << "KS values for " << training9 << ": " << endl;
	get_KS(training9.c_str());
	cout << "KS values for " << training10 << ": " << endl;
	get_KS(training10.c_str());
	cout << "KS values for " << training11 << ": " << endl;
	get_KS(training11.c_str());
	cout << "KS values for " << training12 << ": " << endl;
	get_KS(training12.c_str());




}
void get_KS(const char* file1)
{

	TFile *f1;
	f1 = new TFile(file1);

	if (f1->IsZombie())
	{
		cout << "skipping " << file1 << endl;
		cout << " " << endl;
		return;
	}
	else
	{
	
		//TString MVA = "CFMlpANN";
		TString MVA = "BDT";
	
		TH1F *sig_hist1 = (TH1F*)f1->Get("Method_" + MVA + "/" + MVA + "/MVA_" + MVA + "_S")->Clone();
		TH1F *sig_hist2 = (TH1F*)f1->Get("Method_" + MVA + "/" + MVA + "/MVA_" + MVA + "_Train_S")->Clone();
		TH1F *back_hist1 = (TH1F*)f1->Get("Method_" + MVA + "/" + MVA + "/MVA_" + MVA + "_B")->Clone();
		TH1F *back_hist2 = (TH1F*)f1->Get("Method_" + MVA + "/" + MVA + "/MVA_" + MVA + "_Train_B")->Clone();

		Double_t ks_sig = sig_hist1->KolmogorovTest(sig_hist2);
		Double_t ks_back = back_hist1->KolmogorovTest(back_hist2);


		cout << "KS for signal train vs. test: " << ks_sig << endl;
		cout << "KS for background train vs. test: " << ks_back << endl;
		cout << " " << endl;
	}

}
