#include "TROOT.h"
#include "TStyle.h"
#include "Riostream.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"

#include <algorithm>

#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "../interface/yggdrasil_NLO.h"

#endif

//*****************************************************************************


void yggdrasil_drawQuick_NLO(){

  gStyle->SetOptStat(0);
  gStyle->SetHistLineWidth(3);


  //
  // File 1: FxFx MadSpin ttjets+0jets
  //
  TFile *f_ttjets_0j = new TFile("/uscms_data/d3/jgwood/ttH_ttbb_lepJets/2012_analysis/anaTrees_2012_53x_NLOMC_20140701/Histos/yggdrasil_treeReader_NLO_TTJetsIncl_NLO_FxFx_4FS_tt_0Jet_max1Jet_8TeV_CTEQ6M_MadSpin_job1of1.root", "read");

  //
  // File 2: FxFx MadSpin ttjets+1jets
  //
  TFile *f_ttjets_1j = new TFile("/uscms_data/d3/jgwood/ttH_ttbb_lepJets/2012_analysis/anaTrees_2012_53x_NLOMC_20140701/Histos/yggdrasil_treeReader_NLO_TTJetsIncl_NLO_FxFx_4FS_tt_1Jet_max1Jet_8TeV_CTEQ6M_MadSpin_job1of1.root", "read");

  
  //
  // Delcare Histogram pointers for each
  //
  TH1F *h1_ttjets_0j;
  TH2F *h2_ttjets_0j;

  TH1F *h1_ttjets_1j;
  TH2F *h2_ttjets_1j;


  //
  // Declare pointer for canvas
  //
  TCanvas *can;
  TLegend *leg;
  
  double max = 1.0;
  double min = 0.0;

  //
  // Draw Plots
  //
  
  // ttjets fxfx madspin +0jets, gen jets pt vs eta
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genJet_pt_vs_eta");
  h2_ttjets_0j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genJet_pt_vs_eta.pdf");
  can->~TCanvas();

  // ttjets fxfx madspin +1jets, gen jets pt vs eta
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genJet_pt_vs_eta");
  h2_ttjets_1j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genJet_pt_vs_eta.pdf");
  can->~TCanvas();


  // ttjets fxfx madspin +0jets, selected gen jets pt vs eta
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genJet_selected_pt_vs_eta");
  h2_ttjets_0j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genJet_selected_pt_vs_eta.pdf");
  can->~TCanvas();

  // ttjets fxfx madspin +1jets, selected gen jets pt vs eta
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genJet_selected_pt_vs_eta");
  h2_ttjets_1j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genJet_selected_pt_vs_eta.pdf");
  can->~TCanvas();

  
  // ttjets fxfx madspin +0jets, leading gen jets pt vs eta
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genJet_leading_pt_vs_eta");
  h2_ttjets_0j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genJet_leading_pt_vs_eta.pdf");
  can->~TCanvas();

  // ttjets fxfx madspin +1jets, leading gen jets pt vs eta
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genJet_leading_pt_vs_eta");
  h2_ttjets_1j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genJet_leading_pt_vs_eta.pdf");
  can->~TCanvas();
  

  // nGenJets Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genJet_nGenJets");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genJet_nGenJets");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__nSelectedGenJets.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // ttjets+0j, matched not selected, pt vs eta
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta");
  h2_ttjets_0j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta.pdf");
  can->~TCanvas();

  // ttjets+1j, matched not selected, pt vs eta
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta");
  h2_ttjets_1j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta.pdf");
  can->~TCanvas();



  // genParticle top_quark pT Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_top_quark_pt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_top_quark_pt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_top_quark_pt.pdf");
  leg->~TLegend();
  can->~TCanvas();
  

  // genParticle top_quark mass Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_top_quark_mass");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_top_quark_mass");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_top_quark_mass.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // ttjets+0j dPtRel vs dR top
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genParticle_top_quark_dPtRel_vs_dR_final_intial");
  h2_ttjets_0j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genParticle_top_quark_dPtRel_vs_dR_final_intial.pdf");
  can->~TCanvas();

  // ttjets+1j dPtRel vs dR top
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genParticle_top_quark_dPtRel_vs_dR_final_intial");
  h2_ttjets_1j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genParticle_top_quark_dPtRel_vs_dR_final_intial.pdf");
  can->~TCanvas();



  // genParticle b_quark_fromTop pT Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_b_quark_fromTop_pt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_b_quark_fromTop_pt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_b_quark_fromTop_pt.pdf");
  leg->~TLegend();
  can->~TCanvas();
  

  // genParticle b_quark_fromTop mass Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_b_quark_fromTop_mass");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_b_quark_fromTop_mass");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_b_quark_fromTop_mass.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // ttjets+0j dPtRel vs dR b from top
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genParticle_b_quark_fromTop_dPtRel_vs_dR_final_intial");
  h2_ttjets_0j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genParticle_b_quark_fromTop_dPtRel_vs_dR_final_intial.pdf");
  can->~TCanvas();

  // ttjets+1j dPtRel vs dR b from top
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genParticle_b_quark_fromTop_dPtRel_vs_dR_final_intial");
  h2_ttjets_1j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genParticle_b_quark_fromTop_dPtRel_vs_dR_final_intial.pdf");
  can->~TCanvas();


  // genParticle w_boson_fromTop pT Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_w_boson_fromTop_pt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_w_boson_fromTop_pt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_w_boson_fromTop_pt.pdf");
  leg->~TLegend();
  can->~TCanvas();
  

  // genParticle w_boson_fromTop mass Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_w_boson_fromTop_mass");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_w_boson_fromTop_mass");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_w_boson_fromTop_mass.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // genParticle wDaughter1_fromTop pT Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_wDaughter1_fromTop_pt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_wDaughter1_fromTop_pt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_wDaughter1_fromTop_pt.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // ttjets+0j, genParticle, wDaughter1_fromTop, mother pdgId vs pdgId
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genParticle_wDaughter1_fromTop_motherPdgId_vs_pdgId");
  h2_ttjets_0j->Scale(1.0/h2_ttjets_0j->Integral());
  h2_ttjets_0j->Draw("colz");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genParticle_wDaughter1_fromTop_motherPdgId_vs_pdgId.pdf");
  can->~TCanvas();
  
  // ttjets+1j genParticle, wDaughter1_fromTop, mother pdgId vs pdgId
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genParticle_wDaughter1_fromTop_motherPdgId_vs_pdgId");
  h2_ttjets_1j->Scale(1.0/h2_ttjets_1j->Integral());
  h2_ttjets_1j->Draw("colz");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genParticle_wDaughter1_fromTop_motherPdgId_vs_pdgId.pdf");
  can->~TCanvas();


  // genParticle wDaughter2_fromTop pT Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_wDaughter2_fromTop_pt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_wDaughter2_fromTop_pt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_wDaughter2_fromTop_pt.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // ttjets+0j, genParticle, wDaughter2_fromTop, mother pdgId vs pdgId
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genParticle_wDaughter2_fromTop_motherPdgId_vs_pdgId");
  h2_ttjets_0j->Scale(1.0/h2_ttjets_0j->Integral());
  h2_ttjets_0j->Draw("colz");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genParticle_wDaughter2_fromTop_motherPdgId_vs_pdgId.pdf");
  can->~TCanvas();
  
  // ttjets+1j genParticle, wDaughter2_fromTop, mother pdgId vs pdgId
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genParticle_wDaughter2_fromTop_motherPdgId_vs_pdgId");
  h2_ttjets_1j->Scale(1.0/h2_ttjets_1j->Integral());
  h2_ttjets_1j->Draw("colz");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genParticle_wDaughter2_fromTop_motherPdgId_vs_pdgId.pdf");
  can->~TCanvas();


  
  // genParticle topBar_quark pT Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_topBar_quark_pt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_topBar_quark_pt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_topBar_quark_pt.pdf");
  leg->~TLegend();
  can->~TCanvas();
  

  // genParticle topBar_quark mass Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_topBar_quark_mass");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_topBar_quark_mass");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_topBar_quark_mass.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // ttjets+0j dPtRel vs dR topBar
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genParticle_topBar_quark_dPtRel_vs_dR_final_intial");
  h2_ttjets_0j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genParticle_topBar_quark_dPtRel_vs_dR_final_intial.pdf");
  can->~TCanvas();

  // ttjets+1j dPtRel vs dR topBar
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genParticle_topBar_quark_dPtRel_vs_dR_final_intial");
  h2_ttjets_1j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genParticle_topBar_quark_dPtRel_vs_dR_final_intial.pdf");
  can->~TCanvas();



  // genParticle b_quark_fromTopBar pT Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_b_quark_fromTopBar_pt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_b_quark_fromTopBar_pt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_b_quark_fromTopBar_pt.pdf");
  leg->~TLegend();
  can->~TCanvas();
  

  // genParticle b_quark_fromTopBar mass Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_b_quark_fromTopBar_mass");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_b_quark_fromTopBar_mass");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_b_quark_fromTopBar_mass.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // ttjets+0j dPtRel vs dR b from topBar
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genParticle_b_quark_fromTopBar_dPtRel_vs_dR_final_intial");
  h2_ttjets_0j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genParticle_b_quark_fromTopBar_dPtRel_vs_dR_final_intial.pdf");
  can->~TCanvas();

  // ttjets+1j dPtRel vs dR b from topBar
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genParticle_b_quark_fromTopBar_dPtRel_vs_dR_final_intial");
  h2_ttjets_1j->Draw("colz norm");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genParticle_b_quark_fromTopBar_dPtRel_vs_dR_final_intial.pdf");
  can->~TCanvas();


  // genParticle w_boson_fromTopBar pT Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_w_boson_fromTopBar_pt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_w_boson_fromTopBar_pt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_w_boson_fromTopBar_pt.pdf");
  leg->~TLegend();
  can->~TCanvas();
  

  // genParticle w_boson_fromTopBar mass Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_w_boson_fromTopBar_mass");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_w_boson_fromTopBar_mass");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_w_boson_fromTopBar_mass.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // genParticle wDaughter1_fromTopBar pT Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_wDaughter1_fromTopBar_pt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_wDaughter1_fromTopBar_pt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_wDaughter1_fromTopBar_pt.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // ttjets+0j, genParticle, wDaughter1_fromTopBar, mother pdgId vs pdgId
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genParticle_wDaughter1_fromTopBar_motherPdgId_vs_pdgId");
  h2_ttjets_0j->Scale(1.0/h2_ttjets_0j->Integral());
  h2_ttjets_0j->Draw("colz");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genParticle_wDaughter1_fromTopBar_motherPdgId_vs_pdgId.pdf");
  can->~TCanvas();
  
  // ttjets+1j genParticle, wDaughter1_fromTopBar, mother pdgId vs pdgId
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genParticle_wDaughter1_fromTopBar_motherPdgId_vs_pdgId");
  h2_ttjets_1j->Scale(1.0/h2_ttjets_1j->Integral());
  h2_ttjets_1j->Draw("colz");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genParticle_wDaughter1_fromTopBar_motherPdgId_vs_pdgId.pdf");
  can->~TCanvas();

  
  // genParticle wDaughter2_fromTopBar pT Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_wDaughter2_fromTopBar_pt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_wDaughter2_fromTopBar_pt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_wDaughter2_fromTopBar_pt.pdf");
  leg->~TLegend();
  can->~TCanvas();
  
  // ttjets+0j, genParticle, wDaughter2_fromTopBar, mother pdgId vs pdgId
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genParticle_wDaughter2_fromTopBar_motherPdgId_vs_pdgId");
  h2_ttjets_0j->Scale(1.0/h2_ttjets_0j->Integral());
  h2_ttjets_0j->Draw("colz");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genParticle_wDaughter2_fromTopBar_motherPdgId_vs_pdgId.pdf");
  can->~TCanvas();


  // ttjets+1j genParticle, wDaughter2_fromTopBar, mother pdgId vs pdgId
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genParticle_wDaughter2_fromTopBar_motherPdgId_vs_pdgId");
  h2_ttjets_1j->Scale(1.0/h2_ttjets_1j->Integral());
  h2_ttjets_1j->Draw("colz");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genParticle_wDaughter2_fromTopBar_motherPdgId_vs_pdgId.pdf");
  can->~TCanvas();




  // ttjets+0j nExtra b vs c quarks
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genParticle_nExtraCQuarks_vs_nExtraBQuarks");
  h2_ttjets_0j->Scale(1.0/h2_ttjets_0j->Integral());
  h2_ttjets_0j->Draw("colz text");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genParticle_nExtraCQuarks_vs_nExtraBQuarks.pdf");
  can->~TCanvas();
  
  // ttjets+1j nExtra b vs c quarks
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genParticle_nExtraCQuarks_vs_nExtraBQuarks");
  h2_ttjets_1j->Scale(1.0/h2_ttjets_1j->Integral());
  h2_ttjets_1j->Draw("colz text");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genParticle_nExtraCQuarks_vs_nExtraBQuarks.pdf");
  can->~TCanvas();
  

  // ttjets+0j nExtra b vs c jets
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genParticle_nExtraCJets_vs_nExtraBJets");
  h2_ttjets_0j->Scale(1.0/h2_ttjets_0j->Integral());
  h2_ttjets_0j->Draw("colz text");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genParticle_nExtraCJets_vs_nExtraBJets.pdf");
  can->~TCanvas();
  
  // ttjets+1j nExtra b vs c jets
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genParticle_nExtraCJets_vs_nExtraBJets");
  h2_ttjets_1j->Scale(1.0/h2_ttjets_1j->Integral());
  h2_ttjets_1j->Draw("colz text");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genParticle_nExtraCJets_vs_nExtraBJets.pdf");
  can->~TCanvas();


  // ttjets+0j vs 1j, pT spectrum of all b Quarks
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_extraBQuarks_pt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_0j->Rebin(5);
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_extraBQuarks_pt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  h1_ttjets_1j->Rebin(5);
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->GetXaxis()->SetRangeUser(0.0, 100.0);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_extraBQuarks_pt.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // ttjets+0j vs 1j, max pT of extra b Quarks
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_extraBQuarks_maxPt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_0j->Rebin(5);
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_extraBQuarks_maxPt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  h1_ttjets_1j->Rebin(5);
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->GetXaxis()->SetRangeUser(0.0, 100.0);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_extraBQuarks_maxPt.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // ttjets+0j vs 1j, min pT of extra b Quarks
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_extraBQuarks_minPt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_0j->Rebin(5);
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_extraBQuarks_minPt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  h1_ttjets_1j->Rebin(5);
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->GetXaxis()->SetRangeUser(0.0, 100.0);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_extraBQuarks_minPt.pdf");
  leg->~TLegend();
  can->~TCanvas();


  // ttjets+0j, genParticle, extraBQuarks, mother pdgId vs pdgId
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_extraBQuarks_mother_pdgId");
  h1_ttjets_0j->Scale(1.0/h2_ttjets_0j->Integral());
  h1_ttjets_0j->Draw("hist");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genParticle_extraBQuarks_mother_pdgId.pdf");
  can->~TCanvas();


  // ttjets+1j genParticle, extraBQuarks, mother pdgId 
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_extraBQuarks_mother_pdgId");
  h1_ttjets_1j->Scale(1.0/h2_ttjets_1j->Integral());
  h1_ttjets_1j->Draw("hist");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genParticle_extraBQuarks_mother_pdgId.pdf");
  can->~TCanvas();



  // genParticle tightLepton pT Comparison
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h1_ttjets_0j = (TH1F*)f_ttjets_0j->Get("h_genParticle_tightLepton_pt");
  h1_ttjets_0j->SetLineColor(kBlue);
  h1_ttjets_0j->Scale(1.0/h1_ttjets_0j->Integral());
  h1_ttjets_1j = (TH1F*)f_ttjets_1j->Get("h_genParticle_tightLepton_pt");
  h1_ttjets_1j->SetLineColor(kRed);
  h1_ttjets_1j->Scale(1.0/h1_ttjets_1j->Integral());
  leg = new TLegend( 0.70, 0.70, 0.90, 0.90);
  leg->AddEntry(h1_ttjets_0j, "FxFx MadSpin ttjets+0j", "l");
  leg->AddEntry(h1_ttjets_1j, "FxFx MadSpin ttjets+1j", "l");
  max = std::max( h1_ttjets_0j->GetBinContent( h1_ttjets_0j->GetMaximumBin() ), h1_ttjets_1j->GetBinContent( h1_ttjets_1j->GetMaximumBin() ) );
  max *= 1.1;
  h1_ttjets_0j->SetMaximum(max);
  h1_ttjets_0j->Draw("hist");
  h1_ttjets_1j->Draw("hist same");
  leg->Draw();
  can->SaveAs("ttjets_fxfx_madspin_0jet_vs_1jet__genParticle_tightLepton_pt.pdf");
  leg->~TLegend();
  can->~TCanvas();
  
  // ttjets+0j, genParticle, tightLepton, mother pdgId vs pdgId
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_0j = (TH2F*)f_ttjets_0j->Get("h_genParticle_tightLepton_motherPdgId_vs_pdgId");
  h2_ttjets_0j->Scale(1.0/h2_ttjets_0j->Integral());
  h2_ttjets_0j->Draw("colz");
  can->SaveAs("ttjets_fxfx_madspin_plus0jets__genParticle_tightLepton_motherPdgId_vs_pdgId.pdf");
  can->~TCanvas();


  // ttjets+1j genParticle, tightLepton, mother pdgId vs pdgId
  can = new TCanvas("can", "can", 10, 10, 1000, 1000);
  h2_ttjets_1j = (TH2F*)f_ttjets_1j->Get("h_genParticle_tightLepton_motherPdgId_vs_pdgId");
  h2_ttjets_1j->Scale(1.0/h2_ttjets_1j->Integral());
  h2_ttjets_1j->Draw("colz");
  can->SaveAs("ttjets_fxfx_madspin_plus1jets__genParticle_tightLepton_motherPdgId_vs_pdgId.pdf");
  can->~TCanvas();


  

  // Clean up
  f_ttjets_0j->Close();
  f_ttjets_1j->Close();

  return;
}
