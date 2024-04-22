import uproot
import awkward as ak
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import mplhep
mplhep.style.use(mplhep.style.ROOT)

from tqdm import tqdm
from functools import partialmethod
tqdm.__init__ = partialmethod(tqdm.__init__, disable=False)

import sys
import os
import glob
import argparse

plt.rcParams.update({'font.size': 16,
                     'grid.linestyle': '--',
                     'axes.grid': True,
                     'figure.autolayout': True
                     })


mu1 = []
mu2 = []
mu21 = []
sigma1 = []
b = []
sigmab = []
vgain = []

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-ch", "--channel", type=str)
    parser.add_argument("-xara", "--xara", type=str)
    args = vars(parser.parse_args())
    channel  = args['channel']
    xara = args['xara']
    if xara==None:
        xara=''
    
    

    speinfo = f"/home/henrique/Documents/ADC_data/coldbox_data/January2024run/20240116_SPE_LED/SPE_275nm/run162_{xara}_all_pof_on_LED_275nm_20ns_6V20/sphe{channel}.txt"

    with open(speinfo, 'r') as f:
        f.seek(0)
        lines = f.read().splitlines()
        lastline = lines[-1]
        vals = [ float(v) for v in lastline.split() ]
        mu1.append(vals[0])
        mu2.append(vals[1])
        mu21.append(vals[2])
        sigma1.append(vals[3])
        b.append(vals[4])
        sigmab.append(vals[5])
