#ifndef INC_BEANLOADER
#define INC_BEANLOADER

///////////////////////////////////////////////////////////////////////////////
// File: BEANloader.h
// 
// Purpose:  Easily load samples
//
// Created:   09-DEC-2011  Darren Puigh
// History:   new!
// Modified:  
//
///////////////////////////////////////////////////////////////////////////////

//
// Dependencies (#includes)
//
#include <iostream>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <algorithm>
#include "TString.h"
#include "TSystem.h"
#include "TLorentzVector.h"

using namespace std;

typedef std::vector<std::string> vstring;

//
// Utility Class for Handling Samples
//
class sampleInfo {

public:

  vstring fileNames;                // vector input file names and paths
  std::string sampleName;           // string for describing sample name
  std::string outputDir;            // output path for anaTree
  std::string str_sampleType;       // 'data' or 'mc'
  
  std::string str_leptonSelection;  // 'mu_sel' 'ele_sel'
  std::string str_dataset;          // associated data: 'SingleMu' 'SingleEle'
  std::string hlt_trigger_name;     // trigger to be used with this sample
  
  std::string str_Njobs;            // string for total number of jobs 
  std::string str_jobN;             // string for job number of output

  std::string treeFileName;         // total output name and path 


  double xSec;     // cross section pb^-1
  int nGen;        // total number of events created with samples
  
  bool isMC;       // ==true if this is MC
  bool isMuon;     // ==true if this is muon selection
  bool isTTJets;   // ==true if this is a ttjets sample
  

};


//
// Utility Namespace for Analysis
//
namespace BEAN{

  const double PI        = 2.0*acos(0.);
  const double TWOPI     = 2.0*PI;
  const float  ETA_LIMIT = 15.0;
  const float  EPSILON   = 1.E-10;

  sampleInfo loadSamples( int sample, std::string era, int beanVer, bool isMuon, int Njobs, int jobN, bool siteOSU=true, bool isSkim=true, bool lpcCondor=false);

}


