#define memorydepth 5000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void adc_read_all_data(){
    
    system("rm files.log");
    system("ls -1 *.dat| sed -e 's/.dat$//' > files.log");
    
    Read r;
        
    r.dtime = 4;
    r.nbits = 14;
    r.isBinary = true;
    
    r.baselineTime = 1200; // time limit for baseline
    r.chargeTime = 3800; // last time to integrate
    r.startCharge = 1300;
    r.maxRange = 1600; // max range to search for amplitude peak
    r.fast = 100; // fprompt fast integration time
    r.slow = 5000; //fprompt slow integration time
    r.exclusion_baselines = {20, 20}; // filtered waveform, anything above here will do +exclusion window
    /* r.exclusion_baselines = {20, 3, 30, 20, 3, 4, 20, 4}; // filtered waveform, anything above here will do +exclusion window */
    r.exclusion_window = 100; // time in ns that it will jump for baseline
    r.filter = 4; // denoise filter.. if filter = 0, no denoise is done. r.OnlyOneEvent = false; // Do you want only one event? Choose it wisely
    // r.OnlyOneEvent = true;
    r.stopEvent = 10000;
    r.noBaseline = false;
    r.polarity = -1;
    // r.saveFilter = true;
      

    r.adc_read_all_data();

    
}

