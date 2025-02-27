// ________________________________________ //
// Author: Henrique Souza
// Filename: readingCodes.C
// Created: 2021
// ________________________________________ //
#include "MYCODES.h"
#include <arpa/inet.h>

class DENOISE{

public:
	template<class T>
	void TV1D_denoise(T* input, T* output, unsigned int width, const Double_t lambda) {

		unsigned int* indstart_low = (unsigned int*) malloc(sizeof *indstart_low * width);
		unsigned int* indstart_up = (unsigned int*) malloc(sizeof *indstart_up * width);
		unsigned int j_low = 0, j_up = 0, jseg = 0, indjseg = 0, i=1, indjseg2, ind;
		double output_low_first = input[0]-lambda;
		double output_low_curr = output_low_first;
		double output_up_first = input[0]+lambda;
		double output_up_curr = output_up_first;
		const double twolambda=2.0*lambda;
		if (width==1) {output[0]=input[0]; return;}
		indstart_low[0] = 0;
		indstart_up[0] = 0;
		width--;
		for (; i<width; i++) {
			if (input[i]>=output_low_curr) {
				if (input[i]<=output_up_curr) {
					output_up_curr+=(input[i]-output_up_curr)/(i-indstart_up[j_up]+1);
					output[indjseg]=output_up_first;
					while ((j_up>jseg)&&(output_up_curr<=output[ind=indstart_up[j_up-1]]))
						output_up_curr+=(output[ind]-output_up_curr)*
							((double)(indstart_up[j_up--]-ind)/(i-ind+1));
					if (j_up==jseg) {
						while ((output_up_curr<=output_low_first)&&(jseg<j_low)) {
							indjseg2=indstart_low[++jseg];
							output_up_curr+=(output_up_curr-output_low_first)*
								((double)(indjseg2-indjseg)/(i-indjseg2+1));
							while (indjseg<indjseg2) output[indjseg++]=output_low_first;
							output_low_first=output[indjseg];
						}
						output_up_first=output_up_curr;
						indstart_up[j_up=jseg]=indjseg;
					} else output[indstart_up[j_up]]=output_up_curr;
				} else
				output_up_curr=output[i]=input[indstart_up[++j_up]=i];
				output_low_curr+=(input[i]-output_low_curr)/(i-indstart_low[j_low]+1);
				output[indjseg]=output_low_first;
				while ((j_low>jseg)&&(output_low_curr>=output[ind=indstart_low[j_low-1]]))
					output_low_curr+=(output[ind]-output_low_curr)*
						((double)(indstart_low[j_low--]-ind)/(i-ind+1));
				if (j_low==jseg) {
					while ((output_low_curr>=output_up_first)&&(jseg<j_up)) {
						indjseg2=indstart_up[++jseg];
						output_low_curr+=(output_low_curr-output_up_first)*
							((double)(indjseg2-indjseg)/(i-indjseg2+1));
						while (indjseg<indjseg2) output[indjseg++]=output_up_first;
						output_up_first=output[indjseg];
					}
					if ((indstart_low[j_low=jseg]=indjseg)==i) output_low_first=output_up_first-twolambda;
					else output_low_first=output_low_curr;
				} else output[indstart_low[j_low]]=output_low_curr;
			} else {
				output_up_curr+=((output_low_curr=output[i]=input[indstart_low[++j_low] = i])-
					output_up_curr)/(i-indstart_up[j_up]+1);
				output[indjseg]=output_up_first;
				while ((j_up>jseg)&&(output_up_curr<=output[ind=indstart_up[j_up-1]]))
					output_up_curr+=(output[ind]-output_up_curr)*
						((double)(indstart_up[j_up--]-ind)/(i-ind+1));
				if (j_up==jseg) {
					while ((output_up_curr<=output_low_first)&&(jseg<j_low)) {
						indjseg2=indstart_low[++jseg];
						output_up_curr+=(output_up_curr-output_low_first)*
							((double)(indjseg2-indjseg)/(i-indjseg2+1));
						while (indjseg<indjseg2) output[indjseg++]=output_low_first;
						output_low_first=output[indjseg];
					}
					if ((indstart_up[j_up=jseg]=indjseg)==i) output_up_first=output_low_first+twolambda;
					else output_up_first=output_up_curr;
				} else output[indstart_up[j_up]]=output_up_curr;
			}
		}
		/* here i==width (with value the actual width minus one) */
		if (input[i]+lambda<=output_low_curr) {
			while (jseg<j_low) {
				indjseg2=indstart_low[++jseg];
				while (indjseg<indjseg2) output[indjseg++]=output_low_first;
				output_low_first=output[indjseg];
			}
			while (indjseg<i) output[indjseg++]=output_low_first;
			output[indjseg]=input[i]+lambda;
		} else if (input[i]-lambda>=output_up_curr) {
			while (jseg<j_up) {
				indjseg2=indstart_up[++jseg];
				while (indjseg<indjseg2) output[indjseg++]=output_up_first;
				output_up_first=output[indjseg];
			}
			while (indjseg<i) output[indjseg++]=output_up_first;
			output[indjseg]=input[i]-lambda;
		} else {
			output_low_curr+=(input[i]+lambda-output_low_curr)/(i-indstart_low[j_low]+1);
			output[indjseg]=output_low_first;
			while ((j_low>jseg)&&(output_low_curr>=output[ind=indstart_low[j_low-1]]))
				output_low_curr+=(output[ind]-output_low_curr)*
					((double)(indstart_low[j_low--]-ind)/(i-ind+1));
			if (j_low==jseg) {
				if (output_up_first>=output_low_curr)
					while (indjseg<=i) output[indjseg++]=output_low_curr;
					else {
						output_up_curr+=(input[i]-lambda-output_up_curr)/(i-indstart_up[j_up]+1);
						output[indjseg]=output_up_first;
						while ((j_up>jseg)&&(output_up_curr<=output[ind=indstart_up[j_up-1]]))
							output_up_curr+=(output[ind]-output_up_curr)*
								((double)(indstart_up[j_up--]-ind)/(i-ind+1));
						while (jseg<j_up) {
							indjseg2=indstart_up[++jseg];
							while (indjseg<indjseg2) output[indjseg++]=output_up_first;
							output_up_first=output[indjseg];
						}
						indjseg=indstart_up[j_up];
						while (indjseg<=i) output[indjseg++]=output_up_curr;
					}
			} else {
				while (jseg<j_low) {
					indjseg2=indstart_low[++jseg];
					while (indjseg<indjseg2) output[indjseg++]=output_low_first;
					output_low_first=output[indjseg];
				}
				indjseg=indstart_low[j_low];
				while (indjseg<=i) output[indjseg++]=output_low_curr;
			}
		}
		free(indstart_low);
		free(indstart_up);
	}


