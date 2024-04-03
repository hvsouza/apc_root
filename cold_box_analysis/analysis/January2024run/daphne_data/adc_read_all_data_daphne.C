#define memorydepth 5000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void adc_read_all_data_daphne(string datadir = "./"){
    
    system("rm files.log");
    int datadirlength = datadir.length();
    if(datadir[datadirlength-1] != '/'){
        datadir = datadir + "/";
    }
    string packdata = Form("ls -1 %s*.dat | sed -e 's/.dat$//' > files.log", datadir.c_str());
    // string packadata = Form("ls -1 %s*.dat | grep wave | sed -e 's/.dat$//' > files.log", datadir.c_str()); // to only check one channel
    // string packadata = Form("ls -1 %s*.dat | grep wave[2,3] | sed -e 's/.dat$//' > files.log", datadir.c_str()); // to only check channels 2 and 3
    system(packdata.c_str());
    
    
    Read r;
        
    r.dtime = 16;
    r.nbits = 14;
    r.isBinary = true;
    
    r.baselineTime = 20000; // time limit for baseline
    r.chargeTime = 5500; // last time to integrate
    r.startCharge = 3300;
    r.maxRange = 1600; // max range to search for amplitude peak
    r.fast = 100; // fprompt fast integration time
    r.slow = 5000; //fprompt slow integration time
    r.exclusion_baselines = {10, 10, 10, 9, 9, 12, 10, 10}; // filtered waveform, anything above here will do +exclusion window
    r.exclusion_window = 1400; // time in ns that it will jump for baseline
    r.filter = 4; // denoise filter.. if filter = 0, no denoise is done. r.OnlyOneEvent = false; // Do you want only one event? Choose it wisely
    // r.OnlySomeEvents = true;
    r.stopEvent = 10000;
    r.noBaseline = false;
    r.polarity = 1;
    // r.saveFilter = true;
      

    r.adc_read_all_data();

    
}

