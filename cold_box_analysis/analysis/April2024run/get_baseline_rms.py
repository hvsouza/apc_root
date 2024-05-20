import numpy as np
import argparse
from glob import glob
import uproot


def convert_mV(reference, offset=0.1):
    return (reference-0.1*2**14)*2000/2**14 
if __name__ == "__main__":
    parse = argparse.ArgumentParser()
    # parse.add_argument('-ch', '--channel', type=int, help="channel number")
    parse.add_argument('-f', '--folder', type=str, help="Folder in regex mode to be evaluated")
    args = vars(parse.parse_args())

    # channel = args['channel']
    folder = args['folder']

    files = sorted(glob(f"./{folder}"))

    dtime = 2
    start_signal = 2300 # in ns
    for f in files:
        print(f"Running over {f}")
        rootfname = f"{f}/analyzed.root"
        rootf = uproot.open(rootfname)
        keys = rootf.keys()
        ch = []
        for k in keys:
            if k.startswith("average_ch"):
                tmp = k.split("ch")[1].split(";")[0]
                ch.append(int(tmp))
        ch = sorted(ch)

        base = [ rootf[f't1:Ch{c}.base'].array(library='np') for c in ch ]
        print("\tReference base, baseline (mV), RMS, SNR, Q1, Q2-Q1")
        for c, b in zip(ch, base):
            value_base = np.mean(b)
            value_mv = convert_mV(value_base)
            wvf = rootf[f't1:Ch{c}.wvf'].array()[:,:int(start_signal/dtime)]
            
            all_rms = np.sqrt(np.mean((wvf)**2,axis=-1))
            inside90percent = np.quantile(all_rms,0.9)
            all_rms = all_rms[all_rms<inside90percent]
            rms = np.mean(all_rms)
            fsphename = f"{f}/sphe{c}.txt"
            with open(fsphename, "r") as fsphe:
                fsphe.seek(0)
                lines = fsphe.read().splitlines()
                lastline = lines[-1]
                vals = [ float(v) for v in lastline.split() ]
                mu1 = vals[0]
                mu2 = vals[1]
                mu21 = vals[2]
                sigma1 = vals[3]
                b = vals[4]
                sigmab = vals[5]
                snr = vals[0]/vals[5]
            print(f"\tCh{c}: {value_base:.2f}, {value_mv:.2f}, {rms:.2f}, {snr:.2f}, {mu1:.2f}, {mu2-mu1:.2f}")
            # print(f"\tCh{c}: {rms:.2f} {inside90percent:.2f}")




    
