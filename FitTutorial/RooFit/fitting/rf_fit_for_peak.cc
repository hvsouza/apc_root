/*************************************************************
 * Macro: rf_fit_for_peak.cc
 *
 * This exmple defines a PDF for signal (Gaussian) and for a
 * background (Polynomial), generates a single data set to 
 * fit to and summarises the output of the fit.  It is assumed
 * that the mass of the particle that is being searched for 
 * is a priori known.
 *
 * -----------------------------------------------------------
 * - This example is for a n unbinnned extended maximum      -
 * - likelihood fit                                          -
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

void rf_fit_for_peak()
{
  // gROOT->SetStyle("Plain");
  gSystem->Load("libRooFit.so");

  cout << "Running macro rf_fit_for_peak" << endl;
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

  RooRealVar bg_pol1("bg_pol1", "linear term coefficient",  0.01, -10, 10.); 
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
  RooRealVar backgroundYield("backgroundYield", "", 500.0, 0.0, 15000.0);

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

  // In order to test our signal/background model, we need to generate
  // some Monte Carlo (MC) simulated data.  We can obtain a fast MC
  // simulated data sample from TotalPdf using the generate member function
  // of the PDF.  This will give us a pointer to a RooDataSet object.  We need
  // to specify what variables to generate (just mass in this example), and 
  // how many events to generate (we shall use signalYield+backgroundYield).
  Int_t nToGen = (Int_t)( signalYield.getVal() + backgroundYield.getVal() );
  RooDataSet * data = (RooDataSet*)TotalPdf.generate(RooArgSet(mass), nToGen);

  // now that we have some toy data, we can fit to the data.  There are several
  // options that we want to worry about when fitting.  These are summarised 
  // below, and are reproduced from the RooAbsPdf source file.
  //
  // Available fit options:
  //
  //  "m" = MIGRAD only, i.e. no MINOS
  //  "s" = estimate step size with HESSE before starting MIGRAD
  //  "h" = run HESSE after MIGRAD
  //  "e" = Perform extended MLL fit
  //  "0" = Run MIGRAD with strategy MINUIT 0 (no correlation matrix calculation at end)
  //        Does not apply to HESSE or MINOS, if run afterwards.
  //
  //  "q" = Switch off verbose mode
  //  "l" = Save log file with parameter values at each MINUIT step
  //  "v" = Show changed parameters at each MINUIT step
  //  "t" = Time fit
  //  "r" = Save fit output in RooFitResult object (return value is object RFR pointer)
  //
  // The following options are chosen in order to run MINOS (i.e. get asymmetric 
  // errors from the fit), perform an extended fit, time the fit, and also save 
  // the output of the fit into a RooFitResult object.
  //
  RooFitResult * result = TotalPdf.fitTo(*data, "etr");

  // Now that we have fit the toy data, we can take a look at what has 
  // been generated and by plotting the RooDataSet onto a RooPlot.
  RooPlot * frame = mass.frame(50);
  data->plotOn(frame);

  // We can then add the total PDF compare the fitted PDF to the data
  TotalPdf.plotOn(frame);

  // the following two lines of code, draw frame, and then save the 
  // canvas that the frame is drawn on to an eps file.
  TCanvas *c1 = new TCanvas("c1","c1");
  frame->Draw();
  c1->Print("rf_fit_for_peak.eps");

  // If you are familar with TTrees and would prefer to use this ROOT 
  // data type as opposed to learning about the full functionality of 
  // a RooDataSet and how to access it, that is no problem.  Given
  // a RooDataSet, you can make a TTree with a single function call, 
  // and once this has been done you can set a new name for the tree
  TTree * tree = (TTree*)data->tree();
  tree->SetName("datatree");

  // Now the tree can be used in the normal way (for example by drawing 
  // the mass into a histogram).
  TH1F mass_hist("mass_hist", "", 100, mass.getMin(), mass.getMax() );
  tree->Draw("mass>>mass_hist");

  // Save our generated data set, fit results and plot to a 
  // file: before we do this, we want to make sure that the
  // RooPlot, fit result and data have sensible names that
  // are meaninful to us (you will almost certainly want
  // to change the default name for a RooPlot all of the time).
  data->SetName("generatedData");
  frame->SetName("mass_plot");
  result->SetName("fitResult");
  TFile f("rf_fit_for_peak.root", "RECREATE");
  data->Write();
  frame->Write();
  result->Write();

  // also save the tree and histogram
  tree->Write();
  mass_hist.Write();

  f.Write();
  f.Close();
}
