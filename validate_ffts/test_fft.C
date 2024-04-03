#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void create_wvf2(ANALYZER &z, Int_t npts, Double_t step, Double_t freq, Double_t step_unit, Double_t freq_unit);
void create_wvf(ANALYZER &z, Int_t npts, Double_t step, Double_t freq, Double_t step_unit, Double_t freq_unit){
  z.ch.resize(1);
  z.ch[0] = new ADC_DATA();
  z.ch[0]->Set_npts(npts);
  z.n_points = npts;
  z.dtime = step;
  z.w = new WIENER(Form("%s_w", z.myname.c_str()), step, freq, step_unit, freq_unit, npts);
  WIENER *w = z.w;

  Double_t freq1 = 50;
  Double_t freq2 = 100;

  z.time = new Double_t[npts];

  for (Int_t i = 0; i < npts; i++){
    Double_t rnd = gRandom->Gaus(0,20);

    z.ch[0]->wvf[i]  = rnd;
    z.time[i] = i*z.dtime;
  }

  // z.drawGraph();

  z.getFFT();

  for(Int_t k=0; k<z.n_points/2+1; k++){
    if (w->hfft->GetBinCenter(k) < 0.02 || w->hfft->GetBinCenter(k) > 0.05){
      w->spec[k] = 0;
      w->spec_re[k] = w->spec[k].Re();
      w->spec_im[k] = w->spec[k].Im();
      w->idx_recompt(k);
    }
  }

  

  // z.applyBandCut()

  w->backfft(*w);
  // w->hwvf->Draw();

  for (Int_t i = 0; i < w->hwvf->GetNbinsX(); i++){
    Double_t sin1 = 0.7*TMath::Sin(2*TMath::Pi() * i * freq1 * z.dtime * w->units_step );
    Double_t sin2 = 1*TMath::Sin(2*TMath::Pi() * i * freq2 * z.dtime * w->units_step);
    w->hwvf->SetBinContent(i+1, sin1);
    w->hwvf->AddBinContent(i+1, sin2);
    z.ch[0]->wvf[i]  = w->hwvf->GetBinContent(i+1);
  }


  w->fft(w->hwvf);
  w->backfft(*w);
  // w->convertDecibel(w->hfft, 16e3);
  // w->convertDecibel(w->hPSD, 16e3);

  Double_t integral = 0;
  Double_t integral_fft = 0;
  for(Int_t k=0; k<z.n_points/2+1; k++){
    integral+=w->hPSD->GetBinContent(k+1);
    integral_fft+=pow(w->hfft->GetBinContent(k+1),2);
  }
  cout << z.w->powerSpectrum << endl;
  z.integrate(0,0,0,false,0,true);
  cout << integral_fft << " " << " "  << integral << " " << w->hPSD->Integral() << " " << z.temp_charge/npts/z.dtime << endl;

}

void test_fft(){

  ANALYZER z("z");
  create_wvf(z, 15000, 2, 0.5, 1e-3, 1e3);

  ANALYZER s("s");
  create_wvf(s, 120000, 300, 3.3334, 1e-6, 1e3);


  TCanvas *cback = new TCanvas("cback", "cback",1920,0,1920,1080);
  z.w->hwvf->Draw();
  TCanvas *cback2 = new TCanvas("cback2", "cback2",1920,0,1920,1080);
  s.w->hwvf->SetLineColor(kRed);
  s.w->hwvf->Draw("");

  TCanvas *cfft = new TCanvas("cfft", "cfft",1920,0,1920,1080);
  cfft->cd();
  z.w->hfft->Draw();
  s.w->hfft->SetLineColor(kRed);
  s.w->hfft->Draw("SAME");


  TCanvas *cPSD = new TCanvas("cPSD", "cPSD",1920,0,1920,1080);
  cPSD->cd();
  z.w->hPSD->Draw();
  s.w->hPSD->SetLineColor(kRed);
  s.w->hPSD->Draw("SAME");


}

void create_wvf2(ANALYZER &z, Int_t npts, Double_t step, Double_t freq, Double_t step_unit, Double_t freq_unit){
  z.ch.resize(1);
  z.ch[0] = new ADC_DATA();
  z.ch[0]->Set_npts(npts);
  z.n_points = npts;
  z.w = new WIENER(Form("%s_w", z.myname.c_str()), 0.05, 20, 1, 1, npts);
  WIENER *w = z.w;

  Double_t freq1 = 1./(4*TMath::Pi());

  z.time = new Double_t[npts];


  for (Int_t i = 0; i < w->hwvf->GetNbinsX(); i++){
    Double_t sin1 = 1*TMath::Sin(2*TMath::Pi() * i * 0.05 * freq1 );
    // Double_t sin2 = 1*TMath::Sin(2*TMath::Pi() * i * freq2 * 4 * 1e-9 * 1e6);
    w->hwvf->SetBinContent(i+1, sin1);
    // w->hwvf->AddBinContent(i+1, sin2);
  }


  w->fft(w->hwvf);
  w->backfft(*w);
  // w->convertDecibel(w->hfft, 0);
  // w->convertDecibel(w->hPSD, 0);

  Double_t integral = 0;
  Double_t integral_fft = 0;
  for(Int_t k=0; k<z.n_points/2+1; k++){
    integral+=w->hPSD->GetBinContent(k+1);
    integral_fft+=w->hfft->GetBinContent(k+1);
  }
  z.integrate();
  cout << integral_fft <<  " " << integral << " " << z.temp_charge << endl;

}
