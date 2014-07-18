#ifndef ttH_LeptonPlusJets_AnalysisCode_categoryInfo_h
#define ttH_LeptonPlusJets_AnalysisCode_cateogryInfo_h


#include "TROOT.h"
#include "Riostream.h"
#include "TString.h"

#include <vector>


/////////////////////
//
// Class Definitions
//
/////////////////////

//
// Base class containing information for each category
//

class categoryInfo{

 public:

  int     id;
  TString treeCut;
  TString mvaTreeCut;
  TString histName;
  TString title;
  TString label;
  TString latex;
  int     nJets;
  int     nExtraPartons;
    
  categoryInfo();
  virtual ~categoryInfo();

}; // End of Class Prototype

typedef vector<categoryInfo> vcategoryInfo;

#endif // _categoryInfo_h


  



