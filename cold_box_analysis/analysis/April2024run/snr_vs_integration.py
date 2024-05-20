import matplotlib.pyplot as plt
import numpy as np
from glob import glob
import argparse

import mplhep
mplhep.style.use(mplhep.style.ROOT)
plt.rcParams.update({'font.size': 24,
                     'grid.linestyle': '--',
                     'axes.grid': True,
                     'figure.autolayout': True,
                     # 'text.usetex':True
                     })
def get_values(data):
    _window = []
    _snr = []
    for d in data:
        vals  = [ float(v) for v in d.strip("\n").split() ]
        _window.append(vals[1]-vals[0])
        _snr.append(vals[-1])
    return _snr, _window


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-m", "--module", type=str)
    args = vars(parser.parse_args())
    module = args['module']
    files = sorted(glob("spe_scan*.txt"))
    if len(files)!=2:
        print(f"ERROR!\n{files}")
        exit(0)

    contents = []
    for fname in files:
        f = open(fname, "r")
        contents.append([l for l in f.readlines() if l.strip()])

    data1, data2 = contents

    s1, w1 = get_values(data1)
    s2, w2 = get_values(data2)

    plt.plot(w1,s1,':o', markersize=12, linewidth=2, label=f"{module} SoF 1")
    plt.plot(w2,s2,':o', markersize=12, linewidth=2, label=f"{module} SoF 2")
    plt.xlabel("Integration window (ns)")
    plt.ylabel("SNR")
    plt.legend(loc='best', frameon=True)
    plt.savefig(f'../graphs/{module}_snr_vs_window.png', dpi=200)
    plt.show()

if __name__ == '__main__':
    main()

