
# information : https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial


source /cvmfs/cms.cern.ch/crab3/crab.sh

voms-proxy-init --voms cms --valid 168:00
# * And then, to check grid certification
#  voms-proxy-info --all


# * check write permission
#   crab checkwrite --site=T3_US_FNALLPC


cat<<EOF
#------------ Ready -------------
#
# job cam be submitted by
#
#    crab submit [--config/-c <CRAB-configuration-file>]
#
#
# job monitoring 
#
#    crab status -d crab_projects/crab_test_crab_3_may18
#
# Job re-submit : resubmit
# Report        : report
# Log file      : getlog
# output        : getoutput
#
#--------------------------
EOF

