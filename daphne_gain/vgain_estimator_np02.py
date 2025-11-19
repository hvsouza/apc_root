import numpy as np
import matplotlib.pyplot as plt
import mplhep
mplhep.style.use(mplhep.style.ROOT)
plt.rcParams.update({'font.size': 24,
                     'grid.linestyle': '--',
                     'axes.grid': True,
                     'figure.autolayout': True,
                     })

def vgain_to_vdac_mV(vgain):
    return (1.5 + 2.49) * vgain*1000 / 1.5

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-s", "--spe", type=float, help="SPE amplitude", default=17)
parser.add_argument("-a", "--attenuation", type=int, help="Current atteuation voltage", default=1995)
args = vars(parser.parse_args())
spe_amplitude = args['spe']
current_vadc_adu = args['attenuation']

spe_target = [10, 30, 40]
spe_factor = [ s/spe_amplitude for s in spe_target]

d = np.genfromtxt("vgain.txt")
vgain = d[:,0]
gaindb = d[:,1]
gain = [ 10**(g/20) for g in gaindb]

vdac = vgain_to_vdac_mV(vgain)
vdac_adu = vdac*(2**12-1)/4096

current_gain = np.interp(current_vadc_adu, vdac_adu, gain)
current_vgain = np.interp(current_vadc_adu, vdac_adu, vgain)

target_gain = [ g*current_gain for g in spe_factor]
target_vdac_adu = [ np.interp(g, sorted(gain), [ x for _, x in sorted(zip(gain, vdac_adu)) ] ) for g in target_gain]

print(f"Current VADC [ADU]: {current_vadc_adu}, Vgain: {current_vgain:.2f}, Gain: {current_gain:.02f}")
print(f"Target minimum gain: {target_gain[0]:.02f}, VDAC [ADU]: {target_vdac_adu[0]:.0f}")
print(f"Target maximum gain: {target_gain[-1]:.02f}, VDAC [ADU]: {target_vdac_adu[-1]:.0f}")

print(f"Scan: {target_vdac_adu[0]:.0f} - {target_vdac_adu[1]:.0f} -  {target_vdac_adu[-1]:.0f} ADU")

# plt.figure(figsize=(10,6))
# plt.plot(vgain, gain, 'o--', ms=10, label='Nominal')
# plt.ylabel('Gain')
# plt.xlabel(r'V$_{attenuation}$ [V]')
# plt.legend()
# plt.show()

plt.figure(figsize=(10,6))
plt.plot(vdac_adu, gain, 'o--', ms=10, label='Nominal')
plt.axvline(current_vadc_adu, 0, current_gain, color='red', ls='-', lw=2, label='')
plt.axhline(current_gain, 0, current_vadc_adu, color='red', ls='-', lw=2, label='')

plt.axvline(target_vdac_adu[0], 0, target_gain[0], color='orange', ls='--', label='')
plt.axhline(target_gain[0], 0, target_vdac_adu[0], color='orange', ls='--', label='')

plt.axvline(target_vdac_adu[-1], 0, target_gain[-1], color='orange', ls='--', label='')
plt.axhline(target_gain[-1], 0, target_vdac_adu[-1], color='orange', ls='--', label='')

plt.ylabel('Gain')
plt.xlabel(r'V$_{DAPHNE}$ [ADU]')
plt.legend()
plt.show()
