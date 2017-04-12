source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH="slc6_amd64_gcc530"
export CMSSW_VERSION="CMSSW_8_0_26_patch2"

cmsrel $CMSSW_VERSION
cd $CMSSW_VERSION/src
cmsenv

git cms-init

//deterministic seed producer
git cms-merge-topic riga:deterministicSeeds

//updated MET tools
//this topic is branched from the official cms-met:METRecipe_8020 but fixes the badGlobalMuonTagger
//so that it works like any other MET filter module
git cms-merge-topic riga:badGlobalMuonTagger_fix
git cms-merge-topic cms-met:METRecipe_80X_part2

//updated MET phi corrections
git clone https://github.com/cms-met/MetTools.git

//EGMSmearer and data
//this topic is branched from cms-egamma:EGM_gain_v1 and makes the EGMSmearer use our deterministic
//seeds (and also stores the pt before calibration as a userFloat which we need later for
//lepton/trigger scale factors)
git cms-merge-topic riga:deterministicEGMSmearer_v2
cd EgammaAnalysis/ElectronTools/data
git clone https://github.com/ECALELFS/ScalesSmearings.git -b Moriond17_gainSwitch_unc
cd $CMSSW_BASE/src

//ttHFGenFilter
//(only required when you use the ttHF filtered ttJets dataset)
git cms-merge-topic riga:ttHFGenFilter_tagging

//update PUJetId values
git remote add ahinzmann https://github.com/ahinzmann/cmssw.git
git fetch ahinzmann PUidMiniAODfix80
git cherry-pick ca33756e1747aec27d13971bcfd0874b16724e7f


scram b -j


git clone -b CMSSW_8_0_24_v1_sync https://github.com/cms-ttH/MiniAOD.git
git clone -b PostSync2017 https://github.com/cms-ttH/MiniAOD.git 

scram b -j
