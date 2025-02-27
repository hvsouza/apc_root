#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"

void setup_channel(string channel, vector<string> &files);
void plot_wvfs(string channel = "Ch0."){
  // gStyle->SetCanvasPreferGL(kFALSE);
  bool just_a_test = false;
  string mychannel = channel;
  Double_t minInt = 10380;
  Double_t maxInt = 11400;
  if(mychannel == "Ch0."){minInt = 2500; maxInt = 6000;}
  if(mychannel == "Ch1."){minInt = 2500; maxInt = 6000;}
  if(mychannel == "Ch2."){minInt = 2500; maxInt = 6000;}
  if(mychannel == "Ch3."){minInt = 2500; maxInt = 6000;}
  if(mychannel == "Ch4."){minInt = 2500; maxInt = 6000;}
  if(mychannel == "Ch5."){minInt = 2500; maxInt = 6000;}
  if(mychannel == "Ch6."){minInt = 2500; maxInt = 6000;}
  if(mychannel == "Ch7."){minInt = 2500; maxInt = 6000;}
  vector<string> devices = { "C1 SoF 1", "C1 SoF 2", "C2 SoF 1", "C2 SoF 2", "C3 SoF 1", "C3 SoF 2", "C4 SoF 1", "C4 SoF 2"};

  vector<Double_t> saturations_full = { 11800, 11100, 12000, 16300, 16300, 16300, 11000, 6000};
  vector<Double_t> saturations = { 9000, 8000, 9600, 5500, 16300, 16300, 8500, 4200};
  // vector<Double_t> sphes = {7100,400, 7100, 5800*9.97/515.25, 5800, 0, 0, 0};
  vector<Double_t> sphes = {6178, 5306, 3357, 2609, 1179, 1537, 3417, 1731};
  vector<Double_t> sphes_amp = {7.3, 6.6, 6.2, 5.1, 2.2, 2.9, 7.2, 3.8};

  vector<Double_t> fpromptcut = {0.42, 0.42, 0.57, 0.57, 1, 1, 0.62, 0.62};

  
  vector<string> files;
  setup_channel(channel, files);

  Int_t n = files.size();
  Double_t saturation_level;
  Double_t saturation_full;

  Int_t nbins;
  Double_t dtime = 2;

  string conc = "/analyzed.root";

  vector<ANALYZER*> z(n);

  TH2D *h2p = new TH2D("h2p","h2p",500,0,5000,200,0,1);
  TH2D *h2pvspe = new TH2D("h2pvspe", "h2pvspe", 1000, -1000, 5000, 2000, -1000, 16000);


  TGraphErrors *gpeaks = new TGraphErrors();
  int gpts = 0;
  TCanvas *c1 = new TCanvas("c1","c1",1920,0,1920,1080);
  c1->DrawFrame(0,-.1,20000,1.1);

  Double_t spe = 1;
  Double_t spe_amp= 1;
  vector<vector<Double_t>> group_amp = {
    {50,100},
    {100,200},
    {200,400},
    {400,600},
    {600,1000},
    {1000,2000},
  };
  Int_t ngroups = group_amp.size();
  vector<TCanvas*> cpers_group(ngroups);
  vector<TH1D*> h(ngroups);
  vector<TH1D*> hnorm(ngroups);
  vector<TH2D*> hpers(ngroups);
  vector<TH1D*> hfft(ngroups);
  vector<TH1D*> hrise(ngroups);
  vector<Int_t> nev(ngroups);
  THStack *hs = new THStack("hs", "hs");
  THStack *hsfft = new THStack("hsfft", "hsfft");
  THStack *hsrise = new THStack("hsrise", "hsrise");

  ANALYZER *a = nullptr;

  Bool_t inDecibel = true;

  Int_t idx_of_channel;
  for(Int_t i = 0; i<n; i++){
    cout << "File: " << files[i] << endl;
    files[i] = files[i]+conc;
    z[i] = new ANALYZER(Form("z%d",i));
    z[i]->setAnalyzer(files[i].c_str());
    z[i]->setChannel(mychannel.c_str());
    a = z[i];
    dtime = a->dtime;
    Int_t kch = z[i]->kch;
    idx_of_channel = z[i]->getIdx();
    saturation_level = saturations[idx_of_channel];
    saturation_full = saturations_full[idx_of_channel];
    spe = sphes[idx_of_channel];
    spe_amp = sphes_amp[idx_of_channel];
    

    int npoints = a->n_points;
    if (i == 0){
      for(Int_t k = 0; k < ngroups; k++){
        h[k] = new TH1D(Form("h%d",k),Form("h%d",k),npoints, 0, npoints*z[i]->dtime);
        hpers[k] = new TH2D(Form("hpers%d",k),Form("hpers%d",k), npoints, 0, npoints*z[i]->dtime, 2000, -250, 16000);
        hfft[k] = (TH1D*)a->w->hfft->Clone(Form("hfft_%s_ch%d","w",kch));
        hfft[k]->Reset();
        hrise[k] = new TH1D(Form("hrise%d",k), Form("hrise%d",k), 200, 0, 200);
      }
    }

    a->getSelection(Form("%sselection == 0", channel.c_str()));
    a->excludeByAmplitude(12, 0, 2820, 5*spe_amp);
    
    // for(Int_t j = 0; j < a->nentries; j++){
    for(Int_t j = 0; j < a->lev->GetN(); j++){
      // cout << j << " " << a->lev->GetN() << endl;

      a->getWaveform(a->lev->GetEntry(j));
      // a->applyDenoise(16);
      a->integrate(minInt,maxInt,0,false);
      if (a->temp_pos > minInt + (maxInt-minInt)/3.) continue;
      Double_t max = a->temp_max;
      Double_t charge = a->temp_charge;
      a->getMaximum(8000,10000);
      if (a->temp_max > 100*spe_amp) continue;
      Int_t idx_rise = 0;
      for (Int_t l = 0; l < npoints; l++){
        if (a->ch[kch]->wvf[l] >= 0.1*max){
          idx_rise = l;
          break;
        }
      }
      if (idx_rise < minInt/dtime) continue;

      a->integrate(minInt,minInt+(maxInt-minInt)/3.);
      Double_t cfast = a->temp_charge;
      if(max > 100)
      {
        h2p->Fill(charge/spe,cfast/charge);
      }
      h2pvspe->Fill(charge/spe, max);
      if (cfast/charge>fpromptcut[idx_of_channel] && charge/spe > 100){
        continue;
      }
      if(max < saturation_full){
        gpeaks->SetPoint(gpts,charge/spe,max);
        gpeaks->SetPointError(gpts,sqrt(charge/spe),sqrt(max));
        gpts++;
      }
      
      if (max > saturation_level) continue;
      vector<Double_t> shifted_wvf;
      for (Int_t ishift = idx_rise - 1000; ishift < npoints; ishift++){
        shifted_wvf.push_back(a->ch[kch]->wvf[ishift]);
      }
      for(Int_t k = 0; k < ngroups; k++){
        if(max/spe_amp >= group_amp[k][0] && max/spe_amp <= group_amp[k][1]){
        // if(charge/spe >= group_amp[k][0] && charge/spe <= group_amp[k][1]){
          for(Int_t l = 0; l < shifted_wvf.size(); l++){
            h[k]->AddBinContent(l+1,shifted_wvf[l]);
            hpers[k]->Fill(l*a->dtime,shifted_wvf[l]);
          }
          // for(Int_t l = 0; l < npoints; l++){
          //   h[k]->AddBinContent(l+1,a->ch[kch]->wvf[l]);
          //   hpers[k]->Fill(l*a->dtime,a->ch[kch]->wvf[l]);
          // }
          a->getFFT();
          for (Int_t j = 0; j < npoints/2; j++) hfft[k]->AddBinContent(j+1,a->w->hfft->GetBinContent(j+1));

          Double_t rt = a->rise_time(kch, {0,minInt}, true, {minInt, minInt + (maxInt-minInt)/3.});
          hrise[k]->Fill(rt);

          nev[k] += 1;
        }
      }
    }

    // TF1 *fpol = new TF1("fpol","[0]+[1]*x",0,4000);
    // h2p->Fit("fpol","WL");
  }
  h2p->Draw("colz");

  TCanvas *cpers = new TCanvas("cpers", "cpers",1920,0,1920,1080); cpers->cd();
  h2pvspe->Draw("colz");
  h2pvspe->GetXaxis()->SetTitle("Photo-electrons");
  h2pvspe->GetYaxis()->SetTitle("Amplitude (ADC Channels)");
  gpeaks->Draw("P SAME");


  TCanvas *c2 = new TCanvas("c2", "c2",1920,0,1920,1080);
  c2->DrawFrame(0,-0.1,10000,1.1);


  for(Int_t k = 0; k < ngroups; k++){
    h[k]->Scale(1./nev[k]);
    hnorm[k] = (TH1D*)h[k]->Clone(Form("hnorm%d",k));
    Double_t max = hnorm[k]->GetMaximum();
    hnorm[k]->Scale(1./max);
    stringstream stream;
    stream << std::fixed << std::setprecision(1) << max;
    string correct_max = stream.str();
    string hist_name = Form("%s - Max = %s (%d entries)",devices[idx_of_channel].c_str(), correct_max.c_str(), nev[k]);
    hnorm[k]->SetTitle(hist_name.c_str());
    hnorm[k]->SetLineWidth(2);
    hs->Add(hnorm[k],"hist");


    cpers_group[k] = new TCanvas(Form("cpers_%d", k), Form("cpers_%d", k),1920,0,1920,1080);
    cpers_group[k]->cd();
    hpers[k]->Draw("colz");
    hpers[k]->GetXaxis()->SetTitle("Time (ns)");
    hpers[k]->GetYaxis()->SetTitle("Amplitude (ADC Channels)");
    cpers_group[k]->SetLogz(true);
    h[k]->SetLineWidth(3);
    h[k]->Draw("SAME");
    

    hfft[k]->Scale(1./nev[k]);
    hfft[k]->SetEntries(nev[k]);
    if (inDecibel){
      a->w->convertDecibel(hfft[k], 16e3);
      hfft[k]->GetYaxis()->SetTitle("Magnitude (dB_{FS})");
    }
    hfft[k]->SetTitle(hist_name.c_str());
    hsfft->Add(hfft[k], "hist");


    string rise_name = Form("%s - Mean = %.1f (%d entries)",devices[idx_of_channel].c_str(), hrise[k]->GetMean(), nev[k]);
    hrise[k]->Scale(1./nev[k]);
    hrise[k]->SetTitle(rise_name.c_str());
    hrise[k]->SetLineWidth(2);
    hsrise->Add(hrise[k], "hist");
  }
  c2->cd();
  hs->GetXaxis()->SetTitle("Time (ns)");
  hs->GetYaxis()->SetTitle("Amplitude (A.U.)");
  hs->Draw("nostack plc same");
  c2->BuildLegend();



  TCanvas *cfft = new TCanvas("cfft","cfft",1920,0,1920,1080);
  cfft->cd();
  cfft->DrawFrame(0, -100, 250, 100);
  hsfft->GetXaxis()->SetTitle(hfft[0]->GetXaxis()->GetTitle());
  hsfft->GetYaxis()->SetTitle(hfft[0]->GetYaxis()->GetTitle());
  hsfft->Draw("nostack plc same");
  cfft->BuildLegend();

  TCanvas *crise = new TCanvas("crise","crise",1920,0,1920,1080);
  crise->cd();
  hsrise->Draw("nostack plc");
  hsrise->GetXaxis()->SetTitle("Rise time (ns)");
  hsrise->GetYaxis()->SetTitle("Normalized counts");
  crise->BuildLegend();

  // vector<string> outfilename = { "averaged_C1_SoF1", "averaged_C1_SoF2", "averaged_C2_SoF1", "averaged_C2_SoF2", "averaged_C3_SoF1", "averaged_C3_SoF2", "averaged_C4_SoF1", "averaged_C4_SoF2"};
  //   TFile *faverage = new TFile(Form("%s.root",outfilename[idx_of_channel].c_str()), "RECREATE");
  // for(Int_t k = 0; k < ngroups; k++){
  //   faverage->WriteTObject(h[k], Form("h%d",k), "TObject::kOverwrite");
  // }

  // faverage->Close();
  // delete faverage;
}

