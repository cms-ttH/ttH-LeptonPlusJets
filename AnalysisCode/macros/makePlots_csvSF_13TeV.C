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

void makePlots_csvSF_13TeV( TString inputFileName  = "infile.root", bool isHF = true, TString dirPostFix = "", bool compareIterations = false ) {

  TFile *histFile = TFile::Open(inputFileName);


  TString dirprefix = "Images/Images_2014_08_26_csvSF_13TeV" + dirPostFix + "/";

  struct stat st;
  if( stat(dirprefix.Data(),&st) != 0 )  mkdir(dirprefix.Data(),0777);

  // single jet specific plots
  int nPt = 6;
  int nEta = 1;
  TString flavor = "HF";
  if ( !isHF ){
    nPt = 4; nEta = 3;
    flavor = "LF";
  }

  //////
  TH1D* h_Data_jet_csv[nPt][nEta];
  TH1D* h_MC_b_jet_csv[nPt][nEta];
  TH1D* h_MC_nonb_jet_csv[nPt][nEta];

  /////
  int nCSVBins = 18; //Number of bins 
  double xBins_hf[19] = {-0.04, 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01};

  // int nCSVBins = 14;
  // double xBins_hf[15] = {-0.04, 0.0, 0.244, 0.418, 0.592, 0.679, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01};


  if(!isHF) nCSVBins = 21;
  double xBins_lf[22] = {-0.04, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.915, 0.932, 0.949, 0.966, 0.983, 1.01};
  // if(!isHF) nCSVBins = 15;
  // double xBins_lf[16] = {-0.04, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.825, 0.949, 1.01};


  //TCanvas* c1 = new TCanvas("c1","c1",600,600);
  TCanvas* c1 = new TCanvas("c1","c1",600,500);

  c1->SetTopMargin(0.08);
  c1->SetRightMargin(0.08);


  TString flavor_file = flavor;
  flavor_file.ToLower();

  TFile *fitFile_iter0 = TFile::Open("csv_rwt_fit_" + flavor_file + "_v0.root");
  TFile *fitFile_iter1 = TFile::Open("csv_rwt_fit_" + flavor_file + "_v1.root");
  TFile *fitFile_iter2 = TFile::Open("csv_rwt_fit_" + flavor_file + "_v2.root");



  TString plotName;

  int iHist = -1;
  for ( int iPt=0; iPt<nPt; iPt++){
    for ( int iEta=0; iEta<nEta; iEta++){
      iHist++;

      TString h_Data_Name = Form("csv_Data_Pt%i_Eta%i",iPt,iEta);
      TString h_b_Name = Form("csv_MC_bjets_Pt%i_Eta%i",iPt,iEta);
      TString h_nonb_Name = Form("csv_MC_nonbjets_Pt%i_Eta%i",iPt,iEta);

      
      h_Data_jet_csv[iPt][iEta] = (TH1D*)histFile->Get(h_Data_Name.Data());
      h_MC_b_jet_csv[iPt][iEta] = (TH1D*)histFile->Get(h_b_Name.Data());
      h_MC_nonb_jet_csv[iPt][iEta] = (TH1D*)histFile->Get(h_nonb_Name.Data());

      // rebin
      TH1D* h_csv_data = NULL;
      TH1D* h_csv_mc_b = NULL;
      TH1D* h_csv_mc_nonb = NULL;
      if( isHF ){
	h_csv_data = (TH1D*)h_Data_jet_csv[iPt][iEta]->Rebin( nCSVBins, Form("csv_Data_Pt%i_Eta%i_temp",iPt,iEta), xBins_hf );
	h_csv_mc_b = (TH1D*)h_MC_b_jet_csv[iPt][iEta]->Rebin( nCSVBins, Form("csv_Data_Pt%i_Eta%i_temp",iPt,iEta), xBins_hf );
	h_csv_mc_nonb = (TH1D*)h_MC_nonb_jet_csv[iPt][iEta]->Rebin( nCSVBins, Form("csv_Data_Pt%i_Eta%i_temp",iPt,iEta), xBins_hf );
      }
      else{
	h_csv_data = (TH1D*)h_Data_jet_csv[iPt][iEta]->Rebin( nCSVBins, Form("csv_Data_Pt%i_Eta%i_temp",iPt,iEta), xBins_lf );
	h_csv_mc_b = (TH1D*)h_MC_b_jet_csv[iPt][iEta]->Rebin( nCSVBins, Form("csv_Data_Pt%i_Eta%i_temp",iPt,iEta), xBins_lf );
	h_csv_mc_nonb = (TH1D*)h_MC_nonb_jet_csv[iPt][iEta]->Rebin( nCSVBins, Form("csv_Data_Pt%i_Eta%i_temp",iPt,iEta), xBins_lf );
      }

      h_csv_data->SetBinContent(1,h_Data_jet_csv[iPt][iEta]->GetBinContent(1));
      h_csv_data->SetBinError(1,h_Data_jet_csv[iPt][iEta]->GetBinError(1));

      h_csv_mc_b->SetBinContent(1,h_MC_b_jet_csv[iPt][iEta]->GetBinContent(1));
      h_csv_mc_b->SetBinError(1,h_MC_b_jet_csv[iPt][iEta]->GetBinError(1));

      h_csv_mc_nonb->SetBinContent(1,h_MC_nonb_jet_csv[iPt][iEta]->GetBinContent(1));
      h_csv_mc_nonb->SetBinError(1,h_MC_nonb_jet_csv[iPt][iEta]->GetBinError(1));

      h_csv_data->SetStats(0);
      //h_csv_data->GetXaxis()->SetRangeUser(0.0001, 1.001);

      h_csv_data->SetTitle(";CSV");

      h_csv_data->SetMarkerStyle(20);

      h_csv_mc_b->SetFillColor(kRed);
      h_csv_mc_nonb->SetFillColor(kGreen+1);

      h_csv_mc_b->SetLineColor(kRed);
      h_csv_mc_nonb->SetLineColor(kGreen+1);

      h_csv_data->SetLineWidth(2);
      h_csv_mc_b->SetLineWidth(2);
      h_csv_mc_nonb->SetLineWidth(2);


      TLegend *legend = new TLegend(0.14,0.93,0.9,0.98);

      legend->SetFillColor(kWhite);
      legend->SetLineColor(kWhite);
      legend->SetShadowColor(kWhite);
      legend->SetTextFont(42);
      legend->SetTextSize(0.05);

      legend->SetNColumns(3);

      legend->AddEntry(h_csv_data,"Data","pe");
      if( isHF ){
	legend->AddEntry(h_csv_mc_b,"b jets","l");
	legend->AddEntry(h_csv_mc_nonb,"non-b jets","l");
      }
      else{
	legend->AddEntry(h_csv_mc_b,"HF jets","l");
	legend->AddEntry(h_csv_mc_nonb,"LF jets","l");
      }

      TH1D* h_diff = (TH1D*) h_csv_data->Clone( Form("csv_diff_Pt%i_Eta%i",iPt,iEta) );

      THStack *hs = new THStack("hs","");
      if( isHF ){
	hs->Add(h_csv_mc_b);
	hs->Add(h_csv_mc_nonb);

	h_diff->Add(h_csv_mc_nonb,-1);
      }
      else{
	hs->Add(h_csv_mc_nonb);
	hs->Add(h_csv_mc_b);

	h_diff->Add(h_csv_mc_b,-1);
      }

      TH1D* h_ratio = (TH1D*) h_diff->Clone( Form("csv_ratio_Pt%i_Eta%i",iPt,iEta) );
      if( isHF ) h_ratio->Divide( h_csv_mc_b );
      else       h_ratio->Divide( h_csv_mc_nonb );
      

      TString title    = Form("%s SF Pt%i Eta%i",flavor.Data(),iPt,iEta);

      TLatex BinInfoLatex(0.6, 0.86, title.Data());
	
      BinInfoLatex.SetNDC();
      BinInfoLatex.SetTextFont(42);
      BinInfoLatex.SetTextSize(0.04);

      h_csv_data->Draw("pe1");
      hs->Draw("histsame");
      h_csv_data->Draw("pe1same");
      
      legend->Draw();
      BinInfoLatex.Draw();

      c1->RedrawAxis();

      plotName = dirprefix + Form("csv_%s_SF_Pt%i_Eta%i_astack",flavor.Data(),iPt,iEta) + ".png";

      c1->Print(plotName.Data());



      TLegend *legend_diff = new TLegend(0.14,0.93,0.9,0.98);

      legend_diff->SetFillColor(kWhite);
      legend_diff->SetLineColor(kWhite);
      legend_diff->SetShadowColor(kWhite);
      legend_diff->SetTextFont(42);
      legend_diff->SetTextSize(0.05);

      legend_diff->SetNColumns(2);

      if( isHF ){
	legend_diff->AddEntry(h_diff,"(Data - nonb)","pe");
	legend_diff->AddEntry(h_csv_mc_b,"b jets","l");
      }
      else{
	legend_diff->AddEntry(h_diff,"(Data - b)","pe");
	legend_diff->AddEntry(h_csv_mc_nonb,"LF jets","l");
      }



      h_diff->Draw("pe1");
      if( isHF ) h_csv_mc_b->Draw("pe1same");
      else       h_csv_mc_nonb->Draw("pe1same");
      h_diff->Draw("pe1same");

      legend_diff->Draw();
      BinInfoLatex.Draw();

      c1->RedrawAxis();

      plotName = dirprefix + Form("csv_%s_SF_Pt%i_Eta%i_bdiff",flavor.Data(),iPt,iEta) + ".png";

      c1->Print(plotName.Data());



      h_ratio->GetYaxis()->SetRangeUser(0.,2.);
      h_ratio->SetTitle(";CSV;Data/MC SF");
      h_ratio->Draw("pe1");

      BinInfoLatex.Draw();

      c1->RedrawAxis();

      plotName = dirprefix + Form("csv_%s_SF_Pt%i_Eta%i_cratio",flavor.Data(),iPt,iEta) + ".png";

      c1->Print(plotName.Data());


      if( compareIterations ){


	TString h_iter_Data_Name = Form("h_csv_Data_Pt%i_Eta%i",iPt,iEta);
	TString h_iter_b_Name = Form("h_csv_MC_bjets_Pt%i_Eta%i",iPt,iEta);
	TString h_iter_nonb_Name = Form("h_csv_MC_nonbjets_Pt%i_Eta%i",iPt,iEta);
	TString h_iter_csv_ratio_Name = Form("temp_csv_ratio_Pt%i_Eta%i",iPt,iEta);
 
	TH1D* h_iter_Data_jet_csv_iter0 = (TH1D*)fitFile_iter0->Get(h_iter_Data_Name.Data())->Clone(h_iter_Data_Name+"v0");
	TH1D* h_iter_MC_b_jet_csv_iter0 = (TH1D*)fitFile_iter0->Get(h_iter_b_Name.Data())->Clone(h_iter_b_Name+"v0");
	TH1D* h_iter_MC_nonb_jet_csv_iter0 = (TH1D*)fitFile_iter0->Get(h_iter_nonb_Name.Data())->Clone(h_iter_nonb_Name+"v0");
	
	TH1D* h_iter_Data_jet_csv_iter1 = (TH1D*)fitFile_iter1->Get(h_iter_Data_Name.Data())->Clone(h_iter_Data_Name+"v1");
	TH1D* h_iter_MC_b_jet_csv_iter1 = (TH1D*)fitFile_iter1->Get(h_iter_b_Name.Data())->Clone(h_iter_b_Name+"v1");
	TH1D* h_iter_MC_nonb_jet_csv_iter1 = (TH1D*)fitFile_iter1->Get(h_iter_nonb_Name.Data())->Clone(h_iter_nonb_Name+"v1");

	TH1D* h_iter_Data_jet_csv_iter2 = (TH1D*)fitFile_iter2->Get(h_iter_Data_Name.Data())->Clone(h_iter_Data_Name+"v2");
	TH1D* h_iter_MC_b_jet_csv_iter2 = (TH1D*)fitFile_iter2->Get(h_iter_b_Name.Data())->Clone(h_iter_b_Name+"v2");
	TH1D* h_iter_MC_nonb_jet_csv_iter2 = (TH1D*)fitFile_iter2->Get(h_iter_nonb_Name.Data())->Clone(h_iter_nonb_Name+"v2");

	TH1D* h_iter0 = (TH1D*)h_iter_Data_jet_csv_iter0->Clone(h_iter_csv_ratio_Name+"v0");
	TH1D* h_iter1 = (TH1D*)h_iter_Data_jet_csv_iter1->Clone(h_iter_csv_ratio_Name+"v1");
	TH1D* h_iter2 = (TH1D*)h_iter_Data_jet_csv_iter2->Clone(h_iter_csv_ratio_Name+"v2");
	if( isHF ){
	  h_iter0->Add(h_iter_MC_nonb_jet_csv_iter0, -1);
	  h_iter0->Divide(h_iter_MC_b_jet_csv_iter0);

	  h_iter1->Add(h_iter_MC_nonb_jet_csv_iter1, -1);
	  h_iter1->Divide(h_iter_MC_b_jet_csv_iter1);

	  h_iter2->Add(h_iter_MC_nonb_jet_csv_iter2, -1);
	  h_iter2->Divide(h_iter_MC_b_jet_csv_iter2);
	}
	else {
	  h_iter0->Add(h_iter_MC_b_jet_csv_iter0, -1);
	  h_iter0->Divide(h_iter_MC_nonb_jet_csv_iter0);

	  h_iter1->Add(h_iter_MC_b_jet_csv_iter1, -1);
	  h_iter1->Divide(h_iter_MC_nonb_jet_csv_iter1);

	  h_iter2->Add(h_iter_MC_b_jet_csv_iter2, -1);
	  h_iter2->Divide(h_iter_MC_nonb_jet_csv_iter2);
	}


	TString iter_fit_histo_name = Form("csv_ratio_Pt%d_Eta%d_final",iPt,iEta);

	//TString iter_histo_name = Form("h_csv_ratio_%d",iHist);
	TH1D* h_fit_iter0 = (TH1D*)fitFile_iter0->Get(iter_fit_histo_name)->Clone(iter_fit_histo_name+"v0");
	TH1D* h_fit_iter1 = (TH1D*)fitFile_iter1->Get(iter_fit_histo_name)->Clone(iter_fit_histo_name+"v1");
	TH1D* h_fit_iter2 = (TH1D*)fitFile_iter2->Get(iter_fit_histo_name)->Clone(iter_fit_histo_name+"v2");

	h_fit_iter0->SetLineColor(kRed);
	h_fit_iter1->SetLineColor(kGreen+1);
	h_fit_iter2->SetLineColor(kBlue);

	h_fit_iter0->SetLineWidth(2);
	h_fit_iter1->SetLineWidth(2);
	h_fit_iter2->SetLineWidth(2);

	h_iter0->SetLineColor(kRed);
	h_iter1->SetLineColor(kGreen+1);
	h_iter2->SetLineColor(kBlue);

	h_iter0->SetLineWidth(2);
	h_iter1->SetLineWidth(2);
	h_iter2->SetLineWidth(2);

	h_fit_iter0->SetStats(0);
	h_fit_iter0->GetYaxis()->SetRangeUser(0.,2.);

	h_fit_iter0->SetTitle(";CSV;Data/MC CSV SF");

	h_fit_iter0->Draw("hist");
	h_iter0->Draw("pe1same");
	h_fit_iter1->Draw("histsame");
	h_iter1->Draw("pe1same");
	h_fit_iter2->Draw("histsame");
	h_iter2->Draw("pe1same");

	c1->RedrawAxis();


	TLegend *legend_iter = new TLegend(0.14,0.93,0.9,0.98);

	legend_iter->SetFillColor(kWhite);
	legend_iter->SetLineColor(kWhite);
	legend_iter->SetShadowColor(kWhite);
	legend_iter->SetTextFont(42);
	legend_iter->SetTextSize(0.05);

	legend_iter->SetNColumns(3);

	legend_iter->AddEntry(h_iter0,"Iter0","l");
	legend_iter->AddEntry(h_iter1,"Iter1","l");
	legend_iter->AddEntry(h_iter2,"Iter2","l");

	legend_iter->Draw();
	BinInfoLatex.Draw();

	c1->RedrawAxis();

	plotName = dirprefix + Form("csv_%s_SF_Pt%i_Eta%i_diters",flavor.Data(),iPt,iEta) + ".png";

	c1->Print(plotName.Data());
      }
    }
  }



  std::cout << "Done." << std::endl;

}