	template<class T>
	void cumulativeAverage(T* v, T* res, Int_t myinte = 10, Double_t start = 0, Double_t finish = 0, Bool_t eco = false){

		Double_t sum = 0;
		Double_t avgi = 0;
		Int_t count = 0;
		Double_t normfactor = myinte+1;
		if (eco){
			normfactor = pow(2., myinte); // each point contributes much less (gain some spaces)
		}


		for(Int_t i = 0; i < finish; i++){

			if(i<start){ // make it to start at i = 5
				res[i] = v[i];
			}
			else{
				count += 1;
				if (count > myinte){
					res[i] = avgi + (v[i] - avgi)/normfactor;
					avgi = res[i];
				}
				else if( count == myinte ){
					avgi = sum/myinte;
					res[i] = v[i];
				}
				else{
					res[i] = v[i];
					sum+=v[i];
				}
			}
		}

	}


	template<class T>
	void movingAverage(T* v, T* res, Int_t myinte = 10, Double_t start = 0, Double_t finish = 0){

		Int_t n = finish;
		Int_t midpoint = 0;
		Int_t width = 0;
		Double_t sum = 0;

		if(myinte%2==0){ // if it is even, we insert the middle point, e.g. 8 interactions takes 4 before, mid, 4 later
			myinte+=1;
		}
		midpoint = (myinte-1)/2; // e.g. 9 interactions the midpoint will be 5
		width = myinte;


		for(Int_t i = 0; i < n; i++){

			if(i<midpoint || i>(n-(midpoint+1))){ // make it to start at i = 5 and finish at i = (3000-5) = 2995
				// res[i] = v[i];
				res[i] = 0;
			}
			else if (i > start && i < finish){
				for(Int_t j = (i-midpoint); j < (i+midpoint+1); j++) { //first one: from j = (5-5); j<(5+5)
					sum = sum+v[j];

					//                 cout << sum << endl;
				}
				res[i] = (sum/width);
			}
			else{
				res[i] = 0;
			}
			sum=0;
		}

	}
};








class HeadersProtoDUNE{

public:

	uint32_t EventSize;
	uint64_t TriggerTimeTag;
};


class Headers{

public:

	int EventSize;
	int Board_id;
	int Pattern;
	int Channel;
	int EventCounter;
	uint TriggerTimeTag;
};

class Read{

private:
	int file_size = 0;
	vector<int> expected_wvfs;
public:



	// Created by Henrique Souza, June 2020

	Int_t n_points = memorydepth;

