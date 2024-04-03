/*
 * Program Source Code: FitUT.cc
 *
 * This exmple uses TMinuit with a pre defined chi^2 to minimise.  The
 * chi^2 is based on constraining the apex of the CKM Unitarity Triangle
 * given input angles alpha (phi_2) and beta (phi_1).
 *
 * More information on this set of examples can be found at the
 * following website:
 *
 *     http://www.ph.qmul.ac.uk/~bevan/yeti/
 *
 * Adrian Bevan, QMUL (C) 2006
 * e-mail: a.j.bevan@qmul.ac.uk
 */
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "TMarker.h"
#include "TROOT.h"
#include "TPad.h"
#include "TFile.h"
#include "TH1.h"
#include "TProfile.h"
#include "TF1.h"
#include "TNtuple.h"
#include "TH2.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TChain.h"
#include "TLeaf.h"
#include "TCanvas.h"
#include "TPaveLabel.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TApplication.h"
#include "TRandom.h"
#include "TGraphErrors.h"
#include "TBox.h"
#include "TVector3.h"
#include "TMinuit.h"
#include "TIterator.h"
#include "TVirtualFitter.h"

using namespace std;

int main(int argc, char **argv);

extern void InitGui();
VoidFuncPtr_t initfuncs[] = { InitGui, 0 };

TROOT root("fitUT"," Example ", initfuncs);

/***********************************************
 * Constants
 ***********************************************/
double deg2rad = M_PI/180.0;
double rad2deg = 180.0/M_PI;

/*************************************
 * DATA - two of the angles of the UT
 *************************************/
double  beta_meas = 21.2*deg2rad;
double dbeta_meas = 1.0*deg2rad;

double  alpha_meas = 95.8*deg2rad;
double dalpha_meas = 7.0*deg2rad;

/*
 * function prototypes
 */
void PrintConstants();
void printParam(TString comment, double * var);
void printMinosFitResult(TString comment, double val, double * err);


/***********************************************
 * The Fit Function to minimise (FCN)
 ************************************************/
extern void chi2(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag) 
{
  double thechi2 = 0.0;

  // calculate the alpha chi2 contribution for a given set of rhobar and etabar
  double alpha_arg1(0), alpha_arg2(0);
  if(par[1] != 0.0) { 
    alpha_arg1 = par[0]/par[1];
    alpha_arg2 = (1-par[0])/par[1]; 
  }
  double alpha_chi2 = (alpha_meas  - ( atan(alpha_arg1) + atan(alpha_arg2) ))/dalpha_meas;

  // calculate the beta chi2 contribution for a given set of rhobar and etabar
  double beta_arg = 0.0;
  if( (1-par[0]) != 0.0) beta_arg = par[1]/(1-par[0]);
  double beta_chi2 = (beta_meas - atan(beta_arg))/dbeta_meas;

  // add the chi2 contributions together and set the return value
  thechi2 += alpha_chi2*alpha_chi2;
  thechi2 += beta_chi2*beta_chi2;
  
  f= thechi2;
}


void printParam(TString comment, double * var) {
  cout << " " << comment.Data() << " = " << var[0] << " +/- " << var[1] << endl;
}

void printMinosFitResult(TString comment, double val, double * err) {
  cout << comment << " = " << val << " +" << err[0] << " " << err[1] << endl; 
}


int main(int argc, char **argv){ 
  gStyle->SetPaperSize(20,20);
  gStyle->SetCanvasColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasBorderSize(0);
  gStyle->SetPadColor(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBorderSize(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(0);
  gStyle->SetStatColor(0);
  gStyle->SetTitleColor(0);
 
  gStyle->SetOptFit(111);


  bool run_hesse = false;
  bool run_minos = false;
  for(int iArg = 1; iArg < argc; iArg++)
  {
    if( !strcasecmp(argv[iArg], "-hesse")) run_hesse = true;
    if( !strcasecmp(argv[iArg], "-minos")) run_minos = true;
  }

  TMinuit *gMinuit = new TMinuit(2);
  gMinuit->Clear();

  int iflag;

  // set the function to minimise with minuit.
  gMinuit->SetFCN(chi2);

  Double_t arglist[10];
  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR", arglist, 1, iflag);

  // set up the fit parameters.  There are three parameters
  // of interest:
  //   0. rhobar (the x-coordinate of the apex)
  //   1. etabar (the y-coordinate of the apex)
  gMinuit->mnparm(0, "rhobar",  0.2,     .1, 0.0,    0.5, iflag);
  gMinuit->mnparm(1, "etabar",  0.3,     .1, 0.0,    0.5, iflag);

  arglist[0] = 1;
  gMinuit->mnexcm("CALL FCN", arglist, 1, iflag);

  arglist[0] = 1000;
  arglist[1] = .1;
  // decide what to run: MIGRAD, HESSE or MINOS.
  if(run_hesse){
    gMinuit->mnexcm("MIGRAD", arglist,2, iflag);
    gMinuit->mnexcm("HESSE",  arglist,0, iflag);
  }
  else if(run_minos){
    gMinuit->mnexcm("MINOS",  arglist,0, iflag);
  }
  else {
    gMinuit->mnexcm("MIGRAD", arglist,2, iflag);
  }

  float Chi2Min0 = gMinuit->fAmin;
  gMinuit->mnexcm("RETURN", arglist,0, iflag);
  cout<<endl;
  cout<<endl;
  cout<<" End of Fit with minuit: FCN =  "<< Chi2Min0 << endl;
  cout<<endl;
  cout<<endl;
  // now summarise results:
  //   To read the fit results into local variables, you have to call 
  //   GetParameter for each of the variables.
  double rhobar[2]; 
  double etabar[2]; 

  gMinuit->GetParameter(0,rhobar[0],rhobar[1]);
  gMinuit->GetParameter(1,etabar[0],etabar[1]);

  cout << "Results with parabolic errors" << endl;
  cout << " rhobar   = " << rhobar[0] << " +/- " << rhobar[1] << endl;
  cout << " etabar   = " << etabar[0] << " +/- " << etabar[1] << endl;

  if(run_minos) {
    // Minuit provides additional information when you parse back
    // the asymmetric errors.  These include the parabolic error 
    // (symmetric) that was obtained from the fit and the global
    // correlation coefficient.
    double dRhobar[4];
    double dEtabar[4];
    //              n   +err        -err     +/- err    global corr
    gMinuit->mnerrs(0, dRhobar[0], dRhobar[1], dRhobar[2], dRhobar[3]);
    gMinuit->mnerrs(1, dEtabar[0], dEtabar[1], dEtabar[2], dEtabar[3]);

    cout << endl<<" Results from Minos" << endl << endl;
    printMinosFitResult(" rhobar       ", rhobar[0], dRhobar);
    printMinosFitResult(" etabar       ", etabar[0], dEtabar);
  }
}

 
