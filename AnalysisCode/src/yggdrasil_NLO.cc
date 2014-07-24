#include "../interface/yggdrasil_NLO.h"

//*****************************************************************************

yggdrasil_NLO::yggdrasil_NLO(){

}

//*****************************************************************************

yggdrasil_NLO::~yggdrasil_NLO(){

}

//*****************************************************************************

void yggdrasil_NLO::makeTree(int sampleId, int maxNentries, int Njobs, int jobN){


  //////////////////////////////////////////////////////////////////////////
  ///  Load Files
  //////////////////////////////////////////////////////////////////////////
  
  std::cout << "\n\n" << "=================================" << "\n";
  std::cout           << " WELCOME TO YGGDRASIL TREEMAKER! " << "\n\n";
  std::cout           << "=================================" << "\n";
  std::cout << "\n\n   ===> load the root files! "         << "\n\n";

  if(maxNentries==0){
    std::cout << "No Events To Run Over, exiting..." << std::endl;
    std::cout << std::endl << std::endl;
    return;
  }

  bool verbose_ = false;


  // Load files using analysisInfo_CMS_ttH_LJ helper class
  analysisInfo_CMS_ttH_LJ analysisInfo;

  sampleInfo thisSample;
  for(int iSamp=0; iSamp<(int)analysisInfo.sampleList_trees_.size(); iSamp++){
    if( analysisInfo.sampleList_trees_[iSamp].id == sampleId ){
      thisSample = analysisInfo.sampleList_trees_[iSamp];
    }
  }
  
  // Create a std::vector< std::string > of file names for fwlite::ChainEvent
  vstring fileNames = thisSample.skimFiles;
  
  //fileNames.push_back("/eos/uscms/store/user/tth/NLO_MC_20140610/tt0JetFxFx8TeVCTEQ6M_1_1_tr4.root");
  
  //creates a ChainEvent allowing files to be linked
  fwlite::ChainEvent ev(fileNames);   
  cout << "       Number of Files = " << fileNames.size() << endl;



  //////////////////////////////////////////////////////////////////////////
  ///  Tree branches/leaves
  //////////////////////////////////////////////////////////////////////////

  // Open output file
  
  TString treefilename = analysisInfo.path_tree_nlo_;
  treefilename += "yggdrasil_treeMaker_NLO_";
  treefilename += thisSample.name;
  treefilename += "_job";
  treefilename += jobN;
  treefilename += "of";
  treefilename += Njobs;
  treefilename += ".root";

  //TString treefilename = "/uscms/home/jgwood/nobackup/ttH_ttbb_lepJets/2012_analysis/anaTrees_2012_53x_NLOMC_20140701/anaTree_ttjets_NLO_test.root";
  TFile treefile(treefilename,"recreate");
  treefile.cd();

  // Declare TTree and initialize Branches
  BEANeventVars_NLO *eve=0;
  TTree *worldTree = new TTree("worldTree", "worldTree");
  worldTree->Branch("eve.", "BEANeventVars_NLO", &eve, 8000, 1);
  


  //////////////////////////////////////////////////////////////////////////
  /// Initialize Selection and Counting Quantities  
  //////////////////////////////////////////////////////////////////////////

  
  // Event Counters
  int nevents=0;
  double nevents_wgt=0;

  int nevents_pass_jetCut=0;
  double nevents_pass_jetCut_wgt=0;

  int nevents_pass_lepCut=0;
  double nevents_pass_lepCut_wgt=0;

  int nevents_hadronic=0;
  double nevents_hadronic_wgt=0;
  
  int nevents_semiLeptonic=0;
  double nevents_semiLeptonic_wgt=0;

  int nevents_diLeptonic=0;
  double nevents_diLeptonic_wgt=0;
  
  int nevents_lf=0;
  double nevents_lf_wgt=0;

  int nevents_cc=0;
  double nevents_cc_wgt=0;

  int nevents_1b=0;
  double nevents_1b_wgt=0;

  int nevents_2b=0;
  double nevents_2b_wgt=0;

  int nevents_topIsHadronic=0;
  double nevents_topIsHadronic_wgt=0;
  
  int nevents_topBarIsHadronic=0;
  double nevents_topBarIsHadronic_wgt=0;

  int nevents_match_b_quark_fromTop=0;
  double nevents_match_b_quark_fromTop_wgt=0;
  
  int nevents_match_wDaughter1_fromTop=0;
  double nevents_match_wDaughter1_fromTop_wgt=0;

  int nevents_match_wDaughter2_fromTop=0;
  double nevents_match_wDaughter2_fromTop_wgt=0;

  int nevents_match_b_quark_fromTopBar=0;
  double nevents_match_b_quark_fromTopBar_wgt=0;
  
  int nevents_match_wDaughter1_fromTopBar=0;
  double nevents_match_wDaughter1_fromTopBar_wgt=0;

  int nevents_match_wDaughter2_fromTopBar=0;
  double nevents_match_wDaughter2_fromTopBar_wgt=0;


  // Get number of entries in event chain
  int nentries = ev.size();
  std::cout << "\n";
  std::cout << "\t Number of entries     = " << nentries << std::endl;
  std::cout << "\t Max number of entries = " << maxNentries << std::endl;
  std::cout << "\n" << std::endl;


  //////////////////////////////////////////////////////////////////////////
  /// Loop over Events  
  //////////////////////////////////////////////////////////////////////////

  // Decide which events to run over
  int NeventsPerJob = int( double(nentries)/double(Njobs) + 0.000001 ) + 1;
  int firstEvent    = (jobN-1)*NeventsPerJob + 1;
  int lastEvent     = firstEvent + NeventsPerJob;
  if( jobN==Njobs ) lastEvent = -1;
  

  std::cout << "========  Starting Event Loop  ========" << std::endl;
  int cnt = 0;


  // Begin Loop
  try {
    for( ev.toBegin(); !ev.atEnd(); ++ev) {


      // Increase Counter
      cnt++;


      //////////
      ///
      /// Check if in specified event range
      ///
      //////////
 
      if( cnt<firstEvent ) continue;
      if( cnt==lastEvent ) break;

      // Print status
      if( cnt==1 ){
        std::cout << "     Event " << cnt << std::endl;
      }
      if( cnt%10000==0 && cnt!=1 ){
        std::cout << "           " << cnt << "\t"; 
	std::cout << int(double(cnt-firstEvent)/double(NeventsPerJob)*100);
	std::cout << "% done" << std::endl;
      }

      // Break if above event range
      if( cnt==(maxNentries+1) ) break;

      
      /////////
      ///
      /// fwlite object handles
      ///
      ////////

      fwlite::Handle< GenEventInfoProduct > h_genInfo;
      h_genInfo.getByLabel(ev,"generator");
      GenEventInfoProduct const &genInfo = *h_genInfo;

      fwlite::Handle< std::vector<reco::GenJet> > h_genJet;
      h_genJet.getByLabel(ev,"ak5GenJets");
      std::vector<reco::GenJet> const &genJets = *h_genJet;

      fwlite::Handle< std::vector<reco::GenParticle> > h_genParticle;
      h_genParticle.getByLabel(ev,"genParticles");
      std::vector<reco::GenParticle> const &genParticles = *h_genParticle;
      
      
      
      /////////
      ///
      /// Initialize Tree Variables
      ///
      ////////
      
      eve->initialize(); 


      /////////
      ///
      /// Event Information
      ///
      ////////

      eve->weight_ = genInfo.weight();

      // Increment event counter
      nevents++;
      nevents_wgt+=eve->weight_;
      
      
      /////////
      ///
      /// Selection
      ///
      ////////

      // Get collection of genJets that pass selection
      int jet_cut_nJets = 4;
      double jet_cut_pt  = 25.0;
      double jet_cut_eta = 2.4;
      
      std::vector< reco::GenJet > genJets_selected;
      yggdrasil_NLO::getGoodGenJets( genJets,  genJets_selected, jet_cut_pt, jet_cut_eta);
      
      if( (int)genJets_selected.size() < jet_cut_nJets ) continue;
      
      nevents_pass_jetCut++;
      nevents_pass_jetCut_wgt+=eve->weight_;


      // Get genParticle for lepton that passes selection
      int lep_cut_loose_nLeptons = 0;
      double lep_cut_loose_pt  = 10.0;
      double lep_cut_loose_eta = 2.5;
      
      int lep_cut_tight_nLeptons = 1;
      double lep_cut_tight_pt  = 25.0;
      double lep_cut_tight_eta = 2.4;


      std::vector< std::vector< reco::GenParticle > > genParticle_leptons_loose;
      yggdrasil_NLO::getGoodGenLeptons(genParticles,  genParticle_leptons_loose, false, true, lep_cut_tight_pt, lep_cut_tight_eta, lep_cut_loose_pt, lep_cut_loose_eta);

      std::vector< std::vector< reco::GenParticle > > genParticle_leptons_tight;
      yggdrasil_NLO::getGoodGenLeptons(genParticles,  genParticle_leptons_tight, true, false, lep_cut_tight_pt, lep_cut_tight_eta, lep_cut_loose_pt, lep_cut_loose_eta);

     
      if( (int)genParticle_leptons_loose.size() != lep_cut_loose_nLeptons ) continue;
      if( (int)genParticle_leptons_tight.size() != lep_cut_tight_nLeptons ) continue;
      nevents_pass_lepCut++;
      nevents_pass_lepCut_wgt+=eve->weight_;

      
      
  
      /////////
      ///
      /// Get Gen ttbar Sytem
      ///
      /////////

      // Containers for initial, immediate, and final stages of particle's lifetime in ttbar decay sequence
      std::vector< reco::GenParticle > top_quark; 
      std::vector< reco::GenParticle > b_quark_fromTop;
      std::vector< reco::GenParticle > w_boson_fromTop;
      std::vector< reco::GenParticle > wDaughter1_fromTop;
      std::vector< reco::GenParticle > wDaughter2_fromTop;

      std::vector< reco::GenParticle > topBar_quark;
      std::vector< reco::GenParticle > b_quark_fromTopBar;
      std::vector< reco::GenParticle > w_boson_fromTopBar;
      std::vector< reco::GenParticle > wDaughter1_fromTopBar;
      std::vector< reco::GenParticle > wDaughter2_fromTopBar;

      yggdrasil_NLO::getGenTTbarSystem( genParticles, top_quark, b_quark_fromTop, w_boson_fromTop, wDaughter1_fromTop, wDaughter2_fromTop, topBar_quark, b_quark_fromTopBar, w_boson_fromTopBar, wDaughter1_fromTopBar, wDaughter2_fromTopBar);
      

      // Container for entire decay sequence
      std::vector< reco::GenParticle > top_decayChain_finalState;
      std::vector< reco::GenParticle > topBar_decayChain_finalState;
      
      yggdrasil_NLO::getDecayChainFinalState(top_quark.begin(),    genParticles, top_decayChain_finalState); 
      yggdrasil_NLO::getDecayChainFinalState(topBar_quark.begin(), genParticles, topBar_decayChain_finalState);
      
      
      // Determine if this is a hadronic, semiLeptonic, or DiLeptonic event
      int nTTbarLeptons = 0;
      bool topIsLeptonic=false;
      bool topBarIsLeptonic=false;
      bool topIsTau=false;
      bool topBarIsTau=false;
      // Check if top decays leptonically
      if( fabs(wDaughter1_fromTop[0].pdgId())>=11 &&
	  fabs(wDaughter1_fromTop[0].pdgId())<=16    ){
	topIsLeptonic=true;
	nTTbarLeptons++;
	if( fabs(wDaughter1_fromTop[0].pdgId())==15 ||
	    fabs(wDaughter2_fromTop[0].pdgId())==15    ) topIsTau=true;
      }
      else{
	nevents_topIsHadronic++;
	nevents_topIsHadronic_wgt+=eve->weight_;
      }
      // Check if topbar decays leptonically
      if( fabs(wDaughter1_fromTopBar[0].pdgId())>=11 &&
	  fabs(wDaughter1_fromTopBar[0].pdgId())<=16    ){
	topBarIsLeptonic=true;
	nTTbarLeptons++;
	if( fabs(wDaughter1_fromTopBar[0].pdgId())==15 ||
	    fabs(wDaughter2_fromTopBar[0].pdgId())==15    ) topBarIsTau=true;
      }
      else{
	nevents_topBarIsHadronic++;
	nevents_topBarIsHadronic_wgt+=eve->weight_;
      }

      bool isHadronicDecay=false;
      bool isSemiLeptonicDecay=false;
      bool isDiLeptonicDecay=false;
      if(nTTbarLeptons==0){
	isHadronicDecay=true;
	nevents_hadronic++;
	nevents_hadronic_wgt+=eve->weight_;
      }
      if(nTTbarLeptons==1){
	isSemiLeptonicDecay=true;
	nevents_semiLeptonic++;
	nevents_semiLeptonic_wgt+=eve->weight_;
      }
      if(nTTbarLeptons==2){
	isDiLeptonicDecay=true;
	nevents_diLeptonic++;
	nevents_diLeptonic_wgt+=eve->weight_;
      }

      
      // Containers for hadronization products
      std::vector< reco::GenParticle > ttbar_system_hadrons;
      
      std::vector< reco::GenParticle > b_quark_fromTop_hadrons;
      std::vector< reco::GenParticle > wDaughter1_fromTop_hadrons;
      std::vector< reco::GenParticle > wDaughter2_fromTop_hadrons;

      std::vector< reco::GenParticle > b_quark_fromTopBar_hadrons;
      std::vector< reco::GenParticle > wDaughter1_fromTopBar_hadrons;
      std::vector< reco::GenParticle > wDaughter2_fromTopBar_hadrons;

      yggdrasil_NLO::getDecayChainAll(b_quark_fromTop.end()-1, genParticles, b_quark_fromTop_hadrons);
      if(!topIsLeptonic){
	yggdrasil_NLO::getDecayChainAll(wDaughter1_fromTop.end()-1, genParticles, wDaughter1_fromTop_hadrons);
	yggdrasil_NLO::getDecayChainAll(wDaughter2_fromTop.end()-1, genParticles, wDaughter2_fromTop_hadrons);
      }
      
      yggdrasil_NLO::getDecayChainAll(b_quark_fromTopBar.end()-1,    genParticles, b_quark_fromTopBar_hadrons);
      if(!topBarIsLeptonic){
	yggdrasil_NLO::getDecayChainAll(wDaughter1_fromTopBar.end()-1, genParticles, wDaughter1_fromTopBar_hadrons);
	yggdrasil_NLO::getDecayChainAll(wDaughter2_fromTopBar.end()-1, genParticles, wDaughter2_fromTopBar_hadrons);
      }
      
      // the first element of the _hadrons collection is quark intiating hadronization
      ttbar_system_hadrons.push_back(*(b_quark_fromTop_hadrons.begin()));
       if(!topIsLeptonic){
	ttbar_system_hadrons.push_back(*(wDaughter1_fromTop_hadrons.begin()));
	ttbar_system_hadrons.push_back(*(wDaughter2_fromTop_hadrons.begin()));
      }
      ttbar_system_hadrons.push_back(*(b_quark_fromTopBar_hadrons.begin()));
      if(!topBarIsLeptonic){
	ttbar_system_hadrons.push_back(*(wDaughter1_fromTopBar_hadrons.begin()));
	ttbar_system_hadrons.push_back(*(wDaughter2_fromTopBar_hadrons.begin()));
      }

      
      // Containers for any quark intiating hadronization
      std::vector< reco::GenParticle > quark_initiates_hadronization;
      yggdrasil_NLO::getGenQuarksIntitiatingHadronization(genParticles, quark_initiates_hadronization);

      
      
      //
      // Determine if this is a ttbar+lf/cc/1b/2b event
      //


      //
      // Get extra B/C quarks not associated with ttbar system
      //
      std::vector< std::vector< reco::GenParticle > > extraBQuarks;
      std::vector< std::vector< reco::GenParticle > > extraCQuarks;
      
      std::vector< reco::GenParticle > vetoCandidates;
      vetoCandidates.insert(vetoCandidates.end(), b_quark_fromTop.begin(), b_quark_fromTop.end()); 
      vetoCandidates.insert(vetoCandidates.end(), b_quark_fromTopBar.begin(), b_quark_fromTopBar.end());
      vetoCandidates.insert(vetoCandidates.end(), wDaughter1_fromTop.begin(), wDaughter1_fromTop.end());
      vetoCandidates.insert(vetoCandidates.end(), wDaughter2_fromTop.begin(), wDaughter2_fromTop.end());
      vetoCandidates.insert(vetoCandidates.end(), wDaughter1_fromTopBar.begin(), wDaughter1_fromTopBar.end());
      vetoCandidates.insert(vetoCandidates.end(), wDaughter2_fromTopBar.begin(), wDaughter2_fromTopBar.end());
      
      vetoCandidates.insert(vetoCandidates.end(), b_quark_fromTop_hadrons.begin(), b_quark_fromTop_hadrons.end()); 
      vetoCandidates.insert(vetoCandidates.end(), b_quark_fromTopBar_hadrons.begin(), b_quark_fromTopBar_hadrons.end());
      vetoCandidates.insert(vetoCandidates.end(), wDaughter1_fromTop_hadrons.begin(), wDaughter1_fromTop_hadrons.end());
      vetoCandidates.insert(vetoCandidates.end(), wDaughter2_fromTop_hadrons.begin(), wDaughter2_fromTop_hadrons.end());
      vetoCandidates.insert(vetoCandidates.end(), wDaughter1_fromTopBar_hadrons.begin(), wDaughter1_fromTopBar_hadrons.end());
      vetoCandidates.insert(vetoCandidates.end(), wDaughter2_fromTopBar_hadrons.begin(), wDaughter2_fromTopBar_hadrons.end());
      
      yggdrasil_NLO::getGenExtraBCQuarks(genParticles, vetoCandidates, extraBQuarks, extraCQuarks);
      
      
      // Loop over extraQuarks in the event
      std::vector< reco::GenParticle > extraQuark_initiates_hadronization;
      int nExtraBQuarks=0;
      int nExtraCQuarks=0;
      
      for(int iExtraB=0; iExtraB<(int)extraBQuarks.size(); iExtraB++){
	nExtraBQuarks++;
	int nStatesThisExtraB = (int)extraBQuarks[iExtraB].size();
	extraQuark_initiates_hadronization.push_back( extraBQuarks[iExtraB][ nStatesThisExtraB-1 ] );
      }
      
      for(int iExtraC=0; iExtraC<(int)extraCQuarks.size(); iExtraC++){
	nExtraCQuarks++;
	int nStatesThisExtraC = (int)extraCQuarks[iExtraC].size();
	extraQuark_initiates_hadronization.push_back( extraCQuarks[iExtraC][ nStatesThisExtraC-1 ] );
      }

      
      
      // 
      // Associate GenJets to ttbar system genParticles intiating hadronization
      //
      double deltaR_match = 0.5;
	
      std::vector< reco::GenJet > genJet_matchedToTTbarSystem;
      
      reco::GenJet genJet_b_quark_fromTop;
      reco::GenJet genJet_wDaughter1_fromTop;
      reco::GenJet genJet_wDaughter2_fromTop;

      reco::GenJet genJet_b_quark_fromTopBar;
      reco::GenJet genJet_wDaughter1_fromTopBar;
      reco::GenJet genJet_wDaughter2_fromTopBar;
      

      yggdrasil_NLO::getGenJetsMatchedToTTbarSystem( ttbar_system_hadrons, genJets, deltaR_match, topIsLeptonic, *(b_quark_fromTop.end()-1), *(wDaughter1_fromTop.end()-1), *(wDaughter2_fromTop.end()-1), topBarIsLeptonic, *(b_quark_fromTopBar.end()-1), *(wDaughter1_fromTopBar.end()-1), *(wDaughter2_fromTopBar.end()-1), genJet_b_quark_fromTop, genJet_wDaughter1_fromTop, genJet_wDaughter2_fromTop, genJet_b_quark_fromTopBar, genJet_wDaughter1_fromTopBar, genJet_wDaughter2_fromTopBar);
      
      genJet_matchedToTTbarSystem.push_back( genJet_b_quark_fromTop );
      if(!topIsLeptonic){
	genJet_matchedToTTbarSystem.push_back( genJet_wDaughter1_fromTop );
	genJet_matchedToTTbarSystem.push_back( genJet_wDaughter2_fromTop );
      }
      genJet_matchedToTTbarSystem.push_back( genJet_b_quark_fromTopBar );
      if(!topBarIsLeptonic){
	genJet_matchedToTTbarSystem.push_back( genJet_wDaughter1_fromTopBar );
	genJet_matchedToTTbarSystem.push_back( genJet_wDaughter2_fromTopBar );
      }
      
      
      //
      // Associate genJets to extra B/C quarks in event
      //
      deltaR_match = 0.5;
      int nExtraBJets=0;
      int nExtraCJets=0;
      std::vector< reco::GenJet > genJet_matchedToExtraBQuarks;
      std::vector< reco::GenJet > genJet_matchedToExtraCQuarks;
      
      yggdrasil_NLO::getGenJetsMatchedToExtraQuarks(extraQuark_initiates_hadronization, genJets, genJet_matchedToTTbarSystem, deltaR_match, genJet_matchedToExtraBQuarks, genJet_matchedToExtraCQuarks);
      
      // Count number of selected genJets matched to extra b/c quarks
      for(int iExtraB=0; iExtraB<(int)genJet_matchedToExtraBQuarks.size(); iExtraB++){
	if( yggdrasil_NLO::isGoodGenJet(genJet_matchedToExtraBQuarks[iExtraB], jet_cut_pt, jet_cut_eta) ){
	  nExtraBJets++;
	} // end if good genJet
      } // end loop over genJets matched to extraBs
      
      for(int iExtraC=0; iExtraC<(int)genJet_matchedToExtraCQuarks.size(); iExtraC++){
	if( yggdrasil_NLO::isGoodGenJet(genJet_matchedToExtraCQuarks[iExtraC], jet_cut_pt, jet_cut_eta) ){
	  nExtraCJets++;
	} // end if good genJet
      } // end loop over genJets matched to extraCs

      

      //
      // Classify Event
      //
      bool isTTLF=false;
      bool isTTCC=false;
      bool isTT1B=false;
      bool isTT2B=false;

      // If >=3 extra b quarks, tt+2b
      if(nExtraBQuarks>=3){
	isTT2B=true;
	nevents_2b++;
	nevents_2b_wgt+=eve->weight_;
      }
      // If ==2 extra b quarks, matched to 2 jets, tt+2b
      else if(nExtraBQuarks==2 && nExtraBJets==2){
	isTT2B=true;
	nevents_2b++;
	nevents_2b_wgt+=eve->weight_;
      }
      // If 2 extra B quarks, matched to 1 extra jet, tt+1b
      else if(nExtraBQuarks==2 && nExtraBJets==1){
	isTT1B=true;
	nevents_1b++;
	nevents_1b_wgt+=eve->weight_;
      }
      // If 1 extra B quark, not matched to extra jet, tt+1b
      else if(nExtraBQuarks==1){
	isTT1B=true;
	nevents_1b++;
	nevents_1b_wgt+=eve->weight_;
      }
      // If >=1 extra C quarks, 0 extra b quarks, tt+cc (tt+1/2c)
      else if(nExtraBQuarks==0 && nExtraCQuarks>0){
	isTTCC=true;
	nevents_cc++;
	nevents_cc_wgt+=eve->weight_;
      }
      // If 0 extraB, extraC quarks, tt+lf
      else{
	isTTLF=true;
	nevents_lf++;
	nevents_lf_wgt+=eve->weight_;
      }

      
      
      //
      // Print results of grabbing ttbar system
      //
      bool printCondition_ = ( verbose_ && (nevents_pass_lepCut==1) );
      if(printCondition_){
	
	cout << "NEVENT = " << nevents << endl << endl;

	cout << "HERITAGE OF TOP QUARK: " << endl;
	yggdrasil_NLO::printGenCollectionInfo(top_quark);
	cout << "HERITAGE OF B FROM TOP QUARK: " << endl;
	yggdrasil_NLO::printGenCollectionInfo(b_quark_fromTop);
	cout << "HERITAGE OF W FROM TOP QUARK: " << endl;
	yggdrasil_NLO::printGenCollectionInfo(w_boson_fromTop);
	cout << "HERITAGE OF WDAUGHTER1 FROM TOP QUARK: " << endl;
	yggdrasil_NLO::printGenCollectionInfo(wDaughter1_fromTop);
	cout << "HERITAGE OF WDAUGHTER2 FROM TOP QUARK: " << endl;
	yggdrasil_NLO::printGenCollectionInfo(wDaughter2_fromTop);

	cout << "HERITAGE OF TOPBAR QUARK: " << endl;
	yggdrasil_NLO::printGenCollectionInfo(topBar_quark);
	cout << "HERITAGE OF B FROM TOPBAR QUARK: " << endl;
	yggdrasil_NLO::printGenCollectionInfo(b_quark_fromTopBar);
	cout << "HERITAGE OF W FROM TOPBAR QUARK: " << endl;
	yggdrasil_NLO::printGenCollectionInfo(w_boson_fromTopBar);
	cout << "HERITAGE OF WDAUGHTER1 FROM TOPBAR QUARK: " << endl;
	yggdrasil_NLO::printGenCollectionInfo(wDaughter1_fromTopBar);
	cout << "HERITAGE OF WDAUGHTER2 FROM TOPBAR QUARK: " << endl;
	yggdrasil_NLO::printGenCollectionInfo(wDaughter2_fromTopBar);

	if(nExtraBQuarks>=2){
	  for(int iExtraB=0; iExtraB<(int)extraBQuarks.size(); iExtraB++){
	    cout << "HERITAGE OF EXTRA B QUARK, " << iExtraB << ":" << endl;
	    yggdrasil_NLO::printGenCollectionInfo(extraBQuarks[iExtraB]);
	  } // end loop over extra Bs
	} // end if >= 3 extra Bs

	cout << endl << endl;

	//yggdrasil_NLO::printMothers(wDaughter1_fromTop.begin(),genParticles);
	//yggdrasil_NLO::printDaughters(top_quark.begin(),genParticles);

      } // end print conditions
      
      
      
      
      /////////
      ///
      /// Set values of BEANeventVars_NLO object
      ///
      ////////


      //
      // Information about nature of ttbar decay
      //
      eve->genTopIsLeptonic_    = (topIsLeptonic)    ? 1 : 0;
      eve->genTopBarIsLeptonic_ = (topBarIsLeptonic) ? 1 : 0;

      eve->genTopIsTau_    = (topIsTau)    ? 1 : 0;
      eve->genTopBarIsTau_ = (topBarIsTau) ? 1 : 0;

      eve->isHadronicDecay_     = (isHadronicDecay)     ? 1 : 0;
      eve->isSemiLeptonicDecay_ = (isSemiLeptonicDecay) ? 1 : 0;
      eve->isDiLeptonicDecay_   = (isDiLeptonicDecay)   ? 1 : 0;

      eve->nExtraBQuarks_ = nExtraBQuarks;
      eve->nExtraCQuarks_ = nExtraCQuarks;
            
      eve->nExtraBJets_ = nExtraBJets;
      eve->nExtraCJets_ = nExtraCJets;
     
      eve->isTTLF_ = (isTTLF) ? 1 : 0;
      eve->isTTCC_ = (isTTCC) ? 1 : 0;
      eve->isTT1B_ = (isTT1B) ? 1 : 0;
      eve->isTT2B_ = (isTT2B) ? 1 : 0;

      
      //
      // Get Info on ttbar system 
      //
      
      // top_quark
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle=top_quark.begin(); iGenParticle != top_quark.end(); iGenParticle++){
	
	vdouble gen_TLV;
	gen_TLV.push_back(iGenParticle->px());
	gen_TLV.push_back(iGenParticle->py());
	gen_TLV.push_back(iGenParticle->pz());
	gen_TLV.push_back(iGenParticle->energy());
	eve->genParticle_top_quark_TLV_.push_back(gen_TLV);
	
	eve->genParticle_top_quark_status_.push_back(iGenParticle->status());
	eve->genParticle_top_quark_pdgId_.push_back(iGenParticle->pdgId());
	eve->genParticle_top_quark_mother_pdgId_.push_back(iGenParticle->mother(0)->pdgId());

      } // end loop over top_quark
      
      
      // b_quark_fromTop
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle=b_quark_fromTop.begin(); iGenParticle != b_quark_fromTop.end(); iGenParticle++){
	
	vdouble gen_TLV;
	gen_TLV.push_back(iGenParticle->px());
	gen_TLV.push_back(iGenParticle->py());
	gen_TLV.push_back(iGenParticle->pz());
	gen_TLV.push_back(iGenParticle->energy());
	eve->genParticle_b_quark_fromTop_TLV_.push_back(gen_TLV);
	
	eve->genParticle_b_quark_fromTop_status_.push_back(iGenParticle->status());
	eve->genParticle_b_quark_fromTop_pdgId_.push_back(iGenParticle->pdgId());
	eve->genParticle_b_quark_fromTop_mother_pdgId_.push_back(iGenParticle->mother(0)->pdgId());

      } // end loop over b_quark_fromTop

      
      // w_boson_fromTop
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle=w_boson_fromTop.begin(); iGenParticle != w_boson_fromTop.end(); iGenParticle++){
	
	vdouble gen_TLV;
	gen_TLV.push_back(iGenParticle->px());
	gen_TLV.push_back(iGenParticle->py());
	gen_TLV.push_back(iGenParticle->pz());
	gen_TLV.push_back(iGenParticle->energy());
	eve->genParticle_w_boson_fromTop_TLV_.push_back(gen_TLV);
	
	eve->genParticle_w_boson_fromTop_status_.push_back(iGenParticle->status());
	eve->genParticle_w_boson_fromTop_pdgId_.push_back(iGenParticle->pdgId());
	eve->genParticle_w_boson_fromTop_mother_pdgId_.push_back(iGenParticle->mother(0)->pdgId());

      } // end loop over w_boson_fromTop

      
      // wDaughter1_fromTop
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle=wDaughter1_fromTop.begin(); iGenParticle != wDaughter1_fromTop.end(); iGenParticle++){
	
	vdouble gen_TLV;
	gen_TLV.push_back(iGenParticle->px());
	gen_TLV.push_back(iGenParticle->py());
	gen_TLV.push_back(iGenParticle->pz());
	gen_TLV.push_back(iGenParticle->energy());
	eve->genParticle_wDaughter1_fromTop_TLV_.push_back(gen_TLV);
	
	eve->genParticle_wDaughter1_fromTop_status_.push_back(iGenParticle->status());
	eve->genParticle_wDaughter1_fromTop_pdgId_.push_back(iGenParticle->pdgId());
	eve->genParticle_wDaughter1_fromTop_mother_pdgId_.push_back(iGenParticle->mother(0)->pdgId());

      } // end loop over wDaughter1_fromTop
      

      // wDaughter2_fromTop
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle=wDaughter2_fromTop.begin(); iGenParticle != wDaughter2_fromTop.end(); iGenParticle++){
	
	vdouble gen_TLV;
	gen_TLV.push_back(iGenParticle->px());
	gen_TLV.push_back(iGenParticle->py());
	gen_TLV.push_back(iGenParticle->pz());
	gen_TLV.push_back(iGenParticle->energy());
	eve->genParticle_wDaughter2_fromTop_TLV_.push_back(gen_TLV);
	
	eve->genParticle_wDaughter2_fromTop_status_.push_back(iGenParticle->status());
	eve->genParticle_wDaughter2_fromTop_pdgId_.push_back(iGenParticle->pdgId());
	eve->genParticle_wDaughter2_fromTop_mother_pdgId_.push_back(iGenParticle->mother(0)->pdgId());

      } // end loop over wDaughter2_fromTop
      

       // topBar_quark
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle=topBar_quark.begin(); iGenParticle != topBar_quark.end(); iGenParticle++){
	
	vdouble gen_TLV;
	gen_TLV.push_back(iGenParticle->px());
	gen_TLV.push_back(iGenParticle->py());
	gen_TLV.push_back(iGenParticle->pz());
	gen_TLV.push_back(iGenParticle->energy());
	eve->genParticle_topBar_quark_TLV_.push_back(gen_TLV);
	
	eve->genParticle_topBar_quark_status_.push_back(iGenParticle->status());
	eve->genParticle_topBar_quark_pdgId_.push_back(iGenParticle->pdgId());
	eve->genParticle_topBar_quark_mother_pdgId_.push_back(iGenParticle->mother(0)->pdgId());

      } // end loop over topBar_quark
      
      
      // b_quark_fromTopBar
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle=b_quark_fromTopBar.begin(); iGenParticle != b_quark_fromTopBar.end(); iGenParticle++){
	
	vdouble gen_TLV;
	gen_TLV.push_back(iGenParticle->px());
	gen_TLV.push_back(iGenParticle->py());
	gen_TLV.push_back(iGenParticle->pz());
	gen_TLV.push_back(iGenParticle->energy());
	eve->genParticle_b_quark_fromTopBar_TLV_.push_back(gen_TLV);
	
	eve->genParticle_b_quark_fromTopBar_status_.push_back(iGenParticle->status());
	eve->genParticle_b_quark_fromTopBar_pdgId_.push_back(iGenParticle->pdgId());
	eve->genParticle_b_quark_fromTopBar_mother_pdgId_.push_back(iGenParticle->mother(0)->pdgId());

      } // end loop over b_quark_fromTopBar
      

      // w_boson_fromTopBar
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle=w_boson_fromTopBar.begin(); iGenParticle != w_boson_fromTopBar.end(); iGenParticle++){
	
	vdouble gen_TLV;
	gen_TLV.push_back(iGenParticle->px());
	gen_TLV.push_back(iGenParticle->py());
	gen_TLV.push_back(iGenParticle->pz());
	gen_TLV.push_back(iGenParticle->energy());
	eve->genParticle_w_boson_fromTopBar_TLV_.push_back(gen_TLV);
	
	eve->genParticle_w_boson_fromTopBar_status_.push_back(iGenParticle->status());
	eve->genParticle_w_boson_fromTopBar_pdgId_.push_back(iGenParticle->pdgId());
	eve->genParticle_w_boson_fromTopBar_mother_pdgId_.push_back(iGenParticle->mother(0)->pdgId());

      } // end loop over w_boson_fromTopBar

      
      // wDaughter1_fromTopBar
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle=wDaughter1_fromTopBar.begin(); iGenParticle != wDaughter1_fromTopBar.end(); iGenParticle++){
	
	vdouble gen_TLV;
	gen_TLV.push_back(iGenParticle->px());
	gen_TLV.push_back(iGenParticle->py());
	gen_TLV.push_back(iGenParticle->pz());
	gen_TLV.push_back(iGenParticle->energy());
	eve->genParticle_wDaughter1_fromTopBar_TLV_.push_back(gen_TLV);
	
	eve->genParticle_wDaughter1_fromTopBar_status_.push_back(iGenParticle->status());
	eve->genParticle_wDaughter1_fromTopBar_pdgId_.push_back(iGenParticle->pdgId());
	eve->genParticle_wDaughter1_fromTopBar_mother_pdgId_.push_back(iGenParticle->mother(0)->pdgId());

      } // end loop over wDaughter1_fromTopBar
     
      
      // wDaughter2_fromTopBar
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle=wDaughter2_fromTopBar.begin(); iGenParticle != wDaughter2_fromTopBar.end(); iGenParticle++){
	
	vdouble gen_TLV;
	gen_TLV.push_back(iGenParticle->px());
	gen_TLV.push_back(iGenParticle->py());
	gen_TLV.push_back(iGenParticle->pz());
	gen_TLV.push_back(iGenParticle->energy());
	eve->genParticle_wDaughter2_fromTopBar_TLV_.push_back(gen_TLV);
	
	eve->genParticle_wDaughter2_fromTopBar_status_.push_back(iGenParticle->status());
	eve->genParticle_wDaughter2_fromTopBar_pdgId_.push_back(iGenParticle->pdgId());
	eve->genParticle_wDaughter2_fromTopBar_mother_pdgId_.push_back(iGenParticle->mother(0)->pdgId());

      } // end loop over wDaughter2_fromTopBar
     
      
      

      //
      // Get Information on genJets matched to ttbar system genPartons
      //

      // b quark from top
      eve->genJet_matchedTo_b_quark_fromTop_TLV_.push_back(genJet_b_quark_fromTop.px());
      eve->genJet_matchedTo_b_quark_fromTop_TLV_.push_back(genJet_b_quark_fromTop.py());
      eve->genJet_matchedTo_b_quark_fromTop_TLV_.push_back(genJet_b_quark_fromTop.pz());
      eve->genJet_matchedTo_b_quark_fromTop_TLV_.push_back(genJet_b_quark_fromTop.energy());
      if( yggdrasil_NLO::isGoodGenJet(genJet_b_quark_fromTop, jet_cut_pt, jet_cut_eta) ){
	nevents_match_b_quark_fromTop++;
	nevents_match_b_quark_fromTop_wgt+=eve->weight_;
	eve->genJet_matchedTo_b_quark_fromTop_isSelected_ = 1;
      } // end if good genJet
      else{
	eve->genJet_matchedTo_b_quark_fromTop_isSelected_ = 0;
      }
      
      // Check if top is decaying hadronically
      if(!topIsLeptonic){
	
	// wDaughter1 from top
	eve->genJet_matchedTo_wDaughter1_fromTop_TLV_.push_back(genJet_wDaughter1_fromTop.px());
	eve->genJet_matchedTo_wDaughter1_fromTop_TLV_.push_back(genJet_wDaughter1_fromTop.py());
	eve->genJet_matchedTo_wDaughter1_fromTop_TLV_.push_back(genJet_wDaughter1_fromTop.pz());
	eve->genJet_matchedTo_wDaughter1_fromTop_TLV_.push_back(genJet_wDaughter1_fromTop.energy());
	if( yggdrasil_NLO::isGoodGenJet(genJet_wDaughter1_fromTop, jet_cut_pt, jet_cut_eta) ){
	  nevents_match_wDaughter1_fromTop++;
	  nevents_match_wDaughter1_fromTop_wgt+=eve->weight_;
	  eve->genJet_matchedTo_wDaughter1_fromTop_isSelected_ = 1;
	} // end if good genJet
	else{
	  eve->genJet_matchedTo_wDaughter1_fromTop_isSelected_ = 0;
	}
	
	// wDaughter2 from top
	eve->genJet_matchedTo_wDaughter2_fromTop_TLV_.push_back(genJet_wDaughter2_fromTop.px());
	eve->genJet_matchedTo_wDaughter2_fromTop_TLV_.push_back(genJet_wDaughter2_fromTop.py());
	eve->genJet_matchedTo_wDaughter2_fromTop_TLV_.push_back(genJet_wDaughter2_fromTop.pz());
	eve->genJet_matchedTo_wDaughter2_fromTop_TLV_.push_back(genJet_wDaughter2_fromTop.energy());	
	if( yggdrasil_NLO::isGoodGenJet(genJet_wDaughter2_fromTop, jet_cut_pt, jet_cut_eta) ){
	  nevents_match_wDaughter2_fromTop++;
	  nevents_match_wDaughter2_fromTop_wgt+=eve->weight_;
	  eve->genJet_matchedTo_wDaughter2_fromTop_isSelected_ = 1;
	} // end if good genJet
	else{
	  eve->genJet_matchedTo_wDaughter2_fromTop_isSelected_ = 0;
	}
      
      } // end if top decays hadronically
      

      // b quark from topBar
      eve->genJet_matchedTo_b_quark_fromTopBar_TLV_.push_back(genJet_b_quark_fromTopBar.px());
      eve->genJet_matchedTo_b_quark_fromTopBar_TLV_.push_back(genJet_b_quark_fromTopBar.py());
      eve->genJet_matchedTo_b_quark_fromTopBar_TLV_.push_back(genJet_b_quark_fromTopBar.pz());
      eve->genJet_matchedTo_b_quark_fromTopBar_TLV_.push_back(genJet_b_quark_fromTopBar.energy());
      if( yggdrasil_NLO::isGoodGenJet(genJet_b_quark_fromTopBar, jet_cut_pt, jet_cut_eta) ){
	nevents_match_b_quark_fromTopBar++;
	nevents_match_b_quark_fromTopBar_wgt+=eve->weight_;
	eve->genJet_matchedTo_b_quark_fromTopBar_isSelected_ = 1;
      } // end if good genJet
      else{
	eve->genJet_matchedTo_b_quark_fromTopBar_isSelected_ = 0;
      }
      
      // Check if topBar is decaying hadronically
      if(!topBarIsLeptonic){
	
	// wDaughter1 from topBar
	eve->genJet_matchedTo_wDaughter1_fromTopBar_TLV_.push_back(genJet_wDaughter1_fromTopBar.px());
	eve->genJet_matchedTo_wDaughter1_fromTopBar_TLV_.push_back(genJet_wDaughter1_fromTopBar.py());
	eve->genJet_matchedTo_wDaughter1_fromTopBar_TLV_.push_back(genJet_wDaughter1_fromTopBar.pz());
	eve->genJet_matchedTo_wDaughter1_fromTopBar_TLV_.push_back(genJet_wDaughter1_fromTopBar.energy());
	if( yggdrasil_NLO::isGoodGenJet(genJet_wDaughter1_fromTopBar, jet_cut_pt, jet_cut_eta) ){
	  nevents_match_wDaughter1_fromTopBar++;
	  nevents_match_wDaughter1_fromTopBar_wgt+=eve->weight_;
	  eve->genJet_matchedTo_wDaughter1_fromTopBar_isSelected_ = 1;
	} // end if good genJet
	else{
	  eve->genJet_matchedTo_wDaughter1_fromTopBar_isSelected_ = 0;
	}
	
	// wDaughter2 from topBar
	eve->genJet_matchedTo_wDaughter2_fromTopBar_TLV_.push_back(genJet_wDaughter2_fromTopBar.px());
	eve->genJet_matchedTo_wDaughter2_fromTopBar_TLV_.push_back(genJet_wDaughter2_fromTopBar.py());
	eve->genJet_matchedTo_wDaughter2_fromTopBar_TLV_.push_back(genJet_wDaughter2_fromTopBar.pz());
	eve->genJet_matchedTo_wDaughter2_fromTopBar_TLV_.push_back(genJet_wDaughter1_fromTopBar.energy());
	if( yggdrasil_NLO::isGoodGenJet(genJet_wDaughter2_fromTopBar, jet_cut_pt, jet_cut_eta) ){
	  nevents_match_wDaughter2_fromTopBar++;
	  nevents_match_wDaughter2_fromTopBar_wgt+=eve->weight_;
	  eve->genJet_matchedTo_wDaughter2_fromTopBar_isSelected_ = 1;
	} // end if good genJet
	else{
	  eve->genJet_matchedTo_wDaughter2_fromTopBar_isSelected_ = 0;
	}

      } // end if topBar is decaying hadronically

      
      
      
      //
      // Get information on extra b/c quarks in the event
      //
      for(int iExtraB=0; iExtraB<(int)extraBQuarks.size(); iExtraB++){
	
	std::vector< reco::GenParticle > allStates_thisB=extraBQuarks[iExtraB];
	vvdouble allStates_thisB_TLV;
	vdouble allStates_thisB_status;
	vdouble allStates_thisB_pdgId;
	vdouble allStates_thisB_mother_pdgId;
	for(std::vector< reco::GenParticle >::const_iterator iGenParticle=allStates_thisB.begin(); iGenParticle != allStates_thisB.end(); iGenParticle++){
	  vdouble gen_TLV;
	  gen_TLV.push_back(iGenParticle->px());
	  gen_TLV.push_back(iGenParticle->py());
	  gen_TLV.push_back(iGenParticle->pz());
	  gen_TLV.push_back(iGenParticle->energy());
	  allStates_thisB_TLV.push_back(gen_TLV);
	  
	  allStates_thisB_status.push_back(iGenParticle->status());
	  allStates_thisB_pdgId.push_back(iGenParticle->pdgId());
	  allStates_thisB_mother_pdgId.push_back(iGenParticle->mother(0)->pdgId());
	  	  
	} // end loop over all states of this b quark

	eve->genParticle_extraBQuark_TLV_.push_back( allStates_thisB_TLV );
	eve->genParticle_extraBQuark_status_.push_back(allStates_thisB_status);
	eve->genParticle_extraBQuark_pdgId_.push_back(allStates_thisB_pdgId);
	eve->genParticle_extraBQuark_mother_pdgId_.push_back(allStates_thisB_mother_pdgId);

      } // end loop over extra b quarks

      
      for(int iExtraC=0; iExtraC<(int)extraCQuarks.size(); iExtraC++){

	std::vector< reco::GenParticle > allStates_thisC=extraCQuarks[iExtraC];
	vvdouble allStates_thisC_TLV;
	vdouble allStates_thisC_status;
	vdouble allStates_thisC_pdgId;
	vdouble allStates_thisC_mother_pdgId;
	for(std::vector< reco::GenParticle >::const_iterator iGenParticle=allStates_thisC.begin(); iGenParticle != allStates_thisC.end(); iGenParticle++){
	  vdouble gen_TLV;
	  gen_TLV.push_back(iGenParticle->px());
	  gen_TLV.push_back(iGenParticle->py());
	  gen_TLV.push_back(iGenParticle->pz());
	  gen_TLV.push_back(iGenParticle->energy());
	  allStates_thisC_TLV.push_back(gen_TLV);
	  
	  allStates_thisC_status.push_back(iGenParticle->status());
	  allStates_thisC_pdgId.push_back(iGenParticle->pdgId());
	  allStates_thisC_mother_pdgId.push_back(iGenParticle->mother(0)->pdgId());
	  	  	  	  
	} // end loop over all states of this c quark

	// For extra C quarks with non-gluon mother, check heritage
	//  NOTE: parents seem to be pdgId==2212 (proton) status==4,
	//  a stage of event gen inside pythia, so not from ttbar system decays
	if(extraCQuarks[iExtraC][0].mother(0)->pdgId()!=21){
	  //cout << "PRINTING NONGLUON MOTHER HERITAGE FOR EXTRA C QUARK:" << endl;
	  //yggdrasil_NLO::printMothers(extraCQuarks[iExtraC].begin(), genParticles);
	} // end if non-gluon mother
	
	eve->genParticle_extraCQuark_TLV_.push_back( allStates_thisC_TLV );
	eve->genParticle_extraCQuark_status_.push_back(allStates_thisC_status);
	eve->genParticle_extraCQuark_pdgId_.push_back(allStates_thisC_pdgId);
	eve->genParticle_extraCQuark_mother_pdgId_.push_back(allStates_thisC_mother_pdgId);
	

      } // end loop over extra c quarks

      
      
      //
      // Get Information on genJets matched to extra b/c quarks
      //
      for(int iExtraB=0; iExtraB<(int)genJet_matchedToExtraBQuarks.size(); iExtraB++){
	vdouble genTLV;
	genTLV.push_back(genJet_matchedToExtraBQuarks[iExtraB].px());
	genTLV.push_back(genJet_matchedToExtraBQuarks[iExtraB].py());
	genTLV.push_back(genJet_matchedToExtraBQuarks[iExtraB].pz());
	genTLV.push_back(genJet_matchedToExtraBQuarks[iExtraB].energy());
	eve->genJet_matchedTo_extraBQuark_TLV_.push_back(genTLV);

	if( yggdrasil_NLO::isGoodGenJet(genJet_matchedToExtraBQuarks[iExtraB], jet_cut_pt, jet_cut_eta) ){
	  eve->genJet_matchedTo_extraBQuark_isSelected_.push_back(1);
	} // end if good genJet
	else{
	  eve->genJet_matchedTo_extraBQuark_isSelected_.push_back(0);
	}

      } // end loop over genJets matched to extraBs

      
      for(int iExtraC=0; iExtraC<(int)genJet_matchedToExtraCQuarks.size(); iExtraC++){
	vdouble genTLV;
	genTLV.push_back(genJet_matchedToExtraCQuarks[iExtraC].px());
	genTLV.push_back(genJet_matchedToExtraCQuarks[iExtraC].py());
	genTLV.push_back(genJet_matchedToExtraCQuarks[iExtraC].pz());
	genTLV.push_back(genJet_matchedToExtraCQuarks[iExtraC].energy());
	eve->genJet_matchedTo_extraCQuark_TLV_.push_back(genTLV);

	if( yggdrasil_NLO::isGoodGenJet(genJet_matchedToExtraCQuarks[iExtraC], jet_cut_pt, jet_cut_eta) ){
	  eve->genJet_matchedTo_extraCQuark_isSelected_.push_back(1);
	} // end if good genJet
	else{
	  eve->genJet_matchedTo_extraCQuark_isSelected_.push_back(0);
	}

      } // end loop over genJets matched to extraCs

      
      
   
      //
      // Get Information on tightLepton that passed selection
      //
      for(int iGenLepton=0; iGenLepton<(int)genParticle_leptons_tight[0].size(); iGenLepton++){
	vdouble tight_lep_tlv;
	tight_lep_tlv.push_back(genParticle_leptons_tight[0][iGenLepton].px());
	tight_lep_tlv.push_back(genParticle_leptons_tight[0][iGenLepton].py());
	tight_lep_tlv.push_back(genParticle_leptons_tight[0][iGenLepton].pz());
	tight_lep_tlv.push_back(genParticle_leptons_tight[0][iGenLepton].energy());
	eve->genParticle_tightLepton_TLV_.push_back(tight_lep_tlv);
     
	eve->genParticle_tightLepton_status_.push_back(genParticle_leptons_tight[0][iGenLepton].status());
	eve->genParticle_tightLepton_pdgId_.push_back(genParticle_leptons_tight[0][iGenLepton].pdgId());
	eve->genParticle_tightLepton_mother_pdgId_.push_back(genParticle_leptons_tight[0][iGenLepton].mother(0)->pdgId());
      }
   
      //
      // Loop over genParticles
      //
      
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){

	// Store GenParicle Information
	vdouble gen_TLV;
	gen_TLV.push_back(iGenParticle->px());
	gen_TLV.push_back(iGenParticle->py());
	gen_TLV.push_back(iGenParticle->pz());
	gen_TLV.push_back(iGenParticle->energy());
	eve->genParticle_TLV_.push_back(gen_TLV);

	eve->genParticle_pdgId_.push_back(iGenParticle->pdgId());

	int nGenParticleMothers = (int)iGenParticle->numberOfMothers();
	if(nGenParticleMothers>0){
	  eve->genParticle_mother_pdgId_.push_back(iGenParticle->mother(0)->pdgId());
	}
	else{
	  eve->genParticle_mother_pdgId_.push_back(-99);
	}

      } // end loop over genParticles
      
      

      //
      // Loop over genJets
      //

      for( std::vector< reco::GenJet >::const_iterator iGenJet = genJets.begin(); iGenJet != genJets.end(); iGenJet++){

	// Store GenJet Information
	vdouble gen_TLV;
	gen_TLV.push_back(iGenJet->px());
	gen_TLV.push_back(iGenJet->py());
	gen_TLV.push_back(iGenJet->pz());
	gen_TLV.push_back(iGenJet->energy());
	eve->genJet_TLV_.push_back(gen_TLV);

	if(yggdrasil_NLO::isGoodGenJet(*iGenJet, jet_cut_pt, jet_cut_eta)){
	  eve->genJet_isSelected_.push_back(1);
	}
	else{
	  eve->genJet_isSelected_.push_back(0);
	}

      } // end loop over genJets
      
      
      

      /////////
      ///
      /// Fill Tree
      ///
      /////////

      worldTree->Fill();

    } // end loop over events

  } // end try



  ////////
  ///
  /// Check exception
  ///
  ///////

  catch(std::exception& e) {
    std::cerr << " ==> caught exception " << e.what() << std::endl;
    //continue;
  }

  
  ////////
  ///
  /// Print Event Report
  ///
  ////////
  
  std::cout << " *********************************************************** " << std::endl;
  std::cout << "   Number of Events Processed  = " << nevents << std::endl;
  std::cout << "   Number of Events Processed (wgt) = " << nevents_wgt << std::endl;
  std::cout << "   Number of Events Pass genJet Cut  = " << nevents_pass_jetCut << std::endl;
  std::cout << "   Number of Events Pass genJet Cut (wgt) = " << nevents_pass_jetCut_wgt << std::endl;
  std::cout << "   Number of Events Pass genLep Cut  = " << nevents_pass_lepCut << std::endl;
  std::cout << "   Number of Events Pass genLep Cut (wgt) = " << nevents_pass_lepCut_wgt << std::endl;

  std::cout << "   Number of Events, Hadronic Decay = " << nevents_hadronic << std::endl;
  std::cout << "   Number of Events, Hadronic Decay (wgt) = " << nevents_hadronic_wgt << std::endl;
  std::cout << "   Number of Events, SemiLeptonic Decay  = " << nevents_semiLeptonic << std::endl;
  std::cout << "   Number of Events, SemiLeptonic Decay (wgt) = " << nevents_semiLeptonic_wgt << std::endl;
  std::cout << "   Number of Events, DiLeptonic Decay  = " << nevents_diLeptonic << std::endl;
  std::cout << "   Number of Events, DiLeptonic Decay (wgt) = " << nevents_diLeptonic_wgt << std::endl;
  std::cout << "   Number of Events, tt+lf = " << nevents_lf << std::endl;
  std::cout << "   Number of Events, tt+lf (wgt) = " << nevents_lf_wgt << std::endl;
  std::cout << "   Number of Events, tt+cc = " << nevents_cc << std::endl;
  std::cout << "   Number of Events, tt+cc (wgt) = " << nevents_cc_wgt << std::endl;
  std::cout << "   Number of Events, tt+1b = " << nevents_1b << std::endl;
  std::cout << "   Number of Events, tt+1b (wgt) = " << nevents_1b_wgt << std::endl;
  std::cout << "   Number of Events, tt+2b = " << nevents_2b << std::endl;
  std::cout << "   Number of Events, tt+2b (wgt) = " << nevents_2b_wgt << std::endl;
  
  std::cout << "   Matching efficiency (genParticle, genJet) b quark from Top = " << (double)nevents_match_b_quark_fromTop/(double)nevents_pass_lepCut << " (" << nevents_match_b_quark_fromTop << "/" << nevents_pass_lepCut << ")" << endl;
  std::cout << "   Matching efficiency (genParticle, genJet) wDaughter1 from Top = " << (double)nevents_match_wDaughter1_fromTop/(double)nevents_topIsHadronic << " (" << nevents_match_wDaughter1_fromTop << "/" << nevents_topIsHadronic << ")" << endl;
  std::cout << "   Matching efficiency (genParticle, genJet) wDaughter2 from Top = " << (double)nevents_match_wDaughter2_fromTop/(double)nevents_topIsHadronic << " (" << nevents_match_wDaughter2_fromTop << "/" << nevents_topIsHadronic << ")" << endl;
  std::cout << "   Matching efficiency (genParticle, genJet) b quark from TopBar = " << (double)nevents_match_b_quark_fromTopBar/(double)nevents_pass_lepCut << " (" << nevents_match_b_quark_fromTopBar << "/" << nevents_pass_lepCut << ")" << endl;
  std::cout << "   Matching efficiency (genParticle, genJet) wDaughter1 from TopBar = " << (double)nevents_match_wDaughter1_fromTopBar/(double)nevents_topBarIsHadronic << " (" << nevents_match_wDaughter1_fromTopBar << "/" << nevents_topBarIsHadronic << ")" << endl;
  std::cout << "   Matching efficiency (genParticle, genJet) wDaughter2 from TopBar = " << (double)nevents_match_wDaughter2_fromTopBar/(double)nevents_topBarIsHadronic << " (" << nevents_match_wDaughter2_fromTopBar << "/" << nevents_topBarIsHadronic << ")" << endl;
  

  ////////
  ///
  /// Write and Close AnaTree
  ///
  ////////

  treefile.Write();
  treefile.Close();

  // Bye!
  std::cout << " Done! " << std::endl;



  return;
}



