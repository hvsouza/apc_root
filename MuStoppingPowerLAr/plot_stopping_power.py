import numpy as np
import polars as pl
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

data = pl.read_csv("MuStoppingPowerLAr.csv", separator=',')
data = data.filter(
    pl.col('T') <= 4e5,
    pl.col('T') >= 10,
)
data = data.with_columns(
    E = ((pl.col('p')/1e3)**2+0.1057**2).sqrt(),
    R = pl.col('CSDA Range')/1.396
)
plt.plot(data['E'], data['dE/dx']*1.396)
plt.xlim(0.8*data['E'][0],100)
plt.ylim(1,100)
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Momentum [GeV/c]')
plt.ylabel(r'Stopping power [MeV cm$^2$/g]')
plt.show()

print(len(data['CSDA Range']))
for d in data['dE/dx']:
    print(f"{d:.6}", end=', ')
print()

plt.figure()
plt.plot(data['R'], data['T'])
plt.show()




