#ifndef ttH_LeptonPlusJets_AnalysisCode_systematicInfo_h
#define ttH_LeptonPlusJets_AnalysisCode_systematicInfo_h

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
// Base class containing information for each systematic
//

class systematicInfo{

 public:

  int     id;
  int     treeIndexUp;
  int     treeIndexDown;
  TString type;
  TString title;
  TString label;
  TString latex;
  
  double  rate_ttH;
  double  rate_ttbar;
  double  rate_ttbarPlusB;
  double  rate_ttbarPlusBBbar;
  double  rate_ttbarPlusCCbar;
  double  rate_singlet;
  double  rate_wjets;
  double  rate_zjets;
  double  rate_ttbarW;
  double  rate_ttbarZ;
  double  rate_diboson;
    
  systematicInfo();
  virtual ~systematicInfo();

}; // End of Class Prototype

typedef vector<systematicInfo> vsystematicInfo;


#endif // _systematicInfo_h



  


