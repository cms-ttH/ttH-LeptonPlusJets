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
      /// Initialize Tree Branches
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


      std::vector< reco::GenParticle > genParticle_leptons_loose;
      yggdrasil_NLO::getGoodGenLeptons(genParticles,  genParticle_leptons_loose, false, true, lep_cut_tight_pt, lep_cut_tight_eta, lep_cut_loose_pt, lep_cut_loose_eta);

      std::vector< reco::GenParticle > genParticle_leptons_tight;
      yggdrasil_NLO::getGoodGenLeptons(genParticles,  genParticle_leptons_tight, true, false, lep_cut_tight_pt, lep_cut_tight_eta, lep_cut_loose_pt, lep_cut_loose_eta);

     
      if( (int)genParticle_leptons_loose.size() != lep_cut_loose_nLeptons ) continue;
      if( (int)genParticle_leptons_tight.size() != lep_cut_tight_nLeptons ) continue;
      nevents_pass_lepCut++;
      nevents_pass_lepCut_wgt+=eve->weight_;

      

      /////////
      ///
      /// Loop over genParticles
      ///
      ////////

      // Container for entire decay sequence
      std::vector< reco::GenParticle > top_decayChain_finalState;
      std::vector< reco::GenParticle > topBar_decayChain_finalState;
      
      // Containers for initial, immediate, and final stages of particle's lifetime in ttbar decay sequence
      int pdgIdFirstWDaughterFromTop    = 0;
      int pdgIdFirstWDaughterFromTopBar = 0;

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

      std::vector< reco::GenParticle > b_quark_fromTop_hadrons;
      std::vector< reco::GenParticle > wDaughter1_fromTop_hadrons;
      std::vector< reco::GenParticle > wDaughter2_fromTop_hadrons;

      std::vector< reco::GenParticle > b_quark_fromTopBar_hadrons;
      std::vector< reco::GenParticle > wDaughter1_fromTopBar_hadrons;
      std::vector< reco::GenParticle > wDaughter2_fromTopBar_hadrons;

      // Container for genParticles initiating hadronization
      std::vector< reco::GenParticle > quark_initiates_hadronization;

      for( std::vector< reco::GenParticle >::const_iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){

	// Store GenParicle Information
	eve->genParticle_pt_.push_back(iGenParticle->pt());
	eve->genParticle_eta_.push_back(iGenParticle->eta());
	eve->genParticle_phi_.push_back(iGenParticle->phi());
	eve->genParticle_energy_.push_back(iGenParticle->energy());
	eve->genParticle_pdgId_.push_back(iGenParticle->pdgId());

	int nGenParticleMothers = (int)iGenParticle->numberOfMothers();
	if(nGenParticleMothers>0){
	  eve->genParticle_mother_pdgId_.push_back(iGenParticle->mother()->pdgId());
	}
	else{
	  eve->genParticle_mother_pdgId_.push_back(-99);
	}

	//
	// Get ttbar system
	//
	
	// Get pointer to top quark
	if( iGenParticle->pdgId()==6             && 
	    iGenParticle->numberOfDaughters()==2    ){
	  if( ( iGenParticle->daughter(0)->pdgId()==5  && 
		iGenParticle->daughter(1)->pdgId()==24    ) || 
	      ( iGenParticle->daughter(1)->pdgId()==5  && 
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
	    // Grab hadron daughters
	    yggdrasil_NLO::getDecayChainAll(b_quark_fromTop.end()-1, genParticles, b_quark_fromTop_hadrons);
	  }
	}
      

	// Get pointer to W boson from top
	if( iGenParticle->pdgId()==24          &&
	  iGenParticle->numberOfMothers()==1    ){
	  if( iGenParticle->mother(0)->pdgId()==6 ){
	   yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, w_boson_fromTop);
	  }
	}
	
	// Get pointer to 1st daughter of W daughter from top 
	if( pdgIdFirstWDaughterFromTop==0      &&
	    iGenParticle->numberOfMothers()==1   ){
	  if( iGenParticle->mother(0)->pdgId()==24            && 
	      iGenParticle->mother(0)->numberOfDaughters()==2    ){
	    yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, wDaughter1_fromTop);
	    pdgIdFirstWDaughterFromTop = iGenParticle->pdgId();
	    // Grab hadron daughters
	    if( fabs(iGenParticle->pdgId())<=5) yggdrasil_NLO::getDecayChainAll(wDaughter1_fromTop.end()-1, genParticles, wDaughter1_fromTop_hadrons);
	  }
	}

	// Get pointer to 2nd daughter of W daughter from top 
	if( pdgIdFirstWDaughterFromTop!=0                     &&
	    iGenParticle->pdgId()!=pdgIdFirstWDaughterFromTop &&
	    iGenParticle->numberOfMothers()==1                   ){
	  if( iGenParticle->mother(0)->pdgId()==24            && 
	      iGenParticle->mother(0)->numberOfDaughters()==2     ){
	    yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, wDaughter2_fromTop);
	    // Grab hadron daughters
	    if( fabs(iGenParticle->pdgId())<=5) yggdrasil_NLO::getDecayChainAll(wDaughter2_fromTop.end()-1, genParticles, wDaughter2_fromTop_hadrons);
	  }
	}


	// Get pointer to topBar quark
	if( iGenParticle->pdgId()==-6            && 
	    iGenParticle->numberOfDaughters()==2    ){
	  if( ( iGenParticle->daughter(0)->pdgId()==-5  && 
		iGenParticle->daughter(1)->pdgId()==-24    ) || 
	      ( iGenParticle->daughter(1)->pdgId()==-5  && 
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
	    // Grab hadron daughters
	    yggdrasil_NLO::getDecayChainAll(b_quark_fromTopBar.end()-1, genParticles, b_quark_fromTopBar_hadrons);
	  }
	}

	// Get pointer to W boson from topBar
	if( iGenParticle->pdgId()==-24         &&
	    iGenParticle->numberOfMothers()==1     ){
	  if( iGenParticle->mother(0)->pdgId()==-6 ){
	   yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, w_boson_fromTopBar);
	  }
	}

	// Get pointer to 1st daughter of W daughter from topBar 
	if( pdgIdFirstWDaughterFromTopBar==0   &&
	    iGenParticle->numberOfMothers()==1    ){
	  if( iGenParticle->mother(0)->pdgId()==-24           && 
	      iGenParticle->mother(0)->numberOfDaughters()==2     ){
	    yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, wDaughter1_fromTopBar);
	    pdgIdFirstWDaughterFromTopBar = iGenParticle->pdgId();
	    // Grab hadron daughters
	    if( fabs(iGenParticle->pdgId())<=5) yggdrasil_NLO::getDecayChainAll(wDaughter1_fromTopBar.end()-1, genParticles, wDaughter1_fromTopBar_hadrons);
	  }
	}

	// Get pointer to 2nd daughter of W daughter from topBar 
	if( pdgIdFirstWDaughterFromTopBar!=0                     &&
	    iGenParticle->pdgId()!=pdgIdFirstWDaughterFromTopBar &&
	    iGenParticle->numberOfMothers()==1                      ){
	  if( iGenParticle->mother(0)->pdgId()==-24           && 
	      iGenParticle->mother(0)->numberOfDaughters()==2     ){
	    yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, wDaughter2_fromTopBar);
	    // Grab hadron daughters
	    if( fabs(iGenParticle->pdgId())<=5) yggdrasil_NLO::getDecayChainAll(wDaughter2_fromTopBar.end()-1, genParticles, wDaughter2_fromTopBar_hadrons);
	  }
	}

	
  
	// top final state decay chain
	if( iGenParticle->pdgId()==6   && 
	    iGenParticle->status()==22    ){
	  yggdrasil_NLO::getDecayChainFinalState(iGenParticle, genParticles, top_decayChain_finalState);
	}
	
	// topBar final state decay chain
	if( iGenParticle->pdgId()==-6  && 
	    iGenParticle->status()==22    ){
	  yggdrasil_NLO::getDecayChainFinalState(iGenParticle, genParticles, topBar_decayChain_finalState);
	}


	//
	// Get quarks that intiate hadronizations
	//
	if( fabs(iGenParticle->pdgId())<=5       &&
	    iGenParticle->numberOfDaughters()>=2    ){
	  bool daughtersAreHadrons=false;
	  for(int iD=0; iD<(int)iGenParticle->numberOfDaughters(); iD++){
	    if( fabs(iGenParticle->daughter(iD)->pdgId())>=100 ){
	      daughtersAreHadrons=true;
	    }
	    if(daughtersAreHadrons) break;
	  } // end loop over daughters
	  if(daughtersAreHadrons){
	    quark_initiates_hadronization.push_back(*iGenParticle);
	  }
	  
	} // end if particle is quark with >=2 daughters


	
	//
	// Print Heritage of this Particle
	//
	if( iGenParticle->pdgId()==6   && 
	    iGenParticle->status()==22 && 
	    nevents==1                    ){
	  //yggdrasil_NLO::printMothers(iGenParticle, genParticles);
	  //yggdrasil_NLO::printDaughters(iGenParticle, genParticles);
	} // end if print heritage

      } // end loop over genParticles
      
      
      //
      // Get differences in beginning and final stage of decay sequence
      //
      
      // top quark
      if( (int)top_quark.size()>1){
	std::vector< reco::GenParticle >::const_iterator top_quark_begin = top_quark.begin();
	std::vector< reco::GenParticle >::const_iterator top_quark_end   = top_quark.end()-1;
	eve->dR_final_intial_gen_top_     = sqrt( pow(top_quark_begin->eta()-top_quark_end->eta(), 2) + pow(top_quark_begin->phi()-top_quark_end->phi(),2) );
	eve->dPtRel_final_intial_gen_top_ = fabs( top_quark_begin->pt() - top_quark_end->pt() )/top_quark_begin->pt();
      }
      
      // topBar quark
      if( (int)topBar_quark.size()>1){
	std::vector< reco::GenParticle >::const_iterator topBar_quark_begin = topBar_quark.begin();
	std::vector< reco::GenParticle >::const_iterator topBar_quark_end   = topBar_quark.end()-1;
	eve->dR_final_intial_gen_topBar_     = sqrt( pow(topBar_quark_begin->eta()-topBar_quark_end->eta(), 2) + pow(topBar_quark_begin->phi()-topBar_quark_end->phi(),2) );
	eve->dPtRel_final_intial_gen_topBar_ = fabs( topBar_quark_begin->pt() - topBar_quark_end->pt() )/topBar_quark_begin->pt();
      }
      
      // b quark from top
      if( (int)b_quark_fromTop.size()>1){
	std::vector< reco::GenParticle >::const_iterator b_quark_fromTop_begin = b_quark_fromTop.begin();
	std::vector< reco::GenParticle >::const_iterator b_quark_fromTop_end   = b_quark_fromTop.end()-1;
	eve->dR_final_intial_gen_b_fromTop_     = sqrt( pow(b_quark_fromTop_begin->eta()-b_quark_fromTop_end->eta(), 2) + pow(b_quark_fromTop_begin->phi()-b_quark_fromTop_end->phi(),2) );
	eve->dPtRel_final_intial_gen_b_fromTop_ = fabs( b_quark_fromTop_begin->pt() - b_quark_fromTop_end->pt() )/b_quark_fromTop_begin->pt();
      }
      
      // b quark from topBar
      if( (int)b_quark_fromTopBar.size()>1){
	std::vector< reco::GenParticle >::const_iterator b_quark_fromTopBar_begin = b_quark_fromTopBar.begin();
	std::vector< reco::GenParticle >::const_iterator b_quark_fromTopBar_end   = b_quark_fromTopBar.end()-1;
	eve->dR_final_intial_gen_b_fromTopBar_     = sqrt( pow(b_quark_fromTopBar_begin->eta()-b_quark_fromTopBar_end->eta(), 2) + pow(b_quark_fromTopBar_begin->phi()-b_quark_fromTopBar_end->phi(),2) );
	eve->dPtRel_final_intial_gen_b_fromTopBar_ = fabs( b_quark_fromTopBar_begin->pt() - b_quark_fromTopBar_end->pt() )/b_quark_fromTopBar_begin->pt();
      }
      
          

      /////////
      //
      // Classify event
      //
      /////////


      // 
      // Determine if this is a hadronic, semiLeptonic, or DiLeptonic event
      //
      int nTTbarLeptons = 0;
      bool topIsLeptonic=false;
      bool topBarIsLeptonic=false;
      if( fabs(wDaughter1_fromTop[0].pdgId())>=11 &&
	  fabs(wDaughter1_fromTop[0].pdgId())<=18    ){
	nTTbarLeptons++;
	topIsLeptonic=true;
      }

      if( fabs(wDaughter1_fromTopBar[0].pdgId())>=11 &&
	  fabs(wDaughter1_fromTopBar[0].pdgId())<=18    ){
	nTTbarLeptons++;
	topBarIsLeptonic=true;
      }

      if(!topIsLeptonic){
	nevents_topIsHadronic++;
	nevents_topIsHadronic_wgt+=eve->weight_;
      }
      if(!topBarIsLeptonic){
	nevents_topBarIsHadronic++;
	nevents_topBarIsHadronic_wgt+=eve->weight_;
      }

      if(nTTbarLeptons==0){
	nevents_hadronic++;
	nevents_hadronic_wgt+=eve->weight_;
      }
      if(nTTbarLeptons==1){
	nevents_semiLeptonic++;
	nevents_semiLeptonic_wgt+=eve->weight_;
      }
      if(nTTbarLeptons==2){
	nevents_diLeptonic++;
	nevents_diLeptonic_wgt+=eve->weight_;
      }


      //
      // Determine if this is a ttbar+lf/cc/1b/2b event
      //


      //
      // Find extra B/C quarks not associated with ttbar system
      //
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


      // Loop over genParticles, look for b, c quarks, check heritage
      std::vector< std::vector< reco::GenParticle > > extraBQuarks;
      std::vector< std::vector< reco::GenParticle > > extraCQuarks;
      std::vector< reco::GenParticle > extraQuark_initiates_hadronization;
      int nExtraBQuarks=0;
      int nExtraCQuarks=0;
      double maxPtExtraBQuarks = 0.0;
      double maxPtExtraCQuarks = 0.0;
      double minPtExtraBQuarks = 999.9;
      double minPtExtraCQuarks = 999.9;

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

	    if(isMatched) break; // if matched, break loop over vetoCandidates
	  } // end loop over vetoCandidates

	  if(!isMatched){

	    std::vector< reco::GenParticle > allStatesThisParticle;
	    yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, allStatesThisParticle);

	    vetoCandidates.insert(vetoCandidates.end(), allStatesThisParticle.begin(), allStatesThisParticle.end());

	    // If particle is a b quark
	    if(fabs(iGenParticle->pdgId())==5){
	      nExtraBQuarks++;
	      extraBQuarks.push_back(allStatesThisParticle);
	      extraQuark_initiates_hadronization.push_back( allStatesThisParticle[(int)allStatesThisParticle.size()-1] );

	      maxPtExtraBQuarks = std::max( (double)allStatesThisParticle[(int)allStatesThisParticle.size()-1].pt(), maxPtExtraBQuarks );
	      minPtExtraBQuarks = std::min( (double)allStatesThisParticle[(int)allStatesThisParticle.size()-1].pt(), minPtExtraBQuarks );
	      eve->genParticle_extraBQuarks_pt_.push_back( allStatesThisParticle[(int)allStatesThisParticle.size()-1].pt() );
	      eve->genParticle_extraBQuarks_mother_pdgId_.push_back(allStatesThisParticle[0].mother(0)->pdgId());

	    } // end if particle is a b-quark

	    // If particle is a charm quark
	    if(fabs(iGenParticle->pdgId())==4){
	      nExtraCQuarks++;
	      extraCQuarks.push_back(allStatesThisParticle);
	      extraQuark_initiates_hadronization.push_back( allStatesThisParticle[(int)allStatesThisParticle.size()-1] );

	      maxPtExtraCQuarks = std::max( (double)allStatesThisParticle[(int)allStatesThisParticle.size()-1].pt(), maxPtExtraCQuarks );
	      minPtExtraCQuarks = std::min( (double)allStatesThisParticle[(int)allStatesThisParticle.size()-1].pt(), minPtExtraCQuarks );
	      eve->genParticle_extraCQuarks_pt_.push_back( allStatesThisParticle[(int)allStatesThisParticle.size()-1].pt() );
	      eve->genParticle_extraCQuarks_mother_pdgId_.push_back(allStatesThisParticle[0].mother(0)->pdgId());

	      // For extra C quarks with non-gluon mother, check heritage
	      //  NOTE: parents seem to be pdgId==2212 (proton) status==4 (a stage of event gen inside pythia), so not from ttbar system decays
	      if(allStatesThisParticle[0].mother(0)->pdgId()!=21){
		//cout << "PRINTING NONGLUON MOTHER HERITAGE FOR EXTRA C QUARK:" << endl;
		//yggdrasil_NLO::printMothers(iGenParticle, genParticles);
	      } // end if non-gluon mother
	      
	    } // end if extra particle is a c quark


	  } // end if not matched to vetoCandidate	

	} // end if iGenParticle is b or c quark
     
      } // end loop over genParticles looking for b or c quark

      

      // 
      // Associate GenJets to ttbar system genParticles intiating hadronization
      //
      double deltaR_match = 0.5;
	
      reco::GenJet genJet_b_quark_fromTop;
      reco::GenJet genJet_wDaughter1_fromTop;
      reco::GenJet genJet_wDaughter2_fromTop;

      reco::GenJet genJet_b_quark_fromTopBar;
      reco::GenJet genJet_wDaughter1_fromTopBar;
      reco::GenJet genJet_wDaughter2_fromTopBar;

      std::vector< reco::GenJet > genJet_matchedToTTbarSystem;
      
      std::vector< reco::GenParticle > ttbar_system_hadrons;
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
	    b_quark_fromTop_hadrons.begin()->pdgId()  == matched_ttbarSystem->pdgId()  &&
	    b_quark_fromTop_hadrons.begin()->status() == matched_ttbarSystem->status() &&
	    b_quark_fromTop_hadrons.begin()->pt()     == matched_ttbarSystem->pt()     &&
	    b_quark_fromTop_hadrons.begin()->eta()    == matched_ttbarSystem->eta()    &&
	    b_quark_fromTop_hadrons.begin()->phi()    == matched_ttbarSystem->phi()       ){
	  genJet_b_quark_fromTop = *matched_genJet;
	  genJet_matchedToTTbarSystem.push_back(*matched_genJet);
	  if( yggdrasil_NLO::isGoodGenJet(*matched_genJet, jet_cut_pt, jet_cut_eta) ){
	    nevents_match_b_quark_fromTop++;
	    nevents_match_b_quark_fromTop_wgt+=eve->weight_;
	  } // end if good genJet
	  else{
	    eve->matched_nonSelected_ttbar_genJet_pt_.push_back(matched_genJet->pt());
	    eve->matched_nonSelected_ttbar_genJet_eta_.push_back(matched_genJet->eta());
	  }
	} // end if b quark from top
	
	if(!topIsLeptonic){
	  // If matched is wDaughter1 from top
	  if( deltaR_min <= deltaR_match                                                    &&
	      wDaughter1_fromTop_hadrons.begin()->pdgId()  == matched_ttbarSystem->pdgId()  &&
	      wDaughter1_fromTop_hadrons.begin()->status() == matched_ttbarSystem->status() &&
	      wDaughter1_fromTop_hadrons.begin()->pt()     == matched_ttbarSystem->pt()     &&
	      wDaughter1_fromTop_hadrons.begin()->eta()    == matched_ttbarSystem->eta()    &&
	      wDaughter1_fromTop_hadrons.begin()->phi()    == matched_ttbarSystem->phi()        ){
	    genJet_wDaughter1_fromTop = *matched_genJet;
	    genJet_matchedToTTbarSystem.push_back(*matched_genJet);
	    if( yggdrasil_NLO::isGoodGenJet(*matched_genJet, jet_cut_pt, jet_cut_eta) ){
	      nevents_match_wDaughter1_fromTop++;
	      nevents_match_wDaughter1_fromTop_wgt+=eve->weight_;
	    } // end if good genJet
	    else{
	      eve->matched_nonSelected_ttbar_genJet_pt_.push_back(matched_genJet->pt());
	      eve->matched_nonSelected_ttbar_genJet_eta_.push_back(matched_genJet->eta());
	    }
	  } // end if wDaughter1 from top

	  // If matched is wDaughter2 from top
	  if( deltaR_min <= deltaR_match                                                    &&
	      wDaughter2_fromTop_hadrons.begin()->pdgId()  == matched_ttbarSystem->pdgId()  &&
	      wDaughter2_fromTop_hadrons.begin()->status() == matched_ttbarSystem->status() &&
	      wDaughter2_fromTop_hadrons.begin()->pt()     == matched_ttbarSystem->pt()     &&
	      wDaughter2_fromTop_hadrons.begin()->eta()    == matched_ttbarSystem->eta()    &&
	      wDaughter2_fromTop_hadrons.begin()->phi()    == matched_ttbarSystem->phi()        ){
	    genJet_wDaughter2_fromTop = *matched_genJet;
	    genJet_matchedToTTbarSystem.push_back(*matched_genJet);
	    if( yggdrasil_NLO::isGoodGenJet(*matched_genJet, jet_cut_pt, jet_cut_eta) ){
	      nevents_match_wDaughter2_fromTop++;
	      nevents_match_wDaughter2_fromTop_wgt+=eve->weight_;
	    } // end if good genJet
	    else{
	      eve->matched_nonSelected_ttbar_genJet_pt_.push_back(matched_genJet->pt());
	      eve->matched_nonSelected_ttbar_genJet_eta_.push_back(matched_genJet->eta());
	    }
	  } // end if wDaughter2 from top
	} // end if top is hadronic
	
	// If matched is b quark from topbar
	if( deltaR_min <= deltaR_match                                                    &&
	    b_quark_fromTopBar_hadrons.begin()->pdgId()  == matched_ttbarSystem->pdgId()  &&
	    b_quark_fromTopBar_hadrons.begin()->status() == matched_ttbarSystem->status() &&
	    b_quark_fromTopBar_hadrons.begin()->pt()     == matched_ttbarSystem->pt()     &&
	    b_quark_fromTopBar_hadrons.begin()->eta()    == matched_ttbarSystem->eta()    &&
	    b_quark_fromTopBar_hadrons.begin()->phi()    == matched_ttbarSystem->phi()        ){
	  genJet_b_quark_fromTopBar = *matched_genJet;
	  genJet_matchedToTTbarSystem.push_back(*matched_genJet);
	  if( yggdrasil_NLO::isGoodGenJet(*matched_genJet, jet_cut_pt, jet_cut_eta) ){
	    nevents_match_b_quark_fromTopBar++;
	    nevents_match_b_quark_fromTopBar_wgt+=eve->weight_;
	  } // end if good genJet
	  else{
	    eve->matched_nonSelected_ttbar_genJet_pt_.push_back(matched_genJet->pt());
	    eve->matched_nonSelected_ttbar_genJet_eta_.push_back(matched_genJet->eta());
	  }
	} // end if b quark from topBar
	
	if(!topBarIsLeptonic){
	  // If matched is wDaughter1 from topbar
	  if( deltaR_min <= deltaR_match                                                       &&
	      wDaughter1_fromTopBar_hadrons.begin()->pdgId()  == matched_ttbarSystem->pdgId()  &&
	      wDaughter1_fromTopBar_hadrons.begin()->status() == matched_ttbarSystem->status() &&
	      wDaughter1_fromTopBar_hadrons.begin()->pt()     == matched_ttbarSystem->pt()     &&
	      wDaughter1_fromTopBar_hadrons.begin()->eta()    == matched_ttbarSystem->eta()    &&
	      wDaughter1_fromTopBar_hadrons.begin()->phi()    == matched_ttbarSystem->phi()        ){
	    genJet_wDaughter1_fromTopBar = *matched_genJet;
	    genJet_matchedToTTbarSystem.push_back(*matched_genJet);
	    if( yggdrasil_NLO::isGoodGenJet(*matched_genJet, jet_cut_pt, jet_cut_eta) ){
	      nevents_match_wDaughter1_fromTopBar++;
	      nevents_match_wDaughter1_fromTopBar_wgt+=eve->weight_;
	    } // end if good genJet
	    else{
	      eve->matched_nonSelected_ttbar_genJet_pt_.push_back(matched_genJet->pt());
	      eve->matched_nonSelected_ttbar_genJet_eta_.push_back(matched_genJet->eta());
	    }
	  } // end if wDaughter1 from topBar
	  
	  // If matched is wDaughter2 from topbar
	  if( deltaR_min <= deltaR_match                                                       &&
	      wDaughter2_fromTopBar_hadrons.begin()->pdgId()  == matched_ttbarSystem->pdgId()  &&
	      wDaughter2_fromTopBar_hadrons.begin()->status() == matched_ttbarSystem->status() &&
	      wDaughter2_fromTopBar_hadrons.begin()->pt()     == matched_ttbarSystem->pt()     &&
	      wDaughter2_fromTopBar_hadrons.begin()->eta()    == matched_ttbarSystem->eta()    &&
	      wDaughter2_fromTopBar_hadrons.begin()->phi()    == matched_ttbarSystem->phi()        ){
	    genJet_matchedToTTbarSystem.push_back(*matched_genJet);
	    genJet_wDaughter2_fromTopBar = *matched_genJet;
	    if( yggdrasil_NLO::isGoodGenJet(*matched_genJet, jet_cut_pt, jet_cut_eta) ){
	      nevents_match_wDaughter2_fromTopBar++;
	      nevents_match_wDaughter2_fromTopBar_wgt+=eve->weight_;
	    } // end if good genJet
	    else{
	      eve->matched_nonSelected_ttbar_genJet_pt_.push_back(matched_genJet->pt());
	      eve->matched_nonSelected_ttbar_genJet_eta_.push_back(matched_genJet->eta());
	    }
	  } // end if wDaughter2 from topBar
	} // end if topBar is hadronic

      
	// Erase pair from temporary collection
	ttbar_system_hadrons_temp.erase(matched_ttbarSystem);
	genJets_temp.erase(matched_genJet);
	
      } // end while loop 
      


      //
      // Associate genJets to extra B/C quarks in event
      //
      int nExtraBJets=0;
      int nExtraCJets=0;
      std::vector< reco::GenJet > genJet_matchedToExtraBQuarks;
      std::vector< reco::GenJet > genJet_matchedToExtraCQuarks;

      std::vector< reco::GenParticle > extraQuarks_temp = extraQuark_initiates_hadronization;
      
      // Only consider jets not matched to ttbar system 
      genJets_temp.clear();
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
	  if( yggdrasil_NLO::isGoodGenJet(*matched_genJet, jet_cut_pt, jet_cut_eta) ){
	    nExtraBJets++;
	    genJet_matchedToExtraBQuarks.push_back(*matched_genJet);
	  }
	}
	if( deltaR_min <= deltaR_match           && 
	    fabs(matched_extraQuark->pdgId())==4    ){
	  if( yggdrasil_NLO::isGoodGenJet(*matched_genJet, jet_cut_pt, jet_cut_eta) ){
	    nExtraCJets++;
	    genJet_matchedToExtraCQuarks.push_back(*matched_genJet);
	  }
	}

	// Erase pair from temporary collection
	extraQuarks_temp.erase(matched_extraQuark);
	genJets_temp.erase(matched_genJet);

      } // end while loop




      // If >=3 extra b quarks, tt+2b
      if(nExtraBQuarks>=3){
	nevents_2b++;
	nevents_2b_wgt+=eve->weight_;
      }
      // If ==2 extra b quarks, matched to 2 jets, tt+2b
      else if(nExtraBQuarks==2 && nExtraBJets==2){
	nevents_2b++;
	nevents_2b_wgt+=eve->weight_;
      }
      // If 2 extra B quarks, matched to 1 extra jet, tt+1b
      else if(nExtraBQuarks==2 && nExtraBJets==1){
	nevents_1b++;
	nevents_1b_wgt+=eve->weight_;
      }
      // If 1 extra B quark, not matched to extra jet, tt+1b
      else if(nExtraBQuarks==1){
	nevents_1b++;
	nevents_1b_wgt+=eve->weight_;
      }
      // If >=1 extra C quarks, 0 extra b quarks, tt+cc (tt+1/2c)
      else if(nExtraBQuarks==0 && nExtraCQuarks>0){
	nevents_cc++;
	nevents_cc_wgt+=eve->weight_;
      }
      // If 0 extraB, extraC quarks, tt+lf
      else{
	nevents_lf++;
	nevents_lf_wgt+=eve->weight_;
      }

      eve->nExtraBQuarks_ = nExtraBQuarks;
      eve->nExtraCQuarks_ = nExtraCQuarks;
            
      eve->maxPt_gen_extraBQuark_ = maxPtExtraBQuarks;
      eve->minPt_gen_extraBQuark_ = minPtExtraBQuarks;
      
      eve->maxPt_gen_extraCQuark_ = maxPtExtraCQuarks;
      eve->minPt_gen_extraCQuark_ = minPtExtraCQuarks;
      
      eve->nExtraBJets_ = nExtraBJets;
      eve->nExtraCJets_ = nExtraCJets;
      
    
      //
      // Print results of grabbing ttbar system
      //
      if(verbose_ && (nevents_pass_lepCut==1 || fabs(b_quark_fromTop[0].pdgId())!=5) ){
	
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

      } // end print conditions
      

      
      /////////
      ///
      /// Loop over genJets
      ///
      ////////

      int counter_genJet=0;
      for( std::vector< reco::GenJet >::const_iterator iGenJet = genJets.begin(); iGenJet != genJets.end(); iGenJet++){

	// Store GenJet Information
	eve->genJet_pt_.push_back(iGenJet->pt());
	eve->genJet_eta_.push_back(iGenJet->eta());
	eve->genJet_phi_.push_back(iGenJet->phi());
	eve->genJet_energy_.push_back(iGenJet->energy());
	eve->genJet_pdgId_.push_back(iGenJet->pdgId());

	if(yggdrasil_NLO::isGoodGenJet(*iGenJet, jet_cut_pt, jet_cut_eta)){
	  eve->genJet_isSelected_.push_back(1);
	}
	else{
	  eve->genJet_isSelected_.push_back(0);
	}

	int nGenJetMothers = (int)iGenJet->numberOfMothers();
	if(nGenJetMothers>0){
	  eve->genJet_mother_pdgId_.push_back(iGenJet->mother()->pdgId());
	}
	else{
	  eve->genJet_mother_pdgId_.push_back(-99);
	}


	// Increment int counter
	counter_genJet++;
      }
      
      
      

      

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


