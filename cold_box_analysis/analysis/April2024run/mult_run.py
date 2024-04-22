#! /usr/bin/env python3

from tqdm.contrib.concurrent import process_map
import os
from glob import glob
import pandas as pd
import subprocess
import argparse
import re
import thresholds

if __name__ == '__main__':
        parser = argparse.ArgumentParser()
        parser.add_argument("-m", "--module", type=str, help='VD, HD, C1, C2, C3 or C4')
        parser.add_argument("-i", "--info", type=str, help='subfolder argument (something unique and handsome)')
        parser.add_argument("-n", "--dry",  action="store_true", help='Dry run')
        args = vars(parser.parse_args())
        module = args['module']
        info = args['info']
        dry = args['dry']
        polarity = 1
        if module is None:
                parser.print_help()
                exit(0)
        elif module=="VD":
                dirthreshold = thresholds._vd
                polarity = -1
        elif module=="HD":
                dirthreshold = thresholds._hd
                polarity = -1
        elif module=="C1":
                dirthreshold = thresholds._C1
        elif module=="C2":
                dirthreshold = thresholds._C2
        elif module=="C3":
                dirthreshold = thresholds._C3
        elif module=="C4":
                dirthreshold = thresholds._C4
        path_name = f"./calibrationData/{info}/*"
        
        dirs = sorted(glob(path_name))
        voltages = [  ]
        if dry:
                for f in dirs:
                        print(f)
                exit(0)
        for f in dirs:
                val = re.findall("VGAIN_?([1,0]p?[0-9]?)", f)
                val = val[0]
                val = float(val.replace("p", "."))
                voltages.append(val)
      

        threshold = [ f"{dirthreshold[k]}".replace(" ","") for k in voltages ]
        pol = [ f"{polarity}" for _ in voltages ]
        pargs = [ [d, t, p] for d, t, p in zip(dirs, threshold, pol) ]
        # for p in pargs:
        #         print(p)
        # exit(0)
        def parallel_func(it, ni, pi):
                ret = subprocess.run(["/home/henrique/Documents/ADC_data/coldbox_data/April2024run/process_file.sh", it, ni, pi], capture_output=True, text=True)
                if ret.stderr != '':
                        print(ret.stderr)
                        print(it)

        nproc = min(16, os.cpu_count())
        print(f"Starting decoding with {nproc} processes")
        process_map(parallel_func, dirs, threshold, pol, max_workers=nproc)
