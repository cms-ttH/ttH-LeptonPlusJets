


echo ""
echo ""
echo "# - - - - - - - "
echo remove large files from directory to submit jobs.
rm ../../AnalysisCode/data/Train*/*root
echo " Removing large file is done..."
echo "# - - - - - - "

cat yggdrasil_treeMaker_cfg.py | sed "s|isMC=False|isMC=True|g" > __yggdrasil_treeMaker_MC_cfg.py 
diff yggdrasil_treeMaker_cfg.py __yggdrasil_treeMaker_MC_cfg.py
cat yggdrasil_treeMaker_cfg.py | sed "s|isMC=True|isMC=False|g" > __yggdrasil_treeMaker_DATA_cfg.py 
diff yggdrasil_treeMaker_cfg.py __yggdrasil_treeMaker_DATA_cfg.py

nickname="Satoshi_CHSJETS_2cb4bf82b_ElectronIdFlagUpdated"


ds[1]=/ttHTobb_M125_13TeV_powheg_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
name[1]=tthbb
ismc[1]=MC

ds[2]=/ttHToNonbb_M125_13TeV_powheg_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
name[2]=tthnonbb
ismc[2]=MC

ds[3]=/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
name[3]=DYlow
ismc[3]=MC

ds[4]=/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
name[4]=DYhigh
ismc[4]=MC

ds[5]=/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
name[5]=Wjet
ismc[5]=MC

ds[6]=/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext3-v1/MINIAODSIM
name[6]=ttbar
ismc[6]=MC


ds[7]=/SingleMuon/Run2015D-16Dec2015-v1/MINIAOD
name[7]=DataMuD
ismc[7]=DATA


ds[8]=/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
name[8]=ttbarmadgraphpythia8
ismc[8]=MC

ds[9]=/TT_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
name[9]=ttbaramcatnlopythia8
ismc[9]=MC


name[10]=SingleTop
ds[10]=/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
ismc[10]=MC


name[11]=SingleAntiTop
ds[11]=/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
ismc[11]=MC

name[12]=ww
ds[12]=/WW_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
ismc[12]=MC


name[13]=wz
ds[13]=/WZ_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
ismc[13]=MC

name[14]=zz
ds[14]=/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
ismc[14]=MC


name[15]=powhegherwig
ds[15]=/TT_TuneEE5C_13TeV-powheg-herwigpp/RunIIFall15MiniAODv1-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
ismc[15]=MC


ds[16]=/SingleElectron/Run2015D-16Dec2015-v1/MINIAOD
name[16]=DataElD
ismc[16]=DATA

ds[17]=/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext4-v1/MINIAODSIM
name[17]=ttbarext4
ismc[17]=MC


ds[18]=/TTToSemiLeptonic_13TeV-powheg/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM
name[18]=ttbarsemilep
ismc[18]=MC



ds[19]=/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
ds[20]=/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
ds[21]=/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
ds[22]=/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
name[19]=QCDHT100to200
name[20]=QCDHT200to300
name[21]=QCDHT300to500
name[22]=QCDHT500to700
ismc[19]=MC
ismc[20]=MC
ismc[21]=MC
ismc[22]=MC


ds[23]=/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
ds[24]=/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
ds[25]=/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
ds[26]=/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
name[23]=QCDHT700to1000
name[24]=QCDHT1000to1500
name[25]=QCDHT1500to2000
name[26]=QCDHT2000toInf
ismc[23]=MC
ismc[24]=MC
ismc[25]=MC
ismc[26]=MC




for idx in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26
do


if [ ${ismc[$idx]} == "MC" ]
then
cat crabconfig_template.py | sed "s|XXXXX|${name[${idx}]}|g" | sed "s|YYYYY|${nickname}|g" | sed "s|ZZZZZ|${ds[$idx]}|g" | \
                             sed "s|QQQQQ|${ismc[$idx]}|g"> __CRABCONFIG__${name[${idx}]}.py
else
cat crabconfig_template.py | sed "s|XXXXX|${name[${idx}]}|g" | sed "s|YYYYY|${nickname}|g" | sed "s|ZZZZZ|${ds[$idx]}|g" | \
                             sed "s|QQQQQ|${ismc[$idx]}|g" | \
   sed "s|#PPPPP#|config.Data.lumiMask=\"../data/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_Silver.txt\"|g" \
    > __CRABCONFIG__${name[${idx}]}.py
fi


# sed "s|#PPPPP#|config.Data.lumiMask=\"../data/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON.txt\"|g" \


done