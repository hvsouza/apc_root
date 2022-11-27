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
    Int_t seed;
    TRandom3 *r = nullptr;


    Double_t window = 0.05; // total window of acquisition
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


      Double_t R = 2*freq*freq*coinc_window;
      Double_t trigger_per_time = R*per_time;
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

  Int_t ndet = 4;

  vector<DET*> det(ndet);

  for (Int_t i = 0; i < ndet; i++) det[i] = new DET(i+1);

  const Int_t nvar = 100;
  Int_t avg = 5;
  Double_t _nrate[nvar] = {};
  Double_t _ncoinc[nvar] = {};
  Double_t _log_ncoinc[nvar] = {};


  Double_t ncounts[nvar] = {};
  Double_t nratex[nvar] = {};
  Double_t lognratex[nvar] = {};
  Double_t r_random[nvar] = {};
  Double_t log_r_random[nvar] = {};
  Double_t r_counts[nvar] = {};

  for (Int_t i = 0; i < nvar; i++) {
    _ncoinc[i] = 10e-9 + 10e-9*i;
    _nrate[i] = 1 + 10*i;
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
        det[i]->setDet(100,_ncoinc[aux]);
        // det[i]->setDet(_nrate[aux],100e-9);
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
        if(det[0]->hits[k]>0.5){
          trigger++;
          before = k-coinc_bin < 0 ? 0 : k - coinc_bin;
          after = k+coinc_bin > n ? n : k + coinc_bin;
          for (Int_t i = 1; i < ndet; i++) {
            for(Int_t j = before; j < after; j++){
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
      ncounts[aux] = nratex[aux]*det[0]->per_time;
      r_random[aux] += ntriggers/det[0]->window/avg;
      r_counts[aux] += ntriggers*det[0]->per_time/det[0]->window/avg;


      aux++;
      if(aux==nvar) break;
    }
    printf("\n");

  }

  for(Int_t aux = 0; aux < totalwhile; aux++){
      lognratex[aux] = log(nratex[aux]);
      _log_ncoinc[aux] = log(_ncoinc[aux]);
      if(r_random[aux] > 0){
        log_r_random[aux] = log(r_random[aux]);
      }
      else{
        r_random[aux] = log(0.00000001);
        nratex[aux] = log(0.000000001);
      }
  }

  TCanvas *c1 = new TCanvas("c1","R vs R");
  TGraph *gw = new TGraph(nvar,nratex,r_random);
  gw->SetMarkerStyle(21);
  gw->GetXaxis()->SetTitle("R_{nominal} (Hz)");
  gw->GetYaxis()->SetTitle("R_{random} (Hz)");
  gw->Draw("AP");

  TCanvas *c2 = new TCanvas("c2", "R vs T");
  TGraph *gc = new TGraph(nvar,_ncoinc,r_random);
  gc->GetXaxis()->SetTitle("T(s)");
  gc->GetYaxis()->SetTitle("R_{nominal} (Hz)");
  gc->SetMarkerStyle(21);
  gc->Draw("AP");

  TCanvas *c3 = new TCanvas("c3","log R vs log R");
  TGraph *gcoinc = new TGraph(nvar,lognratex,log_r_random);
  gcoinc->GetXaxis()->SetTitle("Log(R_{nominal}[Hz])");
  gcoinc->GetYaxis()->SetTitle("Log(R_{random}[Hz])");
  gcoinc->SetMarkerStyle(21);
  gcoinc->Draw("AP");

  TCanvas *c4 = new TCanvas("c4","log R vs log T");
  TGraph *g5 = new TGraph(nvar,_log_ncoinc,log_r_random);
  g5->GetXaxis()->SetTitle("Log(T[s])");
  g5->GetYaxis()->SetTitle("Log(R_{random}[Hz])");
  g5->SetMarkerStyle(21);
  g5->Draw("AP");

  TCanvas *c5 = new TCanvas("c5", "C vs C");
  TGraph *g6 = new TGraph(nvar, ncounts, r_counts);

  c5->cd();
  c5->SetLogy();
  g6->GetXaxis()->SetTitle(Form("Events per %d #mus",Int_t(det[0]->per_time*1e6)));
  g6->GetYaxis()->SetTitle(Form("Random trigger per %d #mus",Int_t(det[0]->per_time*1e6)));
  g6->SetMarkerStyle(21);
  g6->Draw("AP");   //draw an axis on the right side

  Double_t xmin = g6->GetXaxis()->GetXmin();
  Double_t xmax = g6->GetXaxis()->GetXmax();
  Double_t ymin = g6->GetYaxis()->GetXmin();
  Double_t ymax = g6->GetYaxis()->GetXmax();
  TGaxis *axis = new TGaxis(xmax,ymin,xmax,ymax,(ymin/det[0]->per_time), (ymax/det[0]->per_time),510,"+LG");
  axis->SetTitle("R_{random} (Hz)");
  axis->SetLineColor(kRed);
  axis->SetLabelColor(kRed);
  axis->Draw();





}