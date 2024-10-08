#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"

void fit_sphe_wave7(Int_t ch = 7){
    
    Calibration Cal;
    
    
    Cal.rebin = 50;
    string histogram = "analyzed_" + to_string(ch);
    Cal.rootFile = "sphe_histograms_Ch"+to_string(ch)+".root";

    Cal.dtime = 2; // steps (ADC's MS/s, 500 MS/s = 2 ns steps)

    // Cal.fixZero = false;
    Cal.make_free_stddevs = true; // starts with false, if good fitting, change to true
    Cal.searchParameters(histogram.c_str(), 2, true); // give a first search in the parameters.
                                                       // Debug = true will show the plot. 2 is the gaussian std used. The minimum value is 1. The lower this is the sharp is the peak.
    // // Uncomment bellow if you want to set anything by hand
    // Cal.n_peaks = 12; // n peaks after baseline and 1 p.e.. Total = 2 + n_peaks
    // Cal.peak0 =  0.014; // amplitude baseline
    // Cal.mean0 = 0; // average baseline
    // Cal.sigma0 = 1300; // sigma baseline

    // Cal.peak1 = 0.016; // same for 1 p.e.
    // Cal.mean1 = 6000;
    // Cal.sigma1 = 1700;

    // Cal.startpoint = 0.015; // amplitude for 2 p.e.

    // Cal.xmin = -10000; // range for graph display (not fit)
    // Cal.xmax = 60000;

    // Cal.deltaplus = 1.35;
    // Cal.deltaminus = 1.4;

    Cal.fit_sphe_wave(histogram.c_str(),false); // set true to make if you want to execute "searchParameters" inside here instead
}
