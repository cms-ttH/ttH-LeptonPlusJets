#ifndef ttH_LeptonPlusJets_AnalysisCode_sampleInfo_h
#define ttH_LeptonPlusJets_AnalysisCode_sampleInfo_h

#include "TROOT.h"
#include "Riostream.h"
#include "TString.h"

#include <vector>
#include <string>

/////////////////////
//
// Class Definitions
//
/////////////////////

//
// Base class containing information for each sample
//

class sampleInfo{

 public:

  int                 id;
  TString             name;
  vector<std::string> skimPath;
  vector<std::string> skimFiles;
  TString             label;
  TString             latex;
  double              xsec;
  double              ngen;
  double              qcd_unc;
  double              pdf_unc;
  

  sampleInfo();
  virtual ~sampleInfo();
  int  getSkimFiles();

}; // End Class Prototype

typedef vector<sampleInfo> vsampleInfo;


#endif // _sampleInfo_h



  



