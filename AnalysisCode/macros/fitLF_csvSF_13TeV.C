#include "TH1.h"
#include "TF1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TVirtualFitter.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"


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

#include "TMatrixDSym.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"

#include "TCanvas.h"

#include "Math/MinimizerOptions.h"

//______________________________________________________________________________
void fitLF_csvSF_13TeV( TString inputFileName  = "infile.root", int iterNum=0, TString dirPostFix = "" ){


  TH1::SetDefaultSumw2();

  TString dirprefix = "Images/Images_2014_08_26_fitLF_csvSF_13TeV" + dirPostFix + "/";

  struct stat st;
  if( stat(dirprefix.Data(),&st) != 0 )  mkdir(dirprefix.Data(),0777);


  TFile *file = TFile::Open(inputFileName);
  TFile *file_JESUp = TFile::Open(inputFileName);
  TFile *file_JESDown = TFile::Open(inputFileName);

  std::cout << " ###===> iteration version " << iterNum << std::endl;


  std::string histofilename = Form("csv_rwt_fit_lf_v%d.root",iterNum) ;
  TFile histofile(histofilename.c_str(),"recreate");
  histofile.cd();


  double useUp = 1.2;
  double useDown = 0.8;

  // int ncsvbins = 17;//21;//17;
  // double csvbins[] = { -10.0, 0.0, 0.040, 0.080, 0.120, 0.160, 0.200, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.949, 1.010 };
  // double csvbins_new[] = { -0.04, 0.0, 0.040, 0.080, 0.120, 0.160, 0.200, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.949, 1.010 };
  // double csvbins[] = {-10.0, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.915, 0.932, 0.949, 0.966, 0.983, 1.01};
  // double csvbins_new[] = {-0.04, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.915, 0.932, 0.949, 0.966, 0.983, 1.01};
  // same as last round
  int ncsvbins = 17;//21;//17;
  double csvbins[] = {-10.0, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.949, 1.01};
  double csvbins_new[] = {-0.04, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.949, 1.01};
  // // Rebin for stats
  // int ncsvbins = 15;//21;//17;
  // double csvbins[] = {-10.0, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.825, 0.949, 1.01};
  // double csvbins_new[] = {-0.04, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.825, 0.949, 1.01};

  std::vector<TString> hist_name;
  std::vector<TString> data_hist_name;
  std::vector<TString> mc_b_hist_name;
  std::vector<TString> mc_nonb_hist_name;

  int maxPt  = 4;
  int maxEta = 3;

  for( int iPt=0; iPt<maxPt; iPt++ ){
    for( int iEta=0; iEta<maxEta; iEta++ ){
      hist_name.push_back( Form("csv_ratio_Pt%d_Eta%d", iPt, iEta) );

      data_hist_name.push_back( Form("csv_Data_Pt%d_Eta%d", iPt, iEta) );
      mc_b_hist_name.push_back( Form("csv_MC_bjets_Pt%d_Eta%d", iPt, iEta) );
      mc_nonb_hist_name.push_back( Form("csv_MC_nonbjets_Pt%d_Eta%d", iPt, iEta) );
    }
  }

  int NumHists_normal = int( hist_name.size() );
  int numHists = NumHists_normal+2;

  TH1D* h_csv_ratio[numHists];
  TH1D* h_csv_ratio_HF[numHists];
  TH1D* h_csv_ratio_HFUp[numHists];
  TH1D* h_csv_ratio_HFDown[numHists];
  TH1D* h_csv_ratio_JESUp[numHists];
  TH1D* h_csv_ratio_JESDown[numHists];
  TH1D* h_csv_ratio_Stats1Up[numHists];
  TH1D* h_csv_ratio_Stats1Down[numHists];
  TH1D* h_csv_ratio_Stats2Up[numHists];
  TH1D* h_csv_ratio_Stats2Down[numHists];

  TH1D* h_csv_data[NumHists_normal];
  TH1D* h_csv_mc_b[NumHists_normal];
  TH1D* h_csv_mc_nonb[NumHists_normal];

  TH1D* h_csv_mc_b_JESUp[NumHists_normal];
  TH1D* h_csv_mc_nonb_JESUp[NumHists_normal];
  TH1D* h_csv_mc_b_JESDown[NumHists_normal];
  TH1D* h_csv_mc_nonb_JESDown[NumHists_normal];


  TH1D* h_csv_data_all;
  TH1D* h_csv_mc_b_all;
  TH1D* h_csv_mc_nonb_all;

  TH1D* h_csv_mc_b_all_JESUp;
  TH1D* h_csv_mc_nonb_all_JESUp;

  TH1D* h_csv_mc_b_all_JESDown;
  TH1D* h_csv_mc_nonb_all_JESDown;

  for( int iHist=0; iHist<NumHists_normal; iHist++ ){
    TH1D* h_csv_data_temp0 = (TH1D*)file->Get( data_hist_name[iHist] )->Clone( Form("h_%s_temp0",data_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_b_temp0 = (TH1D*)file->Get( mc_b_hist_name[iHist] )->Clone( Form("h_%s_temp0",mc_b_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_nonb_temp0 = (TH1D*)file->Get( mc_nonb_hist_name[iHist] )->Clone( Form("h_%s_temp0",mc_nonb_hist_name[iHist].Data()) );

    TH1D* h_csv_data_temp0_rebin = (TH1D*)h_csv_data_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",data_hist_name[iHist].Data()),csvbins);
    TH1D* h_csv_mc_b_temp0_rebin = (TH1D*)h_csv_mc_b_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",mc_b_hist_name[iHist].Data()),csvbins);
    TH1D* h_csv_mc_nonb_temp0_rebin = (TH1D*)h_csv_mc_nonb_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",mc_nonb_hist_name[iHist].Data()),csvbins);

    // JES up/down
    TH1D* h_csv_mc_b_JESUp_temp0 = (TH1D*)file_JESUp->Get( mc_b_hist_name[iHist] )->Clone( Form("h_%s_JESUp_temp0",mc_b_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_nonb_JESUp_temp0 = (TH1D*)file_JESUp->Get( mc_nonb_hist_name[iHist] )->Clone( Form("h_%s_JESUp_temp0",mc_nonb_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_b_JESDown_temp0 = (TH1D*)file_JESDown->Get( mc_b_hist_name[iHist] )->Clone( Form("h_%s_JESDown_temp0",mc_b_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_nonb_JESDown_temp0 = (TH1D*)file_JESDown->Get( mc_nonb_hist_name[iHist] )->Clone( Form("h_%s_JESDown_temp0",mc_nonb_hist_name[iHist].Data()) );

    TH1D* h_csv_mc_b_JESUp_temp0_rebin = (TH1D*)h_csv_mc_b_JESUp_temp0->Rebin(ncsvbins,Form("h_%s_JESUp_temp0_rebin",mc_b_hist_name[iHist].Data()),csvbins);
    TH1D* h_csv_mc_nonb_JESUp_temp0_rebin = (TH1D*)h_csv_mc_nonb_JESUp_temp0->Rebin(ncsvbins,Form("h_%s_JESUp_temp0_rebin",mc_nonb_hist_name[iHist].Data()),csvbins);
    TH1D* h_csv_mc_b_JESDown_temp0_rebin = (TH1D*)h_csv_mc_b_JESDown_temp0->Rebin(ncsvbins,Form("h_%s_JESDown_temp0_rebin",mc_b_hist_name[iHist].Data()),csvbins);
    TH1D* h_csv_mc_nonb_JESDown_temp0_rebin = (TH1D*)h_csv_mc_nonb_JESDown_temp0->Rebin(ncsvbins,Form("h_%s_JESDown_temp0_rebin",mc_nonb_hist_name[iHist].Data()),csvbins);


    h_csv_data[iHist] = new TH1D( Form("h_%s",data_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_b[iHist] = new TH1D( Form("h_%s",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_nonb[iHist] = new TH1D( Form("h_%s",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );

    h_csv_mc_b_JESUp[iHist] = new TH1D( Form("h_%s_JESUp",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_nonb_JESUp[iHist] = new TH1D( Form("h_%s_JESUp",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_b_JESDown[iHist] = new TH1D( Form("h_%s_JESDown",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_nonb_JESDown[iHist] = new TH1D( Form("h_%s_JESDown",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );

    for( int iBin=0; iBin<ncsvbins; iBin++ ){
      h_csv_data[iHist]->SetBinContent(iBin+1, h_csv_data_temp0_rebin->GetBinContent(iBin+1));
      h_csv_data[iHist]->SetBinError(iBin+1, h_csv_data_temp0_rebin->GetBinError(iBin+1));

      h_csv_mc_b[iHist]->SetBinContent(iBin+1, h_csv_mc_b_temp0_rebin->GetBinContent(iBin+1));
      h_csv_mc_b[iHist]->SetBinError(iBin+1, h_csv_mc_b_temp0_rebin->GetBinError(iBin+1));

      h_csv_mc_nonb[iHist]->SetBinContent(iBin+1, h_csv_mc_nonb_temp0_rebin->GetBinContent(iBin+1));
      h_csv_mc_nonb[iHist]->SetBinError(iBin+1, h_csv_mc_nonb_temp0_rebin->GetBinError(iBin+1));


      h_csv_mc_b_JESUp[iHist]->SetBinContent(iBin+1, h_csv_mc_b_JESUp_temp0_rebin->GetBinContent(iBin+1));
      h_csv_mc_b_JESUp[iHist]->SetBinError(iBin+1, h_csv_mc_b_JESUp_temp0_rebin->GetBinError(iBin+1));
      h_csv_mc_nonb_JESUp[iHist]->SetBinContent(iBin+1, h_csv_mc_nonb_JESUp_temp0_rebin->GetBinContent(iBin+1));
      h_csv_mc_nonb_JESUp[iHist]->SetBinError(iBin+1, h_csv_mc_nonb_JESUp_temp0_rebin->GetBinError(iBin+1));

      h_csv_mc_b_JESDown[iHist]->SetBinContent(iBin+1, h_csv_mc_b_JESDown_temp0_rebin->GetBinContent(iBin+1));
      h_csv_mc_b_JESDown[iHist]->SetBinError(iBin+1, h_csv_mc_b_JESDown_temp0_rebin->GetBinError(iBin+1));
      h_csv_mc_nonb_JESDown[iHist]->SetBinContent(iBin+1, h_csv_mc_nonb_JESDown_temp0_rebin->GetBinContent(iBin+1));
      h_csv_mc_nonb_JESDown[iHist]->SetBinError(iBin+1, h_csv_mc_nonb_JESDown_temp0_rebin->GetBinError(iBin+1));
    }

    if( iHist==0 ){
      h_csv_data_all = (TH1D*)h_csv_data[iHist]->Clone("h_csv_data_all");
      h_csv_mc_b_all = (TH1D*)h_csv_mc_b[iHist]->Clone("h_csv_mc_b_all");
      h_csv_mc_nonb_all = (TH1D*)h_csv_mc_nonb[iHist]->Clone("h_csv_mc_nonb_all");

      h_csv_mc_b_all_JESUp = (TH1D*)h_csv_mc_b_JESUp[iHist]->Clone("h_csv_mc_b_all_JESUp");
      h_csv_mc_nonb_all_JESUp = (TH1D*)h_csv_mc_nonb_JESUp[iHist]->Clone("h_csv_mc_nonb_all_JESUp");
      h_csv_mc_b_all_JESDown = (TH1D*)h_csv_mc_b_JESDown[iHist]->Clone("h_csv_mc_b_all_JESDown");
      h_csv_mc_nonb_all_JESDown = (TH1D*)h_csv_mc_nonb_JESDown[iHist]->Clone("h_csv_mc_nonb_all_JESDown");
    }
    else{
      h_csv_data_all->Add(h_csv_data[iHist]);
      h_csv_mc_b_all->Add(h_csv_mc_b[iHist]);
      h_csv_mc_nonb_all->Add(h_csv_mc_nonb[iHist]);

      h_csv_mc_b_all_JESUp->Add(h_csv_mc_b_JESUp[iHist]);
      h_csv_mc_nonb_all_JESUp->Add(h_csv_mc_nonb_JESUp[iHist]);
      h_csv_mc_b_all_JESDown->Add(h_csv_mc_b_JESDown[iHist]);
      h_csv_mc_nonb_all_JESDown->Add(h_csv_mc_nonb_JESDown[iHist]);
    }


    h_csv_ratio[iHist]        = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_%d",iHist));
    h_csv_ratio_HFUp[iHist]   = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_HFUp_%d",iHist));
    h_csv_ratio_HFDown[iHist] = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_HFDown_%d",iHist));

    h_csv_ratio_JESUp[iHist]   = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_JESUp_%d",iHist));
    h_csv_ratio_JESDown[iHist] = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_JESDown_%d",iHist));

    TH1D* h_csv_mc_b_temp0_HFUp = (TH1D*)h_csv_mc_b[iHist]->Clone(Form("h_csv_mc_b_temp0_HFUp_%d",iHist));
    TH1D* h_csv_mc_b_temp0_HFDown = (TH1D*)h_csv_mc_b[iHist]->Clone(Form("h_csv_mc_b_temp0_HFDown_%d",iHist));

    TH1D* h_csv_mc_nonb_temp0_HFUp = (TH1D*)h_csv_mc_nonb[iHist]->Clone(Form("h_csv_mc_nonb_temp0_HFUp_%d",iHist));
    TH1D* h_csv_mc_nonb_temp0_HFDown = (TH1D*)h_csv_mc_nonb[iHist]->Clone(Form("h_csv_mc_nonb_temp0_HFDown_%d",iHist));

    h_csv_mc_b_temp0_HFUp->Scale( h_csv_data[iHist]->Integral() / ( useUp*h_csv_mc_b[iHist]->Integral() + h_csv_mc_nonb[iHist]->Integral() ) );
    h_csv_mc_b_temp0_HFDown->Scale( h_csv_data[iHist]->Integral() / ( useDown*h_csv_mc_b[iHist]->Integral() + h_csv_mc_nonb[iHist]->Integral() ) );

    h_csv_mc_nonb_temp0_HFUp->Scale( h_csv_data[iHist]->Integral() / ( useUp*h_csv_mc_b[iHist]->Integral() + h_csv_mc_nonb[iHist]->Integral() ) );
    h_csv_mc_nonb_temp0_HFDown->Scale( h_csv_data[iHist]->Integral() / ( useDown*h_csv_mc_b[iHist]->Integral() + h_csv_mc_nonb[iHist]->Integral() ) );

    h_csv_ratio[iHist]->Add(h_csv_mc_b[iHist],-1);
    h_csv_ratio_HFUp[iHist]->Add(h_csv_mc_b_temp0_HFUp,-useUp);
    h_csv_ratio_HFDown[iHist]->Add(h_csv_mc_b_temp0_HFDown,-useDown);
    h_csv_ratio_JESUp[iHist]->Add(h_csv_mc_b_JESUp[iHist],-1);
    h_csv_ratio_JESDown[iHist]->Add(h_csv_mc_b_JESDown[iHist],-1);

    h_csv_ratio[iHist]->Divide(h_csv_mc_nonb[iHist]);
    h_csv_ratio_HFUp[iHist]->Divide(h_csv_mc_nonb_temp0_HFUp);
    h_csv_ratio_HFDown[iHist]->Divide(h_csv_mc_nonb_temp0_HFDown);
    h_csv_ratio_JESUp[iHist]->Divide(h_csv_mc_nonb_JESUp[iHist]);
    h_csv_ratio_JESDown[iHist]->Divide(h_csv_mc_nonb_JESDown[iHist]);

  }


  TH1D* h_csv_ratio_all = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_temp");
  TH1D* h_csv_ratio_all_HFUp   = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_HFUp_temp");
  TH1D* h_csv_ratio_all_HFDown = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_HFDown_temp");
  TH1D* h_csv_ratio_all_JESUp   = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_JESUp_temp");
  TH1D* h_csv_ratio_all_JESDown = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_JESDown_temp");
  TH1D* h_csv_ratio_cumulative = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative");

  int nBins = h_csv_ratio_cumulative->GetNbinsX();

  TH1D* h_csv_mc_b_all_HFUp = (TH1D*)h_csv_mc_b_all->Clone("h_csv_mc_b_all_HFUp");
  TH1D* h_csv_mc_b_all_HFDown = (TH1D*)h_csv_mc_b_all->Clone("h_csv_mc_b_all_HFDown");

  TH1D* h_csv_mc_nonb_all_HFUp = (TH1D*)h_csv_mc_nonb_all->Clone("h_csv_mc_nonb_all_HFUp");
  TH1D* h_csv_mc_nonb_all_HFDown = (TH1D*)h_csv_mc_nonb_all->Clone("h_csv_mc_nonb_all_HFDown");


  h_csv_mc_b_all_HFUp->Scale( h_csv_ratio_all->Integral() / (useUp*h_csv_mc_b_all->Integral() +  h_csv_mc_nonb_all->Integral()) );
  h_csv_mc_b_all_HFDown->Scale( h_csv_ratio_all->Integral() / (useDown*h_csv_mc_b_all->Integral() +  h_csv_mc_nonb_all->Integral()) );

  h_csv_mc_nonb_all_HFUp->Scale( h_csv_ratio_all->Integral() / (useUp*h_csv_mc_b_all->Integral() +  h_csv_mc_nonb_all->Integral()) );
  h_csv_mc_nonb_all_HFDown->Scale( h_csv_ratio_all->Integral() / (useDown*h_csv_mc_b_all->Integral() +  h_csv_mc_nonb_all->Integral()) );

  h_csv_ratio_all->Add(h_csv_mc_b_all,-1);
  h_csv_ratio_all_HFUp->Add(h_csv_mc_b_all_HFUp,-useUp);
  h_csv_ratio_all_HFDown->Add(h_csv_mc_b_all_HFDown,-useDown);
  h_csv_ratio_all_JESUp->Add(h_csv_mc_b_all_JESUp,-1);
  h_csv_ratio_all_JESDown->Add(h_csv_mc_b_all_JESDown,-1);

  for( int iBin=0; iBin<nBins; iBin++ ) h_csv_ratio_cumulative->SetBinContent( iBin+1, h_csv_ratio_all->Integral(iBin+1,nBins) );

  h_csv_ratio_all->Divide(h_csv_mc_nonb_all);
  h_csv_ratio_all_HFUp->Divide(h_csv_mc_nonb_all_HFUp);
  h_csv_ratio_all_HFDown->Divide(h_csv_mc_nonb_all_HFDown);
  h_csv_ratio_all_JESUp->Divide(h_csv_mc_nonb_all_JESUp);
  h_csv_ratio_all_JESDown->Divide(h_csv_mc_nonb_all_JESDown);



  TH1D* h_mc_nonb_cumulative = (TH1D*)h_csv_mc_nonb_all->Clone("h_mc_nonb_cumulative");
  TH1D* h_mc_b_cumulative = (TH1D*)h_csv_mc_b_all->Clone("h_mc_b_cumulative");
  for( int iBin=0; iBin<nBins; iBin++ ){
    h_mc_nonb_cumulative->SetBinContent( iBin+1, h_csv_mc_nonb_all->Integral(iBin+1,nBins) );
    h_mc_b_cumulative->SetBinContent( iBin+1, h_csv_mc_b_all->Integral(iBin+1,nBins) );
  }

  for( int iBin=0; iBin<nBins; iBin++ ) printf("\t iBin = %d, numerator = %4.2f, denominator = %4.2f, ratio = %4.2f \n", iBin+1, 
					       h_csv_ratio_cumulative->GetBinContent(iBin+1), h_mc_nonb_cumulative->GetBinContent(iBin+1),
					       h_csv_ratio_cumulative->GetBinContent(iBin+1)/h_mc_nonb_cumulative->GetBinContent(iBin+1) );


  h_csv_ratio_cumulative->Divide(h_mc_nonb_cumulative);


  for( int iBin=0; iBin<nBins; iBin++ ) printf("\t iBin = %d, cumulative ratio = %4.2f \n ", iBin+1, h_csv_ratio_cumulative->GetBinContent(iBin+1));
  for( int iBin=0; iBin<nBins; iBin++ ) printf("\t iBin = %d, ratio = %4.2f \n ", iBin+1, h_csv_ratio_all->GetBinContent(iBin+1));

  hist_name.push_back("csv_ratio_all");

  hist_name.push_back("csv_ratio_cumulative_all");


  h_csv_ratio[numHists-2] = (TH1D*)h_csv_ratio_all->Clone( Form("h_%s",hist_name[numHists-2].Data()) );

  h_csv_ratio_HFUp[numHists-2] = (TH1D*)h_csv_ratio_all_HFUp->Clone( Form("h_%s_HFUp",hist_name[numHists-2].Data()) );
  h_csv_ratio_HFDown[numHists-2] = (TH1D*)h_csv_ratio_all_HFDown->Clone( Form("h_%s_HFDown",hist_name[numHists-2].Data()) );

  h_csv_ratio_JESUp[numHists-2] = (TH1D*)h_csv_ratio_all_JESUp->Clone( Form("h_%s_JESUp",hist_name[numHists-2].Data()) );
  h_csv_ratio_JESDown[numHists-2] = (TH1D*)h_csv_ratio_all_JESDown->Clone( Form("h_%s_JESDown",hist_name[numHists-2].Data()) );


  h_csv_ratio[numHists-1] = (TH1D*)h_csv_ratio_cumulative->Clone( Form("h_%s",hist_name[numHists-1].Data()) );
  h_csv_ratio[numHists-1]->SetMarkerStyle(20);




  for( int iHist=0; iHist<numHists-1; iHist++ ){
    h_csv_ratio_Stats1Up[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats1Up",hist_name[numHists-2].Data()) );
    h_csv_ratio_Stats1Down[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats1Down",hist_name[numHists-2].Data()) );
    h_csv_ratio_Stats2Up[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats2Up",hist_name[numHists-2].Data()) );
    h_csv_ratio_Stats2Down[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats2Down",hist_name[numHists-2].Data()) );

    h_csv_ratio_HF[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_HF",hist_name[numHists-2].Data()) );

    for( int iBin=0; iBin<nBins; iBin++ ){
      double center = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
      double content = h_csv_ratio[iHist]->GetBinContent(iBin+1);
      double hfUp = fabs( content - h_csv_ratio_HFUp[iHist]->GetBinContent(iBin+1) );
      double hfDown = fabs( content - h_csv_ratio_HFDown[iHist]->GetBinContent(iBin+1) );

      double delta = h_csv_ratio[iHist]->GetBinError(iBin+1);
      double stat1Up   = content + delta * ( 1 - 2*center );
      double stat1Down = content + delta * ( 2*center - 1 );

      double stat2Up   = content + delta * ( 1 - 6*center*(1-center) );
      double stat2Down = content - delta * ( 1 - 6*center*(1-center) );

      double hfUnc = 0.5 * ( hfUp + hfDown );
      h_csv_ratio_HF[iHist]->SetBinError(iBin+1,hfUnc);

      h_csv_ratio_Stats1Up[iHist]->SetBinContent(iBin+1,stat1Up);
      h_csv_ratio_Stats1Down[iHist]->SetBinContent(iBin+1,stat1Down);

      h_csv_ratio_Stats2Up[iHist]->SetBinContent(iBin+1,stat2Up);
      h_csv_ratio_Stats2Down[iHist]->SetBinContent(iBin+1,stat2Down);
    }
  }

  //TF1* btagSF = new TF1("btagSF","0.607239*x*x*x*x+-1.49984*x*x*x+1.3473*x*x+-0.599888*x+1.09396",0.,1.0);

  //// pol5
  //TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x",0,0.95 );

  //// pol6
  TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x + [6]*x*x*x*x*x*x",0,0.95 );

  //// pol7
  //TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x + [6]*x*x*x*x*x*x + [7]*x*x*x*x*x*x",0,0.95 );

  //TF1* f0 = new TF1("f0","[0] + [1]*TMath::Log([2]*x + [3])",0,0.9 );
  //TF1* f0 = new TF1("f0","[0] + [1]*TMath::Log([2]*x + [3]) + [4]*x + [5]*x*x + [6]*x*x*x + [7]*x*x*x*x + [8]*x*x*x*x*x",0,0.95 );
  //f0->SetParLimits(8,0,100);


  //TF1* f0 = new TF1("f0","[0] + [1]*TMath::Log([2]*x + [3])",0,0.679 );
  f0->SetLineColor(kBlack);


  TF1* f0_HFUp = (TF1*)f0->Clone("f0_HFUp");
  TF1* f0_HFDown = (TF1*)f0->Clone("f0_HFDown");

  TF1* f0_JESUp = (TF1*)f0->Clone("f0_JESUp");
  TF1* f0_JESDown = (TF1*)f0->Clone("f0_JESDown");

  TF1* f0_Stats1Up = (TF1*)f0->Clone("f0_Stats1Up");
  TF1* f0_Stats1Down = (TF1*)f0->Clone("f0_Stats1Down");

  TF1* f0_Stats2Up = (TF1*)f0->Clone("f0_Stats2Up");
  TF1* f0_Stats2Down = (TF1*)f0->Clone("f0_Stats2Down");

  f0_JESUp->SetLineColor(kRed);
  f0_JESDown->SetLineColor(kBlue);

  f0_HFUp->SetLineColor(kGreen+3);
  f0_HFDown->SetLineColor(kGreen+3);

  f0_Stats1Up->SetLineColor(kCyan+2);
  f0_Stats1Down->SetLineColor(kCyan+2);


  int nPars = f0->GetNpar();

  int n = 100000;
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(n);


  TH1D* h_csv_ratio_final[numHists];
  TH1D* h_csv_ratio_final_HF[numHists];
  TH1D* h_csv_ratio_final_HFUp[numHists];
  TH1D* h_csv_ratio_final_HFDown[numHists];
  TH1D* h_csv_ratio_final_JESUp[numHists];
  TH1D* h_csv_ratio_final_JESDown[numHists];
  TH1D* h_csv_ratio_final_Stats1Up[numHists];
  TH1D* h_csv_ratio_final_Stats1Down[numHists];
  TH1D* h_csv_ratio_final_Stats2Up[numHists];
  TH1D* h_csv_ratio_final_Stats2Down[numHists];


  double par_vals[numHists][nPars];

  double fit_chi2[numHists];
  int    fit_ndof[numHists];
  double fit_prob[numHists];

  double fit_chi2_HFUp[numHists];
  int    fit_ndof_HFUp[numHists];
  double fit_prob_HFUp[numHists];

  double fit_chi2_HFDown[numHists];
  int    fit_ndof_HFDown[numHists];
  double fit_prob_HFDown[numHists];

  double fit_chi2_JESUp[numHists];
  int    fit_ndof_JESUp[numHists];
  double fit_prob_JESUp[numHists];

  double fit_chi2_JESDown[numHists];
  int    fit_ndof_JESDown[numHists];
  double fit_prob_JESDown[numHists];

  double fit_chi2_Stats1Up[numHists];
  int    fit_ndof_Stats1Up[numHists];
  double fit_prob_Stats1Up[numHists];

  double fit_chi2_Stats1Down[numHists];
  int    fit_ndof_Stats1Down[numHists];
  double fit_prob_Stats1Down[numHists];

  double fit_chi2_Stats2Up[numHists];
  int    fit_ndof_Stats2Up[numHists];
  double fit_prob_Stats2Up[numHists];

  double fit_chi2_Stats2Down[numHists];
  int    fit_ndof_Stats2Down[numHists];
  double fit_prob_Stats2Down[numHists];

  int NumFinalBins = 1000;

  TCanvas *c1 = new TCanvas("c1");
  for( int iHist=0; iHist<numHists-1; iHist++ ){

    h_csv_ratio_final[iHist] = new TH1D( Form("h_%s_final",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_JESUp[iHist] = new TH1D( Form("h_%s_final_JESUp",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_JESDown[iHist] = new TH1D( Form("h_%s_final_JESDown",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_HFUp[iHist] = new TH1D( Form("h_%s_final_HFUp",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_HFDown[iHist] = new TH1D( Form("h_%s_final_HFDown",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_Stats1Up[iHist] = new TH1D( Form("h_%s_final_Stats1Up",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_Stats1Down[iHist] = new TH1D( Form("h_%s_final_Stats1Down",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_Stats2Up[iHist] = new TH1D( Form("h_%s_final_Stats2Up",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_Stats2Down[iHist] = new TH1D( Form("h_%s_final_Stats2Down",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );

    for( int iPar=0; iPar<nPars; iPar++ ) f0->SetParameter(iPar,1.);

    TFitResultPtr r = h_csv_ratio[iHist]->Fit(f0,"+mrN0S");

    fit_chi2[iHist] = f0->GetChisquare();
    fit_ndof[iHist] = f0->GetNDF();
    fit_prob[iHist] = f0->GetProb();


    int Nbins = 10000;
    //Create a histogram to hold the confidence intervals
    TH1D *hint = new TH1D("hint", "Fitted gaussian with .95 conf.band", Nbins, 0, 1.);
    (TVirtualFitter::GetFitter())->GetConfidenceIntervals(hint,0.682689292);
    //Now the "hint" histogram has the fitted function values as the 
    //bin contents and the confidence intervals as bin errors
    hint->SetStats(kFALSE);
    hint->SetFillColor(kYellow);

    TMatrixDSym cov = r->GetCovarianceMatrix();  //  to access the covariance matrix
    Double_t chi2   = r->Chi2(); // to retrieve the fit chi2
    Double_t ndof   = r->Ndf();  // to retrieve the fit NDf
    Double_t par0   = r->Parameter(0); // retrieve the value for the parameter 0
    Double_t err0   = r->ParError(0); // retrieve the error for the parameter 0
    //r->Print("V");     // print full information of fit including covariance matrix

    if( iHist!=numHists-1 ){
      TFitResultPtr r_HFUp = h_csv_ratio_HFUp[iHist]->Fit(f0_HFUp,"+mrN0S");
      fit_chi2_HFUp[iHist] = f0_HFUp->GetChisquare();
      fit_ndof_HFUp[iHist] = f0_HFUp->GetNDF();
      fit_prob_HFUp[iHist] = f0_HFUp->GetProb();

      TFitResultPtr r_HFDown = h_csv_ratio_HFDown[iHist]->Fit(f0_HFDown,"+mrN0S");
      fit_chi2_HFDown[iHist] = f0_HFDown->GetChisquare();
      fit_ndof_HFDown[iHist] = f0_HFDown->GetNDF();
      fit_prob_HFDown[iHist] = f0_HFDown->GetProb();

      TFitResultPtr r_JESUp = h_csv_ratio_JESUp[iHist]->Fit(f0_JESUp,"+mrN0S");
      fit_chi2_JESUp[iHist] = f0_JESUp->GetChisquare();
      fit_ndof_JESUp[iHist] = f0_JESUp->GetNDF();
      fit_prob_JESUp[iHist] = f0_JESUp->GetProb();

      TFitResultPtr r_JESDown = h_csv_ratio_JESDown[iHist]->Fit(f0_JESDown,"+mrN0S");
      fit_chi2_JESDown[iHist] = f0_JESDown->GetChisquare();
      fit_ndof_JESDown[iHist] = f0_JESDown->GetNDF();
      fit_prob_JESDown[iHist] = f0_JESDown->GetProb();

      TFitResultPtr r_Stats1Up = h_csv_ratio_Stats1Up[iHist]->Fit(f0_Stats1Up,"+mrN0S");
      fit_chi2_Stats1Up[iHist] = f0_Stats1Up->GetChisquare();
      fit_ndof_Stats1Up[iHist] = f0_Stats1Up->GetNDF();
      fit_prob_Stats1Up[iHist] = f0_Stats1Up->GetProb();

      TFitResultPtr r_Stats1Down = h_csv_ratio_Stats1Down[iHist]->Fit(f0_Stats1Down,"+mrN0S");
      fit_chi2_Stats1Down[iHist] = f0_Stats1Down->GetChisquare();
      fit_ndof_Stats1Down[iHist] = f0_Stats1Down->GetNDF();
      fit_prob_Stats1Down[iHist] = f0_Stats1Down->GetProb();

      TFitResultPtr r_Stats2Up = h_csv_ratio_Stats2Up[iHist]->Fit(f0_Stats2Up,"+mrN0S");
      fit_chi2_Stats2Up[iHist] = f0_Stats2Up->GetChisquare();
      fit_ndof_Stats2Up[iHist] = f0_Stats2Up->GetNDF();
      fit_prob_Stats2Up[iHist] = f0_Stats2Up->GetProb();

      TFitResultPtr r_Stats2Down = h_csv_ratio_Stats2Down[iHist]->Fit(f0_Stats2Down,"+mrN0S");
      fit_chi2_Stats2Down[iHist] = f0_Stats2Down->GetChisquare();
      fit_ndof_Stats2Down[iHist] = f0_Stats2Down->GetNDF();
      fit_prob_Stats2Down[iHist] = f0_Stats2Down->GetProb();
    }


    h_csv_ratio_final[iHist]->SetLineWidth(2);
    h_csv_ratio_final_JESUp[iHist]->SetLineWidth(2);
    h_csv_ratio_final_JESDown[iHist]->SetLineWidth(2);
    h_csv_ratio_final_HFUp[iHist]->SetLineWidth(2);
    h_csv_ratio_final_HFDown[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats1Up[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats1Down[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats2Up[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats2Down[iHist]->SetLineWidth(2);


    h_csv_ratio_final[iHist]->SetLineColor(kBlack);
    h_csv_ratio_final_JESUp[iHist]->SetLineColor(kRed);
    h_csv_ratio_final_JESDown[iHist]->SetLineColor(kBlue);
    h_csv_ratio_final_HFUp[iHist]->SetLineColor(kGreen+3);
    h_csv_ratio_final_HFDown[iHist]->SetLineColor(kGreen+3);
    h_csv_ratio_final_Stats1Up[iHist]->SetLineColor(kMagenta-2);
    h_csv_ratio_final_Stats1Down[iHist]->SetLineColor(kMagenta-2);
    h_csv_ratio_final_Stats2Up[iHist]->SetLineColor(kRed-2);
    h_csv_ratio_final_Stats2Down[iHist]->SetLineColor(kRed-2);


    double firstPoint = 0;
    double lastPoint  = 0.95;
    for( int iBin=0; iBin<n; iBin++ ){
      double center = h_csv_ratio_final[iHist]->GetBinCenter(iBin+1);
      if( center<0 ){
	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1));
	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,h_csv_ratio_JESUp[iHist]->GetBinContent(1));
	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,h_csv_ratio_JESDown[iHist]->GetBinContent(1));
	h_csv_ratio_final_HFUp[iHist]->SetBinContent(iBin+1,h_csv_ratio_HFUp[iHist]->GetBinContent(1));
	h_csv_ratio_final_HFDown[iHist]->SetBinContent(iBin+1,h_csv_ratio_HFDown[iHist]->GetBinContent(1));
	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) + h_csv_ratio[iHist]->GetBinError(1));
	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) - h_csv_ratio[iHist]->GetBinError(1));
	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) + h_csv_ratio[iHist]->GetBinError(1));
	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) - h_csv_ratio[iHist]->GetBinError(1));
      }
      else if( center>lastPoint ){
      	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,f0->Eval( lastPoint ));
      	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,f0_JESUp->Eval( lastPoint ));
      	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,f0_JESDown->Eval( lastPoint ));
      	h_csv_ratio_final_HFUp[iHist]->SetBinContent(iBin+1,f0_HFUp->Eval( lastPoint ));
      	h_csv_ratio_final_HFDown[iHist]->SetBinContent(iBin+1,f0_HFDown->Eval( lastPoint ));
      	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,f0_Stats1Up->Eval( lastPoint ));
      	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,f0_Stats1Down->Eval( lastPoint ));
      	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,f0_Stats2Up->Eval( lastPoint ));
      	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,f0_Stats2Down->Eval( lastPoint ));
      }
      else {
	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,f0->Eval( center ));
	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,f0_JESUp->Eval( center ));
	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,f0_JESDown->Eval( center ));
	h_csv_ratio_final_HFUp[iHist]->SetBinContent(iBin+1,f0_HFUp->Eval( center ));
	h_csv_ratio_final_HFDown[iHist]->SetBinContent(iBin+1,f0_HFDown->Eval( center ));
	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,f0_Stats1Up->Eval( center ));
	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,f0_Stats1Down->Eval( center ));
	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,f0_Stats2Up->Eval( center ));
	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,f0_Stats2Down->Eval( center ));
      }
    }



    for( int iPar=0; iPar<nPars; iPar++ ) par_vals[iHist][iPar] = r->Parameter(iPar);

    h_csv_ratio[iHist]->SetMarkerStyle(20);

    h_csv_ratio_final[iHist]->SetTitle( Form("LF %s", hist_name[iHist].Data() ) );
    //h_csv_ratio_final[iHist]->SetTitle("");
    h_csv_ratio_final[iHist]->GetYaxis()->SetTitle("Data/MC SF");
    h_csv_ratio_final[iHist]->GetXaxis()->SetTitle("CSV");

    h_csv_ratio_final[iHist]->SetStats(0);


    double maxY = 0;
    for( int iBin=1; iBin<h_csv_ratio[iHist]->GetNbinsX()-2; iBin++ ){
      double content = h_csv_ratio[iHist]->GetBinContent(iBin+1);
      double err     = h_csv_ratio[iHist]->GetBinError(iBin+1);

      if( maxY<(content+err) ) maxY = content + err;
    }
    maxY *= 1.3;
    //double maxY = 1.3 * h_csv_ratio[iHist]->GetMaximum();
    //double maxY = 1.3 * h_csv_ratio[iHist]->GetBinContent( h_csv_ratio[iHist]->GetNbinsX()-1 );
    maxY = std::min( maxY, 5. );

    h_csv_ratio[iHist]->SetMaximum(maxY);
    h_csv_ratio[iHist]->SetMinimum(0.4);

    h_csv_ratio[iHist]->GetXaxis()->SetRangeUser(-0.041,0.9489);


    h_csv_ratio_final[iHist]->SetMaximum(maxY);
    h_csv_ratio_final[iHist]->SetMinimum(0.4);
    h_csv_ratio_final[iHist]->GetXaxis()->SetRangeUser(-0.041,0.9489);

    TLegend *legend = new TLegend(0.2,0.75,0.77,0.89);

    h_csv_ratio_HFUp[iHist]->SetLineColor(kGreen+1);
    h_csv_ratio_HFDown[iHist]->SetLineColor(kGreen+1);

    h_csv_ratio_JESUp[iHist]->SetLineColor(kRed);
    h_csv_ratio_JESDown[iHist]->SetLineColor(kBlue);

    h_csv_ratio_Stats1Up[iHist]->SetLineColor(kCyan);
    h_csv_ratio_Stats1Down[iHist]->SetLineColor(kCyan);

    h_csv_ratio_Stats2Up[iHist]->SetLineColor(kCyan);
    h_csv_ratio_Stats2Down[iHist]->SetLineColor(kCyan);





    legend->SetFillColor(kWhite);
    legend->SetLineColor(kWhite);
    legend->SetShadowColor(kWhite);
    legend->SetTextFont(42);
    legend->SetTextSize(0.035);

    legend->SetNColumns(3);

    legend->AddEntry(h_csv_ratio[iHist],"LF SF","p");
    legend->AddEntry(h_csv_ratio_final_JESUp[iHist],"JES Up","l");
    legend->AddEntry(h_csv_ratio_final_JESDown[iHist],"JES Down","l");
    legend->AddEntry(h_csv_ratio_final[iHist],"LF Fit","l");
    legend->AddEntry(h_csv_ratio_final_HFUp[iHist],"HF Err","l");
    legend->AddEntry(h_csv_ratio_final_Stats1Up[iHist],"Stats1 Err","l");
    legend->AddEntry(h_csv_ratio_final_Stats2Up[iHist],"Stats2 Err","l");




    TLegend *legend4 = new TLegend(0.2,0.75,0.77,0.89);
    legend4->SetFillColor(kWhite);
    legend4->SetLineColor(kWhite);
    legend4->SetShadowColor(kWhite);
    legend4->SetTextFont(42);
    legend4->SetTextSize(0.05);

    legend4->SetNColumns(2);

    legend4->AddEntry(h_csv_ratio[iHist],"LF SF","p");
    legend4->AddEntry(h_csv_ratio_final[iHist],"LF Fit","l");


    h_csv_ratio_final[iHist]->Draw("hist");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend4->Draw();

    TString img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_only.png";
    c1->Print(img);
    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_only.pdf";
    c1->Print(img);




    h_csv_ratio_final[iHist]->Draw("hist");
    h_csv_ratio_final_JESUp[iHist]->Draw("histsame");
    h_csv_ratio_final_JESDown[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();

    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_JES.png";
    c1->Print(img);
    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_JES.pdf";
    c1->Print(img);



    h_csv_ratio_final[iHist]->Draw("hist");
    h_csv_ratio_final_HFUp[iHist]->Draw("histsame");
    h_csv_ratio_final_HFDown[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();

    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_HF.png";
    c1->Print(img);
    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_HF.pdf";
    c1->Print(img);



    h_csv_ratio_final[iHist]->Draw("hist");
    h_csv_ratio_final_Stats1Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Down[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();

    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_Stats1.png";
    c1->Print(img);
    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_Stats1.pdf";
    c1->Print(img);



    h_csv_ratio_final[iHist]->Draw("hist");
    h_csv_ratio_final_Stats2Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Down[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();

    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_Stats2.png";
    c1->Print(img);
    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_Stats2.pdf";
    c1->Print(img);


    TH1D* h_ratio_Stats1Up = (TH1D*)h_csv_ratio_Stats1Up[iHist]->Clone("h_ratio_Stats1Up");
    TH1D* h_ratio_Stats1Down = (TH1D*)h_csv_ratio_Stats1Down[iHist]->Clone("h_ratio_Stats1Down");

    h_ratio_Stats1Up->Divide(h_csv_ratio[iHist]);
    h_ratio_Stats1Down->Divide(h_csv_ratio[iHist]);

    TH1D* h_ratio_Stats2Up = (TH1D*)h_csv_ratio_Stats2Up[iHist]->Clone("h_ratio_Stats2Up");
    TH1D* h_ratio_Stats2Down = (TH1D*)h_csv_ratio_Stats2Down[iHist]->Clone("h_ratio_Stats2Down");

    h_ratio_Stats2Up->Divide(h_csv_ratio[iHist]);
    h_ratio_Stats2Down->Divide(h_csv_ratio[iHist]);

    h_ratio_Stats1Up->SetLineColor(kRed);
    h_ratio_Stats1Down->SetLineColor(kBlue);

    h_ratio_Stats2Up->SetLineColor(kMagenta+2);
    h_ratio_Stats2Down->SetLineColor(kGreen+2);


    h_ratio_Stats1Up->SetLineWidth(2);
    h_ratio_Stats1Down->SetLineWidth(2);

    h_ratio_Stats2Up->SetLineWidth(2);
    h_ratio_Stats2Down->SetLineWidth(2);



    TLegend *legend2 = new TLegend(0.2,0.8,0.84,0.89);

    legend2->SetFillColor(kWhite);
    legend2->SetLineColor(kWhite);
    legend2->SetShadowColor(kWhite);
    legend2->SetTextFont(42);
    legend2->SetTextSize(0.035);

    legend2->SetNColumns(2);

    legend2->AddEntry(h_ratio_Stats1Up,"Stats 1 Up","l");
    legend2->AddEntry(h_ratio_Stats1Down,"Stats 1 Down","l");

    h_ratio_Stats1Up->SetTitle(";CSV;Uncertainty/Nominal");
    h_ratio_Stats1Up->SetStats(0);
    h_ratio_Stats1Up->GetYaxis()->SetRangeUser(0.7,1.3);
    h_ratio_Stats1Up->GetXaxis()->SetRangeUser(-0.041,0.9489);

    h_ratio_Stats1Up->Draw("hist");
    h_ratio_Stats1Down->Draw("histsame");
    legend2->Draw();

    img = dirprefix + "lfSF_ratio_" + hist_name[iHist] + "_fit_Stats1.png";
    c1->Print(img);
    img = dirprefix + "lfSF_ratio_" + hist_name[iHist] + "_fit_Stats1.pdf";
    c1->Print(img);



    TLegend *legend3 = new TLegend(0.2,0.8,0.84,0.89);

    legend3->SetFillColor(kWhite);
    legend3->SetLineColor(kWhite);
    legend3->SetShadowColor(kWhite);
    legend3->SetTextFont(42);
    legend3->SetTextSize(0.035);

    legend3->SetNColumns(2);

    legend3->AddEntry(h_ratio_Stats2Up,"Stats 2 Up","l");
    legend3->AddEntry(h_ratio_Stats2Down,"Stats 2 Down","l");

    h_ratio_Stats2Up->SetTitle(";CSV;Uncertainty/Nominal");
    h_ratio_Stats2Up->SetStats(0);
    h_ratio_Stats2Up->GetYaxis()->SetRangeUser(0.7,1.3);
    h_ratio_Stats2Up->GetXaxis()->SetRangeUser(-0.041,0.9489);

    h_ratio_Stats2Up->Draw("hist");
    h_ratio_Stats2Down->Draw("histsame");
    legend3->Draw();

    img = dirprefix + "lfSF_ratio_" + hist_name[iHist] + "_fit_Stats2.png";
    c1->Print(img);
    img = dirprefix + "lfSF_ratio_" + hist_name[iHist] + "_fit_Stats2.pdf";
    c1->Print(img);



    h_csv_ratio_final[iHist]->Draw("hist");
    h_csv_ratio_final_JESUp[iHist]->Draw("histsame");
    h_csv_ratio_final_JESDown[iHist]->Draw("histsame");
    h_csv_ratio_final_HFUp[iHist]->Draw("histsame");
    h_csv_ratio_final_HFDown[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Down[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Down[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();

    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_All.png";
    c1->Print(img);
    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_All.pdf";
    c1->Print(img);




    h_csv_ratio_final[iHist]->Write(Form("%s_final",hist_name[iHist].Data()));
    h_csv_ratio_final_JESUp[iHist]->Write(Form("%s_final_JESUp",hist_name[iHist].Data()));
    h_csv_ratio_final_JESDown[iHist]->Write(Form("%s_final_JESDown",hist_name[iHist].Data()));
    h_csv_ratio_final_HFUp[iHist]->Write(Form("%s_final_HFUp",hist_name[iHist].Data()));
    h_csv_ratio_final_HFDown[iHist]->Write(Form("%s_final_HFDown",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats1Up[iHist]->Write(Form("%s_final_Stats1Up",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats1Down[iHist]->Write(Form("%s_final_Stats1Down",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats2Up[iHist]->Write(Form("%s_final_Stats2Up",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats2Down[iHist]->Write(Form("%s_final_Stats2Down",hist_name[iHist].Data()));



    delete hint;
    delete legend;

  }


  // double fit_par_vals[3][3][nPars];
  // for( int iHist=0; iHist<numHists; iHist++ ){
  //   for( int iPar=0; iPar<nPars; iPar++ ){
  //     int iPt = -1;
  //     if( hist_name[iHist].Contains("Pt0") )      iPt = 0;
  //     else if( hist_name[iHist].Contains("Pt1") ) iPt = 1;
  //     else if( hist_name[iHist].Contains("Pt2") ) iPt = 2;
  //     int iEta = -1;
  //     if( hist_name[iHist].Contains("Eta0") )      iEta = 0;
  //     else if( hist_name[iHist].Contains("Eta1") ) iEta = 1;
  //     else if( hist_name[iHist].Contains("Eta2") ) iEta = 2;

  //     fit_par_vals[iPt][iEta][iPar] = par_vals[iHist][iPar];
  //     //std::cout << "fit_par_vals[" << iPt << "][" << iEta << "][" << iPar << "] = " << fit_par_vals[iPt][iEta][iPar] << ";" << std::endl;
  //   }

  //   printf(" %s:\t chi2 = %.2f,\t ndof = %d,\t prob = %.2f \n", hist_name[iHist].Data(), fit_chi2[iHist], fit_ndof[iHist], fit_prob[iHist] );
  //   printf(" \t(JESUp)  :\t chi2 = %.2f,\t ndof = %d,\t prob = %.2f \n", fit_chi2_JESUp[iHist], fit_ndof_JESUp[iHist], fit_prob_JESUp[iHist] );
  //   printf(" \t(JESDown):\t chi2 = %.2f,\t ndof = %d,\t prob = %.2f \n", fit_chi2_JESDown[iHist], fit_ndof_JESDown[iHist], fit_prob_JESDown[iHist] );
  // }


  std::cout << " Done! " << std::endl;

  histofile.Write();
  histofile.Close();


}

/*
 FCN=4.37714 FROM IMProve     STATUS=RESET          48 CALLS         261 TOTAL
                     EDM=7.07195e-09    STRATEGY= 1  ERROR MATRIX UNCERTAINTY  50.0 per cent
  EXT PARAMETER                APPROXIMATE        STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           1.18246e+00   4.56664e-02   4.56664e-04   4.69042e+02
   2  p1           4.63312e+02   2.03551e+02   2.03551e+00  -4.44797e-01
   3  p2           2.10061e-06   1.49933e-06   1.49933e-08  -7.16288e+07
	 f1->Eval(463.312) = 0.731548
		 f1->GetMinimum(0,1000) = 0.731548
 FCN=0.0571179 FROM IMProve     STATUS=RESET          35 CALLS         207 TOTAL
                     EDM=1.04253e-08    STRATEGY= 1  ERROR MATRIX UNCERTAINTY  50.0 per cent
  EXT PARAMETER                APPROXIMATE        STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           1.21615e+00   4.59417e-02   4.59417e-04   1.71784e+02
   2  p1           3.82802e+02   4.77352e+01   4.77352e-01  -2.76792e-01
   3  p2           2.90873e-06   8.85574e-07   8.85574e-09  -2.20034e+07
 FCN=0.0615925 FROM IMProve     STATUS=RESET          43 CALLS         154 TOTAL
                     EDM=5.13031e-11    STRATEGY= 1  ERROR MATRIX UNCERTAINTY  50.0 per cent
  EXT PARAMETER                APPROXIMATE        STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           1.14857e+00   4.50183e-02   4.50183e-04   6.00544e+02
   2  p1           6.47355e+02   2.73547e+02   2.73547e+00  -4.90261e-01
   3  p2           1.28422e-06   8.54454e-07   8.54454e-09  -1.81000e+08



 TF1* f0 = new TF1("f0","[0] + [1]*TMath::Log([2]*x + [3]) + [4]*x + [5]*x*x + [6]*x*x*x + [7]*x*x*x*x + [8]*x*x*x*x*x",0,0.95 );
 csv_ratio_Pt0_Eta0:     chi2 = 8.98,    ndof = 6,       prob = 0.17 
 csv_ratio_Pt0_Eta1:     chi2 = 3.89,    ndof = 6,       prob = 0.69 
 csv_ratio_Pt0_Eta2:     chi2 = 11.51,   ndof = 6,       prob = 0.07 
 csv_ratio_Pt1_Eta0:     chi2 = 5.51,    ndof = 6,       prob = 0.48 
 csv_ratio_Pt1_Eta1:     chi2 = 9.67,    ndof = 6,       prob = 0.14 
 csv_ratio_Pt1_Eta2:     chi2 = 12.62,   ndof = 6,       prob = 0.05 
 csv_ratio_Pt2_Eta0:     chi2 = 5.62,    ndof = 6,       prob = 0.47 
 csv_ratio_Pt2_Eta1:     chi2 = 7.55,    ndof = 6,       prob = 0.27 
 csv_ratio_Pt2_Eta2:     chi2 = 4.37,    ndof = 6,       prob = 0.63 
 csv_ratio_all:  chi2 = 12.21,   ndof = 6,       prob = 0.06 
 csv_ratio_cumulative_all:       chi2 = 24.52,   ndof = 6,       prob = 0.00


 TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x",0,0.95 );
 csv_ratio_Pt0_Eta0:     chi2 = 8.98,    ndof = 9,       prob = 0.44,    SF (CSV=1) = 2.38 
 csv_ratio_Pt0_Eta1:     chi2 = 3.89,    ndof = 9,       prob = 0.92,    SF (CSV=1) = 3.19 
 csv_ratio_Pt0_Eta2:     chi2 = 12.73,   ndof = 9,       prob = 0.18,    SF (CSV=1) = 1.79 
 csv_ratio_Pt1_Eta0:     chi2 = 5.51,    ndof = 9,       prob = 0.79,    SF (CSV=1) = 1.98 
 csv_ratio_Pt1_Eta1:     chi2 = 9.67,    ndof = 9,       prob = 0.38,    SF (CSV=1) = 0.75 
 csv_ratio_Pt1_Eta2:     chi2 = 12.62,   ndof = 9,       prob = 0.18,    SF (CSV=1) = 1.56 
 csv_ratio_Pt2_Eta0:     chi2 = 5.62,    ndof = 9,       prob = 0.78,    SF (CSV=1) = 5.59 
 csv_ratio_Pt2_Eta1:     chi2 = 7.55,    ndof = 9,       prob = 0.58,    SF (CSV=1) = 2.55 
 csv_ratio_Pt2_Eta2:     chi2 = 4.37,    ndof = 9,       prob = 0.89,    SF (CSV=1) = 3.32 
 csv_ratio_all:  chi2 = 12.21,   ndof = 9,       prob = 0.20,    SF (CSV=1) = 2.67 
 csv_ratio_cumulative_all:       chi2 = 24.52,   ndof = 9,       prob = 0.00,    SF (CSV=1) = 2.67 

 TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x + [6]*x*x*x*x*x*x",0,0.95 );
 csv_ratio_Pt0_Eta0:     chi2 = 6.87,    ndof = 8,       prob = 0.55,    SF (CSV=1) = 3.94 
 csv_ratio_Pt0_Eta1:     chi2 = 3.82,    ndof = 8,       prob = 0.87,    SF (CSV=1) = 2.91 
 csv_ratio_Pt0_Eta2:     chi2 = 10.35,   ndof = 8,       prob = 0.24,    SF (CSV=1) = 0.56 
 csv_ratio_Pt1_Eta0:     chi2 = 3.78,    ndof = 8,       prob = 0.88,    SF (CSV=1) = 4.21 
 csv_ratio_Pt1_Eta1:     chi2 = 8.93,    ndof = 8,       prob = 0.35,    SF (CSV=1) = 2.17 
 csv_ratio_Pt1_Eta2:     chi2 = 10.30,   ndof = 8,       prob = 0.24,    SF (CSV=1) = 3.70 
 csv_ratio_Pt2_Eta0:     chi2 = 3.34,    ndof = 8,       prob = 0.91,    SF (CSV=1) = 8.36 
 csv_ratio_Pt2_Eta1:     chi2 = 7.57,    ndof = 8,       prob = 0.48,    SF (CSV=1) = 2.66 
 csv_ratio_Pt2_Eta2:     chi2 = 3.93,    ndof = 8,       prob = 0.86,    SF (CSV=1) = 4.03 
 csv_ratio_all:  chi2 = 9.51,    ndof = 8,       prob = 0.30,    SF (CSV=1) = 3.38 
 csv_ratio_cumulative_all:       chi2 = 9.84,    ndof = 8,       prob = 0.28,    SF (CSV=1) = 3.63 

 TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x + [6]*x*x*x*x*x*x + [7]*x*x*x*x*x*x",0,0.95 );
 csv_ratio_Pt0_Eta0:     chi2 = 6.87,    ndof = 7,       prob = 0.44,    SF (CSV=1) = 3.94 
 csv_ratio_Pt0_Eta1:     chi2 = 3.82,    ndof = 7,       prob = 0.80,    SF (CSV=1) = 2.91 
 csv_ratio_Pt0_Eta2:     chi2 = 10.35,   ndof = 7,       prob = 0.17,    SF (CSV=1) = 0.56 
 csv_ratio_Pt1_Eta0:     chi2 = 3.78,    ndof = 7,       prob = 0.80,    SF (CSV=1) = 4.21 
 csv_ratio_Pt1_Eta1:     chi2 = 8.93,    ndof = 7,       prob = 0.26,    SF (CSV=1) = 2.17 
 csv_ratio_Pt1_Eta2:     chi2 = 10.30,   ndof = 7,       prob = 0.17,    SF (CSV=1) = 3.70 
 csv_ratio_Pt2_Eta0:     chi2 = 3.34,    ndof = 7,       prob = 0.85,    SF (CSV=1) = 8.36 
 csv_ratio_Pt2_Eta1:     chi2 = 7.57,    ndof = 7,       prob = 0.37,    SF (CSV=1) = 2.66 
 csv_ratio_Pt2_Eta2:     chi2 = 3.93,    ndof = 7,       prob = 0.79,    SF (CSV=1) = 4.04 
 csv_ratio_all:  chi2 = 9.51,    ndof = 7,       prob = 0.22,    SF (CSV=1) = 3.38 
 csv_ratio_cumulative_all:       chi2 = 9.84,    ndof = 7,       prob = 0.20,    SF (CSV=1) = 3.63 


 */
