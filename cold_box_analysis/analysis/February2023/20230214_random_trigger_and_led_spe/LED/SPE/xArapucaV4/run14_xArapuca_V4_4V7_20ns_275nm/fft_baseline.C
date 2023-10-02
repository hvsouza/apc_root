// ________________________________________ //
// Author: Henrique Souza
// Filename: fft_baseline.C
// Created: 2023-06-02
// ________________________________________ //

#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"

ANALYZER z2x2("z2x2");
ANALYZER zch1("zch1");
ANALYZER zch2("zch2");


void process_stuff(ANALYZER *z, Double_t spe_charge, Double_t spe_stddev, Double_t spe_amp){
  // z->xmin = 5000;
  // z->xmax = 7000;
  spe_amp = spe_amp*0.8;

  // z->w = new WIENER(Form("wmod%d",z->getIdx()),z->dtime,250,1e-9,1e6,(z->xmax - z->xmin)/z->dtime);


  // z->persistence_plot(1000, -300, 700, 16, Form("Ch%d.charge<%f-%f", z->getIdx(), spe_charge, spe_stddev));
  for(int i = 0; i <z->nentries; i++){
    z->getWaveform(i);
    z->applyDenoise(16);
    z->getMaxMin(z->xmin, z->xmax);
    if(z->temp_max < spe_amp && abs(z->temp_min)<spe_amp){
      z->lev->Enter(i);
    }
  }

  z->persistence_plot(1000, -300, 700, 16, "use_mine");
  z->averageFFT(0,"use_mine",true, 50);//,true,pow(2,14));
  // z.h->Draw("hist");

}
void fft_baseline(){

  zch1.setAnalyzer("./analyzed.root");
  zch2.setAnalyzer("./analyzed.root");
  zch1.setChannel("Ch4.");
  zch2.setChannel("Ch3.");

  z2x2.setAnalyzer("../../Argon2x2/run8_argon2x2_7V5_20ns_275nm/sphe_waveforms_Ch0.root");

  process_stuff(&zch1, 1552.22, 398.703, 80);
  process_stuff(&zch2, 2189.12, 669.973, 5);

  process_stuff(&z2x2, 6184.18, 1107.12, 42.6);

  TCanvas *cfft = new TCanvas("cfft", "cfft",1920,0,1920,1080);

  cfft->cd();
  zch1.h->Draw("hist");
  zch2.h->Draw("SAME");
  z2x2.h->Draw("SAME");

  zch1.h->SetTitle("February DCem1.2 V4 Ch1");
  zch2.h->SetTitle("February DCem1.2 V4 Ch2");
  zch2.h->SetLineColor(kBlue);
  z2x2.h->SetTitle("Argon2x2");
  z2x2.h->SetLineColor(kRed);

  cfft->BuildLegend();


}
