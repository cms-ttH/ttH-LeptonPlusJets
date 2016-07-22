

targetdir=`head DL_distination.txt`/ttbar/

echo ${targetdir}

mkdir -p ${targetdir}

echo xrdcp root://cmsxrootd.fnal.gov///store/mc/RunIISpring16MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext3-v1/00000/0064B539-803A-E611-BDEA-002590D0B060.root ${targetdir}

ls -1 ${targetdir}/
