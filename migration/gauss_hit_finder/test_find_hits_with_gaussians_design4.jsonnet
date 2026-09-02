{
  driver: {
    cpp: 'generate_layers',
    layers: {
      spill: { parent: 'job', total: 5, starting_number: 0 },
    },
  },
  sources: {
    wires_source: {
      cpp: 'wires_source',
      layer: 'spill',
    },
    cell_id_source: {
      cpp: 'find_hits_with_gaussians_cell_id_hof',
      layer: 'spill',
    },
  },
  modules: {
    // Unfolds: vector<Wire> -> Wire -> wire_roi_data
    unfolds_design4_cpp: {
      cpp: 'unfolds_design4_hof',
      layer_vector_of_wires: 'spill',
      layer_wire: 'wire',
      layer_roi: 'roi',
    },
    // Candidate hit finding: wire_roi_data -> merge_hit_candidate_vec
    cand_hit_standard_design4_cpp: {
      cpp: 'cand_hit_standard_design4_hof',
      layer_roi: 'roi',
      roi_threshold: [6.0, 6.0, 6.0],
    },
    // Gaussian hit fitting: (wire_roi_data, merge_hit_candidate_vec) -> vector<Hit>
    find_hits_with_gaussians_design4_cpp: {
      cpp: 'find_hits_with_gaussians_design4_hof',
      layer_roi: 'roi',
      cand_hit_finder: 'cand_hit_standard_design4',

      filter_hits: false,
      long_max_hits_vec: [1, 1, 1],
      long_pulse_width_vec: [16, 16, 16],
      max_multi_hit: 4,
      area_method: 0,
      area_norms_vec: [13.25, 13.25, 13.25],
      chi2_ndf: 50.0,
      pulse_height_cuts: [3.0,  3.0,  3.0],
      pulse_width_cuts: [2.0, 1.5, 1.0],
      pulse_ratio_cuts: [0.35, 0.40, 0.20],
      peak_fitter_mrqdt_config: {
        min_width: 0.5,
        max_width_mult: 3.0,
        peak_range_fact: 2.0,
        peak_amp_range: 2.0,
      },
      hit_filter_alg_config: {
        min_pulse_height: [5.0, 5.0, 5.0],
        min_pulse_sigma: [1.0, 1.0, 1.0],
      },
    },
    // Folds: vector<Hit> (roi -> wire -> spill)
    folds_design4_cpp: {
      cpp: 'folds_design4_hof',
      layer_vector_of_wires: 'spill',
      layer_wire: 'wire',
      layer_roi: 'roi',
    },
    print_hits_to_file_cpp: {
      cpp: 'print_hits_to_file_hof',
      creator: 'fold_hits_into_vector_design4',
      layer: 'spill',
      filename_prefix: 'hits_design4',
    },
  },
}
