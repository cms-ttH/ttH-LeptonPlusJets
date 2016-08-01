


echo ""
echo ""
echo "# - - - - - - - "
echo remove large files from directory to submit jobs.
rm -rf ../../AnalysisCode/data/Train*/*root
echo " Removing large file is done..."
echo "# - - - - - - "

cat yggdrasil_treeMaker_cfg.py | sed "s|isMC=False|isMC=True|g" > __yggdrasil_treeMaker_MC_cfg.py 
diff yggdrasil_treeMaker_cfg.py __yggdrasil_treeMaker_MC_cfg.py

cat yggdrasil_treeMaker_cfg.py | sed "s|isMC=False|isMC=True|g" | sed "s|isTTBARMC=False|isTTBARMC=True|g" > __yggdrasil_treeMaker_MCTTBAR_cfg.py 
diff yggdrasil_treeMaker_cfg.py __yggdrasil_treeMaker_MCTTBAR_cfg.py

cat yggdrasil_treeMaker_cfg.py | sed "s|isMC=True|isMC=False|g" > __yggdrasil_treeMaker_DATA_cfg.py 
diff yggdrasil_treeMaker_cfg.py __yggdrasil_treeMaker_DATA_cfg.py

nickname="Satoshi_CHSJETS_2cb4bf82b_ElectronIdFlagUpdated"


JobIndexList=""

# I do not use : /SingleElectron/Run2016B-PromptReco-v1/MINIAOD
ds[1]=/SingleElectron/Run2016B-PromptReco-v2/MINIAOD
ds[2]=/SingleElectron/Run2016C-PromptReco-v2/MINIAOD
ds[3]=/SingleElectron/Run2016D-PromptReco-v2/MINIAOD
ds[4]=/SingleElectron/Run2016E-PromptReco-v2/MINIAOD
ds[5]=/SingleElectron/Run2016F-PromptReco-v1/MINIAOD
name[1]=DataMuB
name[2]=DataMuC
name[3]=DataMuD
name[4]=DataMuE
name[5]=DataMuF
ismc[1]=DATA
ismc[2]=DATA
ismc[3]=DATA
ismc[4]=DATA
ismc[5]=DATA

JobIndexList=${JobIndexList}" 1 2 3 4 5 "


ds[11]=/SingleMuon/Run2016B-PromptReco-v2/MINIAOD
ds[12]=/SingleMuon/Run2016C-PromptReco-v2/MINIAOD
ds[13]=/SingleMuon/Run2016D-PromptReco-v2/MINIAOD
ds[14]=/SingleMuon/Run2016E-PromptReco-v2/MINIAOD
name[11]=DataElB
name[12]=DataElC
name[13]=DataElD
name[14]=DataElE
ismc[11]=DATA
ismc[12]=DATA
ismc[13]=DATA
ismc[14]=DATA

JobIndexList=${JobIndexList}" 11 12 13 14 "


ds[21]=/ttHTobb_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM
name[21]=tthbb
ismc[21]=MC



ds[22]=/ttHToNonbb_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM
name[22]=tthnonbb
ismc[22]=MC


# ds[3]=/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# name[3]=DYlow
# ismc[3]=MC
# 
# ds[4]=/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# name[4]=DYhigh
# ismc[4]=MC
# 
# ds[5]=/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# name[5]=Wjet
# ismc[5]=MC
 
ds[26]=/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext3-v1/MINIAODSIM
name[26]=ttbar
ismc[26]=MC
 


JobIndexList=${JobIndexList}" 21 22 26 "


# 
# 
# ds[8]=/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# name[8]=ttbarmadgraphpythia8
# ismc[8]=MC
# 
# ds[9]=/TT_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# name[9]=ttbaramcatnlopythia8
# ismc[9]=MC
# 
# 
# name[10]=SingleTop
# ds[10]=/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# ismc[10]=MC
# 
# 
# name[11]=SingleAntiTop
# ds[11]=/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# ismc[11]=MC
# 
# name[12]=ww
# ds[12]=/WW_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# ismc[12]=MC
# 
# 
# name[13]=wz
# ds[13]=/WZ_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# ismc[13]=MC
# 
# name[14]=zz
# ds[14]=/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# ismc[14]=MC
# 
# 
# name[15]=powhegherwig
# ds[15]=/TT_TuneEE5C_13TeV-powheg-herwigpp/RunIIFall15MiniAODv1-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# ismc[15]=MC
# 
# 
# 
# ds[17]=/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext4-v1/MINIAODSIM
# name[17]=ttbarext4
# ismc[17]=MC
# 
# 
# ds[18]=/TTToSemiLeptonic_13TeV-powheg/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM
# name[18]=ttbarsemilep
# ismc[18]=MC
# 
# 
# 
# ds[19]=/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# ds[20]=/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# ds[21]=/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# ds[22]=/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# name[19]=QCDHT100to200
# name[20]=QCDHT200to300
# name[21]=QCDHT300to500
# name[22]=QCDHT500to700
# ismc[19]=MC
# ismc[20]=MC
# ismc[21]=MC
# ismc[22]=MC
# 
# 
# ds[23]=/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# ds[24]=/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# ds[25]=/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# ds[26]=/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# name[23]=QCDHT700to1000
# name[24]=QCDHT1000to1500
# name[25]=QCDHT1500to2000
# name[26]=QCDHT2000toInf
# ismc[23]=MC
# ismc[24]=MC
# ismc[25]=MC
# ismc[26]=MC




for idx in ` echo $JobIndexList `
do


if [ ${ismc[$idx]} == "MC" ]
then

if [ ${name[$idx]} == "ttbar" ] 
then
cat crabconfig_template.py | sed "s|XXXXX|${name[${idx}]}|g" | sed "s|YYYYY|${nickname}|g" | sed "s|ZZZZZ|${ds[$idx]}|g" | \
                             sed "s|QQQQQ|MCTTBAR|g"> __CRABCONFIG__${name[${idx}]}.py
else
cat crabconfig_template.py | sed "s|XXXXX|${name[${idx}]}|g" | sed "s|YYYYY|${nickname}|g" | sed "s|ZZZZZ|${ds[$idx]}|g" | \
                             sed "s|QQQQQ|${ismc[$idx]}|g"> __CRABCONFIG__${name[${idx}]}.py

fi

else
cat crabconfig_template.py | sed "s|XXXXX|${name[${idx}]}|g" | sed "s|YYYYY|${nickname}|g" | sed "s|ZZZZZ|${ds[$idx]}|g" | \
                             sed "s|QQQQQ|${ismc[$idx]}|g" | \
   sed "s|#PPPPP#|config.Data.lumiMask=\"../data/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt\"|g" \
    > __CRABCONFIG__${name[${idx}]}.py
fi



done