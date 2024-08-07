// ________________________________________ //
// Author: Henrique Souza
// Filename: randomCoincidence.C
// Created: 2022-11-15
// ________________________________________ //
//
#define memorydepth 100
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"

class DET{
  public:
    Double_t freq = 0;
    Double_t counts_per_time = 0;
    Double_t per_time = 10e-6; // in microseconds
    Double_t sipms = 80;
    Double_t area_sipms = 36;
    TRandom3 *r = nullptr;
    Int_t seed;


    Double_t window = 0.5; // total window of acquisition
    Double_t coinc_window = 100e-9; // coincidence window
    Double_t timestep = 10e-9; // time steps
    Int_t coinc_bin = 0;
    Double_t prob_per_step = 0;

    vector<Double_t> hits;
    vector<Double_t> time;
    Int_t n = 0;


    void generateEvents(){
      Int_t nevents = 0;
      Double_t dice = 0;
      for (Int_t i = 0; i < n; i++) {
        dice = r->Uniform(1);
        if(dice<=prob_per_step){
          hits[i] = 1;
          nevents++;
        }
        else{
          hits[i] = 0;
        }
      }
      // cout << nevents << endl;
    }


    void setDet(Double_t mfreq, Double_t mcoinc_window = 100e-9){
      freq = mfreq;
      coinc_window = mcoinc_window;
      coinc_bin = static_cast<Int_t>(coinc_window/timestep);
      freq = freq*sipms*area_sipms;
      freq = r->Gaus(freq,sqrt(freq));
      counts_per_time = freq*per_time;
      prob_per_step = 1. - exp(-freq*timestep);


      // Double_t R = 4*pow(freq,4)*pow(coinc_window,3);
      // Double_t trigger_per_time = R*per_time;
      // cout << "freq = " << freq << " R = " << R << " " << trigger_per_time << endl;

    }

    DET(Int_t mseed) : seed{mseed}{
      r = new TRandom3(seed);
      n = window/timestep;
      hits.resize(n);
      time.resize(n);
      for (Int_t i = 0; i < n; i++) time[i] = i*timestep*1e6;
    }
};



