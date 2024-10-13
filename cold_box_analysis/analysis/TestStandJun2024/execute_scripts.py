import numpy as np
import argparse
from glob import glob
import subprocess
import re
import os

if __name__ == "__main__":
    parse = argparse.ArgumentParser()
    parse.add_argument("code", type=str, help="Code name")
    parse.add_argument('-f', '--folder', type=str, help="Folder in regex mode to be evaluated")
    parse.add_argument("-ch", "--channels", type=int, nargs="+", help="(If empty, try to find channels from .dat files. List of channels to be process, ex: 13 15 (ordering is not required)")
    parse.add_argument("-noargs", "--no_argments", action="store_true", help="Don't give any arguments")
    parse.add_argument("-c", "--c", action="store_true", help="Compile code")
    parse.add_argument("-b", "--b", action="store_true", help="Batch mode")
    parse.add_argument("-q", "--q", action="store_true", help="quite after")
    parse.add_argument("-n", "--dry",  action="store_true", help='Dry run')
    args = vars(parse.parse_args())

    # channel = args['channel']
    folder = args['folder']
    code = args['code']
    doplus = args['c']
    isbatch = args['b']
    doquit = args['q']
    dry = args['dry']
    if doplus:
        code += "+"

    noargs = args['no_argments']

    maindir = os.getcwd()

    files = sorted(glob(f"./{folder}"))
    if dry:
        for f in files:
            print(f)
        exit(0)
    for f in files:
        # print(f"Running over {f}")
        channels = args["channels"]
        if not channels:
            channels = []
            wavefiles = sorted(glob(f"{f}/0_wave*.dat"))
            for w in wavefiles:
                print(w)
                channels.append(re.findall(r"0_wave(\d+)",w)[0])

        os.chdir(f)
        if noargs:
            channels = [""]
        print(f"Inside {f}, channels: {channels}")
        for c in channels:
            execthis = f"{maindir}/{code}({c})"
            command = ["root","-l"]
            if isbatch: command.append("-b")
            if doquit: command.append("-q")
            command.append(execthis)
            ret = subprocess.run(command)
        os.chdir(maindir)




    
