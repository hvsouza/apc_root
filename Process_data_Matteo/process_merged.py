import uproot
import numpy as np
import matplotlib.pyplot as plt
import awkward as ak
import pandas as pd
import polars as pl
from tqdm import tqdm
import argparse

if __name__ == "__main__":
    parse = argparse.ArgumentParser()
    parse.add_argument('-f', '--file', type=str, help="File name (WITHOUT .root), default = merged", default="merged")
    parse.add_argument('-o', '--ofile', type=str, default='', help="Output name, no extension (optional)")
    parse.add_argument('-n', '--n_events', type=int, default=None, help="Maximum events to process")
    args = vars(parse.parse_args())

    filename:str
    filename =  args['file']
    if ".root" in filename:
        for _ in range(3):
            print("COGLIONE!")
        filename = filename.replace(".root", "")
    outputname = args['ofile']
    if outputname == '':
        outputname = filename

    nevents=args['n_events']

    t1:uproot.TTree
    t1 = uproot.open(f"{filename}.root:myanalysis/t1")

    cols = [key for key in t1.keys()]

    # variables not being used
    exclusion_list = ['genTrueMomentum', 'genStartMomentumX', 'genStartMomentumY', 'genStartMomentumZ', 'genEndMomentumX', 'genEndMomentumY', 'genEndMomentumZ', 'numuMichelTrueEnergy', 'genNuMuHasMuTrack', 'mcParticleTrueMomentum', 'mcMichelNumDaughters', 'mcMichelTrueMomentum', 'pfpTrackChi2', 'pfpTrackStartNdof', 'pfpWhichPdg', 'pfpTracksDistance', 'pfpNHits', 'pfpNHitsView', 'pfpSapcePointId', 'pfpSapcePointX', 'pfpSapcePointY', 'pfpSapcePointZ', 'pfpNClusters', 'pfpCluNHits' ]

    # all other
    cols_tmp = [x for x in cols if all(exclusion_item not in x for exclusion_item in exclusion_list)]

    # split into vectors and single variables
    cols_vec = []
    cols_single = []
    for c in cols_tmp:
        b:uproot.TBranch
        b = t1[c]
        if "vector" not in b.typename:
            # print(c, b.typename)
            cols_single.append(c)
        else:
            cols_vec.append(c)

    nrepeat = [ len(v) for v in t1['pfpTrackID'].array(entry_stop=nevents) ]
    arr = {}
    print("Repeating single values")
    for c in tqdm(cols_single):
        v = t1[c].array(entry_stop=nevents)
        arr[c] = np.repeat(v, nrepeat)
    
    print("Collecting vectors")
    for c in tqdm(cols_vec):
        v = t1[c].array(entry_stop=nevents)
        flat = ak.ravel(v)
        arr[c] = flat

    nreference = len(arr['pfpTrackID'])
    toremove=[]

    print("Cleaning...")
    for c, v in arr.items():
        if len(v) != nreference:
            print(f'Discarding {c}, as it is different then pfpTrackID')
            toremove.append(c)
    for rmc in toremove:
        arr.pop(rmc)

    print('Creating data frame...')
    dfpd = pd.DataFrame(arr)
    df = pl.from_pandas(dfpd)
    print(df)

    print('Saving data frame...')
    df.write_parquet(f"{outputname}.parquet")
