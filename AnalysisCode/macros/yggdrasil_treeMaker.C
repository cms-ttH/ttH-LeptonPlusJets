#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH3.h"
#include "TH2F.h"
#include "TF1.h"
#include "TF2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "TMath.h"
#include "TRandom3.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TGraphAsymmErrors.h"
#include "TVector.h"
#include "TLorentzVector.h"
#include "Math/Interpolator.h"


#ifdef __MAKECINT__
#pragma link C++ class std::vector< TLorentzVector >+; 
#endif


#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

//Headers for the data items
#include "BEAN/Collections/interface/BNelectron.h"
#include "BEAN/Collections/interface/BNevent.h"
#include "BEAN/Collections/interface/BNjet.h"
#include "BEAN/Collections/interface/BNmcparticle.h"
#include "BEAN/Collections/interface/BNmet.h"
#include "BEAN/Collections/interface/BNmuon.h"
#include "BEAN/Collections/interface/BNphoton.h"
#include "BEAN/Collections/interface/BNsupercluster.h"
#include "BEAN/Collections/interface/BNtrack.h"
#include "BEAN/Collections/interface/BNtrigger.h"
#include "BEAN/Collections/interface/BNskimbits.h"
#include "BEAN/Collections/interface/BNtrigobj.h"
#include "BEAN/Collections/interface/BNprimaryvertex.h"

#include "BEAN/BEANmaker/interface/BEANhelper.h"
#include "BEAN/BEANmaker/interface/AnglesUtil.h"
#include "ttH-LeptonPlusJets/AnalysisCode/interface/BEANloader.h"

#include "ttH-LeptonPlusJets/AnalysisCode/interface/BEANeventVars.h"

#endif

typedef std::vector<std::vector<double> >      vvdouble;
typedef std::vector<std::vector<std::string> > vvstring;
typedef std::vector<double>                    vdouble;
typedef std::vector<string>                    vstring;
typedef std::vector<bool>                      vbool;
typedef std::vector<int>                       vint;
typedef std::vector< TLorentzVector >          vecTLorentzVector;


//*****************************************************************************

double getBestHiggsMass(TLorentzVector lepton, TLorentzVector met, vecTLorentzVector jets, vdouble btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2, vecTLorentzVector loose_jets, vdouble loose_btag);

double getBestHiggsMass_old(TLorentzVector lepton, TLorentzVector met, vecTLorentzVector jets, vdouble btag, double &minChi);

//*****************************************************************************



