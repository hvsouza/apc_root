import numpy as np
import matplotlib.pyplot as plt

import dunestyle.matplotlib as dunestyle
from matplotlib import cycler
plt.rcParams.update(
    {
    "axes.prop_cycle":cycler(color=['#1f77b4',
                                    '#ff7f0e',
                                    '#2ca02c',
                                    '#d62728',
                                    '#9467bd',
                                    '#8c564b',
                                    '#e377c2',
                                    '#7f7f7f',
                                    '#bcbd22',
                                    '#17becf'])
    })
plt.rcParams.update({
                     'axes.grid': True,
                     'grid.linestyle': '--',
                     'xaxis.labellocation': 'right',
                     'yaxis.labellocation': 'top',
                     })

dt = np.dtype([
    ('p',np.float32),
    ('dEdx',np.float32)
])
data = np.genfromtxt("MuStoppingPowerLAr.csv", delimiter=',', dtype=dt)
# data = np.fromfile("MuStoppingPowerLAr.txt", dtype=dt)

E = np.sqrt((data['p']/1e3)**2+0.1057**2)
plt.plot(E, data['dEdx']*1.396)
plt.xlim(0.1,100)
plt.ylim(1,20)
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Momentum [GeV/c]')
plt.ylabel(r'Stopping power [MeV cm$^2$/g]')
plt.show()

for d in E:
    print(f"{d:.6}", end=', ')
print()



