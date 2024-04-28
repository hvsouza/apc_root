#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"

ANALYZER *zpe = nullptr;
ANALYZER *z = nullptr;

TF1 *flar = nullptr;
TF1 *flar_gaus = nullptr;

Double_t filter = 2;

void getSPEFFT(int ch){

  cout << "Computing SPE..." << endl;

  bool all_done = false;
  Int_t navg = 0;
  zpe = new ANALYZER("zpe");
  zpe->setAnalyzer(Form("./sphe_waveforms_Ch%d.root",ch));
  string selection = "";
  TGraph *gsphe = nullptr;
  vector<Double_t> avgspe(zpe->n_points,0);
  vector<Double_t> time(zpe->n_points,0);
  if (all_done){
    gsphe = (TGraph*)zpe->f->Get("mean");
    selection = "";
  }
  else{
    zpe->getSelection(Form("Ch%d.selection==1",ch));
    cout << "Got selected by fit: " <<  zpe->lev->GetN() << endl;
    zpe->excludeByAmplitude(filter, 0, 5800, 14, "higher"); // remove pretrigger with pulse
    zpe->excludeByAmplitude(filter, 7200, 16000, 14, "higher"); // remove pretrigger with pulse
    cout << "Applied cut for amplitude" << endl;
    zpe->excludeByAmplitude(filter, 7200, 16000, -14, "lower"); // remove some undershoot
    cout << "Applied cut for undershoot" << endl;

    for (int i = 0; i < zpe->lev->GetN(); i++) {
      Int_t iev = zpe->lev->GetEntry(i);
      zpe->getWaveform(iev);
      for(Int_t j = 0; j < zpe->n_points; j++){
        avgspe[j] += zpe->ch[zpe->kch]->wvf[j];
      }
      navg += 1;

    }
    cout << "Valid SPEs = " << navg << endl;
    for(Int_t i = 0; i < zpe->n_points; i++){
      avgspe[i] = avgspe[i]/navg;
      time[i] = i*zpe->dtime;
    }
    selection="use_mine";
    gsphe = new TGraph(zpe->n_points, &time[0], &avgspe[0]);
  }
  
  zpe->persistence_plot(600,-100,500,filter,selection);
  zpe->getWaveFromGraph(gsphe);
  zpe->getFFT();
  gsphe->Draw("SAME");
}
void deconv_sample(int ch=0, bool onlySPE=false){
  Double_t reference_amplitude = 15;
  vector<Double_t> range_amplitude = {4,5};
  Int_t stop_at_event = -1;

  z = new ANALYZER("z");
  z->setAnalyzer("./analyzed.root");
  z->setChannel(Form("Ch%d.", ch));
  Int_t n_points = z->n_points;
  Int_t kch = z->kch;
  Double_t *wvf = nullptr;
  vector<Double_t> avg_wvf(n_points, 0);
  getSPEFFT(ch);

  if (onlySPE) return;
  Int_t nav = 0;
  Int_t nentries = z->nentries;
  // nentries = 100;
  for(Int_t i = 0; i < nentries; i++){ // for(Int_t i = 0; i < 100; i++){ if(i%200==0) cout << "computing event " << i << " of " << z->nentries << "\r" << flush;
    z->getWaveform(i);
    z->printev(i, z->nentries);

    wvf = z->ch[kch]->wvf;
    if(z->ch[kch]->selection == 1) continue;
    Double_t max = z->getMaximum(6100, 6800);
    if(max/reference_amplitude < range_amplitude[0] || max/reference_amplitude > range_amplitude[1]) continue;
    z->getFFT();
    z->w->baseline=5800;
    z->w->deconvolve(*z->w, *zpe->w, 1, "gaus");
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

  flar_gaus = new TF1("flar_gaus","([0]*exp(-(x-[5])/[1])*exp([4]*[4]/(2*[1]*[1])))*TMath::Erfc((([5]-x)/[4]+[4]/[1])/TMath::Power(2,0.5))/2. + ([2]*exp(-(x-[5])/[3])*exp([4]*[4]/(2*[3]*[3])))*TMath::Erfc((([5]-x)/[4]+[4]/[3])/TMath::Power(2,0.5))/2.",0,z->n_points*z->dtime);
  flar_gaus->SetParameters(100,10,3,1400,20,maxpos*z->dtime);
  flar_gaus->SetParName(0,"A_{F}");
  flar_gaus->SetParName(1,"#tau_{F}");
  flar_gaus->SetParName(2,"A_{S}");
  flar_gaus->SetParName(3,"#tau_{S}");
  flar_gaus->SetParName(4,"#sigma");
  flar_gaus->SetParName(5,"t_{0}");


  }

