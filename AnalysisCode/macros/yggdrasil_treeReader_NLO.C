#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"

#include <vector>
#include <string>


#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "../interface/yggdrasil_NLO.h"

#endif

void yggdrasil_treeReader_NLO(int sampleId, int maxNentries=-1, int Njobs=1, int jobN=1){

  //
  // Load files to read
  //
  std::cout << "   ===> load the root files! " << std::endl;

  if(maxNentries==0){
    cout << "  No entries, exiting..." << endl << endl;
    return;
  }


  analysisInfo_CMS_ttH_LJ analysisInfo;

  sampleInfo thisSample;
  for(int iSamp=0; iSamp<(int)analysisInfo.sampleList_trees_.size(); iSamp++){
    if( analysisInfo.sampleList_trees_[iSamp].id == sampleId ){
      thisSample = analysisInfo.sampleList_trees_[iSamp];
    }
  }

  TString treefilename = analysisInfo.path_tree_nlo_;
  treefilename += "yggdrasil_treeMaker_NLO_";
  treefilename += thisSample.name;
  treefilename += "_job*";
  treefilename += ".root";

  //TString treefilename = "/uscms_data/d3/jgwood/ttH_ttbb_lepJets/2012_analysis/anaTrees_2012_53x_NLOMC_20140701/anaTree_ttjets_NLO_test.root";

  TChain *chain = new TChain("worldTree");
  chain->Add(treefilename);


  //
  // Set Branch Addresses
  //
  BEANeventVars_NLO *eve=0;
  chain->SetBranchAddress("eve.", &eve);

  
  //
  // Create outputfile
  //
  TString histFileName = analysisInfo.path_histo_nlo_;
  histFileName += "yggdrasil_treeReader_NLO_";
  histFileName += thisSample.name;
  histFileName += "_job";
  histFileName += jobN;
  histFileName += "of";
  histFileName += Njobs;
  histFileName += ".root";

  //TString histFileName = "/uscms_data/d3/jgwood/ttH_ttbb_lepJets/2012_analysis/anaTrees_2012_53x_NLOMC_20140701/yggdrasil_treeReader_NLOMC_ttjets_test.root";

  TFile *histFile = new TFile(histFileName, "recreate");


  //
  // Declare Pointers to Histograms
  //
  int nBins_pt  = 300;
  double min_pt = 0.0;
  double max_pt = 300;

  int nBins_eta  = 100;
  double min_eta = 0.0;
  double max_eta = 3.5;
  
  int nBins_phi  = 100;
  double min_phi = -3.5;
  double max_phi = 3.5;

  int nBins_dR = 50;
  double min_dR = 0.0;
  double max_dR = 5.0;

  int nBins_dPtRel = 30;
  double min_dPtRel = 0.0;
  double max_dPtRel = 3.0;

  int nBins_pdgId = 200;
  double min_pdgId = -100;
  double max_pdgId = 100;

  int nBins_weight = 100;
  double min_weight = -10;
  double max_weight = 10;

  TH1F *h_nGenJets;

  TH1F *h_genJet_pt;
  TH1F *h_genJet_eta;
  TH1F *h_genJet_phi;
 
  TH1F *h_weight;

  TH2F *h_genJet_pt_vs_eta;
  TH2F *h_genJet_pdgId_vs_motherPdgId;

  TH2F *h_genJet_selected_pt_vs_eta;
  TH2F *h_genJet_selected_pdgId_vs_motherPdgId;

  TH2F *h_dPtRel_vs_dR_final_intial_gen_top;
  TH2F *h_dPtRel_vs_dR_final_intial_gen_topBar;
  TH2F *h_dPtRel_vs_dR_final_intial_gen_b_fromTop;
  TH2F *h_dPtRel_vs_dR_final_intial_gen_b_fromTopBar;  
  
  TH2F *h_matched_nonSelected_ttbar_genJet_pt_vs_eta;
  
  TH2F *h_nExtraCQuarks_vs_nExtraBQuarks;
  TH1F *h_genParticle_extraBQuarks_pt;
  TH1F *h_genParticle_extraCQuarks_pt;
  TH1F *h_genParticle_extraBQuarks_mother_pdgId;
  TH1F *h_genParticle_extraCQuarks_mother_pdgId;
 
  TH1F *h_maxPt_gen_extraBQuark;
  TH1F *h_minPt_gen_extraBQuark;
  
  TH1F *h_maxPt_gen_extraCQuark;
  TH1F *h_minPt_gen_extraCQuark;

  TH1F *h_maxPt_gen_extraQuark;
  TH1F *h_minPt_gen_extraQuark;

  

  //
  // Construct Histograms
  //

  // nGenJets
  h_nGenJets = new TH1F("h_nGenJets", "nGenJets, p_{T}>25, #eta<2.4", 8, 2.5, 10.5);

 
  // Gen Jet Kinematics
  h_genJet_pt = new TH1F("h_genJet_pt", "GenJet p_{T}",  nBins_pt, min_pt, max_pt);
  h_genJet_eta = new TH1F("h_genJet_eta", "GenJet #eta", nBins_eta, min_eta, max_eta);
  h_genJet_phi = new TH1F("h_genJet_phi", "GenJet #phi", nBins_phi, min_phi, max_phi);

  // Event Weight
  h_weight = new TH1F("h_weight", "Event Weight", nBins_weight, min_weight, max_weight);

  // Gen Jet Kinematics, 2-D
  h_genJet_pt_vs_eta = new TH2F("h_genJet_pt_vs_eta", "GenJet p_{T} vs #eta", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);
  h_genJet_pdgId_vs_motherPdgId = new TH2F("h_genJet_pdgId_vs_motherPdgId", "GenJet pdgId vs mother pdgId", nBins_pdgId, min_pdgId, max_pdgId, nBins_pdgId, min_pdgId, max_pdgId);

  h_genJet_selected_pt_vs_eta = new TH2F("h_genJet_selected_pt_vs_eta", "Selected GenJets p_{T} vs #eta", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);
  h_genJet_selected_pdgId_vs_motherPdgId = new TH2F("h_genJet_selected_pdgId_vs_motherPdgId", "Selected GenJets pdgId vs mother pdgId", nBins_pdgId, min_pdgId, max_pdgId, nBins_pdgId, min_pdgId, max_pdgId);

  // Gen Particle information
  h_dPtRel_vs_dR_final_intial_gen_top = new TH2F("h_dPtRel_vs_dR_final_intial_gen_top", "#DeltaP_{TRel} vs #DeltaR, gen t quark", nBins_dR, min_dR, max_dR, nBins_dPtRel, min_dPtRel, max_dPtRel);
  h_dPtRel_vs_dR_final_intial_gen_topBar = new TH2F("h_dPtRel_vs_dR_final_intial_gen_topBar", "#DeltaP_{TRel} vs #DeltaR, gen #bar{t} quark", nBins_dR, min_dR, max_dR, nBins_dPtRel, min_dPtRel, max_dPtRel);
  h_dPtRel_vs_dR_final_intial_gen_b_fromTop = new TH2F("h_dPtRel_vs_dR_final_intial_gen_b_fromTop", "#DeltaP_{TRel} vs #DeltaR, gen b quark from top", nBins_dR, min_dR, max_dR, nBins_dPtRel, min_dPtRel, max_dPtRel);
  h_dPtRel_vs_dR_final_intial_gen_b_fromTopBar = new TH2F("h_dPtRel_vs_dR_final_intial_gen_b_fromTopBar", "#DeltaP_{TRel} vs #DeltaR, gen b quark from topBar", nBins_dR, min_dR, max_dR, nBins_dPtRel, min_dPtRel, max_dPtRel);
  
  h_matched_nonSelected_ttbar_genJet_pt_vs_eta = new TH2F("h_matched_nonSelected_ttbar_genJet_pt_vs_eta", "p_{T} vs #eta, non-selected genJets, matchded to ttbar system genPartons", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);

  
  // Extra b/c quark information
  h_nExtraCQuarks_vs_nExtraBQuarks = new TH2F("h_nExtraCQuarks_vs_nExtraBQuarks", "nExtra C quarks vs nExtra B quarks", 8, -0.5, 7.5, 12, -0.5, 11.5); 
  h_genParticle_extraBQuarks_pt = new TH1F("h_genParticle_extraBQuarks_pt", "p_{T}, extra b quarks", nBins_pt, min_pt, max_pt);
  h_genParticle_extraCQuarks_pt = new TH1F("h_genParticle_extraCQuarks_pt", "p_{T}, extra c quarks", nBins_pt, min_pt, max_pt);
  h_genParticle_extraBQuarks_mother_pdgId = new TH1F("h_genParticle_extraBQuarks_mother_pdgId", "pdgId, mother of extra b quarks", nBins_pt, min_pt, max_pt);
  h_genParticle_extraCQuarks_mother_pdgId = new TH1F("h_genParticle_extraCQuarks_mother_pdgId", "pdgId, mother of extra c quarks", nBins_pt, min_pt, max_pt);
  
  h_maxPt_gen_extraBQuark = new TH1F("h_maxPt_gen_extraBQuarks_pt", "maximum p_{T}, extra b quarks", nBins_pt, min_pt, max_pt);
  h_minPt_gen_extraBQuark = new TH1F("h_minPt_gen_extraBQuarks_pt", "minimum p_{T}, extra b quarks", nBins_pt, min_pt, max_pt);
  
  h_maxPt_gen_extraCQuark = new TH1F("h_maxPt_gen_extraCQuarks_pt", "maximum p_{T}, extra c quarks", nBins_pt, min_pt, max_pt);
  h_minPt_gen_extraCQuark = new TH1F("h_minPt_gen_extraCQuarks_pt", "minimum p_{T}, extra c quarks", nBins_pt, min_pt, max_pt);
  
  h_maxPt_gen_extraQuark = new TH1F("h_maxPt_gen_extraQuarks_pt", "maximum p_{T}, extra quarks", nBins_pt, min_pt, max_pt);
  h_minPt_gen_extraQuark = new TH1F("h_minPt_gen_extraQuarks_pt", "minimum p_{T}, extra quarks", nBins_pt, min_pt, max_pt);
  

  
  //
  // Loop over events
  //
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


    // Check that event is in range
    if( ievt<firstEvent ) continue;
    if( ievt==lastEvent ) break;

    if( ievt==1 )        std::cout << "     Event " << ievt << std::endl;
    if( ievt%10000==0 && ievt!=1 ) std::cout << "           " << ievt << "\t" 
					     << int(double(ievt-firstEvent)/double(NeventsPerJob)*100) << "% done" << std::endl;

    if( ievt==(maxNentries+1) && ievt!=0 ) break;
    
    
    // Initialize Branch Address Objects
    eve->initialize(); 

    // Get Entry
    chain->GetEntry(ievt);
    
    // Set  Weights
    //double wgt = weight_;
    double wgt = 1.0;

    h_weight->Fill(eve->weight_, 1.0);

    
    //
    // Fill Histograms
    //

    // gen Particle information
       
    // dPtRel vs dR for top, topbar, b, bbar between final, intial stages
    h_dPtRel_vs_dR_final_intial_gen_top->Fill( eve->dR_final_intial_gen_top_, eve->dPtRel_final_intial_gen_top_, wgt );
    h_dPtRel_vs_dR_final_intial_gen_topBar->Fill( eve->dR_final_intial_gen_topBar_, eve->dPtRel_final_intial_gen_topBar_, wgt );
    h_dPtRel_vs_dR_final_intial_gen_b_fromTop->Fill( eve->dR_final_intial_gen_b_fromTop_, eve->dPtRel_final_intial_gen_b_fromTop_, wgt );
    h_dPtRel_vs_dR_final_intial_gen_b_fromTopBar->Fill( eve->dR_final_intial_gen_b_fromTopBar_, eve->dPtRel_final_intial_gen_b_fromTopBar_, wgt );  


    // GenJets Matched to ttbar system, not passing selection
    for(int iGenJet=0; iGenJet<(int)eve->matched_nonSelected_ttbar_genJet_pt_.size(); iGenJet++){
      h_matched_nonSelected_ttbar_genJet_pt_vs_eta->Fill(  eve->matched_nonSelected_ttbar_genJet_eta_[iGenJet],  eve->matched_nonSelected_ttbar_genJet_pt_[iGenJet], wgt );
    }
    
    // N extra B/C Quarks
    h_nExtraCQuarks_vs_nExtraBQuarks->Fill( eve->nExtraBQuarks_, eve->nExtraCQuarks_, wgt);

    // Kinematics of extra B/C Quarks

    // pt, B
    for(int iExtraB=0; iExtraB<(int)eve->genParticle_extraBQuarks_pt_.size(); iExtraB++){
      h_genParticle_extraBQuarks_pt->Fill( eve->genParticle_extraBQuarks_pt_[iExtraB], wgt );
    }
    
    // pt, C
    for(int iExtraC=0; iExtraC<(int)eve->genParticle_extraCQuarks_pt_.size(); iExtraC++){
      h_genParticle_extraCQuarks_pt->Fill( eve->genParticle_extraCQuarks_pt_[iExtraC], wgt );
    }
    
    // pdgId of B mother
    for(int iExtraB=0; iExtraB<(int)eve->genParticle_extraBQuarks_mother_pdgId_.size(); iExtraB++){
      h_genParticle_extraBQuarks_mother_pdgId->Fill( eve->genParticle_extraBQuarks_mother_pdgId_[iExtraB], wgt );
    }

    // pdgId of C mother
    for(int iExtraC=0; iExtraC<(int)eve->genParticle_extraCQuarks_mother_pdgId_.size(); iExtraC++){
      h_genParticle_extraCQuarks_mother_pdgId->Fill( eve->genParticle_extraCQuarks_mother_pdgId_[iExtraC], wgt );
    }
    
    // Max/Min pT of extra B Quarks
    if(eve->nExtraBQuarks_>0){
      h_maxPt_gen_extraBQuark->Fill( eve->maxPt_gen_extraBQuark_, wgt );
      h_minPt_gen_extraBQuark->Fill( eve->minPt_gen_extraBQuark_, wgt );
      h_maxPt_gen_extraQuark->Fill( eve->maxPt_gen_extraBQuark_, wgt );
      h_minPt_gen_extraQuark->Fill( eve->minPt_gen_extraBQuark_, wgt );
    }

    // Max/Min pT of extra C Quarks
    if(eve->nExtraCQuarks_>0){
      h_maxPt_gen_extraCQuark->Fill( eve->maxPt_gen_extraCQuark_, wgt );
      h_minPt_gen_extraCQuark->Fill( eve->minPt_gen_extraCQuark_, wgt );
      h_maxPt_gen_extraQuark->Fill( eve->maxPt_gen_extraCQuark_, wgt );
      h_minPt_gen_extraQuark->Fill( eve->minPt_gen_extraCQuark_, wgt );
    }
    
    
    // Loop over genJets
    int nSelectedGenJets=0;
    for(int iGenJet=0; iGenJet<(int)eve->genJet_pt_.size(); iGenJet++){
      h_genJet_pt->Fill(eve->genJet_pt_[iGenJet], wgt);
      h_genJet_eta->Fill(fabs(eve->genJet_eta_[iGenJet]), wgt);
      h_genJet_phi->Fill(eve->genJet_phi_[iGenJet], wgt);

      h_genJet_pt_vs_eta->Fill(fabs(eve->genJet_eta_[iGenJet]), eve->genJet_pt_[iGenJet], wgt);
      h_genJet_pdgId_vs_motherPdgId->Fill(eve->genJet_pdgId_[iGenJet], eve->genJet_mother_pdgId_[iGenJet], wgt);

      if( eve->genJet_isSelected_[iGenJet] ){
        nSelectedGenJets++;
        h_genJet_selected_pt_vs_eta->Fill(fabs(eve->genJet_eta_[iGenJet]), eve->genJet_pt_[iGenJet], wgt);
        h_genJet_selected_pdgId_vs_motherPdgId->Fill(eve->genJet_pdgId_[iGenJet], eve->genJet_mother_pdgId_[iGenJet], wgt);
      }

    } // end loop over genJets

    h_nGenJets->Fill( nSelectedGenJets, wgt );




  } // end loop over events

  std::cout << " Done! " << std::endl;
  
  // Clean Up
  histFile->Write();
  histFile->Close();
  
  
  return;
}
