

targetdir=`head DL_distination.txt`/singlemuon/

echo ${targetdir}

mkdir -p ${targetdir}

xrdcp root://cmsxrootd.fnal.gov//store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/150/00000/34A57FB8-D819-E611-B0A4-02163E0144EE.root ${targetdir}
xrdcp root://cmsxrootd.fnal.gov//store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/158/00000/02D9C19F-571A-E611-AD8E-02163E013732.root ${targetdir}
xrdcp root://cmsxrootd.fnal.gov//store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/158/00000/18383F36-2E1A-E611-8C57-02163E014186.root ${targetdir}
xrdcp root://cmsxrootd.fnal.gov//store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/158/00000/1AF0711C-241A-E611-AC07-02163E0141E1.root ${targetdir}
xrdcp root://cmsxrootd.fnal.gov//store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/158/00000/2096A6AC-261A-E611-99BD-02163E01355E.root ${targetdir}
xrdcp root://cmsxrootd.fnal.gov//store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/158/00000/2226C1A1-571A-E611-B1E2-02163E013441.root ${targetdir}
xrdcp root://cmsxrootd.fnal.gov//store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/158/00000/422892A9-571A-E611-9BB8-02163E011D78.root ${targetdir}
xrdcp root://cmsxrootd.fnal.gov//store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/158/00000/66E2AFB2-571A-E611-BF2A-02163E013674.root ${targetdir}
xrdcp root://cmsxrootd.fnal.gov//store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/158/00000/701EA1A3-571A-E611-95AC-02163E011D78.root ${targetdir}
xrdcp root://cmsxrootd.fnal.gov//store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/158/00000/8A14EDAB-571A-E611-96F5-02163E0123DD.root ${targetdir}


ls -1 ${targetdir}/*
