Yggdrasil Framework
================

### Sync2017 Area Setup
## bash:
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    export SCRAM_ARCH="slc6_amd64_gcc530"
    export CMSSW_VERSION="CMSSW_8_0_26_patch2"

## csh:
    source /cvmfs/cms.cern.ch/cmsset_default.csh
    setenv SCRAM_ARCH "slc6_amd64_gcc530"
    setenv CMSSW_VERSION "CMSSW_8_0_26_patch2"
## Both:

    cmsrel $CMSSW_VERSION
    cd $CMSSW_VERSION/src
    cmsenv

    git cms-init
  

    git cms-merge-topic riga:deterministicSeeds
    git cms-merge-topic riga:badGlobalMuonTagger_fix
    git cms-merge-topic cms-met:METRecipe_80X_part2
 
    git clone https://github.com/cms-met/MetTools.git

    git cms-merge-topic riga:deterministicEGMSmearer_v2
    cd EgammaAnalysis/ElectronTools/data
    git clone https://github.com/ECALELFS/ScalesSmearings.git -b Moriond17_gainSwitch_unc
    cd $CMSSW_BASE/src

    git cms-merge-topic riga:ttHFGenFilter_tagging

    git remote add ahinzmann https://github.com/ahinzmann/cmssw.git
    git fetch ahinzmann PUidMiniAODfix80
    git cherry-pick ca33756e1747aec27d13971bcfd0874b16724e7f

    scram b -j

### Yggdrasil Specific Code
    
    git clone https://github.com/hsatoshi/GenParticleTopOriginChargedleptonFilter.git ttHAnalysisSubprogram/GenParticleTopOriginChargedleptonFilter

    git clone -b CMSSW_8_0_24_v1_sync https://github.com/cms-ttH/MiniAOD.git
    git clone -b PostSync2017 https://github.com/cms-ttH/ttH-LeptonPlusJets.git

    scram b -j