void setup_channel(string channel, vector<string> &files){

  if (channel == "Ch0."){
    files = {
      "run279_cosmic_run_trigger_on_C1_SoF1", 
      "run280_cosmic_run_trigger_on_C1_SoF1_600ADC", 
    };
  }
  else if (channel == "Ch1."){
    files = {
      "run281_cosmic_run_trigger_on_C1_SoF2_60ADC", 
      "run282_cosmic_run_trigger_on_C1_SoF2_600ADC"
    };
  }
  else if (channel == "Ch2."){
    files = {
      "run283_cosmic_run_trigger_on_C2_SoF1_60ADC",
      "run284_cosmic_run_trigger_on_C2_SoF1_600ADC",
    };
  }
  else if (channel == "Ch3."){
    files = {
      "run285_cosmic_run_trigger_on_C2_SoF2_60ADC",
      "run286_cosmic_run_trigger_on_C2_SoF2_600ADC",
    };
  }
  else if (channel == "Ch6."){
    files = {
      "run287_cosmic_run_trigger_on_C4_SoF1_60ADC",
      "run288_cosmic_run_trigger_on_C4_SoF1_600ADC",
    };
  }
  else if (channel == "Ch7."){
    files = {
      "run289_cosmic_run_trigger_on_C4_SoF2_35ADC",
      "run290_cosmic_run_trigger_on_C4_SoF2_70ADC",
    };
  }

}
