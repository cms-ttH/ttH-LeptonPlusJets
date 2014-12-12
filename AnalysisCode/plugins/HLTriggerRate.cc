// -*- C++ -*-
//
// Package:    ttH-LeptonPlusJets/HLTriggerRate
// Class:      HLTriggerRate
// 
/**\class HLTriggerRate HLTriggerRate.cc ttH-LeptonPlusJets/AnalysisCode/plugins/HLTriggerRate.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Darren Puigh
//         Created:  Wed, 02 Jul 2014 20:01:00 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerEvmReadoutRecord.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TLorentzVector.h"


//
// class declaration
//

class HLTriggerRate : public edm::EDAnalyzer {
   public:
      explicit HLTriggerRate(const edm::ParameterSet&);
      ~HLTriggerRate();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      virtual void beginRun(edm::Run const& iRun,edm::EventSetup const& iSetup) override;
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
  edm::EDGetTokenT<L1GlobalTriggerReadoutRecord> gtReadoutRecordToken;

  edm::EDGetTokenT <edm::TriggerResults> triggerResultsToken;
  edm::EDGetTokenT <trigger::TriggerEvent> triggerEventToken;

  bool verbose_;
  bool dumpHLT_;

  int numEvents_;
  int numEvents_passL1_;

  HLTConfigProvider hlt_config_;

  std::string hltTag;
  std::map<std::string, int> hlt_cppath_;
  std::map<std::string, int> hlt_cppath_unprescale_;
  std::map<std::string, int> hlt_cppath_perevent_;
  std::map<std::string, int> hlt_selected_cppath_;
  std::map<std::string, int> hlt_selected_pure_cppath_;
  std::map<std::string, int> hlt_selected_pure_noHLTEle32_cppath_;
  std::map<std::string, int> hlt_prescale_;
  std::map<std::string, int> hlt_initial_prescale_;


  std::map<std::string, int> l1talgo_cppath_;
  std::map<std::string, int> l1talgo_cppath_unprescale_;
  std::map<std::string, int> l1talgo_prescale_;
  std::map<std::string, int> l1talgo_initial_prescale_;


  std::vector<std::string> hlt_triggerNames_;

  std::vector<std::string> hlt_selected_triggerNames_;

  edm::Service<TFileService> fs_;

  // Declare histograms
  TH1D *h_hlt;
  TH1D *h_hlt_overlapEGHTT;
  TH1D *h_hlt_selected;
  TH1D *h_hlt_selected_all;
  TH1D *h_hlt_selected_pure;
  TH1D *h_hlt_selected_pure_noHLTEle32;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HLTriggerRate::HLTriggerRate(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  verbose_ = false;
  dumpHLT_ = false;

  hltTag = "TEST";

  triggerResultsToken = consumes <edm::TriggerResults> (edm::InputTag(std::string("TriggerResults"), std::string(""), hltTag));
  triggerEventToken = consumes <trigger::TriggerEvent> (edm::InputTag(std::string("hltTriggerSummaryAOD"), std::string(""), hltTag));

  gtReadoutRecordToken = consumes <L1GlobalTriggerReadoutRecord> (edm::InputTag(std::string("simGtDigis"), std::string(""), hltTag));

}


HLTriggerRate::~HLTriggerRate()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
HLTriggerRate::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  numEvents_++;


  h_hlt->Fill(0.,1);


  edm::Handle<L1GlobalTriggerReadoutRecord> gtReadoutRecord;
  iEvent.getByToken(gtReadoutRecordToken, gtReadoutRecord);

  edm::Handle<edm::TriggerResults> triggerResults;
  iEvent.getByToken(triggerResultsToken, triggerResults);


  // Get Trigger and Event Handles
  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  const L1GtTriggerMenu* menu = menuRcd.product();

  bool passL1 = false;
  if( gtReadoutRecord.isValid() ){
    const DecisionWord& gtDecisionWord = gtReadoutRecord->decisionWord();

    // L1 algos
    for( CItAlgo algo = menu->gtAlgorithmMap().begin(); algo!=menu->gtAlgorithmMap().end(); ++algo) {
      int algoBitNumber = (algo->second).algoBitNumber();

      std::string name = (algo->second).algoName();
      bool accept = gtDecisionWord.at(algoBitNumber);
      int initial_prescale = l1talgo_initial_prescale_[name];
      bool isMasked = ( initial_prescale<1 );
      int prescale = l1talgo_prescale_[name];

      if( !accept ) continue;
      l1talgo_cppath_unprescale_[name]+=1;

      if( !isMasked ){
	if( prescale==1 ){
	  passL1 = true;
	  l1talgo_cppath_[name]+=1;
	  //if( !(name=="L1_IsoEG20er_TauJet20er") ){
	}
	if( initial_prescale>1 ){
	  if( prescale==1 ) l1talgo_prescale_[name] = initial_prescale;
	  else              l1talgo_prescale_[name]--;
	}
      }
    }
  }


  if( passL1 ) numEvents_passL1_++;

  // require l1 to fire!
  //if( !passL1 ) return;

  // zero counts per path for each event
  for( std::map<std::string, int>::const_iterator iter = hlt_cppath_perevent_.begin(); iter != hlt_cppath_perevent_.end(); iter++){
    std::string name = iter->first;
    hlt_cppath_perevent_[name] = 0;
  }

  if( triggerResults.isValid() ){
    std::vector<std::string> triggerNames = hlt_config_.triggerNames();

    for( unsigned int iPath=0; iPath<triggerNames.size(); iPath++ ){
      std::string pathName = triggerNames[iPath];
      unsigned int hltIndex = hlt_config_.triggerIndex(pathName);

      if( hltIndex >= triggerResults->size() ) continue;

      int accept = triggerResults->accept(hltIndex);
      ////int prescale = -1;//hlt_config_.prescaleValue(iEvent, iSetup, pathName);
      int initial_prescale = hlt_initial_prescale_[pathName];
      bool isMasked = ( initial_prescale<1 );
      int prescale = hlt_prescale_[pathName];

      if( verbose_ && dumpHLT_ ) std::cout << " =====>  HLT: path name = " << pathName << ",\t prescale = " << prescale << ",\t pass = " << accept << std::endl; 

      if( true ){
	TAxis * axis = h_hlt_selected_all->GetXaxis();
	if( !axis ) continue;
	int bin_num = axis->FindBin(pathName.c_str());
	int bn = bin_num - 1;
	h_hlt_selected_all->Fill(bn, 1);
      }

      if( !accept ) continue;
      hlt_cppath_unprescale_[pathName]+=1;

      if( !isMasked ){
	if( prescale==1 ){
	  hlt_cppath_[pathName]+=1;
	  hlt_cppath_perevent_[pathName]+=1;
	}
	if( initial_prescale>1 ){
	  if( prescale==1 ) hlt_prescale_[pathName] = initial_prescale;
	  else              hlt_prescale_[pathName]--;
	}
      }

      if( true ){
	TAxis * axis = h_hlt->GetXaxis();
	if( !axis ) continue;
	int bin_num = axis->FindBin(pathName.c_str());
	int bn = bin_num - 1;
	h_hlt->Fill(bn, 1);
      }

      if( true ){
	TAxis * axis = h_hlt_selected->GetXaxis();
	if( !axis ) continue;
	int bin_num = axis->FindBin(pathName.c_str());
	int bn = bin_num - 1;
	h_hlt_selected->Fill(bn, 1);
      }
    }
  }
  else{
    std::cout << "Trigger results not valid for tag " << hltTag << std::endl;
  }


  unsigned int numHLT_selected = hlt_selected_triggerNames_.size();
  for( unsigned int iPath=0; iPath<numHLT_selected; iPath++ ){
    std::string pathName = hlt_selected_triggerNames_[iPath];

    bool accept = ( hlt_cppath_perevent_[pathName]==1 );

    if( !accept ) continue; 

    hlt_selected_cppath_[pathName]+=1;

    bool passesAnotherHLT = false;
    bool passesHLTEle32paths = false;
    bool passesHLTEle27paths = false;
    for( std::map<std::string, int>::const_iterator iter = hlt_cppath_perevent_.begin(); iter != hlt_cppath_perevent_.end(); iter++){
      std::string pathName2 = iter->first;
      if( pathName==pathName2 ) continue;

      if( iter->second > 0 ){
	passesAnotherHLT = true;

	if( (pathName2.find("HLT_Ele32_eta2p1_WP85_Gsf")!=std::string::npos) ) passesHLTEle32paths = true;
	if( (pathName2.find("HLT_Ele27_eta2p1_WP85_Gsf")!=std::string::npos) ) passesHLTEle27paths = true;

	if( pathName=="HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1" ){
	  TAxis * axis = h_hlt_overlapEGHTT->GetXaxis();
	  if( !axis ) continue;
	  int bin_num = axis->FindBin(pathName2.c_str());
	  int bn = bin_num - 1;
	  h_hlt_overlapEGHTT->Fill(bn, 1);
	}
      }
    }
    if( !passesAnotherHLT ){ 
      hlt_selected_pure_cppath_[pathName]+=1;

      TAxis * axis = h_hlt_selected_pure->GetXaxis();
      if( !axis ) continue;
      int bin_num = axis->FindBin(pathName.c_str());
      int bn = bin_num - 1;
      h_hlt_selected_pure->Fill(bn, 1);
    }
    if( !passesHLTEle32paths && !passesHLTEle27paths ){ 
      hlt_selected_pure_noHLTEle32_cppath_[pathName]+=1;

      TAxis * axis = h_hlt_selected_pure_noHLTEle32->GetXaxis();
      if( !axis ) continue;
      int bin_num = axis->FindBin(pathName.c_str());
      int bn = bin_num - 1;
      h_hlt_selected_pure_noHLTEle32->Fill(bn, 1);
    }
  }


}


// ------------ method called once each job just before starting event loop  ------------
void 
HLTriggerRate::beginJob()
{

  TH1::SetDefaultSumw2(true);

  numEvents_        = 0;
  numEvents_passL1_ = 0;

  hlt_selected_triggerNames_.push_back("HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1");
  hlt_selected_triggerNames_.push_back("HLT_Ele27_eta2p1_WP85_Gsf_v1");
  hlt_selected_triggerNames_.push_back("HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1");
  hlt_selected_triggerNames_.push_back("HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet40_v1");
  hlt_selected_triggerNames_.push_back("HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1");
  hlt_selected_triggerNames_.push_back("HLT_Ele32_eta2p1_WP85_Gsf_v1");
  hlt_selected_triggerNames_.push_back("HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1");
  hlt_selected_triggerNames_.push_back("HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet40_v1");
  hlt_selected_triggerNames_.push_back("HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
HLTriggerRate::endJob() 
{

  std::cout << "***************************************************************" << std::endl;
  std::cout << "  Summary for L1T Algo: Total number of events = " << numEvents_ << ",\t number of events passing L1 = " << numEvents_passL1_ << std::endl;
  int bit=0;
  for( std::map<std::string, int>::const_iterator iter = l1talgo_cppath_.begin(); iter != l1talgo_cppath_.end(); iter++){
    std::string name = iter->first;
    double eff = 100*double(iter->second)/double(numEvents_);
    int unprescaled_count = l1talgo_cppath_unprescale_[name];
    printf("\t %d,\t %s,\t %d,\t %.1f,\t %d \n",bit++, name.c_str(), iter->second, eff, unprescaled_count);
  }  
  std::cout << "***************************************************************" << std::endl;
  std::cout << "  Summary for HLT: Total number of events = " << numEvents_ << ",\t number of events passing L1 = " << numEvents_passL1_ << std::endl;
  for( std::map<std::string, int>::const_iterator iter = hlt_cppath_.begin(); iter != hlt_cppath_.end(); iter++){
    std::string name = iter->first;
    double eff = 100*double(iter->second)/double(numEvents_);
    int unprescaled_count = hlt_cppath_unprescale_[name];
    printf("\t %d,\t %s,\t %d,\t %.1f,\t %d \n",bit++, name.c_str(), iter->second, eff, unprescaled_count);
  }
  std::cout << "***************************************************************" << std::endl;
  std::cout << "  Summary for selected HLT paths: Total number of events = " << numEvents_ << ",\t number of events passing L1 = " << numEvents_passL1_ << std::endl;
  for( std::map<std::string, int>::const_iterator iter = hlt_selected_cppath_.begin(); iter != hlt_selected_cppath_.end(); iter++){
    std::string name = iter->first;
    int counts = iter->second;
    double eff = 100*double(counts)/double(numEvents_);
    int pure_counts = hlt_selected_pure_cppath_[name];
    double pure_eff = 100*double(pure_counts)/double(numEvents_);
    int pure_noHLTEle32_counts = hlt_selected_pure_noHLTEle32_cppath_[name];
    double pure_noHLTEle32_eff = 100*double(pure_noHLTEle32_counts)/double(numEvents_);
    printf("\t %s,\t %d,\t %.1f,\t %d,\t %.1f,\t %d,\t %.1f \n", name.c_str(), counts, eff, pure_counts, pure_eff, pure_noHLTEle32_counts, pure_noHLTEle32_eff);
  }
  std::cout << "***************************************************************" << std::endl;


}

// ------------ method called when starting to processes a run  ------------
/*
void 
HLTriggerRate::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
void 
HLTriggerRate::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{

  bool hltchanged = true;
  if (!hlt_config_.init(iRun, iSetup, hltTag, hltchanged)) {
    std::cout << "Warning, didn't find trigger process HLT,\t" << hltTag << std::endl;
    return;
  }

 edm::ESHandle<L1GtTriggerMenu> menuRcd;
  iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  const L1GtTriggerMenu* menu = menuRcd.product();

  // L1 algos
  for( CItAlgo algo = menu->gtAlgorithmMap().begin(); algo!=menu->gtAlgorithmMap().end(); ++algo) {
    std::string name = (algo->second).algoName();
    l1talgo_cppath_[name] = 0;
    l1talgo_cppath_unprescale_[name] = 0;
    l1talgo_prescale_[name] = 0;
    l1talgo_initial_prescale_[name] = 0;
  }


  l1talgo_initial_prescale_["L1_AlwaysTrue"] = -1;
  l1talgo_initial_prescale_["L1_DoubleEG_15_10"] = 1;
  l1talgo_initial_prescale_["L1_DoubleEG_20_10_1LegIso"] = 1;
  l1talgo_initial_prescale_["L1_DoubleEG_22_10"] = 1;
  l1talgo_initial_prescale_["L1_DoubleMu0_Eta1p6_HighQ_WdEta18"] = 1;
  l1talgo_initial_prescale_["L1_DoubleMu0_Eta1p6_HighQ_WdEta18_OS"] = 1;
  l1talgo_initial_prescale_["L1_DoubleMu0_HighQ"] = 5000;
  l1talgo_initial_prescale_["L1_DoubleMu6_EG6"] = 1;
  l1talgo_initial_prescale_["L1_DoubleMu7_EG7"] = 1;
  l1talgo_initial_prescale_["L1_DoubleMu_10_0_HighQ_WdEta18"] = 1;
  l1talgo_initial_prescale_["L1_DoubleMu_10_3p5"] = 1;
  l1talgo_initial_prescale_["L1_DoubleMu_10_Open"] = 500;
  l1talgo_initial_prescale_["L1_DoubleMu_12_5"] = 1;
  l1talgo_initial_prescale_["L1_DoubleS1Jet36_CenFor_NotWdPhi6"] = -1;
  l1talgo_initial_prescale_["L1_DoubleS1Jet36_CenFor_NotWdPhi7"] = -1;
  l1talgo_initial_prescale_["L1_DoubleS1JetC100"] = 1;
  l1talgo_initial_prescale_["L1_DoubleS1JetC112"] = 1;
  l1talgo_initial_prescale_["L1_DoubleS1JetC120"] = 1;
  l1talgo_initial_prescale_["L1_DoubleS1JetC32_WdPhi7_HTT125"] = 50;
  l1talgo_initial_prescale_["L1_DoubleS1JetC52"] = 5000;
  l1talgo_initial_prescale_["L1_DoubleS1JetC56_ETM60"] = 1;
  l1talgo_initial_prescale_["L1_DoubleS1JetC60_ETM60"] = 1;
  l1talgo_initial_prescale_["L1_DoubleS1JetC84"] = 1;
  l1talgo_initial_prescale_["L1_DoubleTauJet36_Eta2p17"] = 1;
  l1talgo_initial_prescale_["L1_DoubleTauJet52er"] = -1;
  l1talgo_initial_prescale_["L1_DoubleTauJet68er"] = -1;
  l1talgo_initial_prescale_["L1_ETM100"] = 1;
  l1talgo_initial_prescale_["L1_ETM30"] = 20000;
  l1talgo_initial_prescale_["L1_ETM40"] = 4000;
  l1talgo_initial_prescale_["L1_ETM50"] = 1;
  l1talgo_initial_prescale_["L1_ETM60"] = 1;
  l1talgo_initial_prescale_["L1_ETM60_NotS1Jet52WdPhi2"] = 1;
  l1talgo_initial_prescale_["L1_ETM70"] = 1;
  l1talgo_initial_prescale_["L1_ETM70_NotS1Jet52WdPhi2"] = 1;
  l1talgo_initial_prescale_["L1_HTT125"] = 1;
  l1talgo_initial_prescale_["L1_HTT150"] = 1;
  l1talgo_initial_prescale_["L1_HTT175"] = 1;
  l1talgo_initial_prescale_["L1_HTT200"] = 1;
  l1talgo_initial_prescale_["L1_HTT250"] = 1;
  l1talgo_initial_prescale_["L1_IsoEG20er_TauJet20er"] = 1;
  l1talgo_initial_prescale_["L1_Mu0er_ETM55"] = 1;
  l1talgo_initial_prescale_["L1_Mu0er_ETM55_HTT100"] = -1;
  l1talgo_initial_prescale_["L1_Mu0er_ETM55_JetC120"] = -1;
  l1talgo_initial_prescale_["L1_Mu10er_ETM50"] = 1;
  l1talgo_initial_prescale_["L1_Mu14er_ETM30"] = 1;
  l1talgo_initial_prescale_["L1_Mu16er_TauJet20er"] = 1;
  l1talgo_initial_prescale_["L1_Mu20_EG10"] = 1;
  l1talgo_initial_prescale_["L1_Mu20_EG8"] = 1;
  l1talgo_initial_prescale_["L1_Mu3_S1JetC16_WdEtaPhi2"] = 250;
  l1talgo_initial_prescale_["L1_Mu3_S1JetC52_WdEtaPhi2"] = 50;
  l1talgo_initial_prescale_["L1_Mu3_S1JetC92_WdEtaPhi2"] = 262139;
  l1talgo_initial_prescale_["L1_Mu4_EG18"] = 1;
  l1talgo_initial_prescale_["L1_Mu5_DoubleEG5"] = 262139;
  l1talgo_initial_prescale_["L1_Mu5_EG20"] = 1;
  l1talgo_initial_prescale_["L1_Mu5_IsoEG18"] = 1;
  l1talgo_initial_prescale_["L1_Mu6_DoubleEG10"] = 1;
  l1talgo_initial_prescale_["L1_Mu6_HTT150"] = 1;
  l1talgo_initial_prescale_["L1_QuadMu0_HighQ"] = 1;
  l1talgo_initial_prescale_["L1_QuadS1JetC36_TauJet52"] = 10;
  l1talgo_initial_prescale_["L1_QuadS1JetC40"] = 1;
  l1talgo_initial_prescale_["L1_QuadS1JetC60"] = 1; 
  l1talgo_initial_prescale_["L1_QuadS1JetC84"] = 1;
  l1talgo_initial_prescale_["L1_SingleEG10"] = 10000;
  l1talgo_initial_prescale_["L1_SingleEG20"] = 2000;
  l1talgo_initial_prescale_["L1_SingleEG25er_HTT125"] = 1;
  l1talgo_initial_prescale_["L1_SingleEG25"] = 1;
  l1talgo_initial_prescale_["L1_SingleEG30"] = 1;
  l1talgo_initial_prescale_["L1_SingleEG35"] = 1;
  l1talgo_initial_prescale_["L1_SingleEG35er"] = 1;
  l1talgo_initial_prescale_["L1_SingleEG5"] = 262139;
  l1talgo_initial_prescale_["L1_SingleIsoEG18"] = 2000;
  l1talgo_initial_prescale_["L1_SingleIsoEG25er"] = 1;
  l1talgo_initial_prescale_["L1_SingleIsoEG28er"] = 1;
  l1talgo_initial_prescale_["L1_SingleIsoEG30er"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu12"] = 500;
  l1talgo_initial_prescale_["L1_SingleMu14_Eta2p1"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu16"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu16_Eta2p1"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu18er"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu20"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu20er"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu25"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu25er"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu30"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu30er"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu5"] = 2500;
  l1talgo_initial_prescale_["L1_SingleMu6_NotBptxOR"] = -1;
  l1talgo_initial_prescale_["L1_SingleMuBeamHalo"] = -1;
  l1talgo_initial_prescale_["L1_SingleMuOpen"] = 262139;
  l1talgo_initial_prescale_["L1_SingleS1Jet128"] = 1;
  l1talgo_initial_prescale_["L1_SingleS1Jet176"] = 1;
  l1talgo_initial_prescale_["L1_SingleS1Jet200"] = 1;
  l1talgo_initial_prescale_["L1_SingleS1Jet240"] = 1;
  l1talgo_initial_prescale_["L1_SingleS1Jet52"] = 30000;
  l1talgo_initial_prescale_["L1_SingleS1Jet68"] = 20000;
  l1talgo_initial_prescale_["L1_SingleS1Jet92"] = 2000;
  l1talgo_initial_prescale_["L1_SingleS1JetC20_NotBptxOR"] = -1;
  l1talgo_initial_prescale_["L1_SingleS1JetC32_NotBptxOR"] = -1;
  l1talgo_initial_prescale_["L1_TripleEG10"] = -1;
  l1talgo_initial_prescale_["L1_TripleEG_14_10_8"] = 1;
  l1talgo_initial_prescale_["L1_TripleMu0_HighQ"] = 1;
  l1talgo_initial_prescale_["L1_TripleMu_5_5_3_HighQ"] = 1;
  l1talgo_initial_prescale_["L1_TripleS1Jet_92_76_64_VBF"] = 1;
  l1talgo_initial_prescale_["L1_ZeroBias"] = 32000;


  /*
    /// OLD

  l1talgo_initial_prescale_["L1_AlwaysTrue"] = -1;
  l1talgo_initial_prescale_["L1_DoubleEG_15_10"] = 1000;
  l1talgo_initial_prescale_["L1_DoubleEG_20_10_1LegIso"] = 1;
  l1talgo_initial_prescale_["L1_DoubleEG_22_10"] = 1;
  l1talgo_initial_prescale_["L1_DoubleMu0_Eta1p6_HighQ_WdEta18"] = 2000;
  l1talgo_initial_prescale_["L1_DoubleMu0_Eta1p6_HighQ_WdEta18_OS"] = 1;
  l1talgo_initial_prescale_["L1_DoubleMu0_HighQ"] = 10000;
  l1talgo_initial_prescale_["L1_DoubleMu6_EG6"] = 10000;
  l1talgo_initial_prescale_["L1_DoubleMu7_EG7"] = 1;
  l1talgo_initial_prescale_["L1_DoubleMu_10_0_HighQ_WdEta18"] = 1;
  l1talgo_initial_prescale_["L1_DoubleMu_10_3p5"] = 1000;
  l1talgo_initial_prescale_["L1_DoubleMu_10_Open"] = 2000;
  l1talgo_initial_prescale_["L1_DoubleMu_12_5"] = 1;
  l1talgo_initial_prescale_["L1_DoubleS1Jet36_CenFor_NotWdPhi6"] = -1;
  l1talgo_initial_prescale_["L1_DoubleS1Jet36_CenFor_NotWdPhi7"] = -1;
  l1talgo_initial_prescale_["L1_DoubleS1JetC100"] = 1;
  l1talgo_initial_prescale_["L1_DoubleS1JetC112"] = 1;
  l1talgo_initial_prescale_["L1_DoubleS1JetC120"] = 1;
  l1talgo_initial_prescale_["L1_DoubleS1JetC32_WdPhi7_HTT125"] = 100;
  l1talgo_initial_prescale_["L1_DoubleS1JetC52"] = 10000;
  l1talgo_initial_prescale_["L1_DoubleS1JetC56_ETM60"] = 1;
  l1talgo_initial_prescale_["L1_DoubleS1JetC60_ETM60"] = 1;
  l1talgo_initial_prescale_["L1_DoubleS1JetC84"] = 700;
  l1talgo_initial_prescale_["L1_DoubleTauJet36_Eta2p17"] = 1;
  l1talgo_initial_prescale_["L1_DoubleTauJet52er"] = 200;
  l1talgo_initial_prescale_["L1_DoubleTauJet68er"] = 1;
  l1talgo_initial_prescale_["L1_ETM100"] = 1;
  l1talgo_initial_prescale_["L1_ETM30"] = 40000;
  l1talgo_initial_prescale_["L1_ETM40"] = 8000;
  l1talgo_initial_prescale_["L1_ETM50"] = 1000;
  l1talgo_initial_prescale_["L1_ETM60"] = 900;
  l1talgo_initial_prescale_["L1_ETM60_NotS1Jet52WdPhi2"] = 1;
  l1talgo_initial_prescale_["L1_ETM70"] = 1;
  l1talgo_initial_prescale_["L1_ETM70_NotS1Jet52WdPhi2"] = 1;
  l1talgo_initial_prescale_["L1_HTT125"] = 1000;
  l1talgo_initial_prescale_["L1_HTT150"] = 800;
  l1talgo_initial_prescale_["L1_HTT175"] = 1;
  l1talgo_initial_prescale_["L1_HTT200"] = 1;
  l1talgo_initial_prescale_["L1_HTT250"] = 1;
  l1talgo_initial_prescale_["L1_IsoEG20er_TauJet20er"] = 1;
  l1talgo_initial_prescale_["L1_Mu0er_ETM55"] = -1;
  l1talgo_initial_prescale_["L1_Mu0er_ETM55_HTT100"] = -1;
  l1talgo_initial_prescale_["L1_Mu0er_ETM55_JetC120"] = -1;
  l1talgo_initial_prescale_["L1_Mu10er_ETM50"] = 1;
  l1talgo_initial_prescale_["L1_Mu14er_ETM30"] = 1;
  l1talgo_initial_prescale_["L1_Mu16er_TauJet20er"] = 1;
  l1talgo_initial_prescale_["L1_Mu20_EG10"] = 1;
  l1talgo_initial_prescale_["L1_Mu20_EG8"] = 10000;
  l1talgo_initial_prescale_["L1_Mu3_S1JetC16_WdEtaPhi2"] = 1000;
  l1talgo_initial_prescale_["L1_Mu3_S1JetC52_WdEtaPhi2"] = 200;
  l1talgo_initial_prescale_["L1_Mu3_S1JetC92_WdEtaPhi2"] = 262139;
  l1talgo_initial_prescale_["L1_Mu4_EG18"] = 10000;
  l1talgo_initial_prescale_["L1_Mu5_DoubleEG5"] = 10000;
  l1talgo_initial_prescale_["L1_Mu5_EG20"] = 1;
  l1talgo_initial_prescale_["L1_Mu5_IsoEG18"] = 1;
  l1talgo_initial_prescale_["L1_Mu6_DoubleEG10"] = 1;
  l1talgo_initial_prescale_["L1_Mu6_HTT150"] = 1;
  l1talgo_initial_prescale_["L1_QuadMu0_HighQ"] = 1;
  l1talgo_initial_prescale_["L1_QuadS1JetC36_TauJet52"] = 20;
  l1talgo_initial_prescale_["L1_QuadS1JetC40"] = 1000;
  l1talgo_initial_prescale_["L1_QuadS1JetC60"] = 1; 
  l1talgo_initial_prescale_["L1_QuadS1JetC84"] = 1;
  l1talgo_initial_prescale_["L1_SingleEG10"] = 20000;
  l1talgo_initial_prescale_["L1_SingleEG20"] = 4000;
  l1talgo_initial_prescale_["L1_SingleEG25er_HTT125"] = 1;
  l1talgo_initial_prescale_["L1_SingleEG25"] = 1; //1000
  l1talgo_initial_prescale_["L1_SingleEG30"] = 700;
  l1talgo_initial_prescale_["L1_SingleEG35"] = 1;
  l1talgo_initial_prescale_["L1_SingleEG35er"] = 1;
  l1talgo_initial_prescale_["L1_SingleEG5"] = 262139;
  l1talgo_initial_prescale_["L1_SingleIsoEG18"] = 3000;
  l1talgo_initial_prescale_["L1_SingleIsoEG25er"] = 1000;
  l1talgo_initial_prescale_["L1_SingleIsoEG28er"] = 1000;
  l1talgo_initial_prescale_["L1_SingleIsoEG30er"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu12"] = 3000;
  l1talgo_initial_prescale_["L1_SingleMu14_Eta2p1"] = 1000;
  l1talgo_initial_prescale_["L1_SingleMu16"] = 1000;
  l1talgo_initial_prescale_["L1_SingleMu16_Eta2p1"] = 1000;
  l1talgo_initial_prescale_["L1_SingleMu18er"] = 700;
  l1talgo_initial_prescale_["L1_SingleMu20"] = 700;
  l1talgo_initial_prescale_["L1_SingleMu20er"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu25"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu25er"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu30"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu30er"] = 1;
  l1talgo_initial_prescale_["L1_SingleMu5"] = 50000;
  l1talgo_initial_prescale_["L1_SingleMu6_NotBptxOR"] = -1;
  l1talgo_initial_prescale_["L1_SingleMuBeamHalo"] = -1;
  l1talgo_initial_prescale_["L1_SingleMuOpen"] = 262139;
  l1talgo_initial_prescale_["L1_SingleS1Jet128"] = 1000;
  l1talgo_initial_prescale_["L1_SingleS1Jet176"] = 300;
  l1talgo_initial_prescale_["L1_SingleS1Jet200"] = 1;
  l1talgo_initial_prescale_["L1_SingleS1Jet240"] = 1;
  l1talgo_initial_prescale_["L1_SingleS1Jet52"] = 50000;
  l1talgo_initial_prescale_["L1_SingleS1Jet68"] = 40000;
  l1talgo_initial_prescale_["L1_SingleS1Jet92"] = 4000;
  l1talgo_initial_prescale_["L1_SingleS1JetC20_NotBptxOR"] = -1;
  l1talgo_initial_prescale_["L1_SingleS1JetC32_NotBptxOR"] = -1;
  l1talgo_initial_prescale_["L1_TripleEG10"] = -1;
  l1talgo_initial_prescale_["L1_TripleEG_14_10_8"] = 1;
  l1talgo_initial_prescale_["L1_TripleMu0_HighQ"] = 100;
  l1talgo_initial_prescale_["L1_TripleMu_5_5_3_HighQ"] = 1;
  l1talgo_initial_prescale_["L1_TripleS1Jet_92_76_64_VBF"] = 1;
  l1talgo_initial_prescale_["L1_ZeroBias"] = 32000;

    */

  for( std::map<std::string, int>::const_iterator iter = l1talgo_initial_prescale_.begin(); iter != l1talgo_initial_prescale_.end(); iter++){
    std::string name = iter->first;
    l1talgo_prescale_[name] = iter->second;
  }


  // Zero out map
  std::vector<std::string> triggerNames = hlt_config_.triggerNames();

  hlt_triggerNames_.clear();

  hlt_triggerNames_.push_back("All");
  std::string prefix = "HLT_";
  for( unsigned int iPath=0; iPath<triggerNames.size(); iPath++ ){
    std::string pathName = triggerNames[iPath];
    //std::string pathNameNoVer = hlt_config_.removeVersion(name);
    hlt_cppath_[pathName] = 0;
    hlt_cppath_unprescale_[pathName] = 0;
    hlt_cppath_perevent_[pathName] = 0;
    hlt_prescale_[pathName] = 0;
    hlt_initial_prescale_[pathName] = 0;
    if( pathName.compare(0, prefix.length(), prefix) == 0 ) hlt_triggerNames_.push_back(pathName);
  }

  unsigned int numHLT = hlt_triggerNames_.size();

  h_hlt = fs_->make<TH1D>("h_hlt",";HLT path", numHLT , 0 , numHLT );
  h_hlt_overlapEGHTT = fs_->make<TH1D>("h_hlt_overlapEGHTT",";HLT path", numHLT , 0 , numHLT );

  for( unsigned int iPath=0; iPath<numHLT; iPath++ ){
    std::string pathName = hlt_triggerNames_[iPath];
    int jPath = iPath+1;
    if( h_hlt ){
      TAxis * axis = h_hlt->GetXaxis();
      if( axis ) axis->SetBinLabel(jPath, pathName.c_str());
    }
    if( h_hlt_overlapEGHTT ){
      TAxis * axis = h_hlt_overlapEGHTT->GetXaxis();
      if( axis ) axis->SetBinLabel(jPath, pathName.c_str());
    }
  }


  unsigned int numHLT_selected = hlt_selected_triggerNames_.size();

  h_hlt_selected = fs_->make<TH1D>("h_hlt_selected",";HLT path", numHLT_selected , 0 , numHLT_selected );
  h_hlt_selected_all = fs_->make<TH1D>("h_hlt_selected_all",";HLT path", numHLT_selected , 0 , numHLT_selected );
  h_hlt_selected_pure = fs_->make<TH1D>("h_hlt_selected_pure",";HLT path", numHLT_selected , 0 , numHLT_selected );
  h_hlt_selected_pure_noHLTEle32 = fs_->make<TH1D>("h_hlt_selected_pure_noHLTEle32",";HLT path", numHLT_selected , 0 , numHLT_selected );

  for( unsigned int iPath=0; iPath<numHLT_selected; iPath++ ){
    std::string pathName = hlt_selected_triggerNames_[iPath];
    hlt_selected_cppath_[pathName] = 0;
    hlt_selected_pure_cppath_[pathName] = 0;
    hlt_selected_pure_noHLTEle32_cppath_[pathName] = 0;
    int jPath = iPath+1;
    if( h_hlt_selected ){
      TAxis * axis = h_hlt_selected->GetXaxis();
      if( axis ) axis->SetBinLabel(jPath, pathName.c_str());
    }
    if( h_hlt_selected_pure ){
      TAxis * axis = h_hlt_selected_pure->GetXaxis();
      if( axis ) axis->SetBinLabel(jPath, pathName.c_str());
    }
    if( h_hlt_selected_pure_noHLTEle32 ){
      TAxis * axis = h_hlt_selected_pure_noHLTEle32->GetXaxis();
      if( axis ) axis->SetBinLabel(jPath, pathName.c_str());
    }
    if( h_hlt_selected_all ){
      TAxis * axis = h_hlt_selected_all->GetXaxis();
      if( axis ) axis->SetBinLabel(jPath, pathName.c_str());
    }
  }


  hlt_initial_prescale_["HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV0p3_v1"] = 1;
  hlt_initial_prescale_["HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v1"] = 1;
  hlt_initial_prescale_["HLT_AK8PFJet360TrimMod_Mass30_v1"] = 1;
  hlt_initial_prescale_["HLT_BTagCSV07_v1"] = 1; // not in google doc
  hlt_initial_prescale_["HLT_BTagMu_DiJet110_Mu5_v1"] = 100;
  hlt_initial_prescale_["HLT_BTagMu_DiJet20_Mu5_v1"] = 500;
  hlt_initial_prescale_["HLT_BTagMu_DiJet40_Mu5_v1"] = 500;
  hlt_initial_prescale_["HLT_BTagMu_DiJet70_Mu5_v1"] = 100;
  hlt_initial_prescale_["HLT_BTagMu_Jet300_Mu5_v1"] = 1;
  hlt_initial_prescale_["HLT_CaloJet260_v1"] = 1; // not in google doc
  hlt_initial_prescale_["HLT_CaloJet500_NoJetID_v1"] = 1;
  hlt_initial_prescale_["HLT_CaloMHTNoPU90_PFMET90_PFMHT90_IDLoose_BTagCSV0p7_v1"] = 1;
  hlt_initial_prescale_["HLT_CaloMHTNoPU90_PFMET90_PFMHT90_IDLoose_v1"] = 25;
  hlt_initial_prescale_["HLT_DiCentralPFJet70_PFMET120_NoiseCleaned_v1"] = 1;
  hlt_initial_prescale_["HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v1"] = 1;
  hlt_initial_prescale_["HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu140_v1"] = 1;
  hlt_initial_prescale_["HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu80_v1"] = 1;
  hlt_initial_prescale_["HLT_DiPFJetAve100_HFJEC_v1"] = 15000000;
  hlt_initial_prescale_["HLT_DiPFJetAve160_HFJEC_v1"] = 24;
  hlt_initial_prescale_["HLT_DiPFJetAve220_HFJEC_v1"] = 1;
  hlt_initial_prescale_["HLT_DiPFJetAve300_HFJEC_v1"] = 1;
  hlt_initial_prescale_["HLT_DiPFJetAve30_HFJEC_v1"] = 2949063750;
  hlt_initial_prescale_["HLT_DiPFJetAve60_HFJEC_v1"] = 2949063750;
  hlt_initial_prescale_["HLT_DiPFJetAve80_HFJEC_v1"] = 75000000;
  hlt_initial_prescale_["HLT_Dimuon0_Jpsi_Muon_v1"] = 1;
  hlt_initial_prescale_["HLT_Dimuon0_Upsilon_Muon_v1"] = 1;
  hlt_initial_prescale_["HLT_Dimuon0er16_Jpsi_NoOS_NoVertexing_v1"] = 500;
  hlt_initial_prescale_["HLT_Dimuon0er16_Jpsi_NoVertexing_v1"] = 1;
  hlt_initial_prescale_["HLT_Dimuon13_PsiPrime_v1"] = 1;
  hlt_initial_prescale_["HLT_Dimuon13_Upsilon_v1"] = 1;
  hlt_initial_prescale_["HLT_Dimuon20_Jpsi_v1"] = 1;
  hlt_initial_prescale_["HLT_Dimuon6_Jpsi_NoVertexing_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleEle24_22_eta2p1_WP85_Gsf_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v1"] = 0;
  hlt_initial_prescale_["HLT_DoubleEle8_CaloIdL_TrkIdVL_Mass8_PFHT300_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleIsoMu17_eta2p1_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleJet90_Double30_DoubleCSV0p5_v1"] = 25;
  hlt_initial_prescale_["HLT_DoubleJet90_Double30_TripleCSV0p5_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_DoubleMu23NoFiltersNoVtxDisplaced_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu28NoFiltersNoVtxDisplaced_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu33NoFiltersNoVtx_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu38NoFiltersNoVtx_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu4_3_Bs_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu4_3_Jpsi_Displaced_v1"] = 2;
  hlt_initial_prescale_["HLT_DoubleMu4_JpsiTrk_Displaced_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu4_LowMassNonResonantTrk_Displaced_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu4_PsiPrimeTrk_Displaced_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu8_Mass8_PFHT300_v1"] = 1;
  hlt_initial_prescale_["HLT_DoublePhoton85_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele10_CaloId_TrackIdVL_CentralPFJet40_BTagCSV_v1"] = 3932085;
  hlt_initial_prescale_["HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele12_CaloId_TrackId_Iso_PFJet30_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele15_IsoVVVL_BTagtop8CSV07_PFHT400_v1"] = 262139;
  hlt_initial_prescale_["HLT_Ele15_IsoVVVL_PFHT400_PFMET70_v1"] = 262139;
  hlt_initial_prescale_["HLT_Ele15_IsoVVVL_PFHT600_v1"] = 262139;
  hlt_initial_prescale_["HLT_Ele15_PFHT300_v1"] = 13106950;
  hlt_initial_prescale_["HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele17_Ele12_Ele10_CaloId_TrackId_v1"] = 0;
  hlt_initial_prescale_["HLT_Ele17_Ele8_Gsf_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_Ele18_CaloId_TrackId_Iso_PFJet30_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele20WP60_Ele8_Mass55_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele22_eta2p1_WP85_Gsf_LooseIsoPFTau20_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele22_eta2p1_WP85_Gsf_v1"] = 2000;
  hlt_initial_prescale_["HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele23_CaloId_TrackId_Iso_PFJet30_v1"] = 3;
  hlt_initial_prescale_["HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v1"] = 5;
  hlt_initial_prescale_["HLT_Ele23_Ele12_CaloId_TrackId_Iso_v1"] = 0;
  hlt_initial_prescale_["HLT_Ele25WP60_SC4_Mass55_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele25_eta2p1_WP85_Gsf_PFMET80_boostedW_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele27_eta2p1_WP85_Gsf_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele27_eta2p1_WP85_Gsf_LooseIsoPFTau20_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet40_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele27_eta2p1_WP85_Gsf_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele32_eta2p1_WP85_Gsf_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele32_eta2p1_WP85_Gsf_LooseIsoPFTau20_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet40_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele32_eta2p1_WP85_Gsf_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele33_CaloId_TrackId_Iso_PFJet30_v1"] = 2;
  hlt_initial_prescale_["HLT_Ele35_eta2p1_WP85_Gsf_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele40_eta2p1_WP85_Gsf_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele95_CaloIdVT_GsfTrkIdT_v1"] = 1;
  hlt_initial_prescale_["HLT_HT200_DiJet90_AlphaT0p57_v1"] = 1;
  hlt_initial_prescale_["HLT_HT200_v1"] = 4750;
  hlt_initial_prescale_["HLT_HT250_DiJet90_AlphaT0p55_v1"] = 1;
  hlt_initial_prescale_["HLT_HT250_v1"] = 3750;
  hlt_initial_prescale_["HLT_HT300_DiJet90_AlphaT0p53_v1"] = 1;
  hlt_initial_prescale_["HLT_HT300_v1"] = 2250;
  hlt_initial_prescale_["HLT_HT350_DiJet90_AlphaT0p52_v1"] = 1;
  hlt_initial_prescale_["HLT_HT350_DisplacedDijet80_DisplacedTrack_v1"] = 1;
  hlt_initial_prescale_["HLT_HT350_DisplacedDijet80_Tight_DisplacedTrack_v1"] = 1;
  hlt_initial_prescale_["HLT_HT350_v1"] = 1500;
  hlt_initial_prescale_["HLT_HT400_DiJet90_AlphaT0p51_v1"] = 1;
  hlt_initial_prescale_["HLT_HT400_v1"] = 750;
  hlt_initial_prescale_["HLT_HT650_DisplacedDijet80_Inclusive_v1"] = 1;
  hlt_initial_prescale_["HLT_HT650_v1"] = 1;
  hlt_initial_prescale_["HLT_HT750_DisplacedDijet80_Inclusive_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu16_eta2p1_CaloMET30_LooseIsoPFTau50_Trk30_eta2p1_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu16_eta2p1_CaloMET30_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu17_eta2p1_LooseIsoPFTau20_SingleL1_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu17_eta2p1_MediumIsoPFTau40_Trk1_eta2p1_Reg_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu17_eta2p1_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu20_eta2p1_CentralPFJet30_BTagCSV_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu20_eta2p1_TriCentralPFJet40_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu20_eta2p1_TriCentralPFJet60_50_35_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu20_eta2p1_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu24_eta2p1_CentralPFJet30_BTagCSV_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu24_eta2p1_LooseIsoPFTau20_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu24_eta2p1_TriCentralPFJet40_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu24_eta2p1_TriCentralPFJet60_50_35_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu24_eta2p1_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu24_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_IsoMu27_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoTkMu20_eta2p1_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoTkMu24_eta2p1_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoTkMu24_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_IsoTkMu27_v1"] = 1;
  hlt_initial_prescale_["HLT_IterativeTracking_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_JetE30_NoBPTX3BX_NoHalo_v1"] = 10;
  hlt_initial_prescale_["HLT_JetE30_NoBPTX_v1"] = 600;
  hlt_initial_prescale_["HLT_JetE50_NoBPTX3BX_NoHalo_v1"] = 1;
  hlt_initial_prescale_["HLT_JetE70_NoBPTX3BX_NoHalo_v1"] = 1;
  hlt_initial_prescale_["HLT_L1_TripleJet_VBF_v1"] = 5000;
  hlt_initial_prescale_["HLT_L2DoubleMu23_NoVertex_v1"] = 100;
  hlt_initial_prescale_["HLT_L2DoubleMu28_NoVertex_2Cha_Angle2p5_Mass10_v1"] = 1;
  hlt_initial_prescale_["HLT_L2DoubleMu38_NoVertex_2Cha_Angle2p5_Mass10_v1"] = 1;
  hlt_initial_prescale_["HLT_L2Mu10_NoVertex_NoBPTX3BX_NoHalo_v1"] = 100;
  hlt_initial_prescale_["HLT_L2Mu10_NoVertex_NoBPTX_v1"] = 100;
  hlt_initial_prescale_["HLT_L2Mu20_NoVertex_3Sta_NoBPTX3BX_NoHalo_v1"] = 1;
  hlt_initial_prescale_["HLT_L2Mu30_NoVertex_3Sta_NoBPTX3BX_NoHalo_v1"] = 1;
  hlt_initial_prescale_["HLT_LooseIsoPFTau50_Trk30_eta2p1_MET120_v1"] = 1;
  hlt_initial_prescale_["HLT_LooseIsoPFTau50_Trk30_eta2p1_v1"] = 100;
  hlt_initial_prescale_["HLT_MET75_IsoTrk50_v1"] = 1;
  hlt_initial_prescale_["HLT_MET90_IsoTrk50_v1"] = 1;
  hlt_initial_prescale_["HLT_MonoCentralPFJet140_PFMETNoMu100_PFMHTNoMu140_NoiseCleaned_v1"] = 1;
  hlt_initial_prescale_["HLT_MonoCentralPFJet140_PFMETNoMu140_PFMHTNoMu140_NoiseCleaned_v1"] = 1;
  hlt_initial_prescale_["HLT_MonoCentralPFJet150_PFMETNoMu150_PFMHTNoMu150_NoiseCleaned_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu10_CentralPFJet40_BTagCSV_v1"] = 3932085;
  hlt_initial_prescale_["HLT_Mu12_Photon25_CaloIdL_L1ISO_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu12_Photon25_CaloIdL_L1OR_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu12_Photon25_CaloIdL_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_Mu14er_PFMET120_NoiseCleaned_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu15_IsoVVVL_BTagCSV07_PFHT400_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu15_IsoVVVL_PFHT400_PFMET70_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu15_IsoVVVL_PFHT600_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu15_PFHT300_v1"] = 50;
  hlt_initial_prescale_["HLT_Mu16_eta2p1_CaloMET30_v1"] = 110;
  hlt_initial_prescale_["HLT_Mu17_Mu8_DZ_v1"] = 50;
  hlt_initial_prescale_["HLT_Mu17_Mu8_SameSign_DPhi_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu17_Mu8_SameSign_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu17_NoFilters_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_Mu17_Photon30_CaloIdL_L1ISO_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_Mu17_Photon35_CaloIdL_L1ISO_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_Mu17_TkMu8_DZ_v1"] = 50;
  hlt_initial_prescale_["HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v1"] = 50;
  hlt_initial_prescale_["HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v1"] = 50;
  hlt_initial_prescale_["HLT_Mu17_TrkIsoVVL_v1"] = 3;
  hlt_initial_prescale_["HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu23_TrkIsoVVL_Ele12_Gsf_CaloId_TrackId_Iso_MediumWP_v1"] = 0;
  hlt_initial_prescale_["HLT_Mu24_TrkIsoVVL_v1"] = 2;
  hlt_initial_prescale_["HLT_Mu24_eta2p1_v1"] = 350;
  hlt_initial_prescale_["HLT_Mu25_TkMu0_dEta18_Onia_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu27_v1"] = 250;
  hlt_initial_prescale_["HLT_Mu28NoFiltersNoVtxDisplaced_Photon28_CaloIdL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu30_TkMu11_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu33NoFiltersNoVtxDisplaced_Photon33_CaloIdL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu34_TrkIsoVVL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu38NoFiltersNoVtx_Photon38_CaloIdL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu3er_PFHT140_PFMET125_NoiseCleaned_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu40_eta2p1_PFJet200_PFJet50_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu40_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu42NoFiltersNoVtx_Photon42_CaloIdL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu6_PFHT200_PFMET100_NoiseCleaned_BTagCSV07_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu6_PFHT200_PFMET125_NoiseCleaned_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu7p5_L2Mu2_Jpsi_v1"] = 2500;
  hlt_initial_prescale_["HLT_Mu7p5_L2Mu2_Upsilon_v1"] = 2500;
  hlt_initial_prescale_["HLT_Mu7p5_Track2_Jpsi_v1"] = 2500;
  hlt_initial_prescale_["HLT_Mu7p5_Track2_Upsilon_v1"] = 2500;
  hlt_initial_prescale_["HLT_Mu7p5_Track3p5_Jpsi_v1"] = 2500;
  hlt_initial_prescale_["HLT_Mu7p5_Track3p5_Upsilon_v1"] = 2500;
  hlt_initial_prescale_["HLT_Mu7p5_Track7_Jpsi_v1"] = 2500;
  hlt_initial_prescale_["HLT_Mu7p5_Track7_Upsilon_v1"] = 2500;
  hlt_initial_prescale_["HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu8_Ele8_CaloIdL_TrkIdVL_Mass8_PFHT300_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu8_TrkIsoVVL_Ele23_Gsf_CaloId_TrackId_Iso_MediumWP_v1"] = 0;
  hlt_initial_prescale_["HLT_Mu8_TrkIsoVVL_v1"] = 2500;
  hlt_initial_prescale_["HLT_PFHT350_PFMET120_NoiseCleaned_v1"] = 1;
  hlt_initial_prescale_["HLT_PFHT350_v1"] = 1200;
  hlt_initial_prescale_["HLT_PFHT550_4Jet_v1"] = 55;
  hlt_initial_prescale_["HLT_PFHT600_v1"] = 125;
  hlt_initial_prescale_["HLT_PFHT650_4Jet_v1"] = 45;
  hlt_initial_prescale_["HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v1"] = 1;
  hlt_initial_prescale_["HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v1"] = 1;
  hlt_initial_prescale_["HLT_PFHT650_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_PFHT750_4Jet_v1"] = 1;
  hlt_initial_prescale_["HLT_PFHT900_v1"] = 1;
  hlt_initial_prescale_["HLT_PFJet140_v1"] = 2000;
  hlt_initial_prescale_["HLT_PFJet200_v1"] = 1000;
  hlt_initial_prescale_["HLT_PFJet260_v1"] = 85;
  hlt_initial_prescale_["HLT_PFJet320_v1"] = 30;
  hlt_initial_prescale_["HLT_PFJet400_v1"] = 10;
  hlt_initial_prescale_["HLT_PFJet40_v1"] = 30000;
  hlt_initial_prescale_["HLT_PFJet450_v1"] = 1;
  hlt_initial_prescale_["HLT_PFJet500_v1"] = 1;
  hlt_initial_prescale_["HLT_PFJet60_v1"] = 30000;
  hlt_initial_prescale_["HLT_PFJet80_v1"] = 30000;
  hlt_initial_prescale_["HLT_PFMET110_PFMHT110_IDLoose_v1"] = 1;
  hlt_initial_prescale_["HLT_PFMET120_NoiseCleaned_BTagCSV07_v1"] = 1;
  hlt_initial_prescale_["HLT_PFMET120_NoiseCleaned_Mu5_v1"] = 1;
  hlt_initial_prescale_["HLT_PFMET170_NoiseCleaned_v1"] = 1;
  hlt_initial_prescale_["HLT_PFchMET90_NoiseCleaned_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_Photon120_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon120_R9Id90_HE10_Iso40_EBOnly_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon120_R9Id90_HE10_Iso40_v1"] = 2;
  hlt_initial_prescale_["HLT_Photon120_v1"] = 100;
  hlt_initial_prescale_["HLT_Photon135_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon135_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon155_R9Id90_HE10_Iso40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon155_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon20_CaloIdVL_IsoL_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"] = 2000;
  hlt_initial_prescale_["HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_VBF_v1"] = 2000;
  hlt_initial_prescale_["HLT_Photon22_R9Id90_HE10_Iso40_v1"] = 2000;
  hlt_initial_prescale_["HLT_Photon22_v1"] = 12000;
  hlt_initial_prescale_["HLT_Photon250_NoHE_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon26_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon16_AND_HE10_R9Id65_Eta2_Mass60_v1"] = 150;
  hlt_initial_prescale_["HLT_Photon28_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon14_AND_HE10_R9Id65_Mass50_Eta1p5_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon300_NoHE_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon30_R9Id90_HE10_Iso40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon30_v1"] = 18;
  hlt_initial_prescale_["HLT_Photon36_R9Id85_AND_CaloId24b40e_Iso50T80L_Photon18_AND_HE10_R9Id65_Mass30_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon36_R9Id85_AND_CaloId24b40e_Iso50T80L_Photon18_AND_HE10_R9Id65_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon18_AND_HE10_R9Id65_Mass70_v1"] = 50;
  hlt_initial_prescale_["HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon18_AND_HE10_R9Id65_Mass95_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_Eta2_Mass15_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon36_R9Id90_HE10_Iso40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon36_v1"] = 12;
  hlt_initial_prescale_["HLT_Photon42_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"] = 9;
  hlt_initial_prescale_["HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_VBF_v1"] = 3;
  hlt_initial_prescale_["HLT_Photon50_R9Id90_HE10_Iso40_v1"] = 62;
  hlt_initial_prescale_["HLT_Photon50_v1"] = 2850;
  hlt_initial_prescale_["HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"] = 3;
  hlt_initial_prescale_["HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon75_R9Id90_HE10_Iso40_v1"] = 11;
  hlt_initial_prescale_["HLT_Photon75_v1"] = 600;
  hlt_initial_prescale_["HLT_Photon90_CaloIdL_PFHT500_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon90_R9Id90_HE10_Iso40_v1"] = 6;
  hlt_initial_prescale_["HLT_Photon90_v1"] = 300;
  hlt_initial_prescale_["HLT_Physics_v1"] = 0;
  hlt_initial_prescale_["HLT_PixelTracks_Multiplicity110_v1"] = 0;
  hlt_initial_prescale_["HLT_PixelTracks_Multiplicity135_v1"] = 0;
  hlt_initial_prescale_["HLT_PixelTracks_Multiplicity160_v1"] = 0;
  hlt_initial_prescale_["HLT_PixelTracks_Multiplicity60_v1"] = 0;
  hlt_initial_prescale_["HLT_PixelTracks_Multiplicity85_v1"] = 0;
  hlt_initial_prescale_["HLT_QuadJet45_DoubleCSV0p5_v1"] = 25;
  hlt_initial_prescale_["HLT_QuadJet45_TripleCSV0p5_v1"] = 1;
  hlt_initial_prescale_["HLT_QuadMuon0_Dimuon0_Jpsi_v1"] = 1;
  hlt_initial_prescale_["HLT_QuadMuon0_Dimuon0_Upsilon_v1"] = 1;
  hlt_initial_prescale_["HLT_QuadPFJet_BTagCSV_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_QuadPFJet_VBF_v1"] = 500;
  hlt_initial_prescale_["HLT_ReducedIterativeTracking_v1"] = 0; // not in google doc
  hlt_initial_prescale_["HLT_Rsq0p36_v1"] = 1;
  hlt_initial_prescale_["HLT_RsqMR260_Rsq0p09_MR200_4jet_v1"] = 1;
  hlt_initial_prescale_["HLT_RsqMR260_Rsq0p09_MR200_v1"] = 1;
  hlt_initial_prescale_["HLT_RsqMR300_Rsq0p09_MR200_4jet_v1"] = 1;
  hlt_initial_prescale_["HLT_RsqMR300_Rsq0p09_MR200_v1"] = 1;
  hlt_initial_prescale_["HLT_TkMu24_eta2p1_v1"] = 350;
  hlt_initial_prescale_["HLT_TkMu27_v1"] = 250;
  hlt_initial_prescale_["HLT_TripleMu_12_10_5_v1"] = 1;
  hlt_initial_prescale_["HLT_ZeroBias_v1"] = 3000;
  hlt_initial_prescale_["HLTriggerFinalPath"] = 0;
  hlt_initial_prescale_["HLTriggerFirstPath"] = 0;


