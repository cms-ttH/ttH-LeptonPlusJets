#ifndef ttH_LeptonPlusJets_AnalysisCode_analysisInfo_CMS_ttH_LJ_h
#define ttH_LeptonPlusJets_AnalysisCode_analysisInfo_CMS_ttH_LJ_h


#include <string>

#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "AnalysisCode/LeptonPlusJets/interface/categoryInfo.h"
#include "AnalysisCode/LeptonPlusJets/interface/sampleInfo.h"
#include "AnalysisCode/LeptonPlusJets/interface/systematicInfo.h"

#endif



/////////////////////
//
// Class Definitions
//
/////////////////////

//
// Base class containing information for this analysis
//

class analysisInfo_CMS_ttH_LJ{

 public:
  

  // Cateogry List
  vcategoryInfo categoryList_;


  // Categories
  categoryInfo ljets_ee4j_ee2t_;
  categoryInfo ljets_ee5j_ee2t_;
  categoryInfo ljets_ge6j_ee2t_;
  categoryInfo ljets_ee4j_ee3t_;
  categoryInfo ljets_ee5j_ee3t_;
  categoryInfo ljets_ge6j_ee3t_;
  categoryInfo ljets_ee4j_ee4t_;
  categoryInfo ljets_ee5j_ge4t_;
  categoryInfo ljets_ge6j_ge4t_;


  // Sample Path Info
  std::string path_skim_;
  std::string path_tree_;
  std::string path_histo_;
  std::string path_skim_nlo_;
  std::string path_tree_nlo_;
  std::string path_histo_nlo_;


  // Sample List
  vsampleInfo sampleList_trees_;
  vsampleInfo sampleList_histos_;
  vsampleInfo sampleList_plots_;
  vsampleInfo sampleList_limits_;


  // Samples
  sampleInfo ttjets_NLO_fxfx4fs0j_;
  sampleInfo ttjets_NLO_fxfx4fs1j_;

  sampleInfo ttjets_lf_hadronic_;
  sampleInfo ttjets_lf_semiLep_;
  sampleInfo ttjets_lf_diLep_;
  sampleInfo ttjets_lf_;
  
  sampleInfo ttjets_cc_hadronic_;
  sampleInfo ttjets_cc_semiLep_;
  sampleInfo ttjets_cc_diLep_;
  sampleInfo ttjets_cc_;
  
  sampleInfo ttjets_b_hadronic_;
  sampleInfo ttjets_b_semiLep_;
  sampleInfo ttjets_b_diLep_;
  sampleInfo ttjets_b_;
  
  sampleInfo ttjets_bb_hadronic_;
  sampleInfo ttjets_bb_semiLep_;
  sampleInfo ttjets_bb_diLep_;
  sampleInfo ttjets_bb_;
  
  sampleInfo ttjets_1and2b_;
  sampleInfo ttjets_incl_;

  sampleInfo ttW_;
  sampleInfo ttZ_;
  sampleInfo ttV_;
 
  sampleInfo singlet_s_;
  sampleInfo singlet_t_;
  sampleInfo singlet_tW_;
  sampleInfo singletbar_s_;
  sampleInfo singletbar_t_;
  sampleInfo singletbar_tW_;
  sampleInfo singlet_;
  
  sampleInfo w1jets_;
  sampleInfo w2jets_;
  sampleInfo w3jets_;
  sampleInfo w4jets_;
  sampleInfo wjets_;
    
  sampleInfo zjets_lowMass_;
  sampleInfo z1jets_;
  sampleInfo z2jets_;
  sampleInfo z3jets_;
  sampleInfo z4jets_;
  sampleInfo zjets_;
  
  sampleInfo vjets_;

  sampleInfo ww_;
  sampleInfo wz_;
  sampleInfo zz_;
  sampleInfo diboson_;

  sampleInfo ewk_;
  
  sampleInfo ttH_M125_;
  sampleInfo ttH_M125_hbb_;
  sampleInfo ttH_M125_hcc_;
  sampleInfo ttH_M125_hww_;
  sampleInfo ttH_M125_hzz_;
  sampleInfo ttH_M125_htt_;
  sampleInfo ttH_M125_hgg_;
  sampleInfo ttH_M125_hjj_;
  sampleInfo ttH_M125_hzg_;

  sampleInfo data_singleMu_2012ABCD_;
  sampleInfo data_singleEle_2012ABCD_;
  sampleInfo data_singleLep_2012ABCD_;


  // Systematic List
  vsystematicInfo systematicList_;

  // Systematics
  systematicInfo luminosity_;
  systematicInfo QCDscale_ttH_;
  systematicInfo QCDscale_ttbar_;
  systematicInfo QCDscale_ttW_;
  systematicInfo QCDscale_ttZ_;
  systematicInfo QCDscale_V_;
  systematicInfo QCDscale_VV_;
  systematicInfo pdf_gg_;
  systematicInfo pdf_qqbar_;
  systematicInfo pdf_qg_;
  systematicInfo CMS_ttH_pu_;
  systematicInfo CMS_res_j_;
  systematicInfo CMS_ttH_eff_lep_;
  systematicInfo Q2scale_ttH_ttbar0p_;
  systematicInfo Q2scale_ttH_ttbar1p_;
  systematicInfo Q2scale_ttH_ttbar2p_;
  systematicInfo Q2scale_ttH_ttbar_b_;
  systematicInfo Q2scale_ttH_ttbar_bb_;
  systematicInfo Q2scale_ttH_ttbar_cc_;
  systematicInfo CMS_ttH_CSVLF_;
  systematicInfo CMS_ttH_CSVHF_;
  systematicInfo CMS_ttH_CSVLFStats1_;
  systematicInfo CMS_ttH_CSVHFStats1_;
  systematicInfo CMS_ttH_CSVLFStats2_;
  systematicInfo CMS_ttH_CSVHFStats2_;
  systematicInfo CMS_ttH_CSVCErr1_;
  systematicInfo CMS_ttH_CSVCErr2_;
  systematicInfo CMS_scale_j_;
  systematicInfo CMS_ttH_topPtcorr_;
  systematicInfo CMS_ttH_QCDscale_ttb_;
  systematicInfo CMS_ttH_QCDscale_ttbb_;
  systematicInfo CMS_ttH_QCDscale_ttcc_;
  
    
  analysisInfo_CMS_ttH_LJ();
  virtual ~analysisInfo_CMS_ttH_LJ();

}; // End of Class Prototype


#endif // _analysisInfo_CMS_ttH_LJ_h