	Double_t dtime = 4; // steps (ADC's MS/s, 500 MS/s = 2 ns steps)
	Int_t nbits = 14;
	Int_t basebits = nbits;
	Bool_t isBinary         = false;
	Bool_t saveFilter       = false;
	Bool_t with_headers     = true;
	Bool_t withTimestamp    = true;
	Bool_t withRelativeTime = true;
	Bool_t isCAEN           = true;


	Double_t startCharge = 3300;
	Double_t maxRange = 5000;
	Double_t fast = 500;
	Double_t slow = 17000;
	Double_t filter = 9;
	Double_t exclusion_baseline = 30;
	Double_t exclusion_window = 500;
	Double_t currentTime = 0;
	Double_t timeCicle = TMath::Power(2,31)-1;
	Double_t timeflip = TMath::Power(2,32)-1;
	Double_t timeResolution = 8e-9; // 8 ns for 2 ns step, 16 ns for 4 ns step
	Double_t timestamp = 0;
	Double_t timestamp2 = 0;
	Double_t deltastamp = 0;
	Double_t temptime = 0;
	Bool_t bitoverflow = false;

	string file_extension = ".txt";

	Bool_t OnlySomeEvents = false; // Do you want only one event? Choose it wisely
	Int_t stopEvent = 2000;

	Int_t maxEvents = 100000;

	Double_t baselineTime = 10000; // time limit to start looking for baseline
	Double_t baselineStart = 0;
	Double_t chargeTime = 18000; // last time to integrate
	Bool_t noBaseline=false;
	Double_t baselineFraction = 1/3.;

	Double_t nbinsbase = TMath::Power(2,basebits);
	Double_t minbase = 0;
	Double_t maxbase = TMath::Power(2,basebits);
	string standard_log_file = "files.log";



	Double_t polarity=1; // set -1 invert to negative pulses
	// Bool_t noBaseline=true;
	vector<Int_t> channels = {1,2};
	vector<Double_t> exclusion_baselines = {};
	map<string, Double_t> map_exclusion_threshold_baselines = {{"none",0}};

	string reference_name_before_ch = "wave";

	Int_t nfiles = 1;

	// THIS did not work properly, sometimes it gets stuck in a bad region.
	// TH1D *hbase = new TH1D("hbase","finding baseline",TMath::Power(2,nbits),0,0);

	TH1D *htests = new TH1D("htests","htests",1000,0,0);
	TH1D *hbase;
	TF1* fbase = new TF1("fbase","gaus(0)",0,TMath::Power(2,nbits));
	Double_t max_bits = 0;


	vector<Int_t> navg;
	vector<vector<Double_t>> avg;
	vector<Double_t> time;



	string format_time = "hh:mm:ss";
	string format_date = "dd-mmm-yyyy";

	void reset_double_vector(vector<vector<Double_t>> &val){
		Int_t nchannels = (int)channels.size();
		Int_t noriginal = (int)val.size();
		if (noriginal > nchannels){
			if (noriginal-1 >= channels[nchannels-1]){
				for (Int_t j = 0; j < nchannels; j++) {
					val[j] =  val[channels[j]];
				}
				val.resize(nchannels);
			}
		}
		else if (noriginal < nchannels && noriginal != 1) {
			val.resize(nchannels);
			for(Int_t j = noriginal; j < (int)val.size(); j++){
				val[j] = val[0];
			}
		}
	}
	void reset_vector(vector<Double_t> &val){
		Int_t nchannels = (int)channels.size();
		Int_t noriginal = (int)val.size();
		vector<Double_t> tmp(val);
		if (noriginal > nchannels){
			if (noriginal-1 >= channels[nchannels-1]){
				for (Int_t j = 0; j < nchannels; j++) {
					// cout << val[j]  << " " << j << " " << val[channels[j]] << " " << channels[j] << endl;
					val[j] =  val[channels[j]];
				}
				val.resize(nchannels);
			}
		}
		else if (noriginal < nchannels && noriginal != 1) {
			val.resize(nchannels);
			for(Int_t j = noriginal; j < (int)val.size(); j++){
				val[j] = val[0];
			}
		}
		// for (Int_t j = 0; j < nchannels; j++) cout << j << " " << channels[j] << " " <<  exclusion_baselines[j] << endl;
	}


