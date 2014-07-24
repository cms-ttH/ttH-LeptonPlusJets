#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"

#include <algorithm>
#include <vector>
#include <string>


#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "../interface/yggdrasil_NLO.h"

#endif

void yggdrasil_treeReader_NLO(int sampleId, int maxNentries=-1, int Njobs=1, int jobN=1){

 
  ////////////////////////
  ///
  /// Load files to read
  ///
  ////////////////////////

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
  


  ///////////////////////////////////
  ///
  //
  /// Declare Pointers to Histograms
  ///
  ///////////////////////////////////


  // Event Variables
  TH1F *h_weight;

  // GenJet variables
  TH1F *h_genJet_nGenJets;
  TH1F *h_genJet_pt;
  TH1F *h_genJet_eta;
  TH1F *h_genJet_phi;
  
  TH2F *h_genJet_pt_vs_eta;
  TH2F *h_genJet_selected_pt_vs_eta;
  TH2F *h_genJet_leading_pt_vs_eta;
  
  TH2F *h_genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta;
  TH2F *h_genJet_matchedTo_b_quark_fromTop_pt_vs_eta;
  TH2F *h_genJet_matchedTo_wDaughter1_fromTop_pt_vs_eta;
  TH2F *h_genJet_matchedTo_wDaughter2_fromTop_pt_vs_eta;

  TH2F *h_genJet_matchedTo_b_quark_fromTopBar_pt_vs_eta;
  TH2F *h_genJet_matchedTo_wDaughter1_fromTopBar_pt_vs_eta;
  TH2F *h_genJet_matchedTo_wDaughter2_fromTopBar_pt_vs_eta;

  // GenParticle Variables
  TH1F *h_genParticle_top_quark_pt;
  TH1F *h_genParticle_top_quark_eta;
  TH1F *h_genParticle_top_quark_mass;
  TH2F *h_genParticle_top_quark_dPtRel_vs_dR_final_intial;

  TH1F *h_genParticle_b_quark_fromTop_pt;
  TH1F *h_genParticle_b_quark_fromTop_eta;
  TH1F *h_genParticle_b_quark_fromTop_mass;
  TH2F *h_genParticle_b_quark_fromTop_dPtRel_vs_dR_final_intial;

  TH1F *h_genParticle_w_boson_fromTop_pt;
  TH1F *h_genParticle_w_boson_fromTop_eta;
  TH1F *h_genParticle_w_boson_fromTop_mass;
  
  TH1F *h_genParticle_wDaughter1_fromTop_pt;
  TH1F *h_genParticle_wDaughter1_fromTop_eta;
  TH2F *h_genParticle_wDaughter1_fromTop_motherPdgId_vs_pdgId;

  TH1F *h_genParticle_wDaughter2_fromTop_pt;
  TH1F *h_genParticle_wDaughter2_fromTop_eta;
  TH2F *h_genParticle_wDaughter2_fromTop_motherPdgId_vs_pdgId;
  
  TH1F *h_genParticle_topBar_quark_pt;
  TH1F *h_genParticle_topBar_quark_eta;
  TH1F *h_genParticle_topBar_quark_mass;
  TH2F *h_genParticle_topBar_quark_dPtRel_vs_dR_final_intial;

  TH1F *h_genParticle_b_quark_fromTopBar_pt;
  TH1F *h_genParticle_b_quark_fromTopBar_eta;
  TH1F *h_genParticle_b_quark_fromTopBar_mass;
  TH2F *h_genParticle_b_quark_fromTopBar_dPtRel_vs_dR_final_intial;

  TH1F *h_genParticle_w_boson_fromTopBar_pt;
  TH1F *h_genParticle_w_boson_fromTopBar_eta;
  TH1F *h_genParticle_w_boson_fromTopBar_mass;
  
  TH1F *h_genParticle_wDaughter1_fromTopBar_pt;
  TH1F *h_genParticle_wDaughter1_fromTopBar_eta;
  TH2F *h_genParticle_wDaughter1_fromTopBar_motherPdgId_vs_pdgId;

  TH1F *h_genParticle_wDaughter2_fromTopBar_pt;
  TH1F *h_genParticle_wDaughter2_fromTopBar_eta;
  TH2F *h_genParticle_wDaughter2_fromTopBar_motherPdgId_vs_pdgId;
  
   
  TH2F *h_genParticle_nExtraCQuarks_vs_nExtraBQuarks;
  TH2F *h_genParticle_nExtraCJets_vs_nExtraBJets;

  TH1F *h_genParticle_extraBQuarks_pt;
  TH1F *h_genParticle_extraBQuarks_maxPt;
  TH1F *h_genParticle_extraBQuarks_minPt;
  TH1F *h_genParticle_extraBQuarks_mother_pdgId;
  
  TH1F *h_genParticle_extraCQuarks_pt;
  TH1F *h_genParticle_extraCQuarks_maxPt;
  TH1F *h_genParticle_extraCQuarks_minPt;
  TH1F *h_genParticle_extraCQuarks_mother_pdgId;
 
  TH1F *h_genParticle_extraQuarks_pt;
  TH1F *h_genParticle_extraQuarks_maxPt;
  TH1F *h_genParticle_extraQuarks_minPt;
  TH1F *h_genParticle_extraQuarks_mother_pdgId;
  
  
  TH1F *h_genParticle_tightLepton_pt;
  TH1F *h_genParticle_tightLepton_eta;
  TH1F *h_genParticle_tightLepton_status;
  TH2F *h_genParticle_tightLepton_motherPdgId_vs_pdgId;
  


  ///////////////////////////
  ///
  ///
  /// Construct Histograms
  ///
  ///////////////////////////

  int nBins_pt  = 300;
  double min_pt = 0.0;
  double max_pt = 300;

  int nBins_eta  = 100;
  double min_eta = 0.0;
  double max_eta = 3.5;
  
  int nBins_phi  = 100;
  double min_phi = -3.5;
  double max_phi = 3.5;

  int nBins_topMass  = 300;
  double min_topMass = 0.0;
  double max_topMass = 300.0;

  int nBins_wMass  = 200;
  double min_wMass = 0.0;
  double max_wMass = 160.0;

  int nBins_bMass  = 100;
  double min_bMass = 0.0;
  double max_bMass = 50.0;

  int nBins_dR = 50;
  double min_dR = 0.0;
  double max_dR = 5.0;

  int nBins_dPtRel = 30;
  double min_dPtRel = 0.0;
  double max_dPtRel = 3.0;

  int nBins_pdgId = 50;
  double min_pdgId = -25;
  double max_pdgId = 25;

  int nBins_weight = 2000;
  double min_weight = -1000;
  double max_weight = 1000;

  //
  // Event Variables
  //

  // Event Weight
  h_weight = new TH1F("h_weight", "Event Weight", nBins_weight, min_weight, max_weight);


  //
  // GenJet Variables
  //

  // nGenJets
  h_genJet_nGenJets = new TH1F("h_genJet_nGenJets", "N Selected GenJets, p_{T}>25, #eta<2.4;N GenJets", 8, 2.5, 10.5);

  // Gen Jet Kinematics
  h_genJet_pt = new TH1F("h_genJet_pt", "GenJet p_{T};p_{T} GeV",  nBins_pt, min_pt, max_pt);
  h_genJet_eta = new TH1F("h_genJet_eta", "GenJet #eta;#eta", nBins_eta, min_eta, max_eta);
  h_genJet_phi = new TH1F("h_genJet_phi", "GenJet #phi;#phi", nBins_phi, min_phi, max_phi);
  
  // Gen Jet Kinematics, 2-D
  h_genJet_pt_vs_eta = new TH2F("h_genJet_pt_vs_eta", "GenJet p_{T} vs #eta;#eta;p_{T} GeV", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);
  h_genJet_selected_pt_vs_eta = new TH2F("h_genJet_selected_pt_vs_eta", "Selected GenJets p_{T} vs #eta;#eta;p_{T} GeV", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);
  h_genJet_leading_pt_vs_eta = new TH2F("h_genJet_leading_pt_vs_eta", "Leading GenJet p_{T} vs #eta;#eta;p_{T} GeV", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);

  // GenJets Matched to ttbar system genPartons
  h_genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta = new TH2F("h_genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta", "GenJet, matched to t#bar{t} system, failing selection, p_{T} vs #eta;#eta;p_{T} GeV", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);

  h_genJet_matchedTo_b_quark_fromTop_pt_vs_eta = new TH2F("h_genJet_matchedTo_b_quark_fromTop_pt_vs_eta", "GenJet, matched to b-quark from t-quark, p_{T} vs #eta;#eta;p_{T} GeV", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);
  h_genJet_matchedTo_wDaughter1_fromTop_pt_vs_eta = new TH2F("h_genJet_matchedTo_wDaughter1_fromTop_pt_vs_eta", "GenJet, matched to wDaughter1 from t-quark, p_{T} vs #eta;#eta;p_{T} GeV", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);
  h_genJet_matchedTo_wDaughter2_fromTop_pt_vs_eta = new TH2F("h_genJet_matchedTo_wDaughter2_fromTop_pt_vs_eta", "GenJet, matched to wDaughter2 from t-quark, p_{T} vs #eta;#eta;p_{T} GeV", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);

  h_genJet_matchedTo_b_quark_fromTopBar_pt_vs_eta = new TH2F("h_genJet_matchedTo_b_quark_fromTopBar_pt_vs_eta", "GenJet, matched to b-quark from #bar{t}-quark, p_{T} vs #eta;#eta;p_{T} GeV", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);
  h_genJet_matchedTo_wDaughter1_fromTopBar_pt_vs_eta = new TH2F("h_genJet_matchedTo_wDaughter1_fromTopBar_pt_vs_eta", "GenJet, matched to wDaughter1 from #bar{t}-quark, p_{T} vs #eta;#eta;p_{T} GeV", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);
  h_genJet_matchedTo_wDaughter2_fromTopBar_pt_vs_eta = new TH2F("h_genJet_matchedTo_wDaughter2_fromTopBar_pt_vs_eta", "GenJet, matched to wDaughter2 from #bar{t}-quark, p_{T} vs #eta;#eta;p_{T} GeV", nBins_eta, min_eta, max_eta, nBins_pt, min_pt, max_pt);

  
  //
  // GenParticle Variables
  //

  
  // GenParticle ttbar system Variables
  h_genParticle_top_quark_pt = new TH1F("h_genParticle_top_quark_pt", "GenParticle, t-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_top_quark_eta = new TH1F("h_genParticle_top_quark_eta", "GenParticle, t-quark, #eta;#eta", nBins_eta, min_eta, max_eta);
  h_genParticle_top_quark_mass = new TH1F("h_genParticle_top_quark_mass", "GenParticle, t-quark, mass;GeV", nBins_topMass, min_topMass, max_topMass);
  h_genParticle_top_quark_dPtRel_vs_dR_final_intial = new TH2F("h_genParticle_top_quark_dPtRel_vs_dR_final_intial", "GenParticle, t-quark, #Deltap_{T_{Rel}} vs #DeltaR for final vs intial states", nBins_dR, min_dR, max_dR, nBins_dPtRel, min_dPtRel, max_dPtRel);
  
  h_genParticle_b_quark_fromTop_pt = new TH1F("h_genParticle_b_quark_fromTop_pt", "GenParticle, b-quark from t-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_b_quark_fromTop_eta = new TH1F("h_genParticle_b_quark_fromTop_eta", "GenParticle, b-quark from t-quark, #eta;#eta", nBins_eta, min_eta, max_eta);
  h_genParticle_b_quark_fromTop_mass = new TH1F("h_genParticle_b_quark_fromTop_mass", "GenParticle, b-quark from t-quark, mass;GeV", nBins_bMass, min_bMass, max_bMass);
  h_genParticle_b_quark_fromTop_dPtRel_vs_dR_final_intial = new TH2F("h_genParticle_b_quark_fromTop_dPtRel_vs_dR_final_intial", "GenParticle, b-quark from t-quark, #Deltap_{T_{Rel}} vs #DeltaR for final vs intial states", nBins_dR, min_dR, max_dR, nBins_dPtRel, min_dPtRel, max_dPtRel);
  
  h_genParticle_w_boson_fromTop_pt = new TH1F("h_genParticle_w_boson_fromTop_pt", "GenParticle, W boson from t-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_w_boson_fromTop_eta = new TH1F("h_genParticle_w_boson_fromTop_eta", "GenParticle, W boson from t-quark, #eta;#eta", nBins_eta, min_eta, max_eta);
  h_genParticle_w_boson_fromTop_mass = new TH1F("h_genParticle_w_boson_fromTop_mass", "GenParticle, W boson from t-quark, mass;GeV", nBins_wMass, min_wMass, max_wMass);
 
  h_genParticle_wDaughter1_fromTop_pt = new TH1F("h_genParticle_wDaughter1_fromTop_pt", "GenParticle, Daughter 1 of W boson from t-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_wDaughter1_fromTop_eta = new TH1F("h_genParticle_wDaughter1_fromTop_eta", "GenParticle, Daughter 1 of W boson from t-quark, #eta;#eta", nBins_eta, min_eta, max_eta);
  h_genParticle_wDaughter1_fromTop_motherPdgId_vs_pdgId = new TH2F("h_genParticle_wDaughter1_fromTop_motherPdgId_vs_pdgId", "GenParticle, Daughter 1 of W boson from t-quark, mother pdgId vs pdgId;pdgId;mother pdgId", nBins_pdgId, min_pdgId, max_pdgId, nBins_pdgId, min_pdgId, max_pdgId);
 
  h_genParticle_wDaughter2_fromTop_pt = new TH1F("h_genParticle_wDaughter2_fromTop_pt", "GenParticle, Daughter 2 of W boson from t-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_wDaughter2_fromTop_eta = new TH1F("h_genParticle_wDaughter2_fromTop_eta", "GenParticle, Daughter 2 of W boson from t-quark, #eta;#eta", nBins_eta, min_eta, max_eta);
  h_genParticle_wDaughter2_fromTop_motherPdgId_vs_pdgId = new TH2F("h_genParticle_wDaughter2_fromTop_motherPdgId_vs_pdgId", "GenParticle, Daughter 2 of W boson from t-quark, mother pdgId vs pdgId;pdgId;mother pdgId", nBins_pdgId, min_pdgId, max_pdgId, nBins_pdgId, min_pdgId, max_pdgId);
  

  h_genParticle_topBar_quark_pt = new TH1F("h_genParticle_topBar_quark_pt", "GenParticle, #bar{t}-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_topBar_quark_eta = new TH1F("h_genParticle_topBar_quark_eta", "GenParticle, #bar{t}-quark, #eta;#eta", nBins_eta, min_eta, max_eta);
  h_genParticle_topBar_quark_mass = new TH1F("h_genParticle_topBar_quark_mass", "GenParticle, #bar{t}-quark, mass;GeV", nBins_topMass, min_topMass, max_topMass);
  h_genParticle_topBar_quark_dPtRel_vs_dR_final_intial = new TH2F("h_genParticle_topBar_quark_dPtRel_vs_dR_final_intial", "GenParticle, #bar{t}-quark, #Deltap_{T_{Rel}} vs #DeltaR for final vs intial states", nBins_dR, min_dR, max_dR, nBins_dPtRel, min_dPtRel, max_dPtRel);
  
  h_genParticle_b_quark_fromTopBar_pt = new TH1F("h_genParticle_b_quark_fromTopBar_pt", "GenParticle, b-quark from #bar{t}-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_b_quark_fromTopBar_eta = new TH1F("h_genParticle_b_quark_fromTopBar_eta", "GenParticle, b-quark from #bar{t}-quark, #eta;#eta", nBins_eta, min_eta, max_eta);
  h_genParticle_b_quark_fromTopBar_mass = new TH1F("h_genParticle_b_quark_fromTopBar_mass", "GenParticle, b-quark from #bar{t}-quark, mass;GeV", nBins_bMass, min_bMass, max_bMass);
  h_genParticle_b_quark_fromTopBar_dPtRel_vs_dR_final_intial = new TH2F("h_genParticle_b_quark_fromTopBar_dPtRel_vs_dR_final_intial", "GenParticle, b-quark from #bar{t}-quark, #Deltap_{T_{Rel}} vs #DeltaR for final vs intial states", nBins_dR, min_dR, max_dR, nBins_dPtRel, min_dPtRel, max_dPtRel);
  
  h_genParticle_w_boson_fromTopBar_pt = new TH1F("h_genParticle_w_boson_fromTopBar_pt", "GenParticle, W boson from #bar{t}-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_w_boson_fromTopBar_eta = new TH1F("h_genParticle_w_boson_fromTopBar_eta", "GenParticle, W boson from #bar{t}-quark, #eta;#eta", nBins_eta, min_eta, max_eta);
  h_genParticle_w_boson_fromTopBar_mass = new TH1F("h_genParticle_w_boson_fromTopBar_mass", "GenParticle, W boson from #bar{t}-quark, mass;GeV", nBins_wMass, min_wMass, max_wMass);
 
  h_genParticle_wDaughter1_fromTopBar_pt = new TH1F("h_genParticle_wDaughter1_fromTopBar_pt", "GenParticle, Daughter 1 of W boson from #bar{t}-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_wDaughter1_fromTopBar_eta = new TH1F("h_genParticle_wDaughter1_fromTopBar_eta", "GenParticle, Daughter 1 of W boson from #bar{t}-quark, #eta;#eta", nBins_eta, min_eta, max_eta);
  h_genParticle_wDaughter1_fromTopBar_motherPdgId_vs_pdgId = new TH2F("h_genParticle_wDaughter1_fromTopBar_motherPdgId_vs_pdgId", "GenParticle, Daughter 1 of W boson from #bar{t}-quark, mother pdgId vs pdgId;pdgId;mother pdgId", nBins_pdgId, min_pdgId, max_pdgId, nBins_pdgId, min_pdgId, max_pdgId);
 
  h_genParticle_wDaughter2_fromTopBar_pt = new TH1F("h_genParticle_wDaughter2_fromTopBar_pt", "GenParticle, Daughter 2 of W boson from #bar{t}-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_wDaughter2_fromTopBar_eta = new TH1F("h_genParticle_wDaughter2_fromTopBar_eta", "GenParticle, Daughter 2 of W boson from #bar{t}-quark, #eta;#eta", nBins_eta, min_eta, max_eta);
  h_genParticle_wDaughter2_fromTopBar_motherPdgId_vs_pdgId = new TH2F("h_genParticle_wDaughter2_fromTopBar_motherPdgId_vs_pdgId", "GenParticle, Daughter 2 of W boson from #bar{t}-quark, mother pdgId vs pdgId;pdgId;mother pdgId", nBins_pdgId, min_pdgId, max_pdgId, nBins_pdgId, min_pdgId, max_pdgId);
 
 
  
  // Gen Particles, extra b/c quarks in event
  h_genParticle_nExtraCQuarks_vs_nExtraBQuarks = new TH2F("h_genParticle_nExtraCQuarks_vs_nExtraBQuarks", "GenParticle, nExtra c-quarks vs nExtra b-quarks;nExtra b-quarks;nExtra c-quarks", 7, -0.5, 6.5, 13, -0.5, 12.5);
  h_genParticle_nExtraCJets_vs_nExtraBJets = new TH2F("h_genParticle_nExtraCJets_vs_nExtraBJets", "GenParticle, nExtra b/c-quarks matched to GenJets;nExtra b GenJets;nExtra c GenJets", 5, -0.5, 4.5, 5, -0.5, 4.5);

  h_genParticle_extraBQuarks_pt = new TH1F("h_genParticle_extraBQuarks_pt", "GenParticle, extra b-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_extraBQuarks_maxPt = new TH1F("h_genParticle_extraBQuarks_maxPt", "GenParticle, extra b-quarks, leading quark p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_extraBQuarks_minPt = new TH1F("h_genParticle_extraBQuarks_minPt", "GenParticle, extra b-quarks, most trailing quark p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);;
  h_genParticle_extraBQuarks_mother_pdgId = new TH1F("h_genParticle_extraBQuarks_mother_pdgId", "GenParticle, extra b-quarks, mother pdgId;mother pdgId", nBins_pdgId, min_pdgId, max_pdgId);

  h_genParticle_extraCQuarks_pt = new TH1F("h_genParticle_extraCQuarks_pt", "GenParticle, extra c-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_extraCQuarks_maxPt = new TH1F("h_genParticle_extraCQuarks_maxPt", "GenParticle, extra c-quarks, leading quark p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_extraCQuarks_minPt = new TH1F("h_genParticle_extraCQuarks_minPt", "GenParticle, extra c-quarks, most trailing quark p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);;
  h_genParticle_extraCQuarks_mother_pdgId = new TH1F("h_genParticle_extraCQuarks_mother_pdgId", "GenParticle, extra c-quarks, mother pdgId;mother pdgId", nBins_pdgId, min_pdgId, max_pdgId);
  
  h_genParticle_extraQuarks_pt = new TH1F("h_genParticle_extraQuarks_pt", "GenParticle, extra b/c-quark, p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_extraQuarks_maxPt = new TH1F("h_genParticle_extraQuarks_maxPt", "GenParticle, extra b/c-quarks, leading quark p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);
  h_genParticle_extraQuarks_minPt = new TH1F("h_genParticle_extraQuarks_minPt", "GenParticle, extra b/c-quarks, most trailing quark p_{T};p_{T} GeV", nBins_pt, min_pt, max_pt);;
  h_genParticle_extraQuarks_mother_pdgId = new TH1F("h_genParticle_extraQuarks_mother_pdgId", "GenParticle, extra b/c-quarks, mother pdgId;mother pdgId", nBins_pdgId, min_pdgId, max_pdgId);
  
 
  // Gen Particle, tight lepton passing selection
  h_genParticle_tightLepton_pt = new TH1F("h_genParticle_tightLepton_pt", "GenParticle, tight lepton passing selection, p_{T};p_{T}", nBins_pt, min_pt, max_pt);
  h_genParticle_tightLepton_eta = new TH1F("h_genParticle_tightLepton_eta", "GenParticle, tight lepton passing selection, #eta;#eta", nBins_eta, min_eta, max_eta);
  h_genParticle_tightLepton_status = new TH1F("h_genParticle_tightLepton_status", "GenParticle, tight lepton passing selection, status;status", 100, 0, 100);
  h_genParticle_tightLepton_motherPdgId_vs_pdgId = new TH2F("h_genParticle_tightLepton_motherPdgId_vs_pdgId", "GenParticle, tight lepton passing selection, mother pdgId vs pdgId;pdgId;mother pdgId", nBins_pdgId, min_pdgId, max_pdgId, nBins_pdgId, min_pdgId, max_pdgId);
  


  //////////////////////
  ///
  ///
  /// Loop over events
  ///
  //////////////////////

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

    TLorentzVector mydummyguy;
    mydummyguy.SetPxPyPzE(0,0,0,0);


    // Get Entry
    chain->GetEntry(ievt);
    
    // Set  Weights
    //double wgt = eve->weight_;
    double wgt = 1.0;



    
    /////////////////////////
    ///
    /// Fill Histograms
    ///
    /////////////////////////

    h_weight->Fill(eve->weight_, 1.0);


    //
    // Loop over genJets
    //

    int nSelectedGenJets=0;
    double genJet_leading_jet_pt=0.0;
    double genJet_leading_jet_eta=0.0;
    for(int iGenJet=0; iGenJet<(int)eve->genJet_TLV_.size(); iGenJet++){

      vdouble genJet_vect = eve->genJet_TLV_[iGenJet];
      TLorentzVector genJet_TLV = mydummyguy;
      genJet_TLV.SetPxPyPzE(genJet_vect[0], genJet_vect[1], genJet_vect[2], genJet_vect[3]);

      h_genJet_pt->Fill(genJet_TLV.Pt(), wgt);
      h_genJet_eta->Fill(fabs(genJet_TLV.Eta()), wgt);
      h_genJet_phi->Fill(genJet_TLV.Phi(), wgt);

      h_genJet_pt_vs_eta->Fill(fabs(genJet_TLV.Eta()), genJet_TLV.Pt(), wgt);
     
      if( eve->genJet_isSelected_[iGenJet] ){
        nSelectedGenJets++;
        h_genJet_selected_pt_vs_eta->Fill(fabs(genJet_TLV.Eta()), genJet_TLV.Pt(), wgt);
      } // end if genJet is selected

      // Search for max pT genJet
      if(genJet_TLV.Pt()>genJet_leading_jet_pt){
	genJet_leading_jet_pt  = genJet_TLV.Pt();
	genJet_leading_jet_eta = genJet_TLV.Eta();
      } 

    } // end loop over genJets

    h_genJet_nGenJets->Fill( nSelectedGenJets, wgt );
    h_genJet_leading_pt_vs_eta->Fill(fabs(genJet_leading_jet_eta), genJet_leading_jet_pt, wgt);

    
    //
    // GenJets matched to ttbar system
    //
    vdouble genJet_matchedTo_b_quark_fromTop_vect = eve->genJet_matchedTo_b_quark_fromTop_TLV_;
    TLorentzVector genJet_matchedTo_b_quark_fromTop_TLV = mydummyguy;
   
    if( genJet_matchedTo_b_quark_fromTop_vect[0]!=0 &&
	genJet_matchedTo_b_quark_fromTop_vect[1]!=0 &&
	genJet_matchedTo_b_quark_fromTop_vect[2]!=0 &&
	genJet_matchedTo_b_quark_fromTop_vect[3]!=0    ){
      genJet_matchedTo_b_quark_fromTop_TLV.SetPxPyPzE(genJet_matchedTo_b_quark_fromTop_vect[0], genJet_matchedTo_b_quark_fromTop_vect[1], genJet_matchedTo_b_quark_fromTop_vect[2], genJet_matchedTo_b_quark_fromTop_vect[3]);
      h_genJet_matchedTo_b_quark_fromTop_pt_vs_eta->Fill( fabs(genJet_matchedTo_b_quark_fromTop_TLV.Eta()), genJet_matchedTo_b_quark_fromTop_TLV.Pt(), wgt);
      if(!eve->genJet_matchedTo_b_quark_fromTop_isSelected_){
	h_genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta->Fill( fabs(genJet_matchedTo_b_quark_fromTop_TLV.Eta()), genJet_matchedTo_b_quark_fromTop_TLV.Pt(), wgt);
      }
    }
    
    vdouble genJet_matchedTo_wDaughter1_fromTop_vect = eve->genJet_matchedTo_wDaughter1_fromTop_TLV_;
    TLorentzVector genJet_matchedTo_wDaughter1_fromTop_TLV = mydummyguy;
    
    vdouble genJet_matchedTo_wDaughter2_fromTop_vect = eve->genJet_matchedTo_wDaughter2_fromTop_TLV_;
    TLorentzVector genJet_matchedTo_wDaughter2_fromTop_TLV = mydummyguy;
    
    if(!eve->genTopIsLeptonic_){
      
      if( genJet_matchedTo_wDaughter1_fromTop_vect[0]!=0 &&
	  genJet_matchedTo_wDaughter1_fromTop_vect[1]!=0 &&
	  genJet_matchedTo_wDaughter1_fromTop_vect[2]!=0 &&
	  genJet_matchedTo_wDaughter1_fromTop_vect[3]!=0    ){
	genJet_matchedTo_wDaughter1_fromTop_TLV.SetPxPyPzE(genJet_matchedTo_wDaughter1_fromTop_vect[0], genJet_matchedTo_wDaughter1_fromTop_vect[1], genJet_matchedTo_wDaughter1_fromTop_vect[2], genJet_matchedTo_wDaughter1_fromTop_vect[3]);
	h_genJet_matchedTo_wDaughter1_fromTop_pt_vs_eta->Fill( fabs(genJet_matchedTo_wDaughter1_fromTop_TLV.Eta()), genJet_matchedTo_wDaughter1_fromTop_TLV.Pt(), wgt);
	if(!eve->genJet_matchedTo_wDaughter1_fromTop_isSelected_){
	  h_genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta->Fill( fabs(genJet_matchedTo_wDaughter1_fromTop_TLV.Eta()), genJet_matchedTo_wDaughter1_fromTop_TLV.Pt(), wgt);
	}
      }

      if( genJet_matchedTo_wDaughter2_fromTop_vect[0]!=0 &&
	  genJet_matchedTo_wDaughter2_fromTop_vect[1]!=0 &&
	  genJet_matchedTo_wDaughter2_fromTop_vect[2]!=0 &&
	  genJet_matchedTo_wDaughter2_fromTop_vect[3]!=0    ){
	genJet_matchedTo_wDaughter2_fromTop_TLV.SetPxPyPzE(genJet_matchedTo_wDaughter2_fromTop_vect[0], genJet_matchedTo_wDaughter2_fromTop_vect[1], genJet_matchedTo_wDaughter2_fromTop_vect[2], genJet_matchedTo_wDaughter2_fromTop_vect[3]);
	h_genJet_matchedTo_wDaughter2_fromTop_pt_vs_eta->Fill( fabs(genJet_matchedTo_wDaughter2_fromTop_TLV.Eta()), genJet_matchedTo_wDaughter2_fromTop_TLV.Pt(), wgt);
	if(!eve->genJet_matchedTo_wDaughter2_fromTop_isSelected_){
	  h_genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta->Fill( fabs(genJet_matchedTo_wDaughter2_fromTop_TLV.Eta()), genJet_matchedTo_wDaughter2_fromTop_TLV.Pt(), wgt);
	}
      }

    }  // end if top is hadronic

    
    vdouble genJet_matchedTo_b_quark_fromTopBar_vect = eve->genJet_matchedTo_b_quark_fromTopBar_TLV_;
    TLorentzVector genJet_matchedTo_b_quark_fromTopBar_TLV = mydummyguy;
    if( genJet_matchedTo_b_quark_fromTopBar_vect[0]!=0 &&
	genJet_matchedTo_b_quark_fromTopBar_vect[1]!=0 &&
	genJet_matchedTo_b_quark_fromTopBar_vect[2]!=0 &&
	genJet_matchedTo_b_quark_fromTopBar_vect[3]!=0    ){
      genJet_matchedTo_b_quark_fromTopBar_TLV.SetPxPyPzE(genJet_matchedTo_b_quark_fromTopBar_vect[0], genJet_matchedTo_b_quark_fromTopBar_vect[1], genJet_matchedTo_b_quark_fromTopBar_vect[2], genJet_matchedTo_b_quark_fromTopBar_vect[3]);
      h_genJet_matchedTo_b_quark_fromTopBar_pt_vs_eta->Fill( fabs(genJet_matchedTo_b_quark_fromTopBar_TLV.Eta()), genJet_matchedTo_b_quark_fromTopBar_TLV.Pt(), wgt);
      if(!eve->genJet_matchedTo_b_quark_fromTopBar_isSelected_){
	h_genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta->Fill( fabs(genJet_matchedTo_b_quark_fromTopBar_TLV.Eta()), genJet_matchedTo_b_quark_fromTopBar_TLV.Pt(), wgt);
      }
    }
    
    vdouble genJet_matchedTo_wDaughter1_fromTopBar_vect = eve->genJet_matchedTo_wDaughter1_fromTopBar_TLV_;
    TLorentzVector genJet_matchedTo_wDaughter1_fromTopBar_TLV = mydummyguy;
    
    vdouble genJet_matchedTo_wDaughter2_fromTopBar_vect = eve->genJet_matchedTo_wDaughter2_fromTopBar_TLV_;
    TLorentzVector genJet_matchedTo_wDaughter2_fromTopBar_TLV = mydummyguy;

    if(!eve->genTopBarIsLeptonic_){

      if( genJet_matchedTo_wDaughter1_fromTopBar_vect[0]!=0 &&
	  genJet_matchedTo_wDaughter1_fromTopBar_vect[1]!=0 &&
	  genJet_matchedTo_wDaughter1_fromTopBar_vect[2]!=0 &&
	  genJet_matchedTo_wDaughter1_fromTopBar_vect[3]!=0    ){
	genJet_matchedTo_wDaughter1_fromTopBar_TLV.SetPxPyPzE(genJet_matchedTo_wDaughter1_fromTopBar_vect[0], genJet_matchedTo_wDaughter1_fromTopBar_vect[1], genJet_matchedTo_wDaughter1_fromTopBar_vect[2], genJet_matchedTo_wDaughter1_fromTopBar_vect[3]);
	h_genJet_matchedTo_wDaughter1_fromTopBar_pt_vs_eta->Fill( fabs(genJet_matchedTo_wDaughter1_fromTopBar_TLV.Eta()), genJet_matchedTo_wDaughter1_fromTopBar_TLV.Pt(), wgt);
	if(!eve->genJet_matchedTo_wDaughter1_fromTopBar_isSelected_){
	  h_genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta->Fill( fabs(genJet_matchedTo_wDaughter1_fromTopBar_TLV.Eta()), genJet_matchedTo_wDaughter1_fromTopBar_TLV.Pt(), wgt);
	}
      }

      if( genJet_matchedTo_wDaughter2_fromTopBar_vect[0]!=0 &&
	  genJet_matchedTo_wDaughter2_fromTopBar_vect[1]!=0 &&
	  genJet_matchedTo_wDaughter2_fromTopBar_vect[2]!=0 &&
	  genJet_matchedTo_wDaughter2_fromTopBar_vect[3]!=0    ){
	genJet_matchedTo_wDaughter2_fromTopBar_TLV.SetPxPyPzE(genJet_matchedTo_wDaughter2_fromTopBar_vect[0], genJet_matchedTo_wDaughter2_fromTopBar_vect[1], genJet_matchedTo_wDaughter2_fromTopBar_vect[2], genJet_matchedTo_wDaughter2_fromTopBar_vect[3]);
	h_genJet_matchedTo_wDaughter2_fromTopBar_pt_vs_eta->Fill( fabs(genJet_matchedTo_wDaughter2_fromTopBar_TLV.Eta()), genJet_matchedTo_wDaughter2_fromTopBar_TLV.Pt(), wgt);
	if(!eve->genJet_matchedTo_wDaughter2_fromTopBar_isSelected_){
	  h_genJet_matchedTo_ttbarSystem_nonSelected_pt_vs_eta->Fill( fabs(genJet_matchedTo_wDaughter2_fromTopBar_TLV.Eta()), genJet_matchedTo_wDaughter2_fromTopBar_TLV.Pt(), wgt);
	}
      }

    }  // end if topBar is hadronic


    
    //
    // GenParticles, ttbar system
    //

    // top_quark
    vvdouble genParticle_top_quark_vect = eve->genParticle_top_quark_TLV_;
    vTLV genParticle_top_quark_TLV;
    int nStates_top_quark = genParticle_top_quark_vect.size();
    for(int i=0; i<nStates_top_quark; i++){
      TLorentzVector iTLV = mydummyguy;
      iTLV.SetPxPyPzE(genParticle_top_quark_vect[i][0], genParticle_top_quark_vect[i][1], genParticle_top_quark_vect[i][2], genParticle_top_quark_vect[i][3]);
      genParticle_top_quark_TLV.push_back(iTLV);
    }
    h_genParticle_top_quark_pt->Fill(genParticle_top_quark_TLV[nStates_top_quark-1].Pt(), wgt);
    h_genParticle_top_quark_eta->Fill(genParticle_top_quark_TLV[nStates_top_quark-1].Eta(), wgt);
    h_genParticle_top_quark_mass->Fill(genParticle_top_quark_TLV[nStates_top_quark-1].M(), wgt);
    h_genParticle_top_quark_dPtRel_vs_dR_final_intial->Fill( genParticle_top_quark_TLV[nStates_top_quark-1].DeltaR(genParticle_top_quark_TLV[0]), fabs(genParticle_top_quark_TLV[nStates_top_quark-1].Pt()-genParticle_top_quark_TLV[0].Pt())/genParticle_top_quark_TLV[0].Pt(), wgt);


    // b_quark_fromTop
    vvdouble genParticle_b_quark_fromTop_vect = eve->genParticle_b_quark_fromTop_TLV_;
    vTLV genParticle_b_quark_fromTop_TLV;
    int nStates_b_quark_fromTop = genParticle_b_quark_fromTop_vect.size();
    for(int i=0; i<nStates_b_quark_fromTop; i++){
      TLorentzVector iTLV = mydummyguy;
      iTLV.SetPxPyPzE(genParticle_b_quark_fromTop_vect[i][0], genParticle_b_quark_fromTop_vect[i][1], genParticle_b_quark_fromTop_vect[i][2], genParticle_b_quark_fromTop_vect[i][3]);
      genParticle_b_quark_fromTop_TLV.push_back(iTLV);
    }
    h_genParticle_b_quark_fromTop_pt->Fill(genParticle_b_quark_fromTop_TLV[nStates_b_quark_fromTop-1].Pt(), wgt);
    h_genParticle_b_quark_fromTop_eta->Fill(genParticle_b_quark_fromTop_TLV[nStates_b_quark_fromTop-1].Eta(), wgt);
    h_genParticle_b_quark_fromTop_mass->Fill(genParticle_b_quark_fromTop_TLV[nStates_b_quark_fromTop-1].M(), wgt);
    h_genParticle_b_quark_fromTop_dPtRel_vs_dR_final_intial->Fill( genParticle_b_quark_fromTop_TLV[nStates_b_quark_fromTop-1].DeltaR(genParticle_b_quark_fromTop_TLV[0]), fabs(genParticle_b_quark_fromTop_TLV[nStates_b_quark_fromTop-1].Pt()-genParticle_b_quark_fromTop_TLV[0].Pt())/genParticle_b_quark_fromTop_TLV[0].Pt(), wgt);

    
    // w_boson_fromTop
    vvdouble genParticle_w_boson_fromTop_vect = eve->genParticle_w_boson_fromTop_TLV_;
    vTLV genParticle_w_boson_fromTop_TLV;
    int nStates_w_boson_fromTop = genParticle_w_boson_fromTop_vect.size();
    for(int i=0; i<nStates_w_boson_fromTop; i++){
      TLorentzVector iTLV = mydummyguy;
      iTLV.SetPxPyPzE(genParticle_w_boson_fromTop_vect[i][0], genParticle_w_boson_fromTop_vect[i][1], genParticle_w_boson_fromTop_vect[i][2], genParticle_w_boson_fromTop_vect[i][3]);
      genParticle_w_boson_fromTop_TLV.push_back(iTLV);
    }
    h_genParticle_w_boson_fromTop_pt->Fill(genParticle_w_boson_fromTop_TLV[nStates_w_boson_fromTop-1].Pt(), wgt);
    h_genParticle_w_boson_fromTop_eta->Fill(genParticle_w_boson_fromTop_TLV[nStates_w_boson_fromTop-1].Eta(), wgt);
    h_genParticle_w_boson_fromTop_mass->Fill(genParticle_w_boson_fromTop_TLV[nStates_w_boson_fromTop-1].M(), wgt);
    

    // wDaughter1_fromTop
    vvdouble genParticle_wDaughter1_fromTop_vect = eve->genParticle_wDaughter1_fromTop_TLV_;
    vTLV genParticle_wDaughter1_fromTop_TLV;
    int nStates_wDaughter1_fromTop = genParticle_wDaughter1_fromTop_vect.size();
    for(int i=0; i<nStates_wDaughter1_fromTop; i++){
      TLorentzVector iTLV = mydummyguy;
      iTLV.SetPxPyPzE(genParticle_wDaughter1_fromTop_vect[i][0], genParticle_wDaughter1_fromTop_vect[i][1], genParticle_wDaughter1_fromTop_vect[i][2], genParticle_wDaughter1_fromTop_vect[i][3]);
      genParticle_wDaughter1_fromTop_TLV.push_back(iTLV);
    }
    h_genParticle_wDaughter1_fromTop_pt->Fill(genParticle_wDaughter1_fromTop_TLV[nStates_wDaughter1_fromTop-1].Pt(), wgt);
    h_genParticle_wDaughter1_fromTop_eta->Fill(genParticle_wDaughter1_fromTop_TLV[nStates_wDaughter1_fromTop-1].Eta(), wgt);
    h_genParticle_wDaughter1_fromTop_motherPdgId_vs_pdgId->Fill(eve->genParticle_wDaughter1_fromTop_pdgId_[0], eve->genParticle_wDaughter1_fromTop_mother_pdgId_[0], wgt);
    

    // wDaughter2_fromTop
    vvdouble genParticle_wDaughter2_fromTop_vect = eve->genParticle_wDaughter2_fromTop_TLV_;
    vTLV genParticle_wDaughter2_fromTop_TLV;
    int nStates_wDaughter2_fromTop = genParticle_wDaughter2_fromTop_vect.size();
    for(int i=0; i<nStates_wDaughter2_fromTop; i++){
      TLorentzVector iTLV = mydummyguy;
      iTLV.SetPxPyPzE(genParticle_wDaughter2_fromTop_vect[i][0], genParticle_wDaughter2_fromTop_vect[i][1], genParticle_wDaughter2_fromTop_vect[i][2], genParticle_wDaughter2_fromTop_vect[i][3]);
      genParticle_wDaughter2_fromTop_TLV.push_back(iTLV);
    }
    h_genParticle_wDaughter2_fromTop_pt->Fill(genParticle_wDaughter2_fromTop_TLV[nStates_wDaughter2_fromTop-1].Pt(), wgt);
    h_genParticle_wDaughter2_fromTop_eta->Fill(genParticle_wDaughter2_fromTop_TLV[nStates_wDaughter2_fromTop-1].Eta(), wgt);
    h_genParticle_wDaughter2_fromTop_motherPdgId_vs_pdgId->Fill(eve->genParticle_wDaughter2_fromTop_pdgId_[0], eve->genParticle_wDaughter2_fromTop_mother_pdgId_[0], wgt);
    
    
    // topBar_quark
    vvdouble genParticle_topBar_quark_vect = eve->genParticle_topBar_quark_TLV_;
    vTLV genParticle_topBar_quark_TLV;
    int nStates_topBar_quark = genParticle_topBar_quark_vect.size();
    for(int i=0; i<nStates_topBar_quark; i++){
      TLorentzVector iTLV = mydummyguy;
      iTLV.SetPxPyPzE(genParticle_topBar_quark_vect[i][0], genParticle_topBar_quark_vect[i][1], genParticle_topBar_quark_vect[i][2], genParticle_topBar_quark_vect[i][3]);
      genParticle_topBar_quark_TLV.push_back(iTLV);
    }
    h_genParticle_topBar_quark_pt->Fill(genParticle_topBar_quark_TLV[nStates_topBar_quark-1].Pt(), wgt);
    h_genParticle_topBar_quark_eta->Fill(genParticle_topBar_quark_TLV[nStates_topBar_quark-1].Eta(), wgt);
    h_genParticle_topBar_quark_mass->Fill(genParticle_topBar_quark_TLV[nStates_topBar_quark-1].M(), wgt);
    h_genParticle_topBar_quark_dPtRel_vs_dR_final_intial->Fill( genParticle_topBar_quark_TLV[nStates_topBar_quark-1].DeltaR(genParticle_topBar_quark_TLV[0]), fabs(genParticle_topBar_quark_TLV[nStates_topBar_quark-1].Pt()-genParticle_topBar_quark_TLV[0].Pt())/genParticle_topBar_quark_TLV[0].Pt(), wgt);


    // b_quark_fromTopBar
    vvdouble genParticle_b_quark_fromTopBar_vect = eve->genParticle_b_quark_fromTopBar_TLV_;
    vTLV genParticle_b_quark_fromTopBar_TLV;
    int nStates_b_quark_fromTopBar = genParticle_b_quark_fromTopBar_vect.size();
    for(int i=0; i<nStates_b_quark_fromTopBar; i++){
      TLorentzVector iTLV = mydummyguy;
      iTLV.SetPxPyPzE(genParticle_b_quark_fromTopBar_vect[i][0], genParticle_b_quark_fromTopBar_vect[i][1], genParticle_b_quark_fromTopBar_vect[i][2], genParticle_b_quark_fromTopBar_vect[i][3]);
      genParticle_b_quark_fromTopBar_TLV.push_back(iTLV);
    }
    h_genParticle_b_quark_fromTopBar_pt->Fill(genParticle_b_quark_fromTopBar_TLV[nStates_b_quark_fromTopBar-1].Pt(), wgt);
    h_genParticle_b_quark_fromTopBar_eta->Fill(genParticle_b_quark_fromTopBar_TLV[nStates_b_quark_fromTopBar-1].Eta(), wgt);
    h_genParticle_b_quark_fromTopBar_mass->Fill(genParticle_b_quark_fromTopBar_TLV[nStates_b_quark_fromTopBar-1].M(), wgt);
    h_genParticle_b_quark_fromTopBar_dPtRel_vs_dR_final_intial->Fill( genParticle_b_quark_fromTopBar_TLV[nStates_b_quark_fromTopBar-1].DeltaR(genParticle_b_quark_fromTopBar_TLV[0]), fabs(genParticle_b_quark_fromTopBar_TLV[nStates_b_quark_fromTopBar-1].Pt()-genParticle_b_quark_fromTopBar_TLV[0].Pt())/genParticle_b_quark_fromTopBar_TLV[0].Pt(), wgt);

    
    // w_boson_fromTopBar
    vvdouble genParticle_w_boson_fromTopBar_vect = eve->genParticle_w_boson_fromTopBar_TLV_;
    vTLV genParticle_w_boson_fromTopBar_TLV;
    int nStates_w_boson_fromTopBar = genParticle_w_boson_fromTopBar_vect.size();
    for(int i=0; i<nStates_w_boson_fromTopBar; i++){
      TLorentzVector iTLV = mydummyguy;
      iTLV.SetPxPyPzE(genParticle_w_boson_fromTopBar_vect[i][0], genParticle_w_boson_fromTopBar_vect[i][1], genParticle_w_boson_fromTopBar_vect[i][2], genParticle_w_boson_fromTopBar_vect[i][3]);
      genParticle_w_boson_fromTopBar_TLV.push_back(iTLV);
    }
    h_genParticle_w_boson_fromTopBar_pt->Fill(genParticle_w_boson_fromTopBar_TLV[nStates_w_boson_fromTopBar-1].Pt(), wgt);
    h_genParticle_w_boson_fromTopBar_eta->Fill(genParticle_w_boson_fromTopBar_TLV[nStates_w_boson_fromTopBar-1].Eta(), wgt);
    h_genParticle_w_boson_fromTopBar_mass->Fill(genParticle_w_boson_fromTopBar_TLV[nStates_w_boson_fromTopBar-1].M(), wgt);
    

    // wDaughter1_fromTopBar
    vvdouble genParticle_wDaughter1_fromTopBar_vect = eve->genParticle_wDaughter1_fromTopBar_TLV_;
    vTLV genParticle_wDaughter1_fromTopBar_TLV;
    int nStates_wDaughter1_fromTopBar = genParticle_wDaughter1_fromTopBar_vect.size();
    for(int i=0; i<nStates_wDaughter1_fromTopBar; i++){
      TLorentzVector iTLV = mydummyguy;
      iTLV.SetPxPyPzE(genParticle_wDaughter1_fromTopBar_vect[i][0], genParticle_wDaughter1_fromTopBar_vect[i][1], genParticle_wDaughter1_fromTopBar_vect[i][2], genParticle_wDaughter1_fromTopBar_vect[i][3]);
      genParticle_wDaughter1_fromTopBar_TLV.push_back(iTLV);
    }
    h_genParticle_wDaughter1_fromTopBar_pt->Fill(genParticle_wDaughter1_fromTopBar_TLV[nStates_wDaughter1_fromTopBar-1].Pt(), wgt);
    h_genParticle_wDaughter1_fromTopBar_eta->Fill(genParticle_wDaughter1_fromTopBar_TLV[nStates_wDaughter1_fromTopBar-1].Eta(), wgt);
    h_genParticle_wDaughter1_fromTopBar_motherPdgId_vs_pdgId->Fill(eve->genParticle_wDaughter1_fromTopBar_pdgId_[0], eve->genParticle_wDaughter1_fromTopBar_mother_pdgId_[0], wgt);
    

    // wDaughter2_fromTopBar
    vvdouble genParticle_wDaughter2_fromTopBar_vect = eve->genParticle_wDaughter2_fromTopBar_TLV_;
    vTLV genParticle_wDaughter2_fromTopBar_TLV;
    int nStates_wDaughter2_fromTopBar = genParticle_wDaughter2_fromTopBar_vect.size();
    for(int i=0; i<nStates_wDaughter2_fromTopBar; i++){
      TLorentzVector iTLV = mydummyguy;
      iTLV.SetPxPyPzE(genParticle_wDaughter2_fromTopBar_vect[i][0], genParticle_wDaughter2_fromTopBar_vect[i][1], genParticle_wDaughter2_fromTopBar_vect[i][2], genParticle_wDaughter2_fromTopBar_vect[i][3]);
      genParticle_wDaughter2_fromTopBar_TLV.push_back(iTLV);
    }
    h_genParticle_wDaughter2_fromTopBar_pt->Fill(genParticle_wDaughter2_fromTopBar_TLV[nStates_wDaughter2_fromTopBar-1].Pt(), wgt);
    h_genParticle_wDaughter2_fromTopBar_eta->Fill(genParticle_wDaughter2_fromTopBar_TLV[nStates_wDaughter2_fromTopBar-1].Eta(), wgt);
    h_genParticle_wDaughter2_fromTopBar_motherPdgId_vs_pdgId->Fill(eve->genParticle_wDaughter2_fromTopBar_pdgId_[0], eve->genParticle_wDaughter2_fromTopBar_mother_pdgId_[0], wgt);
    
    
    //
    // GenParticles, extra b/c quarks
    //
   
    h_genParticle_nExtraCQuarks_vs_nExtraBQuarks->Fill(eve->nExtraBQuarks_, eve->nExtraCQuarks_, wgt);
    h_genParticle_nExtraCJets_vs_nExtraBJets->Fill(eve->nExtraBJets_, eve->nExtraCJets_, wgt);


    vvvdouble genParticle_extraBQuark_vect = eve->genParticle_extraBQuark_TLV_;
    vector< vTLV > genParticle_extraBQuark_TLV;
    double extraBQuark_minPt=999.9;
    double extraBQuark_maxPt=0.0;
    int nExtraBQuarks = genParticle_extraBQuark_vect.size();
    for(int i=0; i<nExtraBQuarks; i++){
      vTLV allStates_thisBQuark_TLV;
      int nStates_thisBQuark = genParticle_extraBQuark_vect[i].size();
      for(int j=0; j<nStates_thisBQuark; j++){
	TLorentzVector jTLV = mydummyguy;
	jTLV.SetPxPyPzE( genParticle_extraBQuark_vect[i][j][0], genParticle_extraBQuark_vect[i][j][1], genParticle_extraBQuark_vect[i][j][2], genParticle_extraBQuark_vect[i][j][3]);
	allStates_thisBQuark_TLV.push_back(jTLV);
      } // end loop over all states of this b-quark

      genParticle_extraBQuark_TLV.push_back(allStates_thisBQuark_TLV);

      h_genParticle_extraBQuarks_pt->Fill(allStates_thisBQuark_TLV[nStates_thisBQuark-1].Pt(), wgt);
      h_genParticle_extraBQuarks_mother_pdgId->Fill(eve->genParticle_extraBQuark_mother_pdgId_[i][0]);
      h_genParticle_extraQuarks_pt->Fill(allStates_thisBQuark_TLV[nStates_thisBQuark-1].Pt(), wgt);
      h_genParticle_extraQuarks_mother_pdgId->Fill(eve->genParticle_extraBQuark_mother_pdgId_[i][0]);
      
      extraBQuark_minPt = std::min( allStates_thisBQuark_TLV[nStates_thisBQuark-1].Pt(), extraBQuark_minPt );
      extraBQuark_maxPt = std::max( allStates_thisBQuark_TLV[nStates_thisBQuark-1].Pt(), extraBQuark_maxPt );

    } // end loop over b-quarks

    if(nExtraBQuarks>0){
      h_genParticle_extraBQuarks_maxPt->Fill( extraBQuark_maxPt, wgt);
      h_genParticle_extraBQuarks_minPt->Fill( extraBQuark_minPt, wgt);
    }

    vvvdouble genParticle_extraCQuark_vect = eve->genParticle_extraCQuark_TLV_;
    vector< vTLV > genParticle_extraCQuark_TLV;
    double extraCQuark_minPt=999.9;
    double extraCQuark_maxPt=0.0;
    int nExtraCQuarks = genParticle_extraCQuark_vect.size();
    for(int i=0; i<nExtraCQuarks; i++){
      vTLV allStates_thisCQuark_TLV;
      int nStates_thisCQuark = genParticle_extraCQuark_vect[i].size();
      for(int j=0; j<nStates_thisCQuark; j++){
	TLorentzVector jTLV = mydummyguy;
	jTLV.SetPxPyPzE( genParticle_extraCQuark_vect[i][j][0], genParticle_extraCQuark_vect[i][j][1], genParticle_extraCQuark_vect[i][j][2], genParticle_extraCQuark_vect[i][j][3]);
	allStates_thisCQuark_TLV.push_back(jTLV);
      } // end loop over all states of this c-quark

      genParticle_extraCQuark_TLV.push_back(allStates_thisCQuark_TLV);

      h_genParticle_extraCQuarks_pt->Fill(allStates_thisCQuark_TLV[nStates_thisCQuark-1].Pt(), wgt);
      h_genParticle_extraCQuarks_mother_pdgId->Fill(eve->genParticle_extraCQuark_mother_pdgId_[i][0]);
      h_genParticle_extraQuarks_pt->Fill(allStates_thisCQuark_TLV[nStates_thisCQuark-1].Pt(), wgt);
      h_genParticle_extraQuarks_mother_pdgId->Fill(eve->genParticle_extraCQuark_mother_pdgId_[i][0]);

      extraCQuark_minPt = std::min( allStates_thisCQuark_TLV[nStates_thisCQuark-1].Pt(), extraCQuark_minPt );
      extraCQuark_maxPt = std::max( allStates_thisCQuark_TLV[nStates_thisCQuark-1].Pt(), extraCQuark_maxPt );

    } // end loop over c-quarks

    if(nExtraCQuarks>0){
      h_genParticle_extraCQuarks_maxPt->Fill( extraCQuark_maxPt, wgt);
      h_genParticle_extraCQuarks_minPt->Fill( extraCQuark_minPt, wgt);
    }

    if(nExtraBQuarks>0 || nExtraCQuarks>0){
      h_genParticle_extraQuarks_maxPt->Fill( std::max( extraBQuark_maxPt, extraCQuark_maxPt), wgt);
      h_genParticle_extraQuarks_minPt->Fill( std::min( extraBQuark_minPt, extraCQuark_minPt), wgt);
    }
    
  
    //
    // GenParticle, tight lepton that passed selection
    //
    vvdouble genParticle_tightLepton_vect = eve->genParticle_tightLepton_TLV_;
    vTLV genParticle_tightLepton_TLV;
    int nStates_tightLepton = genParticle_tightLepton_vect.size();
    for(int i=0; i<nStates_tightLepton; i++){
      TLorentzVector iTLV = mydummyguy;
      iTLV.SetPxPyPzE(genParticle_tightLepton_vect[i][0], genParticle_tightLepton_vect[i][1], genParticle_tightLepton_vect[i][2], genParticle_tightLepton_vect[i][3]);
      genParticle_tightLepton_TLV.push_back(iTLV);
    }
    
    if(fabs(eve->genParticle_tightLepton_pdgId_[0])==11){
      h_genParticle_tightLepton_pt->Fill( genParticle_tightLepton_TLV[0].Pt(), wgt );
      h_genParticle_tightLepton_eta->Fill( genParticle_tightLepton_TLV[0].Eta(), wgt );
    }
    else{
      h_genParticle_tightLepton_pt->Fill( genParticle_tightLepton_TLV[nStates_tightLepton-1].Pt(), wgt );
      h_genParticle_tightLepton_eta->Fill( genParticle_tightLepton_TLV[nStates_tightLepton-1].Eta(), wgt );
    }
    h_genParticle_tightLepton_status->Fill( eve->genParticle_tightLepton_status_[nStates_tightLepton-1], wgt );
    h_genParticle_tightLepton_motherPdgId_vs_pdgId->Fill( eve->genParticle_tightLepton_pdgId_[0], eve->genParticle_tightLepton_mother_pdgId_[0], wgt);
    
  



  } // end loop over events

  std::cout << " Done! " << std::endl;
  
  // Clean Up
  histFile->Write();
  histFile->Close();
  
  
  return;
}
