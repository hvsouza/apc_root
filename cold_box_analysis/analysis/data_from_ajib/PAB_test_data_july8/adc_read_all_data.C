#define memorydepth 2000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void adc_read_all_data(){
    
    system("rm files.log");
    system("ls -1 *.txt| sed -e 's/.txt$//' > files.log");
    
    Read r;
        
    r.dtime = 5;
    r.nbits = 14;
    r.isBinary = false;
    
    r.baselineTime = 4500; // time limit for baseline
    r.chargeTime = 5000; // last time to integrate 
    r.startCharge = 5500;
    r.maxRange = 5500; // max range to search for amplitude peak
    r.fast = 200; // fprompt fast integration time
    r.slow = 1700; //fprompt slow integration time
    r.exclusion_baseline = 35; // filtered waveform, anything above here will do +exclusion window
    r.exclusion_window = 1000; // time in ns that it will jump for baseline
    r.filter = 0.005; // denoise filter.. if filter = 0, no denoise is done. 
    // r.filter = 0; // denoise filter.. if filter = 0, no denoise is done. 
    r.OnlyOneEvent = false; // Do you want only one event? Choose it wisely 
    r.stopEvent = 2;
    // r.saveFilter = true;
    // r.noBaseline = true;
      
    r.channels = {1,2};
    
    r.adc_read_all_data();

    
}