	void setup_baseline_thresholds(){
		Int_t nchannels = (int)channels.size();
		auto mapIter = map_exclusion_threshold_baselines.find("none");

		if (exclusion_baselines.size() == 0){
			exclusion_baselines.push_back(exclusion_baseline);
		}
		if (mapIter != map_exclusion_threshold_baselines.end()){ // in case the map was not set
			map_exclusion_threshold_baselines.erase("none");
			for (Int_t i = 0; i < (int)exclusion_baselines.size(); i++){
				map_exclusion_threshold_baselines[Form("Ch%d.", i)] = exclusion_baselines[i];
			}
		}
		Double_t refval = exclusion_baselines[0];
		exclusion_baselines.clear();
		exclusion_baselines.resize(nchannels);
		for(Int_t i = 0; i < nchannels; i++){
			mapIter = map_exclusion_threshold_baselines.find(Form("Ch%d.",channels[i]));
			if (mapIter == map_exclusion_threshold_baselines.end()){
				cout << "WARNING!!! " << channels[i] << " not found in baseline map.. using: " << refval << endl;
				exclusion_baselines[i] = refval;
			}
			else{
				exclusion_baselines[i] = mapIter->second;
			}
		}

	}
	void get_ch_info(string logfilename){
		ifstream logfile;
		logfile.open(logfilename.c_str(),ios::in);

		if(logfile.good() && logfile.is_open()){ // Ok
			// cout << "Reading file " << files << " ... " << endl;
		}
		else{
			cout << "The file " << logfilename << " did not open!!" << endl;
			return;
		}
		string dataname = "";
		Int_t aux = 0;
		// While for read the entire file
		string wave_ref = reference_name_before_ch;
		int wavenum = 0;
		int temp = -1;
		vector<Int_t> tempch;
		while(!logfile.fail()){
			logfile >> dataname;
			// cout << dataname << endl;
			if(logfile.bad() || logfile.fail()){
				break;
			}
			int found  = dataname.find(wave_ref);

			if(found==-1){
				tempch.push_back(aux);
			}
			else
		{
				found = found + wave_ref.length();
				string subdataname = dataname.substr(found);
				int afterPos = subdataname.find("_");
				if (afterPos == -1){
					afterPos = subdataname.find(".");
				}
				std::string channelNumberString = afterPos != -1 ? subdataname.substr(0, afterPos) : subdataname;
				// wavenum = (int)dataname[found] - '0';
				try {
					wavenum = std::stoi(channelNumberString);
				} catch (std::invalid_argument const& e) {
					std::cerr << "Invalid channel number format in file: " << dataname << " with substring " << subdataname << std::endl;
				}
				if(wavenum != temp){
					if (aux == 0) temp = wavenum;
					tempch.push_back(wavenum);
				}
				else{
					break;
				}
			}
			aux++;
		}
		channels = tempch;
		setup_baseline_thresholds();
		logfile.close();
		expected_wvfs.resize(channels.size());
	}

	void adc_read_all_data(Bool_t do_get_ch_info = true){
		max_bits = TMath::Power(2,nbits);

		if (maxRange < startCharge) maxRange = startCharge + startCharge;
		if (do_get_ch_info) get_ch_info(standard_log_file);
		readFiles(standard_log_file); //use it like this
		return;

	}

	void adc_read(string filename){

		readFile(filename);
	}

	void readFile(string file){
		if(channels.size()>1){
			channels = {1};
		}
		vector<string> file_ch(channels.size());
		string rootfile;
		file_ch[0] = file;

		rootfile = file + ".root";
		TFile *f1;
		TTree *t1;
		Double_t tEvent = 0;
		vector<ADC_DATA> ch(channels.size());
		vector<TBranch*> bch(channels.size());
		avg.resize(channels.size());
		navg.resize(channels.size(),0);


		f1 = new TFile(rootfile.c_str(),"RECREATE");
		t1 = new TTree("t1","ADC processed waveform");
		for(Int_t i = 0; i< (int)channels.size(); i++){
			// bch[i] = t1->Branch(Form("Ch%i",channels[i]),&ch[i],ch[0]->tobranch.c_str());
			bch[i] = t1->Branch(Form("Ch%i.",channels[i]),&ch[i]);
		}


		f1->Write();

		f1->Close();

		// _______________________________________________________________________________________________________

		readData(file_ch,rootfile,tEvent);

		time.resize(n_points);
		for(Int_t i = 0; i<n_points; i++){
			time[i] = i*dtime;
		}
		for(Int_t i = 0; i < (int)channels.size(); i++){
			for(Int_t j = 0; j < n_points; j++){
				avg[i][j] = avg[i][j]/navg[i];
			}
		}

		f1 = new TFile(rootfile.c_str(),"UPDATE");
		t1 = new TTree("t1","ADC processed waveform");
		for(Int_t i = 0; i < (int)channels.size(); i++){
			bch[i] = t1->Branch(Form("Ch%i.",channels[i]),&ch[i]);
		}
		vector<TGraph*> gavg(channels.size());
		for(Int_t i = 0; i < (int)channels.size(); i++){
			gavg[i] = new TGraph(n_points,&time[0],&avg[i][0]);
			f1->WriteObject(gavg[i],Form("average_ch%i",channels[i]),"TObject::kOverwrite");
		}
		//     f1->Close();


	}

