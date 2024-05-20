#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"
#include "/home/henrique/Dropbox/APC_Paris/Root/ProtoDUNE_HD_ANA/waveselector.C"

ANALYZER *z = nullptr;

TF1 *flar = nullptr;
TF1 *flar_gaus = nullptr;

Double_t filter = 2;

void deconvolve(int ch=34, bool checkSelection=false, string spe_file_name = "", bool isSPE = false){
  Double_t reference_amplitude = 6;
  vector<Double_t> range_amplitude = {200,300};
  Int_t stop_at_event = -1;

  z = new ANALYZER("z");
  z->setAnalyzer("./analyzed.root");

  bool chok = false;
  chok = z->setChannel(Form("Ch%d.", ch));
  if (!chok) return;

  Int_t n_points = z->n_points;
  Int_t kch = z->kch;
  Double_t *wvf = nullptr;
  vector<Double_t> avg_wvf(n_points, 0);

  waveselector(ch, spe_file_name, isSPE);
  if (checkSelection) return;

  Int_t nav = 0;
  Int_t nentries = z->nentries;
  // nentries = 100;
  for(Int_t i = 0; i < nentries; i++){ // for(Int_t i = 0; i < 100; i++){ if(i%200==0) cout << "computing event " << i << " of " << z->nentries << "\r" << flush;
    z->getWaveform(i);
    z->printev(i, z->nentries);

    wvf = z->ch[kch]->wvf;
    if(z->ch[kch]->selection == 1) continue;
    Double_t max = z->getMaximum(1000, 1200);
    if(max/reference_amplitude < range_amplitude[0] || max/reference_amplitude > range_amplitude[1]) continue;
    z->getMaximum(3000,16000);
    if (z->temp_max > 300) continue;
    z->getMinimum(5000,5100);
    if (z->temp_min < -400) continue;
    z->getMinimum(6100,6200);
    if (z->temp_min < -20) continue;


    z->getFFT();
    z->w->maxBin = 2000/z->dtime;
    z->w->baseline=1000;
    z->w->deconvolve(*z->w, *zpe->w, 2., "gaus");
    if (stop_at_event >= i){
      z->drawGraph("");
      z->w->hwvf->SetLineColor(kRed);
      z->w->hwvf->Draw("SAME");
      break;
    }
    for(Int_t j = 0; j < n_points; j++){
      // avg_wvf[j] +=  wvf[j];
      avg_wvf[j] +=  z->w->hwvf->GetBinContent(j+1);
    }
    nav++;
  }

  Double_t *time = new Double_t[n_points];
  Double_t max = -1e12;
  Double_t maxpos = 0;
  for(Int_t j = 0; j < n_points; j++){
    avg_wvf[j] = avg_wvf[j]/nav;
    time[j] = j*z->dtime;
    if(avg_wvf[j] > max){
      max = avg_wvf[j];
      maxpos = j;
    }
  }
  cout << "\n";
  cout << "Found " << nav << " waveforms" << endl;
  TCanvas *c1 = new TCanvas("c1", "c1",1920,0,1920,1080);
  TGraph *gwvf = new TGraph(n_points, time, &avg_wvf[0]);
  gwvf->Draw("AL");
  gwvf->GetXaxis()->SetTitle("Time (ns)");
  gwvf->GetYaxis()->SetTitle("Amplitude (A.U.)");


  flar = new TF1("flar",Form("[0]*exp(-(x-%f)/[1])+[2]*exp(-(x-%f)/[3])",maxpos*z->dtime,maxpos*z->dtime),0,z->n_points*z->dtime);
  flar->SetParameters(100,10,3,1400);
  flar->SetParName(0,"A_{F}");
  flar->SetParName(1,"#tau_{F}");
  flar->SetParName(2,"A_{S}");
  flar->SetParName(3,"#tau_{S}");

  flar_gaus = new TF1("flar_gaus","[4]*( ([0]*exp(-(x-[5])/[1])*exp([3]*[3]/(2*[1]*[1])))*TMath::Erfc((([5]-x)/[3]+[3]/[1])/TMath::Power(2,0.5))/2./[1] + ((1-[0])*exp(-(x-[5])/[2])*exp([3]*[3]/(2*[2]*[2])))*TMath::Erfc((([5]-x)/[3]+[3]/[2])/TMath::Power(2,0.5))/2./[2] )",0,z->n_points*z->dtime);
  flar_gaus->SetParameters(0.27,30,1200,50,500*TMath::MaxElement(n_points, gwvf->GetY()),maxpos*z->dtime);
  flar_gaus->SetParName(0,"A_{F}");
  flar_gaus->SetParName(1,"#tau_{F}");
  flar_gaus->SetParName(2,"#tau_{S}");
  flar_gaus->SetParName(3,"#sigma");
  flar_gaus->SetParName(4,"Scale");
  flar_gaus->SetParName(5,"t_{0}");


  }

