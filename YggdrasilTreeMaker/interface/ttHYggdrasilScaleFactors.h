
#ifndef TTHYGGDRASILSCALEFACTORS_H_
#define TTHYGGDRASILSCALEFACTORS_H_

#include <vector>
#include <TLorentzVector.h>

#include <TH1D.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TGraphAsymmErrors.h>

#ifdef STANDALONECOMPILE
#include "ttHYggdrasilEventSelection.h"
#else
#include "ttH-LeptonPlusJets/YggdrasilTreeMaker/interface/ttHYggdrasilEventSelection.h"
#endif


class ttHYggdrasilScaleFactors{

 public :

  ttHYggdrasilScaleFactors();
  ttHYggdrasilScaleFactors( char * sf_file_directory );
  ~ttHYggdrasilScaleFactors();

  
  double get_csv_wgt( ttHYggdrasilEventSelection * event,
                      int iSys, double &csvWgtHF, double &csvWgtLF, double &csvWgtCF );

  double get_csv_wgt( std::vector<double> jetPts, std::vector<double> jetEtas, std::vector<double> jetCSVs, std::vector<int> jetFlavors, 
                      int iSys, double &csvWgtHF, double &csvWgtLF, double &csvWgtCF );


  double get_pu_wgt( int mc_pu );

  double getTightMuonSF( ttHYggdrasilEventSelection * event );
  double getTightElectronSF( ttHYggdrasilEventSelection * event );

  double getTightMuon_IDSF( ttHYggdrasilEventSelection * event );
  double getTightMuon_IsoSF( ttHYggdrasilEventSelection * event );
  double getLooseMuon_IsoSF( ttHYggdrasilEventSelection * event );
  double getTightMuon_TrackingSF( ttHYggdrasilEventSelection * event );

  double getTightElectron_IDSF( ttHYggdrasilEventSelection * event );
  double getTightElectron_RecoSF( ttHYggdrasilEventSelection * event );

  std::vector<double> getLooseLepton_IDSF_vector( ttHYggdrasilEventSelection * event );
  std::vector<double> getLooseLepton_IsoSF_vector( ttHYggdrasilEventSelection * event );
  std::vector<double> getLooseLepton_TrackingSF_vector( ttHYggdrasilEventSelection * event );
  std::vector<double> getLooseLepton_RecoSF_vector( ttHYggdrasilEventSelection * event );




  // - - -

  double get_TrigMuSF( ttHYggdrasilEventSelection * event );
  double get_TrigElSF( ttHYggdrasilEventSelection * event );

  double get_TrigElEfficiency( ttHYggdrasilEventSelection * event );
  double get_TrigMuEfficiency( ttHYggdrasilEventSelection * event );


  // Can replace PU file if you want.
  void SetupDataPileupFile( std::string filename ); // just filename. the file should be in the common data directory.

  void init_Pileup();

 private :

  std::string SFfileDir ;
  std::string PileupHistogram;

  void init_all();
  void init_TrigElSF();
  void init_TrigMuSF();
  void init_ElectronSF();
  void init_MuonSF();
  void init_btagSF();
  TH2 * getTH2HistogramFromFile( std::string input , std::string histoname );
  TGraphAsymmErrors * getTGraphAsymmErrorsFromFile( std::string input , std::string tgraphname );
  double GetBinValueFromXYValues( TH2 * h , double xVal , double yVal );
  double GetTGraphAsymmErrorsValue( TGraphAsymmErrors * tg , double xVal , std::string err );
  int GetTGraphPoint( TGraphAsymmErrors * tg , double xVal );
  

  // CSV reweighting
  TH1D* h_csv_wgt_hf[9][5];
  TH1D* c_csv_wgt_hf[9][5];
  TH1D* h_csv_wgt_lf[9][4][3];

  // PU weighting
  double PU_weight[75];


  // Lepton SF
  std::vector< TH2F *> h_EleSF_ID;
  std::vector< double> EleSF_ID_Lumi;
  double EleSF_ID_LumiTotal;

  TH2F * h_EleSF_Reco;

  std::vector< TH2D *> h_MuSF_ID;
  std::vector< double> MuSF_ID_Lumi;
  double MuSF_ID_LumiTotal;
  
  std::vector< TH2D *> h_MuSF_tightIso ;
  std::vector< TH2D *> h_MuSF_looseIso ;
  std::vector< double > MuSF_Iso_Lumi ;
  double MuSF_Iso_LumiTotal ;

  std::vector< TGraphAsymmErrors *> tgraph_MuSF_Tracking ;
  std::vector< double > MuSF_Tracking_Lumi ;
  double MuSF_Tracking_LumiTotal ;

  // Trig SF
  TH2D * h_MuSF_Trig_BCDEF;
  TH2D * h_MuSF_Trig_GH;
  TH2D * h_EleSF_Trig;

  // Trig Efficiency
  TH2D * h_MUEff_SingleMuonTrig;

};

#endif
