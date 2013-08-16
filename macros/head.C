{

  TString makeshared(gSystem->GetMakeSharedLib());
  TString dummy = makeshared.ReplaceAll("-W ", "");
  gSystem->SetMakeSharedLib(makeshared);
  //TString dummy = makeshared.ReplaceAll("-Wshadow ", "");
  TString dummy = makeshared.ReplaceAll("-Wshadow ", "-std=c++0x ");
  gSystem->SetMakeSharedLib(makeshared);
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();

  gSystem->Load("libDataFormatsFWLite.so");
  gSystem->Load("libDataFormatsCommon.so");
  gSystem->Load("libNtupleMakerBEANmaker.so");
  gSystem->Load("libAnalysisCodeLeptonPlusJets.so");

  TString path = gSystem->GetIncludePath();
  path.Append(" -I$ROOTSYS/include/root -I./include  ");
  path.Append(" -I$CMSSW_BASE/src/NtupleMaker/BEANmaker/interface/ ");
  gSystem->SetIncludePath(path);


  gROOT->SetStyle("Plain");
  gROOT->ForceStyle();

  gStyle->SetPalette(1);

}
