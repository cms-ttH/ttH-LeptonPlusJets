#!/bin/sh

echo 

echo ""
echo "Using ROOT on Condor"
echo ""
cd ${_CONDOR_SCRATCH_DIR}

SampleType=$1
NumEvents=$2
NumJobs=$3
jobN=$4+1

root -b -q Yggdrasil_Slim.C'('$SampleType','$NumEvents','$NumJobs','$jobN')'

