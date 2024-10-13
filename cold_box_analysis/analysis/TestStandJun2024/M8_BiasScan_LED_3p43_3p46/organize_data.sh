#!/bin/bash

# create directories
allbias=$( ls -1 | grep -Eo "V3.p." | sort | uniq )


for bias in ${allbias[*]}; do
    mkdir $bias

    ls *.dat | grep $bias | xargs -I {} echo 'moving {} to '$bias
    ls *.dat | grep $bias | xargs -I {} mv {} $bias

    cd $bias
    allleds=$( ls -1 | grep -Eo "3p4." | sort | uniq )
    for led in ${allleds[*]}; do
        mkdir $led
        ls *.dat | grep $led | xargs -I {} echo 'moving {} to '$led
        ls *.dat | grep $led | xargs -I {} mv {} $led
    done
    cd - > /dev/null
done
