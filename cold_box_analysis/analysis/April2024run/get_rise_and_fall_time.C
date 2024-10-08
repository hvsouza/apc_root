#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void get_rise_and_fall_time(int channel = 0){

  ANALYZER z("z");
  z.setAnalyzer(Form("sphe_waveforms_Ch%d.root", channel));

  z.getSelection(Form("Ch%d.selection == 1", channel));
  z.excludeByAmplitude(18,0,2000, 7,"higher");
  // z.excludeByAmplitude(12,3500,20000,7,"higher");
  z.persistence_plot(500, -250, 250, 12, "use_mine");
  // TGraph *g = (TGraph*)z.f->Get("mean");
  // g->Draw("SAME");

  z.averageWaveform(0,"use_mine",0);
  z.getWaveFromHistogram(z.haverage[0]);
  // z.getWaveFromGraph(g);
  z.applyDenoise(0.2);
  TCanvas *crise = new TCanvas("crise", "crise",1920,0,1920,1080);
  z.drawGraph();
  crise->Update();
  Double_t rise = z.rise_time(0, {0,2000}, true, {2000,4000}, true);

  TCanvas *cfall = new TCanvas("cfall", "cfall",1920,0,1920,1080);
  z.drawGraph();
  cfall->Update();
  Double_t fall = z.fall_time(0, {0,2000}, true, {2000,8000}, true);

  Double_t amp = z.getMaximum(2000,4000);
  z.applyDenoise(0.2);
  Double_t renorm = z.getMaximum(2000,4000);

  TCanvas *cund = new TCanvas("cund", "cund",1920,0,1920,1080);
  z.scaleWvf(1./renorm);
  z.drawGraph();
  Double_t undershoot = z.getMinimum(3000,10000);

  cout << "A: " << amp << endl;
  cout << "rise: " << round(rise) << endl;
  cout << "fall: " << round(fall) << endl;
  cout << "undershoot: " << undershoot << endl;
  printf("%.1f, %0.f, %.0f, %.1f\n", amp, round(rise), round(fall),undershoot*100);

}
