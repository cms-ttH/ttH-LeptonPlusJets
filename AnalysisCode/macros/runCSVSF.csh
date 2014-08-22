#!/bin/csh -f

if( $#argv < 7 ) then
    echo "usage ./runCSVSF.csh [POSTFIX] [POIS_FLUCT] [USE_8TEV_SF] [P0_HF] [P1_HF] [P0_LF] [P1_LF]"
    exit 2
endif

#set iter=0
set dyname="DYJetsToLL_M_50_13TeV_madgraph_pythia8_PU20bx25_POSTLS170_V5_v1"
set ttname="TTJets_MSDecaysCKM_central_Tune4C_13TeV_madgraph_PU20bx25_POSTLS170_V5_v1"
set postfit=$1
set usePois=$2
set use8TeVSF=$3
set p0_HF=$4
set p1_HF=$5
set p0_LF=$6
set p1_LF=$7


## Iteration 0
#foreach iter ( 0 )
foreach iter ( 0 1 2 )
    root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'(1,'$iter',2510,-1)'
    root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'(1,'$iter',2310,-1)'

    root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'(0,'$iter',2510,-1)'
    root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'(0,'$iter',2310,-1)'
 
    ## hadd DYJets and TTJets
    hadd -f csv_rwt_hf_v{$iter}_mc_histo.root csv_rwt_hf_${dyname}_v0_histo.root csv_rwt_hf_${ttname}_v{$iter}_histo.root
    hadd -f csv_rwt_lf_v{$iter}_mc_histo.root csv_rwt_lf_${dyname}_v0_histo.root csv_rwt_lf_${ttname}_v{$iter}_histo.root
 
    ## Create the data histograms only for iteration 0
    if( $iter < 1 ) then
	root -b -q macros/head13TeV.C macros/createFakeData_csvSF_13TeV.C+'("csv_rwt_hf_v'$iter'_mc_histo.root","csv_rwt_hf_v'$iter'_histo_pseudoData.root",true,'$use8TeVSF','$p0_HF','$p1_HF','$p0_LF','$p1_LF','$usePois')'
	root -b -q macros/head13TeV.C macros/createFakeData_csvSF_13TeV.C+'("csv_rwt_lf_v'$iter'_mc_histo.root","csv_rwt_lf_v'$iter'_histo_pseudoData.root",false,'$use8TeVSF','$p0_HF','$p1_HF','$p0_LF','$p1_LF','$usePois')'
    endif
 
    ## Add data histograms to iteration
    hadd -f csv_rwt_hf_v{$iter}_histo.root csv_rwt_hf_v{$iter}_mc_histo.root csv_rwt_hf_v0_histo_pseudoData.root
    hadd -f csv_rwt_lf_v{$iter}_histo.root csv_rwt_lf_v{$iter}_mc_histo.root csv_rwt_lf_v0_histo_pseudoData.root
 
    ## Do LF and HF fitting
    root -b -q macros/head13TeV.C macros/fitHF_csvSF_13TeV.C+'("csv_rwt_hf_v'$iter'_histo.root",'$iter',"_pseudoData_'$postfit'_iter'$iter'")'
    root -b -q macros/head13TeV.C macros/fitLF_csvSF_13TeV.C+'("csv_rwt_lf_v'$iter'_histo.root",'$iter',"_pseudoData_'$postfit'_iter'$iter'")'

end

## Make stacked plots
root -b -q macros/head13TeV.C macros/makePlots_csvSF_13TeV.C+'("csv_rwt_hf_v0_histo.root",true,"_'$postfit'",true)'
root -b -q macros/head13TeV.C macros/makePlots_csvSF_13TeV.C+'("csv_rwt_lf_v0_histo.root",false,"_'$postfit'",true)'

echo "Finished runCSVSF.csh"

