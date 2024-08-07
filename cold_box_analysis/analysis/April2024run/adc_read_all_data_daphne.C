#define memorydepth 1000
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void adc_read_all_data_daphne(string datadir = "./", map<string, Double_t> exclu_b = {{"Ch0.", 30}}, Int_t polarity = 1){
    
    system("rm files.log");
    int datadirlength = datadir.length();
    if(datadir[datadirlength-1] != '/'){
        datadir = datadir + "/";
    }
    string packdata = Form("ls -1 -v %s*.csv | sed -e 's/.csv$//' > files.log", datadir.c_str());
    // string packadata = Form("ls -1 %s*.dat | grep wave | sed -e 's/.dat$//' > files.log", datadir.c_str()); // to only check one channel
    // string packadata = Form("ls -1 %s*.dat | grep wave[2,3] | sed -e 's/.dat$//' > files.log", datadir.c_str()); // to only check channels 2 and 3
    system(packdata.c_str());
    
    
    Read r;
        
    r.dtime = 16;
    r.nbits = 14;
    r.isBinary = true;
    
    r.baselineTime = 16000; // time limit for baseline
    r.chargeTime = 6800; // last time to integrate
    r.startCharge = 6000;
    r.maxRange = 1600; // max range to search for amplitude peak
    r.fast = 200; // fprompt fast integration time
    r.slow = 1000; //fprompt slow integration time
    // r.exclusion_baselines = {exclu_b};
    r.map_exclusion_threshold_baselines = exclu_b;
    r.exclusion_window = 1000; // time in ns that it will jump for baseline
    r.filter = 2; // denoise filter.. if filter = 0, no denoise is done. r.OnlyOneEvent = false; // Do you want only one event? Choose it wisely
    // r.OnlySomeEvents = true;
    r.stopEvent = 500;
    r.noBaseline = false;
    r.baselineFraction = 1./4;
    r.polarity = polarity;
    r.isBinary = false;
    r.other_ref_name = "channel";
    r.file_extension = ".csv";
    r.with_headers = false;
    r.withTimestamp = false;
    // r.saveFilter = true;
      

    r.adc_read_all_data();

    
}

