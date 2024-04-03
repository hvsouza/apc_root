/*************************************************************
 * Macro: f_triple_gaussian.cc
 *
 * This exmple defines a PDF for the sum of three Gaussian functions
 *
 * -----------------------------------------------------------
 * - This example is for a binned fit to data stored in a 1D -
 * -    histogram.                                            -
 * -----------------------------------------------------------
 *
 * More information on this set of examples can be found at the 
 * following website:
 *
 *     http://www.ph.qmul.ac.uk/~bevan/yeti/
 *
 * Adrian Bevan, QMUL (C) 2006 
 * e-mail: a.j.bevan@qmul.ac.uk
 *************************************************************/

// define the number of parameters and fit range for the function fitFunc.
Int_t npar = 9;
Double_t min_range = 1.0;
Double_t max_range = 10.0;

/*
 * Define the fit function to determine the parameters of the 
 * sum of three Gaussians
 *
 * parameter   name                  description     
 * -------------------------------------------------------------
 * 0          norm              Normalisation.  The value of this parameter is dependent on the
 *                                              fit range and choice of histogram binning.
 * 1          mu1               mean of Gaussian 1
 * 2          sigma1            width of Gaussian 1
 * 3          mu2               mean of Gaussian 2
 * 4          sigma2            width of Gaussian 2
 * 5          mu3               mean of Gaussian 3
 * 6          sigma3            width of Gaussian 3
 * 7          frac1             fraction of Gaussian 1
 * 8          frac2             fraction of Gaussian 2
 */
Double_t fitFunc(Double_t * x, Double_t * par)
{
  Double_t PDF     = 0.0;
  Double_t g1     = 0.0;
  Double_t g2     = 0.0;
  Double_t g3     = 0.0;

  // Calculate the exponents of the Gaussians
  Double_t arg1 = (par[2] != 0.0) ? (x[0] - par[1])/(par[2]) : 0.0;
  Double_t arg2 = (par[4] != 0.0) ? (x[0] - par[3])/(par[4]) : 0.0;
  Double_t arg3 = (par[6] != 0.0) ? (x[0] - par[5])/(par[6]) : 0.0;

  // add each Gaussian contribution to the PDF
  g1 = exp(-0.5*arg1*arg1)/(par[2]*sqrt(2.0*TMath::Pi()));
  g2 = exp(-0.5*arg2*arg2)/(par[4]*sqrt(2.0*TMath::Pi()));
  g3 = exp(-0.5*arg3*arg3)/(par[6]*sqrt(2.0*TMath::Pi()));
  PDF = par[0]*(par[7]*g1 + par[8]*g2 + (1-par[7]-par[8])*g3);

  return PDF;
}

void f_triple_gaussian()
{
  // gROOT->SetStyle("Plain");
  gSystem->Load("libMinuit.so");
  cout << "Running macro f_triple_gaussian" << endl;
  
  // define the user function 'fitFunc', for a given fit range 
  // (min_range to max_range), with a pre-determined number of 
  // fit parameters (npar).
  TF1 * myFitFunc = new TF1("fitFunc", fitFunc, min_range, max_range, npar);
  
  // Now that the fit function has been chose, we have to 
  //
  //  1. Set the parameter names.
  //  2. Give parameters initial values.
  //  3. Set the allowed ranges for fit parameters.
  //  4. Decide if we know any of the parameters well enough to 
  //    fix them to a constant value.
  //
  // The following steps go through this list of 'setting up' 
  // the fit
  
  // 1. Set the parameter names (these names match the comment field
  //    where the function is defined above).
  myFitFunc->SetParName(0, "norm");
  myFitFunc->SetParName(1, "mu1");
  myFitFunc->SetParName(2, "sigma1");
  myFitFunc->SetParName(3, "mu2");
  myFitFunc->SetParName(4, "sigma2");
  myFitFunc->SetParName(5, "mu3");
  myFitFunc->SetParName(6, "sigma3");
  myFitFunc->SetParName(7, "frac1");
  myFitFunc->SetParName(8, "frac2");
  
  // 2. Now set initial parameter values (you can do this for up to 11 
  //    parameters with this function call)
  myFitFunc->SetParameters(1500.0, 5, 0.5, 6, 1.5, 5, 5, 0.4, 0.3);
  
  // 3. Now set the parameter limits for each parameter in turn.
  //    These are the 'allowed' ranges for each of the parameters
  //    used in the fit.
  myFitFunc->SetParLimits(0, 0.0, 2500.0);  // norm
  myFitFunc->SetParLimits(1, 0.01, 10.0);   // mu1
  myFitFunc->SetParLimits(2, 0.0,  5);      // sigma1
  myFitFunc->SetParLimits(3, 0.01, 10.0);   // mu2
  myFitFunc->SetParLimits(4, 0.0,  5);      // sigma2
  myFitFunc->SetParLimits(5, 0.01, 10.0);   // mu3
  myFitFunc->SetParLimits(6, 0.0,  10.);    // sigma3
  myFitFunc->SetParLimits(7, 0.0,  1.0);    // frac1
  myFitFunc->SetParLimits(8, 0.0,  1.0);    // frac2
  
  // 4. Decide if we know any parameters well enough to fix them.  
  // using the function call FixParameter(iPar, val). Fix the mean
  // and width of the third Gaussian
  myFitFunc->FixParameter(5, 5.0); // mu3
  myFitFunc->FixParameter(6, 5.0); // sigma3

  // Now that the fit has been set up, we need to obtain a data sample 
  // to fit to.  Make an empty histogram and fill it with 500
  // entries weighted according to the fit function.
  TH1F *data_hist = new TH1F("data_hist", "", 50, min_range, max_range);
  data_hist->FillRandom("fitFunc", 1500);

  // perform a binned likelihood fit to the histogram.  The possible 
  // options for the second argument are
  //   W  Set all errors to 1
  //   I  Use integral of function in bin instead of value at bin center
  //   L  Use Loglikelihood method (default is chisquare method)
  //   LL Use Loglikelihood method and bin contents are not integers)
  //   U  Use a User specified fitting algorithm (via SetFCN)
  //   Q  Quiet mode (minimum printing)
  //   V  Verbose mode (default is between Q and V)
  //   E  Perform better Errors estimation using Minos technique
  //   B  Use this option when you want to fix one or more parameters
  //      and the fitting function is like "gaus","expo","poln","landau".
  //   M  More. Improve fit results
  //   R  Use the Range specified in the function range
  //   N  Do not store the graphics function, do not draw
  //   0  Do not plot the result of the fit. By default the fitted function
  //      is drawn unless the option"N" above is specified.
  //   +  Add this new fitted function to the list of fitted functions
  //      (by default, any previous function is deleted)

  data_hist->Draw();
  data_hist->Fit("fitFunc", "LE");
  // TCanvas *c1 = new TCanvas();
  // c1->Print("f_triple_gaussian.eps");
}
