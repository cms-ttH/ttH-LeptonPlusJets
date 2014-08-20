//Root includes                                   
#include "TROOT.h"
#include "Riostream.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TH1F.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TTree.h"
#include "TString.h"
#include "TMath.h"
#include "TAxis.h"
#include "TKey.h"
#include "TList.h"

void drop_histograms(TString inputFileName = "ttH_ljets_7TeV_2012_06_06.root",
		     TString outputFileName = "ttH_ljets_scale_7TeV_2012_06_06.root") {


  TFile *outFile = TFile::Open(outputFileName,"RECREATE");

  TFile *histFile = TFile::Open(inputFileName);

  std::cout << "Copying over all histograms..." << std::endl;

  //Now copy all other histograms in the file as is (preserving names...)
  TList *keys = histFile->GetListOfKeys();

  TIter nextKey(keys);
  TKey *key = 0;

  while ((key = (TKey *)nextKey())) {

    TString name = key->GetName();

    if( name.BeginsWith("h_") ) continue;
    if( !name.Contains("final") ) continue;
    if( name.Contains("final_CUp") ) continue;
    if( name.Contains("final_CDown") ) continue;

    TH1 *hist = 0;
    hist = (TH1 *)histFile->Get(name);

    outFile->cd();
    hist->Write(name);
  }

  outFile->Close();

  std::cout << "Done." << std::endl;

}
