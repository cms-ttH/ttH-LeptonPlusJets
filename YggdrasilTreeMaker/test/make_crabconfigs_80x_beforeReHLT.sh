


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

nickname="Satoshi_Yggdra_ButNotReHLTminiAOD_Aug01"

JobIndexList=""

ds[1]=/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM
ds[2]=/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM
ds[3]=/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[4]=/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[5]=/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM
ds[6]=/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[7]=/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[8]=/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM
ds[9]=/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[10]=/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[11]=/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[12]=/WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[13]=/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[14]=/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[15]=/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM
ds[16]=/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[17]=/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[18]=/TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[19]=/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[20]=/TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[21]=/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
ds[22]=/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM



name[1]=wjet_HT-100To200
name[2]=wjet_HT-200To400
name[3]=wjet_HT-400To600
name[4]=wjet_HT-600To800
name[5]=wjet_HT-800To1200
name[6]=wjet_HT-1200To2500
name[7]=wjet_HT-2500ToInf
name[8]=DYJetsToLL_M-50
name[9]=DYJetsToLL_M-10to50
name[10]=ZZ
name[11]=WZ
name[12]=WW
name[13]=ST_s
name[14]=ST_t_antitop
name[15]=ST_t_top
name[16]=ST_tW_antitop
name[17]=ST_tW_top
name[18]=TTZtoQQ
name[19]=TTWJetsToLNu
name[20]=TTWJetsToQQ
name[21]=TTZToLLNuNu
name[22]=wjet_inclusive


ismc[1]=MC
ismc[2]=MC
ismc[3]=MC
ismc[4]=MC
ismc[5]=MC
ismc[6]=MC
ismc[7]=MC
ismc[8]=MC
ismc[9]=MC
ismc[10]=MC
ismc[11]=MC
ismc[12]=MC
ismc[13]=MC
ismc[14]=MC
ismc[15]=MC
ismc[16]=MC
ismc[17]=MC
ismc[18]=MC
ismc[19]=MC
ismc[20]=MC
ismc[21]=MC
ismc[22]=MC


for idx in `seq 1 22`
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