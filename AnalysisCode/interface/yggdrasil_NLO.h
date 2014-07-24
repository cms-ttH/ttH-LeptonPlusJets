#ifndef ttH_LeptonPlusJets_AnalysisCode_yggdrasil_NLO_h
#define ttH_LeptonPlusJets_AnalysisCode_yggdrasil_NLO_h


#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TLorentzVector.h"

#include <algorithm>
#include <vector>
#include <exception>
#include <cmath> 
#include <string>


#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "ttH-LeptonPlusJets/AnalysisCode/interface/analysisInfo_CMS_ttH_LJ.h"
#include "ttH-LeptonPlusJets/AnalysisCode/interface/BEANeventVars_NLO.h"

#endif


#ifdef __MAKECINT__
#pragma link C++ class std::vector< TLorentzVector >+; 
#endif


//*****************************************************************************

typedef std::vector< std::vector< std::vector<double> > > vvvdouble;
typedef std::vector<std::vector<double> >      vvdouble;
typedef std::vector<std::vector<std::string> > vvstring;
typedef std::vector<double>                    vdouble;
typedef std::vector<string>                    vstring;
typedef std::vector<bool>                      vbool;
typedef std::vector<int>                       vint;
typedef std::vector< TLorentzVector >          vTLV;


//*****************************************************************************


class yggdrasil_NLO{

 public:
 
  yggdrasil_NLO();
  virtual ~yggdrasil_NLO();

  void makeTree(int sampleId, int maxNentries=-1, int Njobs=1, int jobN=1);
  
  void printDaughters(std::vector< reco::GenParticle >::const_iterator parent, std::vector< reco::GenParticle > genParticles, int nSpaces=0);
  
  void printMothers(std::vector< reco::GenParticle >::const_iterator daughter,  std::vector< reco::GenParticle > genParticles, int nSpaces=0);
  
  void printGenInfo(std::vector< reco::GenParticle >::const_iterator thisParticle, int iCount=0);

  void printGenCollectionInfo(std::vector< reco::GenParticle > genParticles);

  void getAllStagesOfGenParticle(std::vector< reco::GenParticle >::const_iterator thisParticle, std::vector< reco::GenParticle > genParticles, std::vector< reco::GenParticle > &target, int nCalls=0);

  void getDecayChainAll(std::vector< reco::GenParticle >::const_iterator parent, std::vector< reco::GenParticle > genParticles, std::vector< reco::GenParticle > &target);

  void getDecayChainFinalState(std::vector< reco::GenParticle >::const_iterator parent, std::vector< reco::GenParticle > genParticles, std::vector< reco::GenParticle > &target);

  void getGenTTbarSystem(std::vector< reco::GenParticle > genParticles, std::vector< reco::GenParticle > &top_quark, std::vector< reco::GenParticle > &b_quark_fromTop, std::vector< reco::GenParticle > &w_boson_fromTop, std::vector< reco::GenParticle > &wDaughter1_fromTop, std::vector< reco::GenParticle > &wDaughter2_fromTop, std::vector< reco::GenParticle > &topBar_quark, std::vector< reco::GenParticle > &b_quark_fromTopBar, std::vector< reco::GenParticle > &w_boson_fromTopBar, std::vector< reco::GenParticle > &wDaughter1_fromTopBar, std::vector< reco::GenParticle > &wDaughter2_fromTopBar); 

  void getGenExtraBCQuarks(std::vector< reco::GenParticle > genParticles, std::vector< reco::GenParticle > &vetoCandidates, std::vector< std::vector< reco::GenParticle > > &extraBQuarks, std::vector< std::vector< reco::GenParticle > > &extraCQuarks);

  void getGenQuarksIntitiatingHadronization( std::vector< reco::GenParticle > genParticles, std::vector< reco::GenParticle > &target );

  bool isGoodGenJet(reco::GenJet genJet, double cut_pt, double cut_eta);

  void getGoodGenJets(std::vector< reco::GenJet > genJets,  std::vector< reco::GenJet > &target, double cut_pt, double cut_eta);

  void getGoodGenLeptons(std::vector< reco::GenParticle > genParticles,  std::vector< reco::GenParticle > &target, bool getTightLeptons, bool getLooseLeptons, double cut_tight_pt, double cut_tight_eta, double cut_loose_pt, double cut_loose_eta);

  void getGenJetsMatchedToTTbarSystem(std::vector< reco::GenParticle > ttbar_system_hadrons, std::vector< reco::GenJet > genJets, double deltaR_match, bool topIsLeptonic, reco::GenParticle b_quark_fromTop_hadrons, reco::GenParticle wDaughter1_fromTop_hadrons, reco::GenParticle wDaughter2_fromTop_hadrons, bool topBarIsLeptonic, reco::GenParticle b_quark_fromTopBar_hadrons, reco::GenParticle wDaughter1_fromTopBar_hadrons, reco::GenParticle wDaughter2_fromTopBar_hadrons, reco::GenJet &genJet_b_quark_fromTop, reco::GenJet &genJet_wDaughter1_fromTop, reco::GenJet &genJet_wDaughter2_fromTop, reco::GenJet &genJet_b_quark_fromTopBar, reco::GenJet &genJet_wDaughter1_fromTopBar, reco::GenJet &genJet_wDaughter2_fromTopBar);

  void getGenJetsMatchedToExtraQuarks(std::vector< reco::GenParticle > extraQuarks, std::vector< reco::GenJet > genJets, std::vector< reco::GenJet > genJet_matchedToTTbarSystem, double deltaR_match, std::vector< reco::GenJet > &genJet_matchedToExtraBQuarks, std::vector< reco::GenJet > &genJet_matchedToExtraCQuarks);


}; // End of Class Prototype


#endif // _yggdrasil_NLO_h
