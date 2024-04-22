#!/bin/bash

set -e # if something fails, crash everthing and dont keep going


export april2024

cd $1

root -l -b -q $april2024'/adc_read_all_data_daphne.C("./",'$2', '$3')'

set +e

