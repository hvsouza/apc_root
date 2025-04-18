import numpy as np
import polars as pl
import matplotlib.pyplot as plt

import dunestyle.matplotlib as dunestyle
# dunestyle.enable()
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

def getdata(pdg=13):
    filename:str = ""
    pion_mass = 0.13957039
    muon_mass = 0.10565837
    mass = 0.
    if pdg == 13:
        filename = "splines_muon.txt"
        mass = muon_mass
    elif pdg == 211:
        filename = "splines_pion.txt"
        mass = pion_mass

    data = pl.read_csv(filename)

    data = data.with_columns(
        range = pl.col("CSDA"),
        E = pl.col('Kinetic') + mass,
        p = (pl.col('Kinetic') + mass)**2 - mass**2,
    )
    return data

datapi = getdata(211)
datamu = getdata(13)

mular = pl.read_csv("../MuStoppingPowerLAr/MuStoppingPowerLAr.csv", separator=',')
mular = mular.filter(
    pl.col('T') <= 4e5,
    pl.col('T') >= 10,
)
mular = mular.with_columns(
    E = ((pl.col('p')/1e3)**2+0.1057**2).sqrt(),
    R = pl.col('CSDA Range')/1.396
)
plt.plot(datapi['range'], datapi['Kinetic'], label='Pion')
plt.plot(datamu['range'], datamu['Kinetic'], label='Muon')
plt.plot(mular['R'], mular['T'], '--.', label='Muon in LAr')
plt.yscale('log')
plt.xscale('log')
plt.xlabel('Range [cm]')
plt.ylabel('Kinetic Energy [MeV]')
plt.legend()
plt.show()

print(len(datapi['range']))
for d in datapi['Kinetic']:
    print(f"{d:.6}", end=', ')
print()

