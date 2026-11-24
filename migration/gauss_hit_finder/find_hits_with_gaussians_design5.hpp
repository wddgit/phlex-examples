#ifndef MIGRATION_GAUSS_HIT_FINDER_FIND_HITS_WITH_GAUSSIANS_DESIGN5_HPP
#define MIGRATION_GAUSS_HIT_FINDER_FIND_HITS_WITH_GAUSSIANS_DESIGN5_HPP

// The difference between Design4 and Design5 is that the separate
// two unfolds followed by two folds has been simplified to one unfold
// followed by one fold. Other than that every thing else is the same.

// See README.md for some general comments about this example.

// The following comment is exactly copied from the older version of
// GausHitFinder used with the "art" Framework (some parts of it are
// no longer relevant...).
//
////////////////////////////////////////////////////////////////////////
//
// GaussHitFinder class
//
// jaasaadi@syr.edu
//
//  This algorithm is designed to find hits on wires after deconvolution.
// -----------------------------------
// This algorithm is based on the FFTHitFinder written by Brian Page,
// Michigan State University, for the ArgoNeuT experiment.
//
//
// The algorithm walks along the wire and looks for pulses above threshold
// The algorithm then attempts to fit n-gaussians to these pulses where n
// is set by the number of peaks found in the pulse
// If the Chi2/NDF returned is "bad" it attempts to fit n+1 gaussians to
// the pulse. If this is a better fit it then uses the parameters of the
// Gaussian fit to characterize the "hit" object
//
// To use this simply include the following in your producers:
// gaushit:     @local::microboone_gaushitfinder
// gaushit:	@local::argoneut_gaushitfinder
////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <utility>
#include <vector>

#include "copied_from_larsoft_minor_edits/Hit.h"
#include "copied_from_larsoft_minor_edits/HitFilterAlg.h"
#include "copied_from_larsoft_minor_edits/PeakFitterMrqdt.h"
#include "copied_from_larsoft_minor_edits/Wire.h"
#include "hit_candidate.hpp"
#include "wire_roi_data.hpp"

namespace examples {

  // ---------------------------------------------------------------
  // Unfold: vector<Wire> -> individual wire_roi_data objects
  // ---------------------------------------------------------------
  class unfold_wire_vector_to_roi {
  public:
    explicit unfold_wire_vector_to_roi(std::vector<recob::Wire> const& wires);

    using roi_iterator = std::pair<std::vector<recob::Wire>::const_iterator,
                                   std::size_t>;

    roi_iterator initial_value() const;
    bool predicate(roi_iterator current) const;
    std::pair<roi_iterator, wire_roi_data> unfold(roi_iterator current) const;

  private:
    std::vector<recob::Wire>::const_iterator begin_;
    std::vector<recob::Wire>::const_iterator end_;
  };

  // ---------------------------------------------------------------
  // Configuration for the Gaussian hit-fitting transform
  // ---------------------------------------------------------------
  struct find_hits_with_gaussians_design5_cfg {
    bool filter_hits;

    std::vector<int> long_max_hits_vec;    ///<Maximum number hits on a really long pulse train
    std::vector<int> long_pulse_width_vec; ///<Sets width of hits used to describe long pulses
    int max_multi_hit;                     ///<maximum hits for multi fit
    int area_method;                       ///<Type of area calculation
    std::vector<double> area_norms_vec; ///<factors for converting area to same units as peak height
    double chi2_ndf;                    ///maximum Chisquared / NDF allowed for a hit to be saved

    std::vector<float> pulse_height_cuts;
    std::vector<float> pulse_width_cuts;
    std::vector<float> pulse_ratio_cuts;
  };

  // ---------------------------------------------------------------
  // Transform: processes pre-computed merged hit candidates for a
  // single ROI and returns the hits found.
  //
  // Both the wire_roi_data (from the second unfold) and the
  // merge_hit_candidate_vec (from cand_hit_standard) are provided
  // as separate inputs.
  // ---------------------------------------------------------------
  std::vector<recob::Hit> find_hits_with_gaussians_design5(
    find_hits_with_gaussians_design5_cfg const& cfg,
    wire_roi_data const& roi_data,
    merge_hit_candidate_vec const& merged_candidates,
    PeakFitterMrqdt const& peak_fitter_mrqdt,
    HitFilterAlg const& hit_filter_alg);

  // ---------------------------------------------------------------
  // Fold: collects hits from individual ROIs into a
  // vector  (roi layer -> spill layer)
  // ---------------------------------------------------------------
  void fold_roi_hits_into_vector(std::vector<recob::Hit>& hits,
                                 std::vector<recob::Hit> const& hits_from_roi);
}
#endif // MIGRATION_GAUSS_HIT_FINDER_FIND_HITS_WITH_GAUSSIANS_DESIGN5_HPP
