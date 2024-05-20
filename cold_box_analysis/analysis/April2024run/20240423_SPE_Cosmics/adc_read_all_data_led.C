#define memorydepth 5000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void adc_read_all_data_led(string datadir = "./"){
    
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
        
    r.dtime = 2;
    r.nbits = 14;
    r.isBinary = true;
    
    r.baselineTime = 20000; // time limit for baseline
    r.chargeTime = 4500; // last time to integrate
    r.startCharge = 3100;
    r.maxRange = 4500; // max range to search for amplitude peak
    r.fast = 100; // fprompt fast integration time
    r.slow = 5000; //fprompt slow integration time
    // r.exclusion_baselines = {6, 6, 6, 5, 2.5, 2.5, 4, 3}; // filtered waveform, anything above here will do +exclusion window
    r.map_exclusion_threshold_baselines = {
        {"Ch0.", 5},
        {"Ch1.", 5},
        {"Ch2.", 5},
        {"Ch3.", 4},
        {"Ch4.", 4},
        {"Ch5.", 5},
        {"Ch6.", 4},
        {"Ch7.", 7},
    };
    r.exclusion_window = 1000; // time in ns that it will jump for baseline
    r.filter = 12; // denoise filter.. if filter = 0, no denoise is done. r.OnlyOneEvent = false; // Do you want only one event? Choose it wisely
    // r.OnlySomeEvents = true;
    r.stopEvent = 1000;
    r.noBaseline = false;
    r.polarity = 1;
    r.baselineFraction = 1/4.;
    // r.saveFilter = true;
      

    r.adc_read_all_data();

    
}

