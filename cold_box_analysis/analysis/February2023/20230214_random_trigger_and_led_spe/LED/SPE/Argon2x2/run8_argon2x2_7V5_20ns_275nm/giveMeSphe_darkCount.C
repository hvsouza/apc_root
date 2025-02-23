#define memorydepth 5000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"



void giveMeSphe_darkCount(){
    
    SPHE2 dark("spe");
    
    dark.led_calibration=true;
    
    dark.just_a_test = false;
    dark.just_this = 200;
    // dark.check_selection = false;
    dark.rootfile = "analyzed";
    
    dark.tolerance = 3.5; // n sigmas (smoothed)
    dark.baseLimit = 2; // higher then this wont contribute to the baseline abs(baseLimit)
    dark.baselineTime = 5000;
    // dark.start = 10310; // start the search for peaks or start the integration (led)
    // dark.finish = 10700; // fisish the search or finish the integration (led)

    dark.start = 10280; // start the search for peaks or start the integration (led)
    dark.finish = 10700; // fisish the search or finish the integration (led)

    dark.timeLow = 8; // integration time before peak
    dark.timeHigh = 340; // integration time after peak
    
    dark.lowerThreshold = -9999; // threshold to detect noise (normal waveform)
    dark.maxHits = 1; // maximum hit before discarding  
    
    dark.too_big = 1000; // if there is a peak > "too_big" .. wait "waiting" ns
    dark.waiting = 1000;

    dark.filter = 16;
    dark.interactions = 45; // for moving avarage

    dark.dtime = 4.;

    dark.get_wave_form = true;
    dark.mean_before = 5000; // time recorded before and after the peak found 
    dark.mean_after = 15000;
    // dark.sphe_charge_ch0 = 4486.84; //wave0
    // dark.sphe_charge2_ch0 = 8910.46; // wave0
    dark.deltaplus = 1;
    dark.deltaminus = 0;
    dark.tolerance = 1e12;
    dark.spe_max_val_at_time_cut = 1e12; // after `time_cut`, the signal cannot be higher than this

    dark.time_cut = 2000; // in ns seconds


    dark.sphe_charge = 7169.9; //wave0
    dark.sphe_charge2 = 14257.6; // wave0
    dark.sphe_std = 1705.7;

    dark.channel = 0;
        
    
    dark.giveMeSphe();


    
    gROOT->SetBatch(kFALSE);
    
}
