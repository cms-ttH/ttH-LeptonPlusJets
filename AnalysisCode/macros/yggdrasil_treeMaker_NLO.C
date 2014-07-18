#include "TROOT.h"
#include "Riostream.h"


#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "../interface/yggdrasil_NLO.h"

#endif

//*****************************************************************************


void yggdrasil_treeMaker_NLO(int sampleNum, int maxNentries=-1, int Njobs=1, int jobN=1){


  // Declare Analysis Object
  yggdrasil_NLO analyzer;

  // Run Analysis
  analyzer.makeTree(sampleNum, maxNentries, Njobs, jobN);
  
  // Clean Up

  return;

}

