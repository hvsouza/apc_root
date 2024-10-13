####################################################################################################################################
#
# Library with all the necessary functions to read and perform preliminary analyzes of waveforms recorded in .dat extension
# from a CAEN digitizer.
#
# Author:Renan de Aguiar
#
#####################################################################################################################################

import os
import struct
import numpy as np
from scipy.signal import periodogram, find_peaks
#from ROOT import TH2F
from array import array
import matplotlib.pyplot as plt
from scipy.signal import lfilter, correlate, normalize
from scipy.optimize import curve_fit
from scipy.optimize import leastsq
from scipy.optimize import least_squares


#######################################################################
#
#	       		Essential functions 
#
#######################################################################

def rebining(data, bin_):
	vector = []
	for i in range(0, len(data), bin_):
		vector.append(data[i:i+ticks].sum()/bin_)
	return vector

def func_baseline(x):
	x = np.array(x)
	count = 0    
	while count < 10:
		#base = np.sqrt(np.mean(x**2))
		base = np.mean(x)
		var =  2*np.std(x)
		y = []
		for i in range(len(x)):
			if abs(x[i] - base) <= var:
	                	y.append(x[i])
		x = np.array(y)
		count += 1
		#print(base)
	return base
	
def func_baseline_v2(x):
	peak_height = 10
	peak_distance = 300
	
	x = np.array(x)
	ref = np.mean(x)
	
	peaks, _ = find_peaks(x, height = ref+peak_height, distance = peak_distance)		
	
	#print(peaks)
	#print(x[peaks])
	#plt.plot(x)
	x1 = x
	if len(peaks)>0:
		x_selection = []
		position = 0
		for i in range(len(x)):
			index = peaks[position]
			if i < index-100 or i > index+1000:
				x_selection.append(x[i])
			if i == index+1000:
				if position < len(peaks)-1:
					position += 1
		x = np.array(x_selection)
	
	count = 0
	while count < 10:
		base = np.sqrt(np.mean(x**2))
		var =  2*np.std(x)
		y = []
		for i in range(len(x)):
			if abs(x[i] - base) <= var:
	                	y.append(x[i])
		x = np.array(y)
		count += 1
	
	l = []
	l2 = []
	for i in range(len(x1)):
		l.append(ref)
		l2.append(base)
		
	#if ref < base:
	#	base = ref
	#plt.plot(l)
	#plt.plot(l2, c = 'k')
	#plt.show()
	return base
		

def moving_average(a,n):
	ret = np.cumsum(a, dtype=float)
	ret[n:] = ret[n:] - ret[:-n]
	d=a[0:n-1]	
	a=np.concatenate((d, ret[n-1:]/n), axis=None)
	return a

#######################################################################
#
#	       Reading and converting bytes into integers
#
#######################################################################

def dat_reader(d_path, RECORD_LENGTH, amount):
	
	print("	- Unpacking .dat file:")
	with open(d_path, 'rb') as f:
    		data = f.read()
    		
	# Header has  24 bytes
	h_size = 24
	# Waveforms have RECORD_LENGTH x 2bytes 
	w_size = RECORD_LENGTH*2

	max_count = 200 # quantity of waveforms for the 1st analyses 
	events = [] # List with header and waveforms of each event

	count = 0
	step = h_size + w_size
	wf = []
	count = 0
	for i in range(0,len(data), step):     # Selecting the indexes of each event start 
			# To separate the waveforms from the headers:
			if i+(h_size-1) < len(data):
				event = data[i+h_size : i + step]
				events.append(event)
			
			# To convert bytes into integers
			tick = 0
			count2 = 1
			waveform = []
			for j in range(0,len(event),2):
				waveform_bin = event[j: j+2]
				waveform_int = (struct.unpack('h', waveform_bin)[0])	
				waveform.append(waveform_int)
			
			if amount != "all":
				if count < amount:
					wv = np.array(waveform)
					wf.append(wv)
					#plt.plot(wv)
					#plt.show()
					count += 1
					#print("		Waveforms recorded: ", count) 
				else:
					break
			
			else: 
				wv = np.array(waveform)
				wf.append(wv)
				if (i == 0.2*(len(data))) : print("		20% done ...")
				if (i == 0.4*(len(data))) : print("		40% done ...")
				if (i == 0.6*(len(data))) : print("		60% done ...")
				if (i == 0.8*(len(data))) : print("		80% done ...")

	print("		100% done!")
	return wf