sampleInfo BEAN::loadSamples( int sample, std::string era, int beanVer, bool isMuon, int Njobs, int jobN, bool siteOSU, bool isSkim, bool lpcCondor ){

  //
  // Intialization
  //

  // Declare sampleInfo object for current sample
  sampleInfo mySample;

  // Vector to contain all files to run over
  vstring fileNames;
  fileNames.clear();

  // Vector of paths to location of each sample in this run
  vstring dirNames;
  dirNames.clear();


  //
  // Set Sample Type
  //
  mySample.str_sampleType = ( sample>=0 ) ? "mc" : "data";
  mySample.isMC           = ( sample>=0 );


  //
  // Intialize TTJets type to false
  //
  mySample.isTTJets = false;


  //
  // Set Lepton Selection
  //
  mySample.isMuon = isMuon;
  if(isMuon){
    mySample.str_leptonSelection = "mu_sel";
    mySample.str_dataset         = "SingleMu";
    mySample.hlt_trigger_name    = "HLT_IsoMu24_eta2p1_v";
  }
  else{
    mySample.str_leptonSelection = "ele_sel";
    mySample.str_dataset         = "SingleElectron";
    mySample.hlt_trigger_name    = "HLT_Ele27_WP80_v";
  }

 
  //
  // Set output Directory
  //
  std::string my_base_dir = "";
  
  // Directory of skims
  std::string skims_dir = "";
  
  if( siteOSU ){
    // Get CMSSW path in current working area
    char *my_pPath        = getenv("CMSSW_BASE");
    std::string my_base_dir_tmp(my_pPath);
    std::string skims_location  = "/mnt/hadoop/se/store/user/puigh/BEAN_53xOn53x/SkimLepJets/";
    my_base_dir           = my_base_dir_tmp;
    //mySample.outputDir    = "/data/users/puigh/yggdrasil/v23_newBEANs_noCharmCSVSF/";
    //mySample.outputDir    = "/data/users/puigh/yggdrasil/v25_newBEANs_newCharmCSVSFUnc/";
    //mySample.outputDir    = "/data/users/puigh/yggdrasil/v26_newBEANs_PierreLightCSVSF/";
    //mySample.outputDir    = "/data/users/puigh/yggdrasil/v27_newBEANs_jetPt25/";
    //mySample.outputDir    = "/data/users/puigh/yggdrasil/v28_newBEANs_jetPt25_lepPt25/";
    //mySample.outputDir    = "/data/users/puigh/yggdrasil/v28_newBEANs_jetPt25_lepPt25_fixedSysts/";
    mySample.outputDir    = "/data/users/puigh/yggdrasil_charlieProjections/v0_charlieProjection_v4/";
    //mySample.outputDir    = "/data/users/puigh/yggdrasil/v24_andrewTest/";
    skims_dir             = skims_location;
    if( beanVer==2 ) skims_dir = "/mnt/hadoop/se/store/user/tth/53Xon53X_newBEANs_201304_Skims/jgwood/";
  } //end if OSU
  else{
 
    // LPC storage directory
    //my_base_dir        = "/uscms_data/d3/jgwood/ttH_ttbb_lepJets/";
    my_base_dir        = "/uscms_data/d2/sboutle/TreesFromNewBEANs2/";
   
    mySample.outputDir = my_base_dir + "2012_analysis/anaTrees_2012_localRunTempStorage/";
    skims_dir          = "dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_newBEANs_201304_Skims/jgwood/";

    if(lpcCondor){
      char *my_pPath = getenv("HOME");
      my_base_dir = my_pPath;
      mySample.outputDir = my_base_dir + "/";
    }

  } // end if LPC




  //
  // Set sampleInfo elements
  //

  switch( sample ){


  ////
  //// DATA FILES
  /////

  //
  // SingleMu Run2012 A+B+C+D
  //
  case -1: 
    mySample.xSec = 1;
    mySample.nGen = 1;
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("/store/user/tth/sboutle/SingleMu/Run2012A-PromptReco-v1_BEAN_53xOn52x_V01_test2/4e5e3a0b952aff83c04c73f234fa0786/");
                dirNames.push_back("/store/user/lannon/SingleMu/Run2012B-PromptReco-v1_BEAN_53xOn52x_V01_CV01/f48cd8016a4be563a58f6bc3022c606f/");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/jgwood/SingleMu/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/lannon/SingleMu/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleMu_2012AB_BEAN_53xOn52x";
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
        if (!isSkim){
                dirNames.push_back("/store/user/abrinke1/SingleMu/SingleMu_Run2012A-13Jul2012-v1_BEAN_53xOn53x_V02_CV01/de3c20242948dd0d39f546037f2fbda9/");
                dirNames.push_back("/store/user/abrinke1/SingleMu/SingleMu_Run2012A-recover-06Aug2012-v1_BEAN_53xOn53x_V02_CV01/25216cad7390119ce70a2ab4eee94ee1/");
                dirNames.push_back("/store/user/abrinke1/SingleMu/SingleMu_Run2012B-13Jul2012-v1_BEAN_53xOn53x_V02_CV01/36fb01f1aafbede72fa3cecfbe30213a/");
                dirNames.push_back("/store/user/abrinke1/SingleMu/SingleMu_Run2012C-24Aug2012-v1_BEAN_53xOn53x_V02_CV01/c9b46d412af800cf309521fc23f82cbb/");
                dirNames.push_back("/store/user/abrinke1/SingleMu/SingleMu_Run2012C-PromptReco-v2_BEAN_53xOn53x_V02_CV01/5966f50c1614e99fadd7a4442d7fe207/");
                dirNames.push_back("/store/user/abrinke1/SingleMu/SingleMu_Run2012D-PromptReco-v1_BEAN_53xOn53x_V02_CV01/4c32d30ed3f823639d81416a264f1466/");
        }
        else {
        	dirNames.push_back(skims_dir+"SingleMu/Run2012A-recover-06Aug2012-v1/");
        	dirNames.push_back(skims_dir+"SingleMu/Run2012B-13Jul2012-v1/");
        	dirNames.push_back(skims_dir+"SingleMu/Run2012C-PromptReco-v2/");
        	dirNames.push_back(skims_dir+"SingleMu/Run2012C-24Aug2012-v1/");
        	dirNames.push_back(skims_dir+"SingleMu/Run2012D-PromptReco-v1/");
        	dirNames.push_back(skims_dir+"SingleMu/Run2012A-13Jul2012-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/SingleMu/Run2012A/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/SingleMu/Run2012A_recover/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/SingleMu/Run2012B/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/SingleMu/Run2012C_promptReco/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/SingleMu/Run2012C_24Aug2012/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/SingleMu/Run2012D/");
      }
      mySample.sampleName = "SingleMu_2012ABCD_BEAN_53xOn53x";
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
        if (!isSkim){
                dirNames.push_back("");
        }
        else {
	        dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012A_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
         	dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012Arecover_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        	dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012B_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        	dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012CpromptReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        	dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012CreReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        	dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012D_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012A_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012Arecover_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012B_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012CpromptReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012CreReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012D_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleMu_2012ABCD_BEAN_53xOn53x_beanVer2";
    }
    else if( era == "2012_53x" && beanVer==3 ){
      if( siteOSU ){
      }
      else {
        //dirNames.push_back("/store/user/muell149/lobster_beans_v1/TTH_125/");
        dirNames.push_back("/store/user/muell149/lobster_BNprod_v2/SingleMu_2012A/");
      }
       mySample.sampleName = "SingleMu_2012A_BEAN_53xOn53x_beanVer3";
    }
    break;


  //
  // SingleMu Run2012A+B 53x
  //
  case -11: 
    mySample.xSec = 1;
    mySample.nGen = 1;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
        if (!isSkim){
                dirNames.push_back("/store/user/abrinke1/SingleMu/SingleMu_Run2012A-13Jul2012-v1_BEAN_53xOn53x_V02_CV01/de3c20242948dd0d39f546037f2fbda9/");
                dirNames.push_back("/store/user/abrinke1/SingleMu/SingleMu_Run2012A-recover-06Aug2012-v1_BEAN_53xOn53x_V02_CV01/25216cad7390119ce70a2ab4eee94ee1/");
                dirNames.push_back("/store/user/abrinke1/SingleMu/SingleMu_Run2012B-13Jul2012-v1_BEAN_53xOn53x_V02_CV01/36fb01f1aafbede72fa3cecfbe30213a/");
        }
        else {
        	dirNames.push_back(skims_dir+"SingleMu/Run2012A-recover-06Aug2012-v1/");
        	dirNames.push_back(skims_dir+"SingleMu/Run2012B-13Jul2012-v1/");
        	dirNames.push_back(skims_dir+"SingleMu/Run2012A-13Jul2012-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/SingleMu/Run2012A/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/SingleMu/Run2012A_recover/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/SingleMu/Run2012B/");
               
      }
      mySample.sampleName = "SingleMu_2012AB_BEAN_53xOn53x";
    }

    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
        if (!isSkim){
                dirNames.push_back("");
        }
        else {
	        dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012A_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
         	dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012Arecover_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        	dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012B_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012A_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012Arecover_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012B_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
               
      }
      mySample.sampleName = "SingleMu_2012AB_BEAN_53xOn53x_beanVer2";
    }
    break;

  //
  // SingleMu Run2012 C
  //
  case -111: 
    mySample.xSec = 1;
    mySample.nGen = 1;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
        if (!isSkim){
                dirNames.push_back("/store/user/abrinke1/SingleMu/SingleMu_Run2012C-24Aug2012-v1_BEAN_53xOn53x_V02_CV01/c9b46d412af800cf309521fc23f82cbb/");
                dirNames.push_back("/store/user/abrinke1/SingleMu/SingleMu_Run2012C-PromptReco-v2_BEAN_53xOn53x_V02_CV01/5966f50c1614e99fadd7a4442d7fe207/");
        }
        else {
        	dirNames.push_back(skims_dir+"SingleMu/Run2012C-PromptReco-v2/");
        	dirNames.push_back(skims_dir+"SingleMu/Run2012C-24Aug2012-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/SingleMu/Run2012C_promptReco/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/SingleMu/Run2012C_24Aug2012/");
      }
      mySample.sampleName = "SingleMu_2012C_BEAN_53xOn53x";
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
        if (!isSkim){
                dirNames.push_back("");
        }
        else {
        	dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012CpromptReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        	dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012CreReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012CpromptReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012CreReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleMu_2012C_BEAN_53xOn53x_beanVer2";
    }
    break;

  //
  // SingleMu Run2012D
  //
  case -1111: 
    mySample.xSec = 1;
    mySample.nGen = 1;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
        if (!isSkim){
                dirNames.push_back("/store/user/abrinke1/SingleMu/SingleMu_Run2012D-PromptReco-v1_BEAN_53xOn53x_V02_CV01/4c32d30ed3f823639d81416a264f1466/");
        }
        else {
        	dirNames.push_back(skims_dir+"SingleMu/Run2012D-PromptReco-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/SingleMu/Run2012D/");
      }
      mySample.sampleName = "SingleMu_2012D_BEAN_53xOn53x";
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
        if (!isSkim){
                dirNames.push_back("");
        }
        else {
        	dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012D_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
             
	dirNames.push_back(skims_dir+"SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012D_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleMu_2012D_BEAN_53xOn53x_beanVer2";
    }
    break;



  //
  // SingleEle Run2012 A+B+C+D
  //
  case -2: 
    mySample.xSec = 1;
    mySample.nGen = 1;
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("/store/user/tth/neu/SingleElectron/Run2012A-PromptReco-v1_BEAN_53xOn52x_V01_CV2/3f1746e8e71d4f4227ee21fd0eaedbc1/");
                dirNames.push_back("/store/user/lannon/SingleElectron/Run2012B-PromptReco-v1_BEAN_53xOn52x_V01_CV01/3f1746e8e71d4f4227ee21fd0eaedbc1/");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/jgwood/SingleElectron/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/lannon/SingleElectron/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleElectron_2012AB_BEAN_53xOn52x";
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
                dirNames.push_back("/store/user/abrinke1/SingleElectron/SingleElectron_Run2012A-13Jul2012-v1_BEAN_53xOn53x_V02_CV01/4da6952f3c4fba5e66dcec676ef9c024/");
                dirNames.push_back("/store/user/abrinke1/SingleElectron/SingleElectron_Run2012A-recover-06Aug2012-v1_BEAN_53xOn53x_V02_CV01/1fbf193eb77602744eea7b0e9011a701/");
                dirNames.push_back("/store/user/abrinke1/SingleElectron/SingleElectron_Run2012B-13Jul2012-v1_BEAN_53xOn53x_V02_CV01/668cf0d6b00f8280f551de795dd2251c/");
                dirNames.push_back("/store/user/abrinke1/SingleElectron/SingleElectron_Run2012C-24Aug2012-v1_BEAN_53xOn53x_V02_CV01/01f96d8335df5d6e4a941ce04b810a5c/");
                dirNames.push_back("/store/user/abrinke1/SingleElectron/SingleElectron_Run2012C-PromptReco-v2_BEAN_53xOn53x_V02_CV01/f475894ed7b9f1551e253ca4988d5150/");
                dirNames.push_back("/store/user/abrinke1/SingleElectron/SingleElectron_Run2012D-PromptReco-v1_BEAN_53xOn53x_V02_CV01/19418dac1813e9ec395eda565d0d2807/");
      	}
      	else {
      		dirNames.push_back(skims_dir+"SingleElectron/Run2012A-recover-06Aug2012-v1/");
      		dirNames.push_back(skims_dir+"SingleElectron/Run2012C-PromptReco-v2/");
      		dirNames.push_back(skims_dir+"SingleElectron/Run2012D-PromptReco-v1/");
      		dirNames.push_back(skims_dir+"SingleElectron/Run2012B-13Jul2012-v1/");
      		dirNames.push_back(skims_dir+"SingleElectron/Run2012A-13Jul2012-v1/");
      		dirNames.push_back(skims_dir+"SingleElectron/Run2012C-24Aug2012-v1/");
      	}
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/SingleEle/jgwood/SingleElectron/NovaBeanSkim_53Xon53X_SingleEle_2012A_20130124_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/SingleEle/jgwood/SingleElectron/NovaBeanSkim_53Xon53X_SingleEle_2012Arecover_20130124_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/SingleEle/jgwood/SingleElectron/NovaBeanSkim_53Xon53X_SingleEle_2012B_20130124_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/SingleEle/jgwood/SingleElectron/NovaBeanSkim_53Xon53X_SingleEle_2012CpromptReco_20130124_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/SingleEle/jgwood/SingleElectron/NovaBeanSkim_53Xon53X_SingleEle_2012C24Aug2012_20130124_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/SingleEle/jgwood/SingleElectron/NovaBeanSkim_53Xon53X_SingleEle_2012D_20130125_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleElectron_2012ABCD_BEAN_53xOn53x";
    }

    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
                dirNames.push_back("");
      	}
      	else {
      		dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012A_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      		dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012Arecover_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      		dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012B_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      		dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012CpromptReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      		dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012CreReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      		dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012D_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      	}
      }
      else {
                dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012A_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012Arecover_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012B_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012CpromptReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012CreReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012D_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleElectron_2012ABCD_BEAN_53xOn53x_beanVer2";
    }
    break;



  //
  // SingleEle Run2012 A+B
  //
  case -22: 
    mySample.xSec = 1;
    mySample.nGen = 1;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
                dirNames.push_back("/store/user/abrinke1/SingleElectron/SingleElectron_Run2012A-13Jul2012-v1_BEAN_53xOn53x_V02_CV01/4da6952f3c4fba5e66dcec676ef9c024/");
                dirNames.push_back("/store/user/abrinke1/SingleElectron/SingleElectron_Run2012A-recover-06Aug2012-v1_BEAN_53xOn53x_V02_CV01/1fbf193eb77602744eea7b0e9011a701/");
                dirNames.push_back("/store/user/abrinke1/SingleElectron/SingleElectron_Run2012B-13Jul2012-v1_BEAN_53xOn53x_V02_CV01/668cf0d6b00f8280f551de795dd2251c/");
      	}
      	else {
      		dirNames.push_back(skims_dir+"SingleElectron/Run2012A-recover-06Aug2012-v1/");
      		dirNames.push_back(skims_dir+"SingleElectron/Run2012B-13Jul2012-v1/");
      		dirNames.push_back(skims_dir+"SingleElectron/Run2012A-13Jul2012-v1/");
      	}
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/SingleEle/jgwood/SingleElectron/NovaBeanSkim_53Xon53X_SingleEle_2012A_20130124_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/SingleEle/jgwood/SingleElectron/NovaBeanSkim_53Xon53X_SingleEle_2012Arecover_20130124_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/SingleEle/jgwood/SingleElectron/NovaBeanSkim_53Xon53X_SingleEle_2012B_20130124_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleElectron_2012AB_BEAN_53xOn53x";
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
                dirNames.push_back("");
      	}
      	else {
      		dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012A_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      		dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012Arecover_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      		dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012B_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      	}
      }
      else {
                dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012A_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012Arecover_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012B_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleElectron_2012AB_BEAN_53xOn53x_beanVer2";
    }
    break;


  //
  // SingleEle Run2012 C
  //
  case -222: 
    mySample.xSec = 1;
    mySample.nGen = 1;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
                dirNames.push_back("/store/user/abrinke1/SingleElectron/SingleElectron_Run2012C-24Aug2012-v1_BEAN_53xOn53x_V02_CV01/01f96d8335df5d6e4a941ce04b810a5c/");
                dirNames.push_back("/store/user/abrinke1/SingleElectron/SingleElectron_Run2012C-PromptReco-v2_BEAN_53xOn53x_V02_CV01/f475894ed7b9f1551e253ca4988d5150/");
      	}
      	else {
      		dirNames.push_back(skims_dir+"SingleElectron/Run2012C-PromptReco-v2/");
      		dirNames.push_back(skims_dir+"SingleElectron/Run2012C-24Aug2012-v1/");
      	}
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/SingleEle/jgwood/SingleElectron/NovaBeanSkim_53Xon53X_SingleEle_2012CpromptReco_20130124_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/SingleEle/jgwood/SingleElectron/NovaBeanSkim_53Xon53X_SingleEle_2012C24Aug2012_20130124_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleElectron_2012C_BEAN_53xOn53x";
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
                dirNames.push_back("");
      	}
      	else {
       		dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012CpromptReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      		dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012CreReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      	}
      }
      else {
                dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012CpromptReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012CreReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleElectron_2012C_BEAN_53xOn53x_beanVer2";
    }
    break;


  //
  // SingleEle Run2012D
  //
  case -2222: 
    mySample.xSec = 1;
    mySample.nGen = 1;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
                dirNames.push_back("/store/user/abrinke1/SingleElectron/SingleElectron_Run2012D-PromptReco-v1_BEAN_53xOn53x_V02_CV01/19418dac1813e9ec395eda565d0d2807/");
      	}
      	else {
      		dirNames.push_back(skims_dir+"SingleElectron/Run2012D-PromptReco-v1/");
      	}
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/SingleEle/jgwood/SingleElectron/NovaBeanSkim_53Xon53X_SingleEle_2012D_20130125_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleElectron_2012D_BEAN_53xOn53x";
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
                dirNames.push_back("");
      	}
      	else {
      		dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012D_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      	}
      }
      else {
                dirNames.push_back(skims_dir+"SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012D_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleElectron_2012D_BEAN_53xOn53x_beanVer2";
    }
    break;




  ////
  //// MC FILES
  ////

  //
  // HIGGS
  //


  //
  // ttH: H -> all, all W decays, mH = case
  //
  case 9110: 
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/ttH110/lannon/TTH_HToAll_M_110_8TeV_FastSim_pythia6/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_ttH110/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_FastSim_HtoAll_M_110_8TeV_53xOn52x";
      mySample.xSec = 0.1887;
      mySample.nGen = 1000000;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
                dirNames.push_back("/store/user/abrinke1/TTH_Inclusive_M-110_8TeV_pythia6/TTH_Inclusive_M-110_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/e0a222973cbabe266461e928aeeb860c/");
        }
        else {
        	dirNames.push_back("/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-110_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M110_20130114_skimv1/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-110_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M110_20130117_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_110_8TeV_53xOn53x";
      mySample.xSec = 0.1887;
      mySample.nGen = 975341;
    }

    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
                dirNames.push_back("");
        }
        else {
      		dirNames.push_back(skims_dir+"TTH_Inclusive_M-110_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M110_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-110_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M110_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_110_8TeV_53xOn53x_beanVer2";
      mySample.xSec = 0.1887;
      mySample.nGen = 975341;
    }
    break;


  //
  // ttH: H -> all, all W decays, mH = case
  //
  case 9115: 
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/ttH115/lannon/TTH_HToAll_M_115_8TeV_FastSim_pythia6/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_ttH115/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_FastSim_HtoAll_M_115_8TeV_53xOn52x";
      mySample.xSec = 0.1663;
      mySample.nGen = 1000000;
    }
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTH_Inclusive_M-115_8TeV_pythia6/TTH_Inclusive_M-115_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/e4aedc27c949a9a9e7b4a07d09e690d9/");
        }
        else {
        	dirNames.push_back("/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-115_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M115_20130117_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-115_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M115_20130117_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_115_8TeV_53xOn53x";
      mySample.xSec = 0.1663;
      mySample.nGen = 997426;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-115_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M115_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-115_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M115_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_115_8TeV_53xOn53x_beanVer2";
      mySample.xSec = 0.1663;
      mySample.nGen = 995188;
    }
    break;


  //
  // ttH: H -> all, all W decays, mH = case
  //
  case 9120: 
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("/store/user/lannon/TTH_HToAll_M_120_8TeV_FastSim_pythia6/TTH_Fastsim_BEAN_53xOn52x_V01_CV01/97f05d45c8f36910cd302b237cd447ba/");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/ttH120/lannon/TTH_HToAll_M_120_8TeV_FastSim_pythia6/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_ttH120/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_FastSim_HtoAll_M_120_8TeV_53xOn52x";
      mySample.xSec = 0.147;
      mySample.nGen = 962498;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTH_Inclusive_M-120_8TeV_pythia6/TTH_Inclusive_M-120_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/91c81eb05f20017cb1a31bcff179d10d/");
        }
        else {
        	dirNames.push_back("/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-120_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M120_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-120_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M120_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_120_8TeV_53xOn53x";
      mySample.xSec = 0.147;
      mySample.nGen = 996773;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-120_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M120_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-120_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M120_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_120_8TeV_53xOn53x_beanVer2";
      mySample.xSec = 0.147;
      mySample.nGen = 996773;
    }
    break;


  //
  // ttH: H -> all, all W decays, mH = case
  //
  case 9122: 
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/ttH122p5/lannon/TTH_HToAll_M_122p5_8TeV_FastSim_pythia6/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_ttH122p5/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_FastSim_HtoAll_M_122p5_8TeV_53xOn52x";
      mySample.xSec = 0.1383;
      mySample.nGen = 997500;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTH_Inclusive_M-122_5_8TeV_pythia6/TTH_Inclusive_M-122_5_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/79cb3edb2dc864b2d9b53560db47100b/");
        }
        else {
        	dirNames.push_back("/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-122_5_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M122p5_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-122_5_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M122p5_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_122p5_8TeV_53xOn53x";
      mySample.xSec = 0.1383;
      mySample.nGen = 996756;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-122_5_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M122p5_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-122_5_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M122p5_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_122p5_8TeV_53xOn53x_beanVer2";
      mySample.xSec = 0.1383;
      mySample.nGen = 996756;
    }
    break;


  //
  // ttH: H -> all, all W decays, mH = case
  //
  case 9125: 
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/ttH125/lannon/TTH_HToAll_M_125_8TeV_FastSim_pythia6/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_ttH125/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_FastSim_HtoAll_M_125_8TeV_53xOn52x";
      mySample.xSec = 0.1302;
      mySample.nGen = 1000000;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTH_Inclusive_M-125_8TeV_pythia6/TTH_Inclusive_M-125_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/56e316a58ddab2973d0823a8581eeac7/");
        }
        else {
        	dirNames.push_back("/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M125_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M125_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_125_8TeV_53xOn53x";
      mySample.xSec = 0.1302;
      mySample.nGen = 992997;
    }

    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/awoodard/TTH_Inclusive_M-125_8TeV_pythia6/BEAN_GTV7G_V01_CV01/411302ae24b059b12fe5d86298d5421d/");
        }
        else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M125_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M125_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_125_8TeV_53xOn53x_beanVer2";
      mySample.xSec = 0.1302;
      mySample.nGen = 992997;
    }

    else if( era == "2012_53x" && beanVer==3 ){
      if( siteOSU ){
      }
      else {
	//dirNames.push_back("/store/user/muell149/lobster_beans_v1/TTH_125/");
	dirNames.push_back("/store/user/muell149/lobster_BNprod_v4/ttH_125/");
      }
      mySample.sampleName = "TTH_Inclusive_M_125_8TeV_53xOn53x_beanVer3";
      mySample.xSec = 0.1302;
      mySample.nGen = 995697;//992997;
    }


    break;


  //
  // ttH: H -> all, all W decays, mH = case
  //
  case 9127: 
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/ttH127p5/lannon/TTH_HToAll_M_127p5_8TeV_FastSim_pythia6_crabv2/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_ttH127p5/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_FastSim_HtoAll_M_127p5_8TeV_53xOn52x";
      mySample.xSec = 0.1227;
      mySample.nGen = 1000000;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTH_Inclusive_M-127_5_8TeV_pythia6/TTH_Inclusive_M-127_5_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/3a891b0ca2989fd79a4822c85930fdd3/");
        }
        else {
        	dirNames.push_back("/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-127_5_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M127p5_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-127_5_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M127p5_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_127p5_8TeV_53xOn53x";
      mySample.xSec = 0.1227;
      mySample.nGen = 997257;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-127_5_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M127p5_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-127_5_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M127p5_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_127p5_8TeV_53xOn53x_beanVer2";
      mySample.xSec = 0.1227;
      mySample.nGen = 997257;
    }
    break;


  //
  // ttH: H -> all, all W decays, mH = case
  //
  case 9130: 
    if( era == "2012_52x" ){
      if( siteOSU ){
               dirNames.push_back("");
      }
      else {
               dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/ttH130/lannon/TTH_HToAll_M_130_8TeV_FastSim_pythia6/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_ttH130/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_FastSim_HtoAll_M_130_8TeV_53xOn52x";
      mySample.xSec = 0.1157;
      mySample.nGen = 1000000;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTH_Inclusive_M-130_8TeV_pythia6/TTH_Inclusive_M-130_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/de6df00d5ea2082fdc02079fa215e4d7/");
        }
        else {
        	dirNames.push_back("/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-130_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M130_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-130_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M130_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_130_8TeV_53xOn53x";
      mySample.xSec = 0.1157;
      mySample.nGen = 931369;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-130_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M130_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-130_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M130_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_130_8TeV_53xOn53x_beanVer2";
      mySample.xSec = 0.1157;
      mySample.nGen = 931369;
    }
    break;


  //
  // ttH: H -> all, all W decays, mH = case
  //
  case 9135: 
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/ttH135/lannon/TTH_HToAll_M_135_8TeV_FastSim_pythia6/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_ttH135/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_FastSim_HtoAll_M_135_8TeV_53xOn52x";
      mySample.xSec = 0.1031;
      mySample.nGen = 1000000;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTH_Inclusive_M-135_8TeV_pythia6/TTH_Inclusive_M-135_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/3571ad32b075790f86f8c0a200478a9b/");
        }
        else {
        	dirNames.push_back("/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-135_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M135_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-135_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M135_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_135_8TeV_53xOn53x";
      mySample.xSec = 0.1031;
      mySample.nGen = 993975;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-135_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M135_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-135_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M135_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_135_8TeV_53xOn53x_beanVer2";
      mySample.xSec = 0.1031;
      mySample.nGen = 993975;
    }
    break;


  //
  // ttH: H -> all, all W decays, mH = case
  //
  case 9140: 
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/ttH140/lannon/TTH_HToAll_M_140_8TeV_FastSim_pythia6/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_ttH140/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_FastSim_HtoAll_M_140_8TeV_53xOn52x";
      mySample.xSec = 0.09207;
      mySample.nGen = 1000000;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTH_Inclusive_M-140_8TeV_pythia6/TTH_Inclusive_M-140_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/ca24a355635c4b0f6cc4dbb5bba601dd/");
        }
        else {
        	dirNames.push_back("/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-140_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M140_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTH_Inclusive_M-140_8TeV_pythia6/NovaBeanSkim_53Xon53X_TTH_Inclusive_M140_20130118_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_140_8TeV_53xOn53x";
      mySample.xSec = 0.09207;
      mySample.nGen = 997191;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-140_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M140_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTH_Inclusive_M-140_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M140_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTH_Inclusive_M_140_8TeV_53xOn53x_beanVer2";
      mySample.xSec = 0.09207;
      mySample.nGen = 997191;
    }
    break;





  //
  // Backgrounds
  //

  case 2500: // ttbar madgraph, tt+lf
    mySample.isTTJets = true;
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("/store/user/lannon/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S6_START52_V9-v1_BEAN_53xOn52x_V01_CV01/b9130084bab6b1e2e321806968edb2ef/");
      }
      else {
                dirNames.push_back("/uscms_data/d2/lannon/ttjetsSkim/");
      }
      mySample.sampleName = "TTJetsLF_MassiveBinDECAY_TuneZ2star_8TeV_madgraph_Summer12_53xOn52x";
      mySample.xSec = 245.8;
      mySample.nGen = 7610228;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
	  //dirNames.push_back("/store/user/abrinke1/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/932e15c487207b473572def8d9167a18/");
	  //    dirNames.push_back("/store/user/abrinke1/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/932e15c487207b473572def8d9167a18/");
	  dirNames.push_back("/mnt/hadoop/se/store/user/abrinke1/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/932e15c487207b473572def8d9167a18/");
	  dirNames.push_back("/mnt/hadoop/se/store/user/abrinke1/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/932e15c487207b473572def8d9167a18/");
        }
        else {
   	        dirNames.push_back(skims_dir+"TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/");
	        dirNames.push_back(skims_dir+"TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/NovaBeanSkim_53Xon53X_TTJets_MassiveBinDECAY_prt1_20130119_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/NovaBeanSkim_53Xon53X_TTJets_MassiveBinDECAY_prt2_20130119_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsLF_MassiveBinDECAY_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8;
      mySample.nGen = 8252095;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
	  dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_MassiveBinDECAY_v1_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_MassiveBinDECAY_v1_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsLF_MassiveBinDECAY_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8;
      mySample.nGen = 6912438;
    }
    break;



  case 2544: // ttbar madgraph, tt+cc
    mySample.isTTJets = true;
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("/store/user/lannon/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S6_START52_V9-v1_BEAN_53xOn52x_V01_CV01/b9130084bab6b1e2e321806968edb2ef/");
      }
      else {
                dirNames.push_back("/uscms_data/d2/lannon/ttjetsSkim/");
      }
      mySample.sampleName = "TTJetsCC_MassiveBinDECAY_TuneZ2star_8TeV_madgraph_Summer12_53xOn52x";
      mySample.xSec = 245.8;
      mySample.nGen = 7610228;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
                 dirNames.push_back("/store/user/abrinke1/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/932e15c487207b473572def8d9167a18/");
                 dirNames.push_back("/store/user/abrinke1/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v2_BEAN_53xOn53x_V02_CV01/932e15c487207b473572def8d9167a18/");
        }
        else {
	         dirNames.push_back(skims_dir+"TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/");
	         dirNames.push_back(skims_dir+"TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/NovaBeanSkim_53Xon53X_TTJets_MassiveBinDECAY_prt1_20130119_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/NovaBeanSkim_53Xon53X_TTJets_MassiveBinDECAY_prt2_20130119_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsCC_MassiveBinDECAY_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8;
      mySample.nGen = 8252095;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
	  dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_MassiveBinDECAY_v1_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_MassiveBinDECAY_v1_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsCC_MassiveBinDECAY_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8;
      mySample.nGen = 6912438;
    }
    break;



  case 2555: // ttbar madgraph, tt+bb
    mySample.isTTJets = true;
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("/store/user/lannon/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S6_START52_V9-v1_BEAN_53xOn52x_V01_CV01/b9130084bab6b1e2e321806968edb2ef/");
      }
      else {
                dirNames.push_back("/uscms_data/d2/lannon/ttjetsSkim/");
      }
      mySample.sampleName = "TTJetsBB_MassiveBinDECAY_TuneZ2star_8TeV_madgraph_Summer12_53xOn52x";
      mySample.xSec = 245.8;
      mySample.nGen = 7610228;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
                dirNames.push_back("/store/user/abrinke1/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/932e15c487207b473572def8d9167a18/");
                dirNames.push_back("/store/user/abrinke1/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v2_BEAN_53xOn53x_V02_CV01/932e15c487207b473572def8d9167a18/");
        }
        else {
      	        dirNames.push_back(skims_dir+"TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/");
	        dirNames.push_back(skims_dir+"TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/NovaBeanSkim_53Xon53X_TTJets_MassiveBinDECAY_prt1_20130119_skimv0/03283e8eb3a7752fe70a1a708b319805/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/NovaBeanSkim_53Xon53X_TTJets_MassiveBinDECAY_prt2_20130119_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsBB_MassiveBinDECAY_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8;
      mySample.nGen = 8252095;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
	  dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_MassiveBinDECAY_v1_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_MassiveBinDECAY_v1_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsBB_MassiveBinDECAY_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8;
      mySample.nGen = 6912438;
    }
    break;



  case 2566: // ttbar madgraph, Hadronic decays. tt+lf
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTJets_HadronicMGDecays_8TeV-madgraph/TTJets_HadronicMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_BEAN_53xOn53x_V02_CV01/4f333e090d6ee11093982b8f6cf35a36/");
        }
        else {
        	dirNames.push_back(skims_dir+"TTJets_HadronicMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A_ext-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_TTJets_HadronicMGDecays_20130128_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsLF_HadronicMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8 * 0.457;
      mySample.nGen = 30997580;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsLF_HadronicMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8 * 0.457;
      mySample.nGen = 31111456;
    }
    break;


  case 2576: // ttbar madgraph, Hadronic decays. tt+cc
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTJets_HadronicMGDecays_8TeV-madgraph/TTJets_HadronicMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_BEAN_53xOn53x_V02_CV01/4f333e090d6ee11093982b8f6cf35a36/");
        }
        else {
        	dirNames.push_back(skims_dir+"TTJets_HadronicMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A_ext-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_TTJets_HadronicMGDecays_20130128_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsCC_HadronicMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8 * 0.457;
      mySample.nGen = 30997580;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsCC_HadronicMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8 * 0.457;
      mySample.nGen = 31111456;
    }
    break;

  case 2586: // ttbar madgraph, Hadronic decays. tt+bb
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTJets_HadronicMGDecays_8TeV-madgraph/TTJets_HadronicMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_BEAN_53xOn53x_V02_CV01/4f333e090d6ee11093982b8f6cf35a36/");
        }
        else {
        	dirNames.push_back(skims_dir+"TTJets_HadronicMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A_ext-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_TTJets_HadronicMGDecays_20130128_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsBB_HadronicMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8 * 0.457;
      mySample.nGen = 30997580;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsBB_HadronicMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8 * 0.457;
      mySample.nGen = 31111456;
    }
    break;


  case 2596: // ttbar madgraph, Hadronic decays. tt+b
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTJets_HadronicMGDecays_8TeV-madgraph/TTJets_HadronicMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_BEAN_53xOn53x_V02_CV01/4f333e090d6ee11093982b8f6cf35a36/");
        }
        else {
        	dirNames.push_back(skims_dir+"TTJets_HadronicMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A_ext-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_TTJets_HadronicMGDecays_20130128_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsB_HadronicMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8 * 0.457;
      mySample.nGen = 30997580;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsB_HadronicMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8 * 0.457;
      mySample.nGen = 31111456;
    }
    break;



  case 2563: // ttbar madgraph, Semi-leptonic decays. tt+lf
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTJets_SemiLeptMGDecays_8TeV-madgraph/TTJets_SemiLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_BEAN_53xOn53x_V02_CV01/430405049ac57d233e8b09dbf42a54d4/");
        	dirNames.push_back("/store/user/ahart/BN_TTJets_SemiLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_AODSIM_0/");
        }
        else {
        	dirNames.push_back(skims_dir+"TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A_ext-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_TTJets_SemiLeptMGDecay_20130119_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsLF_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8 * 0.438;
      mySample.nGen = 25165429;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsLF_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8 * 0.438;
      mySample.nGen = 25327478;
    }

    if( era == "2012_53x" && beanVer==3 ){
      if( siteOSU ){

      }
      else {
	//dirNames.push_back("/store/user/muell149/lobster_beans_v1/TTJets_semilep/");
        dirNames.push_back("/store/user/muell149/lobster_BNprod_v4/TTJets_semilep/");
      }
      mySample.sampleName = "TTJetsLF_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer3";
      mySample.xSec = 245.8 * 0.438;
      mySample.nGen = 25327478;
    }



    break;




  case 2573: // ttbar madgraph, Semi-leptonic decays. tt+cc
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTJets_SemiLeptMGDecays_8TeV-madgraph/TTJets_SemiLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_BEAN_53xOn53x_V02_CV01/430405049ac57d233e8b09dbf42a54d4/");
        }
        else {
        	dirNames.push_back(skims_dir+"TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A_ext-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_TTJets_SemiLeptMGDecay_20130119_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsCC_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8 * 0.438;
      mySample.nGen = 25165429;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsCC_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8 * 0.438;
      mySample.nGen = 25327478;
    }
    break;




  case 2583: // ttbar madgraph, Semi-leptonic decays. tt+bb
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
	  dirNames.push_back("/store/user/abrinke1/TTJets_SemiLeptMGDecays_8TeV-madgraph/TTJets_SemiLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_BEAN_53xOn53x_V02_CV01/430405049ac57d233e8b09dbf42a54d4/");
        }
        else {
	  dirNames.push_back(skims_dir+"TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A_ext-v1/");
        }
      }
      else {
	dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_TTJets_SemiLeptMGDecay_20130119_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsBB_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8 * 0.438;
      mySample.nGen = 25165429;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
	  dirNames.push_back("");
        }
        else {
	  dirNames.push_back(skims_dir+"TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
	dirNames.push_back(skims_dir+"TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsBB_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8 * 0.438;
      mySample.nGen = 25327478;
    }
    break;
    
  case 2593: // ttbar madgraph, Semi-leptonic decays. tt+b
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
	  dirNames.push_back("/store/user/abrinke1/TTJets_SemiLeptMGDecays_8TeV-madgraph/TTJets_SemiLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_BEAN_53xOn53x_V02_CV01/430405049ac57d233e8b09dbf42a54d4/");
        }
        else {
	  dirNames.push_back(skims_dir+"TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A_ext-v1/");
        }
      }
      else {
	dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_TTJets_SemiLeptMGDecay_20130119_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsB_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8 * 0.438;
      mySample.nGen = 25165429;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
	  dirNames.push_back("");
        }
        else {
	  dirNames.push_back(skims_dir+"TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
	dirNames.push_back(skims_dir+"TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsB_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8 * 0.438;
      mySample.nGen = 25327478;
    }
    break;
    
    

  case 2533: // ttbar madgraph, di-leptonic decays. tt+lf
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTJets_FullLeptMGDecays_8TeV-madgraph/TTJets_FullLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v2_BEAN_53xOn53x_V02_CV01/556b4c0b16db787b94fb594f5a52f9d8/");
        }
        else {
        	dirNames.push_back(skims_dir+"TTJets_FullLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_TTJets_FullLeptMGDecays_20130131_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsLF_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8 * 0.105;
      mySample.nGen = 12063533;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsLF_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8 * 0.105;
      mySample.nGen = 12100452;
    }
    break;

  case 25333: // ttbar madgraph, di-leptonic decays. tt+lf
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back("/store/user/biliu/BN_TTJets_FullLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM/");
        }
      }
      mySample.sampleName = "TTJetsLF_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2_andrewVer1";
      mySample.xSec = 245.8 * 0.105;
      mySample.nGen = 12100452;
    }
    break;

  case 2543: // ttbar madgraph, di-leptonic decays. tt+cc
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTJets_FullLeptMGDecays_8TeV-madgraph/TTJets_FullLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v2_BEAN_53xOn53x_V02_CV01/556b4c0b16db787b94fb594f5a52f9d8/");
        }
        else {
        	dirNames.push_back(skims_dir+"TTJets_FullLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_TTJets_FullLeptMGDecays_20130131_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsCC_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8 * 0.105;
      mySample.nGen = 12063533;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsCC_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8 * 0.105;
      mySample.nGen = 12100452;
    }
    break;

  case 2553: // ttbar madgraph, di-leptonic decays. tt+bb
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTJets_FullLeptMGDecays_8TeV-madgraph/TTJets_FullLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v2_BEAN_53xOn53x_V02_CV01/556b4c0b16db787b94fb594f5a52f9d8/");
        }
        else {
        	dirNames.push_back(skims_dir+"TTJets_FullLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_TTJets_FullLeptMGDecays_20130131_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsBB_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8 * 0.105;
      mySample.nGen = 12063533;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsBB_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8 * 0.105;
      mySample.nGen = 12100452;
    }
    break;


  case 2599: // ttbar madgraph, di-leptonic decays. tt+bb
    mySample.isTTJets = true;
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTJets_FullLeptMGDecays_8TeV-madgraph/TTJets_FullLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v2_BEAN_53xOn53x_V02_CV01/556b4c0b16db787b94fb594f5a52f9d8/");
        }
        else {
        	dirNames.push_back(skims_dir+"TTJets_FullLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_TTJets_FullLeptMGDecays_20130131_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsB_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 245.8 * 0.105;
      mySample.nGen = 12063533;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTJetsB_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 245.8 * 0.105;
      mySample.nGen = 12100452;
    }
    break;



  // single top
  case 2600: // single top, s-channel
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/t_schan/lannon/T_s-channel_TuneZ2star_8TeV-powheg-tauola/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_t_schan/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleTop_sChan_TuneZ2Star_8TeV_powhegTauola_Summer12_53Xon52X";
      mySample.xSec = 3.79;
      mySample.nGen = 259961;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/T_s-channel_TuneZ2star_8TeV-powheg-tauola/T_s-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/499aa0bfbd64686b70176cfe4efdb880/");
      	}
      	else {
      		dirNames.push_back(skims_dir+"T_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/");
      	}
      } // end if site is OSU
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/T_s-channel_TuneZ2star_8TeV-powheg-tauola/");
      }
      mySample.sampleName = "T_s_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x";
      mySample.xSec = 3.79;
      mySample.nGen = 259657;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
      	}
      	else {
                dirNames.push_back(skims_dir+"T_s-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleT_sChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      	}
      } // end if site is OSU
      else {
                dirNames.push_back(skims_dir+"T_s-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleT_sChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "T_s_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 3.79;
      mySample.nGen = 259657;
    }
    break;

    // single top
  case 2602: // single top, t-channel
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/t_tchan/lannon/T_t-channel_TuneZ2star_8TeV-powheg-tauola/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_t_tchan/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleTop_tChan_TuneZ2Star_8TeV_powhegTauola_Summer12_53Xon52X";
      mySample.xSec = 56.4;
      mySample.nGen = 23777;
      
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/T_t-channel_TuneZ2star_8TeV-powheg-tauola/T_t-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/f2521aecbef05b4aed24a710cd2ccce4/");
        }
        else {
        	dirNames.push_back(skims_dir+"T_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/T_t-channel_TuneZ2star_8TeV-powheg-tauola/");
      }
      mySample.sampleName = "T_t_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x";
      mySample.xSec = 56.4;
      mySample.nGen = 3754544;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"T_t-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleT_tChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"T_t-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleT_tChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "T_t_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 56.4;
      mySample.nGen = 3744404;
    }
    break;

    // single top
  case 2604: // single top, tW-channel
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/t_tW/lannon/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_t_tW/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleTop_tWChan_TuneZ2Star_8TeV_powhegTauola_Summer12_53Xon52X";
      mySample.xSec = 11.1;
      mySample.nGen = 497658;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/b108c013f7585a3aceeff3cf70d0fce5/");
      	}
      	else {
      		dirNames.push_back(skims_dir+"T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/");
      	}
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/");
      }
      mySample.sampleName = "T_tW_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x";
      mySample.xSec = 11.1;
      mySample.nGen = 496918;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
      	}
      	else {
                dirNames.push_back(skims_dir+"T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleT_tWChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      	}
      }
      else {
                dirNames.push_back(skims_dir+"T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleT_tWChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "T_tW_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 11.1;
      mySample.nGen = 496918;
    }
    break;

    // single top-bar
  case 2601: // single top-bar, s-channel
    if( era == "2012_52x" ){
      if( siteOSU ){
                 dirNames.push_back("");
      }
      else {
                 dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/tbar_schan/lannon/Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_tbar_schan/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleTopBar_sChan_TuneZ2Star_8TeV_powhegTauola_Summer12_53Xon52X";
      mySample.xSec = 1.76;
      mySample.nGen = 139974;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/e02415c12306a066361f1f54744e5323/");
      	}
      	else {
      		dirNames.push_back(skims_dir+"Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/");
      	}
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/");
      }
      mySample.sampleName = "Tbar_s_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x";
      mySample.xSec = 1.76;
      mySample.nGen = 139835;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
      	}
      	else {
                dirNames.push_back(skims_dir+"Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleTbar_sChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      	}
      }
      else {
                dirNames.push_back(skims_dir+"Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleTbar_sChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "Tbar_s_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 1.76;
      mySample.nGen = 139835;
    }
    break;

    // single top
  case 2603: // single top-bar, t-channel
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/tbar_tchan/lannon/Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_tbar_tchan/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleTopBar_tChan_TuneZ2Star_8TeV_powhegTauola_Summer12_53Xon52X";
      mySample.xSec = 30.7;
      mySample.nGen = 1935072;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/d36896beaa1e8d9b3a0cbab8c9c2a093/");
        }
        else {
        	dirNames.push_back(skims_dir+"Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/");
      }
      mySample.sampleName = "Tbar_t_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x";
      mySample.xSec = 30.7;
      mySample.nGen = 708980;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleTbar_tChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleTbar_tChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "Tbar_t_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 30.7;
      mySample.nGen = 1933504;
    }
    break;

    // single top
  case 2605: // single top-bar, tW-channel
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/tbar_tW/lannon/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_tbar_tW/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "SingleTopBar_tWChan_TuneZ2Star_8TeV_powhegTauola_Summer12_53Xon52X";
      mySample.xSec = 11.1;
      mySample.nGen = 493460;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
        if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/f976b2e4d0d93e9904e27c9b4d45be89/");
        }
        else {
        	dirNames.push_back(skims_dir+"Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/");
      }
      mySample.sampleName = "Tbar_tW_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x";
      mySample.xSec = 11.1;
      mySample.nGen = 496918;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
        if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleTbar_tWChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleTbar_tWChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "Tbar_tW_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 11.1;
      mySample.nGen = 492779;
    }
    break;


  case 2400: // WJets madgraph
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/wjets/lannon/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_wjets/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "WJetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn52x";
      mySample.xSec = 36257.2;
      mySample.nGen = 18393090;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v2_BEAN_53xOn53x_V02_CV01/1cbefb07e8d3ed406b388c95adf40132/");
        }
        else {
        	dirNames.push_back(skims_dir+"WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/NovaBeanSkim_53Xon53X_WJetsToLNu_20130128_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "WJetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 36257.2;
      mySample.nGen = 57108525;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/NovaBeanSkim_53Xon53X_newBEANs_201304_WJets_InclDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/NovaBeanSkim_53Xon53X_newBEANs_201304_WJets_InclDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "WJetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 36257.2;
      mySample.nGen = 57536319;
    }
    break;

    // W + 1 jets
  case 2401: // WJets madgraph
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/W1JetsToLNu_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/8197ef28f50f6b17ee62764b842df673/");
        }
        else {
        	dirNames.push_back(skims_dir+"W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_W1JetsToLNu_20130128_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "W1JetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 6440.4;
      mySample.nGen = 23136036;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"W1JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W1Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"W1JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W1Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "W1JetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 6440.4;
      mySample.nGen = 23134881;
    }
    break;

    // W + 2 jets
  case 2402: // WJets madgraph
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/W2JetsToLNu_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/45c9015af9e5c1a782fd2556b3df8e17/");
        }
        else {
        	dirNames.push_back(skims_dir+"W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }	
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_W2JetsToLNu_20130128_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "W2JetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 2087.2;
      mySample.nGen = 34030565;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"W2JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W2Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }	
      }
      else {
                dirNames.push_back(skims_dir+"W2JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W2Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "W2JetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 2087.2;
      mySample.nGen = 33933328;
    }
    break;

    // W + 3 jets
  case 2403: // WJets madgraph
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/W3JetsToLNu_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/a0b335bfb69fef3100e6f28834f7f3d3/");
        }
        else {
        	dirNames.push_back(skims_dir+"W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_W3JetsToLNu_20130124_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "W3JetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 619.0;
      mySample.nGen = 15472440;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"W3JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W3Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"W3JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W3Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "W3JetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 619.0;
      mySample.nGen = 15463420;
    }
    break;

    // W + 4 jets
  case 2404: // WJets madgraph
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/W4JetsToLNu_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/6f53779771d522e974732a1f5a4d3ba3/");
        }
        else {
        	dirNames.push_back(skims_dir+"W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }	
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_W4JetsToLNu_20130124_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "W4JetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 255.2;
      mySample.nGen = 13360967;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"W4JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W4Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }	
      }
      else {
                dirNames.push_back(skims_dir+"W4JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W4Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "W4JetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 255.2;
      mySample.nGen = 13365439;
    }
    break;


    // DY -> LL
  case 2800: // all L
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/zjets/lannon/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_zjets/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "DYJetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn52x";
      mySample.xSec = 3503.71;
      mySample.nGen = 30461028;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/894bf83260076a22df0c97ce24c6bb58/");
        }
        else {
        	dirNames.push_back(skims_dir+"DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
	        dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/");
      }
      mySample.sampleName = "DYJetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 3503.71;
      mySample.nGen = 30452141;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
	        dirNames.push_back(skims_dir+"DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/NovaBeanSkim_53Xon53X_newBEANs_201304_DYJets_InclDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
	        dirNames.push_back(skims_dir+"DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/NovaBeanSkim_53Xon53X_newBEANs_201304_DYJets_InclDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "DYJetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 3503.71;
      mySample.nGen = 30072710;
    }
    break;

    // DY -> LL low mass
  case 2850: // low mass
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/lowmassDY/lannon/DYJetsToLL_M-10To50filter_8TeV-madgraph/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_lowmassDY/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "DYJetsToLL_TuneZ2Star_8TeV_madgraph_Summer12_53Xon52X";
      mySample.xSec = 3503.71;
      mySample.nGen = 7132223;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/DYJetsToLL_M-10To50_TuneZ2Star_8TeV-madgraph/DYJetsToLL_M-10To50_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/1f44d834803c8bd3f79988488b8c19b7/");
        	dirNames.push_back("/store/user/abrinke1/DYJetsToLL_M-10To50filter_8TeV-madgraph/DYJetsToLL_M-10To50filter_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/1f44d834803c8bd3f79988488b8c19b7/");
        }
        else {
        	dirNames.push_back(skims_dir+"DYJetsToLL_M-10To50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        	dirNames.push_back(skims_dir+"DYJetsToLL_M-10To50filter_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/DYJetsToLL_M-10To50_TuneZ2Star_8TeV-madgraph/");
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/DYJetsToLL_M-10To50filter_8TeV-madgraph/");
      }
      mySample.sampleName = "DYJetsToLL_M10To50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 14702;
      mySample.nGen = 32600176+7131033;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"DYJetsToLL_M-10To50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DYJets_M10to50_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"DYJetsToLL_M-10To50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DYJets_M10to50_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "DYJetsToLL_M10To50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 14702;
      mySample.nGen = 37828841;
    }
    break;

    // DY + 1 jet
  case 2801: // all L
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/517863706e05a46586b2494240db23f1/");
        }
        else {
        	dirNames.push_back(skims_dir+"DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/");
      }
      mySample.sampleName = "DY1JetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 666.7;
      mySample.nGen = 23698301;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY1Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY1Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "DY1JetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 666.7;
      mySample.nGen = 24032562;
    }
    break;

    // DY + 2 jet
  case 2802: // all L
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/288537beba45142e2476f91ab2c9cf9d/");
        }
        else {
        	dirNames.push_back(skims_dir+"DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/");
      }
      mySample.sampleName = "DY2JetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 215.1;
      mySample.nGen = 2350806;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY2Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY2Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "DY2JetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 215.1;
      mySample.nGen = 2350806;
    }
    break;

    // DY + 3 jet
  case 2803: // all L
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/0734c5e18046a5c3db43587f9b4864b3/");
        }
        else {
        	dirNames.push_back(skims_dir+"DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/");
      }
      mySample.sampleName = "DY3JetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 66.07;
      mySample.nGen = 11004806;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY3Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY3Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "DY3JetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 66.07;
      mySample.nGen = 10753491;
    }
    break;

    // DY + 4 jet
  case 2804: // all L
    if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/4b958bd5cb6e234d5a6e26e39222764d/");
        }
        else {
        	dirNames.push_back(skims_dir+"DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/");
      }
      mySample.sampleName = "DY4JetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 27.38;
      mySample.nGen = 6387161;
    }
    if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY4Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY4Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "DY4JetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 27.38;
      mySample.nGen = 6370630;
    }
    break;




    // ttbarZ
  case 2523:
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/ttZ/lannon/TTZJets_8TeV-madgraph_v2/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_ttZ/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTZJets_TuneZ2Star_8TeV_madgraph_Summer12_53Xon52X";
      mySample.xSec =  0.208;
      mySample.nGen =  209741;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTZJets_8TeV-madgraph_v2/TTZJets_8TeV-madgraph_v2_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/1aa460be05319c2fb88db2e3427420c5/");
        }
        else {
        	dirNames.push_back(skims_dir+"TTZJets_8TeV-madgraph_v2/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTZJets_8TeV-madgraph_v2/");
      }
      mySample.sampleName = "TTZJets_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 0.2057;
      mySample.nGen = 209512;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTZJets_8TeV-madgraph_v2/NovaBeanSkim_53Xon53X_newBEANs_201304_TTZ_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTZJets_8TeV-madgraph_v2/NovaBeanSkim_53Xon53X_newBEANs_201304_TTZ_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTZJets_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 0.2057;
      mySample.nGen = 209512;
    }
    break;

    // ttbarW
  case 2524:
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/ttW/lannon/TTWJets_8TeV-madgraph/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_ttW/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTWJets_TuneZ2Star_8TeV_madgraph_Summer12_53Xon52X";
      mySample.xSec = 0.163*0.208/0.136;
      mySample.nGen = 195301;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/TTWJets_8TeV-madgraph/TTWJets_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/0e73eeb8f872a99f3fbc627fdcc1783e/");
        }
        else {
        	dirNames.push_back(skims_dir+"TTWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/TTWJets_8TeV-madgraph/");
      }
      mySample.sampleName = "TTWJets_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x";
      mySample.xSec = 0.232;
      mySample.nGen = 195396;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"TTWJets_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTW_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"TTWJets_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTW_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "TTWJets_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 0.232;
      mySample.nGen = 195396;
    }
    break;

    // WW
  case 2700: // all L
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/WW/lannon/WW_TuneZ2star_8TeV_pythia6_tauola/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_WW/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "WW_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53Xon52X";
      mySample.xSec = 57.1097;
      mySample.nGen = 10000431;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/WW_TuneZ2star_8TeV_pythia6_tauola/WW_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/251a4d0e63256a9f271413144b01196f/");
        }
        else {
        	dirNames.push_back(skims_dir+"WW_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/WW_TuneZ2star_8TeV_pythia6_tauola/");
      }
      mySample.sampleName = "WW_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53xOn53x";
      mySample.xSec = 57.1097;
      mySample.nGen = 8965049;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"WW_TuneZ2star_8TeV_pythia6_tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_WW_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"WW_TuneZ2star_8TeV_pythia6_tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_WW_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "WW_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 57.1097;
      mySample.nGen = 9955089;
    }
    break;

    // WZ
  case 2701: // all L
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/WZ/lannon/WZ_TuneZ2star_8TeV_pythia6_tauola/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_WZ/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "WZ_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53Xon52X";
      mySample.xSec = 32.3161;
      mySample.nGen = 9996622;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/WZ_TuneZ2star_8TeV_pythia6_tauola/WZ_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/135f1adb4c7c7bb66cae0bd597c8c682/");
        }
        else {
        	dirNames.push_back(skims_dir+"WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/WZ_TuneZ2star_8TeV_pythia6_tauola/");
      }
      mySample.sampleName = "WZ_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53xOn53x";
      mySample.xSec = 32.3161;
      mySample.nGen = 9821291;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"WZ_TuneZ2star_8TeV_pythia6_tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_WZ_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"WZ_TuneZ2star_8TeV_pythia6_tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_WZ_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "WZ_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 32.3161;
      mySample.nGen = 9931257;
    }
    break;

    // ZZ
  case 2702: // all L
    if( era == "2012_52x" ){
      if( siteOSU ){
                dirNames.push_back("");
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/ZZ/lannon/ZZ_TuneZ2star_8TeV_pythia6_tauola/NOVa_SKIM_3Jet_1Tag_1Lep_notrig_v1_ZZ/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "ZZ_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53Xon52X";
      mySample.xSec = 8.25561;
      mySample.nGen = 9799908;
    }
    else if( era == "2012_53x" && beanVer==1 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("/store/user/abrinke1/ZZ_TuneZ2star_8TeV_pythia6_tauola/ZZ_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_BEAN_53xOn53x_V02_CV01/d4ce4bce92962f7c5d6189c7c841b6d0/");
        }
        else {
        	dirNames.push_back(skims_dir+"ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/");
        }
      }
      else {
                dirNames.push_back("dcap:////pnfs/cms/WAX/11/store/user/tth/53Xon53X_Skims/jgwood/ZZ_TuneZ2star_8TeV_pythia6_tauola/");
      }
      mySample.sampleName = "ZZ_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53xOn53x";
      mySample.xSec = 8.25561;
      mySample.nGen = 9568511;
    }
    else if( era == "2012_53x" && beanVer==2 ){
      if( siteOSU ){
      	if (!isSkim){
        	dirNames.push_back("");
        }
        else {
                dirNames.push_back(skims_dir+"ZZ_TuneZ2star_8TeV_pythia6_tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_ZZ_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
        }
      }
      else {
                dirNames.push_back(skims_dir+"ZZ_TuneZ2star_8TeV_pythia6_tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_ZZ_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/");
      }
      mySample.sampleName = "ZZ_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53xOn53x_beanVer2";
      mySample.xSec = 8.25561;
      mySample.nGen = 9755621;
    }
    break;

  } // end switch sample
   



  //
  // Loop over Directories, add files to vector
  //

  std::cout <<"\t\t number of directories = "<< dirNames.size() << std::endl;
  for(int f=0; f<int(dirNames.size()); f++ ){
    TString thisDir = TString(dirNames[f]);
    const char *file;
    std::string file_str;
    int numfiles = 0;
    void *dir = gSystem->OpenDirectory(gSystem->ExpandPathName(thisDir.Data()));
    if (dir) {
      while ((file = gSystem->GetDirEntry(dir))) { // TEMP SBOUTLE
	//while ((file = gSystem->GetDirEntry(dir)) && numfiles<4) {
        file_str = dirNames[f] + string(file);
        if( (file_str.find(".root")!=std::string::npos) ) fileNames.push_back(file_str);
	numfiles++;
      }
    }
  }

  mySample.fileNames = fileNames;



  //
  // Set job numbers and strings 
  //
  char jobBuffer[50];
  sprintf( jobBuffer, "%d", Njobs);
  mySample.str_Njobs = jobBuffer;
  sprintf( jobBuffer, "job%d", jobN);
  mySample.str_jobN  = jobBuffer;
 

  // 
  // Set output file name and path
  //
  mySample.treeFileName  = mySample.outputDir;
  mySample.treeFileName += "yggdrasil_treeMaker_";
  mySample.treeFileName += mySample.str_sampleType;
  mySample.treeFileName += "_";
  mySample.treeFileName += mySample.sampleName;
  //mySample.treeFileName += "_";
  //mySample.treeFileName += mySample.str_leptonSelection; 
  mySample.treeFileName += "_"; 
  mySample.treeFileName += mySample.str_jobN + "of" + mySample.str_Njobs;
  mySample.treeFileName += ".root";



  return mySample;

}

#endif // INC_BEANLOADER
