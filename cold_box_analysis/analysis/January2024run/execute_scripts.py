import numpy as np
import argparse
from glob import glob
import subprocess
import re
import os
from io import StringIO

if __name__ == "__main__":
    parse = argparse.ArgumentParser()
    parse.add_argument("code", type=str, help="Code name")
    parse.add_argument('-f', '--folder', type=str, help="Folder in regex mode to be evaluated")
    parse.add_argument("-nc", "--no_channel", action="store_true", help="Don't try to get any channel")
    parse.add_argument("-c", "--c", action="store_true", help="Compile code")
    parse.add_argument("-b", "--b", action="store_true", help="Batch mode")
    parse.add_argument("-q", "--q", action="store_true", help="quite after")
    parse.add_argument("-n", "--dry",  action="store_true", help='Dry run')
    parse.add_argument('-ch','--channels', type=int, nargs="+", help="Channels to look")
    args = vars(parse.parse_args())
    args = vars(parse.parse_args())

    folder = args['folder']
    code = args['code']
    no_channel = args['no_channel']
    doplus = args['c']
    isbatch = args['b']
    doquit = args['q']
    dry = args['dry']
    if args['channels'] is not None:
        no_channel = True
    else:
        channels = []
    if doplus:
        code += "+"

    maindir = os.getcwd()

    files = sorted(glob(f"./{folder}"))
    if dry:
        for f in files:
            print(f)


    results = {}
    for f in files:
        # print(f"Running over {f}")
        channels = []
        if not no_channel:
            wavefiles = sorted(glob(f"{f}/*_wave*.dat"))
            for w in wavefiles:
                channels.append(re.findall(r"wave(\d+)",w)[0])

            if not wavefiles:
                wavefiles = sorted(glob(f"{f}/../sphe_h*.root"))
                for w in wavefiles:
                    channels.append(re.findall(r"sphe_histograms_Ch(\d+)",w)[0])

            if not channels:
                # print("No file found :o", f)
                continue
            # print(channels)
        else:
            if args['channels'] is not None:
                channels = args['channels']
            else:
                channels.append('')

        p = re.findall(r"run(\d+)_", f)
        run = int(p[0])
        
        os.chdir(f)
        with open(f'../config_used.log', 'r') as flog:
            lines = flog.readlines()
            lines = [ line.strip() for line in lines ]
            for line in lines:
                try:
                    if line.split()[0] == "RECORD_LENGTH":
                        npts = int(line.split()[1])
                        break
                except:
                    continue

        # if npts==10000:
        #     os.chdir(maindir)
        #     continue
        print(f"Inside {f}")


        for c in channels:
            execthis = f"{maindir}/{code}({c})"
            command = ["root","-l"]
            if isbatch: command.append("-b")
            if doquit: command.append("-q")
            command.append(execthis)
            if dry:
                print(command)
                continue
            ret = subprocess.run(command)
            # ret = subprocess.run(command, stdout=subprocess.PIPE)
            # dout = ret.stdout.decode('utf-8')
            # dout = StringIO(dout)
            # for l in dout.readlines():
            #     p = re.findall(r"A: (.+)", l)
            #     if p:
            #         results[run] = float(p[0])
            
        os.chdir(maindir)


    # for r, v in results.items():
    #     print(f"{v}")


    
