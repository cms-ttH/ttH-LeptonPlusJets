#!/bin/sh

echo 

echo ""
echo "Using ROOT on Condor"
echo ""
cd ${_CONDOR_SCRATCH_DIR}

SampleType=$1
SplitType=$2
usedSlimmedTrees=$3
NumEvents=$4
NumJobs=$5
jobN=$6+1

root -b -q head13TeV.C Yggdrasil_TreeReader_2016.C'('$SampleType','$SplitType','$usedSlimmedTrees','$NumEvents','$NumJobs','$jobN')'