//*****************************************************************************

void yggdrasil_NLO::printDaughters(std::vector< reco::GenParticle >::const_iterator parent,  std::vector< reco::GenParticle > genParticles, int nSpaces){

  // Get number of daughters for particle in question
  int nDaughters = parent->numberOfDaughters();

  // Print Information 
  if(nSpaces==0) std::cout << "MOTHER" << std::endl;
  for(int iSpace=0; iSpace<nSpaces; iSpace++) std::cout << " ";
  std::cout << "pdgId=" << parent->pdgId() << ", status=" << parent->status() << ", pT=" << parent->pt() << ", eta=" << parent->eta() << ", phi=" << parent->phi() << ", nDaughters=" << nDaughters;
  for(int iD=0; iD<nDaughters; iD++){
    std::cout << ", Daught:" << iD << ", pdgId=" << parent->daughter(iD)->pdgId() << ", status=" << parent->daughter(iD)->status();
  }
  std::cout << std::endl;
  
  // Increment printing spacing
  nSpaces+=4;

  // For each daughter, find match from genParticle collection, and recursively find their daughters
  for(int iD=0; iD<nDaughters; iD++){
       
    // Print Daughter iD
    for(int iSpace=0; iSpace<nSpaces; iSpace++) std::cout << " ";
    cout << "Daughter " << iD << endl;
    
    // For each daughter, loop over mc particles, to find it
    std::vector< reco::GenParticle >::const_iterator matched = genParticles.begin();
    bool isMatched = false;
    int nMatchedDaughters=0;
    for( std::vector< reco::GenParticle >::const_iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){
      
      if( parent->daughter(iD)->pdgId()  == iGenParticle->pdgId() &&
	  parent->daughter(iD)->pt()     == iGenParticle->pt()    &&
	  parent->daughter(iD)->eta()    == iGenParticle->eta()   &&
	  parent->daughter(iD)->phi()    == iGenParticle->phi()   &&
	  parent->daughter(iD)->status() == iGenParticle->status()   ){

	isMatched = true;
	matched   = iGenParticle;
	nMatchedDaughters  = iGenParticle->numberOfDaughters(); 
	
      } // end if matched
      if(isMatched) break; // break loop over genParticles
      
    } // end loop over genParticles
    
    if(isMatched && nMatchedDaughters>0) yggdrasil_NLO::printDaughters(matched, genParticles, nSpaces);
    
    cout << endl;
  } // end loop over daughters
   
 
  return;
}