def dat_selector(d_path, RECORD_LENGTH, amount, min_, max_):
	
	print("	- Unpacking .dat file:")
	with open(d_path, 'rb') as f:
    		data = f.read()
    		
	# Header has  24 bytes
	h_size = 24
	# Waveforms have RECORD_LENGTH x 2bytes 
	w_size = RECORD_LENGTH*2

	max_count = 200 # quantity of waveforms for the 1st analyses 
	events = [] # List with header and waveforms of each event

	count = 0
	step = h_size + w_size
	wf = []
	count = 0
	for i in range(0,len(data), step):     # Selecting the indexes of each event start 
			# To separate the waveforms from the headers:
			if i+(h_size-1) < len(data):
				event = data[i+h_size : i + step]
				events.append(event)
			
			# To convert bytes into integers
			tick = 0
			count2 = 1
			waveform = []
			for j in range(0,len(event),2):
				waveform_bin = event[j: j+2]
				waveform_int = (struct.unpack('h', waveform_bin)[0])	
				waveform.append(waveform_int)
			waveform = waveform[min_:max_]
			if amount != "all":
				if count < amount:
					wv = np.array(waveform)
					wf.append(wv)
					#plt.plot(wv)
					#plt.show()
					count += 1
					print("		Waveforms recorded: ", count) 
				else:
					break
			
			else: 
				wv = np.array(waveform)
				wf.append(wv)
				if (i == 0.2*(len(data))) : print("		20% done ...")
				if (i == 0.4*(len(data))) : print("		40% done ...")
				if (i == 0.6*(len(data))) : print("		60% done ...")
				if (i == 0.8*(len(data))) : print("		80% done ...")

	print("		100% done!")
	return wf

def dat_selector_withbaseline(d_path, RECORD_LENGTH, amount, signal, min_, max_, min_b, max_b):
	
	print("	- Unpacking .dat file:")
	with open(d_path, 'rb') as f:
    		data = f.read()
    		
	# Header has  24 bytes
	h_size = 24
	# Waveforms have RECORD_LENGTH x 2bytes 
	w_size = RECORD_LENGTH*2

	max_count = 200 # quantity of waveforms for the 1st analyses 
	events = [] # List with header and waveforms of each event

	count = 0
	step = h_size + w_size
	wf = []
	count = 0
	for i in range(0,len(data), step):     # Selecting the indexes of each event start 
			# To separate the waveforms from the headers:
			if i+(h_size-1) < len(data):
				event = data[i+h_size : i + step]
				events.append(event)
			
			# To convert bytes into integers
			tick = 0
			count2 = 1
			waveform = []
			for j in range(0,len(event),2):
				waveform_bin = event[j: j+2]
				waveform_int = (struct.unpack('h', waveform_bin)[0])	
				waveform.append(waveform_int)
			
			baseline = waveform[min_b:max_b]
			waveform = waveform[min_:max_]
			b = func_baseline(baseline)
			#b2 = func_baseline(baseline)
			#b3 = func_baseline_v3(baseline)
			
			#plt.plot(np.array(baseline))
			blist = []
			#blist2 = []
			#blist3 = []
			for i in range(len(baseline)):
				blist.append(b)
				#blist3.append(b3)
			
			#plt.plot(np.array(blist), c = 'k')
			#plt.plot(np.array(blist3), c = 'red')
			#plt.show()
			
			if signal == 'positive':
				w = np.array(waveform) - b
				#w3 = np.array(waveform) - b3
			else:
				w = b - np.array(waveform)
				#w3 = b3 - np.array(waveform)			
			
			if amount != "all":
				if count < amount:
					#peaks, _ = find_peaks(w)
					#pos = np.where(w == max(w[peaks]))
					#print(len(pos))
					
					#if len(pos) == 1:
					# pos[0]<200:
					wf.append(w)
						#print(b)
					#plt.plot(w, c = 'k')
					#plt.show()
						#plt.plot(w3, c ='red')
					count += 1
					
					
					#if w.sum() > 1000 and w.sum() <2000:
						#print(w.sum())
						
						#plt.plot(np.array(baseline))
						#plt.plot(np.array(blist), c = 'k')
						#plt.show()
						#plt.plot(w)
						#plt.show()
					
					
					print("		Waveforms recorded: ", count) 
				else:
					break
			
			else: 
				wf.append(w)
				if (i == 0.2*(len(data))) : print("		20% done ...")
				if (i == 0.4*(len(data))) : print("		40% done ...")
				if (i == 0.6*(len(data))) : print("		60% done ...")
				if (i == 0.8*(len(data))) : print("		80% done ...")

	#plt.show()
	print("		100% done!")
	return wf

