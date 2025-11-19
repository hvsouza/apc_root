import numpy as np
import matplotlib.pyplot as plt
import mplhep
mplhep.style.use(mplhep.style.ROOT)
plt.rcParams.update({'font.size': 24,
                     'grid.linestyle': '--',
                     'axes.grid': True,
                     'figure.autolayout': True,
                     })
import argparse


parser = argparse.ArgumentParser()
parser.add_argument("-i", "--idx", type=int, help="Reference index", default=3)
parser.add_argument("-v", "--value", type=float, help="Value of reference", default=20)
args = vars(parser.parse_args())



# spe = [50, 40, 30, 18, 12, 5, 5, 4, 2,1.8, 1]
spe = [55.7201309867142, 40.76408775125621, 28.975268685541987, 20.09170269186877, 13.412699506983246, 9.022704294210172, 6.096564031064066]
spe = [50.85118443590993, 36.019707510497874, 25.48418000015771, 16.763345924727172, 10.733535989887404, 7.046285120846436, 4.9131816711212855, 3.1644100274594766]
idxref = args['idx']
value_ref = args['value']
sref = spe[idxref]
#
spe = [ g/sref for g in spe]
d = np.genfromtxt("vgain.txt")
vgain = d[:,0]
vgainspe = d[:,0][:len(spe)]
gain = d[:,1]
gain = [ 10**(g/20) for g in gain]
gref = gain[idxref]
gain = [ g/gref for g in gain]
values_spe = [ g*value_ref*0.8 for g in gain]
# print(values_spe)

plt.figure(figsize=(10,6))
plt.plot(vgain, gain, 'o--', ms=10, label='Nominal')
plt.plot(vgainspe, spe, 'o--', ms=10, label='Measured')
plt.ylabel('Relative gain')
plt.xlabel(r'V$_{gain}$ [V]')
plt.legend()
plt.show()
