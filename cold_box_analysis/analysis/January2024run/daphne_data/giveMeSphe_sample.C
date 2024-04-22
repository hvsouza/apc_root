// ________________________________________ //
// Author: Henrique Souza
// Filename: giveMeSphe_darkCount.C
// Created: 2021
// ________________________________________ //
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void adjust_parameters(SPHE2 &dark, int channel){
    switch (channel){
        case 0:
            dark.sphe_charge   = 12000.6; 
            dark.sphe_charge2  = 24253.3;
            dark.sphe_std      = 2541.29;

            dark.spe_max_val_at_time_cut = 12;
            dark.time_cut = 3500; 
            dark.cut_with_filtered = true;
            break;

        case 7:
            dark.sphe_charge   = 9218.32; 
            dark.sphe_charge2  = 18568.5;
            dark.sphe_std      = 2357.13;

            dark.spe_max_val_at_time_cut = 12;
            dark.time_cut = 3500; 
            dark.cut_with_filtered = true;
            break;

        case 10:
            dark.sphe_charge   = 11183.3; 
            dark.sphe_charge2  = 22038.6;
            dark.sphe_std      = 3194.76;

            dark.start = dark.start+40;
            dark.finish = dark.finish+40;

            dark.spe_max_val_at_time_cut = 1e12;
            dark.time_cut = 3500; 
            dark.cut_with_filtered = true;
            break;

        case 17:
            dark.sphe_charge   = 11183.3; 
            dark.sphe_charge2  = 22038.6;
            dark.sphe_std      = 3194.76;

            dark.start = dark.start+40;
            dark.finish = dark.finish+40;

            dark.spe_max_val_at_time_cut = 1e12;
            dark.time_cut = 3500; 
            dark.cut_with_filtered = true;
            break;
            
        case 21:
            dark.sphe_charge   = 11183.3; 
            dark.sphe_charge2  = 22038.6;
            dark.sphe_std      = 3194.76;

            dark.start = 10096;
            dark.finish = dark.start + 50*16;

            dark.spe_max_val_at_time_cut = 1e12;
            dark.time_cut = 3500; 
            dark.cut_with_filtered = true;
            dark.hnbins = 200;
            dark.hxmin = -10e3;
            dark.hxmax = 30e3;
            break;

        case 36:
            dark.sphe_charge   = 11183.3; 
            dark.sphe_charge2  = 22038.6;
            dark.sphe_std      = 3194.76;

            dark.start = 10192;
            dark.finish = dark.start + 50*16;

            dark.spe_max_val_at_time_cut = 1e12;
            dark.time_cut = 3500; 
            dark.cut_with_filtered = true;
            dark.hnbins = 200;
            dark.hxmin = -10e3;
            dark.hxmax = 30e3;
            break;
    }

}

void giveMeSphe_sample(int channel = 0){

    SPHE2 dark("spe");

    dark.led_calibration = true; // if external trigger + led was used, set true
                                 // start and finish will be the time of integration
    dark.just_a_test     = false; // well.. it is just a test, so `just_this` is the total waveforms analysed
    dark.just_this       = 200;
    dark.channel         = channel;
    dark.filename        = Form("sample_Ch%d",channel);

    dark.start  = 10224;            // start the search for peaks or start the integration (led)
    dark.finish = 11024;        // fisish the search or finish the integration (led)

    dark.filter = 4;   // one dimentional denoise filter (0 equal no filder)

    dark.dtime = 16.;            // time step in ns


    dark.get_wave_form = true; // for getting spe waveforms
    dark.mean_before   = dark.start-2300;   // time recorded before and after the peak found
    dark.mean_after    = dark.start-2300+9000;
    dark.sphe_charge   = 36471.5; // charge of 1 and 2 p.e. (use fit_sphe.C)
    dark.sphe_charge2  = 74522.6;
    dark.sphe_std      = 9180.93;

    dark.spe_max_val_at_time_cut = 1e12; // after `time_cut`, the signal cannot be higher than this
                                       // this allows to remove after pulses
    dark.time_cut = 2000; // in ns seconds

    dark.deltaplus  = 1.;
    dark.deltaminus = 0;


    dark.check_selection = true; // uses(or not) variable `selection` to discard wvfs
    dark.withfilter      = true; // Integrate in the filtered waveform
    dark.hnbins = 200;
    dark.hxmin = -20e3;
    dark.hxmax = 100e3;

    adjust_parameters(dark, channel);
    dark.giveMeSphe();



    gROOT->SetBatch(kFALSE);

   
}
