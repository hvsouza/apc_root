/*************************************************************
 * Macro: rf_toy_mc.cc
 *
 * This exmple defines a PDF for signal (Gaussian) and for a
 * background (Polynomial), generates toy data sets to and stores 
 * the output of the ensemble of toy experiments.  It is assumed
 * that the mass of the particle that is being searched for 
 * is a priori known.
 *
 * -----------------------------------------------------------
 * - This example runs a toy Monte Carlo study for an        - 
 * - unbinnned extended maximum likelihood fit.              -
 * - This example is an extension of rf_fit_for_peak.cc      -
 * - RooFit provides its own toy MC class called RooMCStudy, -
 * - so once you are familiar with the technique, you might  -
 * - like to look at examples of using that class (see the   -
 * - RooFit website for details.                             -
 * -----------------------------------------------------------
 *
 * More information on this set of examples can be found at the 
 * following website:
 *
 *     http://www.ph.qmul.ac.uk/~bevan/yeti/
 *
 * Detailed examples and tutorials on the use of the RooFit 
 * fitting package can be found via the RooFit home page:
 *
 *     http://roofit.sourceforge.net/
 *
 * Adrian Bevan, QMUL (C) 2006 
 * e-mail: a.j.bevan@qmul.ac.uk
 *************************************************************/