//*****************************************************************************


void yggdrasil_NLO::printMothers(std::vector< reco::GenParticle >::const_iterator daughter,  std::vector< reco::GenParticle > genParticles, int nSpaces){

  // Get number of mothers
  int nMothers = daughter->numberOfMothers();

  // Print stuff about thisParticles
  if(nSpaces==0) std::cout << "DAUGHTER" << std::endl;
  for(int iSpace=0; iSpace<nSpaces; iSpace++) std::cout << " ";
  std::cout << "pdgId=" << daughter->pdgId() << ", status=" << daughter->status() << ", pT=" << daughter->pt() << ", eta=" << daughter->eta() << ", phi=" << daughter->phi() << ", nMothers=" << nMothers;
  for(int iM=0; iM<nMothers; iM++){
    std::cout << ", Mom:" << iM << ", pdgId=" << daughter->mother(iM)->pdgId() << ", status=" << daughter->mother(iM)->status();
  }
  std::cout << std::endl;

  // Incrememnt print spacing
  nSpaces+=4;

  // For each mother, find match from genParticle collection, and recursively find their mothers
  for(int iM=0; iM<nMothers; iM++){

    for(int iSpace=0; iSpace<nSpaces; iSpace++) std::cout << " ";
    cout << "Mother " << iM << endl;
    
    // Match mom to particle in genParticles collection
    std::vector< reco::GenParticle >::iterator matched = genParticles.begin();
    bool isMatched = false;
    int nMothers = 0;
    for( std::vector< reco::GenParticle >::iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){

      if( daughter->mother(iM)->pdgId()  == iGenParticle->pdgId() &&
	  daughter->mother(iM)->pt()     == iGenParticle->pt()    &&
	  daughter->mother(iM)->eta()    == iGenParticle->eta()   &&
	  daughter->mother(iM)->phi()    == iGenParticle->phi()   &&
	  daughter->mother(iM)->status() == iGenParticle->status()   ){

	isMatched = true;
	matched   = iGenParticle;
	nMothers  = iGenParticle->numberOfMothers(); 
      }
      
      if(isMatched) break;

    } // end loop over genParticles
    
    if(isMatched && nMothers>0) yggdrasil_NLO::printMothers(matched, genParticles, nSpaces);
    
    cout << endl;
  }
   
 
  return;
}


