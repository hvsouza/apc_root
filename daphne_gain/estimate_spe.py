import numpy as np
import matplotlib.pyplot as plt
# spe = [50, 40, 30, 18, 12, 5, 5, 4, 2,1.8, 1]
spe = [55.7201309867142, 40.76408775125621, 28.975268685541987, 20.09170269186877, 13.412699506983246, 9.022704294210172, 6.096564031064066]
idxref = 1
value_ref = 9
sref = spe[idxref]
spe = [ g/sref for g in spe]
d = np.genfromtxt("vgain.txt")
vgain = d[:,0]
vgainspe = d[:,0][:len(spe)]
gain = d[:,1]
gain = [ 10**(g/20) for g in gain]
gref = gain[idxref]
gain = [ g/gref for g in gain]
values_spe = [ g*value_ref*0.8 for g in gain]
print(values_spe)
plt.plot(vgain, gain)
plt.plot(vgainspe, spe)
plt.show()
