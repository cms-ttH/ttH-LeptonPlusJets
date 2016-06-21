
#ifdef STANDALONECOMPILE
#include "./ttHYggdrasilEventSelection.h"
#else
#include "ttH-LeptonPlusJets/YggdrasilTreeMaker/interface/ttHYggdrasilEventSelection.h"
#endif

#include <iostream>

ttHYggdrasilEventSelection::ttHYggdrasilEventSelection(){

 Thre_TightMu_PT  = 25.0 ;
 Thre_LooseMu_PT  = 15.0;

 Thre_TightMu_Eta = 2.1 ;
 Thre_LooseMu_Eta = 2.4 ;

 Thre_TightMu_Iso = 0.15 ;
 Thre_LooseMu_Iso = 0.25 ;

 Thre_TightEl_PT  = 30;
 Thre_LooseEl_PT  = 15;

 Thre_TightEl_Eta = 2.1;
 Thre_LooseEl_Eta = 2.4;

 Thre_TightEl_Iso = 0.15;
 Thre_LooseEl_Iso = 0.15;

 Thre_Jet_PT      = 30.0;
 Thre_Jet_Eta     =  2.4;

 Thre_Jet_Btag    =  0.800;

}


ttHYggdrasilEventSelection::~ttHYggdrasilEventSelection(){

  _InitInternalVariables();

}

void ttHYggdrasilEventSelection::SetElTrigger( const int* trigFlag){

  ElTrig = trigFlag ;

}

void ttHYggdrasilEventSelection::SetMuTrigger( const int * trigFlag){

  MuTrig = trigFlag ;

}
void ttHYggdrasilEventSelection::SetLeptons( const std::vector<double> * pt, 
					     const std::vector<double> * eta, 
					     const std::vector<double> * phi,
					     const std::vector<double> * e,
					     const std::vector<int>    * charge, 
					     const std::vector<int>    * isMuon, 
					     const std::vector<double> * relIso,
					     const std::vector<int> * POGLoose,
					     const std::vector<int> * POGTight ){
  
  lep_pt  = pt ;  
  lep_eta = eta ;  
  lep_phi = phi ;
  lep_e   = e ;
  lep_isMuon = isMuon ; 
  lep_relIso = relIso ; 
  lep_POGLoose = POGLoose ; 
  lep_POGTight = POGTight ;

}

void ttHYggdrasilEventSelection::SetJets( const std::vector<double> * pt, 
					  const std::vector<double> * eta, 
					  const std::vector<double> * phi, 
					  const std::vector<double> * m,
					  const std::vector<double> * bDiscriminant ){

  jet_pt = pt ; 
  jet_eta = eta ; 
  jet_phi = phi ;  
  jet_m = m ;
  jet_bDiscriminant = bDiscriminant ;
  
}

void ttHYggdrasilEventSelection::doEventSelection(){

  _InitInternalVariables();

  // Object Definition 
  _ElectronSelection();
  _MuonSelection();
  _JetSelection();

  // Event selection
  _EventSelection();
  
}

void ttHYggdrasilEventSelection::_InitInternalVariables(){

  // Clear pointers
  // ( Remember TLorentzVector* of TightLeptons are included in the Loose,
  //   and does not need "delete" for them.
  //   Similarly, bjet are in jet.)

  for( std::vector<const TLorentzVector*>::iterator vec = selected_looseLeptons . begin();
       vec != selected_looseLeptons.end();
       vec ++
       ){
    if( (*vec) == 0 ) continue ;
    delete (*vec);
    (*vec) = 0 ;
  }

  for( std::vector<const TLorentzVector*>::iterator vec = selected_jets . begin();
       vec != selected_jets.end();
       vec ++
       ){
    if( (*vec) == 0 ) continue ;
    delete (*vec);
    (*vec) = 0 ;
  }

  selected_tightLeptons.clear();
  selected_tightLeptonsRelIso.clear();
  selected_tightLeptonsIsMuon.clear();
  selected_tightLeptonsCharge.clear();
  
  selected_looseLeptons.clear();
  selected_looseLeptonsRelIso.clear();
  selected_looseLeptonsIsMuon.clear();
  selected_looseLeptonsCharge.clear();
  
  selected_jets.clear();
  selected_jetsBdiscriminant.clear();
  
  selected_bjets.clear();
  selected_bjetsBdiscriminant.clear();

}

void ttHYggdrasilEventSelection::_ElectronSelection(){

  for( unsigned int idx = 0 ; idx < lep_pt->size() ; idx++ ){

    if( lep_isMuon -> at(idx) != 0 ) continue ;
    
    if(     lep_POGTight -> at(idx)  != 1  ) continue ;
    if(        lep_pt    -> at(idx)   < Thre_LooseEl_PT  ) continue ;
    if( fabs( lep_eta    -> at(idx) ) > Thre_LooseEl_Eta ) continue ;
    if(       lep_relIso -> at(idx)   > Thre_LooseEl_Iso ) continue ;

    TLorentzVector * vec = new TLorentzVector;
    vec->SetPtEtaPhiE( lep_pt  -> at(idx),
		       lep_eta -> at(idx),
		       lep_phi -> at(idx),
		       lep_e   -> at(idx) );

    selected_looseLeptons.push_back( vec );
    selected_looseLeptonsRelIso.push_back( lep_relIso -> at(idx) );
    selected_looseLeptonsIsMuon.push_back( 0 );
    selected_looseLeptonsCharge.push_back( 0 ); // tobe implemented

    if(     lep_POGTight -> at(idx)  != 1  ) continue ;
    if(        lep_pt    -> at(idx)   < Thre_TightEl_PT  ) continue ;
    if( fabs( lep_eta    -> at(idx) ) > Thre_TightEl_Eta ) continue ;
    if( fabs( lep_relIso -> at(idx) ) > Thre_TightEl_Iso ) continue ;

    selected_tightLeptons.push_back( vec );
    selected_tightLeptonsRelIso.push_back( lep_relIso -> at(idx) );
    selected_tightLeptonsIsMuon.push_back( 0 );
    selected_tightLeptonsCharge.push_back( 0 ); // tobe implemented

  }


}