//*****************************************************************************

void yggdrasil_NLO::printGenInfo(std::vector< reco::GenParticle >::const_iterator thisParticle, int iCount){
  
  // Print Info on this genParticle
  std::cout << "Particle:" << iCount;
  std::cout << ", pdgId=" << thisParticle->pdgId();
  std::cout << ", status=" << thisParticle->status();
  std::cout << ", pT=" << thisParticle->pt();
  std::cout << ", eta=" << thisParticle->eta();
  std::cout << ", phi=" << thisParticle->phi();
  std::cout << std::endl;
  
  // Print Info on mothers
  std::cout << "     nMothers=" << thisParticle->numberOfMothers(); 
  for(int iM=0; iM<(int)thisParticle->numberOfMothers(); iM++){
    std::cout << ", MOM:" << iM;
    std::cout << ", pdgId=" << thisParticle->mother(iM)->pdgId();
    std::cout << ", status=" << thisParticle->mother(iM)->status();
    std::cout << ", pt=" << thisParticle->mother(iM)->pt();
    std::cout << ", eta=" << thisParticle->mother(iM)->eta();
    std::cout << ", phi=" << thisParticle->mother(iM)->phi();
  }
  std::cout << std::endl;

  // Print Info on daughters
  std::cout << "     nDaughters=" << thisParticle->numberOfDaughters();
  for(int iD=0; iD<(int)thisParticle->numberOfDaughters(); iD++){
    std::cout << ", DAUGHT:" << iD;
    std::cout << ", pdgId=" << thisParticle->daughter(iD)->pdgId();
    std::cout << ", status=" << thisParticle->daughter(iD)->status();
    std::cout << ", pt=" << thisParticle->daughter(iD)->pt();
    std::cout << ", eta=" << thisParticle->daughter(iD)->eta();
    std::cout << ", phi=" << thisParticle->daughter(iD)->phi();
  }
  std::cout << std::endl << std::endl;
	
	 
  return;
}



