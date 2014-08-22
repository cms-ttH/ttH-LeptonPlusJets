//Root includes                                   
#include "TROOT.h"
#include "Riostream.h"
#include "TFile.h"
#include "THStack.h"
#include "TH1.h"
#include "TF1.h"
#include "TH1F.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TTree.h"
#include "TString.h"
#include "TMath.h"
#include "TAxis.h"
#include "TKey.h"
#include "TList.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sstream>

void checkPurity_csvSF_13TeV( TString inputDYfile  = "infile1.root", TString inputTTfile  = "infile2.root", bool isHF = true ) {

  TFile *histFile_dyjets = TFile::Open(inputDYfile);
  TFile *histFile_ttjets = TFile::Open(inputTTfile);


  // single jet specific plots
  int nPt = 6;
  int nEta = 1;
  TString flavor = "HF";
  if ( !isHF ){
    nPt = 4; nEta = 3;
    flavor = "LF";
  }

  //////
  TH1D* h_dyjets_MC_b_jet_csv[nPt][nEta];
  TH1D* h_dyjets_MC_nonb_jet_csv[nPt][nEta];
  TH1D* h_ttjets_MC_b_jet_csv[nPt][nEta];
  TH1D* h_ttjets_MC_nonb_jet_csv[nPt][nEta];

  for ( int iPt=0; iPt<nPt; iPt++){
    for ( int iEta=0; iEta<nEta; iEta++){

      TString h_b_Name = Form("csv_MC_bjets_Pt%i_Eta%i",iPt,iEta);
      TString h_nonb_Name = Form("csv_MC_nonbjets_Pt%i_Eta%i",iPt,iEta);

      h_dyjets_MC_b_jet_csv[iPt][iEta] = (TH1D*)histFile_dyjets->Get(h_b_Name.Data());
      h_dyjets_MC_nonb_jet_csv[iPt][iEta] = (TH1D*)histFile_dyjets->Get(h_nonb_Name.Data());

      h_ttjets_MC_b_jet_csv[iPt][iEta] = (TH1D*)histFile_ttjets->Get(h_b_Name.Data());
      h_ttjets_MC_nonb_jet_csv[iPt][iEta] = (TH1D*)histFile_ttjets->Get(h_nonb_Name.Data());


      double err_dyjets_b=-1, err_dyjets_nonb=-1, err_ttjets_b=-1, err_ttjets_nonb=-1;

      int nBins = h_dyjets_MC_b_jet_csv[iPt][iEta]->GetNbinsX();

      double sum_dyjets_b = h_dyjets_MC_b_jet_csv[iPt][iEta]->IntegralAndError(0, nBins, err_dyjets_b);
      double sum_dyjets_nonb = h_dyjets_MC_nonb_jet_csv[iPt][iEta]->IntegralAndError(0, nBins, err_dyjets_nonb);
      double sum_dyjets = sum_dyjets_b + sum_dyjets_nonb;
      double sum_err_dyjets = sqrt( err_dyjets_b*err_dyjets_b + err_dyjets_nonb*err_dyjets_nonb );


      double sum_ttjets_b = h_ttjets_MC_b_jet_csv[iPt][iEta]->IntegralAndError(0, nBins, err_ttjets_b);
      double sum_ttjets_nonb = h_ttjets_MC_nonb_jet_csv[iPt][iEta]->IntegralAndError(0, nBins, err_ttjets_nonb);
      double sum_ttjets = sum_ttjets_b + sum_ttjets_nonb;
      double sum_err_ttjets = sqrt( err_ttjets_b*err_ttjets_b + err_ttjets_nonb*err_ttjets_nonb );

      double sum = sum_dyjets + sum_ttjets;
      double sum_err = sqrt( sum_err_dyjets*sum_err_dyjets + sum_err_ttjets*sum_err_ttjets );

      printf(" %s Pt%d Eta%d: \n", flavor.Data(), iPt, iEta );

      printf("\t Sum MC = %.1f +/- %.1f,\t Sum DYJets = %.1f +/- %.1f (%.3f),\t Sum TTJets = %.1f +/- %.1f (%.3f) \n", 
	     sum, sum_err, sum_dyjets, sum_err_dyjets, sum_dyjets/sum, sum_ttjets, sum_err_ttjets, sum_ttjets/sum );


    }
  }

}
