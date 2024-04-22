#!/bin/bash

set -e # if something fails, crash everthing and dont keep going

read -p "Are you sure (y/n)? " -n 1 -r
echo    # (optional) move to a new line
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
    return
fi

# for subfolder in $(eval "/bin/ls -d  */"); do
#     cd $subfolder
    for subruns in $(eval "/bin/ls -d  */ | grep run"); do
        # echo $x$subruns
        cd $subruns
        echo $(pwd)
        eval "root -l -b -q $jan2024/adc_read_all_data_random_trigger.C+"
        cd ../
    done
    # cd ../
# done

set +e # now back to normal
