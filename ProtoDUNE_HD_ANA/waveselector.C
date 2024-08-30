
ANALYZER *zpe = nullptr;


void waveselector(int ch, string filename, bool isSPE){
  vector<Double_t> cuts(5);
  switch (ch){
    case 13:
      cuts = {65, 90, 20, -40, -5, 15};
      break;
    case 34:
      cuts = {65, 90, 20, -38, -10, 15};
      break;
    case 1113:
      cuts = {65, 90, 10, -40, -5, 15};
      break;
    case 1225:
      cuts = {65, 90, 6, -30 , -4.2 ,6};
      break;
    case 1225:
      cuts = {65, 90, 6, -30 , -4.2 ,6};
      break;
    case 1245:
      cuts = {0, 0, 3, -6, -2.5, 3.2};
      break;
    default:
      cuts = {0, 1e12, 1e12 ,-1e12, -1e12, 1e12};
  }


  cout << "Computing waveform..." << endl;

  zpe = new ANALYZER("zpe");
  zpe->setAnalyzer(filename);
  
  bool chok = false;
  if (zpe->channels.size()>1) chok = zpe->setChannel(Form("Ch%d.", ch));
  else chok = true;
  if (!chok) return;

  string selection = "";
  TGraph *gsphe = nullptr;
  vector<Double_t> avgspe(zpe->n_points,0);
  vector<Double_t> time(zpe->n_points,0);

  bool stop = false;
  Double_t myfilter = 14;
  int cutidx=1;
  if (isSPE){
    zpe->getSelection(Form("Ch%d.selection == 1", ch));
  }
  else{
  // Simple selection
    zpe->genSelectByAmplitude(14,2100, 2300, cuts[0], "higher");
    if (cuts[cutidx]!=0)
      zpe->excludeByAmplitude(14,2100, 2300, cuts[cutidx], "higher");
    else
      stop = true;
  }

  cutidx++;
  if (cuts[cutidx]!=0)
    zpe->excludeByAmplitude(14,0, 2000, cuts[cutidx], "higher");
  else
    stop = true;

  cutidx++;
  if (!stop && cuts[cutidx]!=0)
  {
    myfilter = 10;
    zpe->excludeByAmplitude(10,2000,zpe->n_points*zpe->dtime, cuts[cutidx], "lower");
  }
  else
    stop = true;

  cutidx++;
  if (!stop && cuts[cutidx]!=0)
    zpe->excludeByAmplitude(10,6000,zpe->n_points*zpe->dtime, cuts[cutidx], "lower");
  else
    stop = true;

  cutidx++;
  if (!stop && cuts[cutidx]!=0)
  zpe->excludeByAmplitude(10,6000,zpe->n_points*zpe->dtime, cuts[cutidx], "higher");
  else
    stop = true;

  Double_t navg = 0;
  for (int i = 0; i < zpe->lev->GetN(); i++) {
    Int_t iev = zpe->lev->GetEntry(i);
    zpe->getWaveform(iev);
    for(Int_t j = 0; j < zpe->n_points; j++){
      avgspe[j] += zpe->ch[zpe->kch]->wvf[j];
    }
    navg += 1;

  }
  cout << "Valid SPEs = " << navg << endl;
  for(Int_t i = 0; i < zpe->n_points; i++){
    avgspe[i] = avgspe[i]/navg;
    time[i] = i*zpe->dtime;
  }
  selection="use_mine";
  gsphe = new TGraph(zpe->n_points, &time[0], &avgspe[0]);

  zpe->persistence_plot(600,-100,500,10,selection);
  zpe->getWaveFromGraph(gsphe);
  zpe->baselineparameters.baselineStart      = 0;
  zpe->baselineparameters.baselineTime       = 1800;
  zpe->baselineparameters.exclusion_baseline = 6;
  zpe->baselineparameters.exclusion_window = 400;
  zpe->reval_baseline();
  // zpe->getFFT();
  zpe->drawGraph("SAME");
  
}

