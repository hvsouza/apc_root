
#define memorydepth 100002
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void fft(){

  ifstream f;
  ifstream ffft;
  f.open("C1TraceC00002.txt",ios::in);
  ffft.open("SpecAnTraceC00002.txt",ios::in);
  string header;

  Int_t dtime = 1000;

  for (int i = 0; i < 5; i++) {
    getline(f,header);
  }

  for (int i = 0; i < 5; i++) {
    getline(ffft,header);
  }

  vector<Double_t> wvf(memorydepth);
  TH1D *hsignal = new TH1D("hsignal","hsignal",memorydepth,0,memorydepth*dtime);
  Double_t val;
  for (int i = 0; i < memorydepth; i++) {
    f >> val >> val;
    // cout << val << endl;
    wvf[i] = val;
    hsignal->SetBinContent(i+1,val);
  }
  Int_t nfft = 13109;
  vector<Double_t> freq(nfft);
  vector<Double_t> mag(nfft);
  Double_t val1, val2;
  for (int i = 0; i < nfft; i++) {
    ffft >> val1 >> val2;
    // cout << val1 << " " << val2 << endl;
    freq[i] = val1/1.e3;
    mag[i] = val2;
  }
  WIENER ws("ws",dtime,1,1e-9,1e6,memorydepth);
  ws.fft(hsignal);

  TCanvas *c1 = new TCanvas();
  hsignal->Draw();

  TCanvas *c2 = new TCanvas();
  ws.hfft->Draw();

  TCanvas *c3 = new TCanvas();
  TGraph *gfft = new TGraph(nfft,&freq[0],&mag[0]);
  gfft->GetXaxis()->SetRangeUser(0,200);
  gfft->GetXaxis()->SetTitle("Frequency (kHz)");
  gfft->GetYaxis()->SetTitle("Magnitude (dB)");
  gfft->Draw("AL");
}
