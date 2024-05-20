import matplotlib.pyplot as plt
import numpy as np

import mplhep
mplhep.style.use(mplhep.style.ROOT)
plt.rcParams.update({'font.size': 24,
                     'grid.linestyle': '--',
                     'axes.grid': True,
                     'figure.autolayout': True,
                     # 'text.usetex':True
                     })
data1 = """
3200 3600 8.06
3200 3700 8.69
3200 3800 8.90
3200 3900 9.10
3200 4000 9.19
3200 4100 9.23
3200 4200 9.05
3200 4300 8.86
3200 4400 8.75
3200 4500 8.69
3200 4600 8.61
3200 4700 8.44
3200 4800 8.42
3200 4900 8.19
3200 5000 7.95
3200 5100 7.83
3200 5200 7.65
"""

data2 = """
3200 3600 5.09
3200 3700 5.39
3200 3800 5.57
3200 3900 5.85
3200 4000 6.14
3200 4100 6.64
3200 4200 7.10
3200 4300 7.56
3200 4400 7.95
3200 4500 8.19
3200 4600 8.28
3200 4700 8.25
3200 4800 8.05
3200 4900 7.76
3200 5000 7.50
3200 5100 7.13
3200 5200 6.96
"""


def get_values(data):
    _window = []
    _snr = []
    data = data.strip().split("\n")
    for d in data:
        vals  = [ float(v) for v in d.split() ]
        _window.append(vals[1]-vals[0])
        _snr.append(vals[-1])
    return _snr, _window

s1, w1 = get_values(data1)
s2, w2 = get_values(data2)

plt.plot(w1,s1,':o', markersize=12, linewidth=2, label="C1 SoF 1")
plt.plot(w2,s2,':o', markersize=12, linewidth=2, label="C1 SoF 2")
plt.xlabel("Integration window (ns)")
plt.ylabel("SNR")
plt.legend(loc='best', frameon=True)
plt.savefig(f'./graphs/snr_vs_window.png', dpi=200)
plt.show()


