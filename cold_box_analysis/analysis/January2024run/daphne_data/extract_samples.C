#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"

void extract_samples(string ch="Ch0.", Double_t spe_threshold = 10){
    ANALYZER s("s");
    s.setAnalyzer("analyzed.root");

    s.baselineparameters.baselineStart = 0;
    s.baselineparameters.baselineTime = 20000;
    s.baselineparameters.exclusion_baseline = spe_threshold;
    s.baselineparameters.exclusion_window = 3000;
    s.baselineparameters.filter = 4;
    s.baselineparameters.use_bits = false;

    s.setChannel(ch);

    s.extract_waveforms(0, 20000, "", true, Form("sample_%sroot",ch.c_str()), 30150, 50000);


}
