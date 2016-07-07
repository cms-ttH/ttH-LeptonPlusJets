
#ifndef TTHYGGDRASILSCALEFACTORS_H_
#define TTHYGGDRASILSCALEFACTORS_H_

#include <vector>
#include <TLorentzVector.h>

#include <TH1D.h>
#include <TH2D.h>
#include <TH2F.h>

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

 private :

  std::string SFfileDir ;

  void init_all();
  void init_btagSF();
  void init_Pileup();
  void init_ElectronSF();
  void init_MuonSF();
  TH2 * getTH2HistogramFromFile( std::string input , std::string histoname );
  double GetBinValueFromXYValues( TH2 * h , double xVal , double yVal );

  // CSV reweighting
  TH1D* h_csv_wgt_hf[9][5];
  TH1D* c_csv_wgt_hf[9][5];
  TH1D* h_csv_wgt_lf[9][4][3];

  // PU weighting
  double PU_weight[50];


  // Lepton SF
  TH2F * h_EleSF_Reco;
  TH2F * h_EleSF_Iso;
  TH2D * h_MuSF_Reco;
  TH2D * h_MuSF_Iso ;

};

#endif
