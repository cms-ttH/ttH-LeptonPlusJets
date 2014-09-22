// -*- C++ -*-
//
// Package:    ttH-LeptonPlusJets/YggdrasilTreeMaker
// Class:      YggdrasilTreeMaker
// 
/**\class YggdrasilTreeMaker YggdrasilTreeMaker.cc ttH-LeptonPlusJets/YggdrasilTreeMaker/plugins/YggdrasilTreeMaker.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Darren Puigh
//         Created:  Fri, 12 Sep 2014 16:58:04 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Lepton.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/strbitset.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "MiniAOD/MiniAODHelper/interface/MiniAODHelper.h"

#include "ttH-LeptonPlusJets/AnalysisCode/interface/YggdrasilEventVars.h"

//
// class declaration
//

class YggdrasilTreeMaker : public edm::EDAnalyzer {
   public:
      explicit YggdrasilTreeMaker(const edm::ParameterSet&);
      ~YggdrasilTreeMaker();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------

  edm::EDGetTokenT <edm::TriggerResults> triggerResultsToken;
  edm::EDGetTokenT <edm::TriggerResults> filterResultsToken;

  edm::EDGetTokenT <reco::VertexCollection> vertexToken;
  edm::EDGetTokenT <pat::ElectronCollection> electronToken;
  edm::EDGetTokenT <pat::MuonCollection> muonToken;
  edm::EDGetTokenT <pat::JetCollection> jetToken;
  edm::EDGetTokenT <pat::METCollection> metToken;

  edm::EDGetTokenT <pat::PackedCandidateCollection> packedpfToken;

  edm::EDGetTokenT <reco::BeamSpot> beamspotToken;
  edm::EDGetTokenT <double> rhoToken;
  edm::EDGetTokenT <reco::GenParticleCollection> mcparicleToken;
  edm::EDGetTokenT <std::vector< PileupSummaryInfo > > puInfoToken;


  bool verbose_;

  bool isLJ_;

  int insample_;

  std::string mySample_sampleName_;
  double mySample_xSec_;
  double mySample_nGen_;
  double intLumi_;

  std::string hltTag;
  std::string filterTag;

  int neventsFillTree[rNumSys];

  int nevents;
  double nevents_wgt;

  int nevents_clean;
  double nevents_clean_wgt;

  int nevents_1l, nevents_1l_ele, nevents_1l_mu;
  double nevents_1l_wgt, nevents_1l_ele_wgt, nevents_1l_mu_wgt;

  int nevents_1l_4j, nevents_1l_4j_ele, nevents_1l_4j_mu;
  double nevents_1l_4j_wgt, nevents_1l_4j_ele_wgt, nevents_1l_4j_mu_wgt;

  int nevents_1l_4j_2t, nevents_1l_4j_2t_ele, nevents_1l_4j_2t_mu;
  double nevents_1l_4j_2t_wgt, nevents_1l_4j_2t_ele_wgt, nevents_1l_4j_2t_mu_wgt;

  int nevents_2l, nevents_2l_2e, nevents_2l_2m, nevents_2l_em;
  double nevents_2l_wgt, nevents_2l_2e_wgt, nevents_2l_2m_wgt, nevents_2l_em_wgt;

  int nevents_2l_2j, nevents_2l_2j_2e, nevents_2l_2j_2m, nevents_2l_2j_em;
  double nevents_2l_2j_wgt, nevents_2l_2j_2e_wgt, nevents_2l_2j_2m_wgt, nevents_2l_2j_em_wgt;

  int nevents_2l_2j_2t, nevents_2l_2j_2t_2e, nevents_2l_2j_2t_2m, nevents_2l_2j_2t_em;
  double nevents_2l_2j_2t_wgt, nevents_2l_2j_2t_2e_wgt, nevents_2l_2j_2t_2m_wgt, nevents_2l_2j_2t_em_wgt;

  TTree *worldTree;
  yggdrasilEventVars *eve; 


  MiniAODHelper miniAODhelper;
};

//
// constants, enums and typedefs
//
typedef std::vector<std::vector<double> >      vvdouble;
typedef std::vector<std::vector<std::string> > vvstring;
typedef std::vector<double>                    vdouble;
typedef std::vector<string>                    vstring;
typedef std::vector<bool>                      vbool;
typedef std::vector<int>                       vint;
typedef std::vector< TLorentzVector >          vecTLorentzVector;

//
// static data member definitions
//

//
// constructors and destructor
//
YggdrasilTreeMaker::YggdrasilTreeMaker(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  verbose_ = false;
  isLJ_ = true;

  hltTag = "HLT";
  filterTag = "PAT";

  triggerResultsToken = consumes <edm::TriggerResults> (edm::InputTag(std::string("TriggerResults"), std::string(""), hltTag));
  filterResultsToken = consumes <edm::TriggerResults> (edm::InputTag(std::string("TriggerResults"), std::string(""), filterTag));

  vertexToken = consumes <reco::VertexCollection> (edm::InputTag(std::string("offlineSlimmedPrimaryVertices")));
  electronToken = consumes <pat::ElectronCollection> (edm::InputTag(std::string("slimmedElectrons")));
  muonToken = consumes <pat::MuonCollection> (edm::InputTag(std::string("slimmedMuons")));
  jetToken = consumes <pat::JetCollection> (edm::InputTag(std::string("slimmedJets")));
  metToken = consumes <pat::METCollection> (edm::InputTag(std::string("slimmedMETs")));

  packedpfToken = consumes <pat::PackedCandidateCollection> (edm::InputTag(std::string("packedPFCandidates")));

  beamspotToken = consumes <reco::BeamSpot> (edm::InputTag(std::string("offlineBeamSpot")));
  rhoToken = consumes <double> (edm::InputTag(std::string("fixedGridRhoAll")));
  mcparicleToken = consumes <reco::GenParticleCollection> (edm::InputTag(std::string("prunedGenParticles")));
  puInfoToken = consumes <std::vector< PileupSummaryInfo > > (edm::InputTag(std::string("addPileupInfo")));


  edm::Service<TFileService> fs_;
  worldTree = fs_->make<TTree>("worldTree", "worldTree");
  eve=0; 
  worldTree->Branch("eve.", "yggdrasilEventVars", &eve, 8000, 1);


  for( int i=0; i<rNumSys; i++ ) neventsFillTree[i] = 0;

  nevents=0;
  nevents_wgt=0;

  nevents_clean=0;
  nevents_clean_wgt=0;

  nevents_1l=0, nevents_1l_ele=0, nevents_1l_mu=0;
  nevents_1l_wgt=0, nevents_1l_ele_wgt=0, nevents_1l_mu_wgt=0;

  nevents_1l_4j=0, nevents_1l_4j_ele=0, nevents_1l_4j_mu=0;
  nevents_1l_4j_wgt=0, nevents_1l_4j_ele_wgt=0, nevents_1l_4j_mu_wgt=0;

  nevents_1l_4j_2t=0, nevents_1l_4j_2t_ele=0, nevents_1l_4j_2t_mu=0;
  nevents_1l_4j_2t_wgt=0, nevents_1l_4j_2t_ele_wgt=0, nevents_1l_4j_2t_mu_wgt=0;

  nevents_2l=0, nevents_2l_2e=0, nevents_2l_2m=0, nevents_2l_em=0;
  nevents_2l_wgt=0, nevents_2l_2e_wgt=0, nevents_2l_2m_wgt=0, nevents_2l_em_wgt=0;

  nevents_2l_2j=0, nevents_2l_2j_2e=0, nevents_2l_2j_2m=0, nevents_2l_2j_em=0;
  nevents_2l_2j_wgt=0, nevents_2l_2j_2e_wgt=0, nevents_2l_2j_2m_wgt=0, nevents_2l_2j_em_wgt=0;

  nevents_2l_2j_2t=0, nevents_2l_2j_2t_2e=0, nevents_2l_2j_2t_2m=0, nevents_2l_2j_2t_em=0;
  nevents_2l_2j_2t_wgt=0, nevents_2l_2j_2t_2e_wgt=0, nevents_2l_2j_2t_2m_wgt=0, nevents_2l_2j_2t_em_wgt=0;


  std::string era = "2012_53x";
  insample_ = 2500;

  mySample_sampleName_ = "TTJets_MSDecaysCKM_central_Tune4C_13TeV_madgraph_PU20bx25_POSTLS170_V5_v1";
  mySample_xSec_ = 689.1;
  mySample_nGen_ = 25474122;
  intLumi_ = 20000;

  analysisType::analysisType iAnalysisType = analysisType::LJ;
  bool isData = ( insample_<0 );

  miniAODhelper.SetUp(era, insample_, iAnalysisType, isData);

  miniAODhelper.SetJetCorrectorUncertainty();

}


YggdrasilTreeMaker::~YggdrasilTreeMaker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
YggdrasilTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;


  double minTightLeptonPt = ( isLJ_ ) ? 30. : 20.;


  edm::Handle<reco::VertexCollection> vtxHandle;
  iEvent.getByToken(vertexToken,vtxHandle);
  reco::VertexCollection vtxs = *vtxHandle;

  edm::Handle<pat::ElectronCollection> electrons;
  iEvent.getByToken(electronToken,electrons);

  edm::Handle<pat::MuonCollection> muons;
  iEvent.getByToken(muonToken,muons);

  edm::Handle<pat::JetCollection> pfjets;
  iEvent.getByToken(jetToken,pfjets);

  edm::Handle<pat::METCollection> pfmet;
  iEvent.getByToken(metToken,pfmet);

  edm::Handle<pat::PackedCandidateCollection> packedPFcands;
  iEvent.getByToken(packedpfToken,packedPFcands);


  edm::Handle<reco::BeamSpot> bsHandle;
  iEvent.getByToken(beamspotToken,bsHandle);

  edm::Handle<reco::GenParticleCollection> mcparticles;
  iEvent.getByToken(mcparicleToken,mcparticles);

  edm::Handle<double> rhoHandle;
  iEvent.getByToken(rhoToken,rhoHandle);

  edm::Handle<std::vector< PileupSummaryInfo > > PupInfo;
  iEvent.getByToken(puInfoToken,PupInfo);



  math::XYZPoint beamSpotPosition;
  beamSpotPosition.SetCoordinates(0,0,0);
  double BSx=0,BSy=0,BSz=0;
  if( (bsHandle.isValid()) ){
    reco::BeamSpot bs = *bsHandle;
    BSx = bs.x0();
    BSy = bs.y0();
    BSz = bs.z0();
    beamSpotPosition = bsHandle->position();
  }


  if( verbose_ ) printf("\t BeamSpot: x = %.2f,\t y = %.2f,\t z = %.2f \n", BSx, BSy, BSz );

  int numpv=0;
  reco::Vertex vertex;
  if( vtxHandle.isValid() ){
    for( reco::VertexCollection::const_iterator vtx = vtxs.begin(); vtx!=vtxs.end(); ++vtx ){
      bool isGood = ( !(vtx->isFake()) &&
		      (vtx->ndof() >= 4.0) &&
		      (abs(vtx->z()) <= 24.0) &&
		      (abs(vtx->position().Rho()) <= 2.0) 
		      );

      if( !isGood ) continue;

      if( numpv==0 ) vertex = (*vtx);

      numpv++;
    }
  }

  if( numpv>0 ) miniAODhelper.SetVertex(vertex);

  // double numTruePV = -1;
  // if( (PupInfo.isValid()) ){
  //   for( std::vector<PileupSummaryInfo>::const_iterator PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI ) {
  //     int BX = PVI->getBunchCrossing();
  //     if( BX==0 ) numTruePV = PVI->getTrueNumInteractions();
  //   }
  // }


  double wgt_lumi = 1.;
  if( insample_>=0 ) wgt_lumi = mySample_xSec_ * intLumi_ *1./ mySample_nGen_;

  // Increment event counter
  nevents++;
  nevents_wgt+=wgt_lumi;

  // Number of events after filtering, triggers, etc. (now nothing)
  nevents_clean++;
  nevents_clean_wgt+=wgt_lumi;

  int run  = iEvent.id().run();
  int lumi = iEvent.luminosityBlock();
  long evt = iEvent.id().event();


  const JetCorrector* corrector = JetCorrector::getJetCorrector( "ak4PFchsL1L2L3", iSetup );   //Get the jet corrector from the event setup

  miniAODhelper.SetJetCorrector(corrector);


  /////////
  ///
  /// Electrons
  ///
  ////////
  std::vector<pat::Electron> selectedElectrons_tight = miniAODhelper.GetSelectedElectrons( *electrons, minTightLeptonPt, electronID::electronTight );
  std::vector<pat::Electron> selectedElectrons_loose = miniAODhelper.GetSelectedElectrons( *electrons, 10., electronID::electronLoose );

  int numTightElectrons = int(selectedElectrons_tight.size());
  int numLooseElectrons = int(selectedElectrons_loose.size());// - numTightElectrons;


  /////////
  ///
  /// Muons
  ///
  ////////
  std::vector<pat::Muon> selectedMuons_tight = miniAODhelper.GetSelectedMuons( *muons, minTightLeptonPt, muonID::muonTight );
  std::vector<pat::Muon> selectedMuons_loose = miniAODhelper.GetSelectedMuons( *muons, 10., muonID::muonLoose );

  int numTightMuons = int(selectedMuons_tight.size());
  int numLooseMuons = int(selectedMuons_loose.size());// - numTightMuons;


  // Pass one tight lepton, zero loose lepton cuts
  bool passLJ = ( (numTightMuons+numTightElectrons)==1 &&
		  (numLooseMuons+numLooseElectrons)==1 );

  // Pass one tight lepton, exactly two leptons cuts
  bool passDIL = ( (numTightMuons+numTightElectrons)>=1 &&
		   (numLooseMuons+numLooseElectrons)==2 );


  // Event must either be LJ or DIL, as requested
  if( !((isLJ_ && passLJ) || (!isLJ_ && passDIL)) ) return;

  eve->PassLJ_ = ( passLJ ) ? 1 : 0;
  eve->PassDIL_ = ( passDIL ) ? 1 : 0;

  //// LJ subcategories
  int OneMuon = 0, OneElectron = 0;
  if( numTightMuons==1 && numLooseMuons==1 && numLooseElectrons == 0 )     OneMuon = 1;
  if( numTightElectrons==1 && numLooseElectrons==1 && numLooseMuons == 0 ) OneElectron = 1;

  eve->OneMuon_ = OneMuon;
  eve->OneElectron_ = OneElectron;

  //// DIL subcategories
  int TwoMuon = 0, TwoElectron = 0, MuonElectron = 0; 
  if ( numTightMuons>=1 && numLooseMuons==2 && numLooseElectrons==0 ) TwoMuon = 1;
  if ( numTightElectrons>=1 && numLooseElectrons==2 && numLooseMuons==0 ) TwoElectron = 1;
  if ( (numTightMuons + numTightElectrons)>=1 && numLooseMuons==1 && numLooseElectrons==1 ) MuonElectron = 1;

  eve->TwoMuon_ = TwoMuon;
  eve->TwoElectron_ = TwoElectron;
  eve->MuonElectron_ = MuonElectron;

   

  // Do jets stuff
  std::vector<pat::Jet> rawJets = miniAODhelper.GetUncorrectedJets(*pfjets);
  std::vector<pat::Jet> jetsNoMu = miniAODhelper.RemoveOverlaps(selectedMuons_loose, rawJets);
  std::vector<pat::Jet> jetsNoEle = miniAODhelper.RemoveOverlaps(selectedElectrons_loose, jetsNoMu);
  std::vector<pat::Jet> correctedJets_noSys = miniAODhelper.GetCorrectedJets(jetsNoEle, iEvent, iSetup);
  std::vector<pat::Jet> selectedJets_noSys_unsorted = miniAODhelper.GetSelectedJets(correctedJets_noSys, 30., 2.4, jetID::jetLoose, '-' );
  std::vector<pat::Jet> selectedJets_tag_noSys_unsorted = miniAODhelper.GetSelectedJets( correctedJets_noSys, 30., 2.4, jetID::jetLoose, 'M' );

  std::vector<pat::Jet> selectedJets_loose_noSys_unsorted = miniAODhelper.GetSelectedJets(correctedJets_noSys, 20., 2.4, jetID::jetLoose, '-' );
  std::vector<pat::Jet> selectedJets_loose_tag_noSys_unsorted = miniAODhelper.GetSelectedJets( correctedJets_noSys, 20., 2.4, jetID::jetLoose, 'M' );



  // Increment counter for passing cuts
  if( passLJ ){
    nevents_1l++; nevents_1l_wgt += wgt_lumi;
    if( OneMuon ){ nevents_1l_mu++; nevents_1l_mu_wgt += wgt_lumi; }
    else if( OneElectron ){ nevents_1l_ele++; nevents_1l_ele_wgt += wgt_lumi; }

    if( selectedJets_noSys_unsorted.size()>=4 ){
      nevents_1l_4j++; nevents_1l_4j_wgt += wgt_lumi;
      if( OneMuon ){ nevents_1l_4j_mu++; nevents_1l_4j_mu_wgt += wgt_lumi; }
      else if( OneElectron ){ nevents_1l_4j_ele++; nevents_1l_4j_ele_wgt += wgt_lumi; }

      if( selectedJets_tag_noSys_unsorted.size()>=2 ){
	nevents_1l_4j_2t++; nevents_1l_4j_2t_wgt += wgt_lumi;
	if( OneMuon ){ nevents_1l_4j_2t_mu++; nevents_1l_4j_2t_mu_wgt += wgt_lumi; }
	else if( OneElectron ){ nevents_1l_4j_2t_ele++; nevents_1l_4j_2t_ele_wgt += wgt_lumi; }
      }
    }
  }
  else if( passDIL ){
    nevents_2l++; nevents_2l_wgt += wgt_lumi;
    if( TwoMuon ){ nevents_2l_2m++; nevents_2l_2m_wgt += wgt_lumi; }
    else if( TwoElectron ){ nevents_2l_2e++; nevents_2l_2e_wgt += wgt_lumi; }
    else if( MuonElectron ){ nevents_2l_em++; nevents_2l_em_wgt += wgt_lumi; }

    if( selectedJets_noSys_unsorted.size()>=2 ){
      nevents_2l_2j++; nevents_2l_2j_wgt += wgt_lumi;
      if( TwoMuon ){ nevents_2l_2j_2m++; nevents_2l_2j_2m_wgt += wgt_lumi; }
      else if( TwoElectron ){ nevents_2l_2j_2e++; nevents_2l_2j_2e_wgt += wgt_lumi; }
      else if( MuonElectron ){ nevents_2l_2j_em++; nevents_2l_2j_em_wgt += wgt_lumi; }

      if( selectedJets_tag_noSys_unsorted.size()>=2 ){
	nevents_2l_2j_2t++; nevents_2l_2j_2t_wgt += wgt_lumi;
	if( TwoMuon ){ nevents_2l_2j_2t_2m++; nevents_2l_2j_2t_2m_wgt += wgt_lumi; }
	else if( TwoElectron ){ nevents_2l_2j_2t_2e++; nevents_2l_2j_2t_2e_wgt += wgt_lumi; }
	else if( MuonElectron ){ nevents_2l_2j_2t_em++; nevents_2l_2j_2t_em_wgt += wgt_lumi; }
      }
    }
  }


  eve->passElectronTrigger_ = 1;//( electron_triggerPass ) ? 1 : 0;
  eve->passMuonTrigger_     = 1;//( muon_triggerPass     ) ? 1 : 0;

  bool matchSingleMuTrigger = false;
  bool matchSingleElectronTrigger = false;

  // if( isMuon ) matchSingleMuTrigger = miniAODhelper.MuonMatchesSingleMuTrigger( selectedMuons_tight[0], hlt, hltobjs );
  // else         matchSingleElectronTrigger = miniAODhelper.ElectronMatchesSingleEleTrigger(selectedElectrons_tight[0], hlt, hltobjs );

  eve->matchSingleMuTrigger_ = ( matchSingleMuTrigger ) ? 1 : 0;
  eve->matchSingleElectronTrigger_ = ( matchSingleElectronTrigger ) ? 1 : 0;



  eve->run_ = run;
  eve->lumi_ = lumi;
  eve->evt_ = evt;



  eve->Q2ScaleUpWgt_ = 1.;
  eve->Q2ScaleDownWgt_ = 1.;
  // if( mySample.isTTJets ){
  // 	eve->Q2ScaleUpWgt_   = miniAODhelper.GetQ2ScaleUp(events.at(0));//1.402 * event->Q2ScaleUpWgt;
  // 	eve->Q2ScaleDownWgt_ = miniAODhelper.GetQ2ScaleDown(events.at(0));//0.683 * event->Q2ScaleDownWgt;
  // }


  eve->numPVs_ = numpv;
  eve->numSys_ = rNumSys;



  int lep_genId=-99;
  int lep_genParentId=-99;
  int lep_genGrandParentId=-99;

  TLorentzVector leptonV;
  double eps = 0.0001;
  leptonV.SetPxPyPzE(eps, eps, eps, eps);

  vdouble vlepton;
      
  double lepton_relIso = -99;

  vvdouble vvleptons;
  if( OneMuon ) {
    leptonV.SetPxPyPzE( selectedMuons_tight.at(0).px(), selectedMuons_tight.at(0).py(), selectedMuons_tight.at(0).pz(), selectedMuons_tight.at(0).energy());
    vlepton.push_back(selectedMuons_tight.at(0).px());
    vlepton.push_back(selectedMuons_tight.at(0).py());
    vlepton.push_back(selectedMuons_tight.at(0).pz());
    vlepton.push_back(selectedMuons_tight.at(0).energy());

    if( (selectedMuons_tight.at(0).genLepton()) ){
      lep_genId = selectedMuons_tight.at(0).genLepton()->pdgId();

      if( selectedMuons_tight.at(0).genLepton()->numberOfMothers()>=1 ){
	lep_genParentId = selectedMuons_tight.at(0).genLepton()->mother(0)->pdgId();
	if( selectedMuons_tight.at(0).genLepton()->mother(0)->numberOfMothers()>=1 ) 
	  lep_genGrandParentId = selectedMuons_tight.at(0).genLepton()->mother(0)->mother(0)->pdgId();
      }
    }

    lepton_relIso = miniAODhelper.GetMuonRelIso(selectedMuons_tight.at(0));
  }
  else if( OneElectron ){
    leptonV.SetPxPyPzE( selectedElectrons_tight.at(0).px(), selectedElectrons_tight.at(0).py(), selectedElectrons_tight.at(0).pz(), selectedElectrons_tight.at(0).energy());
    vlepton.push_back(selectedElectrons_tight.at(0).px());
    vlepton.push_back(selectedElectrons_tight.at(0).py());
    vlepton.push_back(selectedElectrons_tight.at(0).pz());
    vlepton.push_back(selectedElectrons_tight.at(0).energy());

    if( (selectedElectrons_tight.at(0).genLepton()) ){
      lep_genId = selectedElectrons_tight.at(0).genLepton()->pdgId();

      if( selectedElectrons_tight.at(0).genLepton()->numberOfMothers()>=1 ){
	lep_genParentId = selectedElectrons_tight.at(0).genLepton()->mother(0)->pdgId();
	if( selectedElectrons_tight.at(0).genLepton()->mother(0)->numberOfMothers()>=1 ) 
	  lep_genGrandParentId = selectedElectrons_tight.at(0).genLepton()->mother(0)->mother(0)->pdgId();
      }
    }

    lepton_relIso = miniAODhelper.GetElectronRelIso(selectedElectrons_tight.at(0));
  }

  if( OneMuon || OneElectron ){
    eve->tight_lepton_pt_  = leptonV.Pt();
    eve->tight_lepton_eta_ = leptonV.Eta();
    eve->tight_lepton_phi_ = leptonV.Phi();
    eve->tight_lepton_relIso_ = lepton_relIso;
    eve->tight_lepton_isMuon_ = ( OneMuon ) ? 1 : 0;

    eve->tight_lepton_genId_ = lep_genId;
    eve->tight_lepton_genParentId_ = lep_genParentId;
    eve->tight_lepton_genGrandParentId_ = lep_genGrandParentId;
  }


  vint lepton_genId, lepton_genParentId, lepton_genGrandParentId, lepton_trkCharge, lepton_isMuon, lepton_isTight;
  std::vector<TLorentzVector> vec_TLV_lep;
  TLorentzVector sum_lepton_vect;
  sum_lepton_vect.SetPxPyPzE(0., 0., 0., 0.);

  // Loop over loose muons
  for( std::vector<pat::Muon>::const_iterator iMu = selectedMuons_loose.begin(); iMu != selectedMuons_loose.end(); iMu++ ){ 
 
    int genId=-99, genParentId=-99, genGrandParentId=-99;
    if( (iMu->genLepton()) ){
      genId = iMu->genLepton()->pdgId();
      if( iMu->genLepton()->numberOfMothers()>=1 ){
	genParentId = iMu->genLepton()->mother(0)->pdgId();
	if( iMu->genLepton()->mother(0)->numberOfMothers()>=1 ) genGrandParentId = iMu->genLepton()->mother(0)->mother(0)->pdgId();
      }
    }

    int trkCharge = -99;
    if( iMu->muonBestTrack().isAvailable() ) trkCharge = iMu->muonBestTrack()->charge();

    int isTight = ( miniAODhelper.isGoodMuon(*iMu, minTightLeptonPt, muonID::muonTight) ) ? 1 : 0;

    lepton_trkCharge.push_back(trkCharge);
    lepton_isMuon.push_back(1);
    lepton_isTight.push_back(isTight);
    lepton_genId.push_back(genId);
    lepton_genParentId.push_back(genParentId);
    lepton_genGrandParentId.push_back(genGrandParentId);

    // Get muon 4Vector and add to vecTLorentzVector for muons
    TLorentzVector leptonP4;	  
    leptonP4.SetPxPyPzE(iMu->px(),iMu->py(),iMu->pz(),iMu->energy());
    vec_TLV_lep.push_back(leptonP4);

    sum_lepton_vect += leptonP4;

    // make vvdouble version of vecTLorentzVector
    vdouble vleptons;
    vleptons.push_back(iMu->px());
    vleptons.push_back(iMu->py());
    vleptons.push_back(iMu->pz());
    vleptons.push_back(iMu->energy());
    vvleptons.push_back(vleptons);
  }

  // Loop over loose muons
  for( std::vector<pat::Electron>::const_iterator iEle = selectedElectrons_loose.begin(); iEle != selectedElectrons_loose.end(); iEle++ ){ 
 
    int genId=-99, genParentId=-99, genGrandParentId=-99;
    if( (iEle->genLepton()) ){
      genId = iEle->genLepton()->pdgId();
      if( iEle->genLepton()->numberOfMothers()>=1 ){
	genParentId = iEle->genLepton()->mother(0)->pdgId();
	if( iEle->genLepton()->mother(0)->numberOfMothers()>=1 ) genGrandParentId = iEle->genLepton()->mother(0)->mother(0)->pdgId();
      }
    }

    int trkCharge = -99;
    if( iEle->gsfTrack().isAvailable() ) trkCharge = iEle->gsfTrack()->charge();

    int isTight = ( miniAODhelper.isGoodElectron(*iEle, minTightLeptonPt, electronID::electronTight) ) ? 1 : 0;

    lepton_trkCharge.push_back(trkCharge);
    lepton_isMuon.push_back(0);
    lepton_isTight.push_back(isTight);
    lepton_genId.push_back(genId);
    lepton_genParentId.push_back(genParentId);
    lepton_genGrandParentId.push_back(genGrandParentId);

    // Get electron 4Vector and add to vecTLorentzVector for electrons
    TLorentzVector leptonP4;	  
    leptonP4.SetPxPyPzE(iEle->px(),iEle->py(),iEle->pz(),iEle->energy());
    vec_TLV_lep.push_back(leptonP4);

    sum_lepton_vect += leptonP4;

    // make vvdouble version of vecTLorentzVector
    vdouble vleptons;
    vleptons.push_back(iEle->px());
    vleptons.push_back(iEle->py());
    vleptons.push_back(iEle->pz());
    vleptons.push_back(iEle->energy());
    vvleptons.push_back(vleptons);
  }

  eve->lepton_vect_TLV_         = vvleptons;
  eve->lepton_trkCharge_        = lepton_trkCharge;
  eve->lepton_isMuon_           = lepton_isMuon;
  eve->lepton_isTight_          = lepton_isTight;
  eve->lepton_genId_            = lepton_genId;
  eve->lepton_genParentId_      = lepton_genParentId;
  eve->lepton_genGrandParentId_ = lepton_genGrandParentId;


  int oppositeLepCharge = -9;
  if( lepton_trkCharge.size()==2 ){
    int chg0 = lepton_trkCharge[0];
    int chg1 = lepton_trkCharge[1];

    if( (chg0 * chg1)==-1 )     oppositeLepCharge = 1;
    else if( (chg0 * chg1)==1 ) oppositeLepCharge = 0;
    else if( chg0==-99 )        oppositeLepCharge = -1;
    else if( chg1==-99 )        oppositeLepCharge = -2;
    else                        oppositeLepCharge = -3;
  }
  eve->oppositeLepCharge_ = oppositeLepCharge;



  double mass_leplep = -99;
  if( vec_TLV_lep.size()==2 ){
    mass_leplep = sum_lepton_vect.M();
    eve->mass_leplep_ = mass_leplep;
    eve->dR_leplep_ = vec_TLV_lep[0].DeltaR(vec_TLV_lep[1]);
  }

  // PU scale factor
  double wgtPU = 1, wgtPUup = 1, wgtPUdown = 1;

  // wgtPU = miniAODhelper.GetPUweight( numTruePV );
  // wgtPUup = miniAODhelper.GetPUweightUp( numTruePV );
  // wgtPUdown = miniAODhelper.GetPUweightDown( numTruePV );

  eve->wgt_pu_     = wgtPU;
  eve->wgt_puUp_   = wgtPUup;
  eve->wgt_puDown_ = wgtPUdown;


  // Lepton scale factor
  float leptonSF = 1.0;
  // if( insample_>=0 ){
  // 	if( isMuon ) leptonSF = miniAODhelper.GetMuonSF(selectedMuons_tight.at(0));
  // 	else         leptonSF = miniAODhelper.GetElectronSF(selectedElectrons_tight.at(0));
  // }

  eve->wgt_lepSF_  = leptonSF;

  eve->wgt_lumi_  = intLumi_;
  eve->wgt_xs_    = mySample_xSec_;//mySample.xSec;
  eve->wgt_nGen_  = mySample_nGen_;//mySample.nGen;


  double wgt_topPtSF     = 1;//miniAODhelper.GetTopPtweight( mcparticles );
  double wgt_topPtSFUp   = 1;//miniAODhelper.GetTopPtweightUp( mcparticles );
  double wgt_topPtSFDown = 1;//miniAODhelper.GetTopPtweightDown( mcparticles );

  eve->wgt_topPt_ = wgt_topPtSF;
  eve->wgt_topPtUp_ = wgt_topPtSFUp;
  eve->wgt_topPtDown_ = wgt_topPtSFDown;



  double top_pt = -1;
  double antitop_pt = -1;

  // Get top pTs for reweighting downstream
  // if( mySample.isTTJets ){
  // 	for( int iPar=0; iPar<int(mcparticles.size()); iPar++ ){
  // 	  int id = mcparticles.at(iPar).id;
  // 	  if( id==6 )       top_pt     = mcparticles.at(iPar).pt;
  // 	  else if( id==-6 ) antitop_pt = mcparticles.at(iPar).pt;
  // 	}
  // }
  eve->top_pt_ = top_pt;
  eve->antitop_pt_ = antitop_pt;


  // Special collection used for correcting MET collection
  // BNjetCollection pfJets_forMET  = miniAODhelper.GetSelectedJets( pfjets, 12., 4.9, jetID::jetLoose, '-' ); 

  // bool for passing jet/tag cuts, first is for tree filling, second is for event counting
  bool hasNumJetNumTag = false;


  double totalWgt = wgt_lumi;

  // Loop over systematics
  for( int iSys=0; iSys<rNumSys; iSys++ ){

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
    case 5 : iSysType = sysType::JERup;    break;
    case 6 : iSysType = sysType::JERdown;  break;
    case 7 : iSysType = sysType::JESup;    break;
    case 8 : iSysType = sysType::JESdown;  break;
    case 9 : iSysType = sysType::CSVLFup;         break;
    case 10: iSysType = sysType::CSVLFdown;       break;
    case 11: iSysType = sysType::CSVHFup;         break;
    case 12: iSysType = sysType::CSVHFdown;       break;
    case 13: iSysType = sysType::CSVHFStats1up;   break;
    case 14: iSysType = sysType::CSVHFStats1down; break;
    case 15: iSysType = sysType::CSVHFStats2up;   break;
    case 16: iSysType = sysType::CSVHFStats2down; break;
    case 17: iSysType = sysType::CSVLFStats1up;   break;
    case 18: iSysType = sysType::CSVLFStats1down; break;
    case 19: iSysType = sysType::CSVLFStats2up;   break;
    case 20: iSysType = sysType::CSVLFStats2down; break;
    default: iSysType = sysType::NA;       break;
    }


    bool IsTauTauLeptonEvent = false;//miniAODhelper.IsTauEvent(taus, pfjets, electrons, muons, iSysType);
    eve->IsTauTauLeptonEvent_[iSys] = ( IsTauTauLeptonEvent ) ? 1 : 0;

    // Add Q2scale systematic
    if( iSys==21 )      eve->wgt_[iSys] *= eve->Q2ScaleUpWgt_;
    else if( iSys==22 ) eve->wgt_[iSys] *= eve->Q2ScaleDownWgt_;


    // top pT reweight
    if( iSys==23 )      eve->wgt_[iSys] *= wgt_topPtSFUp;
    else if( iSys==24 ) eve->wgt_[iSys] *= wgt_topPtSFDown;
    else                eve->wgt_[iSys] *= wgt_topPtSF;

    if( iSys==0 ) totalWgt *= wgt_topPtSF;


    // nJets/Tags
    // THESE MUST BE INITIALIZED TO -1
    eve->numJets_float_[iSys] = -1;
    eve->numTags_float_[iSys] = -1;


    /////////
    ///
    /// Pfjets
    ///
    ////////

    std::vector<pat::Jet> correctedJets = ( !(iSys>=5 && iSys<=8) ) ? correctedJets_noSys : miniAODhelper.GetCorrectedJets(jetsNoEle, iEvent, iSetup, iSysType);
    std::vector<pat::Jet> selectedJets_unsorted = ( !(iSys>=5 && iSys<=8) ) ? selectedJets_noSys_unsorted : miniAODhelper.GetSelectedJets(correctedJets, 30., 2.4, jetID::jetLoose, '-' );


    // Get CSVM tagged jet collection
    std::vector<pat::Jet> selectedJets_tag_unsorted = ( !(iSys>=5 && iSys<=8) ) ? selectedJets_tag_noSys_unsorted : miniAODhelper.GetSelectedJets( correctedJets, 30., 2.4, jetID::jetLoose, 'M' );

    // Get nontagged jet collection
    std::vector<pat::Jet> selectedJets_untag_unsorted = selectedJets_tag_unsorted;//miniAODhelper.GetSelectedJets( correctedJets, 30., 2.4, jetID::jetLoose, 'M' ); 
    //std::vector<pat::Jet> selectedJets_untag_unsorted = miniAODhelper.GetSymmetricDifference( selectedJets_unsorted, selectedJets_tag_unsorted );



    // // Require at least 2 tags
    // if( !(selectedJets_tag_unsorted.size()>=2) ) continue;

  
    // Sort jet collections by pT
    std::vector<pat::Jet> selectedJets       = miniAODhelper.GetSortedByPt( selectedJets_unsorted );
    std::vector<pat::Jet> selectedJets_tag   = miniAODhelper.GetSortedByPt( selectedJets_tag_unsorted );
    std::vector<pat::Jet> selectedJets_untag = miniAODhelper.GetSortedByPt( selectedJets_untag_unsorted );

    //if( mySample.isTTJets ) splitEvent = miniAODhelper.ttPlusHFKeepEvent( mcparticles, selectedJets );
    //if( !splitEvent ) continue;

    // if( mySample.isTTJets ){
    //   eve->ttbb_algo_result_[iSys] = miniAODhelper.ttPlusBBClassifyEvent( mcparticles, selectedJets );
    //   eve->ttcc_algo_result_[iSys] = miniAODhelper.ttPlusCCClassifyEvent( mcparticles, selectedJets );
    // }


    // Get numJets, numTags
    int numJet = int( selectedJets.size() );
    int numTag = int( selectedJets_tag.size() );

         
    // Get Corrected MET
    pat::MET correctedMET = pfmet->front();//miniAODhelper.GetCorrectedMET( pfmets.at(0), pfJets_forMET, iSysType );

    TLorentzVector metV(correctedMET.px(),correctedMET.py(),0.0,correctedMET.pt());

       
    // Initialize event vars, selected jets
    double mht_px = 0.;
    double mht_py = 0.;
    eve->HT_[iSys] = 0.;


    double min_dR_lep_jet = 99.;
    vecTLorentzVector jetV;
    std::vector<double> csvV;
    std::vector<double> jet_vtxMass;
    std::vector<double> jet_vtxNtracks;
    std::vector<double> jet_vtx3DVal;
    std::vector<double> jet_vtx3DSig;

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


    vint jet_genId_vect;
    vint jet_flavour_vect;
    vint jet_genParentId_vect;
    vint jet_genGrandParentId_vect;


    // Loop over selected jets
    for( std::vector<pat::Jet>::const_iterator iJet = selectedJets.begin(); iJet != selectedJets.end(); iJet++ ){ 

      jet_flavour_vect.push_back(iJet->partonFlavour());

      int genPartonId=-99, genPartonMotherId=-99, genPartonGrandMotherId=-99;
      if( (iJet->genParton()) ){ // if there is a matched parton, fill variables
	genPartonId = iJet->genParton()->pdgId();

	if( iJet->genParton()->numberOfMothers()>=1 ){
	  genPartonMotherId = iJet->genParton()->mother(0)->pdgId();

	  if( iJet->genParton()->mother(0)->numberOfMothers()>=1 ) genPartonMotherId = iJet->genParton()->mother(0)->mother(0)->pdgId();
	}
      }

      jet_genId_vect.push_back(genPartonId);
      jet_genParentId_vect.push_back(genPartonMotherId);
      jet_genGrandParentId_vect.push_back(genPartonGrandMotherId);

      // DR(lepton system, jet)
      double dR_lep_jet = reco::deltaR(sum_lepton_vect.Eta(),sum_lepton_vect.Phi(),iJet->eta(),iJet->phi());
      if( dR_lep_jet<min_dR_lep_jet ) min_dR_lep_jet = dR_lep_jet;

      // Get jet 4Vector and add to vecTLorentzVector for jets
      TLorentzVector jet0p4;	  
      jet0p4.SetPxPyPzE(iJet->px(),iJet->py(),iJet->pz(),iJet->energy());
      jetV.push_back(jet0p4);
	  
      // make vvdouble version of vecTLorentzVector
      vdouble vjets;
      vjets.push_back(iJet->px());
      vjets.push_back(iJet->py());
      vjets.push_back(iJet->pz());
      vjets.push_back(iJet->energy());
      vvjets.push_back(vjets);

      jet_vtxMass.push_back(iJet->userFloat("vtxMass"));
      jet_vtxNtracks.push_back(iJet->userFloat("vtxNtracks"));
      jet_vtx3DVal.push_back(iJet->userFloat("vtx3DVal"));
      jet_vtx3DSig.push_back(iJet->userFloat("vtx3DSig"));

      // Get CSV discriminant, check if passes Med WP 
      double myCSV = iJet->bDiscriminator("combinedSecondaryVertexBJetTags");
      csvV.push_back(myCSV);
      int csvM0 = ( myCSV > 0.679 ) ? 1 : 0;
      if( myCSV>0.679 ){
	numtag += 1;
	sum_btag_disc_btags += myCSV;
      }
      else{
	numuntag += 1;
	sum_btag_disc_non_btags += myCSV;
      }


      // Second Loop over selected jets
      for( std::vector<pat::Jet>::const_iterator jJet = iJet; jJet != selectedJets.end(); jJet++ ){ 

	// Continue if same jet as above loop
	if( iJet==jJet ) continue;

	// Get second jet 4Vector and check bTag discriminant
	TLorentzVector jet1p4;
	jet1p4.SetPxPyPzE(jJet->px(),jJet->py(),jJet->pz(),jJet->energy());
	int csvM1 = ( jJet->bDiscriminator("combinedSecondaryVertexBJetTags") > 0.679 ) ? 1 : 0;

	// Third loop over selected jets
	for( std::vector<pat::Jet>::const_iterator kJet = jJet; kJet != selectedJets.end(); kJet++ ){ 
     
	  // Continue is third jet is same as the above two jets
	  if( iJet==kJet || jJet==kJet ) continue;

	  // Get third jet 4Vector and chekc bTag discriminant
	  TLorentzVector jet2p4;
	  jet2p4.SetPxPyPzE(kJet->px(),kJet->py(),kJet->pz(),kJet->energy());
	  int csvM2 = ( kJet->bDiscriminator("combinedSecondaryVertexBJetTags") > 0.679 ) ? 1 : 0;

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
    std::vector<pat::Jet> selectedJets_loose_unsorted = ( !(iSys>=5 && iSys<=8) ) ? selectedJets_loose_noSys_unsorted : miniAODhelper.GetSelectedJets( correctedJets, 20., 2.4, jetID::jetLoose, '-' );
    std::vector<pat::Jet> selectedJets_loose = miniAODhelper.GetSortedByPt( selectedJets_loose_unsorted );

    std::vector<pat::Jet> selectedJets_loose_tag_unsorted = ( !(iSys>=5 && iSys<=8) ) ? selectedJets_loose_tag_noSys_unsorted : miniAODhelper.GetSelectedJets( correctedJets, 20., 2.4, jetID::jetLoose, 'M' );


    vvdouble vvjets_loose;
    std::vector<double> csvV_loose;
    std::vector<double> jet_loose_vtxMass;
    std::vector<double> jet_loose_vtxNtracks;
    std::vector<double> jet_loose_vtx3DVal;
    std::vector<double> jet_loose_vtx3DSig;
    vint jet_flavour_vect_loose;
    vecTLorentzVector jetV_loose;

    int numJet_loose = 0;
    int numTag_loose = 0;

    vvdouble jet_all_vect_TLV;
    vdouble  jet_all_CSV;
    vint     jet_all_flavour;

    // Loop over selected jets
    for( std::vector<pat::Jet>::const_iterator iJet = selectedJets_loose.begin(); iJet != selectedJets_loose.end(); iJet++ ){ 

      vdouble vjets_loose;
      vjets_loose.push_back(iJet->px());
      vjets_loose.push_back(iJet->py());
      vjets_loose.push_back(iJet->pz());
      vjets_loose.push_back(iJet->energy());
      jet_all_vect_TLV.push_back(vjets_loose);

      double myCSV = iJet->bDiscriminator("combinedSecondaryVertexBJetTags");
      jet_all_CSV.push_back(myCSV);

      jet_all_flavour.push_back(iJet->partonFlavour());

      // MHT
      mht_px += - iJet->px();
      mht_py += - iJet->py();
      eve->HT_[iSys] += iJet->pt();


      // loose represents all jets with pT > 20
      //if( iJet->pt()>=30. ) continue;

      numJet_loose++;
      jet_flavour_vect_loose.push_back(iJet->partonFlavour());

      jet_loose_vtxMass.push_back(iJet->userFloat("vtxMass"));
      jet_loose_vtxNtracks.push_back(iJet->userFloat("vtxNtracks"));
      jet_loose_vtx3DVal.push_back(iJet->userFloat("vtx3DVal"));
      jet_loose_vtx3DSig.push_back(iJet->userFloat("vtx3DSig"));

      TLorentzVector jet0p4;	  
      jet0p4.SetPxPyPzE(iJet->px(),iJet->py(),iJet->pz(),iJet->energy());
      jetV_loose.push_back(jet0p4);

      // make vvdouble version of vecTLorentzVector
      vvjets_loose.push_back(vjets_loose);

      // Get CSV discriminant, check if passes Med WP 
      csvV_loose.push_back(myCSV);
      if( myCSV>0.679 ) numTag_loose++;
    }


  
    // Add lepton 4Vector quantities to MHT
    mht_px += - sum_lepton_vect.Px();
    mht_py += - sum_lepton_vect.Py();
    double MHT = sqrt( mht_px*mht_px + mht_py*mht_py );

    bool PassBigDiamondZmask = ( MuonElectron || 
				 (mass_leplep < (65.5 + 3*MHT/8)) || 
				 (mass_leplep > (108 - MHT/4)) || 
				 (mass_leplep < (79 - 3*MHT/4)) || 
				 (mass_leplep > (99 + MHT/2)) 
				 );
    eve->PassZmask_ = ( PassBigDiamondZmask ) ? 1 : 0;


    double MET = correctedMET.pt();
    bool PassBigDiamondZmaskMET = ( MuonElectron || 
				    (mass_leplep < (65.5 + 3*MET/8)) || 
				    (mass_leplep > (108 - MET/4)) || 
				    (mass_leplep < (79 - 3*MET/4)) || 
				    (mass_leplep > (99 + MET/2)) 
				    );
    eve->PassZmaskMET_ = ( PassBigDiamondZmaskMET ) ? 1 : 0;


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
    for( std::vector<pat::Jet>::const_iterator iJet = selectedJets_tag.begin(); iJet != selectedJets_tag.end(); iJet++ ){ 

      // Get bTag Value
      double myCSV = iJet->bDiscriminator("combinedSecondaryVertexBJetTags");

      // Compute Deviation from Avg bTag
      double dev = myCSV - ave_btag_disc_btags;
      sum_dev_from_ave_disc_btags += dev*dev;

      // Get lowest bTag
      min_btag = std::min( min_btag, myCSV );

      // Get this jet's 4Vector
      TLorentzVector jet0p4;
      jet0p4.SetPxPyPzE(iJet->px(),iJet->py(),iJet->pz(),iJet->energy());

      // Min dR(lep, tag), M(lep, tag)
      double dR_lep_tag = jet0p4.DeltaR(sum_lepton_vect);
      if( dR_lep_tag<min_dR_tag_lep ){
	min_dR_tag_lep = dR_lep_tag;
	TLorentzVector sum_lep_b = sum_lepton_vect + jet0p4;//leptonV + jet0p4;
	mlb_temp = sum_lep_b.M();
      }

      // Second loop over selected, tagged jets
      for( std::vector<pat::Jet>::const_iterator jJet = iJet; jJet != selectedJets_tag.end(); jJet++ ){ 

	// Continue if jet is same as above
	if( iJet==jJet ) continue;

	// Get second jet's 4Vector
	TLorentzVector jet1p4;
	jet1p4.SetPxPyPzE(jJet->px(),jJet->py(),jJet->pz(),jJet->energy());

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
    for( std::vector<pat::Jet>::const_iterator iJet = selectedJets_untag.begin(); iJet != selectedJets_untag.end(); iJet++ ){ 

      // Get CSV discriminant
      double myCSV = iJet->bDiscriminator("combinedSecondaryVertexBJetTags");

      // Compute deviation form Avg bTag Disc
      double dev = myCSV - ave_btag_disc_non_btags;
      sum_dev_from_ave_disc_non_btags += dev*dev;

      // Get this jet's 4Vect
      TLorentzVector jet0p4;
      jet0p4.SetPxPyPzE(iJet->px(),iJet->py(),iJet->pz(),iJet->energy());

      // Second loop over selected, untagged jets
      for( std::vector<pat::Jet>::const_iterator jJet = iJet; jJet != selectedJets_untag.end(); jJet++ ){ 

	// Continue if this jet matches the jet above
	if( iJet==jJet ) continue;

	// Get this jet's 4Vector
	TLorentzVector jet1p4;
	jet1p4.SetPxPyPzE(jJet->px(),jJet->py(),jJet->pz(),jJet->energy());

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
    TLorentzVector all_objects = sum_lepton_vect + metV;//leptonV + metV;
	
    for( int j=0; j<int(jetV.size()); j++ ) all_objects += jetV[j];
	  

    // Compute MHT_
    eve->MHT_[iSys] = sqrt( mht_px*mht_px + mht_py*mht_py );
    eve->MHT_phi_[iSys] = std::atan2(mht_py,mht_px);


    // Comput Ht+Lepton
    //double ht_lep = HT_[iSys] + leptonV.Pt();


    // Compute angular quantities
    // miniAODhelper.getSp(leptonV,metV,jetV,eve->aplanarity_[iSys],eve->sphericity_[iSys]);
    // miniAODhelper.getFox(jetV,eve->h0_[iSys],eve->h1_[iSys],eve->h2_[iSys],eve->h3_[iSys],eve->h4_[iSys]);

    // Intialize 4Vectors, to use in Best Higgs Mass function
    TLorentzVector mydummyguy;
    mydummyguy.SetPxPyPzE(0,0,0,0);
    TLorentzVector bjet1_tmp = mydummyguy;
    TLorentzVector bjet2_tmp = mydummyguy;

    // Get Best Higgs Mass (X^2 method)
    eve->best_higgs_mass_[iSys] = 0.;//getBestHiggsMass(leptonV, metV, jetV, csvV, eve->minChi2_[iSys], eve->dRbb_[iSys], bjet1_tmp, bjet2_tmp, jetV_loose, csvV_loose);

    //double chiSq2=10000;

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
    if( selectedJets.size()>0 ) eve->first_jet_pt_[iSys]  = selectedJets.at(0).pt();
    if( selectedJets.size()>1 ) eve->second_jet_pt_[iSys] = selectedJets.at(1).pt();
    if( selectedJets.size()>2 ) eve->third_jet_pt_[iSys]  = selectedJets.at(2).pt();
    if( selectedJets.size()>3 ) eve->fourth_jet_pt_[iSys] = selectedJets.at(3).pt();

    // MET
    eve->MET_[iSys]      = correctedMET.pt();
    eve->uMET_[iSys]     = correctedMET.uncorrectedPt();
    eve->MET_phi_[iSys]  = correctedMET.phi();
    eve->uMET_phi_[iSys] = correctedMET.uncorrectedPhi();


    // Multi-object Quantities
    eve->all_sum_pt_[iSys]                   = eve->HT_[iSys]+sum_lepton_vect.Pt();//leptonV.Pt();
    eve->all_sum_pt_with_met_[iSys]          = eve->HT_[iSys]+sum_lepton_vect.Pt()+correctedMET.pt();;//leptonV.Pt()+correctedMET.pt();
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
    if( csvV_temp.size()>0 ) eve->first_highest_btag_[iSys]  = csvV_temp[0];
    if( csvV_temp.size()>1 ) eve->second_highest_btag_[iSys] = csvV_temp[1];
    if( csvV_temp.size()>2 ) eve->third_highest_btag_[iSys]  = csvV_temp[2];
    if( csvV_temp.size()>3 ) eve->fourth_highest_btag_[iSys] = csvV_temp[3];
    eve->lowest_btag_[iSys]             = min_btag;


    eve->minChi2_bjet1_px_[iSys] = bjet1_tmp.Px();
    eve->minChi2_bjet1_py_[iSys] = bjet1_tmp.Py();
    eve->minChi2_bjet1_pz_[iSys] = bjet1_tmp.Pz();
    eve->minChi2_bjet1_E_[iSys]  = bjet1_tmp.E();

    eve->minChi2_bjet2_px_[iSys] = bjet2_tmp.Px();
    eve->minChi2_bjet2_py_[iSys] = bjet2_tmp.Py();
    eve->minChi2_bjet2_pz_[iSys] = bjet2_tmp.Pz();
    eve->minChi2_bjet2_E_[iSys]  = bjet2_tmp.E();

    eve->jet_vect_TLV_[iSys]   = vvjets;
    eve->jet_CSV_[iSys]        = csvV;
    eve->jet_vtxMass_[iSys]    = jet_vtxMass;
    eve->jet_vtxNtracks_[iSys] = jet_vtxNtracks;
    eve->jet_vtx3DVal_[iSys]   = jet_vtx3DVal;
    eve->jet_vtx3DSig_[iSys]   = jet_vtx3DSig;
    eve->jet_flavour_[iSys]          = jet_flavour_vect;
    eve->jet_genId_[iSys]            = jet_genId_vect;
    eve->jet_genParentId_[iSys]      = jet_genParentId_vect;
    eve->jet_genGrandParentId_[iSys] = jet_genGrandParentId_vect;

    // loose jets

    // nJets/Tags
    eve->numJets_loose_float_[iSys] = numJet_loose;
    eve->numTags_loose_float_[iSys] = numTag_loose;

    eve->jet_loose_vect_TLV_[iSys] = vvjets_loose;
    eve->jet_loose_CSV_[iSys]      = csvV_loose;
    eve->jet_loose_vtxMass_[iSys]    = jet_loose_vtxMass;
    eve->jet_loose_vtxNtracks_[iSys] = jet_loose_vtxNtracks;
    eve->jet_loose_vtx3DVal_[iSys]   = jet_loose_vtx3DVal;
    eve->jet_loose_vtx3DSig_[iSys]   = jet_loose_vtx3DSig;
    eve->jet_loose_flavour_[iSys]  = jet_flavour_vect_loose;



    double csvWgtHF=1, csvWgtLF=1, csvWgtCF=1;
    double csvWgt = 1;//get_csv_wgt(jet_all_vect_TLV, jet_all_CSV, jet_all_flavour, iSys, csvWgtHF, csvWgtLF, csvWgtCF);
    if( insample_<0 ){
      csvWgt = 1.; csvWgtHF = 1.; csvWgtLF = 1.; csvWgtCF = 1.;
    }

    eve->wgt_csvSF_[iSys]    = csvWgt;
    eve->wgt_csvSF_HF_[iSys] = csvWgtHF;
    eve->wgt_csvSF_LF_[iSys] = csvWgtLF;
    eve->wgt_csvSF_CF_[iSys] = csvWgtCF;


    eve->wgt_[iSys] *= csvWgt;

    if( iSys==0 ) totalWgt *= csvWgt;


    unsigned int minNumLooseJet = ( isLJ_ ) ? 4 : 1;
    unsigned int minNumLooseTag = ( isLJ_ ) ? 1 : 0;

    // if( iSys>=7 && iSys<=8 ){
    //   for( std::vector<pat::Jet>::const_iterator iJet = selectedJets_loose.begin(); iJet != selectedJets_loose.end(); iJet++ ){ 
    // 	printf("\t selectedJets_loose iSys = %d,\t jet %d: pt = %.2f,\t raw pt = %.2f,\t eta = %.2f,\t phi = %.2f \n", 
    // 	       iSys, int(iJet - selectedJets_loose.begin()), iJet->pt(), iJet->correctedJet(0).pt(), iJet->eta(), iJet->phi() );
    //   }
    //   for( std::vector<pat::Jet>::const_iterator iJet = jetsNoEle.begin(); iJet != jetsNoEle.end(); iJet++ ){ 
    // 	if( !( iJet->pt()>20. && fabs(iJet->eta())<2.4 ) ) continue;
    // 	printf("\t jetsNoEle iSys = %d,\t jet %d: pt = %.2f,\t raw pt = %.2f,\t eta = %.2f,\t phi = %.2f \n", 
    // 	       iSys, int(iJet - jetsNoEle.begin()), iJet->pt(), iJet->correctedJet(0).pt(), iJet->eta(), iJet->phi() );
    //   }
    //   for( std::vector<pat::Jet>::const_iterator iJet = correctedJets_noSys.begin(); iJet != correctedJets_noSys.end(); iJet++ ){ 
    // 	if( !( iJet->pt()>20. && fabs(iJet->eta())<2.4 ) ) continue;
    // 	printf("\t correctedJets_noSy iSys = %d,\t jet %d: pt = %.2f,\t raw pt = %.2f,\t eta = %.2f,\t phi = %.2f \n", 
    // 	       iSys, int(iJet - correctedJets_noSys.begin()), iJet->pt(), iJet->correctedJet(0).pt(), iJet->eta(), iJet->phi() );
    //   }
    //   for( std::vector<pat::Jet>::const_iterator iJet = pfjets->begin(); iJet != pfjets->end(); iJet++ ){ 
    // 	if( !( iJet->pt()>20. && fabs(iJet->eta())<2.4 ) ) continue;
    // 	printf("\t pfjets iSys = %d,\t jet %d: pt = %.2f,\t raw pt = %.2f,\t eta = %.2f,\t phi = %.2f \n", 
    // 	       iSys, int(iJet - pfjets->begin()), iJet->pt(), iJet->correctedJet(0).pt(), iJet->eta(), iJet->phi() );
    //   }
    // }
    if( selectedJets_loose_unsorted.size()>=minNumLooseJet && selectedJets_loose_tag_unsorted.size()>=minNumLooseTag ){
      hasNumJetNumTag = true;
      neventsFillTree[iSys]++;
    }

  } // end loop over systematics


  //
  // Fill tree if pass full selection
  //
  if( hasNumJetNumTag ){
    worldTree->Fill();
  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
YggdrasilTreeMaker::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
YggdrasilTreeMaker::endJob() 
{

  std::cout << " *********************************************************** " << std::endl;
  if( isLJ_ ) std::cout << " ** Lepton + jets event selection ** " << std::endl;
  else       std::cout << " ** Opposite sign dilepton event selection ** " << std::endl;
  std::cout << " " << std::endl;
  std::cout << "\t Event information: " << std::endl;
  std::cout << "\t\t Sample = " << mySample_sampleName_ << std::endl;
  std::cout << "\t\t Integrated lumi (pb^-1) = " << intLumi_ << std::endl;
  std::cout << "\t\t Cross section (pb)      = " << mySample_xSec_ << std::endl;
  std::cout << "\t\t Number generated        = " << mySample_nGen_ << std::endl;
  std::cout << " " << std::endl;
  std::cout << "   Number of Events Processed  = " << nevents << std::endl;
  std::cout << "   Number of Events Processed (wgt) = " << nevents_wgt << std::endl;

  std::cout << "   Number of Events Pass Trigger  = " << nevents_clean << std::endl;
  std::cout << "   Number of Events Pass Trigger (wgt) = " << nevents_clean_wgt << std::endl;

  if( isLJ_ ){
    std::cout << "   Number of Events with single lepton = " << nevents_1l << " (ele = " << nevents_1l_ele << ", mu = " << nevents_1l_mu << ")" << std::endl;
    std::cout << "   Number of Events with single lepton (wgt) = " << nevents_1l_wgt << " (ele = " << nevents_1l_ele_wgt << ", mu = " << nevents_1l_mu_wgt << ")" << std::endl;
    std::cout << "   Number of Events with single lepton, >=4 jets = " << nevents_1l_4j << " (ele = " << nevents_1l_4j_ele << ", mu = " << nevents_1l_4j_mu << ")" << std::endl;
    std::cout << "   Number of Events with single lepton, >=4 jets (wgt) = " << nevents_1l_4j_wgt << " (ele = " << nevents_1l_4j_ele_wgt << ", mu = " << nevents_1l_4j_mu_wgt << ")" << std::endl;
    std::cout << "   Number of Events with single lepton, >=4 jets, >=2 tags = " << nevents_1l_4j_2t << " (ele = " << nevents_1l_4j_2t_ele << ", mu = " << nevents_1l_4j_2t_mu << ")" << std::endl;
    std::cout << "   Number of Events with single lepton, >=4 jets, >=2 tags (wgt) = " << nevents_1l_4j_2t_wgt << " (ele = " << nevents_1l_4j_2t_ele_wgt << ", mu = " << nevents_1l_4j_2t_mu_wgt << ")" << std::endl;
  }
  else {
    std::cout << "   Number of Events with two leptons = " << nevents_2l << " (2e = " << nevents_2l_2e << ", 2m = " << nevents_2l_2m << ", em = " << nevents_2l_em << ")" << std::endl;
    std::cout << "   Number of Events with two leptons (wgt) = " << nevents_2l_wgt << " (2e = " << nevents_2l_2e_wgt << ", 2m = " << nevents_2l_2m_wgt << ", em = " << nevents_2l_em_wgt << ")" << std::endl;
    std::cout << "   Number of Events with two leptons, >=2 jets = " << nevents_2l_2j << " (2e = " << nevents_2l_2j_2e << ", 2m = " << nevents_2l_2j_2m << ", em = " << nevents_2l_2j_em << ")" << std::endl;
    std::cout << "   Number of Events with two leptons, >=2 jets (wgt) = " << nevents_2l_2j_wgt << " (2e = " << nevents_2l_2j_2e_wgt << ", 2m = " << nevents_2l_2j_2m_wgt << ", em = " << nevents_2l_2j_em_wgt << ")" << std::endl;
    std::cout << "   Number of Events with two leptons, >=2 jets, >=2 tags = " << nevents_2l_2j_2t << " (2e = " << nevents_2l_2j_2t_2e << ", 2m = " << nevents_2l_2j_2t_2m << ", em = " << nevents_2l_2j_2t_em << ")" << std::endl;
    std::cout << "   Number of Events with two leptons, >=2 jets, >=2 tags (wgt) = " << nevents_2l_2j_2t_wgt << " (2e = " << nevents_2l_2j_2t_2e_wgt << ", 2m = " << nevents_2l_2j_2t_2m_wgt << ", em = " << nevents_2l_2j_2t_em_wgt << ")" << std::endl;
  }

  for( int i=0; i<rNumSys; i++ ) printf(" i = %d,\t Num Pass = %d \n", i, neventsFillTree[i] );
  std::cout << " *********************************************************** " << std::endl;

}

// ------------ method called when starting to processes a run  ------------
/*
void 
YggdrasilTreeMaker::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
YggdrasilTreeMaker::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
YggdrasilTreeMaker::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
YggdrasilTreeMaker::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
YggdrasilTreeMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(YggdrasilTreeMaker);
