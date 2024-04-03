#define memorydepth 5000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"

void plot_average_wvf(string channel = "Ch0.", vector<Int_t> _range_file = {0,18}){
  gStyle->SetCanvasPreferGL(kFALSE);
  bool just_a_test = false;
  string mychannel = channel;
  vector<string> devices = { "C1 SoF 1", "C1 SoF 2", "C2 SoF 1", "C2 SoF 2", "C3 SoF 1", "C3 SoF 2", "C4 SoF 1", "C4 SoF 2"};

  vector<Double_t> saturations = { 16300, 16300, 16300, 16300, 16300, 16300, 16300, 16300};
  // vector<Double_t> sphes = {7100,400, 7100, 5800*9.97/515.25, 5800, 0, 0, 0};
  vector<Double_t> sphes = {7100,400, 7100, 1, 5800, 0, 0, 0};

  vector<string> files = {
"run105_all_pof_on_LED_20ns_3V30", "run106_all_pof_on_LED_20ns_3V40", "run107_all_pof_on_LED_20ns_3V60", "run108_all_pof_on_LED_20ns_3V80", "run109_all_pof_on_LED_20ns_4V00", "run110_all_pof_on_LED_20ns_4V20", "run111_all_pof_on_LED_20ns_4V60", "run112_all_pof_on_LED_20ns_5V00", "run113_all_pof_on_LED_20ns_5V50", "run114_all_pof_on_LED_20ns_6V00", "run115_all_pof_on_LED_20ns_6V60", "run116_all_pof_on_LED_20ns_7V20", "run117_all_pof_on_LED_20ns_7V80", "run118_all_pof_on_LED_20ns_8V20", "run119_all_pof_on_LED_20ns_8V80", "run120_all_pof_on_LED_20ns_9V40", "run121_all_pof_on_LED_20ns_10V0", "run122_all_pof_on_LED_20ns_11V0"};
  vector<Double_t> volts = { 3.30, 3.40, 3.60, 3.80, 4.00, 4.20, 4.60, 5.00, 5.50, 6.00, 6.60, 7.20, 7.80, 8.20, 8.80, 9.40, 10.0, 11.0 };
  Double_t saturation_level;

  Int_t n = volts.size();
  vector<Int_t> range_files(2);
  range_files = _range_file;



  Int_t nbins;
  string conc = "/analyzed.root"; vector<ANALYZER*> z(n);

  vector<TH1D*> h(n);
  THStack *hs = new THStack("hs", "hs");
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

    z[i]->averageWaveform(500);
    h[i] = (TH1D*)z[i]->h->Clone(Form("h%d",i));
    Double_t max = h[i]->GetMaximum();
    h[i]->Scale(1./max);
    stringstream stream;
    stream << std::fixed << std::setprecision(1) << max;
    string correct_max = stream.str();
    string hist_name = Form("%s - LED %.1f V - Max = %s",devices[z[i]->getIdx()].c_str(),volts[i], correct_max.c_str());
    // string hist_name = Form("%s - LED %.1f V - Max = %s",devices[z[i]->getIdx()].c_str(),volts[i], "nope");
    h[i]->SetTitle(hist_name.c_str());
    h[i]->SetLineWidth(2);
    hs->Add(h[i],"hist");
    lg->AddEntry(h[i], hist_name.c_str(),"l");
    hs->GetXaxis()->SetTitle("Time (ns)");
    hs->GetYaxis()->SetTitle("Amplitude (A.U.)");
  }

  hs->Draw("nostack plc same");
  lg->Draw();

}
