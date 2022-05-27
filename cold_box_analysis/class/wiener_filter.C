#include "MYCODES.h"


class WIENER{
public:

  string obj_name;
  Int_t npts = memorydepth;
  Double_t frequency = 250; // in units
  Double_t step = 4; // in ns
  Double_t units_freq = 1e6; // 1e6 = MHz,  
  Double_t units_step = 1e-9; // 1e9 = ns,  
  Double_t factor = 1./(npts);

  TH1D *hfft;
  TH1D *hPSD;
  TH1D *hwvf;
  Double_t powerSpectrum = 0;
  TComplex *spec = nullptr;
  Double_t *spec_re = nullptr;
  Double_t *spec_im = nullptr;
  Double_t *re_comp = nullptr;
  Double_t *im_comp = nullptr;
  
  TF1 *gaus_filter;
  TF1 *flar;

  Double_t *res = nullptr;
  Int_t maxBin=0;

  string unit_time;
  string unit_freq;

  void deconvolve(WIENER y, WIENER h, Double_t cutoff_frequency = 50){ // y is the signal, h is the device response (a.k.a single photo-electron)
    // cutoff_frequency is the cutoff frequency in MHz (or your unit set)
   gaus_filter = new TF1("filter","TMath::Gaus(x,[0],[1])",0,frequency);	// A gaussian filter

   // the cutoff frequency is when x is equal to sqrt(0.7)*sigma, so to use 50 MHz as cutoff, sigma must be 50/sqrt
   cutoff_frequency = cutoff_frequency/sqrt(0.7);
   gaus_filter->SetParameters(0,cutoff_frequency);
   
   // the for is performed in k, need to convert back to frequency
   Double_t convert_freq = frequency/npts;


   
   for(Int_t k=0; k<npts/2+1; k++){
     spec[k] = y.spec[k]*gaus_filter->Eval(convert_freq*k)/h.spec[k];
      
     spec_re[k] = spec[k].Re();
     spec_im[k] = spec[k].Im();

     // or you can do something like this...
     // spec_re[k] = gaus_filter->Eval(k)*(y.re_comp[k]*h.re_comp[k] + y.im_comp[k]*h.im_comp[k])/(pow(h.re_comp[k],2)+pow(h.im_comp[k],2));
     // spec_im[k] = gaus_filter->Eval(k)*(h.re_comp[k]*y.im_comp[k] - y.re_comp[k]*h.im_comp[k])/(pow(h.re_comp[k],2)+pow(h.im_comp[k],2));

   }
   //Now let's make a backward transform:
   Int_t n = npts;
   TVirtualFFT *fft_final = TVirtualFFT::FFT(1, &n, "C2R M K");
   fft_final->SetPointsComplex(spec_re,spec_im);
   fft_final->Transform();
   TH1 *hfinal = 0;
   //Let's look at the output
   hfinal = TH1::TransformHisto(fft_final,hfinal,"Ref");
   hfinal->Scale(factor);

   hwvf = new TH1D(Form("wvf_%s",obj_name.c_str()),Form("wvf_%s",obj_name.c_str()),npts,0,npts*step);
   hwvf->GetXaxis()->SetTitle(Form("Time (%s)",unit_time.c_str()));
   hwvf->GetYaxis()->SetTitle("Amplitude (A.U.)");
   for(Int_t i = 0; i<n; i++){
     res[i] = hfinal->GetBinContent(i+1);
     hwvf->SetBinContent(i+1,res[i]);
   }

   shift_waveform(hwvf,y.maxBin);
   fft(hwvf);
   
   flar = new TF1("flar",Form("[0]*exp(-(x-%f)/[1])+[2]*exp(-(x-%f)/[3])",y.maxBin*step,y.maxBin*step),0,npts*step);
   flar->SetParameters(0.3,10,0.3,1000);
   

   delete hfinal;
   
  }
  void shift_waveform(TH1D *h, Int_t new_max){
    Int_t old_max = h->GetMaximumBin();
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
  
  TVirtualFFT * fft(TH1D *hsignal){

    maxBin = hsignal->GetMaximumBin(); //get maximum to realign waveforms later
    hfft = new TH1D(Form("fft_%s",obj_name.c_str()),Form("FFT %s",obj_name.c_str()),npts/2,0,frequency/2);
    hPSD = new TH1D(Form("PSD_%s",obj_name.c_str()),Form("Power Spectral Density %s",obj_name.c_str()),npts/2,0,frequency/2);
    hfft->GetXaxis()->SetTitle(Form("Frequency (%s)",unit_freq.c_str()));
    hPSD->GetXaxis()->SetTitle(Form("Frequency (%s)",unit_freq.c_str()));
    hfft->GetYaxis()->SetTitle("Magnitude");
    hPSD->GetYaxis()->SetTitle("PSD (Magnitude^{2} Hz^{-1})");
 
    TH1 *hm = 0;
    TVirtualFFT::SetTransform(0);
    hm = hsignal->FFT(hm, "MAG R2C measure");
   //NOTE: for "real" frequencies you have to divide the x-axes range with the range of your function
   //(in this case 4*Pi); y-axes has to be rescaled by a factor of 1/SQRT(n) to be right: this is not done automatically!

    //Look at the DC component and the Nyquist harmonic:
    //That's the way to get the current transform object:
    TVirtualFFT *fft = TVirtualFFT::GetCurrentTransform();
    
   //Use the following method to get the full output:
   fft->GetPointsComplex(re_comp,im_comp);
   
   for(Int_t k = 0; k<npts/2+1; k++){
     spec[k] = TComplex(re_comp[k],im_comp[k])*factor;
     hfft->SetBinContent(k+1,spec[k].Rho2());
     // the same as:
     // hfft->SetBinContent(k+1,hm->GetBinContent(k+1)*factor);

     //for spectrum density
     hPSD->SetBinContent(k+1,spec[k].Rho2());
     powerSpectrum = hPSD->Integral()*2; // *2 because it is only half of the spectrum
     
     // cout << k << " " << re_comp[k] << " " << im_comp[k] << " " << spec[k].Rho2() << endl;
   }
   delete hm;

   return fft;
  }


  void backfft(WIENER y){

   for(Int_t k=0; k<npts/2+1; k++){
     spec[k] = y.spec[k];
      
     spec_re[k] = spec[k].Re();
     spec_im[k] = spec[k].Im();

   }
   //Now let's make a backward transform:
   Int_t n = npts;
   TVirtualFFT *fft_final = TVirtualFFT::FFT(1, &n, "C2R M K");
   fft_final->SetPointsComplex(spec_re,spec_im);
   fft_final->Transform();
   TH1 *hfinal = 0;
   //Let's look at the output
   hfinal = TH1::TransformHisto(fft_final,hfinal,"Ref");
   // hfinal->Scale(factor); // you dont scale to get back ...

   hwvf = new TH1D(Form("wvf_%s",obj_name.c_str()),Form("wvf_%s",obj_name.c_str()),npts,0,npts*step);
   hwvf->GetXaxis()->SetTitle(Form("Time (%s)",unit_time.c_str()));
   hwvf->GetYaxis()->SetTitle("Amplitude (A.U.)");
   for(Int_t i = 0; i<n; i++){
     res[i] = hfinal->GetBinContent(i+1);
     hwvf->SetBinContent(i+1,res[i]);
   }

   fft(hwvf);
   
   flar = new TF1("flar",Form("[0]*exp(-(x-%f)/[1])+[2]*exp(-(x-%f)/[3])",y.maxBin*step,y.maxBin*step),0,npts*step);
   flar->SetParameters(0.3,10,0.3,1000);
   

   delete hfinal;
   
  }

  void rescale_histogram(Int_t newpts);

  

  WIENER(string myname) : obj_name{myname} {startup();};
  WIENER(string myname, Int_t mynpts) : obj_name{myname},
                                        npts{mynpts}  {startup();}
  WIENER(string myname, Double_t mystep, Double_t myfreq, Double_t myunits_step, Double_t myunits_freq, Int_t mynpts ) : obj_name{myname},
                                                                                                                         frequency{myfreq},
                                                                                                                         step{mystep},
                                                                                                                         units_step{myunits_step},
                                                                                                                         units_freq{myunits_freq},
                                                                                                                         npts{mynpts}
                                                                                                                         
  {startup();}
  

  void startup(){

    spec = new TComplex[npts];
    spec_re = new Double_t[npts];
    spec_im = new Double_t[npts];
    re_comp = new Double_t[npts];
    im_comp = new Double_t[npts];
    res = new Double_t[npts];
    factor = 1./(npts);
  
    
    if(units_step == 1e-9)
        unit_time = "ns";
    else if(units_step == 1e-6)
        unit_time = "#mus";
        
    else if(units_step == 1e-3)
        unit_time = "ms";
        
    else if(units_step == 1)
        unit_time = "s";

    if(units_freq == 1e9)
        unit_freq = "GHz";
    else if(units_freq == 1e6)
        unit_freq = "MHz";
        
    else if(units_freq == 1e3)
        unit_freq = "kHz";
        
    else if(units_freq == 1)
        unit_freq = "Hz";

    
    
  }
  
  // based https://dspcookbook.github.io/optimal-filtering/wiener-filter-2/#3-solution
  // creates filter from source s and reference d (such as noise)
  vector<Double_t> create_filter_from_reference(vector<Double_t> signal, vector<Double_t> reference,Int_t filter_size){
    Int_t n = reference.size();
    vector<Double_t> w(filter_size);
    vector<Double_t> rxx(filter_size);
    vector<Double_t> rxd(filter_size);
    TH1D *hnoise = new TH1D("hnoise","hnoise",500,0,0);
    for(Int_t i = 0; i<n; i++){
      hnoise->Fill(signal[i]);
    }
    Double_t rvv = hnoise->GetStdDev();
    rvv = rvv*rvv;
    // evaluate rxx (and rxd ?)
    for(Int_t i = 0; i<filter_size; i++){

      for(Int_t j = 0; j<n-i; j++){
        rxx[i] += signal[j]*signal[j+i];
        rxd[i] += signal[j]*reference[j+i];
      }
      rxx[i]=rxx[i]/(n-i);
      rxd[i]=rxd[i]/(n-i);
      // if(i==0)rxd[i]=rxx[i]-rvv;
      // else rxd[i] = rxx[i];
    }

    TMatrixD Rxx(filter_size,filter_size);
    for(Int_t i=0; i<filter_size; i++){
      for(Int_t j = 0; j<filter_size; j++){
        Rxx[i][j] = rxx[abs(i-j)];
      }        
    }
    cout << "Inverting Rxx matrix... " << endl; Rxx.Invert(); 
    vector<Double_t> x(filter_size);
    for(Int_t i = 0; i<filter_size; i++){
      x[i] = i;
      for(Int_t j = 0; j<filter_size; j++){
        w[i] += Rxx[i][j]*rxd[j];
      }
      // cout << w[i] << endl;
    }
    
    // TCanvas *c2 = new TCanvas();
    // TGraph *gtest = new TGraph(filter_size,&x[0],&rxd[0]);
    // gtest->Draw("ALP");

    // hnoise->Draw();
    
    return w;
  }

  vector<Double_t> filter(vector<Double_t> signal, vector<Double_t> w){
    Int_t n = signal.size();
    Int_t m = w.size();
    vector<Double_t> res(n);
    for(Int_t i = m-1; i<n; i++){
      for(Int_t j = 0; j<m; j++){
        res[i] += signal[i-j]*w[j];
      }
      // cout << res[i] << endl; 
    }
    return res;
  }
  
};