

targetdir=`head DL_distination.txt`/tth/

echo ${targetdir}

mkdir -p ${targetdir}

xrdcp root://cmsxrootd.fnal.gov//store/mc/RunIISpring16MiniAODv2/ttHTobb_M125_13TeV_powheg_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/40000/0089CC67-6338-E611-947D-0025904C4E2A.root ${targetdir}

ls -1 ${targetdir}/