//*****************************************************************************


void yggdrasil_NLO::printGenCollectionInfo(std::vector< reco::GenParticle > genParticles){
  
  int iCount=0;
  for( std::vector< reco::GenParticle >::const_iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){
    
    yggdrasil_NLO::printGenInfo(iGenParticle, iCount);

    iCount++;
  }

  std::cout << std::endl << std::endl << std::endl;

  return;
}


//*****************************************************************************

void yggdrasil_NLO::getDecayChainAll(std::vector< reco::GenParticle >::const_iterator parent, std::vector< reco::GenParticle > genParticles, std::vector< reco::GenParticle > &target){

  // Add this parent to target collection
  target.push_back(*parent);

  // Recurse over daughters
  int nDaughters = (int)parent->numberOfDaughters();
  for(int iD=0; iD<nDaughters; iD++){
    
    // Loop over genParticles, look for daughter
    bool isMatched = false;
    std::vector< reco::GenParticle >::iterator matched = genParticles.begin();
    for( std::vector< reco::GenParticle >::iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){
      
      if( parent->daughter(iD)->pdgId()  == iGenParticle->pdgId() &&
	  parent->daughter(iD)->pt()     == iGenParticle->pt()    &&
	  parent->daughter(iD)->eta()    == iGenParticle->eta()   &&
	  parent->daughter(iD)->phi()    == iGenParticle->phi()   &&
	  parent->daughter(iD)->status() == iGenParticle->status()   ){
	
	 // Check that parent is not own daughter
	if( !(parent->daughter(iD)->pdgId()  == parent->pdgId() &&
	      parent->daughter(iD)->pt()     == parent->pt()    &&
	      parent->daughter(iD)->eta()    == parent->eta()   &&
	      parent->daughter(iD)->phi()    == parent->phi()   &&
	      parent->daughter(iD)->status() == parent->status()   ) ){
	  
	  isMatched = true;
	  matched   = iGenParticle;
	} // end if not own daughter

      } // end if matched
      
      if(isMatched) break; // break loop over genParticles
      
    } // end loop over genParticles
    
    
    // If found daughter recruse function
    if(isMatched) yggdrasil_NLO::getDecayChainAll(matched, genParticles, target);

  } // end loop over genParticles
    
  return;
}

