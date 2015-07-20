ttH-LeptonPlusJets
==================

source /cvmfs/cms.cern.ch/cmsset_default.csh

### 72X

setenv SCRAM_ARCH slc6_amd64_gcc481
 
cmsrel CMSSW_7_2_3

cd CMSSW_7_2_3/src/

cmsenv
 
### 74x

setenv SCRAM_ARCH slc6_amd64_gcc491

cmsrel CMSSW_7_4_4_patch4

cd CMSSW_7_4_4_patch4/src/

cmsenv

#############
 
git cms-merge-topic HuguesBrun:trigElecIdInCommonIsoSelection720  ### rm for 74X
 
git cms-addpkg PhysicsTools/JetMCAlgos

git clone https://github.com/cms-ttH/MiniAOD.git

git clone https://github.com/cms-ttH/ttH-LeptonPlusJets.git
 
git clone https://github.com/cms-ttH/BoostedTTH.git

cp /afs/cern.ch/user/l/lwming/public/74X/GenHFHadronMatcher.cc PhysicsTools/JetMCAlgos/plugins/GenHFHadronMatcher.cc

cp /afs/cern.ch/user/l/lwming/public/74X/GenTtbarCategorizer.cc PhysicsTools/JetMCAlgos/plugins/GenTtbarCategorizer.cc

cp /afs/cern.ch/user/l/lwming/public/74X/GenTtbarCategorizer_cfi.py PhysicsTools/JetMCAlgos/python/GenTtbarCategorizer_cfi.py

git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit

### for 74X only

cd HiggsAnalysis/CombinedLimit

git fetch origin

git checkout v6.0.0

cd -

git clone https://github.com/cms-ttH/ttH-Limits.git ttH/Limits

scram b -j 32
