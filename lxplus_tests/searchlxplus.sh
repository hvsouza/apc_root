#!/bin/bash

errfile=$( mktemp  )
echo $errfile
output=lxouttmux
rm -f $output
touch $output
for n in {900..999}; do
    # var=$( gpg -d -q ~/.password.gpg | timeout 15s sshpass ssh -oStrictHostKeyChecking=no hvieirad@lxplus${n}.cern.ch "nproc --all" 2>> $errfile)
    # var=$( gpg -d -q ~/.password.gpg | timeout 15s sshpass ssh -oStrictHostKeyChecking=no hvieirad@lxplus${n}.cern.ch "systemctl --user status | grep vnc" 2>> $errfile)
    var=$( gpg -d -q ~/.password.gpg | timeout 15s sshpass ssh -oStrictHostKeyChecking=no hvieirad@lxplus${n}.cern.ch "systemctl --user status | grep tmux" 2>> $errfile)
    if [[ ${var} =~ ^[0-9] ]]; then
        echo "${n} ${var}" >> "${output}"
    else
        echo "${n} ${var}" >> "${errfile}"
    fi
done

echo $errfile
cat $errfile
