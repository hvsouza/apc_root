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

  thedir=/eos/home-h/hvieirad/December2024run/

  theruns=$( tail -n +2 $thedir/info_led_scan.csv | cut -d , -f $field | grep -v - )


  # theruns=( 34405 )
  thechannels='channels=$( cat files.log | sed -e "s/.*channel_\([0-9]\+\)/\1/g" )'
  thechannels='channels=( 37 )'


  root -l -q -e ".L $thedir/giveMeSphe.C+"
  root -l -q -e ".L $thedir/get_avg_wvf_info.C+"

  echo ${theruns[@]} | xargs -P5 -n1 bash -c 'cd run_${0}; '"$thechannels"'; for channel in ${channels[@]}; do root -l -q '${thedir}'/giveMeSphe.C+\($channel\); done; cd ../'

  echo ${theruns[@]} | xargs -P5 -n1 bash -c 'cd run_${0}; '"$thechannels"'; for channel in ${channels[@]}; do root -l -q '${thedir}'/try_hard.C\($channel\); done; cd ../'

  echo ${theruns[@]} | xargs -P5 -n1 bash -c 'cd run_${0}; '"$thechannels"'; for channel in ${channels[@]}; do root -l -q '${thedir}'/giveMeSphe.C+\($channel\); done; cd ../'

  echo ${theruns[@]} | xargs -P5 -n1 bash -c 'cd run_${0}; '"$thechannels"'; for channel in ${channels[@]}; do root -l -q -b '${thedir}'/get_avg_wvf_info.C+\($channel\); done; cd ../'

	# for run in ${theruns[@]}; do
	# 	if [ "$run" == - ]; then
      # echo "WTF..."
	# 		continue
	# 	fi
	# 	 cd run_$run
	# 	 if [ ! -f analyzed.root ]; then
	# 		 echo "File is not there!... $run"
	# 		 cd ../
	# 		 continue
	# 	 fi

     # # channels=( 41 46 )
     # # for channel in ${channels[@]}; do

	# 	 for line in $( cat files.log ); do 
	# 		 channel=$( echo $line | sed -e 's/.*channel_\([0-9]\+\)/\1/g')
	# 		 # root -l -q $thedir'/giveMeSphe.C+('$channel')'
	# 		 # root -l -q $thedir'/try_hard.C('$channel')'
	# 		 # root -l -q $thedir'/giveMeSphe.C+('$channel')'
	# 	 done

	# 	 cd ../
	# done
}

process $1 $2