//////// SEPARATE, old

/*
  hlt_initial_prescale_["HLT_AK8PFHT850_TrimR0p1PT0p03Mass50_v1"] = 1;
  hlt_initial_prescale_["HLT_AK8PFJet360TrimMod_Mass30_v1"] = 1;
  hlt_initial_prescale_["HLT_BTagCSV07_v1"] = 1;
  hlt_initial_prescale_["HLT_CaloJet260_v1"] = 1; 
  hlt_initial_prescale_["HLT_CaloMHTNoPU90_PFMET90_PFMHT90_IDLoose_BTagCSV0p7_v1"] = 1;
  hlt_initial_prescale_["HLT_CaloMHTNoPU90_PFMET90_PFMHT90_IDLoose_v1"] = 50;
  hlt_initial_prescale_["HLT_Dimuon13_PsiPrime_v1"] = 1;
  hlt_initial_prescale_["HLT_Dimuon13_Upsilon_v1"] = 1;
  hlt_initial_prescale_["HLT_Dimuon20_Jpsi_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v1"] = 0;
  hlt_initial_prescale_["HLT_DoubleJet90_Double30_DoubleCSV0p5_v1"] = 50;
  hlt_initial_prescale_["HLT_DoubleJet90_Double30_TripleCSV0p5_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu33NoFiltersNoVtx_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu38NoFiltersNoVtx_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu4_3_Bs_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu4_3_Jpsi_Displaced_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu4_JpsiTrk_Displaced_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu4_LowMassNonResonantTrk_Displaced_v1"] = 1;
  hlt_initial_prescale_["HLT_DoubleMu4_PsiPrimeTrk_Displaced_v1"] = 1;
  hlt_initial_prescale_["HLT_DoublePhoton85_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele12_CaloId_TrackId_Iso_PFJet30_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele17_Ele12_Ele10_CaloId_TrackId_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele17_Ele8_Gsf_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele18_CaloId_TrackId_Iso_PFJet30_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele20WP60_Ele8_Mass55_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele22_eta2p1_WP85_Gsf_LooseIsoPFTau20_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele23_CaloId_TrackId_Iso_PFJet30_v1"] = 4;
  hlt_initial_prescale_["HLT_Ele23_Ele12_CaloId_TrackId_Iso_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele25WP60_SC4_Mass55_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1"] = 0;
  hlt_initial_prescale_["HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet40_v1"] = 0;
  hlt_initial_prescale_["HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1"] = 0;
  hlt_initial_prescale_["HLT_Ele27_eta2p1_WP85_Gsf_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet40_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele32_eta2p1_WP85_Gsf_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele33_CaloId_TrackId_Iso_PFJet30_v1"] = 3;
  hlt_initial_prescale_["HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v1"] = 1;
  hlt_initial_prescale_["HLT_Ele95_CaloIdVT_GsfTrkIdT_v1"] = 1;
  hlt_initial_prescale_["HLT_HT650_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu20_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1"] = 0;
  hlt_initial_prescale_["HLT_IsoMu20_eta2p1_IterTrk02_TriCentralPFJet40_v1"] = 0;
  hlt_initial_prescale_["HLT_IsoMu20_eta2p1_IterTrk02_TriCentralPFJet60_50_35_v1"] = 0;
  hlt_initial_prescale_["HLT_IsoMu20_eta2p1_IterTrk02_v1"] = 0;
  hlt_initial_prescale_["HLT_IsoMu24_IterTrk02_v1"] = 0;
  hlt_initial_prescale_["HLT_IsoMu24_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu24_eta2p1_IterTrk02_TriCentralPFJet40_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu24_eta2p1_IterTrk02_TriCentralPFJet60_50_35_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoMu24_eta2p1_IterTrk02_v1"] = 1;
  hlt_initial_prescale_["HLT_IsoTkMu20_eta2p1_IterTrk02_v1"] = 0;
  hlt_initial_prescale_["HLT_IsoTkMu24_IterTrk02_v1"] = 0;
  hlt_initial_prescale_["HLT_IsoTkMu24_eta2p1_IterTrk02_v1"] = 1;
  hlt_initial_prescale_["HLT_IterativeTracking_v1"] = 0;
  hlt_initial_prescale_["HLT_JetE30_NoBPTX3BX_NoHalo_v1"] = 1;
  hlt_initial_prescale_["HLT_JetE30_NoBPTX_v1"] = 1;
  hlt_initial_prescale_["HLT_JetE50_NoBPTX3BX_NoHalo_v1"] = 1;
  hlt_initial_prescale_["HLT_JetE70_NoBPTX3BX_NoHalo_v1"] = 1;
  hlt_initial_prescale_["HLT_L1_TripleJet_VBF_v1"] = 10000;
  hlt_initial_prescale_["HLT_L2DoubleMu23_NoVertex_v1"] = 1;
  hlt_initial_prescale_["HLT_L2DoubleMu28_NoVertex_2Cha_Angle2p5_Mass10_v1"] = 1;
  hlt_initial_prescale_["HLT_L2DoubleMu38_NoVertex_2Cha_Angle2p5_Mass10_v1"] = 1;
  hlt_initial_prescale_["HLT_L2Mu10_NoVertex_NoBPTX3BX_NoHalo_v1"] = 1;
  hlt_initial_prescale_["HLT_L2Mu10_NoVertex_NoBPTX_v1"] = 1;
  hlt_initial_prescale_["HLT_L2Mu20_NoVertex_3Sta_NoBPTX3BX_NoHalo_v1"] = 1;
  hlt_initial_prescale_["HLT_L2Mu30_NoVertex_3Sta_NoBPTX3BX_NoHalo_v1"] = 1;
  hlt_initial_prescale_["HLT_LooseIsoPFTau50_Trk30_eta2p1_MET120_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu17_Mu8_v1"] = 50;
  hlt_initial_prescale_["HLT_Mu17_NoFilters_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu17_TkMu8_v1"] = 50;
  hlt_initial_prescale_["HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu17_TrkIsoVVL_v1"] = 5;
  hlt_initial_prescale_["HLT_Mu23_TrkIsoVVL_Ele12_Gsf_CaloId_TrackId_Iso_MediumWP_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu24_TrkIsoVVL_v1"] = 4;
  hlt_initial_prescale_["HLT_Mu25_TkMu0_dEta18_Onia_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu30_TkMu11_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu34_TrkIsoVVL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu38NoFiltersNoVtx_Photon38_CaloIdL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu40_eta2p1_PFJet200_PFJet50_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu40_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu42NoFiltersNoVtx_Photon42_CaloIdL_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu8_TrkIsoVVL_Ele23_Gsf_CaloId_TrackId_Iso_MediumWP_v1"] = 1;
  hlt_initial_prescale_["HLT_Mu8_TrkIsoVVL_v1"] = 2;
  hlt_initial_prescale_["HLT_PFHT350_PFMET120_NoiseCleaned_v1"] = 1;
  hlt_initial_prescale_["HLT_PFHT350_v1"] = 2400;
  hlt_initial_prescale_["HLT_PFHT550_4Jet_v1"] = 1;
  hlt_initial_prescale_["HLT_PFHT600_v1"] = 250;
  hlt_initial_prescale_["HLT_PFHT650_4Jet_v1"] = 1;
  hlt_initial_prescale_["HLT_PFHT750_4Jet_v1"] = 1;
  hlt_initial_prescale_["HLT_PFHT900_v1"] = 1;
  hlt_initial_prescale_["HLT_PFJet140_v1"] = 2;
  hlt_initial_prescale_["HLT_PFJet200_v1"] = 1;
  hlt_initial_prescale_["HLT_PFJet260_v1"] = 170;
  hlt_initial_prescale_["HLT_PFJet320_v1"] = 60;
  hlt_initial_prescale_["HLT_PFJet400_v1"] = 20;
  hlt_initial_prescale_["HLT_PFJet40_v1"] = 0;
  hlt_initial_prescale_["HLT_PFJet450_v1"] = 1;
  hlt_initial_prescale_["HLT_PFJet500_v1"] = 1;
  hlt_initial_prescale_["HLT_PFJet60_v1"] = 0;
  hlt_initial_prescale_["HLT_PFJet80_v1"] = 2;
  hlt_initial_prescale_["HLT_PFMET110_PFMHT110_IDLoose_v1"] = 1;
  hlt_initial_prescale_["HLT_PFMET120_NoiseCleaned_BTagCSV07_v1"] = 1;
  hlt_initial_prescale_["HLT_PFMET120_NoiseCleaned_Mu5_v1"] = 1;
  hlt_initial_prescale_["HLT_PFMET170_NoiseCleaned_v1"] = 1;
  hlt_initial_prescale_["HLT_PFchMET90_NoiseCleaned_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon135_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon135_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon150_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon150_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon155_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon160_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon160_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon20_CaloIdVL_IsoL_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon250_NoHE_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon250_NoHE_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon300_NoHE_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon300_NoHE_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon18_AND_HE10_R9Id65_Mass95_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon42_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"] = 1;
  hlt_initial_prescale_["HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_Physics_v1"] = 0;
  hlt_initial_prescale_["HLT_QuadJet45_DoubleCSV0p5_v1"] = 50;
  hlt_initial_prescale_["HLT_QuadJet45_TripleCSV0p5_v1"] = 1;
  hlt_initial_prescale_["HLT_QuadPFJet_BTagCSV_VBF_v1"] = 1;
  hlt_initial_prescale_["HLT_QuadPFJet_VBF_v1"] = 1000;
  hlt_initial_prescale_["HLT_ReducedIterativeTracking_v1"] = 0;
  hlt_initial_prescale_["HLT_ZeroBias_v1"] = 3000;
  hlt_initial_prescale_["HLTriggerFinalPath"] = 0;
  hlt_initial_prescale_["HLTriggerFirstPath"] = 0;
*/

  for( std::map<std::string, int>::const_iterator iter = hlt_initial_prescale_.begin(); iter != hlt_initial_prescale_.end(); iter++){
    std::string name = iter->first;
    hlt_prescale_[name] = iter->second;
  }

}

// ------------ method called when ending the processing of a run  ------------
/*
void 
HLTriggerRate::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
HLTriggerRate::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
HLTriggerRate::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HLTriggerRate::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTriggerRate);
