#include "../interface/systematicInfo.h"

//////////////////////////
//
// Systematic Info Functions
//
//////////////////////////


// Constructor
systematicInfo::systematicInfo(){

  id                  = 0;
  treeIndexUp         = 0;
  treeIndexDown       = 0;
  type                = "";
  title               = "";
  label               = "";
  latex               = "";
  
  rate_ttH            = 0.0;
  rate_ttbar          = 0.0;
  rate_ttbarPlusB     = 0.0;
  rate_ttbarPlusBBbar = 0.0;
  rate_ttbarPlusCCbar = 0.0;
  rate_singlet        = 0.0;
  rate_wjets          = 0.0;
  rate_zjets          = 0.0;
  rate_ttbarW         = 0.0;
  rate_ttbarZ         = 0.0;
  rate_diboson        = 0.0;

  return;
}


// Destructor
systematicInfo::~systematicInfo(){
 
  return;
}

