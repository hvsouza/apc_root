#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"
#include "/home/henrique/Dropbox/APC_Paris/Root/ProtoDUNE_HD_ANA/waveselector.C"
#include <cstdio>

ANALYZER *z = nullptr;

Double_t lowLim = 800;
// Double_t highLim = 7400;
Double_t highLim = 16400;

Double_t start_point = 0;
// Double_t start_point = 2550;

Int_t npoints;
Double_t dtime;

Double_t tolerance = 0.01;
Double_t constant = 0;
Int_t manytimes = 0;
const Int_t nparameters = 4;
Double_t realchi2 = 0;
Int_t realNDF = 0;
TF1* fLAr;
TF1* func_fit2;
TF1* fpol1;
TF1* fpol2;


vector<Double_t> x;
vector<Double_t> yraw;
vector<Double_t> ysphe;
vector<Double_t> yconv;

vector<Double_t> y;


Double_t factor = 0;
Double_t smoothness = 10;
Double_t offset = 0;



void convoluted(vector<Double_t> &signal, TF1 *f1, vector<Double_t> f2){ // f1*f2 = int f1(x)*f2(t-x) dx
  Double_t value = 0;
  Int_t mycounter = 0;
  for(Int_t i = 0; i<signal.size(); i++){
    value = 0;
//     f2->SetParameter(9,i*4);
    if(i>=0/dtime && i<npoints){
      for(Int_t j = 0; j<signal.size(); j++){
        if( i - j < 0) continue; // for gain time, otherwise value+=0 or loop back
        else value += f1->Eval(j*dtime)*f2[i-j];

        //       if(j>5000/4) value += f1->Eval(j*4)*f2->Eval(j*4);
        //       else value+=0;

        //       cout << i << " " << j << " " << f1->Eval(j*4) << " " << f2->Eval(j*4) << " " << value << endl;
      }
      signal[i] = value;
      //     if( 750-(i+1) < 0) mycounter = npoints + (750-(i+1));
      //     else mycounter = 750-(i+1);
      //     signal[i] = f2[mycounter];

    }
    else{
      signal[i] = 0;
    }
  }
}

template <class T>
void shift_waveform(T *h, Int_t new_max, Bool_t rawShift = false){
  Int_t npts = h->GetNbinsX();
  Int_t old_max = h->GetMaximumBin();
  if(rawShift) old_max = 0;
  Int_t old_ref = old_max - new_max;
  TH1D *htemp = (TH1D*)h->Clone("htemp");
  Double_t temp;
  if(old_ref<0){
    // cout << " case lower" << endl;
    old_ref = npts-(new_max-old_max);
  }
  for(Int_t i = 1; i<npts-(old_ref); i++){
    temp = htemp->GetBinContent(old_ref+i);
    h->SetBinContent(i,temp);
  }
  Int_t aux = 1;
  for(Int_t i = npts-(old_ref); i<=npts; i++){
    temp = htemp->GetBinContent(aux);
    h->SetBinContent(i,temp);
    aux++;
  }
  delete htemp;
}


void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
  //calculate chisquare
  Double_t chisq = 0;
  Double_t delta =0;
  for(Int_t i = 0; i<nparameters; i++){
    fLAr->SetParameter(i,par[i]);
  }
  // create_sphe(par[4],par[5]);
  /* create_sphe(0,0); */
  convoluted(y,fLAr,ysphe);
  realNDF = 0;
  realchi2 = 0;
  for(Int_t i=0; i<yraw.size(); i++) {
    if(i>=lowLim/dtime && i<=highLim/dtime){
      delta  = (yraw.at(i)-y.at(i));
      //             cout << i << " " << z[i] << " " << y[i] << endl;
      chisq += delta*delta/(5*5);
      // if(abs(yraw[i])>=1){
        // realchi2+=delta*delta/abs(yraw[i]);
        realchi2+= delta*delta/(10*10);
        realNDF++;
      // }
    }
  }
  f = chisq;
  manytimes++;

  cout << manytimes << ", " << f;

  for(Int_t i = 0; i < nparameters; i++){
    cout << ", " << par[i];
  }
  cout << "\n";
}