	void readFiles(string files){

		ifstream filesdata;
		filesdata.open(files.c_str(),ios::in);
		vector<string> file_ch(channels.size());
		string rootfile;
		TFile *f1;
		TTree *t1;
		TTree *thead;
		hbase = new TH1D("hbase","finding baseline",TMath::Power(2,basebits),0,TMath::Power(2,nbits));
		Double_t tEvent = 0;
		vector<ADC_DATA> ch(channels.size());
		vector<TBranch*> bch(channels.size());


		Bool_t first_file = true;
		avg.resize(channels.size());
		navg.resize(channels.size(),0);

		if(isBinary) file_extension = ".dat";

		if(filesdata.good() && filesdata.is_open()){ // Ok
			cout << "Reading file " << files << " ... " << endl;
		}
		else{
			cout << "The file " << files << " did not open!!" << endl;
			return;
		}
		string temp = "";
		// While for read the entire file
		while(!filesdata.fail()){
			for(Int_t i = 0; i < (int)channels.size(); i++){
				filesdata >> temp;
				file_ch[i] = temp;
			}
			// cout << "C1 = " << file_ch[0] << " C2 = " << file_ch[1] << endl;
			if(filesdata.bad() || filesdata.fail()){
				break;
			}

			// ______________________________ Create root files at first files only __________________________________
			if(first_file){
				first_file = false;

				rootfile = "analyzed.root";

				string erase = "rm " + rootfile;
				system(erase.c_str());

				f1 = new TFile(rootfile.c_str(),"RECREATE");
				t1 = new TTree("t1","ADC processed waveform");
				for(Int_t i = 0; i < (int)channels.size(); i++){
					bch[i] = t1->Branch(Form("Ch%i.",channels[i]),&ch[i]);
				}

				thead = new TTree("head","header");
				thead->Branch("dtime", &dtime);
				thead->Branch("startcharge", &startCharge);
				thead->Branch("endcharge", &chargeTime);
				thead->Branch("baselineStart", &baselineStart);
				thead->Branch("baselineTime", &baselineTime);
				thead->Branch("maxRange", &maxRange);
				thead->Branch("fast_time", &fast);
				thead->Branch("slow_time", &slow);
				thead->Branch("exclusion_baselines", &exclusion_baselines);
				thead->Branch("exclusion_window", &exclusion_window);
				thead->Branch("filter", &filter);
				thead->Fill();

				f1->Write();

				f1->Close();
			}
			// _______________________________________________________________________________________________________

			readData(file_ch,rootfile,tEvent);

		}
		time.resize(n_points);
		for(Int_t i = 0; i<n_points; i++){
			time[i] = i*dtime;
		}
		for(Int_t i = 0; i < (int)channels.size(); i++){
			for(Int_t j = 0; j<n_points; j++){
				avg[i][j] = avg[i][j]/navg[i];
			}
		}

		f1 = new TFile(rootfile.c_str(),"UPDATE");
		t1 = new TTree("t1","ADC processed waveform");
		for(Int_t i = 0; i < (int)channels.size(); i++){
			bch[i] = t1->Branch(Form("Ch%i.",channels[i]),&ch[i]);
		}
		vector<TGraph*> gavg(channels.size());
		for(Int_t i = 0; i < (int)channels.size(); i++){
			gavg[i] = new TGraph(n_points,&time[0],&avg[i][0]);
			f1->WriteObject(gavg[i],Form("average_ch%i",channels[i]),"TObject::kOverwrite");
		}
		//     f1->Close();

		// htests->Draw();

	}






