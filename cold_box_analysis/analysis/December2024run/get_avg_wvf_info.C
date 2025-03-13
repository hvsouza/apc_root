// ________________________________________ //
// Author: Henrique Souza
// Filename: get_avg_wvf_info.C
// Created: 2025-01-18
// ________________________________________ //

#include "MYCODES.h"


void get_avg_wvf_info(int channel=31){

  ANALYZER z("z");
  z.setAnalyzer(Form("sphe_waveforms_Ch%d.root", channel));
  double time_base_exclude = 1800;
  double exclude_base = z.head.exclusion_baselines->at(0);

  z.getSelection(Form("Ch%d.selection == 1", channel));
  z.excludeByAmplitude(8,0,time_base_exclude, exclude_base*1.2,"higher");
  z.excludeByAmplitude(8,time_base_exclude+2200, z.n_points*z.dtime, exclude_base*1.8, "higher");
  // z.excludeByAmplitude(12,3500,20000,7,"higher");
  z.persistence_plot(1000, -250, 750, 12, "use_mine");
  // TGraph *g = (TGraph*)z.f->Get("mean");
  // g->Draw("SAME");

  z.averageWaveform(0,"use_mine",0);
  z.getWaveFromHistogram(z.haverage[0]);
  // z.getWaveFromGraph(g);
  z.applyDenoise(0.2);
  TCanvas *crise = new TCanvas("crise", "crise",1920,0,1920,1080);
  z.drawGraph();
  crise->Update();
  Double_t rise = z.rise_time(0, {0,time_base_exclude}, true, {time_base_exclude,time_base_exclude+1000}, true);

  TCanvas *cfall = new TCanvas("cfall", "cfall",1920,0,1920,1080);
  z.drawGraph();
  cfall->Update();
  Double_t fall = z.fall_time(0, {0,time_base_exclude}, true, {time_base_exclude,time_base_exclude+6000}, true);

  Double_t amp = z.getMaximum(time_base_exclude,time_base_exclude+1000);
  Double_t base = z.getMean(0, time_base_exclude);
  z.applyDenoise(0.2);
  Double_t renorm = z.getMaximum(time_base_exclude,time_base_exclude+1000);

  TCanvas *cund = new TCanvas("cund", "cund",1920,0,1920,1080);
  z.scaleWvf(1./renorm);
  z.drawGraph();
  Double_t undershoot = z.getMinimum(time_base_exclude+1000,20000);

  ofstream fout(Form("sphe_wvf_info_Ch%d.txt",channel));
  fout << "A: " << amp - base << endl;
  fout << "rise: " << round(rise) << endl;
  fout << "fall: " << round(fall) << endl;
  fout << "undershoot: " << undershoot << endl;
  fout << "n_avg: " << z.lev->GetN() << endl;
  // printf("%.1f, %0.f, %.0f, %.1f\n", amp, round(rise), round(fall),undershoot*100);
  // printf("%.1f\n", amp);

}