TH1D *hsphe;
void convolution_fit(Int_t ch = 13, Bool_t checkSelection = true, Int_t offset = 68, string spe_file_name = "", bool isSPE = false){
  Double_t reference_amplitude = 6;
  vector<Double_t> range_amplitude = {200,300};
  Int_t stop_at_event = -1;

  z = new ANALYZER("z");
  z->setAnalyzer("./analyzed.root");
  bool chok = false;
  chok = z->setChannel(Form("Ch%d.", ch));
  if (!chok) return;
  Int_t kch = z->kch;
  npoints = z->n_points;
  dtime = z->dtime;

  x.resize(npoints);
  yraw.resize(npoints);
  ysphe.resize(npoints);
  yconv.resize(npoints);
  y.resize(npoints);

  Double_t *wvf = nullptr;
  vector<Double_t> avg_wvf(npoints, 0);
  vector<Double_t> mycuts(5);


  waveselector(ch, spe_file_name, isSPE);
  if (checkSelection) return;

  // return;
   
  hsphe = new TH1D("hsphe","hsphe", npoints, 0, npoints*dtime);
  for(Int_t i = 0; i < npoints; i++){
    hsphe->SetBinContent(i+1, zpe->ch[zpe->kch]->wvf[i]); // factor so fit parameters are not so small
  }
  Double_t starting_max = hsphe->GetMaximumBin();
  shift_waveform(hsphe, starting_max-offset);


  Int_t nav = 0;
  Int_t nentries = z->nentries;
  TH2D *h2avg = new TH2D("h2avg", "h2avg", npoints, 0, npoints*dtime, 2000,-4000,8000);
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

    for(Int_t j = 0; j < npoints; j++){

      // avg_wvf[j] +=  wvf[j];
      avg_wvf[j] +=  wvf[j];
      h2avg->Fill(j*dtime, wvf[j]);
    }
    nav++;
  }

  Double_t *time = new Double_t[npoints];
  Double_t max = -1e12;
  Double_t maxpos = 0;
  for(Int_t j = 0; j < npoints; j++){
    avg_wvf[j] = avg_wvf[j]/nav;
    time[j] = j*z->dtime;
    if(avg_wvf[j] > max){
      max = avg_wvf[j];
      maxpos = j;
    }
  }
  cout << "\n";
  cout << "Found " << nav << " waveforms" << endl;
  TCanvas *cavg = new TCanvas("cavg", "cavg",1920,0,1920,1080);
  h2avg->Draw("colz");
  TGraph *gwvf = new TGraph(npoints, time, &avg_wvf[0]);
  gwvf->Draw("SAME");
  h2avg->GetXaxis()->SetTitle("Time (ns)");
  h2avg->GetYaxis()->SetTitle("Amplitude (A.U.)");





  // gsphe->Fit("func_fit2","R0");
  // fLAr = new TF1("fLAr","(x<([5]-300)) ? 0 : ([0]*exp(-(x-[5])/[1])+[2]*exp(-(x-[5])/[3])+[4])*((1+TMath::Sign(1,x-[5]))/2)",0,npoints*dtime);
  fLAr = new TF1("fLAr","(x<([4]-300)) ? 0 : [0]*([1]*exp(-(x-[4])/[2])/[2]+(1-[1])*exp(-(x-[4])/[3])/[3])*((1+TMath::Sign(1,x-[4]))/2)",0,npoints*dtime);
  // fLAr->SetParameters(0.0271741,37.4799,0.15825,1468.59,0,0);
  fLAr->SetParameters(1e3, 0.3, 30, 1400, 0);
  fLAr->SetParName(0,"A");
  fLAr->SetParName(1,"A_{fast}");
  fLAr->SetParName(2,"#tau_{fast}");
  fLAr->SetParName(3,"#tau_{slow}");
  fLAr->SetNpx(5000);

  for(Int_t i = 0; i<npoints; i++){
    yraw[i] = *(gwvf->GetY()+i);
    x[i] = i*dtime;
    ysphe[i] = hsphe->GetBinContent(i+1);
  }



  TMinuit *gMinuit = new TMinuit(nparameters);  //initialize TMinuit with a maximum of 3 params
  gMinuit->SetFCN(fcn); // setting fcn

  Double_t arglist[10];
  Int_t ierflg = 0;

  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);


  // Set starting values and step sizes for parameters
  static Double_t vstart[nparameters] = { 1e3,  //A
                                          0.3,   //tau_fast
                                          30,     //B
                                          1400, //tau_slow
                                          // 2271, // recovery
                                          // 0 //-1
  }; // smooth
  static Double_t step[nparameters] =   { 100,
                                          0.1,
                                          5,
                                          200,
                                          // 100, // recovery
  }; // smooth

    gMinuit->mnparm(0, "A",         vstart[0], step[0], 100, 5e3,ierflg);
    gMinuit->mnparm(1, "fp",  vstart[1], step[1], 0,1,ierflg);
    gMinuit->mnparm(2, "tau_fast",         vstart[2], step[2], 5,50,ierflg);
    gMinuit->mnparm(3, "tau_slow",  vstart[3], step[3],500,3000,ierflg);
    // gMinuit->mnparm(4, "undershoot",  vstart[4], step[4],-10,-0.01,ierflg);
    // gMinuit->mnparm(5, "smooth",  vstart[5], step[5], 900,1300,ierflg);


    arglist[0] = 1;
    gMinuit->mnexcm("SET PRI", arglist ,1,ierflg);

    // Now ready for minimization step
    arglist[0] = 10000;
    arglist[1] = tolerance;
    gMinuit->mnexcm("MIGRAD", arglist ,2,ierflg);


    // Print results
    /* Double_t amin,edm,errdef; */
    /* Int_t nvpar,nparx,icstat; */
    /* gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat); */
    //     gMinuit->mnprin(3,amin);

    Double_t param[nparameters];
    Double_t Erparam[nparameters];

    for(Int_t i = 0; i < nparameters; i++){
      gMinuit->GetParameter(i,param[i],Erparam[i]);
      if(i<nparameters) fLAr->SetParameter(i,param[i]);
      if(i<nparameters) fLAr->SetParError(i,Erparam[i]);
//       out << param[i] << " +/- " << Erparam[i] << endl;
    } // create_sphe(param[4],param[5]); create_sphe(0,0);
    vector<Double_t> conv_signal(npoints);;

    convoluted(conv_signal,fLAr,ysphe);

    TMultiGraph *gm = new TMultiGraph();
    TGraph *gconv = new TGraph(x.size(),&x[0],&conv_signal[0]);
    gconv->SetLineWidth(2);
    gconv->SetLineColor(kRed);

    gwvf->SetLineWidth(2);

    TCanvas *c = new TCanvas("c", "c",1920,0,1920,1080);
    c->cd(1);
    gm->Add(gconv,"L");
    gm->Add(gwvf),"L";

    gm->Draw("A");
    gm->GetXaxis()->SetTitle("Time (ns)");
    gm->GetYaxis()->SetTitle("Amplitude (ADC channels)");
    gm->GetXaxis()->SetRangeUser(0,npoints*dtime);

    Double_t fast_integral =  fLAr->GetParameter(0)*fLAr->GetParameter(1);
    Double_t slow_integral =  fLAr->GetParameter(2)*fLAr->GetParameter(3);
    Double_t total_integral =  fast_integral+slow_integral; //fLAr->Integral(0,20000);
    cout << "\n\nReal chi2 = " << realchi2 << endl;
    cout << "Real NDF = " << realNDF << endl;

    TLegend *pleg = new TLegend(0.54,0.59,0.89,0.89, "");
    pleg->AddEntry(gwvf, "Averaged waveform", "l");
    pleg->AddEntry(gconv, "Fit", "l");
    pleg->AddEntry((TObject*)0, Form("#chi^{2} / ndf: %.2f/%d", realchi2, realNDF), "");
    // pleg->AddEntry((TObject*)0, Form("%s: %.2f #pm %.2f",fLAr->GetParName(0), fLAr->GetParameter(0), fLAr->GetParError(0)), "");
    pleg->AddEntry((TObject*)0, Form("%s: %.3f #pm %.3f",fLAr->GetParName(1), fLAr->GetParameter(1), fLAr->GetParError(1)), "");
    pleg->AddEntry((TObject*)0, Form("%s: %.2f #pm %.2f",fLAr->GetParName(2), fLAr->GetParameter(2), fLAr->GetParError(2)), "");
    pleg->AddEntry((TObject*)0, Form("%s: %.2f #pm %.2f",fLAr->GetParName(3), fLAr->GetParameter(3), fLAr->GetParError(3)), "");

    // ((TText*)pleg->GetListOfLines()->Last())->SetTextAlign(23);
    pleg->Draw();

    if (gROOT->IsBatch()){
      c->Print("conv_fit.png");
    }

    FILE *fout; 

    fout = fopen("convolution_output.txt", "w");
    z->getWaveform(0);
    fprintf(fout, "%.0f %.3f %.2f %.2f\n", z->ch[kch]->time, fLAr->GetParameter(1), fLAr->GetParameter(2), fLAr->GetParameter(3));
    fclose(fout);

    // fout.open("spe_response.txt", ios::out);
    // for (auto y: ysphe){
    //   fout << y << endl;
    // }
    // fout.close();
    // fout.open("lar_response.txt", ios::out);
    // for (auto y: yraw){
    //   fout << y << endl;
    // }
    // fout.close();
}


