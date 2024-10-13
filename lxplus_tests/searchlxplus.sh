#!/bin/bash

errfile=$( mktemp  )
echo $errfile
output=lxout
rm -f $output
touch $output
for n in {900..999}; do
    var=$( gpg -d -q ~/.password.gpg | timeout 15s sshpass ssh -oStrictHostKeyChecking=no hvieirad@lxplus${n}.cern.ch "nproc --all" 2>> $errfile)
    if [[ ${var} =~ ^[0-9] ]]; then
        echo "${n} ${var}" >> "${output}"
    else
        echo "${n} ${var}" >> "${errfile}"
    fi
done


echo $errfile
cat $errfile