void randomCoincidence(){

  Int_t ndet = 8;

  vector<DET*> det(ndet);

  for (Int_t i = 0; i < ndet; i++) det[i] = new DET(i+1);

  const Int_t nvar = 50;
  Int_t avg = 200;
  Double_t _nrate[nvar] = {};
  Double_t _ncoinc[nvar] = {};
  Double_t _log_ncoinc[nvar] = {};


  Double_t ncounts[nvar] = {};
  Double_t ncounts_error[nvar] = {};
  Double_t nratex[nvar] = {};
  Double_t nratex_error[nvar] = {};
  Double_t lognratex[nvar] = {};
  Double_t lognratex_error[nvar] = {};
  Double_t r_random[nvar] = {};
  Double_t r_random_error[nvar] = {};
  Double_t log_r_random[nvar] = {};
  Double_t log_r_random_error[nvar] = {};
  Double_t r_counts[nvar] = {};
  Double_t r_counts_error[nvar] = {};

  for (Int_t i = 0; i < nvar; i++) {
    _ncoinc[i] = 10e-9 + 10e-9*i;
    _nrate[i] = 10*(i+1);
  }

  Int_t totalwhile = 0;
  for(Int_t ntimes = 0; ntimes < avg; ntimes++)
  {
    Int_t aux = 0;
    totalwhile = 0;
    while (true) {
      totalwhile++;
      printf("\r%d, Event %d", ntimes, aux);
      fflush(stdout);
      for (Int_t i = 0; i < ndet; i++) {
        // det[i] = new DET(100,100e-9,i+1);
        // det[i]->setDet(70,_ncoinc[aux]);
        det[i]->setDet(_nrate[aux],100e-9);
        det[i]->generateEvents();

      }

      Int_t trigger = 0;
      Int_t coinc_bin = det[0]->coinc_bin;
      Int_t before = 0;
      Int_t after = 0;
      Int_t n = det[0]->n;
      Int_t ntriggers = 0;
      for(Int_t k = 0; k < n; k++){
        trigger = 0;
        for (Int_t i = 0; i < ndet; i++){
          if(det[i]->hits[k]>0.5){
            trigger++;
            break;
          }
        }
        if(trigger!=0){
          // before = k-coinc_bin < 0 ? 0 : k - coinc_bin;
          before = k;
          after = k+coinc_bin > n-1 ? n-1 : k + coinc_bin;
          trigger = 0;
          for (Int_t i = 0; i < ndet; i++) {
            for(Int_t j = before; j <= after; j++){
              if(det[i]->hits[j]>0.5) {
                trigger++;
                break;
              }
            }
            if(trigger != i+1) break;
          }
          if(trigger == ndet){
            ntriggers++;
            // cout << "One coincidence !! " << det[0]->time[k] << endl;
            // k = k + coinc_bin/2;
          }
        }
      }
      nratex[aux] = _nrate[aux]*det[0]->sipms*det[0]->area_sipms;
      nratex_error[aux] = sqrt(nratex[aux]);
      ncounts[aux] = nratex[aux]*det[0]->per_time;
      ncounts_error[aux] = sqrt(nratex[aux])*det[0]->per_time;
      r_random[aux] += ntriggers/det[0]->window/avg;
      r_counts[aux] += ntriggers*det[0]->per_time/det[0]->window/avg;


      aux++;
      if(aux==nvar) break;
    }
    printf("\n");

  }

  for(Int_t aux = 0; aux < totalwhile; aux++){
    // r_random_error[aux] = (r_random[aux] < 20*det[0]->window*avg) ? sqrt(r_random[aux]/det[0]->window/avg) : sqrt(r_random[aux]);
    r_random_error[aux] = sqrt(r_random[aux]/det[0]->window/avg);
    r_counts_error[aux] = r_random_error[aux]*det[0]->per_time;
    lognratex[aux] = log(nratex[aux]);
    lognratex_error[aux] = nratex_error[aux]/nratex[aux];
    _log_ncoinc[aux] = log(_ncoinc[aux]);
    if(r_random[aux] > 0){
      log_r_random[aux] = log(r_random[aux]);
      log_r_random_error[aux] = r_random_error[aux]/r_random[aux];
    }
    else {
      r_random[aux] = log(1e-8);
      log_r_random[aux] = 1e-8;
      nratex[aux] = log(1e-8);
      lognratex[aux] = 1e-8;
      _log_ncoinc[aux] = 1e-8;
      _ncoinc[aux] = 0.1e-9;
    }
  }

  TCanvas *c1 = new TCanvas("c1","R vs R");
  TGraphErrors *gw = new TGraphErrors(nvar,nratex,r_random,nratex_error,r_random_error);
  gw->SetMarkerStyle(21);
  gw->GetXaxis()->SetTitle("R_{nominal} (Hz)");
  gw->GetYaxis()->SetTitle("R_{random} (Hz)");
  gw->Draw("AP");

  TCanvas *c2 = new TCanvas("c2", "R vs T");
  TGraphErrors *gc = new TGraphErrors(nvar,_ncoinc,r_random, 0, r_random_error);
  gc->GetXaxis()->SetTitle("T(s)");
  gc->GetYaxis()->SetTitle("R_{random} (Hz)");
  gc->SetMarkerStyle(21);
  gc->Draw("AP");

  TCanvas *c3 = new TCanvas("c3","log R vs log R");
  TGraphErrors *gcoinc = new TGraphErrors(nvar,lognratex,log_r_random, lognratex_error, log_r_random_error);
  gcoinc->GetXaxis()->SetTitle("Log(R_{nominal}[Hz])");
  gcoinc->GetYaxis()->SetTitle("Log(R_{random}[Hz])");
  gcoinc->SetMarkerStyle(21);
  gcoinc->Draw("AP");

  TCanvas *c4 = new TCanvas("c4","log R vs log T");
  TGraphErrors *g5 = new TGraphErrors(nvar,_log_ncoinc,log_r_random, 0, log_r_random_error);
  g5->GetXaxis()->SetTitle("Log(T[s])");
  g5->GetYaxis()->SetTitle("Log(R_{random}[Hz])");
  g5->SetMarkerStyle(21);
  g5->Draw("AP");

  TCanvas *c5 = new TCanvas("c5", "C vs C");
  TGraphErrors *g6 = new TGraphErrors(nvar, nratex, r_random, nratex_error, r_random_error);

  c5->cd();
  c5->SetLogy();
  g6->GetXaxis()->SetTitle("Rate of few p.e. per channel (Hz)");
  g6->GetYaxis()->SetTitle("Random trigger rate (Hz)");
  // g6->GetYaxis()->SetTitle(Form("Random trigger per %d #mus",Int_t(det[0]->per_time*1e6)));
  g6->SetMarkerStyle(21);
  g6->Draw("AP");   //draw an axis on the right side

  Double_t xmin = g6->GetXaxis()->GetXmin();
  Double_t xmax = g6->GetXaxis()->GetXmax();
  Double_t ymin = 0.5;
  Double_t ymax = 20e3;
  g6->GetYaxis()->SetRangeUser(ymin,ymax);
  // Double_t ymin = g6->GetYaxis()->GetXmin();
  // Double_t ymax = g6->GetYaxis()->GetXmax();
  TGaxis *axis = new TGaxis(xmax,ymin,xmax,ymax,(ymin*det[0]->per_time), (ymax*det[0]->per_time),510,"+LG");
  axis->SetTitle(Form("Random trigger per %d #mus",Int_t(det[0]->per_time*1e6)));
  axis->SetLineColor(kRed);
  axis->SetLabelColor(kRed);
  axis->SetTitleColor(kRed);
  axis->Draw();

  TF1 *fteo = new TF1("fteo",Form("[0]*%d*pow(x,%d)*pow(100e-9,%d)",ndet,ndet,ndet-1),1,30);
  fteo->FixParameter(0,1);
  g6->Fit("fteo");





}
