// #include "/eos/home-h/hvieirad/cold_box_analysis/apc_root/cold_box_analysis/class/MYCODES.h"
#include "/home/henrique/Dropbox/APC_Paris/Root/cold_box_analysis/class/MYCODES.h"

struct SumID{
  uint32_t sum;
  Int_t idx;

  bool operator==(const SumID &a) const
  {
    return a.sum == sum;
  }

};


// FNV-1a 32-bit hash function
uint32_t fnv1aHash(const Double_t *array, int n) {
    const uint32_t FNV_prime = 16777619u;
    const uint32_t offset_basis = 2166136261u;
    uint32_t hash = offset_basis;

    for (int i=0; i < n; i++) {
      int value = static_cast<int>(array[i]);
      hash ^= static_cast<uint32_t>(value);
      hash *= FNV_prime;
    }

    return hash;
}

vector<Int_t> control_duplicates(vector<Int_t> &idx_search, ANALYZER *z = nullptr, Int_t ch = 1225) {
  if (z == nullptr)
    z = new ANALYZER("z");
    z->setAnalyzer("./analyzed.root");

  Double_t duplicates=0.;
  bool chok = false;
  chok = z->setChannel(Form("Ch%d.", ch));
  if (!chok) return {};
  Int_t npoints = z->n_points;
  Double_t dtime = z->dtime;;
  Int_t kch = z->kch;
  kch = z->kch;

  size_t nentries = idx_search.size();
  if (nentries==0){
    nentries = z->nentries;
    idx_search.resize(nentries);
    for(Int_t i = 0; i < nentries; i++){
      idx_search[i] = i;
    }
  }

  std::vector<SumID> sumid(nentries);
  cout << "Finding sum..." << endl;
  for(Int_t i = 0; i < nentries; i++){
    z->getWaveform(idx_search[i]);
    sumid[i].sum = 0;
    sumid[i].idx = idx_search[i];
    // for (int k = 0; k < npoints; ++k){
    sumid[i].sum = fnv1aHash(z->ch[kch]->wvf, npoints);
    // for (int k = 0; k < npoints; ++k){
    //   sumid[i].sum += abs(static_cast<int>(z->ch[kch]->wvf[k]));
    // }
  }
  sort(sumid.begin(), sumid.end(), [](const SumID &a, const SumID &b){ return a.sum > b.sum; });
  for(Int_t i = 0; i < sumid.size()-1; i++){
    if (sumid[i].sum == sumid[i+1].sum){
      cout << sumid[i].idx << " " << sumid[i+1].idx << " " << sumid[i].sum << " " << sumid[i+1].sum << endl;
      duplicates+=1;
    }
  }

  // auto last = std::unique(sumid.begin(), sumid.end(), [](const SumID &a, const SumID &b){ return a.sum == b.sum; });
  auto last = std::unique(sumid.begin(), sumid.end());
  sumid.erase(last, sumid.end());


  cout << " " << endl;
  cout << "n entries: " << nentries << endl;
  cout << "n duplicates: " << duplicates << endl;
  cout << "frac " << duplicates/nentries << endl;

  vector<Int_t> good_idx(sumid.size());
  for(Int_t i = 0; i < sumid.size(); i++){
    good_idx[i] = sumid[i].idx;
  }

  return good_idx;

}





