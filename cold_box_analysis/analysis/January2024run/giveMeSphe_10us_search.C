// ________________________________________ //
// Author: Henrique Souza
// Filename: giveMeSphe_darkCount.C
// Created: 2021
// ________________________________________ //
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void adjust_parameters(SPHE2 &dark, int channel){
    switch (channel){
        case 0:
            dark.sphe_charge   = 4918.69; 
            dark.sphe_charge2  = 10071.1;
            dark.sphe_std      = 658.491;

            dark.hnbins = 400;
            dark.hxmin = -10e3;
            dark.hxmax = 60e3;
            dark.spe_max_val_at_time_cut = 7;
            dark.time_cut = 3500; 
            dark.cut_with_filtered = true;
            break;
        case 1:
            dark.sphe_charge   = 4090.98; 
            dark.sphe_charge2  = 8496.92;
            dark.sphe_std      = 982.003;

            dark.hnbins = 400;
            dark.hxmin = -10e3;
            dark.hxmax = 60e3;
            dark.spe_max_val_at_time_cut = 6.2;
            dark.time_cut = 3500;
            dark.cut_with_filtered = true;
            break;
            
    }

}

void giveMeSphe_10us_search(int channel = 0){

    SPHE2 dark("spe");

    dark.led_calibration = false; // if external trigger + led was used, set true
                                 // start and finish will be the time of integration
    dark.just_a_test     = false; // well.. it is just a test, so `just_this` is the total waveforms analysed
    dark.just_this       = 200;
    dark.channel         = channel;
    dark.rootfile        = "../analyzed.root";

    dark.start  = 3000;            // start the search for peaks or start the integration (led)
    dark.finish = 7000;        // fisish the search or finish the integration (led)

    dark.filter = 12;   // one dimentional denoise filter (0 equal no filder)
    dark.nshow_range = {0,100}; // will save some debugging waveforms inside the range.

    dark.tolerance    = 3.5;      // n sigmas (smoothed) (not used for led)
    dark.dtime = 2.;            // time step in ns
    

    dark.timeLow        = 200;   // integration time before peak (not used for led)
    dark.timeHigh       = 1300;  // integration time after peak (not used for led)
    dark.lowerThreshold = -999;  // threshold to detect noise (normal waveform) (not used for led)
    dark.maxHits        = 5;    // maximum hit before discarding   (not used for led)
    dark.too_big        = 2000;  // if there is a peak > "too_big" .. wait "waiting" ns for next peak
    dark.waiting        = 1000;
    dark.interactions   = 40;   // for moving avarage filter (not used on led)
    dark.ninter         = 2;    // N times that moving average is applied
    dark.diff_multiplier = 100;  //derivative can be very small. Use this to make it easier to see
    dark.derivate_raw   = false;
    dark.integrate_from_peak = false;


    dark.get_wave_form = true; // for getting spe waveforms
    dark.mean_before   = 2000;   // time recorded before and after the peak found
    dark.mean_after    = 5000;
    dark.sphe_charge   = 36471.5; // charge of 1 and 2 p.e. (use fit_sphe.C)
    dark.sphe_charge2  = 74522.6;
    dark.sphe_std      = 9180.93;
    dark.hnbins = 400;
    dark.hxmin = -10e3;
    dark.hxmax = 60e3;
    dark.spe_max_val_at_time_cut = 1e12; // after `time_cut`, the signal cannot be higher than this
    // dark.spe_max_val_at_time_cut = 7;
    // dark.time_cut = 3500; 
    // dark.cut_with_filtered = true;
                                       // this allows to remove after pulses

    dark.deltaplus  = 1.;
    dark.deltaminus = 0;


    dark.check_selection = false; // uses(or not) variable `selection` to discard wvfs
    dark.withfilter      = true; // Integrate in the filtered waveform

    // Not so common to change
    dark.social_distance = 5.;   // demands that there is a minimum distance of social_distance * timeHigh between 2 consecutive peaks found
    dark.method          = "derivative"; // `dynamic` or `derivative` evaluation of the baseline
                               // `fix` will not evaluate baseline and use raw threshold
                               // See tolerance, baselineTime and baselineLimit above

    // adjust_parameters(dark, channel);
    dark.giveMeSphe();



    gROOT->SetBatch(kFALSE);

   
}
