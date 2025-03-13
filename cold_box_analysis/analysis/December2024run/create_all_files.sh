#!/bin/bash

process()
{
	module=${1:-""}
	force=${2:-false}
	field=2
	case "$module" in
		C1)
			field=2
			;;
		C2)
			field=3
			;;
		C3)
			field=3
			;;
		C4)
			field=4
			;;
		*)
			echo "Nope"
			return 1
			;;
	esac

	theruns=$( tail -n +2 /eos/home-h/hvieirad/December2024run/info_led_scan.csv | cut -d , -f $field | grep -v -)
	thedata=/eos/experiment/neutplatform/protodune/experiments/ColdBoxVD/December2024run/Daphne_DAQ/binaries/


	root -l -q -e ".L /eos/home-h/hvieirad/December2024run/adc_read_all_data.C+"

	echo ${theruns[@]} | xargs -P5 -n1 bash -c 'cd run_${0}; root -l -q /eos/home-h/hvieirad/December2024run/adc_read_all_data.C+\(\"'$thedata'/run_${0}/\"\); cd ../'

	# # Old shit, working but not multprocess..
	# for run in ${theruns[@]}; do
	# 	if [ "$run" == - ]; then
	# 		echo $run
	# 		continue
	# 	fi
	# 	 mkdir -p run_$run
	# 	 cd run_$run
	# 	 if [ -f analyzed.root ]; then
	# 		 echo "File already there... $run"
	# 		 if $force; then
	# 			 echo "Forcing it..."
	# 		 else
	# 			 echo "Skipp..."
	# 			 cd ..
	# 			 continue
	# 		 fi
	# 	 fi
	# 	 root -l -q '/eos/home-h/hvieirad/December2024run/adc_read_all_data.C+("'$thedata'/run_'$run'/")'
	# 	 cd ../
	# done
}

process $1 $2

