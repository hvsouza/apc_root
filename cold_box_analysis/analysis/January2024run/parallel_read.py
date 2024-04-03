#!/usr/bin/env python3

from tqdm.contrib.concurrent import process_map
import os
from glob import glob
import pandas as pd
import subprocess

if __name__ == '__main__':
    DBFILE = "run_db.txt"
    db = pd.read_csv(DBFILE, delim_whitespace=True)
    selected = []
    for _, row in db.iterrows():
        run = row.run
        scheme = f"/feynman/scratch/dphp/pgranger/aqs/{run}-???.aqs"
        fnames = glob(scheme)

        selected += [fname for fname in fnames if not os.path.exists(os.path.splitext(fname)[0] + ".root")]

    def parallel_func(it):
        subprocess.run(["/feynman/home/dphp/pgranger/pgranger/trunk/bin/decoder.exe", it], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    nproc = min(16, os.cpu_count())
    print(f"Starting decoding with {nproc} processes")
    process_map(parallel_func, selected, max_workers=nproc)
