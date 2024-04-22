#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"



void gen_fft(vector<string> ch_input = {""}){

    vector<string> to_compute = ch_input;

    if (to_compute[0] == ""){
        to_compute = {
            "Ch0.", 
            "Ch7.", 
            "Ch10.",        
            "Ch17.",
            "Ch20.",
            "Ch27.",
            "Ch40.",
            "Ch47.",
            "Ch1.", 
            "Ch36."
        };
    }


    map<string, string> devices = {
        {"Ch0.", "C1 SoF1"},
        {"Ch7.", "C1 SoF2"},
        {"Ch10.", "C2 SoF1"},
        {"Ch17.", "C2 SoF2"},
        {"Ch20.", "C3 SoF1"},
        {"Ch27.", "C3 SoF2"},
        {"Ch40.", "C4 SoF1"},
        {"Ch47.", "C4 SoF2"},
        {"Ch1.", "HD"},
        {"Ch36.", "VD"},
    };

    map<string, Color_t> dcolor = {
        {"Ch0.",kRed},
        {"Ch7.",kRed+2},
        {"Ch10.",kYellow+1},
        {"Ch17.",kYellow+2},
        {"Ch20.",kGreen+1},
        {"Ch27.",kGreen+2},
        {"Ch40.",kBlue+1},
        {"Ch47.",kBlue+2},
        {"Ch1.",kCyan+1},
        {"Ch36.",kMagenta+1}
    };

    ANALYZER s("s");
    s.setAnalyzer("analyzed.root");

    THStack *hs = new THStack();

    string xlabel;
    string ylabel;
    for(auto channel: to_compute){
        s.setChannel(channel);
        s.averageFFT(100,0,"",true,17e3);
        auto mapIter = devices.find(channel);
        if (mapIter == devices.end()){
            cout << "WARNING!!! " << channel << " not found in device map... " << endl;
            return;
        }

        s.hfft[s.kch]->SetTitle(mapIter->second.c_str());
        auto mapIterc = dcolor.find(channel);
        s.hfft[s.kch]->SetLineColor(mapIterc->second);
        hs->Add(s.hfft[s.kch]);
        xlabel = s.hfft[s.kch]->GetXaxis()->GetTitle();
        ylabel = s.hfft[s.kch]->GetYaxis()->GetTitle();
        
    }

    TCanvas *c1 = new TCanvas("c1", "c1",1920,0,1920,1080);
    hs->Draw("nostack");
    hs->GetXaxis()->SetTitle(xlabel.c_str());
    hs->GetYaxis()->SetTitle(ylabel.c_str());
    c1->BuildLegend();
}