void yggdrasil_treeMaker( int insample=1, std::string era = "2012_52x", int beanVersion=1, bool isMuon=true, int maxNentries=-1, int Njobs=1, int jobN=1, bool siteOSU=false, bool isSkim=true, bool lpcCondor=false ) {



  //////////////////////////////////////////////////////////////////////////
  ///  Load Files
  //////////////////////////////////////////////////////////////////////////

  std::cout << "\n\n" << "=================================" << "\n";
  std::cout           << " WELCOME TO YGGDRASIL TREEMAKER! " << "\n\n";
  std::cout           << "=================================" << "\n";
  std::cout << "\n\n   ===> load the root files! "         << "\n\n";


  sampleInfo mySample = BEAN::loadSamples( insample, era, beanVersion, isMuon, Njobs, jobN, siteOSU, isSkim, lpcCondor); 
  vstring fileNames = mySample.fileNames;

  //creates a ChainEvent allowing files to be linked
  fwlite::ChainEvent ev(fileNames);   


  //////////////////////////////////////////////////////////////////////////
  ///  Tree branches/leaves
  //////////////////////////////////////////////////////////////////////////

  // Open output file
  std::string treefilename = mySample.treeFileName;
  TFile treefile(treefilename.c_str(),"recreate");
  treefile.cd();

  // Declare TTree and initialize Branches
  eventVars *eve=0; 
  TTree *worldTree = new TTree("worldTree", "worldTree");
  worldTree->Branch("eve.", "eventVars", &eve, 8000, 1);




  //////////////////////////////////////////////////////////////////////////
  /// Initialize Selection and Counting Quantities  
  //////////////////////////////////////////////////////////////////////////

  
  // Event Counters
  int nevents=0;
  double nevents_wgt=0;

  int nevents_clean=0;
  double nevents_clean_wgt=0;
  int nevents_clean_ele=0;
  double nevents_clean_ele_wgt=0;
  int nevents_clean_mu=0;
  double nevents_clean_mu_wgt=0;

  int nevents_ll=0;
  double nevents_ll_wgt=0;
  int nevents_ll_ele=0;
  double nevents_ll_ele_wgt=0;
  int nevents_ll_mu=0;
  double nevents_ll_mu_wgt=0;

  int nevents_ll_nj=0;
  double nevents_ll_nj_wgt=0;
  int nevents_ll_nj_ele=0;
  double nevents_ll_nj_ele_wgt=0;
  int nevents_ll_nj_mu=0;
  double nevents_ll_nj_mu_wgt=0;

  int nevents_ll_nj_nt=0;
  double nevents_ll_nj_nt_wgt=0;
  int nevents_ll_nj_nt_ele=0;
  double nevents_ll_nj_nt_ele_wgt=0;
  int nevents_ll_nj_nt_mu=0;
  double nevents_ll_nj_nt_mu_wgt=0;


  // Get number of entries in event chain
  int nentries = ev.size();
  std::cout << "\n";
  std::cout << "\t Number of entries     = " << nentries << std::endl;
  std::cout << "\t Max number of entries = " << maxNentries << std::endl;
  std::cout << "\n" << std::endl;



  std::string muon_hlt_trigger_name = "HLT_IsoMu24_eta2p1_v";
  std::string electron_hlt_trigger_name = "HLT_Ele27_WP80_v";

  // Set Quantities used in BEANhelper::SetUp
  analysisType::analysisType iAnalysisType = analysisType::LJ;
  bool isLJ            = true;
  bool useReshapeCSV   = !true;
  bool usePFLeptons    = true;
  std::string pu_str   = "All"; // "2012A_13July,2012A_06Aug,2012B_13July,2012C_PR,2012C_24Aug,2012D_PR"
  //std::string pu_str   = "2012A_13July,2012A_06Aug,2012B_13July";

  // Initialize BEANhelper object
  BEANhelper beanHelper;
  
  //void BEANhelper::SetUp(string iEra, int iSampleNumber, bool iIsLJ, bool iIsData, string iDataset, bool iReshapeCSV, bool iPfLeptons = true, string iCollisionDS)

  mySample.str_dataset = "SingleMu";

  cout << "FOR BEAN::SetUp " << endl;  
  cout << "  sample name = " << mySample.sampleName << endl;
  cout << "  era         = " << era << endl;
  cout << "  insample    = " << insample << endl;
  cout << "  isLJ        = " << isLJ << endl;
  cout << "  analysisType= " << iAnalysisType << endl;
  cout << "  isData      = " << !mySample.isMC << endl;
  cout << "  dataset     = " << mySample.str_dataset << endl;
  cout << "  reshapecsv  = " << useReshapeCSV << endl;
  cout << "  usePFLep    = "  << usePFLeptons << endl;
  cout << "  mu trigger  = " << muon_hlt_trigger_name << endl;
  cout << "  e  trigger  = " << electron_hlt_trigger_name << endl;
  cout << "  pu_str      = "  << pu_str << endl;  
  cout << endl;


  if( insample==25333 ) insample = 2533;

  beanHelper.SetUp(era, insample, iAnalysisType, !mySample.isMC, mySample.str_dataset, useReshapeCSV, usePFLeptons, pu_str);


  BEANhelper beanHelper_2012AB;
  beanHelper_2012AB.SetUp(era, insample, iAnalysisType, !mySample.isMC, mySample.str_dataset, useReshapeCSV, usePFLeptons, "2012A_13July, 2012A_06Aug, 2012B_13July");

  BEANhelper beanHelper_2012ABCD;
  beanHelper_2012ABCD.SetUp(era, insample, iAnalysisType, !mySample.isMC, mySample.str_dataset, useReshapeCSV, usePFLeptons, "All");


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

      // Check if in specified event range
      if( cnt<firstEvent ) continue;
      if( cnt==lastEvent ) break;

      // Print status
      if( cnt==1 ){
        std::cout << "     Event " << cnt << std::endl;
      }
      if( cnt%100000==0 && cnt!=1 ){
        std::cout << "           " << cnt << "\t"; 
	std::cout << int(double(cnt-firstEvent)/double(NeventsPerJob)*100);
	std::cout << "% done" << std::endl;
      }

      if( cnt==(maxNentries+1) ) break;

      // Initialize eventVars object
      eve->initialize();      

      /////////
      ///
      /// fwlite object handles
      ///
      ////////

      fwlite::Handle<BNeventCollection> h_event;
      h_event.getByLabel(ev,"BNproducer");
      BNeventCollection const &events = *h_event;

      fwlite::Handle<BNmuonCollection> h_muons;
      if( era=="2012_52x" ) h_muons.getByLabel(ev,"BNproducer","selectedPatMuonsPFlow");
      else                  h_muons.getByLabel(ev,"BNproducer","selectedPatMuonsLoosePFlow");
      BNmuonCollection const &muons = *h_muons;

      fwlite::Handle<BNmetCollection> h_pfmet;
      if( era=="2012_53x" || era=="2012_52x" ) h_pfmet.getByLabel(ev,"BNproducer","patMETsPFlow");
      else                       h_pfmet.getByLabel(ev,"BNproducer","patMETsTypeIPFlow");
      BNmetCollection const &pfmets = *h_pfmet;

      fwlite::Handle<BNtriggerCollection> h_hlt;
      h_hlt.getByLabel(ev,"BNproducer","HLT");
      BNtriggerCollection const &hlt = *h_hlt;

      fwlite::Handle<BNtrigobjCollection> h_hltobj;
      h_hltobj.getByLabel(ev,"BNproducer","HLT");
      BNtrigobjCollection const &hltobjs = *h_hltobj;

      fwlite::Handle<BNprimaryvertexCollection> h_pvs;
      h_pvs.getByLabel(ev,"BNproducer","offlinePrimaryVertices");
      BNprimaryvertexCollection const &pvs = *h_pvs;

      fwlite::Handle<BNjetCollection> h_pfjets;
      h_pfjets.getByLabel(ev,"BNproducer","selectedPatJetsPFlow");
      BNjetCollection const &pfjets = *h_pfjets;

      fwlite::Handle<BNelectronCollection> h_electrons;
      if( era=="2012_52x" ) h_electrons.getByLabel(ev,"BNproducer","selectedPatElectronsPFlow");
      else                  h_electrons.getByLabel(ev,"BNproducer","selectedPatElectronsLoosePFlow");
      BNelectronCollection const &electrons = *h_electrons;

      fwlite::Handle<BNmcparticleCollection> h_mcparticles;
      h_mcparticles.getByLabel(ev,"BNproducer","MCstatus3");
      BNmcparticleCollection const &mcparticles = *h_mcparticles;

      fwlite::Handle<BNtauCollection> h_taus;
      h_taus.getByLabel(ev,"BNproducer","selectedPatTaus");
      BNtauCollection const &taus = *h_taus;



      /////////
      ///
      /// Event / MET
      ///
      ////////

      EventIter event = events.begin();

      double numTruePV = event->numTruePV;
      //int numGenPV = event->numGenPV;

      double intLumi = 4970.;
      if( era=="2011" ) {
        intLumi = 4970.;
        if( !isMuon ) intLumi = 4980.;
      }
      else if( era=="2012_52x" ){
        intLumi = 5088;
        if( !isMuon ) intLumi = 5080;
      }
      else if( era=="2012_53x" ){
        intLumi = 19417;
        if( !isMuon ) intLumi = 19183;
	if( beanVersion==2 ){
	  intLumi = 19456;
	  if( !isMuon ) intLumi = 19444;
	}
      }

      double wgt_lumi = 1.;
      if( insample>=0 ) wgt_lumi = mySample.xSec * intLumi *1./ mySample.nGen;

      int run  = event->run;
      int lumi = event->lumi;
      long evt = event->evt;


      double PVx = event->PVx;
      double PVy = event->PVy;
      double PVz = event->PVz;

      MetIter pfmet = pfmets.begin();
      

      // Primary Vertex Selection
      double minNDOF = 4;
      double maxAbsZ = 24;
      double maxd0   = 2.;
      int numpv      = 0;
      for( int i=0; i<int(pvs.size()); i++ ){
	bool isGood = ( (pvs.at(i).ndof>=minNDOF) &&
			(fabs(pvs.at(i).z)<maxAbsZ) &&
			(fabs(pvs.at(i).rho)<maxd0) );
	bool isFake = ( pvs.at(i).isFake==1 );
	if( isGood && !isFake ) numpv++;
      }


      bool electron_triggerFound = false;
      bool electron_triggerPass  = false;
      bool muon_triggerFound = false;
      bool muon_triggerPass  = false;
      for( TrigIter hltbit = hlt.begin(); hltbit != hlt.end(); ++hltbit ){
	std::string hlt_name = hltbit->name;
	if( (hlt_name.find(electron_hlt_trigger_name)!=std::string::npos) ){
	  if( hltbit->prescale!=1 ) continue;
	  electron_triggerFound = true;
	  electron_triggerPass = ( hltbit->pass==1 );
	}

	if( (hlt_name.find(muon_hlt_trigger_name)!=std::string::npos) ){
	  if( hltbit->prescale!=1 ) continue;
	  muon_triggerFound = true;
	  muon_triggerPass = ( hltbit->pass==1 );
	}

	if( electron_triggerPass && muon_triggerPass ) break;
      }

      // make sure trigger used to tag exists in the event
      if( (!electron_triggerFound || !muon_triggerFound) && false ){
	std::cout << "  error ===> Trigger not found!!!  Here are the available triggers: " << std::endl;
	for( TrigIter hltbit = hlt.begin(); hltbit != hlt.end(); ++hltbit ){
	  std::string hlt_name = hltbit->name;
	  std::cout << "\t\t " << hlt_name << "\t\t prescale = " << hltbit->prescale << "\t\t pass = " << hltbit->pass << std::endl;
	}
	break;
      }


      // Increment event counter
      nevents++;
      nevents_wgt+=wgt_lumi;


      // Check Quality Cuts on the Event
      bool passGoodVertex = ( numpv>0 ) ? true : false;
      bool passFilterOutScraping = ( event->FilterOutScraping==1 ) ? true : false;
      bool passHBHENoiseFilter = ( event->HBHENoiseFilter==1 ) ? true : false;

      bool passTrigger = ( electron_triggerPass || muon_triggerPass );

      bool cleanEvent = ( passTrigger && passGoodVertex && ((passFilterOutScraping && passHBHENoiseFilter) || (insample>=0)) );

      if( !cleanEvent ) continue;
      //if(!triggerPass) continue;

      // TTJets -> tt+lf, tt+cc, tt+bb seperation
      bool splitEvent = true;
      //if( mySample.isTTJets ) splitEvent = beanHelper.ttPlusHeavyKeepEvent( mcparticles, pfjets );
      if( mySample.isTTJets ) splitEvent = beanHelper.ttPlusHFKeepEvent( mcparticles, pfjets );
      if( !splitEvent ) continue;
      

      double wgt_topPtSF     = beanHelper.GetTopPtweight( mcparticles );
      double wgt_topPtSFUp   = beanHelper.GetTopPtweightUp( mcparticles );
      double wgt_topPtSFDown = beanHelper.GetTopPtweightDown( mcparticles );

      //std::cout << "\t Higgs decay type = " << beanHelper.GetHdecayType( mcparticles ) << std::endl;
      if( insample>=8000 && insample<=9145) eve->higgsDecayType_ = beanHelper.GetHdecayType( mcparticles );

      // Increment event counter
      nevents_clean++;
      nevents_clean_wgt+=wgt_lumi;

      if( electron_triggerPass ){
	nevents_clean_ele++;
	nevents_clean_ele_wgt+=wgt_lumi;
      }
      if( muon_triggerPass ){
	nevents_clean_mu++;
	nevents_clean_mu_wgt+=wgt_lumi;
      }


      /////////
      ///
      /// Electrons
      ///
      ////////
      BNelectronCollection selectedElectrons_tight = beanHelper.GetSelectedElectrons( electrons, electronID::electronTight );
      BNelectronCollection selectedElectrons_loose = beanHelper.GetSelectedElectrons( electrons, electronID::electronLoose );

      int numTightElectrons = int(selectedElectrons_tight.size());
      int numLooseElectrons = int(selectedElectrons_loose.size()) - numTightElectrons;


      /////////
      ///
      /// Muons
      ///
      ////////
      BNmuonCollection selectedMuons_tight = beanHelper.GetSelectedMuons( muons, muonID::muonTight );
      BNmuonCollection selectedMuons_loose = beanHelper.GetSelectedMuons( muons, muonID::muonLoose );

      int numTightMuons = int(selectedMuons_tight.size());
      int numLooseMuons = int(selectedMuons_loose.size()) - numTightMuons;


      // Pass one tight lepton, zero loose lepton cuts
      bool passLJ = ( (numTightMuons+numTightElectrons)==1 &&
      		      numLooseMuons==0 && numLooseElectrons==0 );

      if( !passLJ ) continue;

   
      // Make sure tight lepton matches selection type
      if( !( numTightMuons==1 || numTightElectrons==1) ) continue;
      isMuon = ( numTightMuons==1 );


      //
      if( era=="2011" ) {
        intLumi = 4970.;
        if( !isMuon ) intLumi = 4980.;
      }
      else if( era=="2012_52x" ){
        intLumi = 5088;
        if( !isMuon ) intLumi = 5080;
      }
      else if( era=="2012_53x" ){
        intLumi = 19417;
        if( !isMuon ) intLumi = 19183;
      }
      if( era=="2012_53x" ){
        intLumi = 19417;
        if( !isMuon ) intLumi = 19183;
	if( beanVersion==2 ){
	  intLumi = 19456;
	  if( !isMuon ) intLumi = 19444;
	}
      }
      if( insample>=0 ) wgt_lumi = mySample.xSec * intLumi *1./ mySample.nGen;
      

      // Increment counter for passing lepton cuts
      nevents_ll++;
      nevents_ll_wgt += wgt_lumi;
      if( isMuon ){
	nevents_ll_mu++;
	nevents_ll_mu_wgt += wgt_lumi;
      }
      else{
	nevents_ll_ele++;
	nevents_ll_ele_wgt += wgt_lumi;
      }

      eve->passElectronTrigger_ = ( electron_triggerPass ) ? 1 : 0;
      eve->passMuonTrigger_     = ( muon_triggerPass     ) ? 1 : 0;
      eve->leptonType_  = ( isMuon ) ? 1 : 0;

      bool matchSingleMuTrigger = false;
      bool matchSingleElectronTrigger = false;

      if( isMuon ) matchSingleMuTrigger = beanHelper.MuonMatchesSingleMuTrigger( selectedMuons_tight[0], hlt, hltobjs );
      else         matchSingleElectronTrigger = beanHelper.ElectronMatchesSingleEleTrigger(selectedElectrons_tight[0], hlt, hltobjs );

      eve->matchSingleMuTrigger_ = ( matchSingleMuTrigger ) ? 1 : 0;
      eve->matchSingleElectronTrigger_ = ( matchSingleElectronTrigger ) ? 1 : 0;



      eve->run_ = run;
      eve->lumi_ = lumi;
      eve->evt_ = evt;



      eve->Q2ScaleUpWgt_ = 1.;
      eve->Q2ScaleDownWgt_ = 1.;
      if( mySample.isTTJets ){
	eve->Q2ScaleUpWgt_   = beanHelper.GetQ2ScaleUp(events.at(0));//1.402 * event->Q2ScaleUpWgt;
	eve->Q2ScaleDownWgt_ = beanHelper.GetQ2ScaleDown(events.at(0));//0.683 * event->Q2ScaleDownWgt;
      }


      eve->numPVs_ = numpv;
      eve->numSys_ = kNumSys;



      int lepton_genId=-99;
      int lepton_genParentId=-99;
      int lepton_genGrandParentId=-99;

      TLorentzVector leptonV;
      vdouble vlepton;
      
      double lepton_relIso = -99;
      if( isMuon ) {
        leptonV.SetPxPyPzE( selectedMuons_tight.at(0).px, selectedMuons_tight.at(0).py, selectedMuons_tight.at(0).pz, selectedMuons_tight.at(0).energy);
        vlepton.push_back(selectedMuons_tight.at(0).px);
        vlepton.push_back(selectedMuons_tight.at(0).py);
        vlepton.push_back(selectedMuons_tight.at(0).pz);
        vlepton.push_back(selectedMuons_tight.at(0).energy);

	lepton_genId = selectedMuons_tight.at(0).genId;
	lepton_genParentId = selectedMuons_tight.at(0).genMother0Id;
	lepton_genGrandParentId = selectedMuons_tight.at(0).genGrandMother00Id;

	lepton_relIso = beanHelper.GetMuonRelIso(selectedMuons_tight.at(0));
      }
      else {
        leptonV.SetPxPyPzE( selectedElectrons_tight.at(0).px, selectedElectrons_tight.at(0).py, selectedElectrons_tight.at(0).pz, selectedElectrons_tight.at(0).energy);
        vlepton.push_back(selectedElectrons_tight.at(0).px);
        vlepton.push_back(selectedElectrons_tight.at(0).py);
        vlepton.push_back(selectedElectrons_tight.at(0).pz);
        vlepton.push_back(selectedElectrons_tight.at(0).energy);

	lepton_genId = selectedElectrons_tight.at(0).genId;
	lepton_genParentId = selectedElectrons_tight.at(0).genMother0Id;
	lepton_genGrandParentId = selectedElectrons_tight.at(0).genGrandMother00Id;

	lepton_relIso = beanHelper.GetElectronRelIso(selectedElectrons_tight.at(0));
      }

      eve->tight_lepton_pt_  = leptonV.Pt();
      eve->tight_lepton_eta_ = leptonV.Eta();
      eve->tight_lepton_phi_ = leptonV.Phi();
      eve->tight_lepton_relIso_ = lepton_relIso;


      // PU scale factor
      double wgtPU = 1, wgtPUup = 1, wgtPUdown = 1;
      
      // updated to new BEANhelper 1/30/13:
      wgtPU = beanHelper.GetPUweight( numTruePV );
      wgtPUup = beanHelper.GetPUweightUp( numTruePV );
      wgtPUdown = beanHelper.GetPUweightDown( numTruePV );

      eve->wgt_pu_     = wgtPU;
      eve->wgt_puUp_   = wgtPUup;
      eve->wgt_puDown_ = wgtPUdown;


      // PU scale factor 2012AB
      double wgtPU_2012AB = 1, wgtPUup_2012AB = 1, wgtPUdown_2012AB = 1;
      wgtPU_2012AB = beanHelper_2012AB.GetPUweight( numTruePV );
      wgtPUup_2012AB = beanHelper_2012AB.GetPUweightUp( numTruePV );
      wgtPUdown_2012AB = beanHelper_2012AB.GetPUweightDown( numTruePV );

      eve->wgt_pu_2012AB_     = wgtPU_2012AB;
      eve->wgt_puUp_2012AB_   = wgtPUup_2012AB;
      eve->wgt_puDown_2012AB_ = wgtPUdown_2012AB;

      // PU scale factor 2012ABCD
      double wgtPU_2012ABCD = 1, wgtPUup_2012ABCD = 1, wgtPUdown_2012ABCD = 1;
      wgtPU_2012ABCD = beanHelper_2012ABCD.GetPUweight( numTruePV );
      wgtPUup_2012ABCD = beanHelper_2012ABCD.GetPUweightUp( numTruePV );
      wgtPUdown_2012ABCD = beanHelper_2012ABCD.GetPUweightDown( numTruePV );

      eve->wgt_pu_2012ABCD_     = wgtPU_2012ABCD;
      eve->wgt_puUp_2012ABCD_   = wgtPUup_2012ABCD;
      eve->wgt_puDown_2012ABCD_ = wgtPUdown_2012ABCD;


      // Lepton scale factor
      float leptonSF = 1.0;
      if( insample>=0 ){
	if( isMuon ) leptonSF = beanHelper.GetMuonSF(selectedMuons_tight.at(0));
	else         leptonSF = beanHelper.GetElectronSF(selectedElectrons_tight.at(0));
      }

      eve->wgt_lepSF_  = leptonSF;

      eve->wgt_lumi_  = intLumi;
      eve->wgt_xs_    = mySample.xSec;
      eve->wgt_nGen_  = mySample.nGen;


      eve->wgt_topPt_ = wgt_topPtSF;
      eve->wgt_topPtUp_ = wgt_topPtSFUp;
      eve->wgt_topPtDown_ = wgt_topPtSFDown;



      double top_pt = -1;
      double antitop_pt = -1;

      // Get top pTs for reweighting downstream
      if( mySample.isTTJets ){
	for( int iPar=0; iPar<int(mcparticles.size()); iPar++ ){
	  int id = mcparticles.at(iPar).id;
	  if( id==6 )       top_pt     = mcparticles.at(iPar).pt;
	  else if( id==-6 ) antitop_pt = mcparticles.at(iPar).pt;
	}
      }
      eve->top_pt_ = top_pt;
      eve->antitop_pt_ = antitop_pt;


      // Special collection used for correcting MET collection
      BNjetCollection pfJets_forMET  = beanHelper.GetSelectedJets( pfjets, 12., 4.9, jetID::jetLoose, '-' ); 

      // bool for passing jet/tag cuts, first is for tree filling, second is for event counting
      bool hasNumJetNumTag = false;
      bool hasNumJetNumTag_noSys = false;


      double totalWgt = wgt_lumi;

      // Loop over systematics
      for( int iSys=0; iSys<kNumSys; iSys++ ){

	eve->wgt_[iSys] = wgt_lumi;

	// no systematic up/down for lepton SF for now
	if( iSys==1 )      eve->wgt_[iSys] *= leptonSF;
	else if( iSys==2 ) eve->wgt_[iSys] *= leptonSF;
	else               eve->wgt_[iSys] *= leptonSF;


	// PU weight up/down
	if( iSys==3 )      eve->wgt_[iSys] *= wgtPUup;
	else if( iSys==4 ) eve->wgt_[iSys] *= wgtPUdown;
	else               eve->wgt_[iSys] *= wgtPU;

	if( iSys==0 ) totalWgt *= leptonSF*wgtPU;

	sysType::sysType iSysType = sysType::NA;
	switch( iSys ){
	case 5 : iSysType = sysType::hfSFup;   break;
	case 6 : iSysType = sysType::hfSFdown; break;
	case 7 : iSysType = sysType::lfSFup;   break;
	case 8 : iSysType = sysType::lfSFdown; break;
	case 9 : iSysType = sysType::JERup;    break;
	case 10: iSysType = sysType::JERdown;  break;
	case 11: iSysType = sysType::JESup;    break;
	case 12: iSysType = sysType::JESdown;  break;
	case 17: iSysType = sysType::CSVLFup;         break;
	case 18: iSysType = sysType::CSVLFdown;       break;
	case 19: iSysType = sysType::CSVHFup;         break;
	case 20: iSysType = sysType::CSVHFdown;       break;
	case 21: iSysType = sysType::CSVHFStats1up;   break;
	case 22: iSysType = sysType::CSVHFStats1down; break;
	case 23: iSysType = sysType::CSVLFStats1up;   break;
	case 24: iSysType = sysType::CSVLFStats1down; break;
	case 25: iSysType = sysType::CSVHFStats2up;   break;
	case 26: iSysType = sysType::CSVHFStats2down; break;
	case 27: iSysType = sysType::CSVLFStats2up;   break;
	case 28: iSysType = sysType::CSVLFStats2down; break;
	case 29: iSysType = sysType::CSVCErr1up;   break;
	case 30: iSysType = sysType::CSVCErr1down; break;
	case 31: iSysType = sysType::CSVCErr2up;   break;
	case 32: iSysType = sysType::CSVCErr2down; break;
	default: iSysType = sysType::NA;       break;
	}


	bool IsTauTauLeptonEvent = beanHelper.IsTauEvent(taus, pfjets, electrons, muons, iSysType);
	eve->IsTauTauLeptonEvent_[iSys] = ( IsTauTauLeptonEvent ) ? 1 : 0;

	// Add Q2scale systematic
	if( iSys==13 )      eve->wgt_[iSys] *= eve->Q2ScaleUpWgt_;
	else if( iSys==14 ) eve->wgt_[iSys] *= eve->Q2ScaleDownWgt_;


	// top pT reweight
	if( iSys==15 )      eve->wgt_[iSys] *= wgt_topPtSFUp;
	else if( iSys==16 ) eve->wgt_[iSys] *= wgt_topPtSFDown;
	else                eve->wgt_[iSys] *= wgt_topPtSF;

	if( iSys==0 ) totalWgt *= wgt_topPtSF;


	// nJets/Tags
	// THESE MUST BE INITIALIZED TO -1
	eve->numJets_float_[iSys] = -1;
	eve->numTags_float_[iSys] = -1;


	eve->numJets_PUid_full_loose_float_[iSys]  = -1;
	eve->numJets_PUid_full_medium_float_[iSys] = -1;
	eve->numJets_PUid_full_tight_float_[iSys]  = -1;

	eve->numJets_PUid_simple_loose_float_[iSys]  = -1;
	eve->numJets_PUid_simple_medium_float_[iSys] = -1;
	eve->numJets_PUid_simple_tight_float_[iSys]  = -1;

	eve->numTags_PUid_full_loose_float_[iSys]  = -1;
	eve->numTags_PUid_full_medium_float_[iSys] = -1;
	eve->numTags_PUid_full_tight_float_[iSys]  = -1;

	eve->numTags_PUid_simple_loose_float_[iSys]  = -1;
	eve->numTags_PUid_simple_medium_float_[iSys] = -1;
	eve->numTags_PUid_simple_tight_float_[iSys]  = -1;


	/////////
	///
	/// Pfjets
	///
	////////

        // Get JES/JER corrected jets
	BNjetCollection correctedJets = beanHelper.GetCorrectedJets( pfjets, iSysType );
        // Get jet collection which pass selection
	BNjetCollection selectedJets_unsorted  = beanHelper.GetSelectedJets( correctedJets, 30., 2.4, jetID::jetLoose, '-' ); 
	//BNjetCollection selectedJets_unsorted  = beanHelper.GetSelectedJets( correctedJets, 25., 2.4, jetID::jetLoose, '-' ); // DP changing threshold


	// Require at least 4 jets
	if( !(selectedJets_unsorted.size()>=4) ) continue;

        if(iSys==0){
          nevents_ll_nj++;
          nevents_ll_nj_wgt += wgt_lumi;
	  if( isMuon ){
	    nevents_ll_nj_mu++;
	    nevents_ll_nj_mu_wgt += wgt_lumi;
	  }
	  else{
	    nevents_ll_nj_ele++;
	    nevents_ll_nj_ele_wgt += wgt_lumi;
	  }
	}

        // Get CSVM tagged jet collection
	BNjetCollection selectedJets_tag_unsorted   = beanHelper.GetSelectedJets( correctedJets, 30., 2.4, jetID::jetLoose, 'M' ); 
	//BNjetCollection selectedJets_tag_unsorted   = beanHelper.GetSelectedJets( correctedJets, 25., 2.4, jetID::jetLoose, 'M' );  // DP changing threshold
 
        // Get nontagged jet collection
	BNjetCollection selectedJets_untag_unsorted = beanHelper.GetSymmetricDifference( selectedJets_unsorted, selectedJets_tag_unsorted );



	// Require at least 2 tags
	if( !(selectedJets_tag_unsorted.size()>=2) ) continue;

	if(iSys==0) hasNumJetNumTag_noSys = true;
	hasNumJetNumTag = true;

     
        // Sort jet collections by pT
	BNjetCollection selectedJets       = beanHelper.GetSortedByPt( selectedJets_unsorted );
	BNjetCollection selectedJets_tag   = beanHelper.GetSortedByPt( selectedJets_tag_unsorted );
	BNjetCollection selectedJets_untag = beanHelper.GetSortedByPt( selectedJets_untag_unsorted );

	//if( mySample.isTTJets ) splitEvent = beanHelper.ttPlusHFKeepEvent( mcparticles, selectedJets );
	//if( !splitEvent ) continue;

	if( mySample.isTTJets ){
	  eve->ttbb_algo_result_[iSys] = beanHelper.ttPlusBBClassifyEvent( mcparticles, selectedJets );
	  eve->ttcc_algo_result_[iSys] = beanHelper.ttPlusCCClassifyEvent( mcparticles, selectedJets );
	}

	double csvWgt = beanHelper.GetCSVweight( selectedJets, iSysType );
	//std::cout << "\t CSV weight = " << csvWgt << std::endl;

	eve->wgt_csvSF_[iSys] = ( mySample.isMC ) ? csvWgt : 1.0;
	eve->wgt_[iSys] *= csvWgt;

	if( iSys==0 ) totalWgt *= csvWgt;


	// Get numJets, numTags
	int numJet = int( selectedJets.size() );
	int numTag = int( selectedJets_tag.size() );

         
	// Get Corrected MET
	BNmet correctedMET = beanHelper.GetCorrectedMET( pfmets.at(0), pfJets_forMET, iSysType );

	TLorentzVector metV(correctedMET.px,correctedMET.py,0.0,correctedMET.pt);


	// Get Btag Weight
	int useBtagSys = 0;
	if( iSys==5 )      useBtagSys = 1;
	else if( iSys==6 ) useBtagSys = -1;
	else if( iSys==7 ) useBtagSys = 2;
	else if( iSys==8 ) useBtagSys = -2;

	std::vector<BTagWeight::JetInfo> myjetinfo;
	for( BNjetCollection::const_iterator iJet = selectedJets.begin(); iJet != selectedJets.end(); iJet++ ){ 
	  std::vector<double> myEffSF = beanHelper.getEffSF( useBtagSys, iJet->pt, iJet->eta, iJet->flavour );
	  BTagWeight::JetInfo myjet( myEffSF[0], myEffSF[1] );
	  myjetinfo.push_back(myjet);
	}

	BTagWeight bweight(1,1);
	eve->wgt_btagSF_[iSys] = ( mySample.isMC ) ? bweight.weight(myjetinfo,numTag,numTag) : 1.0;

       
        // Initialize event vars, selected jets
	double mht_px = 0.;
	double mht_py = 0.;
	eve->HT_[iSys] = 0.;


	double min_dR_lep_jet = 99.;
	vecTLorentzVector jetV;
	std::vector<double> csvV;
	std::vector<double> jetPUmva_full_vect;
	std::vector<double> jetPUmva_simple_vect;

	std::vector<int>    jetPUid_full_loose_vect;
	std::vector<int>    jetPUid_full_medium_vect;
	std::vector<int>    jetPUid_full_tight_vect;

	std::vector<int>    jetPUid_simple_loose_vect;
	std::vector<int>    jetPUid_simple_medium_vect;
	std::vector<int>    jetPUid_simple_tight_vect;

	double numtag = 0, numuntag = 0;
	double sum_btag_disc_btags = 0;
	double sum_btag_disc_non_btags = 0;

	double max_m3_pt = -1;
	double max_m3 = -1;
	double max_m3_1tag_pt = -1;
	double max_m3_1tag = -1;

	vvdouble vvjets;
	
	vdouble dR2Mean_vect;
	vdouble dRMean_vect;
	vdouble frac01_vect;
	vdouble frac02_vect;
	vdouble frac03_vect;
	vdouble beta_vect;
	vdouble betaStar_vect;
	vdouble leadCandDistFromPV_vect;
	vdouble minPVDist_vect;

	int numJets_PUid_full_loose = 0;
	int numJets_PUid_full_medium = 0;
	int numJets_PUid_full_tight = 0;

	int numJets_PUid_simple_loose = 0;
	int numJets_PUid_simple_medium = 0;
	int numJets_PUid_simple_tight = 0;

	int numTags_PUid_full_loose = 0;
	int numTags_PUid_full_medium = 0;
	int numTags_PUid_full_tight = 0;

	int numTags_PUid_simple_loose = 0;
	int numTags_PUid_simple_medium = 0;
	int numTags_PUid_simple_tight = 0;


	vint jet_genId_vect;
	vint jet_flavour_vect;
	vint jet_genParentId_vect;
	vint jet_genGrandParentId_vect;

        // Loop over selected jets
	for( BNjetCollection::const_iterator iJet = selectedJets.begin(); iJet != selectedJets.end(); iJet++ ){ 
 
          // MHT
	  mht_px += - iJet->px;
	  mht_py += - iJet->py;
	  eve->HT_[iSys] += iJet->pt;

	  jet_flavour_vect.push_back(iJet->flavour);
	  jet_genId_vect.push_back(iJet->genPartonId);
	  jet_genParentId_vect.push_back(iJet->genPartonMother0Id);
	  jet_genGrandParentId_vect.push_back(iJet->genPartonGrandMotherId);

	  // DR(lepton, jet)
	  double dR_lep_jet = kinem::delta_R(leptonV.Eta(),leptonV.Phi(),iJet->eta,iJet->phi);
	  if( dR_lep_jet<min_dR_lep_jet ) min_dR_lep_jet = dR_lep_jet;

    // Get jet 4Vector and add to vecTLorentzVector for jets
	  TLorentzVector jet0p4;	  
	  jet0p4.SetPxPyPzE(iJet->px,iJet->py,iJet->pz,iJet->energy);
	  jetV.push_back(jet0p4);
	  
	  // make vvdouble version of vecTLorentzVector
	  vdouble vjets;
	  vjets.push_back(iJet->px);
	  vjets.push_back(iJet->py);
	  vjets.push_back(iJet->pz);
	  vjets.push_back(iJet->energy);
	  vvjets.push_back(vjets);
	  

          // Get CSV discriminant, check if passes Med WP 
	  double myCSV = iJet->btagCombinedSecVertex;
	  csvV.push_back(myCSV);
	  int csvM0 = ( iJet->btagCombinedSecVertex > 0.679 ) ? 1 : 0;
	  if( myCSV>0.679 ){
	    numtag += 1;
	    sum_btag_disc_btags += myCSV;
	  }
	  else{
	    numuntag += 1;
	    sum_btag_disc_non_btags += myCSV;
	  }

	  // PU quantities
	  jetPUmva_full_vect.push_back(iJet->puJetMVA_full);
	  jetPUmva_simple_vect.push_back(iJet->puJetMVA_simple);

	  jetPUid_full_loose_vect.push_back(iJet->puJetId_loose_full);
	  jetPUid_full_medium_vect.push_back(iJet->puJetId_medium_full);
	  jetPUid_full_tight_vect.push_back(iJet->puJetId_tight_full);

	  jetPUid_simple_loose_vect.push_back(iJet->puJetId_loose_simple);
	  jetPUid_simple_medium_vect.push_back(iJet->puJetId_medium_simple);
	  jetPUid_simple_tight_vect.push_back(iJet->puJetId_tight_simple);

	  dR2Mean_vect.push_back(iJet->dR2Mean);
	  dRMean_vect.push_back(iJet->dRMean);
	  frac01_vect.push_back(iJet->frac01);
	  frac02_vect.push_back(iJet->frac02);
	  frac03_vect.push_back(iJet->frac03);
	  beta_vect.push_back(iJet->beta);
	  betaStar_vect.push_back(iJet->betaStar);
	  leadCandDistFromPV_vect.push_back(iJet->leadCandDistFromPV);
	  

	  double minDistPV = 99;
	  for( int iPV=0; iPV<int(pvs.size()); iPV++ ){
	    if( fabs(PVx-pvs.at(iPV).x)<0.01 && fabs(PVy-pvs.at(iPV).y)<0.01 && fabs(PVz-pvs.at(iPV).z)<0.01 ) continue;

	    double dX = pvs.at(iPV).x - iJet->leadCandVx;
	    double dY = pvs.at(iPV).y - iJet->leadCandVy;
	    double dZ = pvs.at(iPV).z - iJet->leadCandVz;

	    double distPV = sqrt( dX*dX + dY*dY + dZ*dZ );
	    minDistPV = ( distPV<minDistPV ) ? distPV : minDistPV;
	  }
	  minPVDist_vect.push_back( minDistPV );


	  if( iJet->puJetId_loose_full==1 ){
	    numJets_PUid_full_loose++;
	    if( csvM0 ) numTags_PUid_full_loose++;
	  }
	  if( iJet->puJetId_medium_full==1 ){
	    numJets_PUid_full_medium++;
	    if( csvM0 ) numTags_PUid_full_medium++;
	  }
	  if( iJet->puJetId_tight_full==1 ){
	    numJets_PUid_full_tight++;
	    if( csvM0 ) numTags_PUid_full_tight++;
	  }
	  
	  if( iJet->puJetId_loose_simple==1 ){
	    numJets_PUid_simple_loose++;
	    if( csvM0 ) numTags_PUid_simple_loose++;
	  }
	  if( iJet->puJetId_medium_simple==1 ){
	    numJets_PUid_simple_medium++;
	    if( csvM0 ) numTags_PUid_simple_medium++;
	  }
	  if( iJet->puJetId_tight_simple==1 ){
	    numJets_PUid_simple_tight++;
	    if( csvM0 ) numTags_PUid_simple_tight++;
	  }

	  // Second Loop over selected jets
	  for( BNjetCollection::const_iterator jJet = iJet; jJet != selectedJets.end(); jJet++ ){ 

            // Continue if same jet as above loop
	    if( iJet==jJet ) continue;

	    // Get second jet 4Vector and check bTag discriminant
	    TLorentzVector jet1p4;
	    jet1p4.SetPxPyPzE(jJet->px,jJet->py,jJet->pz,jJet->energy);
	    int csvM1 = ( jJet->btagCombinedSecVertex > 0.679 ) ? 1 : 0;

	    // Third loop over selected jets
	    for( BNjetCollection::const_iterator kJet = jJet; kJet != selectedJets.end(); kJet++ ){ 
     
	      // Continue is third jet is same as the above two jets
	      if( iJet==kJet || jJet==kJet ) continue;

              // Get third jet 4Vector and chekc bTag discriminant
	      TLorentzVector jet2p4;
	      jet2p4.SetPxPyPzE(kJet->px,kJet->py,kJet->pz,kJet->energy);
	      int csvM2 = ( kJet->btagCombinedSecVertex > 0.679 ) ? 1 : 0;

              // Get sum of three jet 4Vectors
	      TLorentzVector sum_jet = jet0p4 + jet1p4 + jet2p4;
	      double temp_pt = sum_jet.Pt();
	      double temp_mass = sum_jet.M();
             
	      // Check if this combination has the highest pT, for M3
	      if( temp_pt>max_m3_pt ){
		max_m3_pt = temp_pt;
		max_m3 = temp_mass;
	      }

              // Check if this combination has highest pT with only 1 bTag
	      if( (csvM0 + csvM1 + csvM2)==1 && temp_pt>max_m3_1tag_pt ){
		max_m3_1tag_pt = temp_pt;
		max_m3_1tag = temp_mass;
	      }

	    }// end loop over kJet
	  }// end loop over jJet
	}// end loop over iJet


        // Compute Avg bTag Discriminant for event
	double ave_btag_disc_non_btags = ( numuntag > 0. ) ? sum_btag_disc_non_btags/numuntag : 0.;
	double ave_btag_disc_btags = ( numtag > 0. ) ? sum_btag_disc_btags/numtag : 0.;


	// Add loose jet container
	BNjetCollection selectedJets_unsorted_loose  = beanHelper.GetSelectedJets( correctedJets, 20., 2.4, jetID::jetLoose, '-' ); 
	BNjetCollection selectedJets_loose = beanHelper.GetSortedByPt( selectedJets_unsorted_loose );

	vvdouble vvjets_loose;
	std::vector<double> csvV_loose;
	vint jet_flavour_vect_loose;
	vecTLorentzVector jetV_loose;

	int numJet_loose = 0;
	int numTag_loose = 0;

        // Loop over selected jets
	for( BNjetCollection::const_iterator iJet = selectedJets_loose.begin(); iJet != selectedJets_loose.end(); iJet++ ){ 
	  if( iJet->pt>=30. ) continue;
	  //if( iJet->pt>=25. ) continue; // DP changing threshold

	  numJet_loose++;
	  jet_flavour_vect_loose.push_back(iJet->flavour);

	  TLorentzVector jet0p4;	  
	  jet0p4.SetPxPyPzE(iJet->px,iJet->py,iJet->pz,iJet->energy);
	  jetV_loose.push_back(jet0p4);

	  // make vvdouble version of vecTLorentzVector
	  vdouble vjets_loose;
	  vjets_loose.push_back(iJet->px);
	  vjets_loose.push_back(iJet->py);
	  vjets_loose.push_back(iJet->pz);
	  vjets_loose.push_back(iJet->energy);
	  vvjets_loose.push_back(vjets_loose);

          // Get CSV discriminant, check if passes Med WP 
	  double myCSV = iJet->btagCombinedSecVertex;
	  csvV_loose.push_back(myCSV);
	  if( myCSV>0.679 ) numTag_loose++;
	}


  
	// Add lepton 4Vector quantities to MHT
	mht_px += - leptonV.Px();
	mht_py += - leptonV.Py();


        // Get sorted vector of bTags
	std::vector<double> csvV_temp;
	for( int j=0; j<int(csvV.size()); j++ ) csvV_temp.push_back(csvV[j]);

	std::sort(csvV_temp.begin(),csvV_temp.end());
	std::reverse(csvV_temp.begin(),csvV_temp.end());



        // Intialize Event Variables for Tagged Jets
	double min_btag = 999;
	double min_dR_tag_tag = 99.;
	double sum_dR_tag_tag = 0.;
	double sum_mass_tag_tag = 0.;
	double num_tag_tag = 0;
	double sum_dev_from_ave_disc_btags = 0;
	std::vector<double> good_pfjet_tag_pt;
	double tag_tag_dijet_mass_min_dR = 0;
	double tag_tag_dijet_mass_closest_to_125 = 0;
	double min_dR_tag_lep = 99.;
	double min_tag_tag_dijet_mass_closest_to_125 = 99.;
	double mlb_temp = -1;

        // Loop over selected, tagged jets
	for( BNjetCollection::const_iterator iJet = selectedJets_tag.begin(); iJet != selectedJets_tag.end(); iJet++ ){ 

	  // Get bTag Value
	  double myCSV = iJet->btagCombinedSecVertex;

          // Compute Deviation from Avg bTag
	  double dev = myCSV - ave_btag_disc_btags;
	  sum_dev_from_ave_disc_btags += dev*dev;

	  // Get lowest bTag
	  min_btag = std::min( min_btag, myCSV );

	  // Get this jet's 4Vector
	  TLorentzVector jet0p4;
	  jet0p4.SetPxPyPzE(iJet->px,iJet->py,iJet->pz,iJet->energy);

          // Min dR(lep, tag), M(lep, tag)
	  double dR_lep_tag = jet0p4.DeltaR(leptonV);
	  if( dR_lep_tag<min_dR_tag_lep ){
	    min_dR_tag_lep = dR_lep_tag;
	    TLorentzVector sum_lep_b = leptonV + jet0p4;
	    mlb_temp = sum_lep_b.M();
	  }

          // Second loop over selected, tagged jets
	  for( BNjetCollection::const_iterator jJet = iJet; jJet != selectedJets_tag.end(); jJet++ ){ 

            // Continue if jet is same as above
	    if( iJet==jJet ) continue;

            // Get second jet's 4Vector
 	    TLorentzVector jet1p4;
	    jet1p4.SetPxPyPzE(jJet->px,jJet->py,jJet->pz,jJet->energy);

            // Compute this combination's diJet mass
	    TLorentzVector diJet = jet0p4 + jet1p4;
	    double diJetMass = diJet.M();

	    // Compute min dR(tag, tag)
	    double dR_tag_tag = jet0p4.DeltaR(jet1p4);
	    if( dR_tag_tag<min_dR_tag_tag ){
	      min_dR_tag_tag = dR_tag_tag;
	      tag_tag_dijet_mass_min_dR = diJetMass;
	    }

	    // compute min Mass(tag,tag, closest to M=125)
	    if( fabs(diJetMass-125)<min_tag_tag_dijet_mass_closest_to_125 ){
	      min_tag_tag_dijet_mass_closest_to_125 = fabs(diJetMass-125);
	      tag_tag_dijet_mass_closest_to_125 = diJetMass;
	    }

            // Sum tag, tag quantities
	    sum_dR_tag_tag += dR_tag_tag;
	    num_tag_tag++;

	    sum_mass_tag_tag += diJetMass;

	  }// end jJet loop
	}// end iJet loop


        // Compute deviation from avg discriminant bTags
	double ave_dev_from_ave_disc_btags = ( numtag > 0. ) ? sum_dev_from_ave_disc_btags/numtag : 0.;

	// Compute avg dR(tag,tag)
	double ave_dR_tag_tag = ( num_tag_tag>0 ) ? sum_dR_tag_tag/num_tag_tag : -1;
	// Compute avg M(tag,tag)
	double ave_mass_tag_tag = ( num_tag_tag>0 ) ? sum_mass_tag_tag/num_tag_tag : -1;



        // Intialize Event Variables for Untagged Jets
	double min_dR_untag_untag = 99.;
	double sum_dR_untag_untag = 0.;
	double sum_mass_untag_untag = 0.;
	double num_untag_untag = 0;
	double sum_dev_from_ave_disc_non_btags = 0;

        // Loop over selected, untagged jets
	for( BNjetCollection::const_iterator iJet = selectedJets_untag.begin(); iJet != selectedJets_untag.end(); iJet++ ){ 

	  // Get CSV discriminant
	  double myCSV = iJet->btagCombinedSecVertex;

          // Compute deviation form Avg bTag Disc
	  double dev = myCSV - ave_btag_disc_non_btags;
	  sum_dev_from_ave_disc_non_btags += dev*dev;

          // Get this jet's 4Vect
	  TLorentzVector jet0p4;
	  jet0p4.SetPxPyPzE(iJet->px,iJet->py,iJet->pz,iJet->energy);

	  // Second loop over selected, untagged jets
	  for( BNjetCollection::const_iterator jJet = iJet; jJet != selectedJets_untag.end(); jJet++ ){ 

            // Continue if this jet matches the jet above
	    if( iJet==jJet ) continue;

	    // Get this jet's 4Vector
 	    TLorentzVector jet1p4;
	    jet1p4.SetPxPyPzE(jJet->px,jJet->py,jJet->pz,jJet->energy);

            // Compute min dR(untag, untag)
	    double dR_untag_untag = jet0p4.DeltaR(jet1p4);
	    if( dR_untag_untag<min_dR_untag_untag ) min_dR_untag_untag = dR_untag_untag;
	    // Compute summed untag variables
	    sum_dR_untag_untag += dR_untag_untag;
	    num_untag_untag++;

	    // Compute untagged diJet mass
	    TLorentzVector diJet = jet0p4 + jet1p4;
	    double diJetMass = diJet.M();

	    sum_mass_untag_untag += diJetMass;

	  }// end jJet loop
	}// end iJet loop


	// Compute avg M(untag, untag)
	double ave_mass_untag_untag = ( num_untag_untag>0 ) ? sum_mass_untag_untag/num_untag_untag : -1;


   
        // Sum 4Vectors of all objects in event
	TLorentzVector all_objects = leptonV + metV;
	
	for( int j=0; j<int(jetV.size()); j++ ) all_objects += jetV[j];
	  

	// Compute MHT_
	eve->MHT_[iSys] = sqrt( mht_px*mht_px + mht_py*mht_py );
	eve->MHT_phi_[iSys] = std::atan2(mht_py,mht_px);


	// Comput Ht+Lepton
	//double ht_lep = HT_[iSys] + leptonV.Pt();


        // Compute angular quantities
	beanHelper.getSp(leptonV,metV,jetV,eve->aplanarity_[iSys],eve->sphericity_[iSys]);
	beanHelper.getFox(jetV,eve->h0_[iSys],eve->h1_[iSys],eve->h2_[iSys],eve->h3_[iSys],eve->h4_[iSys]);

	// Intialize 4Vectors, to use in Best Higgs Mass function
	TLorentzVector mydummyguy;
	mydummyguy.SetPxPyPzE(0,0,0,0);
	TLorentzVector bjet1_tmp = mydummyguy;
	TLorentzVector bjet2_tmp = mydummyguy;

        // Get Best Higgs Mass (X^2 method)
	eve->best_higgs_mass_[iSys] = getBestHiggsMass(leptonV, metV, jetV, csvV, eve->minChi2_[iSys], eve->dRbb_[iSys], bjet1_tmp, bjet2_tmp, jetV_loose, csvV_loose);

        double chiSq2=10000;
        eve->best_higgs_mass_old_[iSys] = getBestHiggsMass_old(leptonV, metV, jetV, csvV, chiSq2);


        eve->minChi2_bjet1_px_[iSys] = bjet1_tmp.Px();
        eve->minChi2_bjet1_py_[iSys] = bjet1_tmp.Py();
        eve->minChi2_bjet1_pz_[iSys] = bjet1_tmp.Pz();
        eve->minChi2_bjet1_E_[iSys]  = bjet1_tmp.E();

        eve->minChi2_bjet2_px_[iSys] = bjet2_tmp.Px();
        eve->minChi2_bjet2_py_[iSys] = bjet2_tmp.Py();
        eve->minChi2_bjet2_pz_[iSys] = bjet2_tmp.Pz();
        eve->minChi2_bjet2_E_[iSys]  = bjet2_tmp.E();


	// nJets/Tags
	eve->numJets_float_[iSys] = numJet;
	eve->numTags_float_[iSys] = numTag;


	// jet1-4 pT
	eve->first_jet_pt_[iSys]  = selectedJets.at(0).pt;
	eve->second_jet_pt_[iSys] = selectedJets.at(1).pt;
	eve->third_jet_pt_[iSys]  = selectedJets.at(2).pt;
	eve->fourth_jet_pt_[iSys] = selectedJets.at(3).pt;

        // MET
	eve->MET_[iSys]      = correctedMET.pt;
	eve->uMET_[iSys]     = pfmet->Upt;
	eve->MET_phi_[iSys]  = correctedMET.phi;
	eve->uMET_phi_[iSys] = pfmet->Uphi;


	// Multi-object Quantities
	eve->all_sum_pt_[iSys]                   = eve->HT_[iSys]+leptonV.Pt();
	eve->all_sum_pt_with_met_[iSys]          = eve->HT_[iSys]+leptonV.Pt()+correctedMET.pt;
	eve->M3_[iSys]                           = max_m3;
	eve->M3_1tag_[iSys]                      = max_m3_1tag;
	eve->Mlb_[iSys]     = mlb_temp;
	eve->invariant_mass_of_everything_[iSys] = all_objects.M();


	// dR Quantities
	eve->min_dr_tagged_jets_[iSys]             = min_dR_tag_tag;
	eve->avg_dr_tagged_jets_[iSys]             = ave_dR_tag_tag;
	eve->dr_between_lep_and_closest_jet_[iSys] = min_dR_lep_jet;


	// diJet Quantities 
	eve->avg_tagged_dijet_mass_[iSys]     = ave_mass_tag_tag;
        eve->avg_untagged_dijet_mass_[iSys]   = ave_mass_untag_untag;
	eve->closest_tagged_dijet_mass_[iSys] = tag_tag_dijet_mass_min_dR;
	eve->tagged_dijet_mass_closest_to_125_[iSys] = tag_tag_dijet_mass_closest_to_125;
       
	
        // bTag Quantities
        eve->avg_btag_disc_btags_[iSys]     = ave_btag_disc_btags; 
	eve->avg_btag_disc_non_btags_[iSys] = ave_btag_disc_non_btags; 
        eve->dev_from_avg_disc_btags_[iSys] = ave_dev_from_ave_disc_btags;
	eve->first_highest_btag_[iSys]      = csvV_temp[0];
	eve->second_highest_btag_[iSys]     = csvV_temp[1];
	eve->third_highest_btag_[iSys]      = csvV_temp[2];
	eve->fourth_highest_btag_[iSys]     = csvV_temp[3];
        eve->lowest_btag_[iSys]             = min_btag;


        // Jet 1 Quantities
	eve->jet_1_chargedHadronEnergyFraction_[iSys] = selectedJets.at(0).chargedHadronEnergyFraction;
	eve->jet_1_neutralHadronEnergyFraction_[iSys] = selectedJets.at(0).neutralHadronEnergyFraction;
	eve->jet_1_chargedEmEnergyFraction_[iSys] = selectedJets.at(0).chargedEmEnergyFraction;
	eve->jet_1_neutralEmEnergyFraction_[iSys] = selectedJets.at(0).neutralEmEnergyFraction;
	eve->jet_1_chargedMultiplicity_[iSys] = selectedJets.at(0).chargedMultiplicity;
	eve->jet_1_neutralMultiplicity_[iSys] = selectedJets.at(0).neutralMultiplicity;
	eve->jet_1_nconstituents_[iSys] = selectedJets.at(0).nconstituents;


        // Jet 2 Quantities
	eve->jet_2_chargedHadronEnergyFraction_[iSys] = selectedJets.at(1).chargedHadronEnergyFraction;
	eve->jet_2_neutralHadronEnergyFraction_[iSys] = selectedJets.at(1).neutralHadronEnergyFraction;
	eve->jet_2_chargedEmEnergyFraction_[iSys] = selectedJets.at(1).chargedEmEnergyFraction;
	eve->jet_2_neutralEmEnergyFraction_[iSys] = selectedJets.at(1).neutralEmEnergyFraction;
	eve->jet_2_chargedMultiplicity_[iSys] = selectedJets.at(1).chargedMultiplicity;
	eve->jet_2_neutralMultiplicity_[iSys] = selectedJets.at(1).neutralMultiplicity;
	eve->jet_2_nconstituents_[iSys] = selectedJets.at(1).nconstituents;


        // Jet 3 Quantities
	eve->jet_3_chargedHadronEnergyFraction_[iSys] = selectedJets.at(2).chargedHadronEnergyFraction;
	eve->jet_3_neutralHadronEnergyFraction_[iSys] = selectedJets.at(2).neutralHadronEnergyFraction;
	eve->jet_3_chargedEmEnergyFraction_[iSys] = selectedJets.at(2).chargedEmEnergyFraction;
	eve->jet_3_neutralEmEnergyFraction_[iSys] = selectedJets.at(2).neutralEmEnergyFraction;
	eve->jet_3_chargedMultiplicity_[iSys] = selectedJets.at(2).chargedMultiplicity;
	eve->jet_3_neutralMultiplicity_[iSys] = selectedJets.at(2).neutralMultiplicity;
	eve->jet_3_nconstituents_[iSys] = selectedJets.at(2).nconstituents;


        // Jet 4 Quantities
 	eve->jet_4_chargedHadronEnergyFraction_[iSys] = selectedJets.at(3).chargedHadronEnergyFraction;
	eve->jet_4_neutralHadronEnergyFraction_[iSys] = selectedJets.at(3).neutralHadronEnergyFraction;
	eve->jet_4_chargedEmEnergyFraction_[iSys] = selectedJets.at(3).chargedEmEnergyFraction;
	eve->jet_4_neutralEmEnergyFraction_[iSys] = selectedJets.at(3).neutralEmEnergyFraction;
	eve->jet_4_chargedMultiplicity_[iSys] = selectedJets.at(3).chargedMultiplicity;
	eve->jet_4_neutralMultiplicity_[iSys] = selectedJets.at(3).neutralMultiplicity;
	eve->jet_4_nconstituents_[iSys] = selectedJets.at(3).nconstituents;

	eve->minChi2_bjet1_px_[iSys] = bjet1_tmp.Px();
	eve->minChi2_bjet1_py_[iSys] = bjet1_tmp.Py();
	eve->minChi2_bjet1_pz_[iSys] = bjet1_tmp.Pz();
	eve->minChi2_bjet1_E_[iSys]  = bjet1_tmp.E();

	eve->minChi2_bjet2_px_[iSys] = bjet2_tmp.Px();
	eve->minChi2_bjet2_py_[iSys] = bjet2_tmp.Py();
	eve->minChi2_bjet2_pz_[iSys] = bjet2_tmp.Pz();
	eve->minChi2_bjet2_E_[iSys]  = bjet2_tmp.E();

        eve->lepton_TLV_[iSys]   = vlepton;
        eve->lepton_genId_[iSys]            = lepton_genId;
        eve->lepton_genParentId_[iSys]      = lepton_genParentId;
        eve->lepton_genGrandParentId_[iSys] = lepton_genGrandParentId;

	eve->jet_vect_TLV_[iSys] = vvjets;
	eve->jet_CSV_[iSys]      = csvV;
	eve->jet_flavour_[iSys]          = jet_flavour_vect;
	eve->jet_genId_[iSys]            = jet_genId_vect;
	eve->jet_genParentId_[iSys]      = jet_genParentId_vect;
	eve->jet_genGrandParentId_[iSys] = jet_genGrandParentId_vect;

	// PU quantities
	
	eve->jet_PUid_full_loose_[iSys]  = jetPUid_full_loose_vect;
	eve->jet_PUid_full_medium_[iSys] = jetPUid_full_medium_vect;
	eve->jet_PUid_full_tight_[iSys]  = jetPUid_full_tight_vect;

	eve->jet_PUid_simple_loose_[iSys]  = jetPUid_simple_loose_vect;
	eve->jet_PUid_simple_medium_[iSys] = jetPUid_simple_medium_vect;
	eve->jet_PUid_simple_tight_[iSys]  = jetPUid_simple_tight_vect;

	eve->jet_PUmva_full_[iSys]   = jetPUmva_full_vect;
	eve->jet_PUmva_simple_[iSys] = jetPUmva_simple_vect;

	eve->jet_dR2Mean_[iSys]  = dR2Mean_vect;
	eve->jet_dRMean_[iSys]   = dRMean_vect;
	eve->jet_frac01_[iSys]   = frac01_vect;
	eve->jet_frac02_[iSys]   = frac02_vect;
	eve->jet_frac03_[iSys]   = frac03_vect;
	eve->jet_beta_[iSys]     = beta_vect;
	eve->jet_betaStar_[iSys] = betaStar_vect;
	eve->jet_leadCandDistFromPV_[iSys] = leadCandDistFromPV_vect;
	eve->jet_minPVDist_[iSys] = minPVDist_vect;
	
	eve->numJets_PUid_full_loose_float_[iSys]  = numJets_PUid_full_loose;
	eve->numJets_PUid_full_medium_float_[iSys] = numJets_PUid_full_medium;
	eve->numJets_PUid_full_tight_float_[iSys]  = numJets_PUid_full_tight;

	eve->numJets_PUid_simple_loose_float_[iSys]  = numJets_PUid_simple_loose;
	eve->numJets_PUid_simple_medium_float_[iSys] = numJets_PUid_simple_medium;
	eve->numJets_PUid_simple_tight_float_[iSys]  = numJets_PUid_simple_tight;

	eve->numTags_PUid_full_loose_float_[iSys]  = numTags_PUid_full_loose;
	eve->numTags_PUid_full_medium_float_[iSys] = numTags_PUid_full_medium;
	eve->numTags_PUid_full_tight_float_[iSys]  = numTags_PUid_full_tight;

	eve->numTags_PUid_simple_loose_float_[iSys]  = numTags_PUid_simple_loose;
	eve->numTags_PUid_simple_medium_float_[iSys] = numTags_PUid_simple_medium;
	eve->numTags_PUid_simple_tight_float_[iSys]  = numTags_PUid_simple_tight;


	// loose jets

	// nJets/Tags
	eve->numJets_loose_float_[iSys] = numJet_loose;
	eve->numTags_loose_float_[iSys] = numTag_loose;

	eve->jet_loose_vect_TLV_[iSys] = vvjets_loose;
	eve->jet_loose_CSV_[iSys]      = csvV_loose;
	eve->jet_loose_flavour_[iSys]  = jet_flavour_vect_loose;


      } // end loop over systematics


      //
      // Fill tree if pass full selection
      //
      if( hasNumJetNumTag ){
	worldTree->Fill();

	if( hasNumJetNumTag_noSys ){
	  nevents_ll_nj_nt++;
	  nevents_ll_nj_nt_wgt += totalWgt;
	  if( isMuon ){
	    nevents_ll_nj_nt_mu++;
	    nevents_ll_nj_nt_mu_wgt += totalWgt;
	  }
	  else{
	    nevents_ll_nj_nt_ele++;
	    nevents_ll_nj_nt_ele_wgt += totalWgt;
	  }
	}
      }


    } // end loop over events


  }// end try

  catch(std::exception& e) {
    std::cerr << " ==> caught exception " << e.what() << std::endl;
    //continue;
  }


  //
  // Print Event Report
  //
  std::cout << " *********************************************************** " << std::endl;
  std::cout << "   Number of Events Processed  = " << nevents << std::endl;
  std::cout << "   Number of Events Processed (wgt) = " << nevents_wgt << std::endl;
  std::cout << "   Number of Events Pass Trigger  = " << nevents_clean << " (ele = " << nevents_clean_ele << ", mu = " << nevents_clean_mu << ")" << std::endl;
  std::cout << "   Number of Events Pass Trigger (wgt) = " << nevents_clean_wgt << " (ele = " << nevents_clean_ele_wgt << ", mu = " << nevents_clean_mu_wgt << ")" << std::endl;
  std::cout << "   Number of Events with single lepton = " << nevents_ll << " (ele = " << nevents_ll_ele << ", mu = " << nevents_ll_mu << ")" << std::endl;
  std::cout << "   Number of Events with single lepton (wgt) = " << nevents_ll_wgt << " (ele = " << nevents_ll_ele_wgt << ", mu = " << nevents_ll_mu_wgt << ")" << std::endl;
  std::cout << "   Number of Events with single lepton, >=4 jets = " << nevents_ll_nj << " (ele = " << nevents_ll_nj_ele << ", mu = " << nevents_ll_nj_mu << ")" << std::endl;
  std::cout << "   Number of Events with single lepton, >=4 jets (wgt) = " << nevents_ll_nj_wgt << " (ele = " << nevents_ll_nj_ele_wgt << ", mu = " << nevents_ll_nj_mu_wgt << ")" << std::endl;
  std::cout << "   Number of Events with single lepton, >=4 jets, >=2 tags = " << nevents_ll_nj_nt << " (ele = " << nevents_ll_nj_nt_ele << ", mu = " << nevents_ll_nj_nt_mu << ")" << std::endl;
  std::cout << "   Number of Events with single lepton, >=4 jets, >=2 tags (wgt) = " << nevents_ll_nj_nt_wgt << " (ele = " << nevents_ll_nj_nt_ele_wgt << ", mu = " << nevents_ll_nj_nt_mu_wgt << ")" << std::endl;
  std::cout << " *********************************************************** " << std::endl;


  treefile.Write();
  treefile.Close();

  std::cout << " Done! " << std::endl;

}



