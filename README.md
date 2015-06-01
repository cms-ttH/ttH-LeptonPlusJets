ttH-LeptonPlusJets
==================


source /cvmfs/cms.cern.ch/cmsset_default.csh
setenv SCRAM_ARCH slc6_amd64_gcc481
 
cmsrel CMSSW_7_2_3
cd CMSSW_7_2_3/src/
cmsenv
 
 
git cms-merge-topic HuguesBrun:trigElecIdInCommonIsoSelection720
 
git clone https://github.com/cms-ttH/MiniAOD.git
git clone https://github.com/cms-ttH/ttH-LeptonPlusJets.git
 
git clone https://github.com/cms-ttH/BoostedTTH.git

git checkout tags/CMSSW_7_5_X_2015-02-25-1400 PhysicsTools/JetMCAlgos

git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
git clone https://github.com/cms-ttH/ttH-Limits.git ttH/Limits


scram b -j 32