//*****************************************************************************


void yggdrasil_NLO::getDecayChainFinalState(std::vector< reco::GenParticle >::const_iterator parent, std::vector< reco::GenParticle > genParticles, std::vector< reco::GenParticle > &target){

  
  // Get nDaughters
  int nDaughters = parent->numberOfDaughters();
  
  
  // If parent has ==0 daughters, this is a final state particle
  if(nDaughters==0){
    
    target.push_back(*parent); 
  }
  
  // If parent has ==1 daughter, it is not final state, find daughter, recuse this function 
  else if(nDaughters==1){
    
    // Loop over genParticles, look for daughter
    bool isMatched = false;
    std::vector< reco::GenParticle >::iterator matched = genParticles.begin();
    for( std::vector< reco::GenParticle >::iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){
      
      if( parent->daughter(0)->pdgId()  == iGenParticle->pdgId() &&
	  parent->daughter(0)->pt()     == iGenParticle->pt()    &&
	  parent->daughter(0)->eta()    == iGenParticle->eta()   &&
	  parent->daughter(0)->phi()    == iGenParticle->phi()   &&
	  parent->daughter(0)->status() == iGenParticle->status()   ){
	
	isMatched = true;
	matched   = iGenParticle;
      } // end if matched
      
      if(isMatched) break; // break loop over genParticles
      
    } // end loop over genParticles
   
    // Check that parent is not own daughter
    bool isOwnDaughter=false;
    if( parent->daughter(0)->pdgId()  == parent->pdgId() &&
	parent->daughter(0)->pt()     == parent->pt()    &&
	parent->daughter(0)->eta()    == parent->eta()   &&
	parent->daughter(0)->phi()    == parent->phi()   &&
	parent->daughter(0)->status() == parent->status()   ){
      isOwnDaughter=true;
    }

    // If found daughter recurse function
    if(isMatched && !isOwnDaughter) yggdrasil_NLO::getDecayChainFinalState( matched, genParticles, target);
    // Else if not found, this is final state 
    else          target.push_back(*parent); 
    
  }

  // If parent has ==2 daughters, add it to decay chain container, recurse over duahgters
  else if(nDaughters==2){
    
    // Ignore gluon splitting in this 
    if( !(parent->pdgId()==21              &&
	  parent->daughter(0)->pdgId()==21 &&
	  parent->daughter(1)->pdgId()==21    ) ){
      
      target.push_back(*parent);
    }

    // Check that if 2 daughters, they aren't mesons/baryons
    if( fabs(parent->daughter(0)->pdgId())<100 && 
	fabs(parent->daughter(1)->pdgId())<100     ){
      
      // Find each daughter and their daughters
      for(int iD=0; iD<nDaughters; iD++){
	
	// Loop over genParticles, look for daughter
	bool isMatched = false;
	std::vector< reco::GenParticle >::iterator matched = genParticles.begin();
	for( std::vector< reco::GenParticle >::iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){
	  
	  if( parent->daughter(iD)->pdgId()  == iGenParticle->pdgId() &&
	      parent->daughter(iD)->pt()     == iGenParticle->pt()    &&
	      parent->daughter(iD)->eta()    == iGenParticle->eta()   &&
	      parent->daughter(iD)->phi()    == iGenParticle->phi()   &&
	      parent->daughter(iD)->status() == iGenParticle->status()   ){

	    isMatched = true;
	    matched   = iGenParticle;
	  } // end if matched
	  
	  if(isMatched) break; // break loop over genParticles
      
	} // end loop over genParticles
	
	// If found daughter recruse function
	if(isMatched) yggdrasil_NLO::getDecayChainFinalState( matched, genParticles, target);
	// Else if not found, this is final state 
	else          target.push_back(*parent); 
	
      } // end loop over daughters

    } // end if daughters arent meson/baryons   

  }

  
  // If parent has >2 daughters, it is initiating final state jet
  else if(nDaughters>2){
    
    target.push_back(*parent);
  }

  return;
}

//*****************************************************************************


void yggdrasil_NLO::getAllStagesOfGenParticle(std::vector< reco::GenParticle >::const_iterator thisParticle, std::vector< reco::GenParticle > genParticles, std::vector< reco::GenParticle > &target, int nCalls){

  // increment number of times this function's been called
  nCalls++;
  
  // If 1st call, add this particle to the target
  if(nCalls==1) target.push_back(*thisParticle);
  
  // Check if any of mothers have same pdgId
  int nMothers = thisParticle->numberOfMothers();
  for(int iM=0; iM<nMothers; iM++){
    
    // If mom has same pdgId, then find in genParticles collection, recursively call function
    if(thisParticle->pdgId()==thisParticle->mother(iM)->pdgId()){
      
      bool isMatched = false;
      bool isNew     = true;
      std::vector< reco::GenParticle >::const_iterator matched = genParticles.begin();
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){
	
	if( thisParticle->mother(iM)->pdgId()  == iGenParticle->pdgId() &&
	    thisParticle->mother(iM)->pt()     == iGenParticle->pt()    &&
	    thisParticle->mother(iM)->eta()    == iGenParticle->eta()   &&
	    thisParticle->mother(iM)->phi()    == iGenParticle->phi()   &&
	    thisParticle->mother(iM)->status() == iGenParticle->status()   ){
	  
	  isMatched = true;
	  matched   = iGenParticle;
	  
	  // Check if already added to collection
	  for( std::vector< reco::GenParticle >::const_iterator jGenParticle = target.begin(); jGenParticle != target.end(); jGenParticle++){

	    if( jGenParticle->pdgId()  == iGenParticle->pdgId() &&
		jGenParticle->pt()     == iGenParticle->pt()    &&
		jGenParticle->eta()    == iGenParticle->eta()   &&
		jGenParticle->phi()    == iGenParticle->phi()   &&
		jGenParticle->status() == iGenParticle->status()   ){
	      isNew=false;
	    }
	  }

	  //  Add mother to front of target vector
     	  if(isNew) target.insert( target.begin(), *iGenParticle );
	  
	  
	} // end if matched
	
	if(isMatched) break; // break loop over genParticles
	
      } // end loop over genParticles
      
      // If found mother with same pdgId, recruse function
      if(isMatched && isNew) yggdrasil_NLO::getAllStagesOfGenParticle( matched, genParticles, target, nCalls);
           
    } // end if thisParticle pdgId == mom pdgId
  
  } // end loop over mothers

  
  // Check if any daughters have same pdgId
  int nDaughters = thisParticle->numberOfDaughters();
  for(int iD=0; iD<nDaughters; iD++){
    
    // If daughter has same pdgId, then find in genParticles collection, recursively call function
    if(thisParticle->pdgId()==thisParticle->daughter(iD)->pdgId()){
      
      bool isMatched = false;
      bool isNew     = true;
      std::vector< reco::GenParticle >::const_iterator matched = genParticles.begin();
      for( std::vector< reco::GenParticle >::const_iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){
	
	if( thisParticle->daughter(iD)->pdgId()  == iGenParticle->pdgId() &&
	    thisParticle->daughter(iD)->pt()     == iGenParticle->pt()    &&
	    thisParticle->daughter(iD)->eta()    == iGenParticle->eta()   &&
	    thisParticle->daughter(iD)->phi()    == iGenParticle->phi()   &&
	    thisParticle->daughter(iD)->status() == iGenParticle->status()   ){
	  
	  isMatched = true;
	  matched   = iGenParticle;
	  
	  // Check if already added to collection
	  for( std::vector< reco::GenParticle >::const_iterator jGenParticle = target.begin(); jGenParticle != target.end(); jGenParticle++){

	    if( jGenParticle->pdgId()  == iGenParticle->pdgId() &&
		jGenParticle->pt()     == iGenParticle->pt()    &&
		jGenParticle->eta()    == iGenParticle->eta()   &&
		jGenParticle->phi()    == iGenParticle->phi()   &&
		jGenParticle->status() == iGenParticle->status()   ){
	      isNew=false;
	    }
	  }

	  //  Add daughter to back of target vector
	  if(isNew) target.push_back( *iGenParticle );
	  
	} // end if matched
	
	if(isMatched) break; // break loop over genParticles
	
      } // end loop over genParticles
      
      // If found daughter, and grandDaughter has same pdgId, recruse function
      if(isMatched && isNew) yggdrasil_NLO::getAllStagesOfGenParticle( matched, genParticles, target, nCalls);
      
    } // end if thisParticle pdgId == daughter pdgId

  } // end loop over daughters
  

  return;
}