void yggdrasil_NLO::getGoodGenLeptons(std::vector< reco::GenParticle > genParticles,  std::vector< reco::GenParticle > &target, bool getTightLeptons, bool getLooseLeptons, double cut_tight_pt, double cut_tight_eta, double cut_loose_pt, double cut_loose_eta){


  // Loop over genJets and check that they pass quality cuts
  for( std::vector< reco::GenParticle >::const_iterator iGenParticle = genParticles.begin(); iGenParticle != genParticles.end(); iGenParticle++){

    // If electron or muon
    if( fabs(iGenParticle->pdgId()) == 11 || fabs(iGenParticle->pdgId()) == 13 ){

      std::vector< reco::GenParticle > allStagesThisLepton;
      yggdrasil_NLO::getAllStagesOfGenParticle(iGenParticle, genParticles, allStagesThisLepton);
      
      std::vector< reco::GenParticle >::const_iterator lastStage = allStagesThisLepton.end()-1;
      
      if(getTightLeptons){

	if( lastStage->pt()        > cut_tight_pt   &&
	    fabs(lastStage->eta()) < cut_tight_eta     ){
	  
	  target.push_back(*lastStage);
	  
	} // end if pass cuts
      
      } // end if getting tight leptons

      if(getLooseLeptons){

	if( lastStage->pt() < cut_tight_pt         &&
	    lastStage->pt() > cut_loose_pt         &&
	    fabs(lastStage->eta()) < cut_loose_eta    ){

	  target.push_back(*lastStage);

	} // end if pass cuts
 
      } // end if getting loose leptons

    } // end if electron or muon


  } // end loop over genParticles

  return;
}


//*****************************************************************************
