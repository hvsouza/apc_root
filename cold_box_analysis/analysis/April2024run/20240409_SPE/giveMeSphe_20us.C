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
        case 2:
            dark.sphe_charge   = 3060.86; 
            dark.sphe_charge2  = 6274.35;
            dark.sphe_std      = 644.044;

            dark.hnbins = 200;
            dark.hxmin = -5e3;
            dark.hxmax = 30e3;
            dark.spe_max_val_at_time_cut = 6.2;
            dark.time_cut = 3500;
            dark.cut_with_filtered = true;
            break;
        case 3:
            dark.sphe_charge   = 2413.16; 
            dark.sphe_charge2  = 5077.03;
            dark.sphe_std      = 626.65 ;

            dark.hnbins = 200;
            dark.hxmin = -5e3;
            dark.hxmax = 30e3;
            dark.spe_max_val_at_time_cut = 6.2;
            dark.time_cut = 3500;
            dark.cut_with_filtered = true;
            break;

        case 4:
            dark.sphe_charge   = 849.976; 
            dark.sphe_charge2  = 1730.79;
            dark.sphe_std      = 236.46 ;
            dark.finish        = 9000;

            dark.hnbins = 200;
            dark.hxmin = -5e3;
            dark.hxmax = 8e3;
            dark.spe_max_val_at_time_cut = 3.2;
            dark.time_cut = 3500;
            dark.cut_with_filtered = true;
            break;

        case 5:
            dark.sphe_charge   = 1101.26; 
            dark.sphe_charge2  = 2241.95;
            dark.sphe_std      = 275.341;

            dark.finish        = 9000;

            dark.hnbins = 200;
            dark.hxmin = -5e3;
            dark.hxmax = 8e3;
            dark.spe_max_val_at_time_cut = 3.2;
            dark.time_cut = 3500;
            dark.cut_with_filtered = true;
            break;
            
        case 6:
            dark.sphe_charge   = 3231.63; 
            dark.sphe_charge2  = 6515.54;
            dark.sphe_std      = 469.463;

            dark.hnbins = 200;
            dark.hxmin = -5e3;
            dark.hxmax = 30e3;
            dark.spe_max_val_at_time_cut = 6.2;
            dark.time_cut = 3500;
            dark.cut_with_filtered = true;
            break;
        case 7:
            dark.sphe_charge   = 1389.05; 
            dark.sphe_charge2  = 2822.11;
            dark.sphe_std      = 324.941;

            dark.finish        = 9000;

            dark.hnbins = 200;
            dark.hxmin = -5e3;
            dark.hxmax = 10e3;
            dark.spe_max_val_at_time_cut = 3.2;
            dark.time_cut = 3500;
            dark.cut_with_filtered = true;
            break;

        case 8:
            dark.sphe_charge   = 6615.18; 
            dark.sphe_charge2  = 13445.2;
            dark.sphe_std      = 877.564;


            dark.hnbins = 400;
            dark.hxmin = -10e3;
            dark.hxmax = 60e3;
            dark.spe_max_val_at_time_cut = 10;
            dark.time_cut = 3500; 
            dark.cut_with_filtered = true;
            dark.channel = 0;
            break;

        case 9:
            dark.sphe_charge   = 4958.45; 
            dark.sphe_charge2  = 10109  ;
            dark.sphe_std      = 1033.39;


            dark.hnbins = 400;
            dark.hxmin = -10e3;
            dark.hxmax = 60e3;
            dark.spe_max_val_at_time_cut = 10;
            dark.time_cut = 3500; 
            dark.cut_with_filtered = true;
            dark.channel = 1;
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
    if (channel >= 4){
        dark.start+=40;
        dark.finish+=40;
    }

    dark.filter = 12;   // one dimentional denoise filter (0 equal no filder)

    dark.dtime = 2.;            // time step in ns


    dark.get_wave_form = true; // for getting spe waveforms
    dark.mean_before   = dark.start-2300;   // time recorded before and after the peak found
    dark.mean_after    = dark.mean_before+14000;

    dark.spe_max_val_at_time_cut = 10; // after `time_cut`, the signal cannot be higher than this
                                       // this allows to remove after pulses
    dark.time_cut = 3500; // in ns seconds
    dark.cut_with_filtered = true;

    dark.deltaplus  = 1.;
    dark.deltaminus = 0;


    dark.check_selection = false; // uses(or not) variable `selection` to discard wvfs
    dark.withfilter      = true; // Integrate in the filtered waveform
    dark.hnbins = 2000;
    dark.hxmin = -2.5;
    dark.hxmax = 20;
    dark.normalize_histogram = true;

    // adjust_parameters(dark, channel);
    dark.giveMeSphe();



    gROOT->SetBatch(kFALSE);

   
}
