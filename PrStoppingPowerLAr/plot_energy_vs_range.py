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

def larsoft_thing(trkrange):
    KE = 0
    if trkrange > 0 and trkrange <= 80:
        KE = 29.9317 * trkrange**0.586304
    elif trkrange > 80 and trkrange <= 3.022e3:
     KE = 149.904 + (3.34146 * trkrange) + (-0.00318856 * trkrange * trkrange) + \
          (4.34587e-6 * trkrange * trkrange * trkrange) + \
          (-3.18146e-9 * trkrange * trkrange * trkrange * trkrange) + \
          (1.17854e-12 * trkrange * trkrange * trkrange * trkrange * trkrange) + \
          (-1.71763e-16 * trkrange * trkrange * trkrange * trkrange * trkrange * trkrange)

    return KE

data = pl.read_csv("./PrStoppingPowerLAr.csv")

data = data.with_columns(
    range = pl.col("CSDA")/1.396,
    E = pl.col('Kinetic') + 938.272,
    p = ((pl.col('Kinetic') + 938.272)**2 - (938.272)**2).sqrt(),
)

fakeR = np.linspace(data['range'].min(), data['range'].max(),1000)
fakeK = [ larsoft_thing(R) for R in fakeR ]
fakeP = [ np.sqrt((938.272 + larsoft_thing(R))**2 - (938.272)**2) for R in fakeR ]


plt.plot(data['range'], data['p'])
plt.plot(fakeR, fakeP)
plt.show()
