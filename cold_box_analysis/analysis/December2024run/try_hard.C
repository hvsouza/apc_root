#include "MYCODES.h"

void fit_sphe_wave0(Int_t ch = 40, Int_t rebin = 1, Double_t deconv = 6){
    
    Calibration Cal;
    Cal.rebin = rebin;
    Cal.channel = ch;
    string histogram = "analyzed";
    Cal.rootFile = "sphe_histograms_Ch"+to_string(ch)+".root";

    Cal.searchParameters(histogram.c_str(), deconv, true); // give a first search in the parameters.

    Cal.deltaplus = 1;
    Cal.deltaminus = 0;

    // Cal.drawDebugLines = true;
    Cal.fit_sphe_wave(histogram.c_str(),false); // set true to make if you want to execute "searchParameters" inside here instead
    return;
}
void try_hard(Int_t ch = 0, Int_t minrebin = 1, Int_t maxrebin = 1, Double_t min_sigma = 3, Double_t max_sigma = 9, Double_t sigma_step = 1., bool save_last=true, string forcetype="f"){

    Calibration Cal;

    if (ch == 41 || ch == 46)
    {
      min_sigma = 2;
      max_sigma = 4;
      if (ch == 41)
        Cal.fix_baseline_from_second_general_fit = true;
    }

    Cal.forcetype = forcetype;
    string histogram = "analyzed";
    Cal.rootFile = "sphe_histograms_Ch"+to_string(ch)+".root";
    Cal.nameplotsave = "fit_result_Ch"+to_string(ch)+".png";
    Cal.sphe_fit_try_hard(nullptr, ch, minrebin, maxrebin,  min_sigma, max_sigma, save_last, 0.1, sigma_step); 

    gROOT->SetBatch(kFALSE);
}
