#define memorydepth 4000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"



void giveMeSphe_darkCount(){
    
    SPHE dark;
    
    dark.led_calibration=true;
    
    dark.just_a_test = false;
    dark.just_this = 200;
    
    dark.tolerance = 7; // n sigmas (smoothed)
    dark.baseLimit = 0.003; // higher then this wont contribute to the baseline abs(baseLimit)
    dark.baselineTime = 4*600;
    dark.start = 8016; // start the search for peaks or start the integration (led)
    dark.finish = 8164; // fisish the search or finish the integration (led)
    
    dark.timeLimit = 0; // time after LED signal
    dark.timeLow = 8; // integration time before peak
    dark.timeHigh = 240; // integration time after peak
    
    dark.lowerThreshold = -9999; // threshold to detect noise (normal waveform)
    dark.maxHits = 1; // maximum hit before discarding  
    
    dark.too_big = 1000; // if there is a peak > "too_big" .. wait "waiting" ns
    dark.waiting = 1000;
    
    dark.interactions = 25; // for moving avarage
    
    
    dark.dtime = 4.;

    dark.get_wave_form = true;
    dark.mean_before = 120; // time recorded before and after the peak found 
    dark.mean_after = 1000;
    dark.sphe_charge_ch0 = 1967.83; // wave0
    dark.sphe_charge2_ch0 = 3575.63; // wave0
    dark.deltaplus = 1.2;
    dark.deltaminus = 2;
    
    dark.sphe_charge_ch1 = 232.449; // wave0
    dark.sphe_charge2_ch1 = 542.264; // wave0
    
    dark.channel = 1;
        
    
    dark.giveMeSphe_darkCount("analyzed");


    
    gROOT->SetBatch(kFALSE);
    
}
