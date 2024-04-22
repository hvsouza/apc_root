#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import os
import glob

def read_and_write(filename, ch):
    data = np.load(filename, allow_pickle=True)
    print(f"Writting {filename} of ch {ch} ... ")
    with open(f'wave{ch}.dat','ab') as f:
        for d in data['wvfms']:
            leng = (len(d)*2+24).to_bytes(4,'little')
            for _ in range(6):
                f.write(leng)
            f.write(bytearray(d))


if __name__ == "__main__":
    all_files = sorted(os.listdir())
    fileisthere = False
    datfiles = glob.glob("wave*.dat")
    for dfile in datfiles:
        os.remove(dfile)

    for filename in all_files:
        if not filename.startswith("np02"): continue
        ch = filename.split("ch")[1]
        ch = int(ch.split(".")[0])
        read_and_write(filename, ch)
