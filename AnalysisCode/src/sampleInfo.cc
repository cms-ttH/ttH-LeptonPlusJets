#include "TROOT.h"
#include "TSystem.h"
#include "Riostream.h"
#include "TString.h"

#include <string>

#include "../interface/sampleInfo.h"

//////////////////////////
//
// Sample Info Functions
//
//////////////////////////


//
// sampleInfo
//   Constructor, void return
//
sampleInfo::sampleInfo(){

  id      = 0;
  name    = "";
  skimPath.clear();
  skimFiles.clear();
  label   = "";
  latex   = "";
  xsec    = 1.0;
  ngen    = 1.0;
  qcd_unc = 1.0;
  pdf_unc = 1.0;
  
  return;
}


//
// ~sampleInfo()
//   Destructor, void return
//
sampleInfo::~sampleInfo(){

  return;
}


//
// getSkimFiles
//   Get Skim Files from a Path, returns number of files if successfull
//
int sampleInfo::getSkimFiles(){

  // Input Sanitation
  if( (int)skimPath.size()==0 ){
    cout << "  NO DIRECTORIES SET FOR THIS SAMPLE... " << endl;
    return -1;
  }
  
  // Print Number of Directories
  //cout << "  Number of directories = "<< skimPath.size() << endl;

  // Loop over Directories
  int nFiles=0;
  for(int iDir=0; iDir<(int)skimPath.size(); iDir++ ){

    TString thisDir = skimPath[iDir];
    const char *file;
    std::string file_str;
    
    // Expand Wildcards
    void *dir=gSystem->OpenDirectory(gSystem->ExpandPathName(thisDir.Data()));

    // Check that directory pointer is good
    if(dir){

      // Loop over files in this directory
      while( (file = gSystem->GetDirEntry(dir)) ){

        file_str = skimPath[iDir] + file;

        // Check that object found is a root file
	if( (file_str.find(".root")!=std::string::npos) ) skimFiles.push_back(file_str);
	nFiles++;

      } // end while loop over files in this directory
    } // end if directory good
  } // end loop over directories


  return nFiles;
}



  