#######################################################################
#
#	       Waveform analyses and modifications
#
#######################################################################
	
def rebining_waveforms(waveform, ticks):
	data = []
	for i in range(len(waveform)):
		data.append(rebining(waveform[i], ticks))
	return data
	
def mean_waveform(data, number):
	print("Calculating the mean waveform ...")
	sum_waveforms = np.zeros(len(data[0]))
	for i in range(0, number, 1):
		wf = np.array(data[i])
		sum_waveforms += wf
	wv_final = sum_waveforms/number
	print(wv_final)
	print(len(wv_final))
	plt.plot(wv_final)
	plt.xlabel("Ticks")
	plt.ylabel("ADC")
	#plt.savefig("MeanWaveform.png")
	plt.show()
	return wv_final

def wf_baseline(waveform, baseline, signal):
	wf_final = []
	for i in range(len(waveform)):
		b = func_baseline(baseline[i])
		bx = []
		l = 0
		while l < len(waveform[i]):
			bx.append(b)
			l += 1
		#plt.plot(waveform[i], 'red')
		#plt.plot(bx, 'k')
		#plt.show()
		if signal == 'positive':
			wf = np.array(waveform[i]) - b
		else:
			wf = b - np.array(waveform[i])			
		
		wf_final.append(np.array(wf))
	
	return wf_final
	
#######################################################################
#
#	       		      Filters
#
#######################################################################
		
def filter(data, template, noisePDS):
	t0 = np.zeros(len(data[0]))
	for i in range(len(template)):
		t0[i] = template[i]
	
	t = np.fft.fft(t0)
	template_fft = []
	for i in range(int(len(t)/2)+1):
		template_fft.append(t[i])
	template_fft = np.array(template_fft)
	n_PDS = np.array(noisePDS)
	G = np.conj(template_fft)/(np.abs(template_fft)**2 + np.abs(n_PDS)**2)
	
	for i in range(len(data)):
		data_fft = np.fft.fft(data[i])
		data_filt = data_fft*G
		data_final = np.fft.ifft(data_filt)
	return data_final
		
def matched_filter(waveform, template0, number):
	print('Applying matched filter: ')
	template = template0[::-1]
	out = []
	ng = len(template)
	for i in range(len(waveform)):
		sample = waveform[i][number:]
		nf = len(sample)
		n = nf + ng -1
		out_vector = []
		for j in range(n):
			if(j >= ng -1): jmn = j - (ng - 1)
			if(j < ng -1): 	jmn = 0
			if(j >= nf -1): jmx = nf - 1
			else : 
				jmx = j 
				
			out_value = 0
			
			k = jmn
			while k <= jmx:
				#print(ng,i, n, jmn, jmx, k, j)
				out_value += sample[k]*template[j-k] 
				k += 1
				
			out_vector.append(out_value)
		out.append(np.array(out_vector))
		
		x1 = np.linspace(0, nf, nf)
		x1 = x1*0.4

		x2 = np.linspace(0, len(out_vector), len(out_vector))
		x2 = x2*0.4		
		
		fig, (ax1, ax2) = plt.subplots(2)
		ax1.plot(x1,sample)
		ax2.plot(x2,out_vector)
		fig.supxlabel('Time (µs)')
		fig.supylabel('ADC')
		plt.show()
	return out
		
def wf_mafilter(waveform, n):
	wf_final = np.empty((len(waveform), len(waveform[0])))
	for i in range(len(waveform)):
		wf_final[i] = moving_average(waveform[i], n)
	
	return wf_final
	
def selector(input_, heigth, number):
	data = []
	for i in range(len(input_)):
		x = input_[i][number:]	
		peaks, _ = find_peaks(x)
		if round(max(x[peaks]),2) >= heigth:
			#print(max(x[peaks]))
			for j in range(len(peaks)):
				if x[peaks[j]] == max(x[peaks]) and j>500:
					ref = peaks[j]
					if len(x[ref-500:ref+3000]) == 3500:
						#plt.plot(x[ref-500:ref+3000])
						#plt.show()
						data.append(x[ref-500:ref+3000])
	return data
	
