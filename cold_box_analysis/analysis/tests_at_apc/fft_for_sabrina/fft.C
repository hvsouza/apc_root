
#define memorydepth 4000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void fft(){

  ifstream f;
  f.open("data.txt",ios::in);
  string header;

  Int_t dtime = 4;

  for (int i = 0; i < 6; i++) {
    getline(f,header);
  }

  vector<Double_t> wvf(memorydepth);
  TH1D *hsignal = new TH1D("hsignal","hsignal",memorydepth,0,memorydepth*dtime);

  for (int i = 0; i < memorydepth; i++) {
    f >> val;
    wvf[i] = val;
    hsignal->SetBinContent(i+1,val);
  }

  WIENER ws;
  WIENER ws("ws",dtime,250,1e-9,1e6,memorydepth);
  ws.fft(hsignal);

  TCanvas *c1 = new TCanvas();
  hsignal->Draw();

  TCanvas *c2 = new TCanvas();
  ws.hfft->Draw();
}
