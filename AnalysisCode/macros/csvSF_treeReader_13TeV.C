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


#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"


#if !defined(__CINT__) && !defined(__MAKECINT__)

//#include "ttH-LeptonPlusJets/AnalysisCode/interface/BEANloader.h"
#include "ttH-LeptonPlusJets/AnalysisCode/interface/YggdrasilEventVars.h"

#endif


//*****************************************************************************
typedef std::vector< TLorentzVector >          vecTLorentzVector;
typedef std::vector<int>                       vint;
typedef std::vector<double>                    vdouble;
typedef std::vector<std::vector<double> >      vvdouble;



// ------------ csv applying functions -------------
void fillCSVhistos(TFile *fileHF, TFile *fileLF);
double get_csv_wgt( vvdouble jets, vdouble jetCSV, vint jetFlavor, int iSys, double &csvWgtHF, double &csvWgtLF, double &csvWgtCF );

// CSV reweighting
TH1D* h_csv_wgt_hf[9][6];
TH1D* c_csv_wgt_hf[9][6];
TH1D* h_csv_wgt_lf[9][4][3];

//*****************************************************************************

void csvSF_treeReader_13TeV( bool isHF=1, int verNum = 0, int insample=1, int maxNentries=-1, int Njobs=1, int jobN=1, double intLumi=-1 ) {

  std::string inputFileHF = "data/csv_rwt_hf_IT.root";
  std::string inputFileLF = "data/csv_rwt_lf_IT.root";

  if( verNum>0 ){
    inputFileHF = Form("CSVHistoFiles/csv_rwt_fit_hf_v%i.root", verNum-1);
    inputFileLF = Form("CSVHistoFiles/csv_rwt_fit_lf_v%i.root", verNum-1);

    std::cout << "\t inputFileHF = " << inputFileHF << std::endl;
    std::cout << "\t inputFileLF = " << inputFileLF << std::endl;
  }

  TFile* f_CSVwgt_HF = new TFile ((string(getenv("CMSSW_BASE")) + "/src/ttH-LeptonPlusJets/AnalysisCode/" + inputFileHF).c_str());
  TFile* f_CSVwgt_LF = new TFile ((string(getenv("CMSSW_BASE")) + "/src/ttH-LeptonPlusJets/AnalysisCode/" + inputFileLF).c_str());

  fillCSVhistos(f_CSVwgt_HF, f_CSVwgt_LF);

  std::cout << " ###===> iteration version " << verNum << std::endl;

  ////
  std::cout << "   ===> load the root files! " << std::endl;

  std::string sampleType = ( insample>=0 ) ? "mc" : "data";
  std::string str_jobN;
  std::stringstream stream;
  stream << jobN;
  str_jobN = stream.str();

  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat13TeV
  double mySample_xSec_ = 1.;
  double mySample_nGen_ = 1.;
  std::string mySample_sampleName_ = "delete";
  std::string mySample_inputDir_ = "";
  if( insample==2500 ){
    mySample_xSec_ = 831.76;//https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO
    mySample_nGen_ = 25446993;
    mySample_sampleName_ = "TTJets";
    mySample_inputDir_ = "/eos/uscms/store/user/puigh/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1_yggdrasilTree_v1/150217_005136/0000/";
    //mySample_inputDir_ = "/uscms_data/d2/dpuigh/TTH/miniAOD/CMSSW_7_2_3/src/ttH-LeptonPlusJets/YggdrasilTreeMaker/";
  }
  else if( insample==2300 ){
    mySample_xSec_ = 2008.4; 
    mySample_nGen_ = 2829164;
    mySample_sampleName_ = "DYJetsToLL";
    mySample_inputDir_ = "/eos/uscms/store/user/puigh/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1_yggdrasilTree_v1/150216_233924/0000/";
  }
  else if( insample==2400 ){
    mySample_xSec_ = 20508.9;  
    mySample_nGen_ = 10017462;
    mySample_sampleName_ = "WJetsToLNu";
    mySample_inputDir_ = "/eos/uscms/store/user/puigh/WJetsToLNu_13TeV-madgraph-pythia8-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1_yggdrasilTree_v1/150217_010312/0000/";
  }
  else if( insample==2524 ){
    mySample_xSec_ = 1.152;  
    mySample_nGen_ = 246521;
    mySample_sampleName_ = "TTWJets";
    mySample_inputDir_ = "/eos/uscms/store/user/puigh/TTWJets_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1_yggdrasilTree_v1/150217_005352/0000/";
  }
  else if( insample==2523 ){
    mySample_xSec_ = 2.232;  
    mySample_nGen_ = 249275;
    mySample_sampleName_ = "TTZJets";
    mySample_inputDir_ = "/eos/uscms/store/user/puigh/TTZJets_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1_yggdrasilTree_v1/150217_005607/0000/";
  }
  else if( insample==2510 ){
    mySample_xSec_ = 2.232;  
    mySample_nGen_ = 500000;
    mySample_sampleName_ = "TToLeptons_s";
    mySample_inputDir_ = "/eos/uscms/store/user/puigh/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1_yggdrasilTree_v1/150217_005853/0000/";
  }
  else if( insample==2511 ){
    mySample_xSec_ = 2.232;  
    mySample_nGen_ = 250000;
    mySample_sampleName_ = "TBarToLeptons_s";
    mySample_inputDir_ = "/eos/uscms/store/user/puigh/TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1_yggdrasilTree_v1/150217_004555/0000/";
  }
  else if( insample==2512 ){
    mySample_xSec_ = 2.232;  
    mySample_nGen_ = 3991000;
    mySample_sampleName_ = "TToLeptons_t";
    mySample_inputDir_ = "/eos/uscms/store/user/puigh/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1_yggdrasilTree_v1/150217_005929/0000/";
  }
  else if( insample==2513 ){
    mySample_xSec_ = 2.232;  
    mySample_nGen_ = 1999800;
    mySample_sampleName_ = "TBarToLeptons_t";
    mySample_inputDir_ = "/eos/uscms/store/user/puigh/TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1_yggdrasilTree_v1/150217_004732/0000/";
  }
  else if( insample==2514 ){
    mySample_xSec_ = 35.6;  
    mySample_nGen_ = 986100;
    mySample_sampleName_ = "T_tW_DR";
    mySample_inputDir_ = "/eos/uscms/store/user/puigh/T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1_yggdrasilTree_v1/150217_010006/0000/";
  }
  else if( insample==2515 ){
    mySample_xSec_ = 35.6;  
    mySample_nGen_ = 971800;
    mySample_sampleName_ = "Tbar_tW_DR";
    mySample_inputDir_ = "/eos/uscms/store/user/puigh/Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1_yggdrasilTree_v1/150217_010035/0000/";
  }
  else if( insample==9125 ){
    mySample_xSec_ = 0.5085 * 1.0;// YR3 * BR(all)  
    mySample_nGen_ = 199700;//199000;
    mySample_sampleName_ = "TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola_PU20bx25_tsg_PHYS14_25_V1-v2";
    mySample_inputDir_ = "/eos/uscms/store/user/puigh/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2_v1_yggdrasilTree_v1/150217_004834/0000/";
  }


  std::string treefilename = mySample_inputDir_ + "yggdrasil_treeMaker*.root";


  std::string s_end = "_histo_" + str_jobN + ".root";
  if( Njobs==1 ) s_end = "_histo.root";


  std::string histofilename = Form("CSVHistoFiles/csv_rwt_hf_%s_v%i%s",mySample_sampleName_.c_str(),verNum, s_end.c_str());
  if( !isHF ) histofilename = Form("CSVHistoFiles/csv_rwt_lf_%s_v%i%s",mySample_sampleName_.c_str(),verNum, s_end.c_str());

  std::cout << "  treefilename  = " << treefilename.c_str() << std::endl;
  std::cout << "  histofilename = " << histofilename.c_str() << std::endl;

  TChain *chain = new TChain("ttHTreeMaker/worldTree");
  chain->Add(treefilename.c_str());


  //////////////////////////////////////////////////////////////////////////
  ///  Tree branches/leaves
  //////////////////////////////////////////////////////////////////////////

  yggdrasilEventVars *eve=0;
  chain->SetBranchAddress("eve.", &eve );

  //////////////////////////////////////////////////////////////////////////
  ///  Histogram making
  //////////////////////////////////////////////////////////////////////////


  TFile histofile(histofilename.c_str(),"recreate");
  histofile.cd();


  bool verbose = false;

  //////////////////////////////////////////////////////////////////////////
  ///  Histograms
  //////////////////////////////////////////////////////////////////////////

  TH1::SetDefaultSumw2();

  TH1D* h_first_jet_pt  = new TH1D("h_first_jet_pt",";first jet p_{T}", 100, 0., 500. );
  TH1D* h_first_jet_eta = new TH1D("h_first_jet_eta",";first jet #eta", 70, -3.5, 3.5 );
  TH1D* h_first_jet_csv = new TH1D("h_first_jet_csv",";first jet CSV", 102, -0.01, 1.01 );
  TH1D* h_first_jet_flavour = new TH1D("h_first_jet_flavour",";first jet flavour", 28, -6, 22 );
  TH1D* h_first_jet_vtxMass = new TH1D("h_first_jet_vtxMass",";first jet vertex mass", 80, 0., 8.0 );
  TH1D* h_first_jet_vtxNtracks = new TH1D("h_first_jet_vtxNtracks",";first jet vertex number of tracks", 14, -0.5, 13.5 );
  TH1D* h_first_jet_vtx3DVal = new TH1D("h_first_jet_vtx3DVal",";first jet vertex 3D value", 141, -0.01, 14.0 );
  TH1D* h_first_jet_vtx3DSig = new TH1D("h_first_jet_vtx3DSig",";first jet vertex 3D significance", 401, -0.01, 400 );
  TH1D* h_first_jet_vtxMass_b = new TH1D("h_first_jet_vtxMass_b",";first jet vertex mass", 80, 0., 8.0 );
  TH1D* h_first_jet_vtxMass_c = new TH1D("h_first_jet_vtxMass_c",";first jet vertex mass", 80, 0., 8.0 );
  TH1D* h_first_jet_vtxMass_l = new TH1D("h_first_jet_vtxMass_l",";first jet vertex mass", 80, 0., 8.0 );
  TH1D* h_first_jet_vtxMass_o = new TH1D("h_first_jet_vtxMass_o",";first jet vertex mass", 80, 0., 8.0 );

  TH1D* h_second_jet_pt  = new TH1D("h_second_jet_pt",";second jet p_{T}", 100, 0., 500. );
  TH1D* h_second_jet_eta = new TH1D("h_second_jet_eta",";second jet #eta", 70, -3.5, 3.5 );
  TH1D* h_second_jet_csv = new TH1D("h_second_jet_csv",";second jet CSV", 102, -0.01, 1.01 );
  TH1D* h_second_jet_flavour = new TH1D("h_second_jet_flavour",";second jet flavour", 28, -6, 22 );
  TH1D* h_second_jet_vtxMass = new TH1D("h_second_jet_vtxMass",";second jet vertex mass", 80, 0., 8.0 );
  TH1D* h_second_jet_vtxNtracks = new TH1D("h_second_jet_vtxNtracks",";second jet vertex number of tracks", 14, -0.5, 13.5 );
  TH1D* h_second_jet_vtx3DVal = new TH1D("h_second_jet_vtx3DVal",";second jet vertex 3D value", 141, -0.01, 14.0 );
  TH1D* h_second_jet_vtx3DSig = new TH1D("h_second_jet_vtx3DSig",";second jet vertex 3D significance", 401, -0.01, 400 );
  TH1D* h_second_jet_vtxMass_b = new TH1D("h_second_jet_vtxMass_b",";second jet vertex mass", 80, 0., 8.0 );
  TH1D* h_second_jet_vtxMass_c = new TH1D("h_second_jet_vtxMass_c",";second jet vertex mass", 80, 0., 8.0 );
  TH1D* h_second_jet_vtxMass_l = new TH1D("h_second_jet_vtxMass_l",";second jet vertex mass", 80, 0., 8.0 );
  TH1D* h_second_jet_vtxMass_o = new TH1D("h_second_jet_vtxMass_o",";second jet vertex mass", 80, 0., 8.0 );



  TH2D* h_first_jet_csv_vtxMass = new TH2D("h_first_jet_csv_vtxMass",";first jet CSV;first jet vertex mass", 102, -0.01, 1.01, 80, 0., 8.0 );
  TH2D* h_first_jet_csv_vtxMass_b = new TH2D("h_first_jet_csv_vtxMass_b",";first jet CSV;first jet vertex mass", 102, -0.01, 1.01, 80, 0., 8.0 );
  TH2D* h_first_jet_csv_vtxMass_c = new TH2D("h_first_jet_csv_vtxMass_c",";first jet CSV;first jet vertex mass", 102, -0.01, 1.01, 80, 0., 8.0 );
  TH2D* h_first_jet_csv_vtxMass_l = new TH2D("h_first_jet_csv_vtxMass_l",";first jet CSV;first jet vertex mass", 102, -0.01, 1.01, 80, 0., 8.0 );
  TH2D* h_first_jet_csv_vtxMass_o = new TH2D("h_first_jet_csv_vtxMass_o",";first jet CSV;first jet vertex mass", 102, -0.01, 1.01, 80, 0., 8.0 );

  TH2D* h_second_jet_csv_vtxMass = new TH2D("h_second_jet_csv_vtxMass",";second jet CSV;second jet vertex mass", 102, -0.01, 1.01, 80, 0., 8.0 );
  TH2D* h_second_jet_csv_vtxMass_b = new TH2D("h_second_jet_csv_vtxMass_b",";second jet CSV;second jet vertex mass", 102, -0.01, 1.01, 80, 0., 8.0 );
  TH2D* h_second_jet_csv_vtxMass_c = new TH2D("h_second_jet_csv_vtxMass_c",";second jet CSV;second jet vertex mass", 102, -0.01, 1.01, 80, 0., 8.0 );
  TH2D* h_second_jet_csv_vtxMass_l = new TH2D("h_second_jet_csv_vtxMass_l",";second jet CSV;second jet vertex mass", 102, -0.01, 1.01, 80, 0., 8.0 );
  TH2D* h_second_jet_csv_vtxMass_o = new TH2D("h_second_jet_csv_vtxMass_o",";second jet CSV;second jet vertex mass", 102, -0.01, 1.01, 80, 0., 8.0 );

  TH1D* h_minDR_lepton_first_jet  = new TH1D("h_minDR_lepton_first_jet",";min #DeltaR(lepton, first jet)", 102, -0.01, 6.01 );
  TH1D* h_minDR_lepton_second_jet = new TH1D("h_minDR_lepton_second_jet",";min #DeltaR(lepton, second jet)", 102, -0.01, 6.01 );

  TH1D* h_second_jet_eta_TwoMuon = new TH1D("h_second_jet_eta_TwoMuon",";second jet #eta", 70, -3.5, 3.5 );
  TH1D* h_second_jet_eta_TwoElectron = new TH1D("h_second_jet_eta_TwoElectron",";second jet #eta", 70, -3.5, 3.5 );
  TH1D* h_second_jet_eta_MuonElectron = new TH1D("h_second_jet_eta_MuonElectron",";second jet #eta", 70, -3.5, 3.5 );

  TH2D* h_second_jet_pt_eta = new TH2D("h_second_jet_pt_eta",";second jet #eta;second jet p_{T}", 70, -3.5, 3.5, 100, 0., 500. );
  TH2D* h_second_jet_flavour_eta = new TH2D("h_second_jet_flavour_eta",";second jet #eta;second flavour", 70, -3.5, 3.5, 28, -6, 22 );

  TH1D* h_mass_lepton_first_jet  = new TH1D("h_mass_lepton_first_jet",";first jet + lepton mass", 100, 0., 200. );
  TH1D* h_mass_lepton_second_jet  = new TH1D("h_mass_lepton_second_jet",";second jet + lepton mass", 100, 0., 200. );
  TH1D* h_mass_tight_lepton_first_jet  = new TH1D("h_mass_tight_lepton_first_jet",";first jet + tight lepton mass", 100, 0., 200. );
  TH1D* h_mass_tight_lepton_second_jet  = new TH1D("h_mass_tight_lepton_second_jet",";second jet + tight lepton mass", 100, 0., 200. );

  TH2D* h_mass_lepton_second_jet_eta  = new TH2D("h_mass_lepton_second_jet_eta",";second jet #eta;second jet + lepton mass", 70, -3.5, 3.5, 100, 0., 200. );


  int NumCutsHF = 10;
  TH1D* h_hf_event_selection  = new TH1D("h_hf_event_selection",";cut", NumCutsHF, 0, NumCutsHF );

  h_hf_event_selection->GetXaxis()->SetBinLabel(1,"All");
  h_hf_event_selection->GetXaxis()->SetBinLabel(2,"==2 jets");
  h_hf_event_selection->GetXaxis()->SetBinLabel(3,"Dilepton trigger");
  h_hf_event_selection->GetXaxis()->SetBinLabel(4,"==2 leptons");
  h_hf_event_selection->GetXaxis()->SetBinLabel(5,"Opposite charge");
  h_hf_event_selection->GetXaxis()->SetBinLabel(6,"#Delta R(lep,lep) > 0.2");
  h_hf_event_selection->GetXaxis()->SetBinLabel(7,"M(lep,lep) > 12");
  h_hf_event_selection->GetXaxis()->SetBinLabel(8,"ZVeto");
  h_hf_event_selection->GetXaxis()->SetBinLabel(9,"MET > 50");
  h_hf_event_selection->GetXaxis()->SetBinLabel(10,"jet passes medium b-tag");

  int NumCutsLF = 10;
  TH1D* h_lf_event_selection  = new TH1D("h_lf_event_selection",";cut", NumCutsLF, 0, NumCutsLF );

  h_lf_event_selection->GetXaxis()->SetBinLabel(1,"All");
  h_lf_event_selection->GetXaxis()->SetBinLabel(2,"==2 jets");
  h_lf_event_selection->GetXaxis()->SetBinLabel(3,"Dilepton trigger");
  h_lf_event_selection->GetXaxis()->SetBinLabel(4,"==2 leptons");
  h_lf_event_selection->GetXaxis()->SetBinLabel(5,"Opposite charge");
  h_lf_event_selection->GetXaxis()->SetBinLabel(6,"#Delta R(lep,lep) > 0.2");
  h_lf_event_selection->GetXaxis()->SetBinLabel(7,"M(lep,lep) > 12");
  h_lf_event_selection->GetXaxis()->SetBinLabel(8,"Zmass window");
  h_lf_event_selection->GetXaxis()->SetBinLabel(9,"MET < 30");
  h_lf_event_selection->GetXaxis()->SetBinLabel(10,"jet fails loose b-tag");

  TH1D* h_numLooseLeptons = new TH1D("h_numLooseLeptons",";number of loose leptons", 5, 0, 5 );

  // single jet specific plots
  int nPt = 6;
  int nEta = 1;
  if ( !isHF ){
    nPt = 4; nEta = 3;
  }


  //////
  TH1D* h_Data_jet_csv[6][3];
  TH1D* h_MC_b_jet_csv[6][3];
  TH1D* h_MC_nonb_jet_csv[6][3];

  TH1D* h_MC_b_jet_vtxMass[6][3];
  TH1D* h_MC_nonb_jet_vtxMass[6][3];

  TH2D* h_MC_b_jet_csv_vtxMass[6][3];
  TH2D* h_MC_nonb_jet_csv_vtxMass[6][3];


  /////
  int nBins = 18; //Number of bins 
  double xBins_hf[19] = {-10.0, 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01};

  if(!isHF) nBins = 21;
  double xBins_lf[22] = {-10.0, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.915, 0.932, 0.949, 0.966, 0.983, 1.01};
 


  for ( int iPt=0; iPt<nPt; iPt++){
    for ( int iEta=0; iEta<nEta; iEta++){

      TString h_Data_Name = Form("csv_Data_Pt%i_Eta%i",iPt,iEta);
      TString h_b_Name = Form("csv_MC_bjets_Pt%i_Eta%i",iPt,iEta);
      TString h_nonb_Name = Form("csv_MC_nonbjets_Pt%i_Eta%i",iPt,iEta);

      TString h_vtxMass_b_Name = Form("vtxMass_MC_bjets_Pt%i_Eta%i",iPt,iEta);
      TString h_vtxMass_nonb_Name = Form("vtxMass_MC_nonbjets_Pt%i_Eta%i",iPt,iEta);

      if ( isHF ){
	h_Data_jet_csv[iPt][iEta] = new TH1D(h_Data_Name, h_Data_Name, nBins, xBins_hf); 
	h_MC_b_jet_csv[iPt][iEta] = new TH1D(h_b_Name, h_b_Name, nBins, xBins_hf); 
	h_MC_nonb_jet_csv[iPt][iEta] = new TH1D(h_nonb_Name, h_nonb_Name, nBins, xBins_hf); 

	h_MC_b_jet_vtxMass[iPt][iEta] = new TH1D(h_vtxMass_b_Name, h_vtxMass_b_Name, 16, 0., 8.0); 
	h_MC_nonb_jet_vtxMass[iPt][iEta] = new TH1D(h_vtxMass_nonb_Name, h_vtxMass_nonb_Name, 16, 0., 8.0); 

	h_MC_b_jet_csv_vtxMass[iPt][iEta] = new TH2D("csv_"+h_vtxMass_b_Name, "csv_"+h_vtxMass_b_Name, nBins, xBins_hf, 16, 0., 8.0); 
	h_MC_nonb_jet_csv_vtxMass[iPt][iEta] = new TH2D("csv_"+h_vtxMass_nonb_Name, "csv_"+h_vtxMass_nonb_Name, nBins, xBins_hf, 16, 0., 8.0); 
      }
      else { 
	h_Data_jet_csv[iPt][iEta] = new TH1D(h_Data_Name, h_Data_Name, nBins, xBins_lf); 
	h_MC_b_jet_csv[iPt][iEta] = new TH1D(h_b_Name, h_b_Name, nBins, xBins_lf); 
	h_MC_nonb_jet_csv[iPt][iEta] = new TH1D(h_nonb_Name, h_nonb_Name, nBins, xBins_lf); 

	h_MC_b_jet_vtxMass[iPt][iEta] = new TH1D(h_vtxMass_b_Name, h_vtxMass_b_Name, 16, 0., 8.0); 
	h_MC_nonb_jet_vtxMass[iPt][iEta] = new TH1D(h_vtxMass_nonb_Name, h_vtxMass_nonb_Name, 16, 0., 8.0); 

	h_MC_b_jet_csv_vtxMass[iPt][iEta] = new TH2D("csv_"+h_vtxMass_b_Name, "csv_"+h_vtxMass_b_Name, nBins, xBins_lf, 16, 0., 8.0); 
	h_MC_nonb_jet_csv_vtxMass[iPt][iEta] = new TH2D("csv_"+h_vtxMass_nonb_Name, "csv_"+h_vtxMass_nonb_Name, nBins, xBins_lf, 16, 0., 8.0); 
      }   
    }
  }

  //////////////////////////////////////////////////////////////////////////
  /////
  //////////////////////////////////////////////////////////////////////////
 
  int numEvents_all=0;
  int numEvents_2jets=0;

  int numEvents_lepselection2=0;
  int numEvents_lepselection1a=0;
  int numEvents_lepselection1b=0;
  int numEvents_lepselection1c=0;

  int numEvents_exselection=0;

  int nentries = chain->GetEntries();
  std::cout << "\n\t Number of entries = " << nentries << std::endl;
  std::cout << "\t Max number of entries = " << maxNentries << std::endl;
  std::cout << "\n" << std::endl;

  int use_nentries = std::max( maxNentries, nentries);

  int NeventsPerJob = int( double(use_nentries)/double(Njobs) + 0.000001 ) + 1;

  int firstEvent = (jobN-1)*NeventsPerJob + 1;
  int lastEvent  = firstEvent + NeventsPerJob;
  if( jobN==Njobs ) lastEvent = -1;
  if( jobN==1 ) firstEvent = 0;

  int cnt = 0;
  int nPass = 0;
  std::cout << "========  Starting Event Loop  ========" << std::endl;
  for (Long64_t ievt=0; ievt<chain->GetEntries();ievt++) {    //Long64_t
    cnt++;
    if( ievt<firstEvent ) continue;
    if( ievt==lastEvent ) break;

    if( ievt==1 )        std::cout << "     Event " << ievt << std::endl;
    if( ievt%10000==0 && ievt!=1 ) std::cout << "           " << ievt << "\t" 
					     << int(double(ievt-firstEvent)/double(NeventsPerJob)*100) << "% done" << std::endl;

    //if( ievt==(maxNentries+1) ) break;
    if( ievt==(maxNentries+1) && ievt!=0 ) break;

    chain->GetEntry(ievt);
    numEvents_all++;

    //// --------- various weights: PU, topPt, triggerSF, leptonSF...
    // double  wgt_topPtSF = eve->wgt_topPt_;
    double Xsec = mySample_xSec_;//eve->wgt_xs_;
    double nGen = ( maxNentries>0 ) ? maxNentries : mySample_nGen_;//eve->wgt_nGen_;
    double lumi = ( intLumi > 0 ) ? intLumi : eve->wgt_lumi_ ;

    double wgt_gen = ( insample==9125 || insample==9135 ) ? eve->wgt_generator_ : 1;
    double wgt = wgt_gen * lumi * (Xsec/nGen);//"weight_PU*topPtWgt*osTriggerSF*lepIDAndIsoSF*"; // various weights

    ///////////////////
    ////// selections
    ///////////////////


    h_hf_event_selection->Fill(0.5, wgt);
    h_lf_event_selection->Fill(0.5, wgt);

    int iSys = 0;
    //////------- exactly 2 jets -----
    vvdouble jet_vect_TLV = eve->jet_loose_vect_TLV_[iSys];
    vdouble jet_CSVv2IVf = eve->jet_loose_combinedInclusiveSecondaryVertexV2BJetTags_[iSys];

    int numJets = int(jet_vect_TLV.size()) ;
    if (numJets !=2) continue;
    numEvents_2jets++;

    h_hf_event_selection->Fill(1.5, wgt);
    h_lf_event_selection->Fill(1.5, wgt);

    double jet1_btag = jet_CSVv2IVf[0];
    double jet2_btag = jet_CSVv2IVf[1];

    bool passTightBtag = false;
    bool failLooseBtag = false;
    if( jet1_btag>0.814 || jet2_btag>0.814 ) passTightBtag = true;
    if( jet1_btag<0.423 || jet1_btag<0.423 ) failLooseBtag = true;

    double MHT = eve->MHT_[iSys];

    //////------- two leptons -----
    double met_pt = eve->MET_[iSys];
    //bool PassZmask = eve->PassZmask_ ;
    int TwoMuon = eve->TwoMuon_;
    int TwoElectron = eve->TwoElectron_ ;
    int MuonElectron = eve->MuonElectron_ ;

    double mass_leplep = eve->mass_leplep_;
    double dR_leplep = eve->dR_leplep_;
    int oppositeLepCharge = eve->oppositeLepCharge_;

    vvdouble lepton_vect_TLV = eve->lepton_vect_TLV_;
    vint lepton_isTight = eve->lepton_isTight_;
    vint lepton_isLoose = eve->lepton_isLoose_;
    vint lepton_trkCharge = eve->lepton_trkCharge_;

    int numLooseLeptons=0;
    for( int iLep=0; iLep<int(lepton_vect_TLV.size()); iLep++ ){
      bool isLoose1 = lepton_isLoose[iLep];
      if( !isLoose1 ) continue;
      numLooseLeptons++;

      int charge1 = lepton_trkCharge[iLep];

      TLorentzVector myLep1;
      myLep1.SetPxPyPzE( lepton_vect_TLV[iLep][0], lepton_vect_TLV[iLep][1], lepton_vect_TLV[iLep][2], lepton_vect_TLV[iLep][3] );

      for( int jLep=iLep+1; jLep<int(lepton_vect_TLV.size()); jLep++ ){
	if( iLep==jLep ) continue;
	bool isLoose2 = lepton_isLoose[jLep];
	if( !isLoose2 ) continue;

	int charge2 = lepton_trkCharge[jLep];

	TLorentzVector myLep2;
	myLep2.SetPxPyPzE( lepton_vect_TLV[jLep][0], lepton_vect_TLV[jLep][1], lepton_vect_TLV[jLep][2], lepton_vect_TLV[jLep][3] );

	TLorentzVector sum = myLep1 + myLep2;
	mass_leplep = sum.M();

	dR_leplep = myLep1.DeltaR(myLep2);

	if( (charge1 * charge2)==-1 )     oppositeLepCharge = 1;
	else if( (charge1 * charge2)==1 ) oppositeLepCharge = 0;
	else if( charge1==-99 )           oppositeLepCharge = -1;
	else if( charge2==-99 )           oppositeLepCharge = -2;
	else                              oppositeLepCharge = -3;
      }
    }

    h_numLooseLeptons->Fill(numLooseLeptons,wgt);

    bool PassZmask = ( MuonElectron || 
		       (mass_leplep < (65.5 + 3*MHT/8)) || 
		       (mass_leplep > (108 - MHT/4)) || 
		       (mass_leplep < (79 - 3*MHT/4)) || 
		       (mass_leplep > (99 + MHT/2)) 
		       );

    

    /// triggers 
    bool isDoubleMuTriggerPass = eve->passDoubleMuonTrigger_;//1;
    bool isDoubleElectronTriggerPass = eve->passDoubleElectronTrigger_;//1;
    bool isMuEGTriggerPass = eve->passElectronMuonTrigger_;//1;


    // for different datasets or sub-lep categories
    bool lepselection1a = ( TwoMuon && isDoubleMuTriggerPass && (PassZmask==1) && (met_pt>50) ); //Selection for TwoMuon data events
    bool lepselection1b = ( TwoElectron && isDoubleElectronTriggerPass && (PassZmask==1) && (met_pt>50) ); //Selection for TwoEle data events
    bool lepselection1c = ( MuonElectron && isMuEGTriggerPass ); //Selection for MuonEle data events
    if (!isHF){
      lepselection1a = ( TwoMuon && isDoubleMuTriggerPass && (PassZmask==0) && (met_pt<30) && abs(mass_leplep-91)<10 ); //Selection for TwoMuon data events
      lepselection1b = ( TwoElectron && isDoubleElectronTriggerPass && (PassZmask==0) && (met_pt<30) && abs(mass_leplep-91)<10 ); //Selection for TwoEle data events
      lepselection1c = 0; // ( MuonElectron && isMuEGTriggerPass ); //Selection for MuonEle data events
    }
    // for MC events
    bool lepselection2 = ( lepselection1a || lepselection1b || lepselection1c ) ;
    // if (!isHF) lepselection2 = ( lepselection1a || lepselection1b || lepselection1c ) ;

    if( lepselection1a ) numEvents_lepselection1a++;
    if( lepselection1b ) numEvents_lepselection1b++;
    if( lepselection1c ) numEvents_lepselection1c++;


    bool isCleanEvent = 1;

    bool exselection = ((dR_leplep > 0.2) && (mass_leplep > 12) && (isCleanEvent == 1) && (oppositeLepCharge == 1)); //General dilepton selection   


    // trigger
    if( isDoubleMuTriggerPass || isDoubleElectronTriggerPass || isMuEGTriggerPass ){
      h_hf_event_selection->Fill(2.5, wgt);
      if( TwoMuon || TwoElectron || MuonElectron ){
	h_hf_event_selection->Fill(3.5, wgt);
	if( oppositeLepCharge == 1 ){
	  h_hf_event_selection->Fill(4.5, wgt);
	  if( (dR_leplep > 0.2) ){
	    h_hf_event_selection->Fill(5.5, wgt);
	    if( mass_leplep > 12 ){
	      h_hf_event_selection->Fill(6.5, wgt);
	      if( PassZmask==1 && fabs(mass_leplep-91)>10 ){
		h_hf_event_selection->Fill(7.5, wgt);
		if( met_pt>50 ){
		  h_hf_event_selection->Fill(8.5, wgt);
		  if( passTightBtag ){
		    h_hf_event_selection->Fill(9.5, wgt);
		  }
		}
	      }
	    }
	  }
	}
      }
    }

    if( isDoubleMuTriggerPass || isDoubleElectronTriggerPass ){
      h_lf_event_selection->Fill(2.5, wgt);
      if( TwoMuon || TwoElectron ){
	h_lf_event_selection->Fill(3.5, wgt);
	if( oppositeLepCharge == 1 ){
	  h_lf_event_selection->Fill(4.5, wgt);
	  if( (dR_leplep > 0.2) ){
	    h_lf_event_selection->Fill(5.5, wgt);
	    if( mass_leplep > 12 ){
	      h_lf_event_selection->Fill(6.5, wgt);
	      if( PassZmask==0 && fabs(mass_leplep-91)<10 ){
		h_lf_event_selection->Fill(7.5, wgt);
		if( met_pt<30 ){
		  h_lf_event_selection->Fill(8.5, wgt);
		  if( failLooseBtag ){
		    h_lf_event_selection->Fill(9.5, wgt);
		  }
		}
	      }
	    }
	  }
	}
      }
    }


    if ( !lepselection2 ) continue;
    numEvents_lepselection2++;



    /////------ extra seletions -----
    // double dR_leplep = eve->dR_leplep_;
    // bool isCleanEvent = 1;
    // int oppositeLepCharge = eve->oppositeLepCharge_;

    // bool exselection = ((dR_leplep > 0.2) && (mass_leplep > 12) && (isCleanEvent == 1) && (oppositeLepCharge == 1)); //General dilepton selection   
    // bool exselection = ((dR_leplep > 0.2) && (mass_leplep > 12) && (isCleanEvent == 1)); //General dilepton selection // opposite sign applied in treemaking

    if ( !exselection ) continue;
    numEvents_exselection++;


    nPass++;
    ///// --------jet variables
    //vdouble jet_CSV = eve->jet_loose_CSV_[iSys];
    vdouble jet_vtxMass = eve->jet_loose_vtxMass_[iSys];
    vdouble jet_vtxNtracks = eve->jet_loose_vtxNtracks_[iSys];
    vdouble jet_vtx3DVal = eve->jet_loose_vtx3DVal_[iSys];
    vdouble jet_vtx3DSig = eve->jet_loose_vtx3DSig_[iSys];
    vint jet_flavour = eve->jet_loose_flavour_[iSys];

    double first_jet_pt = -9.;
    double first_jet_eta = -9.;
    double first_jet_csv = -9.;
    double first_jet_vtxMass = -9.;
    double first_jet_vtxNtracks = -9.;
    double first_jet_vtx3DVal = -9.;
    double first_jet_vtx3DSig = -9.;
    int    first_jet_flavour = -9;

    double second_jet_pt = -9.;
    double second_jet_eta = -9.;
    double second_jet_csv = -9.;
    double second_jet_vtxMass = -9.;
    double second_jet_vtxNtracks = -9.;
    double second_jet_vtx3DVal = -9.;
    double second_jet_vtx3DSig = -9.;
    int    second_jet_flavour = -9;

    if( verbose ) std::cout << "--for event  "<< cnt << std::endl;    
    if( verbose ) std::cout << " --number of jets is "<< numJets << std::endl;    

    double minDR_lepton_first_jet = 99, minDR_lepton_second_jet = 99;


    for( int iJet=0; iJet<int(jet_vect_TLV.size()); iJet++ ){
      TLorentzVector myJet;
      myJet.SetPxPyPzE( jet_vect_TLV[iJet][0], jet_vect_TLV[iJet][1], jet_vect_TLV[iJet][2], jet_vect_TLV[iJet][3] );

      double myCSV = jet_CSVv2IVf[iJet];
      double myVtxMass = jet_vtxMass[iJet];
      double myVtxNtracks = jet_vtxNtracks[iJet];
      double myVtx3DVal = jet_vtx3DVal[iJet];
      double myVtx3DSig = jet_vtx3DSig[iJet];

      double myJetPt = myJet.Pt();
      double myJetEta = myJet.Eta();
      int myFlavor = jet_flavour[iJet];
      
      if( iJet==0 ){
	first_jet_pt = myJetPt;
	first_jet_eta = myJetEta;
	first_jet_csv = myCSV;
	first_jet_vtxMass = myVtxMass;
	first_jet_vtxNtracks = myVtxNtracks;
	first_jet_vtx3DVal = myVtx3DVal;
	first_jet_vtx3DSig = myVtx3DSig;
	first_jet_flavour = myFlavor;

	for( int iLep=0; iLep<int(lepton_vect_TLV.size()); iLep++ ){
	  TLorentzVector myLep;
	  myLep.SetPxPyPzE( lepton_vect_TLV[iLep][0], lepton_vect_TLV[iLep][1], lepton_vect_TLV[iLep][2], lepton_vect_TLV[iLep][3] );

	  double dR = myLep.DeltaR(myJet);
	  if( dR<minDR_lepton_first_jet ) minDR_lepton_first_jet = dR;

	  TLorentzVector sum = myLep + myJet;
	  h_mass_lepton_first_jet->Fill(sum.M());
	  if( lepton_isTight[iLep] ) h_mass_tight_lepton_first_jet->Fill(sum.M());
	}
      }
      if( iJet==1 ){
	second_jet_pt = myJetPt;
	second_jet_eta = myJetEta;
	second_jet_csv = myCSV;
	second_jet_vtxMass = myVtxMass;
	second_jet_vtxNtracks = myVtxNtracks;
	second_jet_vtx3DVal = myVtx3DVal;
	second_jet_vtx3DSig = myVtx3DSig;
	second_jet_flavour = myFlavor;

	for( int iLep=0; iLep<int(lepton_vect_TLV.size()); iLep++ ){
	  TLorentzVector myLep;
	  myLep.SetPxPyPzE( lepton_vect_TLV[iLep][0], lepton_vect_TLV[iLep][1], lepton_vect_TLV[iLep][2], lepton_vect_TLV[iLep][3] );

	  double dR = myLep.DeltaR(myJet);
	  if( dR<minDR_lepton_second_jet ) minDR_lepton_second_jet = dR;

	  TLorentzVector sum = myLep + myJet;
	  h_mass_lepton_second_jet->Fill(sum.M());
	  h_mass_lepton_second_jet_eta->Fill(second_jet_eta,sum.M());
	  if( lepton_isTight[iLep] ) h_mass_tight_lepton_second_jet->Fill(sum.M());

	}
      }


    } // end loop over jets
    

    if( verbose ) std::cout << "   -first jet pt, eta is " << first_jet_pt << ";  "<<first_jet_eta << std::endl;
    if( verbose ) std::cout << "   -second jet pt, eta is " << second_jet_pt << ";  "<<second_jet_eta << std::endl;

    h_first_jet_pt->Fill(first_jet_pt);
    h_first_jet_eta->Fill(first_jet_eta);
    h_first_jet_csv->Fill(first_jet_csv);
    h_first_jet_vtxMass->Fill(first_jet_vtxMass);
    h_first_jet_vtxNtracks->Fill(first_jet_vtxNtracks);
    h_first_jet_vtx3DVal->Fill(first_jet_vtx3DVal);
    h_first_jet_vtx3DSig->Fill(first_jet_vtx3DSig);
    h_first_jet_flavour->Fill(first_jet_flavour);
    if( abs(first_jet_flavour)==5 )      h_first_jet_vtxMass_b->Fill(first_jet_vtxMass);
    else if( abs(first_jet_flavour)==4 ) h_first_jet_vtxMass_c->Fill(first_jet_vtxMass);
    else if( abs(first_jet_flavour)==0 ) h_first_jet_vtxMass_o->Fill(first_jet_vtxMass);
    else                                 h_first_jet_vtxMass_l->Fill(first_jet_vtxMass);

    h_first_jet_csv_vtxMass->Fill(first_jet_csv,first_jet_vtxMass);
    if( abs(first_jet_flavour)==5 )      h_first_jet_csv_vtxMass_b->Fill(first_jet_csv,first_jet_vtxMass);
    else if( abs(first_jet_flavour)==4 ) h_first_jet_csv_vtxMass_c->Fill(first_jet_csv,first_jet_vtxMass);
    else if( abs(first_jet_flavour)==0 ) h_first_jet_csv_vtxMass_o->Fill(first_jet_csv,first_jet_vtxMass);
    else                                 h_first_jet_csv_vtxMass_l->Fill(first_jet_csv,first_jet_vtxMass);


    h_second_jet_pt->Fill(second_jet_pt);
    h_second_jet_eta->Fill(second_jet_eta);
    h_second_jet_csv->Fill(second_jet_csv);
    h_second_jet_vtxMass->Fill(second_jet_vtxMass);
    h_second_jet_vtxNtracks->Fill(second_jet_vtxNtracks);
    h_second_jet_vtx3DVal->Fill(second_jet_vtx3DVal);
    h_second_jet_vtx3DSig->Fill(second_jet_vtx3DSig);
    h_second_jet_flavour->Fill(second_jet_flavour);
    if( abs(second_jet_flavour)==5 )      h_second_jet_vtxMass_b->Fill(second_jet_vtxMass);
    else if( abs(second_jet_flavour)==4 ) h_second_jet_vtxMass_c->Fill(second_jet_vtxMass);
    else if( abs(second_jet_flavour)==0 ) h_second_jet_vtxMass_o->Fill(second_jet_vtxMass);
    else                                  h_second_jet_vtxMass_l->Fill(second_jet_vtxMass);

    h_second_jet_csv_vtxMass->Fill(second_jet_csv,second_jet_vtxMass);
    if( abs(second_jet_flavour)==5 )      h_second_jet_csv_vtxMass_b->Fill(second_jet_csv,second_jet_vtxMass);
    else if( abs(second_jet_flavour)==4 ) h_second_jet_csv_vtxMass_c->Fill(second_jet_csv,second_jet_vtxMass);
    else if( abs(second_jet_flavour)==0 ) h_second_jet_csv_vtxMass_o->Fill(second_jet_csv,second_jet_vtxMass);
    else                                  h_second_jet_csv_vtxMass_l->Fill(second_jet_csv,second_jet_vtxMass);

    h_second_jet_pt_eta->Fill(second_jet_eta,second_jet_pt);
    h_second_jet_flavour_eta->Fill(second_jet_eta,second_jet_flavour);

    h_minDR_lepton_first_jet->Fill(minDR_lepton_first_jet);
    h_minDR_lepton_second_jet->Fill(minDR_lepton_second_jet);

    if( TwoMuon ) h_second_jet_eta_TwoMuon->Fill(second_jet_eta);
    if( TwoElectron ) h_second_jet_eta_TwoElectron->Fill(second_jet_eta);
    if( MuonElectron ) h_second_jet_eta_MuonElectron->Fill(second_jet_eta);



    double csvWgtHF, csvWgtLF, csvWgtCF;
    double newCSVwgt = ( insample<0 ) ? 1 : get_csv_wgt(jet_vect_TLV, jet_CSVv2IVf, jet_flavour,iSys, csvWgtHF, csvWgtLF, csvWgtCF);
    double wgtfakeData = wgt*newCSVwgt;
    if( verbose ) std::cout << " HF/LF csv wgts are: " << csvWgtHF << "/"<< csvWgtLF << "\t new CSV wgt = " << newCSVwgt << std::endl; 
    ///// for iteration
    if (verNum !=0 ) {
      if ( isHF ) wgt *= csvWgtLF; // applying lfSFs
      else        wgt *= csvWgtHF; // applying lfSFs
    }


    ///// ------  tag and proble jet selections --------
    // 2nd jet --> tag, 1st jet --> proble
    //bool jetselection2a = (second_jet_csv > 0.679) ? 1:0; //Probe jet being the first_jet
    bool jetselection2a = (second_jet_csv > 0.814) ? 1:0; //Probe jet being the first_jet
    bool firstjetb = ( abs(first_jet_flavour)==5 ) ? 1:0;


    if(!isHF) {
      //jetselection2a = second_jet_csv < 0.244 ? 1:0 ; 
      jetselection2a = second_jet_csv < 0.423 ? 1:0 ; 
      firstjetb = ( abs(first_jet_flavour)==5 || abs(first_jet_flavour)==4 );
    }


    if ( jetselection2a ){
      double jetPt = first_jet_pt;
      double jetAbsEta = fabs(first_jet_eta);

      int iPt = -1; int iEta = -1;
      if (jetPt >=19.99 && jetPt<30) iPt = 0;
      else if (jetPt >=30 && jetPt<40) iPt = 1;
      else if (jetPt >=40 && jetPt<60) iPt = 2;
      else if (jetPt >=60 && jetPt<100) iPt = 3;
      else if (jetPt >=100 && jetPt<160) iPt = 4;
      else if (jetPt >=160 && jetPt<10000) iPt = 5;
      
      if (jetAbsEta >=0 &&  jetAbsEta<0.8 ) iEta = 0;
      else if ( jetAbsEta>=0.8 && jetAbsEta<1.6 )  iEta = 1;
      else if ( jetAbsEta>=1.6 && jetAbsEta<2.41 ) iEta = 2;

      if (isHF && iEta>0) iEta=0;
      if (!isHF && iPt>3) iPt=3;

      ///fake data
      h_Data_jet_csv[iPt][iEta]->Fill(first_jet_csv, wgtfakeData); 

      if( firstjetb ){
	h_MC_b_jet_csv[iPt][iEta]->Fill(first_jet_csv, wgt);
	h_MC_b_jet_vtxMass[iPt][iEta]->Fill(first_jet_vtxMass, wgt);
	h_MC_b_jet_csv_vtxMass[iPt][iEta]->Fill(first_jet_csv, first_jet_vtxMass, wgt);
      }
      else  {
	h_MC_nonb_jet_csv[iPt][iEta]->Fill(first_jet_csv, wgt);
	h_MC_nonb_jet_vtxMass[iPt][iEta]->Fill(first_jet_vtxMass, wgt);
	h_MC_nonb_jet_csv_vtxMass[iPt][iEta]->Fill(first_jet_csv, first_jet_vtxMass, wgt);
      }
    }

    // 1st jet --> tag, 2nd jet --> proble
    //bool jetselection2b = ( first_jet_csv > 0.679) ; //Probe jet being the second_jet
    bool jetselection2b = ( first_jet_csv > 0.814) ; //Probe jet being the second_jet
    bool secondjetb = ( abs(second_jet_flavour)==5 );
    if(!isHF) {
      //jetselection2b = first_jet_csv < 0.244 ? 1:0 ; 
      jetselection2b = first_jet_csv < 0.423 ? 1:0 ; 
      secondjetb = ( abs(second_jet_flavour)==5 || abs(second_jet_flavour)==4 );
    }


    if ( jetselection2b ){
      double jetPt = second_jet_pt;
      double jetAbsEta = fabs(second_jet_eta);

      int iPt = -1; int iEta = -1;
      if (jetPt >=19.99 && jetPt<30) iPt = 0;
      else if (jetPt >=30 && jetPt<40) iPt = 1;
      else if (jetPt >=40 && jetPt<60) iPt = 2;
      else if (jetPt >=60 && jetPt<100) iPt = 3;
      else if (jetPt >=100 && jetPt<160) iPt = 4;
      else if (jetPt >=160 && jetPt<10000) iPt = 5;
      
      if (jetAbsEta >=0 &&  jetAbsEta<0.8 ) iEta = 0;
      else if ( jetAbsEta>=0.8 && jetAbsEta<1.6 )  iEta = 1;
      else if ( jetAbsEta>=1.6 && jetAbsEta<2.41 ) iEta = 2;

      if (isHF && iEta>0) iEta=0;
      if (!isHF && iPt>3) iPt=3;

      ///fake data
      h_Data_jet_csv[iPt][iEta]->Fill(second_jet_csv, wgtfakeData);       

      if( secondjetb ){
	h_MC_b_jet_csv[iPt][iEta]->Fill(second_jet_csv, wgt); 
	h_MC_b_jet_vtxMass[iPt][iEta]->Fill(second_jet_vtxMass, wgt);
	h_MC_b_jet_csv_vtxMass[iPt][iEta]->Fill(second_jet_csv, second_jet_vtxMass, wgt);
      }
      else {
	h_MC_nonb_jet_csv[iPt][iEta]->Fill(second_jet_csv, wgt); 
	h_MC_nonb_jet_vtxMass[iPt][iEta]->Fill(second_jet_vtxMass, wgt);
	h_MC_nonb_jet_csv_vtxMass[iPt][iEta]->Fill(second_jet_csv, second_jet_vtxMass, wgt);
      }
    }

    

  } // end loop over events
  std::cout << "total selected events is " << nPass << std::endl;

  std::cout << "===========================================" << std::endl;
  std::cout << "\t Number of all events = " << numEvents_all << std::endl;
  std::cout << "\t Number of events with ==2 jets = " << numEvents_2jets << std::endl;
  std::cout << "\t Number of events with lepselection2  = " << numEvents_lepselection2 << std::endl;
  std::cout << "\t Number of events with lepselection1a = " << numEvents_lepselection1a << std::endl;
  std::cout << "\t Number of events with lepselection1b = " << numEvents_lepselection1b << std::endl;
  std::cout << "\t Number of events with lepselection1c = " << numEvents_lepselection1c << std::endl;
  std::cout << "\t Number of events with exselection    = " << numEvents_exselection << std::endl;
  std::cout << "===========================================" << std::endl;


  std::cout << " Done! " << std::endl;

  histofile.Write();
  histofile.Close();

}




