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
data = """
3300,3600,9.77
3300,3700,10.32
3300,3800,10.40
3300,3900,10.35
3300,4000,10.14
3300,4100,10.08
3300,4200,9.91
3300,4300,9.68
3300,4400,9.41
3300,4500,9.12
3300,4600,8.78
3300,4700,8.58
3300,4800,8.29
"""

# data = """
# 3100,3500,3.59
# 3100,3600,4.48
# 3100,3700,4.85
# 3100,3800,4.87
# 3100,3900,4.79
# 3100,4000,4.70
# 3100,4100,4.76
# 3100,4200,4.65
# 3100,4300,4.51
# 3100,4400,4.35
# 3100,4500,4.20
# 3100,4600,4.10
# 3100,4700,3.95
# 3100,4800,3.87
# """
data = data.strip().split()

window = []
snr = []
for d in data:
    vals  = [ float(v) for v in d.split(',') ]
    window.append(vals[1]-vals[0])
    snr.append(vals[-1])

plt.plot(window,snr,':o', markersize=12, linewidth=2, label="C3 SoF 1")
plt.xlabel("Integration window (ns)")
plt.ylabel("SNR")
plt.legend(loc='best', frameon=True)
plt.savefig(f'../graphs/C3_SoF1_snr_vs_window.png', dpi=200)
plt.show()


