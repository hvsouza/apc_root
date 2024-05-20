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
    parse.add_argument("-nc", "--no_channel", action="store_true", help="Don't try to get any channel")
    parse.add_argument("-c", "--c", action="store_true", help="Compile code")
    parse.add_argument("-b", "--b", action="store_true", help="Batch mode")
    parse.add_argument("-q", "--q", action="store_true", help="quite after")
    parse.add_argument("-n", "--dry",  action="store_true", help='Dry run')
    args = vars(parse.parse_args())

    # channel = args['channel']
    folder = args['folder']
    code = args['code']
    no_channel = args['no_channel']
    doplus = args['c']
    isbatch = args['b']
    doquit = args['q']
    dry = args['dry']
    if doplus:
        code += "+"

    maindir = os.getcwd()

    files = sorted(glob(f"./{folder}"))
    if dry:
        for f in files:
            print(f)
        exit(0)
    for f in files:
        # print(f"Running over {f}")
        channels = []
        if not no_channel:
            wavefiles = sorted(glob(f"{f}/*_wave*.dat"))
            for w in wavefiles:
                # print(w)
                channels.append(re.findall(r"wave(\d+)",w)[0])
            # print(channels)
        else:
            channels.append('')
        
        os.chdir(f)
        print(f"Inside {f}")
        for c in channels:
            execthis = f"{maindir}/{code}({c})"
            command = ["root","-l"]
            if isbatch: command.append("-b")
            if doquit: command.append("-q")
            command.append(execthis)
            ret = subprocess.run(command)
        os.chdir(maindir)




    
