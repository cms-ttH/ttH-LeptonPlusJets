#include "../interface/analysisInfo_CMS_ttH_LJ.h"


//////////////////////////
//
// Analysis Info Functions
//
//////////////////////////


// Constructor
analysisInfo_CMS_ttH_LJ::analysisInfo_CMS_ttH_LJ(){


  //
  // Intialize containers
  //
  categoryList_.clear();
  sampleList_trees_.clear();
  sampleList_histos_.clear();
  sampleList_plots_.clear();
  sampleList_limits_.clear();
  systematicList_.clear();


  //
  // Initialize Categories
  //
  
  // Lepton+Jets, ==4Jets, ==2Tags
  ljets_ee4j_ee2t_.id            = 0;
  ljets_ee4j_ee2t_.treeCut       = "( eve.numJets_float_[iSys]==4 && eve.numTags_float_[iSys]==2 )";
  ljets_ee4j_ee2t_.mvaTreeCut    = "( numJets==4 && numTags==2 )";
  ljets_ee4j_ee2t_.histName      = "4j2t";
  ljets_ee4j_ee2t_.title         = "Lepton, 4 Jets, 2 b-tags";
  ljets_ee4j_ee2t_.label         = "ljets_j4_t2";
  ljets_ee4j_ee2t_.latex         = "4 jets + 2 b-tags";
  ljets_ee4j_ee2t_.nJets         = 4;
  ljets_ee4j_ee2t_.nExtraPartons = 0;
  

  // Lepton+Jets, ==5Jets, ==2Tags
  ljets_ee5j_ee2t_.id            = 1;
  ljets_ee5j_ee2t_.treeCut       = "( eve.numJets_float_[iSys]==5 && eve.numTags_float_[iSys]==2 )";
  ljets_ee5j_ee2t_.mvaTreeCut    = "( numJets==5 && numTags==2 )";
  ljets_ee5j_ee2t_.histName      = "5j2t";
  ljets_ee5j_ee2t_.title         = "Lepton, 5 Jets, 2 b-tags";
  ljets_ee5j_ee2t_.label         = "ljets_j5_t2";
  ljets_ee5j_ee2t_.latex         = "5 jets + 2 b-tags";
  ljets_ee5j_ee2t_.nJets         = 5;
  ljets_ee5j_ee2t_.nExtraPartons = 1;


  // Lepton+Jets, >=6Jets, ==2Tags
  ljets_ge6j_ee2t_.id            = 2;
  ljets_ge6j_ee2t_.treeCut       = "( eve.numJets_float_[iSys]>=6 && eve.numTags_float_[iSys]==2 )";
  ljets_ge6j_ee2t_.mvaTreeCut    = "( numJets>=6 && numTags==2 )";
  ljets_ge6j_ee2t_.histName      = "6j2t";
  ljets_ge6j_ee2t_.title         = "Lepton, #geq6 Jets, 2 b-tags";
  ljets_ge6j_ee2t_.label         = "ljets_jge6_t2";
  ljets_ge6j_ee2t_.latex         = "$\\ge$6 jets + 2 b-tags";
  ljets_ge6j_ee2t_.nJets         = 6;
  ljets_ge6j_ee2t_.nExtraPartons = 2;


  // Lepton+Jets, ==4Jets, ==3Tags
  ljets_ee4j_ee3t_.id            = 3;
  ljets_ee4j_ee3t_.treeCut       = "( eve.numJets_float_[iSys]==4 && eve.numTags_float_[iSys]==3 )";
  ljets_ee4j_ee3t_.mvaTreeCut    = "( numJets==4 && numTags==3 )";
  ljets_ee4j_ee3t_.histName      = "4j3t";
  ljets_ee4j_ee3t_.title         = "Lepton, 4 Jets, 3 b-tags";
  ljets_ee4j_ee3t_.label         = "ljets_j4_t3";
  ljets_ee4j_ee3t_.latex         = "4 jets + 3 b-tags";
  ljets_ee4j_ee3t_.nJets         = 4;
  ljets_ee4j_ee3t_.nExtraPartons = 0;

  
  // Lepton+Jets, ==5Jets, ==3Tags
  ljets_ee5j_ee3t_.id            = 4;
  ljets_ee5j_ee3t_.treeCut       = "( eve.numJets_float_[iSys]==5 && eve.numTags_float_[iSys]==3 )";
  ljets_ee5j_ee3t_.mvaTreeCut    = "( numJets==5 && numTags==3 )";
  ljets_ee5j_ee3t_.histName      = "5j3t";
  ljets_ee5j_ee3t_.title         = "Lepton, 5 Jets, 3 b-tags";
  ljets_ee5j_ee3t_.label         = "ljets_j5_t3";
  ljets_ee5j_ee3t_.latex         = "5 jets + 3 b-tags";
  ljets_ee5j_ee3t_.nJets         = 5;
  ljets_ee5j_ee3t_.nExtraPartons = 1;


  // Lepton+Jets, >=6Jets, ==3Tags
  ljets_ge6j_ee3t_.id            = 5;
  ljets_ge6j_ee3t_.treeCut       = "( eve.numJets_float_[iSys]>=6 && eve.numTags_float_[iSys]==3 )";
  ljets_ge6j_ee3t_.mvaTreeCut    = "( numJets>=6 && numTags==3 )";
  ljets_ge6j_ee3t_.histName      = "6j3t";
  ljets_ge6j_ee3t_.title         = "Lepton, #geq6 Jets, 3 b-tags";
  ljets_ge6j_ee3t_.label         = "ljets_jge6_t3";
  ljets_ge6j_ee3t_.latex         = "$\\ge$6 jets + 3 b-tags";
  ljets_ge6j_ee3t_.nJets         = 6;
  ljets_ge6j_ee3t_.nExtraPartons = 2;


  // Lepton+Jets, ==4Jets, ==4Tags
  ljets_ee4j_ee4t_.id            = 6;
  ljets_ee4j_ee4t_.treeCut       = "( eve.numJets_float_[iSys]==4 && eve.numTags_float_[iSys]==4 )";
  ljets_ee4j_ee4t_.mvaTreeCut    = "( numJets==4 && numTags==4 )";
  ljets_ee4j_ee4t_.histName      = "4j4t";
  ljets_ee4j_ee4t_.title         = "Lepton, 4 Jets, 4 b-tags";
  ljets_ee4j_ee4t_.label         = "ljets_j4_t4";
  ljets_ee4j_ee4t_.latex         = "4 jets + 4 b-tags";
  ljets_ee4j_ee4t_.nJets         = 4;
  ljets_ee4j_ee4t_.nExtraPartons = 0;

  
  // Lepton+Jets, ==5Jets, ==4Tags
  ljets_ee5j_ge4t_.id            = 7;
  ljets_ee5j_ge4t_.treeCut       = "( eve.numJets_float_[iSys]==5 && eve.numTags_float_[iSys]>=4 )";
  ljets_ee5j_ge4t_.mvaTreeCut    = "( numJets==5 && numTags>=4 )";
  ljets_ee5j_ge4t_.histName      = "5j4t";
  ljets_ee5j_ge4t_.title         = "Lepton, 5 Jets, #geq4 b-tags";
  ljets_ee5j_ge4t_.label         = "ljets_j5_tge4";
  ljets_ee5j_ge4t_.latex         = "5 jets + $\\ge$4 b-tags";
  ljets_ee5j_ge4t_.nJets         = 5;
  ljets_ee5j_ge4t_.nExtraPartons = 1;


  // Lepton+Jets, >=6Jets, ==4Tags
  ljets_ge6j_ge4t_.id            = 8;
  ljets_ge6j_ge4t_.treeCut       = "( eve.numJets_float_[iSys]>=6 && eve.numTags_float_[iSys]>=4 )";
  ljets_ge6j_ge4t_.mvaTreeCut    = "( numJets>=6 && numTags>=4 )";
  ljets_ge6j_ge4t_.histName      = "6j4t";
  ljets_ge6j_ge4t_.title         = "Lepton, #geq6 Jets, #geq4 b-tags";
  ljets_ge6j_ge4t_.label         = "ljets_jge6_tge4";
  ljets_ge6j_ge4t_.latex         = "$\\ge$6 jets + $\\ge$4 b-tags";
  ljets_ge6j_ge4t_.nJets         = 6;
  ljets_ge6j_ge4t_.nExtraPartons = 2;


  //
  // Fill Category List Container
  //
  categoryList_.push_back(ljets_ee4j_ee2t_);
  categoryList_.push_back(ljets_ee5j_ee2t_);
  categoryList_.push_back(ljets_ge6j_ee2t_);
  categoryList_.push_back(ljets_ee4j_ee3t_);
  categoryList_.push_back(ljets_ee5j_ee3t_);
  categoryList_.push_back(ljets_ge6j_ee3t_);
  categoryList_.push_back(ljets_ee4j_ee4t_);
  categoryList_.push_back(ljets_ee5j_ge4t_);
  categoryList_.push_back(ljets_ge6j_ge4t_);


  //
  // Initialize Samples
  //
  
  path_skim_  = "/eos/uscms/store/user/tth/53Xon53X_newBEANs_201304_Skims/fromDCache_20140318/jgwood/";

  path_tree_  = "/uscms_data/d3/jgwood/ttH_ttbb_lepJets/2012_analysis/anaTrees_2012_53x_beanVer2_kinFit_lite__CMS_jetPt_30__lepPt_30__20140429_153748/";

  path_histo_ = path_tree_ + "Histos/";
 

  path_skim_nlo_  = "/eos/uscms/store/user/tth/NLO_MC_20140610/";

  path_tree_nlo_  = "/uscms_data/d3/jgwood/ttH_ttbb_lepJets/2012_analysis/anaTrees_2012_53x_NLOMC_20140701/";

  path_histo_nlo_ = path_tree_nlo_ + "Histos/";



  // TTJets Incl NLO 4FS+0Jet Decays
  ttjets_NLO_fxfx4fs0j_.id       = 2520;
  ttjets_NLO_fxfx4fs0j_.name     = "TTJetsIncl_NLO_FxFx_4FS_tt_0Jet_max1Jet_8TeV_CTEQ6M_MadSpin";
  ttjets_NLO_fxfx4fs0j_.skimPath.push_back( path_skim_nlo_ + "FxFx_4FS_tt_0Jet_max1Jet_8TeV_CTEQ6M_MadSpin/" );
  ttjets_NLO_fxfx4fs0j_.getSkimFiles();
  ttjets_NLO_fxfx4fs0j_.label    = "ttbar_nlo_4fs_0jet";
  ttjets_NLO_fxfx4fs0j_.latex    = "t\\bar{t}+jets, NLO, 4FS+0Jet Madspin decays";
  ttjets_NLO_fxfx4fs0j_.xsec     = 245.8;
  ttjets_NLO_fxfx4fs0j_.ngen     = 6988;
  ttjets_NLO_fxfx4fs0j_.qcd_unc  = 19.5/157.5;
  ttjets_NLO_fxfx4fs0j_.pdf_unc  = 14.7/157.5;

  // TTJets Incl NLO 4FS+1Jet Decays
  ttjets_NLO_fxfx4fs1j_.id       = 2521;
  ttjets_NLO_fxfx4fs1j_.name     = "TTJetsIncl_NLO_FxFx_4FS_tt_1Jet_max1Jet_8TeV_CTEQ6M_MadSpin";
  ttjets_NLO_fxfx4fs1j_.skimPath.push_back( path_skim_nlo_ + "FxFx_4FS_tt_1Jet_max1Jet_8TeV_CTEQ6M_MadSpin/" );
  ttjets_NLO_fxfx4fs1j_.getSkimFiles();
  ttjets_NLO_fxfx4fs1j_.label    = "ttbar_nlo_4fs_1jet";
  ttjets_NLO_fxfx4fs1j_.latex    = "t\\bar{t}+jets, NLO, 4FS+1Jet Madspin decays";
  ttjets_NLO_fxfx4fs1j_.xsec     = 245.8;
  ttjets_NLO_fxfx4fs1j_.ngen     = 85750;
  ttjets_NLO_fxfx4fs1j_.qcd_unc  = 19.5/157.5;
  ttjets_NLO_fxfx4fs1j_.pdf_unc  = 14.7/157.5;


  // TTJets+LF Hadronic Decays
  ttjets_lf_hadronic_.id       = 2566;
  ttjets_lf_hadronic_.name     = "TTJetsLF_HadronicMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x";
  ttjets_lf_hadronic_.skimPath.push_back( path_skim_ + "TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_lf_hadronic_.getSkimFiles();
  ttjets_lf_hadronic_.label    = "ttbar_hadronic";
  ttjets_lf_hadronic_.latex    = "t\\bar{t}+lf, hadronic decays";
  ttjets_lf_hadronic_.xsec     = 245.8 * 0.457;
  ttjets_lf_hadronic_.ngen     = 30997580;
  ttjets_lf_hadronic_.qcd_unc  = 19.5/157.5;
  ttjets_lf_hadronic_.pdf_unc  = 14.7/157.5;


  // TTJets+LF Semilep Decays
  ttjets_lf_semiLep_.id       = 2563;
  ttjets_lf_semiLep_.name     = "TTJetsLF_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_lf_semiLep_.skimPath.push_back( path_skim_ + "TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );  
  ttjets_lf_semiLep_.getSkimFiles();
  ttjets_lf_semiLep_.label    = "ttbar_semiLep";
  ttjets_lf_semiLep_.latex    = "t\\bar{t}+lf, semi-lep decays";
  ttjets_lf_semiLep_.xsec     = 245.8 * 0.438;
  ttjets_lf_semiLep_.ngen     = 25327478;
  ttjets_lf_semiLep_.qcd_unc  = 19.5/157.5;
  ttjets_lf_semiLep_.pdf_unc  = 14.7/157.5;
  

  // TTJets+LF Dilep Decays
  ttjets_lf_diLep_.id       = 2533;
  ttjets_lf_diLep_.name     = "TTJetsLF_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_lf_diLep_.skimPath.push_back( path_skim_ + "TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttjets_lf_diLep_.getSkimFiles();
  ttjets_lf_diLep_.label    = "ttbar_diLep";
  ttjets_lf_diLep_.latex    = "t\\bar{t}+lf, di-lep decays";
  ttjets_lf_diLep_.xsec     = 245.8 * 0.105;
  ttjets_lf_diLep_.ngen     = 12100452;
  ttjets_lf_diLep_.qcd_unc  = 19.5/157.5;
  ttjets_lf_diLep_.pdf_unc  = 14.7/157.5;


  // TTJets+LF Incl Decays
  ttjets_lf_.id       = 2501;
  ttjets_lf_.name     = "TTJetsLF_MGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_lf_.skimPath.push_back( path_skim_ + "TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_lf_.skimPath.push_back( path_skim_ + "TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttjets_lf_.skimPath.push_back( path_skim_ + "TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttjets_lf_.getSkimFiles();
  ttjets_lf_.label    = "ttbar";
  ttjets_lf_.latex    = "t\\bar{t}+lf";
  ttjets_lf_.xsec     = 245.8;
  ttjets_lf_.ngen     = 30997580+25327478+12100452;
  ttjets_lf_.qcd_unc  = 19.5/157.5;
  ttjets_lf_.pdf_unc  = 14.7/157.5;


  // TTJets+CC Hadronic Decays
  ttjets_cc_hadronic_.id       = 2576;
  ttjets_cc_hadronic_.name     = "TTJetsCC_HadronicMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_cc_hadronic_.skimPath.push_back( path_skim_ + "TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_cc_hadronic_.getSkimFiles(); 
  ttjets_cc_hadronic_.label    = "ttbarPlusCCbar_hadronic";
  ttjets_cc_hadronic_.latex    = "t\\bar{t}+c\\bar{c}, hadronic decays";
  ttjets_cc_hadronic_.xsec     = 245.8 * 0.457;
  ttjets_cc_hadronic_.ngen     = 31111456;
  ttjets_cc_hadronic_.qcd_unc  = 19.5/157.5;
  ttjets_cc_hadronic_.pdf_unc  = 14.7/157.5;


  // TTJets+CC Semilep Decays
  ttjets_cc_semiLep_.id       = 2573;
  ttjets_cc_semiLep_.name     = "TTJetsCC_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_cc_semiLep_.skimPath.push_back( path_skim_ + "TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );  
  ttjets_cc_semiLep_.getSkimFiles();
  ttjets_cc_semiLep_.label    = "ttbarPlusCCbar_semiLep";
  ttjets_cc_semiLep_.latex    = "t\\bar{t}+c\\bar{c}, semi-lep decays";
  ttjets_cc_semiLep_.xsec     = 245.8 * 0.438;
  ttjets_cc_semiLep_.ngen     = 25327478;
  ttjets_cc_semiLep_.qcd_unc  = 19.5/157.5;
  ttjets_cc_semiLep_.pdf_unc  = 14.7/157.5;
  

  // TTJets+CC Dilep Decays
  ttjets_cc_diLep_.id       = 2543;
  ttjets_cc_diLep_.name     = "TTJetsCC_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_cc_diLep_.skimPath.push_back( path_skim_ + "TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttjets_cc_diLep_.getSkimFiles();
  ttjets_cc_diLep_.label    = "ttbarPlusCCbar_diLep";
  ttjets_cc_diLep_.latex    = "t\\bar{t}+c\\bar{c}, di-lep decays";
  ttjets_cc_diLep_.xsec     = 245.8 * 0.105;
  ttjets_cc_diLep_.ngen     = 12100452;
  ttjets_cc_diLep_.qcd_unc  = 19.5/157.5;
  ttjets_cc_diLep_.pdf_unc  = 14.7/157.5;


  // TTJets+CC Incl Decays
  ttjets_cc_.id       = 2502;
  ttjets_cc_.name     = "TTJetsCC_MGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_cc_.skimPath.push_back( path_skim_ + "TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_cc_.skimPath.push_back( path_skim_ + "TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_cc_.skimPath.push_back( path_skim_ + "TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttjets_cc_.getSkimFiles();
  ttjets_cc_.label    = "ttbarPlusCCbar";
  ttjets_cc_.latex    = "t\\bar{t}+lf";
  ttjets_cc_.xsec     = 245.8;
  ttjets_cc_.ngen     = 30997580+25327478+12100452;
  ttjets_cc_.qcd_unc  = 19.5/157.5;
  ttjets_cc_.pdf_unc  = 14.7/157.5;
  

  // TTJets+1B Hadronic Decays
  ttjets_b_hadronic_.id       = 2596;
  ttjets_b_hadronic_.name     = "TTJetsB_HadronicMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_b_hadronic_.skimPath.push_back( path_skim_ + "TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_b_hadronic_.getSkimFiles();
  ttjets_b_hadronic_.label    = "ttbarPlusB_hadronic";
  ttjets_b_hadronic_.latex    = "t\\bar{t}+b, hadronic decays";
  ttjets_b_hadronic_.xsec     = 245.8 * 0.457;
  ttjets_b_hadronic_.ngen     = 31111456;
  ttjets_b_hadronic_.qcd_unc  = 19.5/157.5;
  ttjets_b_hadronic_.pdf_unc  = 14.7/157.5;


  // TTJets+1B Semilep Decays
  ttjets_b_semiLep_.id       = 2593;
  ttjets_b_semiLep_.name     = "TTJetsB_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_b_semiLep_.skimPath.push_back( path_skim_ + "TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttjets_b_semiLep_.getSkimFiles();
  ttjets_b_semiLep_.label    = "ttbarPlusB_semiLep";
  ttjets_b_semiLep_.latex    = "t\\bar{t}+b, semi-lep decays";
  ttjets_b_semiLep_.xsec     = 245.8 * 0.438;
  ttjets_b_semiLep_.ngen     = 25327478;
  ttjets_b_semiLep_.qcd_unc  = 19.5/157.5;
  ttjets_b_semiLep_.pdf_unc  = 14.7/157.5;
  

  // TTJets+1B Dilep Decays
  ttjets_b_diLep_.id       = 2599;
  ttjets_b_diLep_.name     = "TTJetsB_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_b_diLep_.skimPath.push_back( path_skim_ + "TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_b_diLep_.getSkimFiles();
  ttjets_b_diLep_.label    = "ttbarPlusB_diLep";
  ttjets_b_diLep_.latex    = "t\\bar{t}+b, di-lep decays";
  ttjets_b_diLep_.xsec     = 245.8 * 0.105;
  ttjets_b_diLep_.ngen     = 12100452;
  ttjets_b_diLep_.qcd_unc  = 19.5/157.5;
  ttjets_b_diLep_.pdf_unc  = 14.7/157.5;


  // TTJets+1B Incl Decays
  ttjets_b_.id       = 2503;
  ttjets_b_.name     = "TTJetsB_MGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_b_.skimPath.push_back( path_skim_ + "TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttjets_b_.skimPath.push_back( path_skim_ + "TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttjets_b_.skimPath.push_back( path_skim_ + "TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_b_.getSkimFiles();
  ttjets_b_.label    = "ttbarPlusB";
  ttjets_b_.latex    = "t\\bar{t}+b";
  ttjets_b_.xsec     = 245.8;
  ttjets_b_.ngen     = 30997580+25327478+12100452;
  ttjets_b_.qcd_unc  = 19.5/157.5;
  ttjets_b_.pdf_unc  = 14.7/157.5;


  // TTJets+2B Hadronic Decays
  ttjets_bb_hadronic_.id       = 2586;
  ttjets_bb_hadronic_.name     = "TTJetsBB_HadronicMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_bb_hadronic_.skimPath.push_back( path_skim_ + "TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_bb_hadronic_.getSkimFiles(); 
  ttjets_bb_hadronic_.label    = "ttbarPlusBBbar_hadronic";
  ttjets_bb_hadronic_.latex    = "t\\bar{t}+b\\bar{b}, hadronic decays";
  ttjets_bb_hadronic_.xsec     = 245.8 * 0.457;
  ttjets_bb_hadronic_.ngen     = 31111456;
  ttjets_bb_hadronic_.qcd_unc  = 19.5/157.5;
  ttjets_bb_hadronic_.pdf_unc  = 14.7/157.5;


  // TTJets+2B Semilep Decays
  ttjets_bb_semiLep_.id       = 2583;
  ttjets_bb_semiLep_.name     = "TTJetsBB_SemiLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_bb_semiLep_.skimPath.push_back( path_skim_ + "TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );  
  ttjets_bb_semiLep_.getSkimFiles(); 
  ttjets_bb_semiLep_.label    = "ttbarPlusBBbar_semiLep";
  ttjets_bb_semiLep_.latex    = "t\\bar{t}+b\\bar{b}, semi-lep decays";
  ttjets_bb_semiLep_.xsec     = 245.8 * 0.438;
  ttjets_bb_semiLep_.ngen     = 25327478;
  ttjets_bb_semiLep_.qcd_unc  = 19.5/157.5;
  ttjets_bb_semiLep_.pdf_unc  = 14.7/157.5;
  

  // TTJets+2B Dilep Decays
  ttjets_bb_diLep_.id       = 2553;
  ttjets_bb_diLep_.name     = "TTJetsBB_FullLeptMGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_bb_diLep_.skimPath.push_back( path_skim_ + "TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_bb_diLep_.getSkimFiles(); 
  ttjets_bb_diLep_.label    = "ttbarPlusBBbar_diLep";
  ttjets_bb_diLep_.latex    = "t\\bar{t}+b\\bar{b}, di-lep decays";
  ttjets_bb_diLep_.xsec     = 245.8 * 0.105;
  ttjets_bb_diLep_.ngen     = 12100452;
  ttjets_bb_diLep_.qcd_unc  = 19.5/157.5;
  ttjets_bb_diLep_.pdf_unc  = 14.7/157.5;


  // TTJets+2B Incl Decays
  ttjets_bb_.id       = 2504;
  ttjets_bb_.name     = "TTJetsBB_MGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_bb_.skimPath.push_back( path_skim_ + "TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_bb_.skimPath.push_back( path_skim_ + "TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttjets_bb_.skimPath.push_back( path_skim_ + "TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_bb_.getSkimFiles(); 
  ttjets_bb_.label    = "ttbarPlusBBbar";
  ttjets_bb_.latex    = "t\\bar{t}+b\\bar{b}";
  ttjets_bb_.xsec     = 245.8;
  ttjets_bb_.ngen     = 30997580+25327478+12100452;
  ttjets_bb_.qcd_unc  = 19.5/157.5;
  ttjets_bb_.pdf_unc  = 14.7/157.5;


  // TTJets+1and2B Incl Decays
  ttjets_1and2b_.id       = 2505;
  ttjets_1and2b_.name     = "TTJets1and2B_MGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_1and2b_.skimPath.push_back( path_skim_ + "TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_1and2b_.skimPath.push_back( path_skim_ + "TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttjets_1and2b_.skimPath.push_back( path_skim_ + "TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_1and2b_.getSkimFiles(); 
  ttjets_1and2b_.label    = "ttbarPlus1and2B";
  ttjets_1and2b_.latex    = "t\\bar{t}+b/b\\bar{b}";
  ttjets_1and2b_.xsec     = 245.8;
  ttjets_1and2b_.ngen     = 30997580+25327478+12100452;
  ttjets_1and2b_.qcd_unc  = 19.5/157.5;
  ttjets_1and2b_.pdf_unc  = 14.7/157.5;


  // TTJets+Incl Decays
  ttjets_incl_.id       = 2500;
  ttjets_incl_.name     = "TTJetsIncl_MGDecays_TuneZ2star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttjets_incl_.skimPath.push_back( path_skim_ + "TTJets_HadronicMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_HadronicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_incl_.skimPath.push_back( path_skim_ + "TTJets_SemiLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_TTJets_SemiLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttjets_incl_.skimPath.push_back( path_skim_ + "TTJets_FullLeptMGDecays_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTJets_FullLeptonicDecays_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  ttjets_incl_.getSkimFiles(); 
  ttjets_incl_.label    = "ttbarIncl";
  ttjets_incl_.latex    = "t\\bar{t}+inclusive";
  ttjets_incl_.xsec     = 245.8;
  ttjets_incl_.ngen     = 30997580+25327478+12100452;
  ttjets_incl_.qcd_unc  = 19.5/157.5;
  ttjets_incl_.pdf_unc  = 14.7/157.5;

  
  // TTW
  ttW_.id       = 2524;
  ttW_.name     = "TTWJets_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttW_.skimPath.push_back( path_skim_ + "TTWJets_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTW_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttW_.getSkimFiles(); 
  ttW_.label    = "ttbarW";
  ttW_.latex    = "t\\bar{t}+W";
  ttW_.xsec     = 0.232;
  ttW_.ngen     = 195396;
  ttW_.qcd_unc  = 0.0;
  ttW_.pdf_unc  = 0.0;
  

  // TTZ
  ttZ_.id       = 2523;
  ttZ_.name     = "TTZJets_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttZ_.skimPath.push_back( path_skim_ + "TTZJets_8TeV-madgraph_v2/NovaBeanSkim_53Xon53X_newBEANs_201304_TTZ_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttZ_.getSkimFiles(); 
  ttZ_.label    = "ttbarZ";
  ttZ_.latex    = "t\\bar{t}+Z";
  ttZ_.xsec     = 0.2057;
  ttZ_.ngen     = 209512;
  ttZ_.qcd_unc  = 0.0;
  ttZ_.pdf_unc  = 0.0;


  // TTV
  ttV_.id       = 2525;
  ttV_.name     = "TTVJets_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  ttV_.skimPath.push_back( path_skim_ + "TTWJets_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_TTW_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttV_.skimPath.push_back( path_skim_ + "TTZJets_8TeV-madgraph_v2/NovaBeanSkim_53Xon53X_newBEANs_201304_TTZ_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttV_.getSkimFiles(); 
  ttV_.label    = "ttbarV";
  ttV_.latex    = "t\\bar{t}+V";
  ttV_.xsec     = ((0.232/195396)+(0.2057/209512))*(195396+209512);
  ttV_.ngen     = 195396+209512;
  ttV_.qcd_unc  = 0.0;
  ttV_.pdf_unc  = 0.0;
  

  // SingleT sChan
  singlet_s_.id       = 2600;
  singlet_s_.name     = "T_s_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
  singlet_s_.skimPath.push_back( path_skim_ + "T_s-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleT_sChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  singlet_s_.getSkimFiles(); 
  singlet_s_.label    = "singlet_s";
  singlet_s_.latex    = "single t, s-channel";
  singlet_s_.xsec     = 3.79;
  singlet_s_.ngen     = 259657;
  singlet_s_.qcd_unc  = 0.0;
  singlet_s_.pdf_unc  = 0.16/4.21;


  // SingleT tChan
  singlet_t_.id       = 2602;
  singlet_t_.name     = "T_t_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
  singlet_t_.skimPath.push_back( path_skim_ + "T_t-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleT_tChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  singlet_t_.getSkimFiles(); 
  singlet_t_.label    = "singlet_t";
  singlet_t_.latex    = "single t, t-channel";
  singlet_t_.xsec     = 56.4;
  singlet_t_.ngen     = 3744404;
  singlet_t_.qcd_unc  = 0.0;
  singlet_t_.pdf_unc  = 3.0/64.6;


  // SingleT tWChan
  singlet_tW_.id       = 2604;
  singlet_tW_.name     = "T_tW_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
  singlet_tW_.skimPath.push_back( path_skim_ + "T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleT_tWChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  singlet_tW_.getSkimFiles(); 
  singlet_tW_.label    = "singlet_tW";
  singlet_tW_.latex    = "single t, tW-channel";
  singlet_tW_.xsec     = 11.1;
  singlet_tW_.ngen     = 496918;
  singlet_tW_.qcd_unc  = 0.0;
  singlet_tW_.pdf_unc  = 0.8/10.6;


  // SingleTbar sChan
  singletbar_s_.id       = 2601;
  singletbar_s_.name     = "Tbar_s_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
  singletbar_s_.skimPath.push_back( path_skim_ + "Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleTbar_sChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  singletbar_s_.getSkimFiles(); 
  singletbar_s_.label    = "singletbar_s";
  singletbar_s_.latex    = "single \\bar{t}, s-channel";
  singletbar_s_.xsec     = 1.76;
  singletbar_s_.ngen     = 139835;
  singletbar_s_.qcd_unc  = 0.0;
  singletbar_s_.pdf_unc  = 0.16/4.21;


  // SingleTbar tChan
  singletbar_t_.id       = 2603;
  singletbar_t_.name     = "Tbar_t_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
  singletbar_t_.skimPath.push_back( path_skim_ + "Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleTbar_tChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  singletbar_t_.getSkimFiles(); 
  singletbar_t_.label    = "singletbar_t";
  singletbar_t_.latex    = "single \\bar{t}, t-channel";
  singletbar_t_.xsec     = 30.7;
  singletbar_t_.ngen     = 1933504;
  singletbar_t_.qcd_unc  = 0.0;
  singletbar_t_.pdf_unc  = 3.0/64.6;


  // SingleTbar tWChan
  singletbar_tW_.id       = 2605;
  singletbar_tW_.name     = "Tbar_tW_channel_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
  singletbar_tW_.skimPath.push_back( path_skim_ + "Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleTbar_tWChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  singletbar_tW_.getSkimFiles(); 
  singletbar_tW_.label    = "singletbar_tW";
  singletbar_tW_.latex    = "single \\bar{t}, tW-channel";
  singletbar_tW_.xsec     = 11.1;
  singletbar_tW_.ngen     = 492779;
  singletbar_tW_.qcd_unc  = 0.0;
  singletbar_tW_.pdf_unc  = 0.8/10.6;


  // SingleT Inclusive
  singlet_.id       = 2606;
  singlet_.name     = "SingleT_TuneZ2star_8TeV_powheg_Summer12_53xOn53x_beanVer2";
  singlet_.skimPath.push_back( path_skim_ + "T_s-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleT_sChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  singlet_.skimPath.push_back( path_skim_ + "T_t-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleT_tChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  singlet_.skimPath.push_back( path_skim_ + "T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleT_tWChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  singlet_.skimPath.push_back( path_skim_ + "Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleTbar_sChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  singlet_.skimPath.push_back( path_skim_ + "Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleTbar_tChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  singlet_.skimPath.push_back( path_skim_ + "Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_singleTbar_tWChan_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  singlet_.getSkimFiles(); 
  singlet_.label    = "singlet";
  singlet_.latex    = "single t";
  singlet_.xsec     = ( (3.79/259657)+(56.4/3744404)+(11.1/496918)+(1.76/139835)+(30.7/1933504)+(11.1/492779) )*(259657+3744404+496918+139835+1933504+492779);
  singlet_.ngen     = 259657+3744404+496918+139835+1933504+492779;
  singlet_.qcd_unc  = 0.0;
  singlet_.pdf_unc  = 0.0;


  // W1Jets
  w1jets_.id       = 2401;
  w1jets_.name     = "W1JetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  w1jets_.skimPath.push_back( path_skim_ + "W1JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W1Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  w1jets_.getSkimFiles(); 
  w1jets_.label    = "w1jets";
  w1jets_.latex    = "W+1 Jet";
  w1jets_.xsec     = 6440.4;
  w1jets_.ngen     = 23134881;
  w1jets_.qcd_unc  = 407.0/31314;
  w1jets_.pdf_unc  = 1504.0/31314;


  // W2Jets
  w2jets_.id       = 2402;
  w2jets_.name     = "W2JetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  w2jets_.skimPath.push_back( path_skim_ + "W2JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W2Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  w2jets_.getSkimFiles(); 
  w2jets_.label    = "w2jets";
  w2jets_.latex    = "W+2 Jets";
  w2jets_.xsec     = 2087.2;
  w2jets_.ngen     = 33933328;
  w2jets_.qcd_unc  = 407.0/31314;
  w2jets_.pdf_unc  = 1504.0/31314;
  

  // W3Jets
  w3jets_.id       = 2403;
  w3jets_.name     = "W3JetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  w3jets_.skimPath.push_back( path_skim_ + "W3JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W3Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  w3jets_.getSkimFiles(); 
  w3jets_.label    = "w3jets";
  w3jets_.latex    = "W+3 Jets";
  w3jets_.xsec     = 619.0;
  w3jets_.ngen     = 15463420;
  w3jets_.qcd_unc  = 407.0/31314;
  w3jets_.pdf_unc  = 1504.0/31314;


  // W4Jets
  w4jets_.id       = 2404;
  w4jets_.name     = "W4JetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  w4jets_.skimPath.push_back( path_skim_ + "W4JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W4Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  w4jets_.getSkimFiles(); 
  w4jets_.label    = "w4jets";
  w4jets_.latex    = "W+4 Jets";
  w4jets_.xsec     = 255.2;
  w4jets_.ngen     = 13365439;
  w4jets_.qcd_unc  = 407.0/31314;
  w4jets_.pdf_unc  = 1504.0/31314;


  // WJets 
  wjets_.id       = 2405;
  wjets_.name     = "WJetsToLNu_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  wjets_.skimPath.push_back( path_skim_ + "W1JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W1Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  wjets_.skimPath.push_back( path_skim_ + "W2JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W2Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  wjets_.skimPath.push_back( path_skim_ + "W3JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W3Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  wjets_.skimPath.push_back( path_skim_ + "W4JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W4Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  wjets_.getSkimFiles(); 
  wjets_.label    = "wjets";
  wjets_.latex    = "W+Jets";
  wjets_.xsec     = ( (6440.4/23134881)+(2087.2/33933328)+(619.0/15463420)+(255.2/13365439) )*(23134881+33933328+15463420+13365439);
  wjets_.ngen     = 23134881+33933328+15463420+13365439;
  wjets_.qcd_unc  = 407.0/31314;
  wjets_.pdf_unc  = 1504.0/31314;


  // ZJets, M10-50
  zjets_lowMass_.id       = 2850;
  zjets_lowMass_.name     = "DYJetsToLL_M10To50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  zjets_lowMass_.skimPath.push_back( path_skim_ + "DYJetsToLL_M-10To50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DYJets_M10to50_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  zjets_lowMass_.getSkimFiles(); 
  zjets_lowMass_.label    = "zjets_lowMass";
  zjets_lowMass_.latex    = "Z+Jets, M10-50";
  zjets_lowMass_.xsec     = 14702.0;
  zjets_lowMass_.ngen     = 37828841;
  zjets_lowMass_.qcd_unc  = 34.0/3048;
  zjets_lowMass_.pdf_unc  = 128.0/3048;

  
  // Z1Jets
  z1jets_.id       = 2801;
  z1jets_.name     = "DY1JetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  z1jets_.skimPath.push_back( path_skim_ + "DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY1Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  z1jets_.getSkimFiles(); 
  z1jets_.label    = "z1jets";
  z1jets_.latex    = "Z+1 Jet";
  z1jets_.xsec     = 666.7;
  z1jets_.ngen     = 24032562;
  z1jets_.qcd_unc  = 34.0/3048;
  z1jets_.pdf_unc  = 128.0/3048;
  

  // Z2Jets
  z2jets_.id       = 2802;
  z2jets_.name     = "DY2JetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  z2jets_.skimPath.push_back( path_skim_ + "DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY2Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  z2jets_.getSkimFiles(); 
  z2jets_.label    = "z2jets";
  z2jets_.latex    = "Z+2 Jets";
  z2jets_.xsec     = 215.1;
  z2jets_.ngen     = 2350806;
  z2jets_.qcd_unc  = 34.0/3048;
  z2jets_.pdf_unc  = 128.0/3048;


  // Z3Jets
  z3jets_.id       = 2803;
  z3jets_.name     = "DY3JetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  z3jets_.skimPath.push_back( path_skim_ + "DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY3Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  z3jets_.getSkimFiles(); 
  z3jets_.label    = "z3jets";
  z3jets_.latex    = "Z+3 Jets";
  z3jets_.xsec     = 66.07;
  z3jets_.ngen     = 10753491;
  z3jets_.qcd_unc  = 34.0/3048;
  z3jets_.pdf_unc  = 128.0/3048;


  // Z4Jets
  z4jets_.id       = 2804;
  z4jets_.name     = "DY4JetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  z4jets_.skimPath.push_back( path_skim_ + "DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY4Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  z4jets_.getSkimFiles(); 
  z4jets_.label    = "z4jets";
  z4jets_.latex    = "Z+4 Jets";
  z4jets_.xsec     = 27.38;
  z4jets_.ngen     = 6370630;
  z4jets_.qcd_unc  = 34.0/3048;
  z4jets_.pdf_unc  = 128.0/3048;


  // ZJets Inclusive
  zjets_.id       = 2805;
  zjets_.name     = "DYJetsToLL_M50_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  zjets_.skimPath.push_back( path_skim_ + "DYJetsToLL_M-10To50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DYJets_M10to50_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  zjets_.skimPath.push_back( path_skim_ + "DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY1Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  zjets_.skimPath.push_back( path_skim_ + "DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY2Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  zjets_.skimPath.push_back( path_skim_ + "DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY3Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  zjets_.skimPath.push_back( path_skim_ + "DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY4Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  zjets_.getSkimFiles(); 
  zjets_.label    = "zjets";
  zjets_.latex    = "Z+Jets";
  zjets_.xsec     = ( (14702.0/37828841)+(666.7/24032562)+(215.1/2350806)+(66.07/10753491)+(27.38/6370630) )*(37828841+24032562+2350806+10753491+6370630);
  zjets_.ngen     = 37828841+24032562+2350806+10753491+6370630;
  zjets_.qcd_unc  = 34.0/3048;
  zjets_.pdf_unc  = 128.0/3048;


  // VJets Inclusive
  vjets_.id       = 2806;
  vjets_.name     = "VJets_TuneZ2Star_8TeV_madgraph_Summer12_53xOn53x_beanVer2";
  vjets_.skimPath.push_back( path_skim_ + "W1JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W1Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  vjets_.skimPath.push_back( path_skim_ + "W2JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W2Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  vjets_.skimPath.push_back( path_skim_ + "W3JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W3Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  vjets_.skimPath.push_back( path_skim_ + "W4JetsToLNu_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_W4Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  vjets_.skimPath.push_back( path_skim_ + "DYJetsToLL_M-10To50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DYJets_M10to50_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  vjets_.skimPath.push_back( path_skim_ + "DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY1Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  vjets_.skimPath.push_back( path_skim_ + "DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY2Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  vjets_.skimPath.push_back( path_skim_ + "DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY3Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  vjets_.skimPath.push_back( path_skim_ + "DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/NovaBeanSkim_53Xon53X_newBEANs_201304_DY4Jets_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  vjets_.getSkimFiles(); 
  vjets_.label    = "vjets";
  vjets_.latex    = "V+Jets";
  vjets_.xsec     = ( (6440.4/23134881)+(2087.2/33933328)+(619.0/15463420)+(255.2/13365439)+(14702.0/37828841)+(666.7/24032562)+(215.1/2350806)+(66.07/10753491)+(27.38/6370630) )*(23134881+33933328+15463420+13365439+37828841+24032562+2350806+10753491+6370630);
  vjets_.ngen     = 23134881+33933328+15463420+13365439+37828841+24032562+2350806+10753491+6370630;
  vjets_.qcd_unc  = 0.0;
  vjets_.pdf_unc  = 0.0;


  // WW
  ww_.id       = 2700;
  ww_.name     = "WW_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53xOn53x_beanVer2";
  ww_.skimPath.push_back( path_skim_ + "WW_TuneZ2star_8TeV_pythia6_tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_WW_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ww_.getSkimFiles(); 
  ww_.label    = "ww";
  ww_.latex    = "WW";
  ww_.xsec     = 57.1097;
  ww_.ngen     = 9955089;
  ww_.qcd_unc  = 0.0;
  ww_.pdf_unc  = 1.5/43.;


  // WZ
  wz_.id       = 2701;
  wz_.name     = "WZ_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53xOn53x_beanVer2";
  wz_.skimPath.push_back( path_skim_ + "WZ_TuneZ2star_8TeV_pythia6_tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_WZ_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  wz_.getSkimFiles(); 
  wz_.label    = "wz";
  wz_.latex    = "WZ";
  wz_.xsec     = 32.3161;
  wz_.ngen     = 9931257;
  wz_.qcd_unc  = 0.0;
  wz_.pdf_unc  = 0.7/18.2;


  // ZZ
  zz_.id       = 2702;
  zz_.name     = "ZZ_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53xOn53x_beanVer2";
  zz_.skimPath.push_back( path_skim_ + "ZZ_TuneZ2star_8TeV_pythia6_tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_ZZ_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  zz_.getSkimFiles(); 
  zz_.label    = "zz";
  zz_.latex    = "ZZ";
  zz_.xsec     = 8.25561;
  zz_.ngen     = 9755621;
  zz_.qcd_unc  = 0.0;
  zz_.pdf_unc  = 0.15/5.9;


  // diboson
  diboson_.id       = 2703;
  diboson_.name     = "VV_TuneZ2Star_8TeV_pythia6Tauola_Summer12_53xOn53x_beanVer2";
  diboson_.skimPath.push_back( path_skim_ + "WW_TuneZ2star_8TeV_pythia6_tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_WW_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  diboson_.skimPath.push_back( path_skim_ + "WZ_TuneZ2star_8TeV_pythia6_tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_WZ_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  diboson_.skimPath.push_back( path_skim_ + "ZZ_TuneZ2star_8TeV_pythia6_tauola/NovaBeanSkim_53Xon53X_newBEANs_201304_ZZ_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  diboson_.getSkimFiles(); 
  diboson_.label    = "diboson";
  diboson_.latex    = "VV";
  diboson_.xsec     = ( (57.1097/9955089)+(32.3161/9931257)+(8.25561/9755621) )*(9955089+9931257+9755621);
  diboson_.ngen     = 9955089+9931257+9755621;
  diboson_.qcd_unc  = 0.0;
  diboson_.pdf_unc  = 0.0;


  // ttH, M125, Inclusive Decays
  ttH_M125_.id       = 9125;
  ttH_M125_.name     = "TTH_Inclusive_M_125_8TeV_53xOn53x_beanVer2";
  ttH_M125_.skimPath.push_back( path_skim_ + "TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M125_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttH_M125_.getSkimFiles(); 
  ttH_M125_.label    = "ttH125";
  ttH_M125_.latex    = "t\\bar{t}+H, M(125)";
  ttH_M125_.xsec     = 0.1302;
  ttH_M125_.ngen     = 992997;
  ttH_M125_.qcd_unc  = 0.0;
  ttH_M125_.pdf_unc  = 0.0;


  // ttH, M125, H->bb Decays
  ttH_M125_hbb_.id       = 91250;
  ttH_M125_hbb_.name     = "TTH_hbb_M_125_8TeV_53xOn53x_beanVer2";
  ttH_M125_hbb_.skimPath.push_back( path_skim_ + "TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M125_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttH_M125_hbb_.getSkimFiles(); 
  ttH_M125_hbb_.label    = "ttH125_hbb";
  ttH_M125_hbb_.latex    = "t\\bar{t}+H, M(125), H \\rightarrow b\\bar{b}";
  ttH_M125_hbb_.xsec     = 0.1302;
  ttH_M125_hbb_.ngen     = 992997;
  ttH_M125_hbb_.qcd_unc  = 0.0;
  ttH_M125_hbb_.pdf_unc  = 0.0;


  // ttH, M125, H->cc Decays
  ttH_M125_hcc_.id       = 91251;
  ttH_M125_hcc_.name     = "TTH_hcc_M_125_8TeV_53xOn53x_beanVer2";
  ttH_M125_hcc_.skimPath.push_back( path_skim_ + "TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M125_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttH_M125_hcc_.getSkimFiles(); 
  ttH_M125_hcc_.label    = "ttH125_hcc";
  ttH_M125_hcc_.latex    = "t\\bar{t}+H, M(125), H \\rightarrow c\\bar{c}";
  ttH_M125_hcc_.xsec     = 0.1302;
  ttH_M125_hcc_.ngen     = 992997;
  ttH_M125_hcc_.qcd_unc  = 0.0;
  ttH_M125_hcc_.pdf_unc  = 0.0;

  
  // ttH, M125, H->WW Decays
  ttH_M125_hww_.id       = 91252;
  ttH_M125_hww_.name     = "TTH_hww_M_125_8TeV_53xOn53x_beanVer2";
  ttH_M125_hww_.skimPath.push_back( path_skim_ + "TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M125_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttH_M125_hww_.getSkimFiles(); 
  ttH_M125_hww_.label    = "ttH125_hww";
  ttH_M125_hww_.latex    = "t\\bar{t}+H, M(125), H \\rightarrow WW";
  ttH_M125_hww_.xsec     = 0.1302;
  ttH_M125_hww_.ngen     = 992997;
  ttH_M125_hww_.qcd_unc  = 0.0;
  ttH_M125_hww_.pdf_unc  = 0.0;


  // ttH, M125, H->ZZ Decays
  ttH_M125_hzz_.id       = 91253;
  ttH_M125_hzz_.name     = "TTH_hzz_M_125_8TeV_53xOn53x_beanVer2";
  ttH_M125_hzz_.skimPath.push_back( path_skim_ + "TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M125_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttH_M125_hzz_.getSkimFiles(); 
  ttH_M125_hzz_.label    = "ttH125_hzz";
  ttH_M125_hzz_.latex    = "t\\bar{t}+H, M(125), H \\rightarrow ZZ";
  ttH_M125_hzz_.xsec     = 0.1302;
  ttH_M125_hzz_.ngen     = 992997;
  ttH_M125_hzz_.qcd_unc  = 0.0;
  ttH_M125_hzz_.pdf_unc  = 0.0;

  
  // ttH, M125, H->tautau Decays
  ttH_M125_htt_.id       = 91254;
  ttH_M125_htt_.name     = "TTH_htt_M_125_8TeV_53xOn53x_beanVer2";
  ttH_M125_htt_.skimPath.push_back( path_skim_ + "TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M125_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttH_M125_htt_.getSkimFiles(); 
  ttH_M125_htt_.label    = "ttH125_htt";
  ttH_M125_htt_.latex    = "t\\bar{t}+H, M(125), H \\rightarrow \\tau \\tau";
  ttH_M125_htt_.xsec     = 0.1302;
  ttH_M125_htt_.ngen     = 992997;
  ttH_M125_htt_.qcd_unc  = 0.0;
  ttH_M125_htt_.pdf_unc  = 0.0;
  
  
  // ttH, M125, H->gg Decays
  ttH_M125_hgg_.id       = 91255;
  ttH_M125_hgg_.name     = "TTH_hgg_M_125_8TeV_53xOn53x_beanVer2";
  ttH_M125_hgg_.skimPath.push_back( path_skim_ + "TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M125_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttH_M125_hgg_.getSkimFiles(); 
  ttH_M125_hgg_.label    = "ttH125_hgg";
  ttH_M125_hgg_.latex    = "t\\bar{t}+H, M(125), H \\rightarrow \\gamma \\gamma";
  ttH_M125_hgg_.xsec     = 0.1302;
  ttH_M125_hgg_.ngen     = 992997;
  ttH_M125_hgg_.qcd_unc  = 0.0;
  ttH_M125_hgg_.pdf_unc  = 0.0;


  // ttH, M125, H->jj Decays
  ttH_M125_hjj_.id       = 91256;
  ttH_M125_hjj_.name     = "TTH_hjj_M_125_8TeV_53xOn53x_beanVer2";
  ttH_M125_hjj_.skimPath.push_back( path_skim_ + "TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M125_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttH_M125_hjj_.getSkimFiles(); 
  ttH_M125_hjj_.label    = "ttH125_hjj";
  ttH_M125_hjj_.latex    = "t\\bar{t}+H, M(125), H \\rightarrow jj";
  ttH_M125_hjj_.xsec     = 0.1302;
  ttH_M125_hjj_.ngen     = 992997;
  ttH_M125_hjj_.qcd_unc  = 0.0;
  ttH_M125_hjj_.pdf_unc  = 0.0;

  
  // ttH, M125, H->zg Decays
  ttH_M125_hzg_.id       = 91257;
  ttH_M125_hzg_.name     = "TTH_hzg_M_125_8TeV_53xOn53x_beanVer2";
  ttH_M125_hzg_.skimPath.push_back( path_skim_ + "TTH_Inclusive_M-125_8TeV_pythia6/NovaBeanSkim_53Xon53X_newBEANs_201304_TTH_Inclusive_M125_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  ttH_M125_hzg_.getSkimFiles(); 
  ttH_M125_hzg_.label    = "ttH125_hzg";
  ttH_M125_hzg_.latex    = "t\\bar{t}+H, M(125), H \\rightarrow Z \\gamma";
  ttH_M125_hzg_.xsec     = 0.1302;
  ttH_M125_hzg_.ngen     = 992997;
  ttH_M125_hzg_.qcd_unc  = 0.0;
  ttH_M125_hzg_.pdf_unc  = 0.0;


  // data, single mu, 2012 ABCD
  data_singleMu_2012ABCD_.id       = -1;
  data_singleMu_2012ABCD_.name     = "SingleMu_2012ABCD_BEAN_53xOn53x_beanVer2";
  data_singleMu_2012ABCD_.skimPath.push_back( path_skim_ + "SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012A_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleMu_2012ABCD_.skimPath.push_back( path_skim_ + "SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012Arecover_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleMu_2012ABCD_.skimPath.push_back( path_skim_ + "SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012B_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleMu_2012ABCD_.skimPath.push_back( path_skim_ + "SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012CpromptReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleMu_2012ABCD_.skimPath.push_back( path_skim_ + "SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012CreReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleMu_2012ABCD_.skimPath.push_back( path_skim_ + "SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012D_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleMu_2012ABCD_.getSkimFiles(); 
  data_singleMu_2012ABCD_.label    = "data_obs_mu";
  data_singleMu_2012ABCD_.latex    = "Data, Single Muon Trigger, 2012ABCD";
  data_singleMu_2012ABCD_.xsec     = 1.0;
  data_singleMu_2012ABCD_.ngen     = 1.0;
  data_singleMu_2012ABCD_.qcd_unc  = 0.0;
  data_singleMu_2012ABCD_.pdf_unc  = 0.0;


  // data, single ele, 2012 ABCD
  data_singleEle_2012ABCD_.id       = -2;
  data_singleEle_2012ABCD_.name     = "SingleElectron_2012ABCD_BEAN_53xOn53x_beanVer2";
  data_singleEle_2012ABCD_.skimPath.push_back( path_skim_ + "SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012A_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleEle_2012ABCD_.skimPath.push_back( path_skim_ + "SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012Arecover_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  data_singleEle_2012ABCD_.skimPath.push_back( path_skim_ + "SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012B_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  data_singleEle_2012ABCD_.skimPath.push_back( path_skim_ + "SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012CpromptReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  data_singleEle_2012ABCD_.skimPath.push_back( path_skim_ + "SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012CreReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  data_singleEle_2012ABCD_.skimPath.push_back( path_skim_ + "SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012D_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  data_singleEle_2012ABCD_.getSkimFiles(); 
  data_singleEle_2012ABCD_.label    = "data_obs_ele";
  data_singleEle_2012ABCD_.latex    = "Data, Single Electron Trigger, 2012ABCD";
  data_singleEle_2012ABCD_.xsec     = 1.0;
  data_singleEle_2012ABCD_.ngen     = 1.0;
  data_singleEle_2012ABCD_.qcd_unc  = 0.0;
  data_singleEle_2012ABCD_.pdf_unc  = 0.0;


  // data, single lepton, 2012 ABCD
  data_singleLep_2012ABCD_.id       = -3;
  data_singleLep_2012ABCD_.name     = "SingleLepton_2012ABCD_BEAN_53xOn53x_beanVer2";
  data_singleLep_2012ABCD_.skimPath.push_back( path_skim_ + "SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012A_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleLep_2012ABCD_.skimPath.push_back( path_skim_ + "SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012Arecover_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleLep_2012ABCD_.skimPath.push_back( path_skim_ + "SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012B_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleLep_2012ABCD_.skimPath.push_back( path_skim_ + "SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012CpromptReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleLep_2012ABCD_.skimPath.push_back( path_skim_ + "SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012CreReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleLep_2012ABCD_.skimPath.push_back( path_skim_ + "SingleMu/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleMu_2012D_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  data_singleLep_2012ABCD_.skimPath.push_back( path_skim_ + "SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012A_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" ); 
  data_singleLep_2012ABCD_.skimPath.push_back( path_skim_ + "SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012Arecover_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  data_singleLep_2012ABCD_.skimPath.push_back( path_skim_ + "SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012B_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  data_singleLep_2012ABCD_.skimPath.push_back( path_skim_ + "SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012CpromptReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  data_singleLep_2012ABCD_.skimPath.push_back( path_skim_ + "SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012CreReco_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  data_singleLep_2012ABCD_.skimPath.push_back( path_skim_ + "SingleElectron/NovaBeanSkim_53Xon53X_newBEANs_201304_SingleEle_2012D_20130426_skimv0/03283e8eb3a7752fe70a1a708b319805/" );
  data_singleLep_2012ABCD_.getSkimFiles(); 
  data_singleLep_2012ABCD_.label    = "data_obs";
  data_singleLep_2012ABCD_.latex    = "Data, Single Lepton Trigger, 2012ABCD";
  data_singleLep_2012ABCD_.xsec     = 1.0;
  data_singleLep_2012ABCD_.ngen     = 1.0;
  data_singleLep_2012ABCD_.qcd_unc  = 0.0;
  data_singleLep_2012ABCD_.pdf_unc  = 0.0;



  //
  // Fill Sample List Containers
  //

  // Collection of Samples that appear as anaTrees
  sampleList_trees_.push_back(ttjets_NLO_fxfx4fs0j_);
  sampleList_trees_.push_back(ttjets_NLO_fxfx4fs1j_);
  sampleList_trees_.push_back(ttjets_lf_semiLep_);
  sampleList_trees_.push_back(ttjets_lf_diLep_);
  sampleList_trees_.push_back(ttjets_cc_hadronic_);
  sampleList_trees_.push_back(ttjets_cc_semiLep_);
  sampleList_trees_.push_back(ttjets_cc_diLep_);
  sampleList_trees_.push_back(ttjets_b_hadronic_);
  sampleList_trees_.push_back(ttjets_b_semiLep_);
  sampleList_trees_.push_back(ttjets_b_diLep_);
  sampleList_trees_.push_back(ttjets_bb_hadronic_);
  sampleList_trees_.push_back(ttjets_bb_semiLep_);
  sampleList_trees_.push_back(ttjets_bb_diLep_);
  sampleList_trees_.push_back(ttW_);
  sampleList_trees_.push_back(ttZ_);
  sampleList_trees_.push_back(singlet_s_);
  sampleList_trees_.push_back(singlet_t_);
  sampleList_trees_.push_back(singlet_tW_);
  sampleList_trees_.push_back(singletbar_s_);
  sampleList_trees_.push_back(singletbar_t_);
  sampleList_trees_.push_back(singletbar_tW_);
  sampleList_trees_.push_back(w1jets_);
  sampleList_trees_.push_back(w2jets_);
  sampleList_trees_.push_back(w3jets_);
  sampleList_trees_.push_back(w4jets_);
  sampleList_trees_.push_back(zjets_lowMass_);
  sampleList_trees_.push_back(z1jets_);
  sampleList_trees_.push_back(z2jets_);
  sampleList_trees_.push_back(z3jets_);
  sampleList_trees_.push_back(z4jets_);
  sampleList_trees_.push_back(ww_);
  sampleList_trees_.push_back(wz_);
  sampleList_trees_.push_back(zz_);
  sampleList_trees_.push_back(ttH_M125_);
  sampleList_trees_.push_back(data_singleMu_2012ABCD_);
  sampleList_trees_.push_back(data_singleEle_2012ABCD_);


  // Collection of Samples often grouped together into histograms
  sampleList_histos_.push_back(ttjets_lf_);
  sampleList_histos_.push_back(ttjets_cc_);
  sampleList_histos_.push_back(ttjets_b_);
  sampleList_histos_.push_back(ttjets_bb_);
  sampleList_histos_.push_back(ttW_);
  sampleList_histos_.push_back(ttZ_);
  sampleList_histos_.push_back(singlet_);
  sampleList_histos_.push_back(wjets_);
  sampleList_histos_.push_back(zjets_);
  sampleList_histos_.push_back(diboson_);
  sampleList_histos_.push_back(ttH_M125_);
  sampleList_histos_.push_back(data_singleLep_2012ABCD_);
  
  
  // Collection of Samples used in plotting
  sampleList_plots_.push_back(ttjets_lf_);
  sampleList_plots_.push_back(ttjets_cc_);
  sampleList_plots_.push_back(ttjets_b_);
  sampleList_plots_.push_back(ttjets_bb_);
  sampleList_plots_.push_back(ttV_);
  sampleList_plots_.push_back(singlet_);
  sampleList_plots_.push_back(ewk_);
  sampleList_plots_.push_back(ttH_M125_);
  sampleList_plots_.push_back(data_singleLep_2012ABCD_);
  
  
  // Collection of samples used in limit setting
  sampleList_limits_.push_back(ttjets_lf_);
  sampleList_limits_.push_back(ttjets_cc_);
  sampleList_limits_.push_back(ttjets_b_);
  sampleList_limits_.push_back(ttjets_bb_);
  sampleList_limits_.push_back(ttW_);
  sampleList_limits_.push_back(ttZ_);
  sampleList_limits_.push_back(singlet_);
  sampleList_limits_.push_back(wjets_);
  sampleList_limits_.push_back(zjets_);
  sampleList_limits_.push_back(diboson_);
  sampleList_limits_.push_back(ttH_M125_);
  sampleList_limits_.push_back(data_singleLep_2012ABCD_);


  //
  // Initialize Systematics
  //
  
  // Luminosity
  luminosity_.id                  = 0;
  luminosity_.type                = "lnN";
  luminosity_.title               = "Luminosity";
  luminosity_.label               = "lumi";
  luminosity_.latex               = "Luminosity";
  luminosity_.rate_ttH            = 1.044;
  luminosity_.rate_ttbar          = 1.044;
  luminosity_.rate_ttbarPlusB     = 1.044;
  luminosity_.rate_ttbarPlusBBbar = 1.044;
  luminosity_.rate_ttbarPlusCCbar = 1.044;
  luminosity_.rate_singlet        = 1.044;
  luminosity_.rate_wjets          = 1.044;
  luminosity_.rate_zjets          = 1.044;
  luminosity_.rate_ttbarW         = 1.044;
  luminosity_.rate_ttbarZ         = 1.044;
  luminosity_.rate_diboson        = 1.044;


  // QCDscale_ttH
  QCDscale_ttH_.id                  = 1;
  QCDscale_ttH_.type                = "lnN";
  QCDscale_ttH_.title               = "QCD scale, t#bar{t}H";
  QCDscale_ttH_.label               = "QCDscale_ttH";
  QCDscale_ttH_.latex               = "QCD scale, t$\\bar{t}$H";
  QCDscale_ttH_.rate_ttH            = 1.125;
  QCDscale_ttH_.rate_ttbar          = 0.0;
  QCDscale_ttH_.rate_ttbarPlusB     = 0.0;
  QCDscale_ttH_.rate_ttbarPlusBBbar = 0.0;
  QCDscale_ttH_.rate_ttbarPlusCCbar = 0.0;
  QCDscale_ttH_.rate_singlet        = 0.0;
  QCDscale_ttH_.rate_wjets          = 0.0;
  QCDscale_ttH_.rate_zjets          = 0.0;
  QCDscale_ttH_.rate_ttbarW         = 0.0;
  QCDscale_ttH_.rate_ttbarZ         = 0.0;
  QCDscale_ttH_.rate_diboson        = 0.0;

  
  // QCDscale_ttbar
  QCDscale_ttbar_.id                  = 2;
  QCDscale_ttbar_.type                = "lnN";
  QCDscale_ttbar_.title               = "QCD scale, t#bar{t}";
  QCDscale_ttbar_.label               = "QCDscale_ttbar";
  QCDscale_ttbar_.latex               = "QCD scale, t$\\bar{t}$";
  QCDscale_ttbar_.rate_ttH            = 0.0;
  QCDscale_ttbar_.rate_ttbar          = 1.030;
  QCDscale_ttbar_.rate_ttbarPlusB     = 1.030;
  QCDscale_ttbar_.rate_ttbarPlusBBbar = 1.030;
  QCDscale_ttbar_.rate_ttbarPlusCCbar = 1.030;
  QCDscale_ttbar_.rate_singlet        = 1.02;
  QCDscale_ttbar_.rate_wjets          = 0.0;
  QCDscale_ttbar_.rate_zjets          = 0.0;
  QCDscale_ttbar_.rate_ttbarW         = 0.0;
  QCDscale_ttbar_.rate_ttbarZ         = 0.0;
  QCDscale_ttbar_.rate_diboson        = 0.0;
  

  // QCDscale_ttW
  QCDscale_ttW_.id                  = 3;
  QCDscale_ttW_.type                = "lnN";
  QCDscale_ttW_.title               = "QCD scale, t#bar{t}W";
  QCDscale_ttW_.label               = "QCDscale_ttW";
  QCDscale_ttW_.latex               = "QCD scale, t$\\bar{t}$W";
  QCDscale_ttW_.rate_ttH            = 0.0;
  QCDscale_ttW_.rate_ttbar          = 0.0;
  QCDscale_ttW_.rate_ttbarPlusB     = 0.0;
  QCDscale_ttW_.rate_ttbarPlusBBbar = 0.0;
  QCDscale_ttW_.rate_ttbarPlusCCbar = 0.0;
  QCDscale_ttW_.rate_singlet        = 0.0;
  QCDscale_ttW_.rate_wjets          = 0.0;
  QCDscale_ttW_.rate_zjets          = 0.0;
  QCDscale_ttW_.rate_ttbarW         = 1.1;
  QCDscale_ttW_.rate_ttbarZ         = 0.0;
  QCDscale_ttW_.rate_diboson        = 0.0;
  

  // QCDscale_ttZ
  QCDscale_ttZ_.id                  = 4;
  QCDscale_ttZ_.type                = "lnN";
  QCDscale_ttZ_.title               = "QCD scale, t#bar{t}Z";
  QCDscale_ttZ_.label               = "QCDscale_ttZ";
  QCDscale_ttZ_.latex               = "QCD scale, t$\\bar{t}$Z";
  QCDscale_ttZ_.rate_ttH            = 0.0;
  QCDscale_ttZ_.rate_ttbar          = 0.0;
  QCDscale_ttZ_.rate_ttbarPlusB     = 0.0;
  QCDscale_ttZ_.rate_ttbarPlusBBbar = 0.0;
  QCDscale_ttZ_.rate_ttbarPlusCCbar = 0.0;
  QCDscale_ttZ_.rate_singlet        = 0.0;
  QCDscale_ttZ_.rate_wjets          = 0.0;
  QCDscale_ttZ_.rate_zjets          = 0.0;
  QCDscale_ttZ_.rate_ttbarW         = 0.0;
  QCDscale_ttZ_.rate_ttbarZ         = 1.11;
  QCDscale_ttZ_.rate_diboson        = 0.0;
  

  // QCDscale_V
  QCDscale_V_.id                  = 5;
  QCDscale_V_.type                = "lnN";
  QCDscale_V_.title               = "QCD scale, V";
  QCDscale_V_.label               = "QCDscale_V";
  QCDscale_V_.latex               = "QCD scale, V";
  QCDscale_V_.rate_ttH            = 0.0;
  QCDscale_V_.rate_ttbar          = 0.0;
  QCDscale_V_.rate_ttbarPlusB     = 0.0;
  QCDscale_V_.rate_ttbarPlusBBbar = 0.0;
  QCDscale_V_.rate_ttbarPlusCCbar = 0.0;
  QCDscale_V_.rate_singlet        = 0.0;
  QCDscale_V_.rate_wjets          = 1.013;
  QCDscale_V_.rate_zjets          = 1.012;
  QCDscale_V_.rate_ttbarW         = 0.0;
  QCDscale_V_.rate_ttbarZ         = 0.0;
  QCDscale_V_.rate_diboson        = 0.0;


  // QCDscale_VV
  QCDscale_VV_.id                  = 6;
  QCDscale_VV_.type                = "lnN";
  QCDscale_VV_.title               = "QCD scale, VV";
  QCDscale_VV_.label               = "QCDscale_VV";
  QCDscale_VV_.latex               = "QCD scale, VV";
  QCDscale_VV_.rate_ttH            = 0.0;
  QCDscale_VV_.rate_ttbar          = 0.0;
  QCDscale_VV_.rate_ttbarPlusB     = 0.0;
  QCDscale_VV_.rate_ttbarPlusBBbar = 0.0;
  QCDscale_VV_.rate_ttbarPlusCCbar = 0.0;
  QCDscale_VV_.rate_singlet        = 0.0;
  QCDscale_VV_.rate_wjets          = 0.0;
  QCDscale_VV_.rate_zjets          = 0.0;
  QCDscale_VV_.rate_ttbarW         = 0.0;
  QCDscale_VV_.rate_ttbarZ         = 0.0;
  QCDscale_VV_.rate_diboson        = 1.035;


  // pdf_gg
  pdf_gg_.id                  = 7;
  pdf_gg_.type                = "lnN";
  pdf_gg_.title               = "PDF, gluon-gluon Initiated";
  pdf_gg_.label               = "pdf_gg";
  pdf_gg_.latex               = "PDF, gluon-gluon Initiated";
  pdf_gg_.rate_ttH            = 0.92;
  pdf_gg_.rate_ttbar          = 0.974;
  pdf_gg_.rate_ttbarPlusB     = 0.974;
  pdf_gg_.rate_ttbarPlusBBbar = 0.974;
  pdf_gg_.rate_ttbarPlusCCbar = 0.974;
  pdf_gg_.rate_singlet        = 0.0;
  pdf_gg_.rate_wjets          = 0.0;
  pdf_gg_.rate_zjets          = 0.0;
  pdf_gg_.rate_ttbarW         = 0.0;
  pdf_gg_.rate_ttbarZ         = 0.91;
  pdf_gg_.rate_diboson        = 0.0;


  // pdf_qqbar
  pdf_qqbar_.id                  = 8;
  pdf_qqbar_.type                = "lnN";
  pdf_qqbar_.title               = "PDF, q#bar{q} Initiated";
  pdf_qqbar_.label               = "pdf_qqbar";
  pdf_qqbar_.latex               = "PDF, q$\\bar{q}$ Initiated";
  pdf_qqbar_.rate_ttH            = 0.0;
  pdf_qqbar_.rate_ttbar          = 0.0;
  pdf_qqbar_.rate_ttbarPlusB     = 0.0;
  pdf_qqbar_.rate_ttbarPlusBBbar = 0.0;
  pdf_qqbar_.rate_ttbarPlusCCbar = 0.0;
  pdf_qqbar_.rate_singlet        = 0.0;
  pdf_qqbar_.rate_wjets          = 1.048;
  pdf_qqbar_.rate_zjets          = 1.042;
  pdf_qqbar_.rate_ttbarW         = 1.07;
  pdf_qqbar_.rate_ttbarZ         = 0.0;
  pdf_qqbar_.rate_diboson        = 0.0;


  // pdf_qg
  pdf_qg_.id                  = 9;
  pdf_qg_.type                = "lnN";
  pdf_qg_.title               = "PDF, q-gluon Initiated";
  pdf_qg_.label               = "pdf_qg";
  pdf_qg_.latex               = "PDF, q-gluon Initiated";
  pdf_qg_.rate_ttH            = 0.0;
  pdf_qg_.rate_ttbar          = 0.0;
  pdf_qg_.rate_ttbarPlusB     = 0.0;
  pdf_qg_.rate_ttbarPlusBBbar = 0.0;
  pdf_qg_.rate_ttbarPlusCCbar = 0.0;
  pdf_qg_.rate_singlet        = 1.046;
  pdf_qg_.rate_wjets          = 0.0;
  pdf_qg_.rate_zjets          = 0.0;
  pdf_qg_.rate_ttbarW         = 0.0;
  pdf_qg_.rate_ttbarZ         = 0.0;
  pdf_qg_.rate_diboson        = 0.0;


  // CMS_ttH_pu
  CMS_ttH_pu_.id                  = 10;
  CMS_ttH_pu_.type                = "lnN";
  CMS_ttH_pu_.title               = "CMS Pileup";
  CMS_ttH_pu_.label               = "CMS_ttH_pu";
  CMS_ttH_pu_.latex               = "CMS Pileup";
  CMS_ttH_pu_.rate_ttH            = 1.01;
  CMS_ttH_pu_.rate_ttbar          = 1.01;
  CMS_ttH_pu_.rate_ttbarPlusB     = 1.01;
  CMS_ttH_pu_.rate_ttbarPlusBBbar = 1.01;
  CMS_ttH_pu_.rate_ttbarPlusCCbar = 1.01;
  CMS_ttH_pu_.rate_singlet        = 1.01;
  CMS_ttH_pu_.rate_wjets          = 1.01;
  CMS_ttH_pu_.rate_zjets          = 1.01;
  CMS_ttH_pu_.rate_ttbarW         = 1.01;
  CMS_ttH_pu_.rate_ttbarZ         = 1.01;
  CMS_ttH_pu_.rate_diboson        = 1.01;


  // CMS_res_j
  CMS_res_j_.id                  = 11;
  CMS_res_j_.type                = "lnN";
  CMS_res_j_.title               = "CMS Jet Energy Resolution";
  CMS_res_j_.label               = "CMS_res_j";
  CMS_res_j_.latex               = "CMS Jet Energy Resolution";
  CMS_res_j_.rate_ttH            = 1.015;
  CMS_res_j_.rate_ttbar          = 1.015;
  CMS_res_j_.rate_ttbarPlusB     = 1.015;
  CMS_res_j_.rate_ttbarPlusBBbar = 1.015;
  CMS_res_j_.rate_ttbarPlusCCbar = 1.015;
  CMS_res_j_.rate_singlet        = 1.015;
  CMS_res_j_.rate_wjets          = 1.015;
  CMS_res_j_.rate_zjets          = 1.015;
  CMS_res_j_.rate_ttbarW         = 1.015;
  CMS_res_j_.rate_ttbarZ         = 1.015;
  CMS_res_j_.rate_diboson        = 1.015;


  // CMS_ttH_eff_lep
  CMS_ttH_eff_lep_.id                  = 12;
  CMS_ttH_eff_lep_.type                = "lnN";
  CMS_ttH_eff_lep_.title               = "CMS Lepton Trigger and ID Efficiency";
  CMS_ttH_eff_lep_.label               = "CMS_ttH_eff_lep";
  CMS_ttH_eff_lep_.latex               = "CMS Lepton Trigger and ID Efficiency";
  CMS_ttH_eff_lep_.rate_ttH            = 1.014;
  CMS_ttH_eff_lep_.rate_ttbar          = 1.014;
  CMS_ttH_eff_lep_.rate_ttbarPlusB     = 1.014;
  CMS_ttH_eff_lep_.rate_ttbarPlusBBbar = 1.014;
  CMS_ttH_eff_lep_.rate_ttbarPlusCCbar = 1.014;
  CMS_ttH_eff_lep_.rate_singlet        = 1.014;
  CMS_ttH_eff_lep_.rate_wjets          = 1.014;
  CMS_ttH_eff_lep_.rate_zjets          = 1.014;
  CMS_ttH_eff_lep_.rate_ttbarW         = 1.014;
  CMS_ttH_eff_lep_.rate_ttbarZ         = 1.014;
  CMS_ttH_eff_lep_.rate_diboson        = 1.014;


  // Q2scale_ttH_ttbar0p
  Q2scale_ttH_ttbar0p_.id                  = 13;
  Q2scale_ttH_ttbar0p_.type                = "shape";
  Q2scale_ttH_ttbar0p_.title               = "Q^{2} scale uncercentainty, t#bar{t}+0 additional partons";
  Q2scale_ttH_ttbar0p_.label               = "Q2scale_ttH_ttbar0p";
  Q2scale_ttH_ttbar0p_.latex               = "$Q^{2}$ scale uncercentainty, t$\\bar{t}$+0 additional partons";
  Q2scale_ttH_ttbar0p_.treeIndexUp         = 13;
  Q2scale_ttH_ttbar0p_.treeIndexDown       = 14;
  Q2scale_ttH_ttbar0p_.rate_ttH            = 0.0;
  Q2scale_ttH_ttbar0p_.rate_ttbar          = 1;
  Q2scale_ttH_ttbar0p_.rate_ttbarPlusB     = 0.0;
  Q2scale_ttH_ttbar0p_.rate_ttbarPlusBBbar = 0.0;
  Q2scale_ttH_ttbar0p_.rate_ttbarPlusCCbar = 0.0;
  Q2scale_ttH_ttbar0p_.rate_singlet        = 0.0;
  Q2scale_ttH_ttbar0p_.rate_wjets          = 0.0;
  Q2scale_ttH_ttbar0p_.rate_zjets          = 0.0;
  Q2scale_ttH_ttbar0p_.rate_ttbarW         = 0.0;
  Q2scale_ttH_ttbar0p_.rate_ttbarZ         = 0.0;
  Q2scale_ttH_ttbar0p_.rate_diboson        = 0.0;

  
  // Q2scale_ttH_ttbar1p
  Q2scale_ttH_ttbar1p_.id                  = 14;
  Q2scale_ttH_ttbar1p_.type                = "shape";
  Q2scale_ttH_ttbar1p_.title               = "Q^{2} scale uncercentainty, t#bar{t}+1 additional parton";
  Q2scale_ttH_ttbar1p_.label               = "Q2scale_ttH_ttbar1p";
  Q2scale_ttH_ttbar1p_.latex               = "$Q^{2}$ scale uncercentainty, t$\\bar{t}$+1 additional parton";
  Q2scale_ttH_ttbar1p_.treeIndexUp         = 13;
  Q2scale_ttH_ttbar1p_.treeIndexDown       = 14;
  Q2scale_ttH_ttbar1p_.rate_ttH            = 0.0;
  Q2scale_ttH_ttbar1p_.rate_ttbar          = 1;
  Q2scale_ttH_ttbar1p_.rate_ttbarPlusB     = 0.0;
  Q2scale_ttH_ttbar1p_.rate_ttbarPlusBBbar = 0.0;
  Q2scale_ttH_ttbar1p_.rate_ttbarPlusCCbar = 0.0;
  Q2scale_ttH_ttbar1p_.rate_singlet        = 0.0;
  Q2scale_ttH_ttbar1p_.rate_wjets          = 0.0;
  Q2scale_ttH_ttbar1p_.rate_zjets          = 0.0;
  Q2scale_ttH_ttbar1p_.rate_ttbarW         = 0.0;
  Q2scale_ttH_ttbar1p_.rate_ttbarZ         = 0.0;
  Q2scale_ttH_ttbar1p_.rate_diboson        = 0.0;


  // Q2scale_ttH_ttbar2p
  Q2scale_ttH_ttbar2p_.id                  = 15;
  Q2scale_ttH_ttbar2p_.type                = "shape";
  Q2scale_ttH_ttbar2p_.title               = "Q^{2} scale uncercentainty, t#bar{t}+2 additional partons";
  Q2scale_ttH_ttbar2p_.label               = "Q2scale_ttH_ttbar2p";
  Q2scale_ttH_ttbar2p_.latex               = "$Q^{2}$ scale uncercentainty, t$\\bar{t}$+2 additional partons";
  Q2scale_ttH_ttbar2p_.treeIndexUp         = 13;
  Q2scale_ttH_ttbar2p_.treeIndexDown       = 14;
  Q2scale_ttH_ttbar2p_.rate_ttH            = 0.0;
  Q2scale_ttH_ttbar2p_.rate_ttbar          = 1;
  Q2scale_ttH_ttbar2p_.rate_ttbarPlusB     = 0.0;
  Q2scale_ttH_ttbar2p_.rate_ttbarPlusBBbar = 0.0;
  Q2scale_ttH_ttbar2p_.rate_ttbarPlusCCbar = 0.0;
  Q2scale_ttH_ttbar2p_.rate_singlet        = 0.0;
  Q2scale_ttH_ttbar2p_.rate_wjets          = 0.0;
  Q2scale_ttH_ttbar2p_.rate_zjets          = 0.0;
  Q2scale_ttH_ttbar2p_.rate_ttbarW         = 0.0;
  Q2scale_ttH_ttbar2p_.rate_ttbarZ         = 0.0;
  Q2scale_ttH_ttbar2p_.rate_diboson        = 0.0;


  // Q2scale_ttH_ttbar_b
  Q2scale_ttH_ttbar_b_.id                  = 16;
  Q2scale_ttH_ttbar_b_.type                = "shape";
  Q2scale_ttH_ttbar_b_.title               = "Q^{2} scale uncercentainty, t#bar{t}+b partons";
  Q2scale_ttH_ttbar_b_.label               = "Q2scale_ttH_ttbar_b";
  Q2scale_ttH_ttbar_b_.latex               = "$Q^{2}$ scale uncercentainty, t$\\bar{t}$+b partons";
  Q2scale_ttH_ttbar_b_.treeIndexUp         = 13;
  Q2scale_ttH_ttbar_b_.treeIndexDown       = 14;
  Q2scale_ttH_ttbar_b_.rate_ttH            = 0.0;
  Q2scale_ttH_ttbar_b_.rate_ttbar          = 0.0;
  Q2scale_ttH_ttbar_b_.rate_ttbarPlusB     = 1;
  Q2scale_ttH_ttbar_b_.rate_ttbarPlusBBbar = 0.0;
  Q2scale_ttH_ttbar_b_.rate_ttbarPlusCCbar = 0.0;
  Q2scale_ttH_ttbar_b_.rate_singlet        = 0.0;
  Q2scale_ttH_ttbar_b_.rate_wjets          = 0.0;
  Q2scale_ttH_ttbar_b_.rate_zjets          = 0.0;
  Q2scale_ttH_ttbar_b_.rate_ttbarW         = 0.0;
  Q2scale_ttH_ttbar_b_.rate_ttbarZ         = 0.0;
  Q2scale_ttH_ttbar_b_.rate_diboson        = 0.0;


  // Q2scale_ttH_ttbar_bb
  Q2scale_ttH_ttbar_bb_.id                  = 17;
  Q2scale_ttH_ttbar_bb_.type                = "shape";
  Q2scale_ttH_ttbar_bb_.title               = "Q^{2} scale uncercentainty, t#bar{t}+b#bar{b} partons";
  Q2scale_ttH_ttbar_bb_.label               = "Q2scale_ttH_ttbar_bb";
  Q2scale_ttH_ttbar_bb_.latex               = "$Q^{2}$ scale uncercentainty, t$\\bar{t}$+b$\\bar{b}$ partons";
  Q2scale_ttH_ttbar_bb_.treeIndexUp         = 13;
  Q2scale_ttH_ttbar_bb_.treeIndexDown       = 14;
  Q2scale_ttH_ttbar_bb_.rate_ttH            = 0.0;
  Q2scale_ttH_ttbar_bb_.rate_ttbar          = 0.0;
  Q2scale_ttH_ttbar_bb_.rate_ttbarPlusB     = 0.0;
  Q2scale_ttH_ttbar_bb_.rate_ttbarPlusBBbar = 1;
  Q2scale_ttH_ttbar_bb_.rate_ttbarPlusCCbar = 0.0;
  Q2scale_ttH_ttbar_bb_.rate_singlet        = 0.0;
  Q2scale_ttH_ttbar_bb_.rate_wjets          = 0.0;
  Q2scale_ttH_ttbar_bb_.rate_zjets          = 0.0;
  Q2scale_ttH_ttbar_bb_.rate_ttbarW         = 0.0;
  Q2scale_ttH_ttbar_bb_.rate_ttbarZ         = 0.0;
  Q2scale_ttH_ttbar_bb_.rate_diboson        = 0.0;


  // Q2scale_ttH_ttbar_cc
  Q2scale_ttH_ttbar_cc_.id                  = 18;
  Q2scale_ttH_ttbar_cc_.type                = "shape";
  Q2scale_ttH_ttbar_cc_.title               = "Q^{2} scale uncercentainty, t#bar{t}+c#bar{c} partons";
  Q2scale_ttH_ttbar_cc_.label               = "Q2scale_ttH_ttbar_cc";
  Q2scale_ttH_ttbar_cc_.latex               = "$Q^{2}$ scale uncercentainty, t$\\bar{t}$+c$\\bar{c}$ partons";
  Q2scale_ttH_ttbar_cc_.treeIndexUp         = 13;
  Q2scale_ttH_ttbar_cc_.treeIndexDown       = 14;
  Q2scale_ttH_ttbar_cc_.rate_ttH            = 0.0;
  Q2scale_ttH_ttbar_cc_.rate_ttbar          = 0.0;
  Q2scale_ttH_ttbar_cc_.rate_ttbarPlusB     = 0.0;
  Q2scale_ttH_ttbar_cc_.rate_ttbarPlusBBbar = 0.0;
  Q2scale_ttH_ttbar_cc_.rate_ttbarPlusCCbar = 1;
  Q2scale_ttH_ttbar_cc_.rate_singlet        = 0.0;
  Q2scale_ttH_ttbar_cc_.rate_wjets          = 0.0;
  Q2scale_ttH_ttbar_cc_.rate_zjets          = 0.0;
  Q2scale_ttH_ttbar_cc_.rate_ttbarW         = 0.0;
  Q2scale_ttH_ttbar_cc_.rate_ttbarZ         = 0.0;
  Q2scale_ttH_ttbar_cc_.rate_diboson        = 0.0;


  // CMS_ttH_CSVLF
  CMS_ttH_CSVLF_.id                  = 19;
  CMS_ttH_CSVLF_.type                = "shape";
  CMS_ttH_CSVLF_.title               = "CSV b-tagging algorithm, LF uncertainty";
  CMS_ttH_CSVLF_.label               = "CMS_ttH_CSVLF";
  CMS_ttH_CSVLF_.latex               = "CSV b-tagging algorithm, LF uncertainty";
  CMS_ttH_CSVLF_.treeIndexUp         = 17;
  CMS_ttH_CSVLF_.treeIndexDown       = 18;
  CMS_ttH_CSVLF_.rate_ttH            = 1;
  CMS_ttH_CSVLF_.rate_ttbar          = 1;
  CMS_ttH_CSVLF_.rate_ttbarPlusB     = 1;
  CMS_ttH_CSVLF_.rate_ttbarPlusBBbar = 1;
  CMS_ttH_CSVLF_.rate_ttbarPlusCCbar = 1;
  CMS_ttH_CSVLF_.rate_singlet        = 1;
  CMS_ttH_CSVLF_.rate_wjets          = 1;
  CMS_ttH_CSVLF_.rate_zjets          = 1;
  CMS_ttH_CSVLF_.rate_ttbarW         = 1;
  CMS_ttH_CSVLF_.rate_ttbarZ         = 1;
  CMS_ttH_CSVLF_.rate_diboson        = 1;


  // CMS_ttH_CSVHF
  CMS_ttH_CSVHF_.id                  = 20;
  CMS_ttH_CSVHF_.type                = "shape";
  CMS_ttH_CSVHF_.title               = "CSV b-tagging algorithm, HF uncertainty";
  CMS_ttH_CSVHF_.label               = "CMS_ttH_CSVHF";
  CMS_ttH_CSVHF_.latex               = "CSV b-tagging algorithm, HF uncertainty";
  CMS_ttH_CSVHF_.treeIndexUp         = 19;
  CMS_ttH_CSVHF_.treeIndexDown       = 20;
  CMS_ttH_CSVHF_.rate_ttH            = 1;
  CMS_ttH_CSVHF_.rate_ttbar          = 1;
  CMS_ttH_CSVHF_.rate_ttbarPlusB     = 1;
  CMS_ttH_CSVHF_.rate_ttbarPlusBBbar = 1;
  CMS_ttH_CSVHF_.rate_ttbarPlusCCbar = 1;
  CMS_ttH_CSVHF_.rate_singlet        = 1;
  CMS_ttH_CSVHF_.rate_wjets          = 1;
  CMS_ttH_CSVHF_.rate_zjets          = 1;
  CMS_ttH_CSVHF_.rate_ttbarW         = 1;
  CMS_ttH_CSVHF_.rate_ttbarZ         = 1;
  CMS_ttH_CSVHF_.rate_diboson        = 1;


  // CMS_ttH_CSVLFStats1
  CMS_ttH_CSVLFStats1_.id                  = 21;
  CMS_ttH_CSVLFStats1_.type                = "shape";
  CMS_ttH_CSVLFStats1_.title               = "CSV b-tagging algorithm, LF Statistical uncertainty, version 1";
  CMS_ttH_CSVLFStats1_.label               = "CMS_ttH_CSVLFStats1";
  CMS_ttH_CSVLFStats1_.latex               = "CSV b-tagging algorithm, LF Statistical uncertainty, version 1";
  CMS_ttH_CSVLFStats1_.treeIndexUp         = 23;
  CMS_ttH_CSVLFStats1_.treeIndexDown       = 24;
  CMS_ttH_CSVLFStats1_.rate_ttH            = 1;
  CMS_ttH_CSVLFStats1_.rate_ttbar          = 1;
  CMS_ttH_CSVLFStats1_.rate_ttbarPlusB     = 1;
  CMS_ttH_CSVLFStats1_.rate_ttbarPlusBBbar = 1;
  CMS_ttH_CSVLFStats1_.rate_ttbarPlusCCbar = 1;
  CMS_ttH_CSVLFStats1_.rate_singlet        = 1;
  CMS_ttH_CSVLFStats1_.rate_wjets          = 1;
  CMS_ttH_CSVLFStats1_.rate_zjets          = 1;
  CMS_ttH_CSVLFStats1_.rate_ttbarW         = 1;
  CMS_ttH_CSVLFStats1_.rate_ttbarZ         = 1;
  CMS_ttH_CSVLFStats1_.rate_diboson        = 1;


  // CMS_ttH_CSVHFStats1
  CMS_ttH_CSVHFStats1_.id                  = 22;
  CMS_ttH_CSVHFStats1_.type                = "shape";
  CMS_ttH_CSVHFStats1_.title               = "CSV b-tagging algorithm, HF Statistical uncertainty, version 1";
  CMS_ttH_CSVHFStats1_.label               = "CMS_ttH_CSVHFStats1";
  CMS_ttH_CSVHFStats1_.latex               = "CSV b-tagging algorithm, HF Statistical uncertainty, version 1";
  CMS_ttH_CSVHFStats1_.treeIndexUp         = 21;
  CMS_ttH_CSVHFStats1_.treeIndexDown       = 22;
  CMS_ttH_CSVHFStats1_.rate_ttH            = 1;
  CMS_ttH_CSVHFStats1_.rate_ttbar          = 1;
  CMS_ttH_CSVHFStats1_.rate_ttbarPlusB     = 1;
  CMS_ttH_CSVHFStats1_.rate_ttbarPlusBBbar = 1;
  CMS_ttH_CSVHFStats1_.rate_ttbarPlusCCbar = 1;
  CMS_ttH_CSVHFStats1_.rate_singlet        = 1;
  CMS_ttH_CSVHFStats1_.rate_wjets          = 1;
  CMS_ttH_CSVHFStats1_.rate_zjets          = 1;
  CMS_ttH_CSVHFStats1_.rate_ttbarW         = 1;
  CMS_ttH_CSVHFStats1_.rate_ttbarZ         = 1;
  CMS_ttH_CSVHFStats1_.rate_diboson        = 1;


  // CMS_ttH_CSVLFStats2
  CMS_ttH_CSVLFStats2_.id                  = 23;
  CMS_ttH_CSVLFStats2_.type                = "shape";
  CMS_ttH_CSVLFStats2_.title               = "CSV b-tagging algorithm, LF Statistical uncertainty, version 2";
  CMS_ttH_CSVLFStats2_.label               = "CMS_ttH_CSVLFStats2";
  CMS_ttH_CSVLFStats2_.latex               = "CSV b-tagging algorithm, LF Statistical uncertainty, version 2";
  CMS_ttH_CSVLFStats2_.treeIndexUp         = 27;
  CMS_ttH_CSVLFStats2_.treeIndexDown       = 28;
  CMS_ttH_CSVLFStats2_.rate_ttH            = 1;
  CMS_ttH_CSVLFStats2_.rate_ttbar          = 1;
  CMS_ttH_CSVLFStats2_.rate_ttbarPlusB     = 1;
  CMS_ttH_CSVLFStats2_.rate_ttbarPlusBBbar = 1;
  CMS_ttH_CSVLFStats2_.rate_ttbarPlusCCbar = 1;
  CMS_ttH_CSVLFStats2_.rate_singlet        = 1;
  CMS_ttH_CSVLFStats2_.rate_wjets          = 1;
  CMS_ttH_CSVLFStats2_.rate_zjets          = 1;
  CMS_ttH_CSVLFStats2_.rate_ttbarW         = 1;
  CMS_ttH_CSVLFStats2_.rate_ttbarZ         = 1;
  CMS_ttH_CSVLFStats2_.rate_diboson        = 1;


  // CMS_ttH_CSVHFStats2
  CMS_ttH_CSVHFStats2_.id                  = 24;
  CMS_ttH_CSVHFStats2_.type                = "shape";
  CMS_ttH_CSVHFStats2_.title               = "CSV b-tagging algorithm, HF Statistical uncertainty, version 2";
  CMS_ttH_CSVHFStats2_.label               = "CMS_ttH_CSVHFStats2";
  CMS_ttH_CSVHFStats2_.latex               = "CSV b-tagging algorithm, HF Statistical uncertainty, version 2";
  CMS_ttH_CSVHFStats2_.treeIndexUp         = 25;
  CMS_ttH_CSVHFStats2_.treeIndexDown       = 26;
  CMS_ttH_CSVHFStats2_.rate_ttH            = 1;
  CMS_ttH_CSVHFStats2_.rate_ttbar          = 1;
  CMS_ttH_CSVHFStats2_.rate_ttbarPlusB     = 1;
  CMS_ttH_CSVHFStats2_.rate_ttbarPlusBBbar = 1;
  CMS_ttH_CSVHFStats2_.rate_ttbarPlusCCbar = 1;
  CMS_ttH_CSVHFStats2_.rate_singlet        = 1;
  CMS_ttH_CSVHFStats2_.rate_wjets          = 1;
  CMS_ttH_CSVHFStats2_.rate_zjets          = 1;
  CMS_ttH_CSVHFStats2_.rate_ttbarW         = 1;
  CMS_ttH_CSVHFStats2_.rate_ttbarZ         = 1;
  CMS_ttH_CSVHFStats2_.rate_diboson        = 1;


  // CMS_ttH_CSVCErr1
  CMS_ttH_CSVCErr1_.id                  = 25;
  CMS_ttH_CSVCErr1_.type                = "shape";
  CMS_ttH_CSVCErr1_.title               = "CSV b-tagging algorithm, Charm uncertainty, version 1";
  CMS_ttH_CSVCErr1_.label               = "CMS_ttH_CSVCErr1";
  CMS_ttH_CSVCErr1_.latex               = "CSV b-tagging algorithm, Charm uncertainty, version 1";
  CMS_ttH_CSVCErr1_.treeIndexUp         = 29;
  CMS_ttH_CSVCErr1_.treeIndexDown       = 30;
  CMS_ttH_CSVCErr1_.rate_ttH            = 1;
  CMS_ttH_CSVCErr1_.rate_ttbar          = 1;
  CMS_ttH_CSVCErr1_.rate_ttbarPlusB     = 1;
  CMS_ttH_CSVCErr1_.rate_ttbarPlusBBbar = 1;
  CMS_ttH_CSVCErr1_.rate_ttbarPlusCCbar = 1;
  CMS_ttH_CSVCErr1_.rate_singlet        = 1;
  CMS_ttH_CSVCErr1_.rate_wjets          = 1;
  CMS_ttH_CSVCErr1_.rate_zjets          = 1;
  CMS_ttH_CSVCErr1_.rate_ttbarW         = 1;
  CMS_ttH_CSVCErr1_.rate_ttbarZ         = 1;
  CMS_ttH_CSVCErr1_.rate_diboson        = 1;


  // CMS_ttH_CSVCErr2
  CMS_ttH_CSVCErr2_.id                  = 26;
  CMS_ttH_CSVCErr2_.type                = "shape";
  CMS_ttH_CSVCErr2_.title               = "CSV b-tagging algorithm, Charm uncertainty, version 2";
  CMS_ttH_CSVCErr2_.label               = "CMS_ttH_CSVCErr2";
  CMS_ttH_CSVCErr2_.latex               = "CSV b-tagging algorithm, Charm uncertainty, version 2";
  CMS_ttH_CSVCErr2_.treeIndexUp         = 31;
  CMS_ttH_CSVCErr2_.treeIndexDown       = 32;
  CMS_ttH_CSVCErr2_.rate_ttH            = 1;
  CMS_ttH_CSVCErr2_.rate_ttbar          = 1;
  CMS_ttH_CSVCErr2_.rate_ttbarPlusB     = 1;
  CMS_ttH_CSVCErr2_.rate_ttbarPlusBBbar = 1;
  CMS_ttH_CSVCErr2_.rate_ttbarPlusCCbar = 1;
  CMS_ttH_CSVCErr2_.rate_singlet        = 1;
  CMS_ttH_CSVCErr2_.rate_wjets          = 1;
  CMS_ttH_CSVCErr2_.rate_zjets          = 1;
  CMS_ttH_CSVCErr2_.rate_ttbarW         = 1;
  CMS_ttH_CSVCErr2_.rate_ttbarZ         = 1;
  CMS_ttH_CSVCErr2_.rate_diboson        = 1;


  // CMS Jet Energy Scaling Uncertainty
  CMS_scale_j_.id                  = 27;
  CMS_scale_j_.type                = "shape";
  CMS_scale_j_.title               = "CMS Jet Energy Scaling Uncertainty";
  CMS_scale_j_.label               = "CMS_scale_j";
  CMS_scale_j_.latex               = "CMS Jet Energy Scaling Uncertainty";
  CMS_scale_j_.treeIndexUp         = 11;
  CMS_scale_j_.treeIndexDown       = 12;
  CMS_scale_j_.rate_ttH            = 1;
  CMS_scale_j_.rate_ttbar          = 1;
  CMS_scale_j_.rate_ttbarPlusB     = 1;
  CMS_scale_j_.rate_ttbarPlusBBbar = 1;
  CMS_scale_j_.rate_ttbarPlusCCbar = 1;
  CMS_scale_j_.rate_singlet        = 1;
  CMS_scale_j_.rate_wjets          = 1;
  CMS_scale_j_.rate_zjets          = 1;
  CMS_scale_j_.rate_ttbarW         = 1;
  CMS_scale_j_.rate_ttbarZ         = 1;
  CMS_scale_j_.rate_diboson        = 1;


  // CMS Top pT Spectrum Correction Uncertainty
  CMS_ttH_topPtcorr_.id                  = 28;
  CMS_ttH_topPtcorr_.type                = "shape";
  CMS_ttH_topPtcorr_.title               = "CMS top pT correction uncertainty";
  CMS_ttH_topPtcorr_.label               = "CMS_ttH_topPtcorr";
  CMS_ttH_topPtcorr_.latex               = "CMS top pT correction uncertainty";
  CMS_ttH_topPtcorr_.treeIndexUp         = 15;
  CMS_ttH_topPtcorr_.treeIndexDown       = 16;
  CMS_ttH_topPtcorr_.rate_ttH            = 1;
  CMS_ttH_topPtcorr_.rate_ttbar          = 1;
  CMS_ttH_topPtcorr_.rate_ttbarPlusB     = 1;
  CMS_ttH_topPtcorr_.rate_ttbarPlusBBbar = 1;
  CMS_ttH_topPtcorr_.rate_ttbarPlusCCbar = 1;
  CMS_ttH_topPtcorr_.rate_singlet        = 1;
  CMS_ttH_topPtcorr_.rate_wjets          = 1;
  CMS_ttH_topPtcorr_.rate_zjets          = 1;
  CMS_ttH_topPtcorr_.rate_ttbarW         = 1;
  CMS_ttH_topPtcorr_.rate_ttbarZ         = 1;
  CMS_ttH_topPtcorr_.rate_diboson        = 1;


  // Additional Normalization Uncertianty for tt+b
  CMS_ttH_QCDscale_ttb_.id                  = 29;
  CMS_ttH_QCDscale_ttb_.type                = "lnN";
  CMS_ttH_QCDscale_ttb_.title               = "Additional Cross-Section Uncertainty for t#bar{t}+b";
  CMS_ttH_QCDscale_ttb_.label               = "CMS_ttH_QCDscale_ttb";
  CMS_ttH_QCDscale_ttb_.latex               = "Additional Cross-Section Uncertainty for t$\\bar{t}$+b";
  CMS_ttH_QCDscale_ttb_.rate_ttH            = 0.0;
  CMS_ttH_QCDscale_ttb_.rate_ttbar          = 0.0;
  CMS_ttH_QCDscale_ttb_.rate_ttbarPlusB     = 1.5;
  CMS_ttH_QCDscale_ttb_.rate_ttbarPlusBBbar = 0.0;
  CMS_ttH_QCDscale_ttb_.rate_ttbarPlusCCbar = 0.0;
  CMS_ttH_QCDscale_ttb_.rate_singlet        = 0.0;
  CMS_ttH_QCDscale_ttb_.rate_wjets          = 0.0;
  CMS_ttH_QCDscale_ttb_.rate_zjets          = 0.0;
  CMS_ttH_QCDscale_ttb_.rate_ttbarW         = 0.0;
  CMS_ttH_QCDscale_ttb_.rate_ttbarZ         = 0.0;
  CMS_ttH_QCDscale_ttb_.rate_diboson        = 0.0;


  // Additional Normalization Uncertianty for tt+bb
  CMS_ttH_QCDscale_ttbb_.id                  = 29;
  CMS_ttH_QCDscale_ttbb_.type                = "lnN";
  CMS_ttH_QCDscale_ttbb_.title               = "Additional Cross-Section Uncertainty for t#bar{t}+b#bar{b}";
  CMS_ttH_QCDscale_ttbb_.label               = "CMS_ttH_QCDscale_ttbb";
  CMS_ttH_QCDscale_ttbb_.latex               = "Additional Cross-Section Uncertainty for t$\\bar{t}$+b$\\bar{b}$";
  CMS_ttH_QCDscale_ttbb_.rate_ttH            = 0.0;
  CMS_ttH_QCDscale_ttbb_.rate_ttbar          = 0.0;
  CMS_ttH_QCDscale_ttbb_.rate_ttbarPlusB     = 0.0;
  CMS_ttH_QCDscale_ttbb_.rate_ttbarPlusBBbar = 1.5;
  CMS_ttH_QCDscale_ttbb_.rate_ttbarPlusCCbar = 0.0;
  CMS_ttH_QCDscale_ttbb_.rate_singlet        = 0.0;
  CMS_ttH_QCDscale_ttbb_.rate_wjets          = 0.0;
  CMS_ttH_QCDscale_ttbb_.rate_zjets          = 0.0;
  CMS_ttH_QCDscale_ttbb_.rate_ttbarW         = 0.0;
  CMS_ttH_QCDscale_ttbb_.rate_ttbarZ         = 0.0;
  CMS_ttH_QCDscale_ttbb_.rate_diboson        = 0.0;


  // Additional Normalization Uncertianty for tt+cc
  CMS_ttH_QCDscale_ttcc_.id                  = 30;
  CMS_ttH_QCDscale_ttcc_.type                = "lnN";
  CMS_ttH_QCDscale_ttcc_.title               = "Additional Cross-Section Uncertainty for t#bar{t}+c#bar{c}";
  CMS_ttH_QCDscale_ttcc_.label               = "CMS_ttH_QCDscale_ttcc";
  CMS_ttH_QCDscale_ttcc_.latex               = "Additional Cross-Section Uncertainty for t$\\bar{t}$+c$\\bar{c}$";
  CMS_ttH_QCDscale_ttcc_.rate_ttH            = 0.0;
  CMS_ttH_QCDscale_ttcc_.rate_ttbar          = 0.0;
  CMS_ttH_QCDscale_ttcc_.rate_ttbarPlusB     = 0.0;
  CMS_ttH_QCDscale_ttcc_.rate_ttbarPlusBBbar = 0.0;
  CMS_ttH_QCDscale_ttcc_.rate_ttbarPlusCCbar = 1.5;
  CMS_ttH_QCDscale_ttcc_.rate_singlet        = 0.0;
  CMS_ttH_QCDscale_ttcc_.rate_wjets          = 0.0;
  CMS_ttH_QCDscale_ttcc_.rate_zjets          = 0.0;
  CMS_ttH_QCDscale_ttcc_.rate_ttbarW         = 0.0;
  CMS_ttH_QCDscale_ttcc_.rate_ttbarZ         = 0.0;
  CMS_ttH_QCDscale_ttcc_.rate_diboson        = 0.0;



  //
  // Fill Systematics List Containers
  //
  systematicList_.push_back(luminosity_);
  systematicList_.push_back(QCDscale_ttH_);
  systematicList_.push_back(QCDscale_ttbar_);
  systematicList_.push_back(QCDscale_ttW_);
  systematicList_.push_back(QCDscale_ttZ_);
  systematicList_.push_back(QCDscale_V_);
  systematicList_.push_back(QCDscale_VV_);
  systematicList_.push_back(pdf_gg_);
  systematicList_.push_back(pdf_qqbar_);
  systematicList_.push_back(pdf_qg_);
  systematicList_.push_back(CMS_ttH_pu_);
  systematicList_.push_back(CMS_res_j_);
  systematicList_.push_back(CMS_ttH_eff_lep_);
  systematicList_.push_back(Q2scale_ttH_ttbar0p_);
  systematicList_.push_back(Q2scale_ttH_ttbar1p_);
  systematicList_.push_back(Q2scale_ttH_ttbar2p_);
  systematicList_.push_back(Q2scale_ttH_ttbar_b_);
  systematicList_.push_back(Q2scale_ttH_ttbar_bb_);
  systematicList_.push_back(Q2scale_ttH_ttbar_cc_);
  systematicList_.push_back(CMS_ttH_CSVLF_);
  systematicList_.push_back(CMS_ttH_CSVHF_);
  systematicList_.push_back(CMS_ttH_CSVLFStats1_);
  systematicList_.push_back(CMS_ttH_CSVHFStats1_);
  systematicList_.push_back(CMS_ttH_CSVLFStats2_);
  systematicList_.push_back(CMS_ttH_CSVHFStats2_);
  systematicList_.push_back(CMS_ttH_CSVCErr1_);
  systematicList_.push_back(CMS_ttH_CSVCErr2_);
  systematicList_.push_back(CMS_scale_j_);
  systematicList_.push_back(CMS_ttH_topPtcorr_);
  systematicList_.push_back(CMS_ttH_QCDscale_ttb_);
  systematicList_.push_back(CMS_ttH_QCDscale_ttbb_);
  systematicList_.push_back(CMS_ttH_QCDscale_ttcc_);


  return;
}


// Destructor
analysisInfo_CMS_ttH_LJ::~analysisInfo_CMS_ttH_LJ(){

  //
  // Clear containers
  //
  categoryList_.clear();
  sampleList_trees_.clear();
  sampleList_histos_.clear();
  sampleList_plots_.clear();
  sampleList_limits_.clear();
  systematicList_.clear();

  return;
}
