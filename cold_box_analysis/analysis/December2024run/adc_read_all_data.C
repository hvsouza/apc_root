// ________________________________________ //
// Author: Henrique Souza
// Filename: adc_read_all_data.C
// Created: 2021
// ________________________________________ //
#include "MYCODES.h"
#include <filesystem>


void adc_read_all_data(string datadir = "./"){


    Read r;

    r.dtime = 16; // step time in ns
    r.nbits = 14; // this is used only for baseline histogram, changing it to 12 might help
    r.isBinary = true;

    r.n_points = 1024;
    r.baselineTime = 1024*16; // time limit for baseline
    r.chargeTime = 3008; // last time to integrate
    r.startCharge = 2160;
    r.maxRange = 3000; // max range to search for amplitude peak
    r.fast = 200; // fprompt fast integration time
    r.slow = 1700; //fprompt slow integration time
    r.exclusion_baseline = 17; // filtered waveform, anything above here will do +exclusion window while evaluating the baseline
    r.map_exclusion_threshold_baselines = {
        { "Ch31.", 17},  // C1
        { "Ch36.", 24},  // C1
        { "Ch30.", 120}, // C2,
        { "Ch37.", 100}, // C2
        { "Ch41.", 250}, // C3
        { "Ch46.", 120}, // C3
        { "Ch40.", 40},  // C4
        { "Ch47.", 17},  // C4
    };
    // r.exclusion_baselines = {35,15};
    r.exclusion_window = 2000; // time in ns that it will jump for baseline
    r.baselineFraction = 1/6.;
    r.filter = 4; // denoise filter.. if filter = 0, no denoise is done.
    // r.OnlySomeEvents = true; // Do you want only one event? Choose it wisely (you can set stopEvent)

    r.stopEvent = 1000; //
    r.noBaseline = false; // if you dont want to calculate baseline

    r.with_headers = false;
    r.reference_name_before_ch = "channel_";

    // r.saveFilter = true;

    // r.channels = {1};
    // r.channels = {1,2,3,4};
    r.standard_log_file = "files.log";

    const char *logfilename = r.standard_log_file.c_str();

    std::filesystem::path filePath = datadir;
    if (filePath.extension() == ".log") // Passing log of files I want..
        r.standard_log_file = datadir;
    else
    {
        if (system(Form("touch %s && rm %s", logfilename, logfilename))){
            return;
        }
        if (filePath.extension() == ".dat") // Heed the dot.
        {
            string packdata = Form("echo %s | sed -e 's/.dat$//' > %s", datadir.c_str(), logfilename);
            if (system(packdata.c_str()))
                return;
        }
        else
        {
            string packdata = Form("ls -1 %s*.dat | sed -e 's/.dat$//' > files.log", datadir.c_str());
            // string packadata = Form("ls -1 %s*.dat | grep wave | sed -e 's/.dat$//' > files.log", datadir.c_str()); // to only check one channel
            // string packadata = Form("ls -1 %s*.dat | grep wave[2,3] | sed -e 's/.dat$//' > files.log", datadir.c_str()); // to only check channels 2 and 3
            if (system(packdata.c_str())) return;
        }
    }

    r.adc_read_all_data();


}

