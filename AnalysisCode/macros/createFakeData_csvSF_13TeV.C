//Root includes                                   
#include "TROOT.h"
#include "Riostream.h"
#include "TFile.h"
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

void createFakeData_csvSF_13TeV( TString inputFileName  = "infile.root",
				 TString outputFileName = "outfile.root",
				 int iter = 0,
				 bool isHF = true, bool use8TeV = false, 
				 double a_HF = 1.0, double b_HF = 1.0,
				 double a_LF = 1.0, double b_LF = 1.0,
				 bool addStatFluctuations = false ) {

  TFile *outFile = TFile::Open(outputFileName,"RECREATE");
  TFile *histFile = TFile::Open(inputFileName);

  TH1::SetDefaultSumw2();


  // single jet specific plots
  int nPt = 6;
  int nEta = 1;
  if ( !isHF ){
    nPt = 4; nEta = 3;
  }

  //////
  TH1D* h_Data_jet_csv[nPt][nEta];
  TH1D* h_MC_b_jet_csv[nPt][nEta];
  TH1D* h_MC_nonb_jet_csv[nPt][nEta];

  TRandom3 r;


  for ( int iPt=0; iPt<nPt; iPt++){
    for ( int iEta=0; iEta<nEta; iEta++){

      TString h_Data_Name = Form("csv_Data_Pt%i_Eta%i",iPt,iEta);
      TString h_b_Name = Form("csv_MC_bjets_Pt%i_Eta%i",iPt,iEta);
      TString h_nonb_Name = Form("csv_MC_nonbjets_Pt%i_Eta%i",iPt,iEta);
      
      h_Data_jet_csv[iPt][iEta] = (TH1D*)histFile->Get(h_Data_Name.Data());
      h_MC_b_jet_csv[iPt][iEta] = (TH1D*)histFile->Get(h_b_Name.Data());
      h_MC_nonb_jet_csv[iPt][iEta] = (TH1D*)histFile->Get(h_nonb_Name.Data());

      int nBins = h_MC_b_jet_csv[iPt][iEta]->GetNbinsX();

      for( int iBin=0; iBin<nBins; iBin++ ){
	double content_data = h_Data_jet_csv[iPt][iEta]->GetBinContent(iBin+1);
	double content_b_mc = h_MC_b_jet_csv[iPt][iEta]->GetBinContent(iBin+1);
	double content_nonb_mc = h_MC_nonb_jet_csv[iPt][iEta]->GetBinContent(iBin+1);

	double csv = h_MC_b_jet_csv[iPt][iEta]->GetBinCenter(iBin+1);

	double use_data = 0.;
	if( use8TeV || iter!=0 ) use_data = content_data;
	else{
	  content_b_mc *= ( a_HF + csv * b_HF );
	  content_nonb_mc *= ( a_LF + csv * b_LF );

	  use_data = content_b_mc + content_nonb_mc;
	}

	int data = int( use_data + 0.0001 );

	if( addStatFluctuations && iter==0 ){
	  data = int( r.Poisson( double(data) ) );
	}
	if( data<0 ) data = 0;

	h_Data_jet_csv[iPt][iEta]->SetBinContent(iBin+1, data);
	h_Data_jet_csv[iPt][iEta]->SetBinError(iBin+1, sqrt(data));
      }

      outFile->cd();
      h_Data_jet_csv[iPt][iEta]->Write(h_Data_Name.Data());
      h_MC_b_jet_csv[iPt][iEta]->Write(h_b_Name.Data());
      h_MC_nonb_jet_csv[iPt][iEta]->Write(h_nonb_Name.Data());
    }
  }

  outFile->Close();

  std::cout << "Done." << std::endl;

}
