#define memorydepth 5000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void adc_read_all_data(){
    
    system("rm files.log");
    system("ls -1 *.dat| sed -e 's/.dat$//' > files.log");
    
    Read r;
        
    r.dtime = 2;
    r.nbits = 14;
    r.isBinary = true;
    
    r.baselineTime = 2400; // time limit for baseline
    r.chargeTime = 2650; // last time to integrate
    r.startCharge = 2550;
    r.maxRange = 1600; // max range to search for amplitude peak
    r.fast = 100; // fprompt fast integration time
    r.slow = 5000; //fprompt slow integration time
    r.exclusion_baselines = {38, 3, 30, 20, 3, 4, 20, 4}; // filtered waveform, anything above here will do +exclusion window
    r.exclusion_window = 200; // time in ns that it will jump for baseline
    r.filter = 16; // denoise filter.. if filter = 0, no denoise is done. r.OnlyOneEvent = false; // Do you want only one event? Choose it wisely
    // r.OnlyOneEvent = true;
    r.stopEvent = 10000;
    r.noBaseline = false;
    r.polarity = 1;
    // r.saveFilter = true;
      

    r.adc_read_all_data();

    
}