/// algos / supporting functions:

void fillCSVhistos(TFile* fileHF, TFile* fileLF){

  for( int iSys=0; iSys<9; iSys++ ){
    for( int iPt=0; iPt<5; iPt++ ) h_csv_wgt_hf[iSys][iPt] = NULL;
    for( int iPt=0; iPt<3; iPt++ ){
      for( int iEta=0; iEta<3; iEta++ )h_csv_wgt_lf[iSys][iPt][iEta] = NULL;
    }
  }
  for( int iSys=0; iSys<5; iSys++ ){
    for( int iPt=0; iPt<5; iPt++ ) c_csv_wgt_hf[iSys][iPt] = NULL;
  }

  // CSV reweighting /// only care about the nominal ones
  for( int iSys=0; iSys<1; iSys++ ){
    TString syst_csv_suffix_hf = "final";
    TString syst_csv_suffix_c = "final";
    TString syst_csv_suffix_lf = "final";
    
    switch( iSys ){
    case 0:
      // this is the nominal case
      break;
    case 1:
      // JESUp
      syst_csv_suffix_hf = "final_JESUp"; syst_csv_suffix_lf = "final_JESUp";
      syst_csv_suffix_c  = "final_cErr1Up";
      break;
    case 2:
      // JESDown
      syst_csv_suffix_hf = "final_JESDown"; syst_csv_suffix_lf = "final_JESDown";
      syst_csv_suffix_c  = "final_cErr1Down";
      break;
    case 3:
      // purity up
      syst_csv_suffix_hf = "final_LFUp"; syst_csv_suffix_lf = "final_HFUp";
      syst_csv_suffix_c  = "final_cErr2Up";
      break;
    case 4:
      // purity down
      syst_csv_suffix_hf = "final_LFDown"; syst_csv_suffix_lf = "final_HFDown";
      syst_csv_suffix_c  = "final_cErr2Down";
      break;
    case 5:
      // stats1 up
      syst_csv_suffix_hf = "final_Stats1Up"; syst_csv_suffix_lf = "final_Stats1Up";
      break;
    case 6:
      // stats1 down
      syst_csv_suffix_hf = "final_Stats1Down"; syst_csv_suffix_lf = "final_Stats1Down";
      break;
    case 7:
      // stats2 up
      syst_csv_suffix_hf = "final_Stats2Up"; syst_csv_suffix_lf = "final_Stats2Up";
      break;
    case 8:
      // stats2 down
      syst_csv_suffix_hf = "final_Stats2Down"; syst_csv_suffix_lf = "final_Stats2Down";
      break;
    }

    for( int iPt=0; iPt<6; iPt++ ) h_csv_wgt_hf[iSys][iPt] = (TH1D*)fileHF->Get( Form("csv_ratio_Pt%i_Eta0_%s",iPt,syst_csv_suffix_hf.Data()) );

    if( iSys<5 ){
      for( int iPt=0; iPt<6; iPt++ ) c_csv_wgt_hf[iSys][iPt] = (TH1D*)fileHF->Get( Form("c_csv_ratio_Pt%i_Eta0_%s",iPt,syst_csv_suffix_c.Data()) );
    }
    
    for( int iPt=0; iPt<4; iPt++ ){
      for( int iEta=0; iEta<3; iEta++ )h_csv_wgt_lf[iSys][iPt][iEta] = (TH1D*)fileLF->Get( Form("csv_ratio_Pt%i_Eta%i_%s",iPt,iEta,syst_csv_suffix_lf.Data()) );
    }
  }

  return;
}

