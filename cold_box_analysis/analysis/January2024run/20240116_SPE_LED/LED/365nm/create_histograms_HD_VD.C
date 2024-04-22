#define memorydepth 5000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void create_histograms_HD_VD(string channel = "Ch0.", vector<Int_t> _range_file = {0,-1}, Double_t lower_led_fit = 4.5, Double_t upper_led_fit = 11, Double_t upper_phe_fit = 1600, bool istest = true);

void run_HD(string channel = "Ch0.", vector<Int_t> _range_file = {0,-1}, Double_t lower_led_fit = 4.5, Double_t upper_led_fit = 11, Double_t upper_phe_fit = 1500, bool istest = true){
  create_histograms_HD_VD(channel, _range_file, lower_led_fit, upper_led_fit, upper_phe_fit, istest);
}

void run_VD(string channel = "Ch1.", vector<Int_t> _range_file = {0,-1}, Double_t lower_led_fit = 4.5, Double_t upper_led_fit = 5.8, Double_t upper_phe_fit = 1900, bool istest = true){
  create_histograms_HD_VD(channel, _range_file, lower_led_fit, upper_led_fit, upper_phe_fit, istest);
}


void create_histograms_HD_VD(string channel = "Ch0.", vector<Int_t> _range_file = {0,-1}, Double_t lower_led_fit = 4.5, Double_t upper_led_fit = 11, Double_t upper_phe_fit = 1600, bool istest = false){
  gStyle->SetCanvasPreferGL(kFALSE);
  bool just_a_test = istest;
  int ntests = 500;
  string mychannel = channel;
  Double_t minInt = 10380;
  Double_t maxInt = 11400;
  if(mychannel == "Ch0."){minInt = 3300; maxInt = 6000;}
  if(mychannel == "Ch1."){minInt = 3300; maxInt = 6000;}
  vector<string> devices = { "M2 HD", "M1 VD" };

  vector<Double_t> saturations = {16300, 16300};
  vector<Double_t> sphes = {7732, 5711};

  vector<string> files = { "run124_HD_and_VD_LED_20ns_3V20_hd_low_gain_no_transformer", "run125_HD_and_VD_LED_20ns_3V30_hd_low_gain_no_transformer", "run126_HD_and_VD_LED_20ns_3V40_hd_low_gain_no_transformer", "run127_HD_and_VD_LED_20ns_3V50_hd_low_gain_no_transformer", "run128_HD_and_VD_LED_20ns_3V60_hd_low_gain_no_transformer", "run129_HD_and_VD_LED_20ns_3V70_hd_low_gain_no_transformer", "run130_HD_and_VD_LED_20ns_3V80_hd_low_gain_no_transformer", "run131_HD_and_VD_LED_20ns_3V90_hd_low_gain_no_transformer", "run132_HD_and_VD_LED_20ns_4V00_hd_low_gain_no_transformer", "run133_HD_and_VD_LED_20ns_4V10_hd_low_gain_no_transformer", "run134_HD_and_VD_LED_20ns_4V20_hd_low_gain_no_transformer", "run135_HD_and_VD_LED_20ns_4V30_hd_low_gain_no_transformer", "run136_HD_and_VD_LED_20ns_4V40_hd_low_gain_no_transformer", "run137_HD_and_VD_LED_20ns_4V50_hd_low_gain_no_transformer", "run138_HD_and_VD_LED_20ns_4V60_hd_low_gain_no_transformer", "run139_HD_and_VD_LED_20ns_4V70_hd_low_gain_no_transformer", "run140_HD_and_VD_LED_20ns_4V80_hd_low_gain_no_transformer", "run141_HD_and_VD_LED_20ns_4V90_hd_low_gain_no_transformer", "run142_HD_and_VD_LED_20ns_5V00_hd_low_gain_no_transformer", "run143_HD_and_VD_LED_20ns_5V10_hd_low_gain_no_transformer", "run144_HD_and_VD_LED_20ns_5V20_hd_low_gain_no_transformer", "run145_HD_and_VD_LED_20ns_5V30_hd_low_gain_no_transformer", "run146_HD_and_VD_LED_20ns_5V40_hd_low_gain_no_transformer", "run147_HD_and_VD_LED_20ns_5V50_hd_low_gain_no_transformer", "run148_HD_and_VD_LED_20ns_5V60_hd_low_gain_no_transformer", "run149_HD_and_VD_LED_20ns_5V70_hd_low_gain_no_transformer", "run150_HD_and_VD_LED_20ns_5V80_hd_low_gain_no_transformer", "run151_HD_and_VD_LED_20ns_5V90_hd_low_gain_no_transformer", "run152_HD_and_VD_LED_20ns_6V00_hd_low_gain_no_transformer", "run153_HD_and_VD_LED_20ns_6V10_hd_low_gain_no_transformer", "run154_HD_and_VD_LED_20ns_6V20_hd_low_gain_no_transformer", "run155_HD_and_VD_LED_20ns_6V30_hd_low_gain_no_transformer", "run156_HD_and_VD_LED_20ns_6V40_hd_low_gain_no_transformer", "run157_HD_and_VD_LED_20ns_6V50_hd_low_gain_no_transformer"};
  vector<Double_t> volts = { 3.20, 3.30, 3.40, 3.50, 3.60, 3.70, 3.80, 3.90, 4.00, 4.10, 4.20, 4.30, 4.40, 4.50, 4.60, 4.70, 4.80, 4.90, 5.00, 5.10, 5.20, 5.30, 5.40, 5.50, 5.60, 5.70, 5.80, 5.90, 6.00, 6.10, 6.20, 6.30, 6.40, 6.50};
  Double_t saturation_level;

  //Int_t n = volts.size();
  if (_range_file[1] == -1) _range_file[1] = volts.size() - 1;
  Int_t n = _range_file[1] - _range_file[0] + 1;

  // vector<string> files(n);
  // for(Int_t i=0; i<n; i++){
  //   if(static_cast<Int_t>(volts[i])-volts[i]!=0) files[i] = Form("%.1fV",volts[i]);
  //   else files[i] = Form("%.0fV",volts[i]);
  // }
  vector<Double_t> ranges = {-4000,4000};

  Int_t nbins;

  string conc = "/analyzed.root";

  vector<ANALYZER*> z(n);

  vector<TH1D*> h(n);
  vector<TH1D*> hpeak(n);
  //TH2D *h2p = new TH2D("h2p","h2p",2000,-5,2300,2000,-100,16000);
  TH2D *h2p = new TH2D("h2p","h2p",750,0,100,750,0,500);
  TH2D *h2 = new TH2D("h2","h2",300,0,30,4000,-50,2000);
  TGraphErrors *gpeaks = new TGraphErrors();
  gStyle->SetPadTickX(0);
  gStyle->SetPadTickY(0);
  TCanvas *c1 = new TCanvas("c1","c1",1920,0,1920,1080); // charge
  TCanvas *cpeak = new TCanvas("cpeak","cpeak",1920,0,1920,1080); // peak
  vector<TF1*> fga(n);
  vector<TF1*> fgap(n);
  Double_t charge = 0;
  Double_t max = 0;
  Int_t m1=0,m2=0;
  m1=m2=static_cast<Int_t>(sqrt(n));
  // cout << m2 << endl;
  if(m1*m2==n){}
  else if((m2+1)*m1>=n) m2+=1;
  else if((m2+1)*(m1+1)>=n){
    m2+=1;
    m1+=1;
  }

  // cout << m1 << " " << m2 << endl;
  c1->Divide(m1,m2);
  c1->Update();

  cpeak->Divide(m1,m2);
  cpeak->Update();


  Double_t sphe = 1; // V.ns
  Double_t increase = 0;
  Int_t nbins_d = 100;
  Int_t gpts = 0;
  ANALYZER a;

  vector<Int_t> range_files(2);
  range_files = _range_file;

  Double_t N_c = 12; // Number of sigma_charge for set h xaxis range
  Double_t N_m = 20; // Number of sigma_max for set hpeak xaxis range

  Double_t s_charge = 0;
  Double_t sum_c = 0;
  Double_t y = 0;
  vector<Double_t> m_charge(n);
  vector<Double_t> sigma_charge(n);
  Double_t minRange_charge = 0;
  Double_t maxRange_charge = 0;

  Double_t s_max = 0;
  Double_t sum_m = 0;
  Double_t d = 0;
  vector<Double_t> m_max(n);
  vector<Double_t> sigma_max(n);
  Double_t minRange_max = 0;
  Double_t maxRange_max = 0;

  vector<Double_t> ratio10(n);
  vector<Double_t> ratio20(n);
  vector<Double_t> x_plot(n);

  Int_t auxi = 0;
  cout << n << " " << h.size() << endl;
  for(Int_t i = range_files[0]; i<=range_files[1]; i++){
    cout << "File: " << files[i] << endl;
    files[i] = files[i]+conc;
    z[auxi] = new ANALYZER(Form("z%.2f",volts[i]));
    z[auxi]->setAnalyzer(files[i].c_str());
    z[auxi]->setChannel(mychannel.c_str());
    Int_t kch = z[auxi]->kch;
    saturation_level = saturations[z[auxi]->getIdx()];
    sphe = sphes[z[auxi]->getIdx()];


    DENOISE dn;
    Int_t filter = 12;
    s_charge = 0;
    sum_c = 0;
    minRange_charge = 0;
    maxRange_charge = 0;
    s_max = 0;
    sum_m = 0;
    minRange_max = 0;
    maxRange_max = 0;
    vector<Double_t> wvf(z[auxi]->n_points);
    Int_t nentries = (just_a_test) ? ntests : z[auxi]->nentries;
    vector<Double_t> v_charge(nentries);
    vector<Double_t> v_max(nentries);
    for(Int_t j = 0; j<nentries; j++){
      z[auxi]->getWaveform(j,kch);
      charge = 0;
      if(z[auxi]->ch[kch]->selection!=2){

        z[auxi]->applyDenoise(filter);
        z[auxi]->integrate(minInt,maxInt);
        charge = z[auxi]->temp_charge;
        v_charge[j] = z[auxi]->temp_charge;
	s_charge += v_charge[j]/sphe;
        max = z[auxi]->temp_max;
        v_max[j] = z[auxi]->temp_max;
	s_max += v_max[j];

        h2p->Fill(charge/sphe,max);
        gpeaks->SetPoint(gpts,charge/sphe,max);
        gpeaks->SetPointError(gpts,sqrt(charge/sphe),sqrt(max));
        // cout << gpts << " " << gpeaks->GetN() << " " << h2p->GetEntries() << endl;
        gpts++;
        h2->Fill(volts[auxi],charge/sphe);
      }
    }

    m_charge[auxi] = s_charge/nentries;
    //cout << "Charge mean value: " << m_charge[auxi] << endl;

    m_max[auxi] = s_max/nentries;
    //cout << "Max mean value: " << m_max[auxi] << endl << endl;

   // for(Int_t j = 0; j<nentries; j++){
   //   y = 0;
   //   d = 0;
   //   y = pow(v_charge[j]/sphe-m_charge[auxi],2);
   //   sum_c += y;
   //   d = pow(v_max[j]-m_max[auxi],2);
   //   sum_m += d;
   // }
    
	
    // sigma_charge[auxi] = sqrt(sum_c/nentries);
    sigma_charge[auxi] = sqrt(abs(m_charge[auxi]));
    //cout << "Charge sigma value: " << sigma_charge[auxi] << endl;

    //sigma_max[auxi] = sqrt(sum_m/nentries);
    sigma_max[auxi] = sqrt(abs(m_max[auxi]));
    //cout << "Max sigma value: " << sigma_max[auxi] << endl << endl;

    minRange_charge = m_charge[auxi]-N_c*sigma_charge[auxi];
    maxRange_charge = m_charge[auxi]+N_c*sigma_charge[auxi];
    //cout << "Charge min-Range: " << minRange_charge << " ~ Charge max-Range: "  << maxRange_charge  << endl << endl;

    minRange_max = m_max[auxi]-N_m*sigma_max[auxi];
    maxRange_max = m_max[auxi]+N_m*sigma_max[auxi];
    //cout << "Max min-Range: " << minRange_max << " ~ Max max-Range: "  << maxRange_max  << endl << endl;

    h[auxi] = new TH1D(Form("h%d",auxi),Form("h%d",auxi),100,minRange_charge,maxRange_charge);
    h[auxi]->GetXaxis()->SetLabelSize(0.08);
    hpeak[auxi] = new TH1D(Form("hpeak%d",auxi),Form("hpeak%d",auxi),100,minRange_max,maxRange_max);
    hpeak[auxi]->GetXaxis()->SetLabelSize(0.08);

    for(Int_t j = 0; j<nentries; j++){
	h[auxi]->Fill(v_charge[j]/sphe);		
        hpeak[auxi]->Fill(v_max[j]);
    }

    c1->cd(auxi+1);
    h[auxi]->Draw();
    string hist_name = Form("%s - LED %.1f V",devices[z[auxi]->getIdx()].c_str(),volts[i]);
    h[auxi]->SetNameTitle(hist_name.c_str(),hist_name.c_str());
    h[auxi]->GetYaxis()->SetTitle("# of events");
    h[auxi]->GetXaxis()->SetTitle("Photo-electrons");
    fga[auxi] = new TF1(Form("f%d",auxi),"gaus(0)",-1,1);
    fga[auxi]->SetParameters(0.1,0.1,0.1);
    // h[i]->Fit(fga[i],"QW");
    // fga[i]->Draw("SAME");
    
    //Double_t counter2 = 0;
    //Double_t counter1 = 0;
    //Double_t counter0 = 0;

    //for(Int_t j = 0; j<100; j++){
    //	Double_t binContent = h[auxi]->GetBinContent(j);
    //    if(binContent == 2){
    //    	counter2++;
    //    }else if(binContent == 1){
    //    	counter1++;
    //    }else if(binContent == 0){
    //    	counter0++;
    //    }
    //}
    //x_plot[auxi] = i;
    //ratio10[auxi] = counter1/counter0;
    //cout << "Ratio 1 entry/0 entry: " << ratio10[auxi] << endl;
    //ratio20[auxi] = counter2/counter0;
    //cout << "Ratio 2 entry/0 entry: " << ratio20[auxi] << endl;

    cpeak->cd(auxi+1);
    hpeak[auxi]->Draw();
    string histp_name = Form("%s peak - LED %.1f V",devices[z[auxi]->getIdx()].c_str(),volts[i]);
    hpeak[auxi]->SetNameTitle(histp_name.c_str(),hist_name.c_str());
    hpeak[auxi]->GetYaxis()->SetTitle("# of events");
    hpeak[auxi]->GetXaxis()->SetTitle("Amplitude (ADC Channels)");
    fgap[auxi] = new TF1(Form("fp%d",auxi),"gaus(0)",-1,1);
    fgap[auxi]->SetParameters(0.1,0.1,0.1);
    // hpeak[i]->Fit(fgap[i],"QW");
    // fgap[i]->Draw("SAME");
    
    auxi += 1;
  }

  //auxi = 0;
  //TCanvas *cr1 = new TCanvas("cr1","cr1");
  //TGraph *gr = new TGraph(x_plot.size(), &x_plot[0], &ratio10[0]);
  //cr1->cd();
  //gr->Draw("AP*");

   vector<Double_t> peak(n);
   vector<Double_t> sigma_peak(n);
   vector<Double_t> Erpeak(n);
   
   vector<Double_t> avgs(n);
   vector<Double_t> sigma_mu(n);
   vector<Double_t> Ersigma_mu(n);
   vector<Double_t> Eravgs(n);
   vector<Double_t> ErSTD(n);
   for(Int_t i = 0; i<n; i++){
  // for(Int_t i = range_files[0]; i<=range_files[1]; i++){
     avgs[i] = h[i]->GetMean();
     ErSTD[i] = h[i]->GetStdDev();
     Double_t aux_arror_avgs = h[i]->GetMeanError();
     Double_t aux_arror_std = h[i]->GetStdDevError();
 
      
     // peak[i] = fgap[i]->GetParameter(1);
     // sigma_peak[i] = fgap[i]->GetParameter(2);
     // Double_t aux_arror_peak = fgap[i]->GetParError(1);
     // Double_t aux_arror_std_peak = fgap[i]->GetParError(2);
     // if(i==3 || i==4){
       peak[i] = hpeak[i]->GetMean();
       sigma_peak[i] = hpeak[i]->GetStdDev();
       Double_t aux_arror_peak = hpeak[i]->GetMeanError();
       Double_t aux_arror_std_peak = hpeak[i]->GetStdDevError();
 
     // }
    
     // avgs[i] = fga[i]->GetParameter(1);
     // ErSTD[i] = fga[i]->GetParameter(2);
     // Double_t aux_arror_avgs = fga[i]->GetParError(1);
     // Double_t aux_arror_std = fga[i]->GetParError(2);
     
     
     // Eravgs[i] = sqrt(pow(aux_arror_avgs,2)+pow(ErSTD[i],2)/* /h[i]->GetEntries() */);
     Eravgs[i] = ErSTD[i];
     // Eravgs[i] = ErSTD[i]/sqrt(avgs[i]);
     // Erpeak[i] = sqrt(pow(aux_arror_peak,2)+pow(sigma_peak[i],2)/hpeak[i]->GetEntries());
     Erpeak[i] = sigma_peak[i];
 
     sigma_mu[i] = 100*ErSTD[i]/avgs[i];
     Ersigma_mu[i] = sigma_mu[i]*sqrt(pow(aux_arror_std/ErSTD[i],2)+pow(aux_arror_avgs/avgs[i],2));
   }
   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);
   TCanvas *c2 = new TCanvas("c2","c2",1920,0,1920,1080); // LED voltage VS Average phe
   c2->SetGridx();
   c2->SetGridy();
 
   TGraphErrors *gmeans = new TGraphErrors(n,&volts[0],&avgs[0],0,&Eravgs[0]);
 //   TGraphErrors *gSTDs = new TGraphErrors(n,&volts[0],&avgs[0],0,&ErSTD[0]);
   gmeans->GetXaxis()->SetTitle("LED Voltage (V)");
   gmeans->GetYaxis()->SetTitle("Average photo-electrons");
   gmeans->SetMarkerStyle(21);
   gmeans->SetMarkerSize(0.5);
 //   gSTDs->SetMarkerSize(0);
 //   gSTDs->SetLineWidth(2);
 //   gSTDs->Draw("AP []");
   TF1 *fpol1 = new TF1("fpol1","pol1",lower_led_fit,upper_led_fit);
   fpol1->SetParameters(10,10);
   gmeans->Draw("AP");
   gmeans->Fit("fpol1","R");
   gStyle->SetOptFit(111);
  
   TCanvas *c3 = new TCanvas("c3","c3",1920,0,1920,1080); // mu VS sigma/mu
   c3->SetGridx();
   c3->SetGridy();
   TGraphErrors *gstdmu = new TGraphErrors(n,&avgs[0],&sigma_mu[0],&Eravgs[0],&Ersigma_mu[0]);
   TF1 *myfunc = new TF1("myfunc","sqrt([0]*[0]+[1]*[1]/x+pow([2]/x,2))",0,200000);
   myfunc->SetParameters(1,2,3);
   myfunc->SetParLimits(0,0,1e3);
   myfunc->SetParLimits(1,0,1e3);
   myfunc->SetParLimits(2,0,1e3);
   gstdmu->GetXaxis()->SetRangeUser(0.001,200000);
   gstdmu->GetYaxis()->SetRangeUser(0,600);
   gstdmu->GetXaxis()->SetTitle("#mu (photo-electrons)");
   gstdmu->GetYaxis()->SetTitle("#sigma/#mu (%)");
   gstdmu->SetMarkerStyle(21);
   gstdmu->SetMarkerSize(0.5);
   gstdmu->Draw("AP");
   gstdmu->Fit("myfunc","R");
  
   TCanvas *c4 = new TCanvas("c4","c4",1920,0,1920,1080); // mu VS Average amplitude
   c4->SetGridx();
   c4->SetGridy(); 
   TGraphErrors *gpeakph = new TGraphErrors(n,&avgs[0],&peak[0],&Eravgs[0],&Erpeak[0]);
   TF1 *myfunc2 = new TF1("myfunc2","pol1",300,upper_phe_fit);
   myfunc2->SetParameters(0.001,0.003);
   // gpeakph->GetXaxis()->SetRangeUser(0.001,200000);
   // gpeakph->GetYaxis()->SetRangeUser(0,600);
   gpeakph->GetXaxis()->SetTitle("#mu (photo-electrons)");
   gpeakph->GetYaxis()->SetTitle("Average amplitude (ADC Channels)");
   gpeakph->SetMarkerStyle(21);
   gpeakph->SetMarkerSize(0.5);
   gpeakph->Draw("AP");
   gpeakph->GetXaxis()->SetLimits(0.01,3000);
   gpeakph->Fit("myfunc2","R");
   gStyle->SetOptFit(111);
   // myfunc2->SetRange(0,250);
   // myfunc2->Draw("SAME");
  
   TCanvas *c2p = new TCanvas("c2p","c2p",1920,0,1920,1080); // mu VS Amplitude
   h2p->GetXaxis()->SetTitle("#mu (photo-electrons)");
   h2p->GetYaxis()->SetTitle("Amplitude (ADC Channels)");
   h2p->Draw("colz");
   
   TCanvas *c2c = new TCanvas("c2c","c2c",1920,0,1920,1080); // LED voltage VS Phe
   h2->GetXaxis()->SetTitle("LED voltage (V)");
   h2->GetYaxis()->SetTitle("Photo-electrons");
   h2->Draw("colz");
   
   c2p->cd();
   c2p->SetGridx();
   c2p->SetGridy();
   gpeaks->Draw("L SAME");
   gpeaks->SetLineWidth(0);
   TF1 *myfunc3 = new TF1("myfunc3","pol1",300,upper_phe_fit);
   gpeaks->Fit("myfunc3","R");
   gStyle->SetOptFit(111);

}
