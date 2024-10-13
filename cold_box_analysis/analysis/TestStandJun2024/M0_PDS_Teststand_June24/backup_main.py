####################################################################################
###    Author: Renan de Aguiar
###  Script to read the CAEN binaries and save the charge information into a Root
###  file for PDS analysis pourpose.



import functions
import os
import pandas as pd
import numpy as np
#from ROOT import TFile, TTree
import matplotlib.pyplot as plt
from array import array
import scipy
from scipy.signal import lfilter, correlate, normalize
from scipy.signal import periodogram, find_peaks, savgol_filter

###################################################################################
############################# Configuration

path = os.getcwd()
config = pd.read_csv("Configure", sep = ' ', header = None)

####################################################################################
############################# Samples

d = str(config.iloc[0,1]) # file path
RECORD_LENGTH = int(config.iloc[1,1]) # length in ticks
Resolution = 2**int(config.iloc[2,1]) # resolution in ADC

n = int(config.iloc[3,1]) # for the moving average
#limits of the waveforms
wf_l = int(config.iloc[4,1])
wf_u = int(config.iloc[5,1]) 
#baselines limits
bl_l = int(config.iloc[6,1])
bl_u = int(config.iloc[7,1])

print("Reading ", d, " file:")
print("	- Configurations:") 
print("		Record legth :",  RECORD_LENGTH)
print("		Resolution :",  Resolution)
print("		Baseline Limits : [", bl_l, ",", bl_u, "];" )
print("		Waveform Limits : [", wf_l, ",", wf_u, "];")

d_path = path + '/Data/' + d

amount = 10000
signal = 'positive'

wf = functions.dat_selector_withbaseline(d_path, RECORD_LENGTH, amount, signal, wf_l, wf_u, bl_l, bl_u)
wf2 = functions.wf_mafilter(wf, n)

wf3 = []
for i in range(len(wf2)):
	wf3.append(savgol_filter(wf2[i], 20, 1))

wf_mean = functions.mean_waveform(wf3, amount)
charge = functions.charge(wf2, 4, n)
functions.hist_1D(charge, 1000)