//*****************************************************************************

void yggdrasil_NLO::getGenTTbarSystem(std::vector< reco::GenParticle > genParticles, std::vector< reco::GenParticle > &top_quark, std::vector< reco::GenParticle > &b_quark_fromTop, std::vector< reco::GenParticle > &w_boson_fromTop, std::vector< reco::GenParticle > &wDaughter1_fromTop, std::vector< reco::GenParticle > &wDaughter2_fromTop, std::vector< reco::GenParticle > &topBar_quark, std::vector< reco::GenParticle > &b_quark_fromTopBar, std::vector< reco::GenParticle > &w_boson_fromTopBar, std::vector< reco::GenParticle > &wDaughter1_fromTopBar, std::vector< reco::GenParticle > &wDaughter2_fromTopBar){

  
  //
  // Get ttbar system
  //
  
  int pdgIdFirstWDaughterFromTop    = 0;
  int pdgIdFirstWDaughterFromTopBar = 0;
  
  // Loop over genParticles
  for( std::vector< reco::GenParticle >::const_iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){

    // Get pointer to top quark
    if( iGenParticle->pdgId()==6             && 
	iGenParticle->numberOfDaughters()==2    ){
      if( ( iGenParticle->daughter(0)->pdgId()<=5  && 
	    iGenParticle->daughter(0)->pdgId()>=1  && 
	    iGenParticle->daughter(1)->pdgId()==24    ) || 
	  ( iGenParticle->daughter(1)->pdgId()<=5  && 
	    iGenParticle->daughter(1)->pdgId()>=1  && 
	    iGenParticle->daughter(0)->pdgId()==24)        ){
	yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, top_quark);
      } // end if duaghters are b-qaurk and W
    } // end if iGenParticle is top quark with 2 daughters
	

    // Get pointer to b quark from top
    if( iGenParticle->pdgId()<=5           &&
	iGenParticle->pdgId()>=1           &&
	iGenParticle->numberOfMothers()==1    ){
      if( iGenParticle->mother(0)->pdgId()==6 ){
	yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, b_quark_fromTop);
      } // end if mother is top quark
    } // end if iGenParticle is a quark
      

    // Get pointer to W boson from top
    if( iGenParticle->pdgId()==24          &&
	iGenParticle->numberOfMothers()==1    ){
      if( iGenParticle->mother(0)->pdgId()==6 ){
	yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, w_boson_fromTop);
      } // end if mother is top quark
    } // end if iGenParticle is W boson
	

    // Get pointer to 1st daughter of W daughter from top 
    if( pdgIdFirstWDaughterFromTop==0      &&
	iGenParticle->numberOfMothers()==1   ){
      if( iGenParticle->mother(0)->pdgId()==24            && 
	  iGenParticle->mother(0)->numberOfDaughters()==2    ){
	yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, wDaughter1_fromTop);
	pdgIdFirstWDaughterFromTop = iGenParticle->pdgId();
      } // end if mother is W boson that decays to daughters
    } // end if not found wDaughter1 and this particle has 1 mother


    // Get pointer to 2nd daughter of W daughter from top 
    if( pdgIdFirstWDaughterFromTop!=0                     &&
	iGenParticle->pdgId()!=pdgIdFirstWDaughterFromTop &&
	iGenParticle->numberOfMothers()==1                   ){
      if( iGenParticle->mother(0)->pdgId()==24            && 
	  iGenParticle->mother(0)->numberOfDaughters()==2     ){
	yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, wDaughter2_fromTop);
      } // end if mother is W boson that decays to daughters
    } // end if wDaughter1 is found and this particle has 1 mother 


    // Get pointer to topBar quark
    if( iGenParticle->pdgId()==-6            && 
	iGenParticle->numberOfDaughters()==2    ){
      if( ( iGenParticle->daughter(0)->pdgId()>=-5  && 
	    iGenParticle->daughter(0)->pdgId()<=-1  && 
	    iGenParticle->daughter(1)->pdgId()==-24    ) || 
	  ( iGenParticle->daughter(1)->pdgId()>=-5  && 
	    iGenParticle->daughter(1)->pdgId()<=-1  &&
	    iGenParticle->daughter(0)->pdgId()==-24)        ){
	yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, topBar_quark);
      } // end if duaghters are b-qaurk and W
    } // end if iGenParticle is topBar quark with 2 daughters


    // Get pointer to b quark from topBar
    if( iGenParticle->pdgId()>=-5          &&
	iGenParticle->pdgId()<=-1          &&
	iGenParticle->numberOfMothers()==1    ){
      if( iGenParticle->mother(0)->pdgId()==-6  ){
	yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, b_quark_fromTopBar);
      } // end if mother is topBar
    } // end if iGenParticle is a quark


    // Get pointer to W boson from topBar
    if( iGenParticle->pdgId()==-24         &&
	iGenParticle->numberOfMothers()==1     ){
      if( iGenParticle->mother(0)->pdgId()==-6 ){
	yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, w_boson_fromTopBar);
      } // end if mother is topBar
    } // end if W- boson


    // Get pointer to 1st daughter of W daughter from topBar 
    if( pdgIdFirstWDaughterFromTopBar==0   &&
	iGenParticle->numberOfMothers()==1    ){
      if( iGenParticle->mother(0)->pdgId()==-24           && 
	  iGenParticle->mother(0)->numberOfDaughters()==2     ){
	yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, wDaughter1_fromTopBar);
	pdgIdFirstWDaughterFromTopBar = iGenParticle->pdgId();
      } // end if mother is W- boson that decays to 2 daughters
    } // end if wDaughter1_fromTopBar not found, and iGenParticle has 1 mother


    // Get pointer to 2nd daughter of W daughter from topBar 
    if( pdgIdFirstWDaughterFromTopBar!=0                     &&
	iGenParticle->pdgId()!=pdgIdFirstWDaughterFromTopBar &&
	iGenParticle->numberOfMothers()==1                      ){
      if( iGenParticle->mother(0)->pdgId()==-24           && 
	  iGenParticle->mother(0)->numberOfDaughters()==2     ){
	yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, wDaughter2_fromTopBar);
      } // end if mother is W- boson that decays to 2 daughters
    } // end if wDaughter2_fromTopBar is found, and iGenParticle has 1 mother
 
 
  
  } // end loop over genParticles
      
  
  return;
}


//*****************************************************************************

void yggdrasil_NLO::getGenExtraBCQuarks(std::vector< reco::GenParticle > genParticles, std::vector< reco::GenParticle > &vetoCandidates, std::vector< std::vector< reco::GenParticle > > &extraBQuarks, std::vector< std::vector< reco::GenParticle > > &extraCQuarks){

  // Loop over genParticles, look for b, c quarks, check heritage
  for( std::vector< reco::GenParticle >::const_iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){

    if(fabs(iGenParticle->pdgId())==5 ||
       fabs(iGenParticle->pdgId())==4    ){
      
      // Check if this particle belongs to veto collection
      bool isMatched=false;
      for( std::vector< reco::GenParticle >::const_iterator jGenParticle = vetoCandidates.begin(); jGenParticle != vetoCandidates.end(); jGenParticle++){
	
	if( jGenParticle->pdgId()  == iGenParticle->pdgId() &&
	    jGenParticle->pt()     == iGenParticle->pt()    &&
	    jGenParticle->eta()    == iGenParticle->eta()   &&
	    jGenParticle->phi()    == iGenParticle->phi()   &&
	    jGenParticle->status() == iGenParticle->status()   ){
	    
	  isMatched=true;
	} // end if matched
	
	// if matched, break loop over vetoCandidates
	if(isMatched) break; 

      } // end loop over vetoCandidates


      // If not matched, add to correct collection
      if(!isMatched){
	
	std::vector< reco::GenParticle > allStatesThisParticle;
	yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, allStatesThisParticle);
	
	vetoCandidates.insert(vetoCandidates.end(), allStatesThisParticle.begin(), allStatesThisParticle.end());

	// If particle is a b quark
	if(fabs(iGenParticle->pdgId())==5) extraBQuarks.push_back(allStatesThisParticle);
	  
	// If particle is a charm quark
	if(fabs(iGenParticle->pdgId())==4) extraCQuarks.push_back(allStatesThisParticle);
	  
      } // end if not matched to vetoCandidate	
      
    } // end if iGenParticle is b or c quark
    
  } // end loop over genParticles looking for b or c quark
  
  return;
}


//*****************************************************************************

void yggdrasil_NLO::getGenQuarksIntitiatingHadronization( std::vector< reco::GenParticle > genParticles, std::vector< reco::GenParticle > &target ){

  //
  // Get quarks that intiate hadronizations
  //
  
  for( std::vector< reco::GenParticle >::const_iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){
    
    if( fabs(iGenParticle->pdgId())<=5        &&
	iGenParticle->numberOfDaughters()>=2  && 
	iGenParticle->status()>=71            &&
	iGenParticle->status()<=79               ){
      
      target.push_back(*iGenParticle);
   
    } // end if particle is quark with >=2 daughters, and hadronization status code

  } // end loop over genParticles
  
  return;
}


//*****************************************************************************

bool yggdrasil_NLO::isGoodGenJet(reco::GenJet genJet, double cut_pt, double cut_eta){


  if( genJet.pt()        > cut_pt   &&
      fabs(genJet.eta()) < cut_eta     ){

    return true;

  } // end if pass cuts
  else{

    return false;
  
  }

}

			       
//*****************************************************************************


void yggdrasil_NLO::getGoodGenJets(std::vector< reco::GenJet > genJets,  std::vector< reco::GenJet > &target, double cut_pt, double cut_eta){


  // Loop over genJets and check that they pass quality cuts
  for( std::vector< reco::GenJet >::const_iterator iGenJet = genJets.begin(); iGenJet != genJets.end(); iGenJet++){

    if( yggdrasil_NLO::isGoodGenJet( *iGenJet, cut_pt, cut_eta ) ){

      target.push_back(*iGenJet);
      
    } // end if pass cuts
    
  } // end loop over genJets

  return;
}


