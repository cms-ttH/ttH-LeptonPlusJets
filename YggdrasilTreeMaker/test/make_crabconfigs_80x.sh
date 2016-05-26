


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


nickname="80xMiniAOD_Ygg_a0a09b35c5"
#nickname="MVA_hogehoge"



# ds[1]=/ttHTobb_M125_13TeV_powheg_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# name[1]=tthbb
# ismc[1]=MC
# 
# ds[2]=/ttHToNonbb_M125_13TeV_powheg_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
# name[2]=tthnonbb
# ismc[2]=MC
# 
# ds[3]=
# name[3]=DYlow
# ismc[3]=MC

ds[4]=/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM
name[4]=zjets
ismc[4]=MC

ds[5]=/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
name[5]=Wjet
ismc[5]=MC

ds[6]=/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext3-v1/MINIAODSIM
name[6]=ttbar
ismc[6]=MC


ds[7]=/SingleMuon/Run2016B-PromptReco-v1/MINIAOD
name[7]=DataMuD
ismc[7]=DATA


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


ds[16]=/SingleElectron/Run2016B-PromptReco-v1/MINIAOD
name[16]=DataElD
ismc[16]=DATA


for idx in 4 5 6 7 16
do


if [ ${ismc[$idx]} == "MC" ]
then
cat crabconfig_template.py | sed "s|XXXXX|${name[${idx}]}|g" | sed "s|YYYYY|${nickname}|g" | sed "s|ZZZZZ|${ds[$idx]}|g" | \
                             sed "s|QQQQQ|${ismc[$idx]}|g"> __CRABCONFIG__${name[${idx}]}.py
else
cat crabconfig_template.py | sed "s|XXXXX|${name[${idx}]}|g" | sed "s|YYYYY|${nickname}|g" | sed "s|ZZZZZ|${ds[$idx]}|g" | \
                             sed "s|QQQQQ|${ismc[$idx]}|g" | \
   sed "s|#PPPPP#|config.Data.lumiMask=\"../data/Cert_271036-273450_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt\"|g" \
    > __CRABCONFIG__${name[${idx}]}.py
fi


# sed "s|#PPPPP#|config.Data.lumiMask=\"../data/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON.txt\"|g" \


done