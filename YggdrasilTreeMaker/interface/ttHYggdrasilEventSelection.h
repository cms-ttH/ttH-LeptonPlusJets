
#ifndef TTHYGGDRASILEVENTSELECTION
#define TTHYGGDRASILEVENTSELECTION

#include <vector>
#include <TLorentzVector.h>

class ttHYggdrasilEventSelection{

 public :

  ttHYggdrasilEventSelection();
  ~ttHYggdrasilEventSelection();

  void SetElTrigger( const long * trigFlag );
  void SetMuTrigger( const long * trigFlag );
  void SetLeptons( const std::vector<double> * pt, 
		   const std::vector<double> * eta, 
		   const std::vector<double> * phi,
		   const std::vector<double> * e,
		   const std::vector<int>    * charge,
		   const std::vector<int>    * isMuon, 
		   const std::vector<double> * relIso,
		   const std::vector<int> * POGLoose,
		   const std::vector<int> * POGTight );

  void SetJets( const std::vector<double> * pt, 
		const std::vector<double> * eta, 
		const std::vector<double> * phi, 
		const std::vector<double> * m,
		const std::vector<double> * bDiscriminant );

  void doEventSelection();

  // ttH Tight lepton
  std::vector<const TLorentzVector*> leptons();
  std::vector<double>                leptonsRelIso();
  std::vector<int>                   leptonsIsMuon();
  std::vector<int>                   leptonsCharge();

  // ttH Loose leoton
  std::vector<const TLorentzVector*> looseLeptons();
  std::vector<double>                looseLeptonsRelIso();
  std::vector<int>                   looseLeptonsIsMuon();
  std::vector<int>                   looseLeptonsCharge();

  // ttH Jet 
  std::vector<const TLorentzVector*> jets();
  std::vector<double> jetsBdiscriminant();

  // passed bTagging
  std::vector<const TLorentzVector*> bjets();
  std::vector<double> bjetsBdiscriminant();


 private :
   
  void _InitInternalVariables();

  void _ElectronSelection();
  void _MuonSelection();
  void _JetSelection();
  bool _OverlapWithLooseLeptons( double eta, double phi);
  void _EventSelection();
  double _calcDR2( double eta1, double eta2, double phi1, double phi2 );


  const long * ElTrig ; 
  const long * MuTrig ; 

  double Thre_TightMu_PT ;
  double Thre_TightMu_Eta ;
  double Thre_TightMu_Iso ;

  double Thre_LooseMu_PT ;
  double Thre_LooseMu_Eta ;
  double Thre_LooseMu_Iso ;

  double Thre_TightEl_PT ;
  double Thre_TightEl_Eta ;
  double Thre_TightEl_Iso ;

  double Thre_LooseEl_PT ;
  double Thre_LooseEl_Eta ;
  double Thre_LooseEl_Iso ;

  double Thre_Jet_PT ;
  double Thre_Jet_Eta ;
  double Thre_Jet_Btag ;


  const std::vector<double> * lep_pt;
  const std::vector<double> * lep_eta; 
  const std::vector<double> * lep_phi;
  const std::vector<double> * lep_e;
  const std::vector<int>    * lep_isMuon; 
  const std::vector<double> * lep_relIso;
  const std::vector<int>    * lep_POGLoose;
  const std::vector<int>    * lep_POGTight;

  const std::vector<double> * jet_pt; 
  const std::vector<double> * jet_eta; 
  const std::vector<double> * jet_phi; 
  const std::vector<double> * jet_m;
  const std::vector<double> * jet_bDiscriminant ;

  std::vector<const TLorentzVector*> selected_tightLeptons;
  std::vector<double>                selected_tightLeptonsRelIso;
  std::vector<int>                   selected_tightLeptonsIsMuon;
  std::vector<int>                   selected_tightLeptonsCharge;

  std::vector<const TLorentzVector*> selected_looseLeptons;
  std::vector<double>                selected_looseLeptonsRelIso;
  std::vector<int>                   selected_looseLeptonsIsMuon;
  std::vector<int>                   selected_looseLeptonsCharge;

  std::vector<const TLorentzVector*> selected_jets;
  std::vector<double>                selected_jetsBdiscriminant;

  std::vector<const TLorentzVector*> selected_bjets;
  std::vector<double>                selected_bjetsBdiscriminant;

};



#endif