	// This function will read your data and create a root file with the same name
	void readData(vector<string> file_ch,string rootfile, Double_t &tEvent){

		TFile *f1 = new TFile(rootfile.c_str(),"UPDATE");
		TTree *t1 = (TTree*)f1->Get("t1");


		vector<ADC_DATA*> ch(channels.size());
		vector<TBranch*> bch(channels.size());
		vector<string> filename_ch(channels.size());
		TimeREADER myTimer;

		DENOISE dn;


		// if(dtime == 2)  timeResolution = 8e-9; // 8 ns for 2 ns step, 16 ns for 4 ns step
		// if(dtime == 4)  timeResolution = 16e-9; // 16 ns for 4 ns step

		Int_t numberoflines=0;
		Double_t temp;

		for(Int_t i = 0; i < (int)channels.size(); i++){
			bch[i] = t1->GetBranch(Form("Ch%i.",channels[i]));
			bch[i]->SetAddress(&ch[i]);
			filename_ch[i] = file_ch[i] + file_extension;
		}



		vector<ifstream> fin(channels.size());
		for(Int_t i = 0; i < (int)channels.size(); i++){
			if(!isBinary) fin[i].open(filename_ch[i].c_str(),ios::in);
			else          fin[i].open(filename_ch[i].c_str(),ios::in | ios::binary);
			// here i check if the data file is open (or exist), if not, I remove the .root files and close the program.
			if(fin[i].good() && fin[i].is_open()){ // Ok
				cout << "Reading file " << filename_ch[i] << " ... " << endl;
			}
			else{
				cout << "File " << filename_ch[i] << " did not open!!" << endl;
				return;

			}
		}
		Bool_t closeMyWhile = false; // keep it false
		// While for read the entire file
		Double_t bl = 0;
		Int_t eventFile = 0;
		string headers;
		Double_t init_time = 0;
		uint16_t valbin = 0;
		Headers headbin;
		HeadersProtoDUNE headbinP;
		int nbytes = 4;
		Int_t headers_npoints = 0;
		Int_t headers_nwvfs = 0;

		string date, time;
		Double_t stamp;
		vector<Double_t> event_time;
		Double_t starting_time = 0;
		Int_t aux_time = 0;
		for(Int_t i = 0; i < (int)channels.size(); i++){
			if(isBinary==false){
				if (with_headers)
				{
					// LECROYWR104MXi-A/� 49455 Waveform
					// Segments 2000 SegmentSize 2502
					// Segment TrigTime TimeSinceSegment1
					// #1 01-Jan-2002 00:32:41 0
					// #2 01-Jan-2002 00:32:41 0.0001
					// continue;
					getline(fin[i],headers);
					// cout << headers << endl;
					fin[i] >> headers >> headers_nwvfs >> headers >> headers_npoints;
					// cout << headers << endl;
					getline(fin[i],headers); // taking extra \r


					if(withTimestamp){
						getline(fin[i],headers); // reads the header of time stamp
						for(Int_t ln=0;ln<headers_nwvfs;ln++){
							fin[i] >> headers >> date >> time >> stamp;
							// cout << ln <<  " " << headers_nwvfs << " " <<  headers << " " << date << " " <<  time << endl;
							if(ln==0){
								// cout << date << " " << time << endl;
								starting_time = myTimer.timeRead(date,time,format_date,format_time);
								event_time[ln] = starting_time;
								// printf("Starting time = %1.f\n",starting_time);
							}
							else{
								starting_time+=stamp;
								event_time[ln] = starting_time;
							}
						}
						getline(fin[i],headers); // reads the extra \r
					}

					getline(fin[i],headers);
					// cout << headers << endl;
				}
			}
		}


		if(isBinary){
			Int_t tmpnpoints = 0;
			for(Int_t i = 0; i < (int)channels.size(); i++){
				Double_t evtsize = 0;
				if(with_headers){
					if (isCAEN){
						fin[i].read((char *) &headbin, nbytes*6);
						tmpnpoints = (headbin.EventSize-24)/2;
						if (tmpnpoints != 0){
							n_points = tmpnpoints;
						}
						else if(tmpnpoints != n_points){
							cout << "WARNING !!! different waveform length in files!!!" << endl;
						}
						evtsize = headbin.EventSize;
					}
					else{
						fin[i].read((char *) &headbinP.EventSize, 4);
						fin[i].read((char *) &headbinP.TriggerTimeTag, 8);
						tmpnpoints = headbinP.EventSize;
						if (tmpnpoints != 0){
							n_points = tmpnpoints;
						}
						else if(tmpnpoints != n_points){
							cout << "WARNING !!! different waveform length in files!!!" << endl;
						}
						evtsize = headbinP.EventSize*2+4+8;
					}
				}
				else{
					evtsize = n_points*2;
				}

				// Get file size in bytes
				fin[i].seekg(0, ios::end);
				file_size = fin[i].tellg();

				expected_wvfs[i] = file_size / evtsize;

				fin[i].clear();
				fin[i].seekg(0);
			}
		}
		vector<int> sorted_expected_wvfs = expected_wvfs;
		std::sort(sorted_expected_wvfs.begin(), sorted_expected_wvfs.end());
		int ref_expected_wvfs = sorted_expected_wvfs[0];
		bool sendwarning = false;
		Int_t tmpstopEvent = stopEvent;
		Bool_t forcestop = false;
		for(Int_t i = 0; i < (int)channels.size(); i++){
			avg[i].resize(n_points,0);
			ch[i]->Set_npts(n_points); // gain a few ns
			if (expected_wvfs[i] != ref_expected_wvfs){
				sendwarning = true;
				forcestop = true;
				stopEvent = ref_expected_wvfs;
			}
		}
		if (forcestop){
			if (OnlySomeEvents){
				stopEvent = (stopEvent > tmpstopEvent) ? tmpstopEvent : stopEvent;
			}
			else{
				OnlySomeEvents = true;
			}
		}
		if (sendwarning)
			cout << "Different number of waveforms... setting maximum at " << stopEvent << endl;

		if(baselineTime > n_points*dtime || baselineTime == 0){
			baselineTime = n_points*dtime;
		}

		vector<Double_t> raw(n_points);
		Double_t *filtered = new Double_t[n_points];
		event_time.resize(n_points);
		while(!fin[0].fail() && closeMyWhile == false){ // We can just look for one file, they shold have the same amount of lines anyway!
			Int_t n_reads = 0;

			Int_t tempsize = n_points;
			for(Int_t i = 0; i < (int)channels.size(); i++){
				if(isBinary==false){

					for(int j = 0; j < n_points; j++)
					{
						if(withTimestamp) fin[i] >> timestamp >> temp;
						else fin[i] >> temp;
						// cout << timestamp << " " << temp << endl;
						if(fin[i].bad() || fin[i].fail()){
							cout << "goodbye... ";
							break;
						}
						n_reads++;
						if(polarity==-1){
							temp = max_bits - temp;
						}
						raw[j] = temp;
						ch[i]->wvf[j] = temp;
						filtered[j] = temp;
					}
					//           cout << "............................ \n";
					//           getline(fin[i],headers);
				}
				else{
					if(with_headers){
						if (isCAEN){
							fin[i].read((char *) &headbin, nbytes*6);
							timestamp = headbin.TriggerTimeTag;
							n_points = (headbin.EventSize-24)/2;
							if(n_points != tempsize){
								ch[i]->Set_npts(n_points);
								avg[i].resize(n_points);
								raw.resize(n_points);
								delete[] filtered;
								filtered = new Double_t[n_points];
							}
						}
						else{
							fin[i].read((char *) &headbinP.EventSize, 4);
							fin[i].read((char *) &headbinP.TriggerTimeTag, 8);
							timestamp = headbinP.TriggerTimeTag;

							n_points = headbinP.EventSize;
							if(n_points != tempsize){
								ch[i]->Set_npts(n_points);
								avg[i].resize(n_points);
								raw.resize(n_points);
								delete[] filtered;
								filtered = new Double_t[n_points];
							}

						}
					}

					//           printf("%.0f\n",timestamp);
					for(int j = 0; j < n_points; j++)
					{
						fin[i].read((char *) &valbin, 2);
						// valbin = be16toh(valbin); // if big ending was used, this converts to small ending
						//             if(j==0) printf("%d\n -- \n",valbin);

						if(fin[i].bad() || fin[i].fail()){
							break;
						}
						// cout << valbin << endl;
						if(polarity==-1){
							valbin = max_bits - valbin;
						}
						raw[j] = valbin;
						ch[i]->wvf[j] = valbin;
						filtered[j] = valbin;
					}
				}

				if((fin[i].bad() || fin[i].fail()) && n_reads<n_points-5){
					// cout << "problems ??" << endl;
					break; // giving a 5 points relaxiation
				}
				if (withRelativeTime){
					if(i==0){

						if(timestamp<temptime){
							deltastamp = (timeflip - temptime) + (timestamp-timeCicle); // same as 2^31 + temptime + timestamp
						}
						else{
							deltastamp = timestamp-temptime;
						}

						temptime = timestamp;

						if(init_time!=0 && eventFile<maxEvents){
							currentTime = currentTime+deltastamp*timeResolution;
						}
						else{
							init_time = 1;
						}
						ch[i]->time = currentTime;
					}
					else{
						ch[i]->time = ch[0]->time;

					}
				}
				else{
					ch[i]->time = timestamp;
				}

				// if(!isBinary) ch[i]->time = event_time[aux_time];
				// printf("time of event = %11f\n",event_time[aux_time]);
				aux_time++;
				if(filter>0) dn.TV1D_denoise<Double_t>(&ch[i]->wvf[0],&filtered[0],n_points,filter);
				// if(filter>0) dn.TV1D_denoise<Double_t>(&raw[0],&ch[i]->wvf[0],n_points,filter);
				if(saveFilter==true){
					for(Int_t l = 0; l<n_points; l++){
						ch[i]->wvf[l] = filtered[l];
					}
				}

				exclusion_baseline = exclusion_baselines[i];

				// cout << "kch = " << i << " " << channels[i] << " baseline = " << exclusion_baseline << endl;
				bl = baseline(filtered,ch[i]->selection,i,tEvent);
				// bl = baseline(ch[i]->wvf,ch[i]->selection,i,tEvent);
				// if(bl==-9999) cout << i << " " << tEvent << endl;

				ch[i]->base = bl;
				getvalues(i,*ch[i],filtered,bl);
				ch[i]->event = tEvent;

				numberoflines++;


			}
			if((fin[0].bad() || fin[0].fail()) && n_reads<n_points-5){
				// cout << "problems ??" << endl;
				break; // giving a 5 points relaxiation
			}
			// if(fin[0].eof()){
			// numberoflines--;
			// break;
			// }
			if(eventFile<maxEvents){
				t1->Fill();
				tEvent+=1;

				if(static_cast<Int_t>(tEvent)%200==0) {
					cout << "Events: " << tEvent << " out of " << ref_expected_wvfs << "\r" << flush;
				}
			}

			if(OnlySomeEvents == true && eventFile==stopEvent-1){
				closeMyWhile=true;
			}
			eventFile++;

		}
		cout << "\n";
		cout << tEvent << " events in " << numberoflines << " lines " << endl;

		f1->WriteObject(t1,"t1","TObject::kOverwrite");

		f1->Close();

	}



