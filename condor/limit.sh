#!/bin/bash
loop=0
while [ $loop -lt 40 ]
do
echo "loop $loop"
while IFS= read -r line
do
  nn=`condor_q|wc -l` 
while [ $nn -gt 19 ]
do
  nn=`condor_q|wc -l`
  echo 'wait'
  sleep 60
done
condor_submit $line
done < suball_ex.sh
loop=$((loop + 1))
done


