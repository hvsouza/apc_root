#!/bin/bash


module=$1
mdir=$(pwd)
export april2024

cd $1
tmp=$( /bin/ls | grep -o "channel[0-9][0-9]\?" )
echo "Processing $1 ..."
for t in $tmp; do
    ch=${t//[^0-9]/}
    # echo $ch
    eval 'root -l -b -q -e ".L $april2024/giveMeSphe.C" -e "giveMeSphe($ch)"'
done
cd $mdir
