#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import os
import glob
import re
import os

class Waveforms:
    def __init__(self, datfileslist:list, channels:list = None, npts = 2048):
        self.npts = npts
        self.dt = np.dtype([('wvfs', (np.uint16, self.npts))])
        self.fileslist = {}
        if channels is None:
            channels = []
            for file in datfileslist:
                p = re.findall(r"channel_(\d+).dat", file)
                ch = int(p[0])
                channels.append(ch)
                self.createfilelist(ch, file)
        else:
            for ch in channels:
                for file in datfileslist:
                    p = re.findall(r"channel_(\d+).dat", file)
                    if int(p[0]) == ch:
                        self.createfilelist(ch, file)
                


        self.channels = channels
        self.wvfs = { ch: None for ch in self.channels }
        for ch, file in self.fileslist.items():
            if not (os.stat(file).st_size/(self.dt.itemsize)).is_integer():
                print(f"Waveforms might not have the length of {self.npts} or data is trucated.")
                print(f"Filesize (bytes): {os.stat(file).st_size}")
                print(f"Waveform size (bytes): {self.dt.itemsize}")
            self.read_data(file, ch)

    
    def read_data(self, filename, ch):
        print(f"Processing {filename} of ch {ch} ... ")
        data = np.fromfile(filename, dtype=self.dt)
        self.wvfs[ch] = data['wvfs']
    
    def createfilelist(self, ch, file):
        if ch not in self.fileslist.keys():
            self.fileslist[ch] = file
        else:
            print("For now, only one file per channel...")
           



# example of usage
if __name__ == "__main__":
    datfiles = sorted(glob.glob("channel_*.dat"))
    data = Waveforms(datfiles, npts=2048)
    print(data.wvfs[8][:2])