/////////////////////////

double getBestHiggsMass(TLorentzVector lepton, TLorentzVector met, vecTLorentzVector jets, vdouble btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2, vecTLorentzVector loose_jets, vdouble loose_btag)
{

  if( jets.size()<6 && loose_jets.size()>0 ){
    jets.push_back( loose_jets[0] );
    btag.push_back( loose_btag[0] );
  }

  int nJets = int(jets.size());

  double chi_top_lep=10000;
  double chi_top_had=10000;
  //double chi_W_lep=10000; //isn't really used
  double chi_W_had=10000;

  minChi = 1000000;
  dRbb = 1000000;
  double btagCut = 0.679;
  double W_mass = 80.0;
  double top_mass = 172.5;
  //double H_mass=120.0;

  // updated 8/22/2012 from J. Timcheck
  //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttH
  double sigma_hadW   = 12.77;
  double sigma_hadTop = 18.9;
  double sigma_lepTop = 32.91;

  // //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttH
  // double sigma_hadW   = 12.59;
  // double sigma_hadTop = 19.9;
  // double sigma_lepTop = 39.05;

  //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttJets
  /*double sigma_hadW		= 12.72,
    sigma_hadTop	= 18.12,
    sigma_lepTop	= 38.72;
  */

  double metPz[2];
  double chi=999999;

  //stuff to find:
  double higgs_mass_high_energy=0;

  int nBtags = 0;
  for(int i=0;i<nJets;i++){
    if(btag[i]>btagCut) nBtags++;
  }

  int nUntags = nJets-nBtags;

  double lowest_btag = 99.;
  double second_lowest_btag = 999.;
  int ind_lowest_btag = 999;
  int ind_second_lowest_btag = 999;

  if( nJets>=6 && nBtags>=4 ){
    if( nUntags<2 ){
      for(int i=0;i<nJets;i++){
	if( btag[i]<lowest_btag ){
	  second_lowest_btag = lowest_btag;
	  ind_second_lowest_btag = ind_lowest_btag;

	  lowest_btag = btag[i];
	  ind_lowest_btag = i;
	}
	else if( btag[i]<second_lowest_btag ){
	  second_lowest_btag = btag[i];
	  ind_second_lowest_btag = i;
	}
      }
    }
  }


  //Handle 6j3t.
  int ind_promoted_btag = 999;

  if( nJets>=6 && nBtags==3 ){
    for(int i=0;i<nJets;i++){
      int rank = 0;
      for(int j=0;j<nJets;j++){
	if( btag[j] > btag[i] ){
	  rank++;
	}
      }
      if( rank == 3 ) ind_promoted_btag = i;
    }
  }

  // First get the neutrino z
  double energyLep = lepton.E();
  double a = (W_mass*W_mass/(2.0*energyLep)) + (lepton.Px()*met.Px() + lepton.Py()*met.Py())/energyLep;
  double radical = (2.0*lepton.Pz()*a/energyLep)*(2.0*lepton.Pz()*a/energyLep);
  radical = radical - 4.0*(1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep))*(met.Px()*met.Px() + met.Py()*met.Py()- a*a);
  if (radical < 0.0) radical = 0.0;
  metPz[0] = (lepton.Pz()*a/energyLep) + 0.5*sqrt(radical);
  metPz[0] = metPz[0] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));
  metPz[1] = (lepton.Pz()*a/energyLep) - 0.5*sqrt(radical);
  metPz[1] = metPz[1] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));


  // Loop over all jets, both Pz, calcaulte chi-square
  TLorentzVector metNew;
  for( int ipznu=0; ipznu<2; ipznu++ ){
    metNew.SetXYZM(met.Px(),met.Py(),metPz[ipznu],0.0); //neutrino has mass 0
    //with b-tag info
    if( (nJets>=6 && nBtags>=4) || (nJets>=6 && nBtags==3) ){
      vecTLorentzVector not_b_tagged,b_tagged;
      //fill not_b_tagged and b_tagged
      for( int i=0;i<nJets;i++ ){
	if( (btag[i]>btagCut && i!=ind_second_lowest_btag && i!=ind_lowest_btag) || (i==ind_promoted_btag) ) b_tagged.push_back(jets[i]);
	else not_b_tagged.push_back(jets[i]);
      }
      //first make possible t_lep's with b-tagged jets (includes making W_lep)
      for( int i=0; i<int(b_tagged.size()); i++ ){
	TLorentzVector W_lep=metNew+lepton; //used for histogram drawing only
	TLorentzVector top_lep=metNew+lepton+b_tagged.at(i);
	chi_top_lep=pow((top_lep.M()-top_mass)/sigma_lepTop,2);
	//next make possible W_had's with not b-tagged jets
	for( int j=0; j<int(not_b_tagged.size()); j++ ){
	  for( int k=0; k<int(not_b_tagged.size()); k++ ){
	    if( j!=k ){
	      TLorentzVector W_had=not_b_tagged.at(j)+not_b_tagged.at(k);
	      chi_W_had=pow((W_had.M()-W_mass)/sigma_hadW,2);
	      //now make possible top_had's (using the W_had + some b-tagged jet)
	      for( int l=0; l<int(b_tagged.size()); l++ ){
		if( l!=i ){
		  TLorentzVector top_had=W_had+b_tagged.at(l);
		  chi_top_had=pow((top_had.M()-top_mass)/sigma_hadTop,2);
		  chi=chi_top_lep+chi_W_had+chi_top_had;
		  //accept the lowest chi
		  if( chi<minChi ){
		    minChi=chi;
		    //pick the other two b's that have the highest et (energy in transverse plane) as higgs mass constituents
		    TLorentzVector H2;
		    int numH2Constituents=0;
		    TLorentzVector bBest[2];
		    for( int m=0; m<int(b_tagged.size()); m++ ){
		      if( m!=i && m!=l && numH2Constituents<2 ){
			bBest[numH2Constituents] = b_tagged.at(m);
			numH2Constituents++;
			H2+=b_tagged.at(m);
		      }
		    }
		    dRbb = bBest[0].DeltaR( bBest[1] );
		    higgs_mass_high_energy=H2.M();
		    bjet1 = bBest[0];
		    bjet2 = bBest[1];
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  return higgs_mass_high_energy;
}



//=============================================================================
//
double getBestHiggsMass_old(TLorentzVector lepton, TLorentzVector met, vecTLorentzVector jets, vdouble btag, double &minChi)
{

  int nJets = int(jets.size());

  double chi_top_lep=10000;
  double chi_top_had=10000;
  //double chi_W_lep=10000; //isn't really used
  double chi_W_had=10000;

  minChi = 1000000;
  double btagCut = 0.679;
  double W_mass = 80.0;
  double top_mass = 172.5;
  //double H_mass=120.0;

  //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttH
  double sigma_hadW 	= 12.59,
    sigma_hadTop 	= 19.9,
    sigma_lepTop	= 39.05;

  //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttJets
  /*double sigma_hadW		= 12.72,
    sigma_hadTop	= 18.12,
    sigma_lepTop	= 38.72;
  */

  double metPz[2];
  double chi=999999;
  int nBtags=0;

  //stuff to find:
  double higgs_mass_high_energy=0;

  //get nBtags
  for(int i=0;i<nJets;i++)
    if(btag[i]>btagCut)
      nBtags++;
  // First get the neutrino Pz
  double energyLep = lepton.E();
  double a = (W_mass*W_mass/(2.0*energyLep)) + (lepton.Px()*met.Px() + lepton.Py()*met.Py())/energyLep;
  double radical = (2.0*lepton.Pz()*a/energyLep)*(2.0*lepton.Pz()*a/energyLep);
  radical = radical - 4.0*(1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep))*(met.Px()*met.Px() + met.Py()*met.Py()- a*a);
  if (radical < 0.0) radical = 0.0;
  metPz[0] = (lepton.Pz()*a/energyLep) + 0.5*sqrt(radical);
  metPz[0] = metPz[0] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));
  metPz[1] = (lepton.Pz()*a/energyLep) - 0.5*sqrt(radical);
  metPz[1] = metPz[1] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));

  // Loop over all jets, both Pz, calcaulte chi-square
  TLorentzVector metNew;
  for (int ipznu=0; ipznu<2; ipznu++) 
    {
      metNew.SetXYZM(met.Px(),met.Py(),metPz[ipznu],0.0); //neutrino has mass 0
      //with b-tag info
      if(nJets>=6&&nBtags>=4)
	{
	  vecTLorentzVector not_b_tagged,b_tagged;
	  //fill not_b_tagged and b_tagged
	  for(int i=0;i<nJets;i++)
	    {
	      if(btag[i]>btagCut) b_tagged.push_back(jets[i]);
	      else not_b_tagged.push_back(jets[i]);
	    }
	  //first make possible t_lep's with b-tagged jets (includes making W_lep)
	  for(int i=0;i<(int)b_tagged.size();i++)
	    {
	      TLorentzVector W_lep=metNew+lepton; //used for histogram drawing only
	      TLorentzVector top_lep=metNew+lepton+b_tagged.at(i);
	      chi_top_lep=pow((top_lep.M()-top_mass)/sigma_lepTop,2);
	      //next make possible W_had's with not b-tagged jets
	      for(int j=0;j<(int)not_b_tagged.size();j++)
		{
		  for(int k=0;k<(int)not_b_tagged.size();k++)
		    {
		      if(j!=k)
			{
			  TLorentzVector W_had=not_b_tagged.at(j)+not_b_tagged.at(k);
			  chi_W_had=pow((W_had.M()-W_mass)/sigma_hadW,2);
			  //now make possible top_had's (using the W_had + some b-tagged jet)
			  for(int l=0;l<(int)b_tagged.size();l++)
			    {
			      if(l!=i)
				{
				  TLorentzVector top_had=W_had+b_tagged.at(l);
				  chi_top_had=pow((top_had.M()-top_mass)/sigma_hadTop,2);
				  chi=chi_top_lep+chi_W_had+chi_top_had;
				  //accept the lowest chi
				  if(chi<minChi)
				    {
				      minChi=chi;
				      //pick the other two b's that have the highest et (energy in transverse plane) as higgs mass constituents
				      TLorentzVector H2;
				      int numH2Constituents=0;
				      for(int m=0;m<(int)b_tagged.size();m++)
					{
					  if(m!=i && m!=l && numH2Constituents<2)
					    {
					      numH2Constituents++;
					      H2+=b_tagged.at(m);
					    }
					}
				      higgs_mass_high_energy=H2.M();
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
  return higgs_mass_high_energy;
}

