#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void get_rise_and_fall_time(int channel = 0){

  ANALYZER z("z");
  z.setAnalyzer(Form("sphe_waveforms_Ch%d.root", channel));

  Double_t veryend = z.n_points*z.dtime;
  Double_t finishbase = 5000;
  Double_t finishsignal = 7000;

  z.getSelection(Form("Ch%d.selection == 1", channel));
  z.excludeByAmplitude(4,0,finishbase, 5,"higher");
  // z.excludeByAmplitude(12,3500,20000,7,"higher");
  z.persistence_plot(500, -250, 250, 2, "use_mine");
  // TGraph *g = (TGraph*)z.f->Get("mean");
  // g->Draw("SAME");

  z.averageWaveform(0,"use_mine",0);
  z.getWaveFromHistogram(z.haverage[0]);
  // z.getWaveFromGraph(g);
  z.applyDenoise(0.2);
  TCanvas *crise = new TCanvas("crise", "crise",1920,0,1920,1080);
  z.drawGraph();
  crise->Update();
  Double_t rise = z.rise_time(0, {0,finishbase}, true, {finishbase,finishsignal}, true);

  TCanvas *cfall = new TCanvas("cfall", "cfall",1920,0,1920,1080);
  z.drawGraph();
  cfall->Update();
  Double_t fall = z.fall_time(0, {0,finishbase}, true, {finishbase,veryend}, true);

  Double_t amp = z.getMaximum(finishbase,finishsignal);
  z.applyDenoise(0.2);
  Double_t renorm = z.getMaximum(finishbase,finishsignal);

  TCanvas *cund = new TCanvas("cund", "cund",1920,0,1920,1080);
  z.scaleWvf(1./renorm);
  z.drawGraph();
  Double_t undershoot = z.getMinimum(finishsignal,veryend);

  cout << "A: " << amp << endl;
  cout << "rise: " << round(rise) << endl;
  cout << "fall: " << round(fall) << endl;
  cout << "undershoot: " << undershoot << endl;
  printf("%.1f, %0.f, %.0f, %.1f\n", amp, round(rise), round(fall),undershoot*100);

}
