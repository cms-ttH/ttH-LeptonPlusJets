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

// ------------ csv applying functions -------------
void fillCSVhistos(TFile *fileHF, TFile *fileLF);

// CSV reweighting
TH1D* h_csv_wgt_hf[9][6];
TH1D* c_csv_wgt_hf[9][6];
TH1D* h_csv_wgt_lf[9][4][3];


void createFakeData_csvSF_13TeV( TString inputFileName  = "infile.root",
				 TString outputFileName = "outfile.root",
				 bool isHF = true, bool use8TeV = false, 
				 double a_HF = 1.0, double b_HF = 1.0,
				 double a_LF = 1.0, double b_LF = 1.0,
				 bool addStatFluctuations = false ) {

  TFile* f_CSVwgt_HF = new TFile ((string(getenv("CMSSW_BASE")) + "/src/ttH-LeptonPlusJets/AnalysisCode/data/csv_rwt_hf_IT.root").c_str());
  TFile* f_CSVwgt_LF = new TFile ((string(getenv("CMSSW_BASE")) + "/src/ttH-LeptonPlusJets/AnalysisCode/data/csv_rwt_lf_IT.root").c_str());

  fillCSVhistos(f_CSVwgt_HF, f_CSVwgt_LF);


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
      
      h_Data_jet_csv[iPt][iEta] = (TH1D*)histFile->Get(h_b_Name.Data())->Clone(h_Data_Name.Data());
      h_MC_b_jet_csv[iPt][iEta] = (TH1D*)histFile->Get(h_b_Name.Data());
      h_MC_nonb_jet_csv[iPt][iEta] = (TH1D*)histFile->Get(h_nonb_Name.Data());

      int nBins = h_MC_b_jet_csv[iPt][iEta]->GetNbinsX();

      for( int iBin=0; iBin<nBins; iBin++ ){
	double content_b_mc = h_MC_b_jet_csv[iPt][iEta]->GetBinContent(iBin+1);
	double content_nonb_mc = h_MC_nonb_jet_csv[iPt][iEta]->GetBinContent(iBin+1);

	double csv = h_MC_b_jet_csv[iPt][iEta]->GetBinCenter(iBin+1);

	if( use8TeV ){
	  int bin_hf = h_csv_wgt_hf[0][iPt]->FindBin( csv );
	  double scale_hf = h_csv_wgt_hf[0][iPt]->GetBinContent( bin_hf );

	  int iPt_LF = std::min( iPt, 3 );
	  int bin_lf = h_csv_wgt_lf[0][iPt_LF][iEta]->FindBin( csv );
	  double scale_lf = h_csv_wgt_lf[0][iPt_LF][iEta]->GetBinContent( bin_lf );

	  content_b_mc *= scale_hf;
	  content_nonb_mc *= scale_lf;
	}
	else{
	  content_b_mc *= ( a_HF + csv * b_HF );
	  content_nonb_mc *= ( a_LF + csv * b_LF );
	}

	int data = int( content_b_mc + content_nonb_mc + 0.0001 );

	if( addStatFluctuations ){
	  data = int( r.Poisson( double(data) ) );
	}
	if( data<0 ) data = 0;

	h_Data_jet_csv[iPt][iEta]->SetBinContent(iBin+1, data);
	h_Data_jet_csv[iPt][iEta]->SetBinError(iBin+1, sqrt(data));
      }

      outFile->cd();
      h_Data_jet_csv[iPt][iEta]->Write(h_Data_Name.Data());
    }
  }

  outFile->Close();

  std::cout << "Done." << std::endl;

}


void fillCSVhistos(TFile* fileHF, TFile* fileLF){

  for( int iSys=0; iSys<9; iSys++ ){
    for( int iPt=0; iPt<5; iPt++ ) h_csv_wgt_hf[iSys][iPt] = NULL;
    for( int iPt=0; iPt<3; iPt++ ){
      for( int iEta=0; iEta<3; iEta++ )h_csv_wgt_lf[iSys][iPt][iEta] = NULL;
    }
  }
  for( int iSys=0; iSys<5; iSys++ ){
    for( int iPt=0; iPt<5; iPt++ ) c_csv_wgt_hf[iSys][iPt] = NULL;
  }

  // CSV reweighting /// only care about the nominal ones
  for( int iSys=0; iSys<1; iSys++ ){
    TString syst_csv_suffix_hf = "final";
    TString syst_csv_suffix_c = "final";
    TString syst_csv_suffix_lf = "final";
    
    switch( iSys ){
    case 0:
      // this is the nominal case
      break;
    case 1:
      // JESUp
      syst_csv_suffix_hf = "final_JESUp"; syst_csv_suffix_lf = "final_JESUp";
      syst_csv_suffix_c  = "final_cErr1Up";
      break;
    case 2:
      // JESDown
      syst_csv_suffix_hf = "final_JESDown"; syst_csv_suffix_lf = "final_JESDown";
      syst_csv_suffix_c  = "final_cErr1Down";
      break;
    case 3:
      // purity up
      syst_csv_suffix_hf = "final_LFUp"; syst_csv_suffix_lf = "final_HFUp";
      syst_csv_suffix_c  = "final_cErr2Up";
      break;
    case 4:
      // purity down
      syst_csv_suffix_hf = "final_LFDown"; syst_csv_suffix_lf = "final_HFDown";
      syst_csv_suffix_c  = "final_cErr2Down";
      break;
    case 5:
      // stats1 up
      syst_csv_suffix_hf = "final_Stats1Up"; syst_csv_suffix_lf = "final_Stats1Up";
      break;
    case 6:
      // stats1 down
      syst_csv_suffix_hf = "final_Stats1Down"; syst_csv_suffix_lf = "final_Stats1Down";
      break;
    case 7:
      // stats2 up
      syst_csv_suffix_hf = "final_Stats2Up"; syst_csv_suffix_lf = "final_Stats2Up";
      break;
    case 8:
      // stats2 down
      syst_csv_suffix_hf = "final_Stats2Down"; syst_csv_suffix_lf = "final_Stats2Down";
      break;
    }

    for( int iPt=0; iPt<6; iPt++ ) h_csv_wgt_hf[iSys][iPt] = (TH1D*)fileHF->Get( Form("csv_ratio_Pt%i_Eta0_%s",iPt,syst_csv_suffix_hf.Data()) );

    if( iSys<5 ){
      for( int iPt=0; iPt<6; iPt++ ) c_csv_wgt_hf[iSys][iPt] = (TH1D*)fileHF->Get( Form("c_csv_ratio_Pt%i_Eta0_%s",iPt,syst_csv_suffix_c.Data()) );
    }
    
    for( int iPt=0; iPt<4; iPt++ ){
      for( int iEta=0; iEta<3; iEta++ )h_csv_wgt_lf[iSys][iPt][iEta] = (TH1D*)fileLF->Get( Form("csv_ratio_Pt%i_Eta%i_%s",iPt,iEta,syst_csv_suffix_lf.Data()) );
    }
  }

  return;
}
