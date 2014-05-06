s#!/bin/csh

mkdir weights #complains if "weights" exists, but job runs fine.


echo "Starting training script."
echo "... "



#### final BDT training, each category ####

root -b -q ../head.C 'train.C++(433)' >&! trainlog_433.txt
echo "433 done ..."

root -b -q ../head.C 'train.C++(443)' >&! trainlog_443.txt
echo "443 done ..."

root -b -q ../head.C 'train.C++(533)' >&! trainlog_533.txt
echo "533 done ..."

root -b -q ../head.C 'train.C++(543)' >&! trainlog_543.txt
echo "543 done ..."

root -b -q ../head.C 'train.C++(623)' >&! trainlog_623.txt
echo "623 done ..."

root -b -q ../head.C 'train.C++(633)' >&! trainlog_633.txt
echo "633 done ..."

root -b -q ../head.C 'train.C++(643)' >&! trainlog_643.txt
echo "643 done ..."

#### ttbb/ttH BDT training ####

root -b -q ../head.C 'train.C++(540000)' >&! trainlog_540000.txt
echo "540000 done ..."

root -b -q ../head.C 'train.C++(630000)' >&! trainlog_630000.txt
echo "630000 done ..."

root -b -q ../head.C 'train.C++(640203)' >&! trainlog_640203.txt
echo "640203 done ..."





sleep 2
echo "  "
echo "All done. Checking KS tests... "
echo "  "
sleep 3
root -b -q 'get_KS_values.C+'
echo "  "
echo "  "
echo "If any of the above were <0.05, they need to be trained again. See train.csh and comment out lines where appropriate."