	void getvalues(Int_t &nch,ADC_DATA &ch,Double_t filtered[],Double_t bl){

		ch.peak =0;
		Double_t fastcomp = 0;
		Double_t slowcomp = 0;
		ch.charge=0;
		navg[nch]++;
		for(Int_t i = 0; i<n_points; i++){
			ch.wvf[i] = ch.wvf[i]-bl;
			filtered[i] = filtered[i]-bl;
			avg[nch][i]+=ch.wvf[i];
			//       cout << i << " " << ch.wvf[i] << endl;
			if(i>=startCharge/dtime && i<chargeTime/dtime){
				ch.charge+=filtered[i]*dtime;
				if(i <= maxRange/dtime){
					if(ch.peak==0){ ch.peak = filtered[i]; ch.peakpos = i*dtime;}
					else if(ch.peak<filtered[i]){ch.peak=filtered[i];ch.peakpos = i*dtime;}
				}
				if(i<(startCharge+fast)/dtime){
					fastcomp+=ch.wvf[i];
				}
				if(i<(startCharge+slow)/dtime){
					slowcomp+=ch.wvf[i];
				}

			}
		}
		//     cout << fastcomp << " " << slowcomp << endl;
		ch.fprompt = fastcomp/slowcomp;
		if (!isCAEN) ch.fprompt = timestamp2;
	}