double get_csv_wgt( vvdouble jets, vdouble jetCSV, vint jetFlavor, int iSys, double &csvWgtHF, double &csvWgtLF, double &csvWgtCF ){

  int iSysHF = 0;
  switch(iSys){
  case 11: iSysHF=1; break;
  case 12: iSysHF=2; break;
  case 17: iSysHF=3; break;
  case 18: iSysHF=4; break;
  case 21: iSysHF=5; break;
  case 22: iSysHF=6; break;
  case 25: iSysHF=7; break;
  case 26: iSysHF=8; break;
  default : iSysHF = 0; break;
  }

  int iSysC = 0;
  switch(iSys){
  case 29: iSysC=1; break;
  case 30: iSysC=2; break;
  case 31: iSysC=3; break;
  case 32: iSysC=4; break;
  default : iSysC = 0; break;
  }

  int iSysLF = 0;
  switch(iSys){
  case 11: iSysLF=1; break;
  case 12: iSysLF=2; break;
  case 19: iSysLF=3; break;
  case 20: iSysLF=4; break;
  case 23: iSysLF=5; break;
  case 24: iSysLF=6; break;
  case 27: iSysLF=7; break;
  case 28: iSysLF=8; break;
  default : iSysLF = 0; break;
  }

  double csvWgthf = 1.;
  double csvWgtC  = 1.;
  double csvWgtlf = 1.;

  for( int iJet=0; iJet<int(jets.size()); iJet++ ){
    TLorentzVector myJet;
    myJet.SetPxPyPzE( jets[iJet][0], jets[iJet][1], jets[iJet][2], jets[iJet][3] );
	  
    double csv = jetCSV[iJet];
    double jetPt = myJet.Pt();
    double jetAbsEta = fabs(myJet.Eta());
    int flavor = jetFlavor[iJet];

    int iPt = -1; int iEta = -1;
    if (jetPt >=19.99 && jetPt<30) iPt = 0;
    else if (jetPt >=30 && jetPt<40) iPt = 1;
    else if (jetPt >=40 && jetPt<60) iPt = 2;
    else if (jetPt >=60 && jetPt<100) iPt = 3;
    else if (jetPt >=100 && jetPt<160) iPt = 4;
    else if (jetPt >=160 && jetPt<10000) iPt = 5;

    if (jetAbsEta >=0 &&  jetAbsEta<0.8 ) iEta = 0;
    else if ( jetAbsEta>=0.8 && jetAbsEta<1.6 )  iEta = 1;
    else if ( jetAbsEta>=1.6 && jetAbsEta<2.41 ) iEta = 2;

    if (iPt < 0 || iEta < 0) std::cout << "Error, couldn't find Pt, Eta bins for this b-flavor jet, jetPt = " << jetPt << ", jetAbsEta = " << jetAbsEta << std::endl;

    if (abs(flavor) == 5 ){
      int useCSVBin = (csv>=0.) ? h_csv_wgt_hf[iSysHF][iPt]->FindBin(csv) : 1;
      double iCSVWgtHF = h_csv_wgt_hf[iSysHF][iPt]->GetBinContent(useCSVBin);
      if( iCSVWgtHF!=0 ) csvWgthf *= iCSVWgtHF;

      // if( iSysHF==0 ) printf(" iJet,\t flavor=%d,\t pt=%.1f,\t eta=%.2f,\t csv=%.3f,\t wgt=%.2f \n",
      // 			     flavor, jetPt, iJet->eta, csv, iCSVWgtHF );
    }
    else if( abs(flavor) == 4 ){
      int useCSVBin = (csv>=0.) ? c_csv_wgt_hf[iSysC][iPt]->FindBin(csv) : 1;
      double iCSVWgtC = c_csv_wgt_hf[iSysC][iPt]->GetBinContent(useCSVBin);
      if( iCSVWgtC!=0 ) csvWgtC *= iCSVWgtC;
      // if( iSysC==0 ) printf(" iJet,\t flavor=%d,\t pt=%.1f,\t eta=%.2f,\t csv=%.3f,\t wgt=%.2f \n",
      //      flavor, jetPt, iJet->eta, csv, iCSVWgtC );
    }
    else {
      if (iPt >=3) iPt=3;       /// [30-40], [40-60] and [60-10000] only 3 Pt bins for lf
      int useCSVBin = (csv>=0.) ? h_csv_wgt_lf[iSysLF][iPt][iEta]->FindBin(csv) : 1;
      double iCSVWgtLF = h_csv_wgt_lf[iSysLF][iPt][iEta]->GetBinContent(useCSVBin);
      if( iCSVWgtLF!=0 ) csvWgtlf *= iCSVWgtLF;

      // if( iSysLF==0 ) printf(" iJet,\t flavor=%d,\t pt=%.1f,\t eta=%.2f,\t csv=%.3f,\t wgt=%.2f \n",
      // 			     flavor, jetPt, iJet->eta, csv, iCSVWgtLF );
    }
  }

  double csvWgtTotal = csvWgthf * csvWgtC * csvWgtlf;

  csvWgtHF = csvWgthf;
  csvWgtLF = csvWgtlf;
  csvWgtCF = csvWgtC;

  return csvWgtTotal;
}
