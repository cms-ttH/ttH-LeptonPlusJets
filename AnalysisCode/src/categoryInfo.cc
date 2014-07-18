#include "../interface/categoryInfo.h"

//////////////////////////
//
// Category Info Functions
//
//////////////////////////


// Constructor
categoryInfo::categoryInfo(){

  id            = -99;
  treeCut       = "";
  mvaTreeCut    = "";
  histName      = "";
  title         = "";
  label         = "";
  latex         = "";
  nJets         = 0;
  nExtraPartons = 0;

  return;
}


// Destructor
categoryInfo::~categoryInfo(){

 
  return;
}
