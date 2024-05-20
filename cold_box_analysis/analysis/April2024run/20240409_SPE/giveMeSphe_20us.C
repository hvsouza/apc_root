// ________________________________________ //
// Author: Henrique Souza
// Filename: giveMeSphe_darkCount.C
// Created: 2021
// ________________________________________ //
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void adjust_parameters(SPHE2 &dark, int channel){
    switch (channel){
        case 0:
            dark.start = 8300;
            dark.finish = dark.start+900;
            break;
        case 1:
            dark.start = 8300;
            dark.finish = dark.start+1400;
            break;
        case 2:
            dark.start = 8340;
            dark.finish = dark.start+400;
            break;
        case 3:
            dark.start = 8340;
            dark.finish = dark.start+500;
            break;
        case 4:
            dark.start = 8380;
            dark.finish = dark.start+500;
            break;
        case 5:
            dark.start = 8380;
            dark.finish = dark.start+500;
            break;
        case 6:
            dark.start = 8380;
            dark.finish = dark.start+500;
            break;
        case 7:
            dark.start = 8380;
            dark.finish = dark.start+500;
            break;
    }
}

void giveMeSphe_20us(int channel = 0){

    SPHE2 dark("spe");

    dark.led_calibration = true; // if external trigger + led was used, set true
                                 // start and finish will be the time of integration
    dark.just_a_test     = false; // well.. it is just a test, so `just_this` is the total waveforms analysed
    dark.just_this       = 200;
    dark.channel         = channel;
    dark.rootfile        = "analyzed.root";

    dark.start  = 8320;            // start the search for peaks or start the integration (led)
    dark.finish = 9520;        // fisish the search or finish the integration (led)

    dark.filter = 12;   // one dimentional denoise filter (0 equal no filder)


    dark.get_wave_form = true; // for getting spe waveforms
    dark.mean_before   = dark.start-2300;   // time recorded before and after the peak found
    dark.mean_after    = dark.mean_before+14000;

    dark.spe_max_val_at_time_cut = 10; // after `time_cut`, the signal cannot be higher than this
                                       // this allows to remove after pulses
    dark.time_cut = 3500; // in ns seconds
    dark.cut_with_filtered = true;

    dark.deltaplus  = 1.;
    dark.deltaminus = 0;


    dark.check_selection = true; // uses(or not) variable `selection` to discard wvfs
    dark.withfilter      = true; // Integrate in the filtered waveform
    dark.hnbins = 2000;
    dark.hxmin = -2.5;
    dark.hxmax = 20;
    dark.normalize_histogram = true;

    adjust_parameters(dark, channel);
    dark.giveMeSphe();



    gROOT->SetBatch(kFALSE);

   
}
