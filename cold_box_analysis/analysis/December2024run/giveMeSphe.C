// ________________________________________ //
// Author: Henrique Souza
// Filename: giveMeSphe.C
// Created: 2024
// ________________________________________ //
#include "MYCODES.h"



void giveMeSphe(Int_t channel = 40){

  SPHE2 dark("spe");


  dark.led_calibration = true; // if external trigger + led was used, set true
                               // start and finish will be the time of integration
                               // dark.just_a_test     = true; // well.. it is just a test, so `just_this` is the total waveforms analysed
  dark.just_this       = 2000;
  dark.channel         = channel;
  dark.rootfile        = "analyzed.root";

  dark.start  = 2192;            // start the search for peaks or start the integration (led)
  dark.finish = 3008;        // fisish the search or finish the integration (led)

  dark.filter        = 4;   // one dimentional denoise filter (0 equal no filder)
  dark.too_big       = 1500;

  dark.get_wave_form = true; // for getting spe waveforms
  dark.mean_before   = 0;   // time recorded before and after the peak found
  dark.mean_after    = 1024*16;
  dark.sphe_charge   = 3062.51; // charge of 1 and 2 p.e. (use fit_sphe.C)
  dark.sphe_charge2  = 7913.06;
  dark.sphe_std      = 1982.13;
  dark.spe_max_val_at_time_cut = 1e12; // after `time_cut`, the signal cannot be higher than this
                                       // this allows to remove after pulses
  dark.time_cut = 2000; // in ns seconds

  // coeficients to surround gaussian of 1 spe.
  // Gain             = (sphe_charge2 - sphe_charge)
  // spe's get events where charge < Gain*deltaplus  and charge < Gain/deltaminus
  // If deltaminus is set to zero, sphe_std*deltaplus will be used instead
  // This value can be checked with fit_sphe.C
  dark.deltaplus  = 1.;
  dark.deltaminus = 0;


  // Not so common to change
  dark.check_selection     = true; // uses(or not) variable `selection` to discard
  dark.withfilter          = true; // Integrate in the filtered waveform
  dark.hnbins = 1000;       // Output histogram bins and limits. Does not change this unless you will analyze alway the same device
  dark.hxmin  = -2.5;           // The fit function has the `rebin` argument to avoid changing this values
  dark.hxmax  = 20;
  dark.normalize_histogram = true; // will normalize histogram by the average value

  if ( dark.channel == 41 || dark.channel == 46 ){
    // dark.check_selection = false;
    dark.too_big = 3000;
    dark.hnbins = 2000;
  }
  if ( dark.channel == 41 ){
    dark.finish = 3488;
  }


  dark.modifyFunction([&dark](){
      Double_t vmin = *std::min_element(dark.denoise_wvf.begin(), dark.denoise_wvf.begin() + 5);
      Double_t vmax = *std::max_element(dark.denoise_wvf.begin(), dark.denoise_wvf.begin() + 5);
      if (abs(vmax-vmin) < 1)
      {
        return false;  
      }
      vmin = *std::min_element(dark.denoise_wvf.end()-5, dark.denoise_wvf.end());
      vmax = *std::max_element(dark.denoise_wvf.end()-5, dark.denoise_wvf.end());
      if (abs(vmax-vmin) < 1)
      {
        return false;  
      }
      vmin = *std::min_element(dark.denoise_wvf.begin()+(2192/16), dark.denoise_wvf.begin()+(2191/16)+5);
      vmax = *std::max_element(dark.denoise_wvf.begin()+(2192/16), dark.denoise_wvf.begin()+(2191/16)+5);
      if (abs(vmax-vmin) < 1)
      {
        return false;  
      }
      return true;
  });

  dark.giveMeSphe();



  gROOT->SetBatch(kFALSE);


}
