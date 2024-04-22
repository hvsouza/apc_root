#! /usr/bin/env python3

from tqdm.contrib.concurrent import process_map
import os
from glob import glob
import pandas as pd
import subprocess
import argparse
import re

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-m", "--module", type=str, help='VD, HD, C1, C2, C3 or C4')
    parser.add_argument("-i", "--info", type=str, help='subfolder argument (something unique and handsome)')
    args = vars(parser.parse_args())
    module = args['module']
    info = args['info']
    path_name = f"./{info}/*"
    dirs = sorted(glob(path_name))
    def parallel_func(it):
        subprocess.run(["/home/henrique/Documents/ADC_data/coldbox_data/April2024run/calibrationData/run_sphe.sh", it])
    nproc = min(16, os.cpu_count())
    print(f"Starting decoding with {nproc} processes")
    process_map(parallel_func, dirs, max_workers=nproc)