void ttHYggdrasilEventSelection::_MuonSelection(){

  for( unsigned int idx = 0 ; idx < lep_pt->size() ; idx++ ){

    if( lep_isMuon -> at(idx) != 1 ) continue ;
    
    if(     lep_POGTight -> at(idx)  != 1  ) continue ;
    if(        lep_pt    -> at(idx)   < Thre_LooseMu_PT  ) continue ;
    if( fabs( lep_eta    -> at(idx) ) > Thre_LooseMu_Eta ) continue ;
    if(       lep_relIso -> at(idx)   > Thre_LooseMu_Iso ) continue ;

    TLorentzVector * vec = new TLorentzVector;
    vec->SetPtEtaPhiE( lep_pt  -> at(idx),
		       lep_eta -> at(idx),
		       lep_phi -> at(idx),
		       lep_e   -> at(idx) );

    selected_looseLeptons.push_back( vec );
    selected_looseLeptonsRelIso.push_back( lep_relIso -> at(idx) );
    selected_looseLeptonsIsMuon.push_back( 1 );
    selected_looseLeptonsCharge.push_back( 0 ); // tobe implemented

    if(     lep_POGTight -> at(idx)  != 1  ) continue ;
    if(        lep_pt    -> at(idx)   < Thre_TightMu_PT  ) continue ;
    if( fabs( lep_eta    -> at(idx) ) > Thre_TightMu_Eta ) continue ;
    if( fabs( lep_relIso -> at(idx) ) > Thre_TightMu_Iso ) continue ;

    selected_tightLeptons.push_back( vec );
    selected_tightLeptonsRelIso.push_back( lep_relIso -> at(idx) );
    selected_tightLeptonsIsMuon.push_back( 1 );
    selected_tightLeptonsCharge.push_back( 0 ); // tobe implemented

  }

}

void ttHYggdrasilEventSelection::_JetSelection(){


  for( unsigned int idx = 0 ; idx < jet_pt->size() ; idx++ ){

    if(        jet_pt    -> at(idx)   < Thre_Jet_PT  ) continue ;
    if( fabs( jet_eta    -> at(idx) ) > Thre_Jet_Eta ) continue ;

    if( _OverlapWithLooseLeptons( jet_eta -> at(idx), jet_phi -> at(idx) ) ) continue ; 

    TLorentzVector * vec = new TLorentzVector;
    vec->SetPtEtaPhiE( jet_pt  -> at(idx),
		       jet_eta -> at(idx),
		       jet_phi -> at(idx),
		       jet_m   -> at(idx) );

    selected_jets.push_back( vec );
    selected_jetsBdiscriminant.push_back( jet_bDiscriminant ->at(idx) );

    if( jet_bDiscriminant ->at(idx) < Thre_Jet_Btag ) continue ;

    selected_bjets.push_back( vec );
    selected_bjetsBdiscriminant.push_back( jet_bDiscriminant ->at(idx) );

  }

}

bool ttHYggdrasilEventSelection::_OverlapWithLooseLeptons( double eta, double phi){

  for( std::vector<const TLorentzVector*>::iterator lep = selected_looseLeptons.begin(); 
       lep != selected_looseLeptons.end();
       lep++){
    if( _calcDR2( eta, (*lep)->Eta(), phi, (*lep)->Phi() ) < 0.4 * 0.4 ){ return true ; }
  }
  return false ;
}

void ttHYggdrasilEventSelection::_EventSelection(){

}

double ttHYggdrasilEventSelection::_calcDR2( double eta1, double eta2, double phi1, double phi2 ){
  
  double d_eta = eta1 - eta2 ;
  double d_phi = fabs( phi1 - phi2 ) ; 
  d_phi = ( d_phi < M_PI ) ? d_phi : 2 * M_PI - d_phi ; 
  
  return  d_eta*d_eta + d_phi*d_phi ;
  
}

std::vector<const TLorentzVector*> ttHYggdrasilEventSelection::leptons()      {return selected_tightLeptons;}
std::vector<double>                ttHYggdrasilEventSelection::leptonsRelIso(){return selected_tightLeptonsRelIso; }
std::vector<int>                   ttHYggdrasilEventSelection::leptonsIsMuon(){return selected_tightLeptonsIsMuon; }
std::vector<int>                   ttHYggdrasilEventSelection::leptonsCharge(){return selected_tightLeptonsCharge; }

std::vector<const TLorentzVector*> ttHYggdrasilEventSelection::looseLeptons()      {return selected_looseLeptons;}
std::vector<double>                ttHYggdrasilEventSelection::looseLeptonsRelIso(){return selected_looseLeptonsRelIso;}
std::vector<int>                   ttHYggdrasilEventSelection::looseLeptonsIsMuon(){return selected_looseLeptonsIsMuon;}
std::vector<int>                   ttHYggdrasilEventSelection::looseLeptonsCharge(){return selected_looseLeptonsCharge;}

std::vector<const TLorentzVector*> ttHYggdrasilEventSelection::jets()             { return selected_jets;}
std::vector<double> ttHYggdrasilEventSelection::jetsBdiscriminant(){ return selected_jetsBdiscriminant;}

std::vector<const TLorentzVector*> ttHYggdrasilEventSelection::bjets()             { return selected_bjets;}
std::vector<double> ttHYggdrasilEventSelection::bjetsBdiscriminant(){ return selected_bjetsBdiscriminant;}
