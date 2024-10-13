// ________________________________________ //
// Author: Henrique Souza
// Filename: giveMeSphe_darkCount.C
// Created: 2021
// ________________________________________ //
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void giveMeSphe_20us(int channel = 1){

    SPHE2 dark("spe");

    dark.led_calibration = true; // if external trigger + led was used, set true
                                 // start and finish will be the time of integration
    dark.just_a_test     = false; // well.. it is just a test, so `just_this` is the total waveforms analysed
    dark.just_this       = 200;
    dark.channel         = channel;
    dark.rootfile        = "analyzed.root";

    dark.start  = 10500;            // start the search for peaks or start the integration (led)
    dark.finish = 10900;        // fisish the search or finish the integration (led)

    dark.filter = 2;   // one dimentional denoise filter (0 equal no filder)



    dark.get_wave_form = true; // for getting spe waveforms
    dark.mean_before   = 0;   // time recorded before and after the peak found
    dark.mean_after    = 20000;
    dark.spe_max_val_at_time_cut = 1e12; // after `time_cut`, the signal cannot be higher than this
                                       // this allows to remove after pulses
    dark.time_cut = 2000; // in ns seconds

    dark.deltaplus  = 1.;
    dark.deltaminus = 0;

    dark.check_selection = false; // uses(or not) variable `selection` to discard wvfs
    dark.withfilter      = true; // Integrate in the filtered waveform
    dark.hnbins = 800;
    dark.hxmin = -2.5;
    dark.hxmax = 20;
    dark.normalize_histogram = true;

    dark.giveMeSphe();



    gROOT->SetBatch(kFALSE);

   
}