	Double_t baseline(Double_t v[],Int_t &selection, Int_t idx, Int_t mevent){
		if(noBaseline) return 0;
		Double_t result = 0;
		hbase->Reset();
		for(Int_t i=baselineStart/dtime; i<baselineTime/dtime; i++) hbase->Fill(v[i]);
		Double_t res0 = hbase->GetBinCenter(hbase->GetMaximumBin());
		Double_t hmean = hbase->GetMean();
		Double_t hstd = hbase->GetStdDev();

		// fbase->SetParameters(1000,res0,hstd);
		// hbase->Print();

		// // to debug ! Note, fitting the baseline makes its about 10 times slower..

		Bool_t changed_mean = false;
		// if(hstd/hmean>0.1) hstd = 0.05*hmean; // in the case the standard deviation is bigger then X%, we correct it
		// if(res0>hmean+hstd || res0<hmean-hstd){ // res0 is way too out of the mean, so:

		// TFitResultPtr r = hbase->Fit("fbase","WQ0");
		//   Int_t fitStatus = r;
		//   // if(fitStatus==0){
		//     hmean = fbase->GetParameter(1);
		//     hstd = fbase->GetParameter(2);
		//     res0 = hmean;
		//     changed_mean = true;
		//   // }
		// }

		// if(idx == 0 && mevent == 3294){
		// cout << hmean << " " << hstd << " " << res0 << endl;
		// }

		Double_t bins=0;
		for(Int_t i=baselineStart/dtime; i<baselineTime/dtime;){
			if(v[i] > res0 + exclusion_baseline || v[i]<res0 - exclusion_baseline) {
				i+=exclusion_window/dtime;
			}
			else{
				result += v[i];
				bins+=1;
				i++;
			}
		}
		if(bins>0)result/=bins;
		if(bins > ((baselineTime - baselineStart)/dtime)*baselineFraction){
			selection = 0;
			// // You can use this to debug. If selection == 0, there should not have events here.
			// // this means that res0 should be pretty much the average for a good baseline.
			// if(res0>hmean+hstd || res0<hmean-hstd){
			//   cout << "PROBLEEEMMM " << endl;
			//   cout << result << endl;
			//     cout << res0 << endl;
			//     cout << hmean << endl;
			//     cout << hstd << endl;
			//     // cout << fitStatus << endl;
			//     result = -9999;
			// }


			return result;
		}
		else{
			// cout << result << " " << res0 << endl;
			// htests->Fill(hstd/hmean);
			// if(hstd/hmean<0.4) cout << idx << " " << mevent << endl;
			if(changed_mean==false)selection = 1;
			else selection = 2;
			return res0;
		}
	}
};






















