//*****************************************************************************


void yggdrasil_NLO::getGoodGenLeptons(std::vector< reco::GenParticle > genParticles,  std::vector< std::vector< reco::GenParticle > > &target, bool getTightLeptons, bool getLooseLeptons, double cut_tight_pt, double cut_tight_eta, double cut_loose_pt, double cut_loose_eta){


  // Loop over genJets and check that they pass quality cuts
  for( std::vector< reco::GenParticle >::const_iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){

    // If electron or muon
    if( fabs(iGenParticle->pdgId()) == 11 || fabs(iGenParticle->pdgId()) == 13 ){

      std::vector< reco::GenParticle > allStagesThisLepton;
      yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, allStagesThisLepton);
      
      std::vector< reco::GenParticle >::iterator checkStage = allStagesThisLepton.end()-1;
      if( fabs(iGenParticle->pdgId()) == 11 ) checkStage = allStagesThisLepton.begin();

      if(getTightLeptons){

	if( checkStage->pt()        > cut_tight_pt   &&
	    fabs(checkStage->eta()) < cut_tight_eta     ){
	  
	  target.push_back(allStagesThisLepton);
	  
	} // end if pass cuts
      
      } // end if getting tight leptons

      if(getLooseLeptons){

	if( checkStage->pt() < cut_tight_pt         &&
	    checkStage->pt() > cut_loose_pt         &&
	    fabs(checkStage->eta()) < cut_loose_eta    ){

	  target.push_back(allStagesThisLepton);

	} // end if pass cuts
 
      } // end if getting loose leptons

    } // end if electron or muon


  } // end loop over genParticles

  return;
}


//*****************************************************************************

void yggdrasil_NLO::getGenJetsMatchedToTTbarSystem(std::vector< reco::GenParticle > ttbar_system_hadrons, std::vector< reco::GenJet > genJets, double deltaR_match, bool topIsLeptonic, reco::GenParticle b_quark_fromTop_hadrons, reco::GenParticle wDaughter1_fromTop_hadrons, reco::GenParticle wDaughter2_fromTop_hadrons, bool topBarIsLeptonic, reco::GenParticle b_quark_fromTopBar_hadrons, reco::GenParticle wDaughter1_fromTopBar_hadrons, reco::GenParticle wDaughter2_fromTopBar_hadrons, reco::GenJet &genJet_b_quark_fromTop, reco::GenJet &genJet_wDaughter1_fromTop, reco::GenJet &genJet_wDaughter2_fromTop, reco::GenJet &genJet_b_quark_fromTopBar, reco::GenJet &genJet_wDaughter1_fromTopBar, reco::GenJet &genJet_wDaughter2_fromTopBar){

  
  // Find minDr pairs between ttbar system daughters and genJets
  std::vector< reco::GenParticle > ttbar_system_hadrons_temp = ttbar_system_hadrons;
  std::vector< reco::GenJet >      genJets_temp              = genJets;
  while( (int)genJets_temp.size()>0 && (int)ttbar_system_hadrons_temp.size()>0){
    
    std::vector< reco::GenParticle >::iterator matched_ttbarSystem = ttbar_system_hadrons_temp.begin();
    std::vector< reco::GenJet >::iterator      matched_genJet      = genJets_temp.begin();

    double deltaR     = 0.0;
    double deltaR_min = 99.9;
    for( std::vector< reco::GenJet >::iterator iGenJet = genJets_temp.begin(); iGenJet != genJets_temp.end(); iGenJet++){
      for( std::vector< reco::GenParticle >::iterator iGenParticle = ttbar_system_hadrons_temp.begin(); iGenParticle != ttbar_system_hadrons_temp.end(); iGenParticle++){
	    
	deltaR = sqrt( pow( iGenJet->eta()-iGenParticle->eta(), 2) + pow( iGenJet->phi()-iGenParticle->phi(), 2) );
	   
	if( deltaR < deltaR_min ){
	      
	  deltaR_min          = deltaR;
	  matched_ttbarSystem = iGenParticle;
	  matched_genJet      = iGenJet;
	      	      
	} // end if this is the minDr pair
	    
      } // end loop over genParticles
	  
    } // end loop over genJets
    

    // If matched is b quark from top
    if( deltaR_min <= deltaR_match                                                 &&
	b_quark_fromTop_hadrons.pdgId()  == matched_ttbarSystem->pdgId()  &&
	b_quark_fromTop_hadrons.status() == matched_ttbarSystem->status() &&
	b_quark_fromTop_hadrons.pt()     == matched_ttbarSystem->pt()     &&
	b_quark_fromTop_hadrons.eta()    == matched_ttbarSystem->eta()    &&
	b_quark_fromTop_hadrons.phi()    == matched_ttbarSystem->phi()       ){
      genJet_b_quark_fromTop = *matched_genJet;
    } // end if b quark from top
    
	
    if(!topIsLeptonic){
      
      // If matched is wDaughter1 from top
      if( deltaR_min <= deltaR_match                                                    &&
	  wDaughter1_fromTop_hadrons.pdgId()  == matched_ttbarSystem->pdgId()  &&
	  wDaughter1_fromTop_hadrons.status() == matched_ttbarSystem->status() &&
	  wDaughter1_fromTop_hadrons.pt()     == matched_ttbarSystem->pt()     &&
	  wDaughter1_fromTop_hadrons.eta()    == matched_ttbarSystem->eta()    &&
	  wDaughter1_fromTop_hadrons.phi()    == matched_ttbarSystem->phi()        ){
	genJet_wDaughter1_fromTop = *matched_genJet;
      } // end if wDaughter1 from top

      // If matched is wDaughter2 from top
      if( deltaR_min <= deltaR_match                                                    &&
	  wDaughter2_fromTop_hadrons.pdgId()  == matched_ttbarSystem->pdgId()  &&
	  wDaughter2_fromTop_hadrons.status() == matched_ttbarSystem->status() &&
	  wDaughter2_fromTop_hadrons.pt()     == matched_ttbarSystem->pt()     &&
	  wDaughter2_fromTop_hadrons.eta()    == matched_ttbarSystem->eta()    &&
	  wDaughter2_fromTop_hadrons.phi()    == matched_ttbarSystem->phi()        ){
	genJet_wDaughter2_fromTop = *matched_genJet;
      } // end if wDaughter2 from top

    } // end if top is hadronic

    
    // If matched is b quark from topbar
    if( deltaR_min <= deltaR_match                                                    &&
	b_quark_fromTopBar_hadrons.pdgId()  == matched_ttbarSystem->pdgId()  &&
	b_quark_fromTopBar_hadrons.status() == matched_ttbarSystem->status() &&
	b_quark_fromTopBar_hadrons.pt()     == matched_ttbarSystem->pt()     &&
	b_quark_fromTopBar_hadrons.eta()    == matched_ttbarSystem->eta()    &&
	b_quark_fromTopBar_hadrons.phi()    == matched_ttbarSystem->phi()        ){
      genJet_b_quark_fromTopBar = *matched_genJet;
    } // end if b quark from topBar
	

    if(!topBarIsLeptonic){
      
      // If matched is wDaughter1 from topbar
      if( deltaR_min <= deltaR_match                                                       &&
	  wDaughter1_fromTopBar_hadrons.pdgId()  == matched_ttbarSystem->pdgId()  &&
	  wDaughter1_fromTopBar_hadrons.status() == matched_ttbarSystem->status() &&
	  wDaughter1_fromTopBar_hadrons.pt()     == matched_ttbarSystem->pt()     &&
	  wDaughter1_fromTopBar_hadrons.eta()    == matched_ttbarSystem->eta()    &&
	  wDaughter1_fromTopBar_hadrons.phi()    == matched_ttbarSystem->phi()        ){
	genJet_wDaughter1_fromTopBar = *matched_genJet;
      } // end if wDaughter1 from topBar
      
      // If matched is wDaughter2 from topbar
      if( deltaR_min <= deltaR_match                                                       &&
	  wDaughter2_fromTopBar_hadrons.pdgId()  == matched_ttbarSystem->pdgId()  &&
	  wDaughter2_fromTopBar_hadrons.status() == matched_ttbarSystem->status() &&
	  wDaughter2_fromTopBar_hadrons.pt()     == matched_ttbarSystem->pt()     &&
	  wDaughter2_fromTopBar_hadrons.eta()    == matched_ttbarSystem->eta()    &&
	  wDaughter2_fromTopBar_hadrons.phi()    == matched_ttbarSystem->phi()        ){
	genJet_wDaughter2_fromTopBar = *matched_genJet;
      } // end if topBar is hadronic
      
    } // end if topBar is hadronic 

    
    // Erase pair from temporary collection
    ttbar_system_hadrons_temp.erase(matched_ttbarSystem);
    genJets_temp.erase(matched_genJet);
      

  } // end while loop

  
  return;

}

//*****************************************************************************

void yggdrasil_NLO::getGenJetsMatchedToExtraQuarks(std::vector< reco::GenParticle > extraQuarks, std::vector< reco::GenJet > genJets, std::vector< reco::GenJet > genJet_matchedToTTbarSystem, double deltaR_match, std::vector< reco::GenJet > &genJet_matchedToExtraBQuarks, std::vector< reco::GenJet > &genJet_matchedToExtraCQuarks){

  std::vector< reco::GenParticle > extraQuarks_temp = extraQuarks;
  std::vector< reco::GenJet > genJets_temp;

  // Only consider jets not matched to ttbar system 
  for( std::vector< reco::GenJet >::const_iterator iGenJet = genJets.begin(); iGenJet != genJets.end(); iGenJet++){
    bool alreadyMatched=false;
    for( std::vector< reco::GenJet >::const_iterator jGenJet = genJet_matchedToTTbarSystem.begin(); jGenJet != genJet_matchedToTTbarSystem.end(); jGenJet++){
      if( iGenJet->pt()  == jGenJet->pt()  &&
	  iGenJet->eta() == jGenJet->eta() &&
	  iGenJet->phi() == jGenJet->phi()    ) alreadyMatched=true;
      if(alreadyMatched) break;
    } // end loop over genJets already matched to ttbar system
	
    if(!alreadyMatched) genJets_temp.push_back(*iGenJet);

  } // end loop over genJets
      
  // minDr match
  while( (int)genJets_temp.size()>0 && (int)extraQuarks_temp.size()>0){
	
    std::vector< reco::GenParticle >::iterator matched_extraQuark = extraQuarks_temp.begin();
    std::vector< reco::GenJet >::iterator      matched_genJet     = genJets_temp.begin();
    
    double deltaR     = 0.0;
    double deltaR_min = 99.9;
    for( std::vector< reco::GenJet >::iterator iGenJet = genJets_temp.begin(); iGenJet != genJets_temp.end(); iGenJet++){
      for( std::vector< reco::GenParticle >::iterator iGenParticle = extraQuarks_temp.begin(); iGenParticle != extraQuarks_temp.end(); iGenParticle++){
	
	deltaR = sqrt( pow( iGenJet->eta()-iGenParticle->eta(), 2) + pow( iGenJet->phi()-iGenParticle->phi(), 2) );
	
	if( deltaR < deltaR_min ){
	  
	  deltaR_min         = deltaR;
	  matched_extraQuark = iGenParticle;
	  matched_genJet     = iGenJet;
	  
	} // end if this is the minDr pair
	
      } // end loop over genParticles
	  
    } // end loop over genJets
      
    if( deltaR_min <= deltaR_match           &&
	fabs(matched_extraQuark->pdgId())==5    ){
      genJet_matchedToExtraBQuarks.push_back(*matched_genJet);
    }

    if( deltaR_min <= deltaR_match           && 
	fabs(matched_extraQuark->pdgId())==4    ){
      genJet_matchedToExtraCQuarks.push_back(*matched_genJet);
    }

    // Erase pair from temporary collection
    extraQuarks_temp.erase(matched_extraQuark);
    genJets_temp.erase(matched_genJet);

  } // end while loop



 return;

}

//*****************************************************************************