void rf_toy_mc()
{
  gROOT->SetStyle("Plain");
  gSystem->Load("libRooFit.so");

  cout << "Running macro rf_toy_mc" << endl;
  // define the discriminating variable to be used in the fit.  This
  // is the mass of the hypothetical particle that we want to search 
  // for.  The variables used in RooFit are RooRealVars, and the argument
  // syntax used here is RooRealVar name("name", "title", min, max, "units");
  // where the convention is to have "name" matching the local variable 
  // name that you use in root macros.
  RooRealVar mass("mass", "Mass", 1, 10, "GeV/c^{2}");

  // define the variables that will be used to define the shape of the 
  // PDFs.  Signal variables are prefixed with sig_, and background
  // variables are prefixed with bg_
  RooRealVar sig_mu("sig_mu", "mean", 3.15);  // assume that the particle mass is known 
  RooRealVar sig_sigma("sig_sigma", "width", 0.05, 0.0, 0.1);

  RooRealVar bg_pol1("bg_pol1", "linear term coefficient", 0.01, -10, 10.); 
  RooRealVar bg_pol2("bg_pol2", "quadratic term coefficient", 0.05, -10, 10); 

  // The next step is to define the PDFs used in the fit.  The signal
  // is a Gaussian PDF as a function of mass, with mean and width
  // sig_mu and sig_sigma respectively
  RooGaussian sig_pdf("sig_pdf", "signal pdf", mass, sig_mu, sig_sigma);

  // and the background is a polynomial, which has two parameters 
  // corresponding to linear and quadratic terms (bg_pol0 and bg_pol1,
  // respectively).
  RooArgList bgCoefList;
  bgCoefList.add(bg_pol1);
  bgCoefList.add(bg_pol2);
  RooPolynomial bg_pdf("bg_pdf", "background pdf", mass, bgCoefList);

  // now that the individual background and signal PDS have been defined,
  // we can write the total PDF.  We need to have an extended PDF as we 
  // are searching for event yields, and so we need to account for the
  // poissonian nature of the problem.  
  //
  // First we have to declare a signalYield and a backgroundYield, then 
  // we can associate these with the sig_pdf and bg_pdf when forming a
  // total pdf = (poisson term)[signal yield * signal pdf + 
  //                    background yield * background pdf]
  //
  RooRealVar signalYield("signalYield", "", 50.0, -10.0, 150.0);
  RooRealVar backgroundYield("backgroundYield", "", 500.0, 0.0, 1500.0);

  // make a list of PDFs (note the order)
  RooArgList pdfList;
  pdfList.add(sig_pdf);
  pdfList.add(bg_pdf);

  // make a list of coefficients (add to the list in the same order
  // as done for the PDFs)
  RooArgList coefList;
  coefList.add(signalYield);
  coefList.add(backgroundYield);

  // now we can make the total PDF.  Note that if the number if entries
  // in both pdfList and coefList match, RooFit will make sure that this
  // is an extended PDF when used in the fit.
  RooAddPdf TotalPdf("TotalPdf", "Extended PDF", pdfList, coefList);

  // Now we have defined and build our PDF, want to run a toy MC study
  // to validate that we get unbiased values of the signal and background
  // yields.  We also need to keep track of what our original parameters are
  // that we want to generate from so that each toy is generated correctly.
  Int_t nToys = 100;  // the number of toy MC experiments to generate and fit back 
  Double_t signalYieldVal      = signalYield.getVal();
  Double_t backgroundYieldVal  = backgroundYield.getVal();
  Double_t sig_sigmaVal        = sig_sigma.getVal();
  Double_t bg_pol1Val          = bg_pol1.getVal();
  Double_t bg_pol2Val          = bg_pol2.getVal();

  // before running the toy, set up a tree for the fitted results
  Double_t fitted_signalYield(0.0);
  Double_t fitted_backgroundYield(0.0);
  Double_t fitted_sig_sigma(0.0);
  Double_t fitted_bg_pol1(0.0);
  Double_t fitted_bg_pol2(0.0);
  Double_t fitted_signalYieldError(0.0);
  Double_t fitted_backgroundYieldError(0.0);
  Double_t fitted_sig_sigmaError(0.0);
  Double_t fitted_bg_pol1Error(0.0);
  Double_t fitted_bg_pol2Error(0.0);
  Int_t minNll(0);
  Int_t status(0);
  TTree tree("toyTree", "");
  tree.Branch("signalYield",     &fitted_signalYield,    "signalYield/D");
  tree.Branch("backgroundYield", &fitted_backgroundYield,"backgroundYield/D");
  tree.Branch("sig_sigma",       &fitted_sig_sigma,      "sig_sigma/D");
  tree.Branch("bg_pol1",         &fitted_bg_pol1,        "bg_pol1/D");
  tree.Branch("bg_pol2",         &fitted_bg_pol2,        "bg_pol2/D");
  tree.Branch("signalYieldError",     &fitted_signalYieldError,    "signalYieldError/D");
  tree.Branch("backgroundYieldError", &fitted_backgroundYieldError,"backgroundYieldError/D");
  tree.Branch("sig_sigmaError",       &fitted_sig_sigmaError,      "sig_sigmaError/D");
  tree.Branch("bg_pol1Error",         &fitted_bg_pol1Error,        "bg_pol1Error/D");
  tree.Branch("bg_pol2Error",         &fitted_bg_pol2Error,        "bg_pol2Error/D");
  tree.Branch("status",          &status,                "status/I");
  tree.Branch("minNll",          &minNll,                "minNll/I");

  TRandom rand;
  int i = 0;
  for(i=0; i < nToys; i++) {
    // in order to obtain the correct spread of results, we need to generate
    // each toy with a Poissonian fluctuation about the nominal value to 
    // be generated.
    Int_t nToGen = rand.Poisson( signalYield.getVal() + backgroundYield.getVal() );
    RooDataSet * data = (RooDataSet*)TotalPdf.generate(RooArgSet(mass), nToGen);

    // as before with rf_fit_to_peak.cc, we can now fit the generated data sample
    RooFitResult * result = TotalPdf.fitTo(*data);

    // store the fit results in the tree.  The minimum of interesting 
    // information is the fit status (to check if fit converged 
    // properly or not), the value of -lnL and the fitted values of
    // the PDF parameters allowed tovary in the fit AND their errors.
    minNll = result->minNll();  // this is the -lnL value for the fit
    status = result->status();  // this is the fit status
    fitted_signalYield          = signalYield.getVal();
    fitted_backgroundYield      = backgroundYield.getVal();
    fitted_sig_sigma            = sig_sigma.getVal();
    fitted_bg_pol1              = bg_pol1.getVal();
    fitted_bg_pol2              = bg_pol2.getVal();
    fitted_signalYieldError     = signalYield.getError();
    fitted_backgroundYieldError = backgroundYield.getError();
    fitted_sig_sigmaError       = sig_sigma.getError();
    fitted_bg_pol1Error         = bg_pol1.getError();
    fitted_bg_pol2Error         = bg_pol2.getError();
    tree.Fill();                                   // store the results of this toy

    // before we run the next toy MC study, we have to reset the initial values
    // for generation.
    signalYield.setVal( signalYieldVal );
    backgroundYield.setVal( backgroundYieldVal );
    sig_sigma.setVal( sig_sigmaVal );
    bg_pol1.setVal( bg_pol1Val );
    bg_pol2.setVal( bg_pol2Val );
  }

  // We can now plot some interesting distributions
  //
  //
  TCanvas can("toyCanvas", "toy canvas");
  can.Divide(2,2);
  can.cd(1);
  tree.Draw("signalYield");

  can.cd(2);
  tree.Draw("(signalYield-50)/signalYieldError");

  can.cd(3);
  tree.Draw("minNll");

  can.cd(4);
  tree.Draw("status");

  can.Print("rf_toy_mc.eps");

  // save the tree and canvas of the toy results to a file
  TFile f("rf_toy_mc.root", "RECREATE");
  tree.Write();
  can.Write();

  f.Write();
  f.Close();
}
