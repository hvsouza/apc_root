import matplotlib.pyplot as plt
import numpy as np

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


def tH0(p, segsize):
    def mom_dep_c(p):
        a = 0.1049
        c = 11.0038
        return (a/(p*p)) + c

    m_muon = 0.1057
    beta = np.sqrt( 1 - m_muon**2/(p**2+m_muon**2) )
    dth0 = (mom_dep_c(p)/(p*beta))*np.sqrt(segsize/14)*(1+0.038*np.log(segsize/14/beta/beta))

    return dth0

p = np.logspace(np.log10(0.047),2,200)

plt.plot(p, tH0(p,10))
plt.yscale('log')
plt.xscale('log')
plt.show()


