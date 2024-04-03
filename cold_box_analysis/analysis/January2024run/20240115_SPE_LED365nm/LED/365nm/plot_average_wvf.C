#define memorydepth 5000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"

void plot_average_wvf(string channel = "Ch0.", vector<Int_t> _range_file = {0,18}){
  gStyle->SetCanvasPreferGL(kFALSE);
  bool just_a_test = false;
  int totalevents = 500;
  string mychannel = channel;
  vector<string> devices = { "C1 SoF 1", "C1 SoF 2", "C2 SoF 1", "C2 SoF 2", "C3 SoF 1", "C3 SoF 2", "C4 SoF 1", "C4 SoF 2"};

  vector<Double_t> saturations = { 16300, 16300, 16300, 16300, 16300, 16300, 16300, 16300};
  // vector<Double_t> sphes = {7100,400, 7100, 5800*9.97/515.25, 5800, 0, 0, 0};
  vector<Double_t> sphes = {6178, 5306, 3357, 2609, 1179, 1537, 3417, 1731};

  vector<string> files = {"run24_all_pof_on_LED_20ns_3V20", "run25_all_pof_on_LED_20ns_3V25", "run26_all_pof_on_LED_20ns_3V30", "run27_all_pof_on_LED_20ns_3V35", "run28_all_pof_on_LED_20ns_3V40", "run29_all_pof_on_LED_20ns_3V45", "run30_all_pof_on_LED_20ns_3V50", "run31_all_pof_on_LED_20ns_3V55", "run32_all_pof_on_LED_20ns_3V60", "run33_all_pof_on_LED_20ns_3V65", "run34_all_pof_on_LED_20ns_3V70", "run35_all_pof_on_LED_20ns_3V80", "run36_all_pof_on_LED_20ns_3V90", "run37_all_pof_on_LED_20ns_4V00", "run38_all_pof_on_LED_20ns_4V10", "run39_all_pof_on_LED_20ns_4V20", "run40_all_pof_on_LED_20ns_4V30", "run41_all_pof_on_LED_20ns_4V40", "run42_all_pof_on_LED_20ns_4V50", "run43_all_pof_on_LED_20ns_4V60", "run44_all_pof_on_LED_20ns_4V70", "run45_all_pof_on_LED_20ns_4V80", "run46_all_pof_on_LED_20ns_4V90", "run47_all_pof_on_LED_20ns_5V00", "run48_all_pof_on_LED_20ns_5V10", "run49_all_pof_on_LED_20ns_5V20", "run50_all_pof_on_LED_20ns_5V30", "run51_all_pof_on_LED_20ns_5V40", "run52_all_pof_on_LED_20ns_5V50", "run53_all_pof_on_LED_20ns_5V60", "run54_all_pof_on_LED_20ns_5V70", "run56_all_pof_on_LED_20ns_5V90", "run57_all_pof_on_LED_20ns_6V00", "run58_all_pof_on_LED_20ns_6V20", "run59_all_pof_on_LED_20ns_6V40", "run60_all_pof_on_LED_20ns_6V60", "run61_all_pof_on_LED_20ns_6V80", "run62_all_pof_on_LED_20ns_7V00", "run63_all_pof_on_LED_20ns_7V20", "run64_all_pof_on_LED_20ns_7V40", "run65_all_pof_on_LED_20ns_7V60", "run66_all_pof_on_LED_20ns_7V80", "run67_all_pof_on_LED_20ns_8V00", "run68_all_pof_on_LED_20ns_8V20", "run69_all_pof_on_LED_20ns_8V40", "run70_all_pof_on_LED_20ns_8V60", "run71_all_pof_on_LED_20ns_8V80", "run72_all_pof_on_LED_20ns_9V00", "run73_all_pof_on_LED_20ns_9V20", "run74_all_pof_on_LED_20ns_9V40", "run75_all_pof_on_LED_20ns_9V60", "run76_all_pof_on_LED_20ns_9V80", "run77_all_pof_on_LED_20ns_10V0", "run78_all_pof_on_LED_20ns_10V2", "run79_all_pof_on_LED_20ns_10V4", "run80_all_pof_on_LED_20ns_10V6", "run81_all_pof_on_LED_20ns_10V8", "run82_all_pof_on_LED_20ns_11V0", "run83_all_pof_on_LED_20ns_11V5", "run84_all_pof_on_LED_20ns_12V0"};
  vector<Double_t> volts = {3.20, 3.25, 3.30, 3.35, 3.40, 3.45, 3.50, 3.55, 3.60, 3.65, 3.70, 3.80, 3.90, 4.00, 4.10, 4.20, 4.30, 4.40, 4.50, 4.60, 4.70, 4.80, 4.90, 5.00, 5.10, 5.20, 5.30, 5.40, 5.50, 5.60, 5.70, 5.90, 6.00, 6.20, 6.40, 6.60, 6.80, 7.00, 7.20, 7.40, 7.60, 7.80, 8.00, 8.20, 8.40, 8.60, 8.80, 9.00, 9.20, 9.40, 9.60, 9.80, 10.0, 10.2, 10.4, 10.6, 10.8, 11.0, 11.5, 12.0};
  Double_t saturation_level;

  Int_t n = volts.size();
  vector<Int_t> range_files(2);
  range_files = _range_file;



  Int_t nbins;
  string conc = "/analyzed.root"; vector<ANALYZER*> z(n);

  vector<TH1D*> h(n);
  vector<TH1D*> hrise(n);
  vector<TH1D*> hfall(n);
  vector<TH1D*> hunder(n);
  THStack *hs = new THStack("hs", "hs");
  THStack *hsrise = new THStack("hsrise", "hsrise");
  THStack *hsfall = new THStack("hsfall", "hsfall");
  THStack *hsunder = new THStack("hsunder", "hsunder");
  TCanvas *c1 = new TCanvas("c1","c1",1920,0,1920,1080);
  c1->DrawFrame(0,-.1,10000,1);

  Double_t sphe = 1;
  TLegend *lg = new TLegend(0.8,0.8,1,1);




  for(Int_t i = range_files[0]; i<range_files[1]; i++){
    cout << "File: " << files[i] << endl;
    files[i] = files[i]+conc;
    z[i] = new ANALYZER(Form("z%.2f",volts[i]));
    z[i]->setAnalyzer(files[i].c_str());
    z[i]->setChannel(mychannel.c_str());
    Int_t kch = z[i]->kch;
    saturation_level = saturations[z[i]->getIdx()];
    sphe = sphes[z[i]->getIdx()];
    if (!just_a_test){
      totalevents = z[i]->nentries;
    }

    z[i]->averageWaveform(totalevents);
    h[i] = (TH1D*)z[i]->h->Clone(Form("h%d",i));
    hrise[i] = new TH1D(Form("hrise_%d", i), Form("hrise_%.2f", volts[i]), 500, 0, 500);
    hfall[i] = new TH1D(Form("hfall_%d", i), Form("hfall_%.2f", volts[i]), 6000, 0, 6000);
    hunder[i] = new TH1D(Form("hunder_%d", i), Form("hunder_%.2f", volts[i]), 400, -1, 1);

    Double_t filterlevel = 12;
    if (h[i]->GetMean()>20){
      filterlevel = 0;
    }
    for (Int_t j = 0; j < totalevents; j++){
        z[i]->getWaveform(j, kch);
        z[i]->applyDenoise(filterlevel);
        Double_t valrise = z[i]->rise_time(kch, {0,3000}, true, {3300,4000});
        Double_t valmax = z[i]->temp_max - z[i]->temp_dummy;
        Double_t valfall = z[i]->fall_time(kch, {0,3000}, true, {3300,10000});
        Double_t valmin = z[i]->getMinimum(3500, 10000);
        hrise[i]->Fill(valrise); 
        hfall[i]->Fill(valfall);
        hunder[i]->Fill(valmin/valmax);
    }

    Double_t max = h[i]->GetMaximum();
    h[i]->Scale(1./max);
    stringstream stream;
    stream << std::fixed << std::setprecision(1) << max;
    string correct_max = stream.str();
    string hist_name = Form("%s - LED %.2f V - Max = %s",devices[z[i]->getIdx()].c_str(),volts[i], correct_max.c_str());
    // string hist_name = Form("%s - LED %.1f V - Max = %s",devices[z[i]->getIdx()].c_str(),volts[i], "nope");
    h[i]->SetTitle(hist_name.c_str());
    h[i]->SetLineWidth(2);
    hs->Add(h[i],"hist");
    lg->AddEntry(h[i], hist_name.c_str(),"l");
    hs->GetXaxis()->SetTitle("Time (ns)");
    hs->GetYaxis()->SetTitle("Amplitude (A.U.)");

    hsrise->Add(hrise[i]);
    hsfall->Add(hfall[i]);
    hsunder->Add(hunder[i]);
    hrise[i]->SetLineWidth(2);
    hfall[i]->SetLineWidth(2);
    hunder[i]->SetLineWidth(2);
    hsrise->GetXaxis()->SetTitle("Rise time (ns)");
    hsrise->GetYaxis()->SetTitle("# of events");

    hsfall->GetXaxis()->SetTitle("Fall time (ns)");
    hsfall->GetYaxis()->SetTitle("# of events");

    hsunder->GetXaxis()->SetTitle("Minimum (%)");
    hsunder->GetYaxis()->SetTitle("# of events");
  }

  hs->Draw("nostack plc");
  lg->Draw();

  TCanvas *crise = new TCanvas("crise","crise",1920,0,1920,1080);
  hsrise->Draw("nostack plc");
  lg->Draw();

  TCanvas *cfall = new TCanvas("cfall","cfall",1920,0,1920,1080);
  hsfall->Draw("nostack plc");
  lg->Draw();

  TCanvas *cunder = new TCanvas("cunder","cunder",1920,0,1920,1080);
  hsunder->Draw("nostack plc");
  lg->Draw();
}
