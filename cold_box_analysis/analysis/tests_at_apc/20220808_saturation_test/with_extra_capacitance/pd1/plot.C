#define memorydepth 20002
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"

void plot(){
  TFile *f = new TFile("analyzed.root","READ");
  TTree *t1 = (TTree*)f->Get("t1");

  ADC_DATA ch;
  TBranch *bch = t1->GetBranch("Ch1");
  bch->SetAddress(&ch);
  Int_t n = t1->GetEntries();
  vector<Double_t> y(memorydepth);
  vector<Double_t> x(memorydepth);

  vector<TGraph*> g(n);
  TMultiGraph *gm = new TMultiGraph("pd1","pd1");
  TLegend *lg = new TLegend();
  lg->SetHeader("Modified capacitance");
  for (Int_t i = 0; i < n; i++) {
    bch->GetEvent(i);
   for (Int_t j = 0; j < memorydepth; j++) {
     y[j] = ch.wvf[j];
     x[j] = j*5e-2;
   }
   g[i] = new TGraph(memorydepth,&x[0],&y[0]);
   Int_t input = 13 + 2*i;
   char *gname = Form("Input %d mV",input);
   g[i]->SetNameTitle(gname,gname);
   gm->Add(g[i],"L");
   lg->AddEntry(g[i],gname,"l");

  }
  TCanvas *c1 = new TCanvas();
  gm->GetXaxis()->SetTitle("Time (ns)");
  gm->GetYaxis()->SetTitle("Amplitude (mV)");
  gm->Draw("A plc");
  lg->Draw();
  // c1->BuildLegend();


}