def denoise(input, width, lambda_value):

	if width > 0:
		k, k0 = 0, 0
		umin, umax = lambda_value, -lambda_value
		vmin, vmax = input[0] - lambda_value, input[0] + lambda_value
		kplus, kminus = 0, 0
		twolambda = 2.0 * lambda_value
		minlambda = -lambda_value        
		output = [0] * width
        
	while True:
		while k == width - 1:
		    	if umin < 0.0:
		    		while k0 <= kminus:
		    			output[k0] = vmin
		    			k0 += 1
		    		umin = lambda_value
		    		k = k0
		    		kminus = k
		    		umax = vmin = input[kminus] + (umin) - vmax
		    	
		    	elif umax > 0.0:
		    		while k0 <= kplus:
		    			output[k0] = vmax
		    			k0 += 1
		    		umax = minlambda
		    		k = k0
		    		kplus = k
		    		umin = vmax = input[kplus] + (umax) - vmin
		    	
		    	else:
		    		vmin += umin / (k - k0 + 1)
		    		while k0 <= k:
		    			output[k0] = vmin
		    			k0 += 1
		    		return output 
            		
		umin = input[k+1]-vmin+umin
		if(umin < minlambda):
			while k0 <= kminus:
				output[k0] = vmin
				k0 += 1
			
			k = k0
			kplus = kminus
			vmax = vmin = input[kplus] + twolambda
			umin = lambda_value
			umax = minlambda
		umax = input[k+1]-vmax+umax  
		
		if(umax  > lambda_value):
                	while k0 <= kplus:
                		output[k0] = vmax
                		k0 += 1
                	k = k0
                	kminus = k
                	vmin = vmax = input[kplus] - twolambda
                	umin = lambda_value
                	umax = minlambda
		else:
			k += 1
			if umin >= lambda_value:
				kminus = k
				vmin += (umin - lambda_value) / (kminus) - k0 + 1
				umin = lambda_value
			if umax <= minlambda:
				kplus = k
				vmax += (umax + lambda_value) / (kplus) - k0 + 1
				umax = minlambda
	
#######################################################################
#
#	       		      Charge
#
#######################################################################

def charge(waveform, time_scale, n):
	charge = []
	for i in range(len(waveform)):
		#data = denoise(waveform[i], len(waveform[i]), 3)
		data = waveform[i]
		w = np.array(data[n:])
		#if w[0] > -1 and w[0] < 3:
			#print(w.sum())
			#plt.plot(w)
			#plt.show()
		charge.append(w.sum())  
	return charge
	
def charge_selector(input_, heigth, number, template):
	#data = []
	charge_total = []
	for i in range(len(input_)):
		x = input_[i][number:]	
		peaks, _ = find_peaks(x)
		if round(max(x[peaks]),2) >= heigth:
			#print(max(x[peaks]))
			for j in range(len(peaks)):
				if x[peaks[j]] == max(x[peaks]) and j>500:
					ref = peaks[j]
					min_ = min(x[ref-500:ref+3000])
					if len(x[ref-500:ref+3000]) == 3500:
						charge = (np.array(x[ref-500:ref+3000])-min_).sum()
						y = lfilter(template, 1, x[ref-500:ref+3000])
						#data.append(x[ref-500:ref+5000])
					#print(charge)
						#plt.show()
					#if charge > 0:
						#plt.plot(y)
						#plt.plot(x[ref-500:ref+3000])
						#plt.show()
						#charge_total.append(charge)
		#for k in range(j, len(x)):
		#	charge_value += x[k]
		#	if x[k] < baseline:
		#		break 
		#charge_total.append(charge_value)	
	return charge_total

#######################################################################
#
#	       		      Histograms
#
#######################################################################

def hist_1D(data, bin_):
	plt.hist(data, bin_)
	plt.ylabel ( " Entries " )
	plt.xlabel ( " Charge (ADC x ns) ")
	plt.show()
	
def hist_2D(data, bin_x, bin_y, amount):

	print("Creating a 2D histogram ...")
	x = []
	y = []
	
	for i in range(amount):
		for j in range(len(data[i])):
			y.append(data[i][j])
			x.append(j)
	
	plt.hist2d(x, y, [bin_x, bin_y])
		
	plt.ylabel ( " ADC counts " )
	plt.xlabel( " Ticks " )
	plt.savefig("2DWaveformHist.png")
	plt.show()

	

	


	
