#define memorydepth 5000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"



void giveMeSphe_darkCount(){
    
    SPHE dark;
    
    dark.led_calibration=true;
    
    dark.just_a_test = false;
    dark.just_this = 10000;
    // dark.check_selection = false;
    
    dark.tolerance = 3.5; // n sigmas (smoothed)
    dark.baseLimit = 2; // higher then this wont contribute to the baseline abs(baseLimit)
    dark.baselineTime = 4000;
    // dark.start = 10310; // start the search for peaks or start the integration (led)
    // dark.finish = 10700; // fisish the search or finish the integration (led)

    dark.start = 5650; // start the search for peaks or start the integration (led)
    dark.finish = 6100; // fisish the search or finish the integration (led)

    dark.timeLimit = 0; // time after LED signal
    dark.timeLow = 8; // integration time before peak
    dark.timeHigh = 340; // integration time after peak
    
    dark.lowerThreshold = -9999; // threshold to detect noise (normal waveform)
    dark.maxHits = 1; // maximum hit before discarding  
    
    dark.too_big = 1000; // if there is a peak > "too_big" .. wait "waiting" ns
    dark.waiting = 1000;

    dark.filter = 6;
    dark.interactions = 45; // for moving avarage
    dark.shifter = 20;
    
    dark.dtime = 4.;

    dark.get_wave_form = true;
    dark.mean_before = 120; // time recorded before and after the peak found 
    dark.mean_after = 1000;
    // dark.sphe_charge_ch0 = 4486.84; //wave0
    // dark.sphe_charge2_ch0 = 8910.46; // wave0
    dark.deltaplus = 1.35;
    dark.deltaminus = 1.4;
    

    dark.sphe_charge_ch0 = 2208.27; //wave0
    dark.sphe_charge2_ch0 = 4273.29; // wave0

    // dark.channel = 3;
    // dark.check_selection = true;
    // dark.giveMeSphe_darkCount("analyzed");

    dark.channel = 0;
    dark.deltaplus = 1.35;
    dark.deltaminus = 1.4;
    dark.check_selection = true;
    dark.giveMeSphe_darkCount("analyzed");

    
    gROOT->SetBatch(kFALSE);
    
}
