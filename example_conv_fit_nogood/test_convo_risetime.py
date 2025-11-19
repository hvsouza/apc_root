import numpy as np
import matplotlib.pyplot as plt
import pickle
from scipy import interpolate

nticks = 1024
timeres = 16 #ns

times = np.linspace(0,nticks*timeres, nticks, endpoint=False)

with open("template_run00_ch11114_avg.pkl", "rb") as f:
    template = pickle.load(f)[0]

templateinter = interpolate.interp1d(times, template, kind='linear', fill_value="extrapolate")
newtimes = np.linspace(0,(8*len(template))*2,8*len(template),endpoint=False)
template = templateinter(newtimes)

timeres /= 8
nticks *= 8
times = np.linspace(0,nticks*timeres, nticks, endpoint=False)




def lar_response(t, A, fp, t1, t3, offset):
    t0 = t - offset
    y = np.zeros(len(t0),dtype=float)
    _t = t0[t0>0]
    y[t0>0] = A*(fp*np.exp(-_t/t1)/t1 + (1-fp)*np.exp(-_t/t3)/t3)
    # return y
    return np.convolve(y,template,mode='full')[:len(y)]


larparameters = dict(t = times,
                     A = 5e3,
                     fp = 0.23,
                     t1 = 7,
                     t3 = 1600,
                     offset = 0,
                     )

for offset in np.arange(0, 2, 0.5):
    larparameters["offset"] = 2000//1+offset
    larparameters["A"] = 5e3
        
    response = lar_response(**larparameters)
    plt.plot(times, response, '.', label=f"{offset:.1f}")
for offset in np.arange(100, 104, 0.5):
    larparameters["offset"] = 2000//1+offset
    larparameters["A"] = 5e3
        
    response = lar_response(**larparameters)
    plt.plot(times, response, '.', label=f"{offset:.1f}")
plt.legend()
# plt.xlim(1980,2080)
# plt.yscale('log')
# plt.savefig("example.png", dpi=200)
plt.show()
