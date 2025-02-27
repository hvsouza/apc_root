////////////////////////////////////////////////////////////////////////
// Class:       AnalyzeEvents
// Plugin Type: analyzer (Unknown Unknown)
// File:        AnalyzeEvents_module.cc
//
// Generated at Thu Apr  6 12:38:09 2023 by Henrique Vieira de Souza using cetskelgen
// from  version .
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// Additional framwork includes
#include "art_root_io/TFileService.h"
#include "canvas/Persistency/Common/FindManyP.h"

// Additional LArSoft includes
#include "lardataobj/AnalysisBase/Calorimetry.h"
#include "lardataobj/AnalysisBase/T0.h"
#include "lardataobj/RecoBase/PFParticle.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/Slice.h"

// ROOT includes
#include <TH1F.h>
#include "TTree.h"

// STL includes
#include <string>
#include <vector>

namespace test {
  class AnalyzeEvents;
}


class test::AnalyzeEvents : public art::EDAnalyzer {
public:
  explicit AnalyzeEvents(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  AnalyzeEvents(AnalyzeEvents const&) = delete;
  AnalyzeEvents(AnalyzeEvents&&) = delete;
  AnalyzeEvents& operator=(AnalyzeEvents const&) = delete;
  AnalyzeEvents& operator=(AnalyzeEvents&&) = delete;

  // Required functions.
  void analyze(art::Event const& e) override;

  // Selected optional functions.
  void beginJob() override;
  void endJob() override;

private:

  // Declare member data here.

  //Create output TTree
  TTree *fTree = nullptr;
  TH1F *fTrackLengthHist = nullptr;

  // Tree Variables
  unsigned int fEventID;
  unsigned int fNPFParticles;
  unsigned int fNPrimaries;
  int fNPrimaryDaughters;
  float fT0;


  std::vector<float> fDaughterTrackLengths;
  std::vector<bool> fDaughterLongestTrack;

  std::vector<std::vector<float>> fDaughterTrackdEdx;
  std::vector<std::vector<float>> fDaughterTrackResidualRange;


  // Declare input labels
  const std::string fPFParticleLabel;
  const std::string fTrackLabel;
  const std::string fCalorimetryLabel;
  const std::string fSliceLabel;
  const std::string fOptLabel;


};


test::AnalyzeEvents::AnalyzeEvents(fhicl::ParameterSet const& p)
  : EDAnalyzer{p},
  fPFParticleLabel(p.get<std::string>("PFParticleLabel")),
  fTrackLabel(p.get<std::string>("TrackLabel")),
  fCalorimetryLabel(p.get<std::string>("CalorimetryLabel")),
  fSliceLabel(p.get<std::string>("SliceLabel")),
  fOptLabel(p.get<std::string>("OptLabel"))
  // More initializers here.
{
  // Call appropriate consumes<>() for any products to be retrieved by this module.
}

void test::AnalyzeEvents::analyze(art::Event const& e)
{
  // Implementation of required member function here.
  // Increment the event ID
  fEventID = e.id().event();


  // Reset all of our variables to 0 or empty vectors
  // This ensures things are not kept from the previous event
  fNPFParticles      = 0;
  fNPrimaries        = 0;
  fNPrimaryDaughters = 0;
  fT0 = 0;
  fDaughterTrackLengths.clear();
  fDaughterLongestTrack.clear();
  fDaughterTrackdEdx.clear();
  fDaughterTrackResidualRange.clear();

  // Load the PFParticles from pandora
  art::Handle<std::vector<recob::PFParticle>> pfpHandle;
  std::vector<art::Ptr<recob::PFParticle>> pfpVec;
  if (e.getByLabel(fPFParticleLabel, pfpHandle))
    art::fill_ptr_vector(pfpVec, pfpHandle);


  // Load the Tracks from pandora
  art::Handle<std::vector<recob::Track>> trackHandle;
  std::vector<art::Ptr<recob::Track>> trackVec;
  if (e.getByLabel(fTrackLabel, trackHandle))
    art::fill_ptr_vector(trackVec, trackHandle);


  // Load all of the slices from pandora
  art::Handle<std::vector<recob::Slice>> sliceHandle;
  std::vector<art::Ptr<recob::Slice>> sliceVec;
  if (e.getByLabel(fSliceLabel, sliceHandle))
    art::fill_ptr_vector(sliceVec, sliceHandle);

  // If there are no PFParticles then give up and skip the event
  if (pfpVec.empty())
    return;
  
  // Initialise neutrino ID
  size_t neutrinoID(std::numeric_limits<size_t>::max());
  
  // Loop over the PFParticles and find the neutrino
  for (const art::Ptr<recob::PFParticle>& pfp : pfpVec) {
    fNPFParticles++;

    // Check that we are looking at a primary that has a neutrino PDG code, if not move on to the next PFP
    if (!(pfp->IsPrimary() && (std::abs(pfp->PdgCode()) == 14 || std::abs(pfp->PdgCode()) == 12)))
      continue;

    neutrinoID = pfp->Self();
    fNPrimaryDaughters = pfp->NumDaughters();
    fNPrimaries++;
  }

  // Check that we found a reconstructed neutrino, if not skip the event
  if (neutrinoID == std::numeric_limits<size_t>::max())
    return;

  // Load the associations between PFPs, Tracks and Calorimetries
  art::FindManyP<recob::Track> pfpTrackAssns(pfpVec, e, fTrackLabel);
  art::FindManyP<anab::Calorimetry> trackCaloAssns(trackVec, e, fCalorimetryLabel);


  //Search for the longest daugther ID
  int longestID = -1;
  float longestLength = std::numeric_limits<float>::lowest();
  // A first loop to search the longest particle length
  for (const art::Ptr<recob::PFParticle>& pfp : pfpVec) {
    if(pfp->Parent() != neutrinoID) continue;

    // Get the tracks associated with this PFParticle
    const std::vector<art::Ptr<recob::Track>> pfpTracks(pfpTrackAssns.at(pfp.key()));

    // There should only ever be 0 or 1 tracks associated with a sigle PFParticle
    if (pfpTracks.size() == 1) {
      // Get the first (only) element of the vector
      const art::Ptr<recob::Track>& pfpTrack(pfpTracks.front());

      // If this track is the longest, save the ID and set the legnth of the longest track
      if(pfpTrack->Length() > longestLength){
        longestID = pfpTrack->ID();
        longestLength = pfpTrack->Length();
      } // length check
    } // PFParticle track
  } // PFP Particle

  // Now access the slices and corresponding timing information
  for (const art::Ptr<recob::PFParticle>& pfp : pfpVec) {
    // Start by assessing the neutrino PFParticle itself
    /* if(pfp->Self() != neutrinoID) continue; */
    // When interested in neutrino's daughters 
    if(pfp->Parent() != neutrinoID) continue;

    // Get the tracks associated with this PFParticle
    const std::vector<art::Ptr<recob::Track>> pfpTracks(pfpTrackAssns.at(pfp.key()));

    // There should only ever be 0 or 1 tracks associated with a sigle PFParticle
    if (pfpTracks.size() == 1) {
      // Get the first (only) element of the vector
      const art::Ptr<recob::Track>& pfpTrack(pfpTracks.front());

      // Add parameters from the track to the branch vector
      fDaughterTrackLengths.push_back(pfpTrack->Length());
      fDaughterLongestTrack.push_back(pfpTrack->ID() == longestID);

      // Fill the histogram with the track length
      fTrackLengthHist->Fill(pfpTrack->Length());

      // Now access the calorimetry association for this track
      const std::vector<art::Ptr<anab::Calorimetry>> trackCalos(trackCaloAssns.at(pfpTrack.key()));

      // Now loop over the calorimetry objects and select the one on the collection plane
      for(const art::Ptr<anab::Calorimetry> &calo : trackCalos){
        
        // Get the plane number in a simple format
        const int planeNum(calo->PlaneID().Plane);

        // If it is not on the collection plane, skip it
        if(planeNum != 2)
          continue;

        // Add parameters from the calorimetry objects to the branch vector
        fDaughterTrackdEdx.push_back(calo->dEdx());
        fDaughterTrackResidualRange.push_back(calo->ResidualRange());

      } // Calorimetry track association 
 
    } // PFParticle Track
  } // PFParticles


  // Load the associations between PFPs, Slices and T0
  art::FindManyP<recob::Slice> pfpSliceAssns(pfpVec, e, fSliceLabel);
  art::FindManyP<anab::T0> sliceT0Assns(sliceVec, e, fOptLabel);

  // Now access the slices and corresponding timing information
  for (const art::Ptr<recob::PFParticle>& pfp : pfpVec) {
    // Start by assessing the neutrino PFParticle itself
    if(pfp->Self() != neutrinoID) continue;

    // Get the slices associated with the current PFParticle
    const std::vector<art::Ptr<recob::Slice>> pfpSlices(pfpSliceAssns.at(pfp.key()));

    // There should only ever be 0 or 1 slices associated to the neutrino PFP
    if (pfpSlices.size() == 1) {
      // Get the first (only) element of the vector
      const art::Ptr<recob::Slice>& pfpSlice(pfpSlices.front());

      // Get the T0 object associated with the slice
      const std::vector<art::Ptr<anab::T0>> sliceT0s(sliceT0Assns.at(pfpSlice.key()));

      // There should only be 1 T0 per slice
      if (sliceT0s.size() == 1) {
        const art::Ptr<anab::T0>& t0(sliceT0s.front());
        fT0 = t0->Time();
      } // T0s
    } // Slices
  } // PFParticles


  // Store the outputs in the TTree
  fTree->Fill();
}

void test::AnalyzeEvents::beginJob()
{
  // Implementation of optional member function here.
  // Get the TFileService to create the output TTree for us
  art::ServiceHandle<art::TFileService> tfs;
  fTree = tfs->make<TTree>("tree","Output TTree");
  fTrackLengthHist = tfs->make<TH1F>("trackLegthHist", "Reconstruced track lenghts; Trach Length [cm]", 20, 0, 350);

  // Add branches to TTree
  fTree->Branch("eventID", &fEventID);
  fTree->Branch("nPFParticles", &fNPFParticles);
  fTree->Branch("nPrimaries", &fNPrimaries);
  fTree->Branch("nPrimaryDaughters", &fNPrimaryDaughters);
  fTree->Branch("daughterTrackLengths", &fDaughterTrackLengths);
  fTree->Branch("daughterLongestTrack", &fDaughterLongestTrack);
  fTree->Branch("daughterTrackdEdx", &fDaughterTrackdEdx);
  fTree->Branch("daughterTrackResidualRange", &fDaughterTrackResidualRange);
  fTree->Branch("T0", &fT0);
}

void test::AnalyzeEvents::endJob()
{
  // Implementation of optional member function here.
}

DEFINE_ART_MODULE(test::AnalyzeEvents)
