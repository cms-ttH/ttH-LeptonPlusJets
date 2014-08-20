#!/usr/local/bin/perl

$script = "yggdrasil_treeMaker_13TeV";       # Name of job

$workingDir = "/uscms_data/d2/dpuigh/TTH/miniAOD/CMSSW_7_0_7_patch1/src/ttH-LeptonPlusJets/AnalysisCode";

$sample = 2510;
$isLJ   = 0;
$Njobs  = 1;

$num = @ARGV;

if( $num >= 1 ){
    $sample = $ARGV[0];

    if( $num>=2 ){
	$isLJ = $ARGV[1];

	if( $num>=3 ){
	    $Njobs = $ARGV[2];
	  }
      }
  }



open SHFILE, "> Script/condor\_$script.sh";
print SHFILE "#!/bin/sh\n";
print SHFILE "\n";
print SHFILE "echo $1\n";
print SHFILE "\n";
print SHFILE "echo \"\"\n";
print SHFILE "echo \"Using ROOT on Condor\"\n";
print SHFILE "echo \"\"\n";
print SHFILE "cd \${_CONDOR_SCRATCH_DIR}\n";
print SHFILE "\n";
print SHFILE "sample=\$1\n";
print SHFILE "isLJ=\$2\n";
print SHFILE "NumEvents=\$3\n";
print SHFILE "NumJobs=\$4\n";
print SHFILE "jobN=\$5+1\n";
print SHFILE "\n";
print SHFILE "root -b -q $workingDir/macros/head13TeV.C '$workingDir/macros/$script.C+('\$sample','\$isLJ','\$NumEvents','\$NumJobs','\$jobN')'\n";
print SHFILE "\n";
close SHFILE;


open CONDORFILE, "> Condor/condor\_$script.jdl";
print CONDORFILE "# A Condor submission file\n";
print CONDORFILE "Executable              = Script/condor\_$script.sh\n";
print CONDORFILE "Universe                = vanilla\n";
print CONDORFILE "Getenv                  = true\n";
print CONDORFILE "\n";
print CONDORFILE "Arguments               = $sample $isLJ -1 $Njobs \$(Process)\n";
print CONDORFILE "Output                  = Output/condor\_$sample\_$script\_\$(Process).out\n";
print CONDORFILE "Error                   = Error/condor\_$sample\_$script\_\$(Process).err\n";
print CONDORFILE "Log                     = Log/condor\_$sample\_$script\_\$(Process).log\n";
print CONDORFILE "\n";
print CONDORFILE "use_x509userproxy = true\n";
print CONDORFILE "Should_Transfer_Files   = YES\n";
print CONDORFILE "When_To_Transfer_Output = ON_EXIT\n";
#print CONDORFILE "Transfer_Input_Files = filelist_DYJetsToLL_M_50_13TeV_madgraph_pythia8_PU20bx25_POSTLS170_V5_v1.txt, filelist_TTJets_MSDecaysCKM_central_Tune4C_13TeV_madgraph_PU20bx25_POSTLS170_V5_v1.txt\n";
print CONDORFILE "\n";
print CONDORFILE "#+IsLocalJob             = true\n";
print CONDORFILE "#Rank                    = TARGET.IsLocalSlot\n";
print CONDORFILE "\n";
print CONDORFILE "Queue $Njobs\n";
print CONDORFILE "\n";
close CONDORFILE;

system("chmod a+x Script/condor\_$script.sh");
print "submitting: condor_submit Condor/condor\_$script.jdl\n";
system("condor_submit Condor/condor\_$script.jdl");

