import numpy as np
import matplotlib.pyplot as plt
import mplhep
mplhep.style.use(mplhep.style.ROOT)
plt.rcParams.update({'font.size': 24,
                     'grid.linestyle': '--',
                     'axes.grid': True,
                     'figure.autolayout': True,
                     # 'text.usetex':True
                     })



sof = [1,2,1,2]
warmgain = [1,1,4,4]
label = ["4k", "4k", "1k", "1k"]
colors = ["darkblue", "red", "royalblue", "orange"]
module = "C3"

snr = [[] for _ in sof]
spes = [[] for _ in sof]
vgains = [[] for _ in sof]

for i, s in enumerate(sof):
    g = warmgain[i]
    data = np.genfromtxt(f"{module}_SoF_{s}_snr_vs_vgain_{g}k.csv",delimiter=',')
    vgains[i] = data[:,0]
    snr[i] = data[:,1]

for i, s in enumerate(sof):
    g = warmgain[i]
    data = np.genfromtxt(f"{module}_SoF_{s}_spes_vs_vgain_{g}k.csv",delimiter=',')
    spes[i] = data[:,1]

def make_plot(xv, yv):
    for i, (s, x, y) in enumerate(zip(sof, xv, yv)):
        _label = f'{module} SoF {s}'
        if label[i] != "":
            _label += f" - {label[i]}"

        plt.plot(x, y, ':o', markersize=12, linewidth=2, label=_label, color=colors[i])


plt.figure()
make_plot(vgains, snr)
plt.xlabel(r'$V_{GAIN} [V]$')
plt.ylabel(r'SNR')
plt.legend(loc='best', frameon=True)
plt.savefig(f'graphs/{module}_different_gains_snr_vs_gain.png', dpi=200)

plt.figure()
make_plot(vgains, spes)
plt.xlabel(r'$V_{GAIN} [V]$')
plt.ylabel(r'SPE amplitude [ADCs]')
plt.legend(loc='best', frameon=True)
plt.savefig(f'graphs/{module}_different_gains_spe_vs_gain.png', dpi=200)

plt.figure()
make_plot(spes, snr)
plt.xlabel(r'SPE amplitude [ADCs]')
plt.ylabel(r'SNR')
plt.legend(loc='best', frameon=True)
plt.savefig(f'graphs/{module}_different_gains_snr_vs_spe.png', dpi=200)
plt.show()

