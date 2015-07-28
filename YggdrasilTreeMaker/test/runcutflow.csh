#!/bin/csh -f

#if( $#argv < 7 ) then
#    echo "usage ./runCSVSF.csh [POSTFIX] [POIS_FLUCT] [USE_8TEV_SF] [P0_HF] [P1_HF] [P0_LF] [P1_LF]"
#    exit 2
#endif
#
set infile=$1


set dir="cutflow"
if( ! -e $dir ) then
    mkdir $dir
endif


## Iterations (three seems to be sufficent)
#foreach cat ( ee mumu emu ele muon )
foreach cat ( ele muon )
    foreach iter ( 1 2 3 4 5 6 7 )
	grep ''$cat'  '$iter'' $infile >&! $dir/{$cat}_cut{$iter}.txt
	sed -i 's|'$cat'  '$iter' ||' $dir/{$cat}_cut{$iter}.txt
    end
end

echo "Finished runcutflow.csh"

