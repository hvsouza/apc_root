#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"


void process_file(vector<TH1D*> h, string pathfile, string histname){

    ANALYZER z("z");
    z.setAnalyzer(pathfile);

    z.setChannel("Ch0.");
    z.averageFFT(0,0,"Entry$<500");
    string hname = histname + "; SoF 1";
    h[0] = (TH1D*)z.hfft[z.kch]->Clone(hname.c_str());

    z.setChannel("Ch7.");
    z.averageFFT(0,0,"Entry$<500");
    hname = histname + "; SoF 2";
    h[1] = (TH1D*)z.hfft[z.kch]->Clone(hname.c_str());
}

void compare_ffts(){

    string path1k = "./C3/C3_Apr112024_5p4V_500Hz_allChannelsON_3/";
    string path4k = "./run18/C3_run18_2024425/";
    int nvgains = 9;
    vector<int> vgains(nvgains);
    vector<vector<TH1D*>> h1k(nvgains,vector<TH1D*>(2, nullptr));
    vector<vector<TH1D*>> h4k(nvgains,vector<TH1D*>(2, nullptr));
    
    for (int i = 0; i<nvgains; i++)
    {
        vgains[i] = i;
        string tmppath = Form("%sVGAIN0p%d/analyzed.root",path1k.c_str(),i);
        process_file(h1k[i], tmppath, Form("C3 1k - Vgain = 0.%d V", i));

        tmppath = Form("%sVGAIN0p%d/analyzed.root",path4k.c_str(),i);
        process_file(h1k[i], tmppath, Form("C3 4k - Vgain = 0.%d V", i));
    }



}
