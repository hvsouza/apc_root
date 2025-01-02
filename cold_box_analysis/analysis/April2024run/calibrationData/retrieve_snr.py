#!/bin/python3

import numpy as np
import matplotlib.pyplot as plt
import uproot
import mplhep
mplhep.style.use(mplhep.style.ROOT)
from glob import glob

import re

import argparse


import mplhep
mplhep.style.use(mplhep.style.ROOT)
plt.rcParams.update({'font.size': 24,
                     'grid.linestyle': '--',
                     'axes.grid': True,
                     'figure.autolayout': True,
                     # 'text.usetex':True
                     })


info = ""
label = ""
def retrieve_info(module, channel):
    dirs = sorted(glob(f"{info}/*"))
    print(dirs)
    vgains = []
    snr = []
    drange = []
    spes = []
      
    mu1 = []
    mu2 = []
    mu21 = []
    sigma1 = []
    b = []
    sigmab = []

    if module != "VD" and module !="HD":
        offset = 2000
    else:
        offset = 6000


    for d in dirs:
        try:
            filespe = f"{d}/sphe{channel}.txt"
            with open(filespe, "r") as f:
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
                snr.append(vals[0]/vals[5])
                 
        except Exception as error:
            # print(error)
            continue

        try:
            filemean = f"{d}/sphe_waveforms_Ch{channel}.root"
            filehist = f"{d}/sphe_histograms_Ch{channel}.root"

            with uproot.open(filehist) as fm:
                timeinter = fm['head'].arrays(["start","finish"])
                startc = timeinter["start"][0]
                endc = timeinter["finish"][0]
            with uproot.open(filemean) as fm:
                x, y = fm['mean'].values()
                ymax = np.max(y[ (x > startc) & (x < endc) ])
                dmax = (2**14 - offset)/ymax
                drange.append(dmax)
                spes.append(ymax)
                # print(startc, endc, ymax)

        except Exception as error:
            # print(error)
            continue


        val = re.findall("VGAIN_?([1,0]p?[0-9]?)", d)
        val = val[0]
        val = float(val.replace("p", "."))
        vgains.append(val)


    return vgains, snr, drange, spes

def make_plot(module, channels, xv, yv):
    sof = [ 1 if ch == 0 else 2 for ch in channels ]
    for s, x, y in zip(sof, xv, yv):
        if module == "VD" or module == "HD":
            _label = f'{module}'
        else:
            _label = f'{module} SoF {s}'
        if label != "":
            _label += f" - {label}"

        plt.plot(x, y, ':o', markersize=12, linewidth=2, label=_label)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-m", "--module", type=str, help='VD, HD, C1, C2, C3 or C4')
    parser.add_argument("-ch", "--channel", type=str, help='Channel number')
    parser.add_argument("-i", "--info", type=str, help='subfolder argument (something unique and handsome)')
    parser.add_argument("-l", "--label", type=str, help='Label for plots', default="")
    args = vars(parser.parse_args())

    label=args['label']
    module = args['module']
    channels = args['channel']
    info = args['info']
    if channels is None:
        channels = [0, 7]
    # else:
    # channels = [int(args['channel'])]

    vgains = [[] for _ in channels]
    snr = [[] for _ in channels]
    drange = [[] for _ in channels]
    spes = [[] for _ in channels]

    for i, ch in enumerate(channels):
        vgains[i], snr[i], drange[i], spes[i] = retrieve_info(module, ch)


    # print(vgains, snr, drange, spes)


    plt.figure()
    make_plot(module, channels, vgains, snr)
    plt.xlabel(r'$V_{GAIN} [V]$')
    plt.ylabel(r'SNR')
    plt.legend(loc='best', frameon=True)
    # plt.savefig(f'graphs/{module}_snr_vs_gain.png', dpi=200)

    plt.figure()
    make_plot(module, channels, vgains, spes)
    plt.xlabel(r'$V_{GAIN} [V]$')
    plt.ylabel(r'SPE amplitude [ADCs]')
    plt.legend(loc='best', frameon=True)
    # plt.savefig(f'graphs/{module}_spe_vs_gain.png', dpi=200)

    plt.figure()
    make_plot(module, channels, spes, snr)
    plt.xlabel(r'SPE amplitude [ADCs]')
    plt.ylabel(r'SNR')
    plt.legend(loc='best', frameon=True)
    # plt.savefig(f'graphs/{module}_snr_vs_spe.png', dpi=200)

    # plt.figure()
    # make_plot(module, channels, vgains, drange)
    # plt.xlabel(r'$V_{GAIN} [V]$')
    # plt.ylabel(r'Dynamic (norminal) [P.E.]')
    # plt.legend(loc='best', frameon=True)
    # plt.savefig(f'graphs/{module}_range_vs_gain.png', dpi=200)

    # plt.figure()
    # make_plot(module, channels, drange, snr)
    # plt.xlabel(r'Dynamic (norminal) [P.E.]')
    # plt.ylabel(r'SNR')
    # plt.legend(loc='best', frameon=True)
    # plt.savefig(f'graphs/{module}_snr_vs_range.png', dpi=200)



    # print(spes)



    plt.show()

