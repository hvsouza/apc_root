#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import os


def read_and_write(filename, ch):
    data = np.load(filename, allow_pickle=True)
    with open(f'wave{ch}.dat','wb') as f:
        for d in data:
            leng = (len(d)*2+24).to_bytes(4,'little')
            for _ in range(6):
                f.write(leng)
            f.write(bytearray(d))


if __name__ == "__main__":
    all_files = os.listdir()
    for filename in all_files:
        if not filename.startswith("adc_data"): continue
        ch = filename.split("ch")[1]
        ch = int(ch.split("_")[1])
        read_and_write(filename, ch)
