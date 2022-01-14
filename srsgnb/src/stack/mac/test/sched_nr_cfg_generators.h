/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2021 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#ifndef SRSRAN_SCHED_NR_CFG_GENERATORS_H
#define SRSRAN_SCHED_NR_CFG_GENERATORS_H

#include "srsgnb/hdr/stack/mac/sched_nr_interface_utils.h"
#include "srsran/common/phy_cfg_nr_default.h"

namespace srsenb {

inline srsran_coreset_t get_default_coreset0(uint32_t nof_prb)
{
  srsran_coreset_t coreset{};
  coreset.id                   = 0;
  coreset.duration             = 1;
  coreset.precoder_granularity = srsran_coreset_precoder_granularity_reg_bundle;
  for (uint32_t i = 0; i < SRSRAN_CORESET_FREQ_DOMAIN_RES_SIZE; ++i) {
    coreset.freq_resources[i] = i < (nof_prb / 6);
  }
  return coreset;
}

inline srsran_search_space_t get_default_search_space0()
{
  srsran_search_space_t ss{};
  ss.coreset_id        = 0;
  ss.nof_formats       = 1;
  ss.formats[0]        = srsran_dci_format_nr_1_0;
  ss.type              = srsran_search_space_type_common_0;
  ss.id                = 0;
  ss.nof_candidates[2] = 1;
  ss.duration          = 1;
  return ss;
}

inline sched_nr_interface::cell_cfg_t get_default_cell_cfg(
    const srsran::phy_cfg_nr_t& phy_cfg = srsran::phy_cfg_nr_default_t{srsran::phy_cfg_nr_default_t::reference_cfg_t{}})
{
  sched_nr_interface::cell_cfg_t cell_cfg{};

  cell_cfg.carrier          = phy_cfg.carrier;
  cell_cfg.duplex           = phy_cfg.duplex;
  cell_cfg.ssb              = phy_cfg.ssb;
  cell_cfg.mib.coreset0_idx = 6;
  cell_cfg.mib.scs_common   = srsran_subcarrier_spacing_15kHz;

  cell_cfg.bwps.resize(1);
  cell_cfg.bwps[0].pdcch    = phy_cfg.pdcch;
  cell_cfg.bwps[0].pdsch    = phy_cfg.pdsch;
  cell_cfg.bwps[0].pusch    = phy_cfg.pusch;
  cell_cfg.bwps[0].pucch    = phy_cfg.pucch;
  cell_cfg.bwps[0].prach    = phy_cfg.prach;
  cell_cfg.bwps[0].harq_ack = phy_cfg.harq_ack;
  cell_cfg.bwps[0].rb_width = phy_cfg.carrier.nof_prb;

  return cell_cfg;
}

inline std::vector<sched_nr_interface::cell_cfg_t> get_default_cells_cfg(
    uint32_t                    nof_sectors,
    const srsran::phy_cfg_nr_t& phy_cfg = srsran::phy_cfg_nr_default_t{srsran::phy_cfg_nr_default_t::reference_cfg_t{}})
{
  std::vector<sched_nr_interface::cell_cfg_t> cells;
  cells.reserve(nof_sectors);
  for (uint32_t i = 0; i < nof_sectors; ++i) {
    cells.push_back(get_default_cell_cfg(phy_cfg));
  }
  return cells;
}

inline sched_nr_interface::ue_cfg_t get_rach_ue_cfg(uint32_t                    cc,
                                                    const srsran::phy_cfg_nr_t& phy_cfg = srsran::phy_cfg_nr_default_t{
                                                        srsran::phy_cfg_nr_default_t::reference_cfg_t{}})
{
  sched_nr_interface::ue_cfg_t uecfg{};

  // set PCell
  uecfg.carriers.resize(1);
  uecfg.carriers[0].active = true;
  uecfg.carriers[0].cc     = cc;

  // set basic PHY config
  uecfg.phy_cfg     = srsran::phy_cfg_nr_default_t{srsran::phy_cfg_nr_default_t::reference_cfg_t{}};
  uecfg.phy_cfg.csi = {};
  for (srsran_search_space_t& ss : view_active_search_spaces(uecfg.phy_cfg.pdcch)) {
    // disable UE-specific search spaces
    if (ss.type == srsran_search_space_type_ue) {
      uecfg.phy_cfg.pdcch.search_space_present[ss.id] = false;
    }
  }

  return uecfg;
}

inline sched_nr_interface::ue_cfg_t get_default_ue_cfg(
    uint32_t                    nof_cc,
    const srsran::phy_cfg_nr_t& phy_cfg = srsran::phy_cfg_nr_default_t{srsran::phy_cfg_nr_default_t::reference_cfg_t{}})
{
  sched_nr_interface::ue_cfg_t uecfg{};
  uecfg.carriers.resize(nof_cc);
  for (uint32_t cc = 0; cc < nof_cc; ++cc) {
    uecfg.carriers[cc].cc     = cc;
    uecfg.carriers[cc].active = true;
  }
  uecfg.phy_cfg = phy_cfg;

  return uecfg;
}

inline sched_nr_interface::cell_cfg_t get_default_sa_cell_cfg_common()
{
  srsran::phy_cfg_nr_default_t::reference_cfg_t ref;
  ref.duplex                                        = srsran::phy_cfg_nr_default_t::reference_cfg_t::R_DUPLEX_FDD;
  sched_nr_interface::cell_cfg_t cell_cfg           = get_default_cell_cfg(srsran::phy_cfg_nr_default_t{ref});
  cell_cfg.bwps[0].pdcch.coreset_present[0]         = true;
  cell_cfg.bwps[0].pdcch.coreset[0]                 = get_default_coreset0(52);
  cell_cfg.bwps[0].pdcch.coreset[0].offset_rb       = 1;
  cell_cfg.bwps[0].pdcch.search_space_present[0]    = true;
  cell_cfg.bwps[0].pdcch.search_space[0]            = get_default_search_space0();
  cell_cfg.bwps[0].pdcch.coreset_present[1]         = false;
  cell_cfg.bwps[0].pdcch.search_space[1].coreset_id = 0;
  cell_cfg.bwps[0].pdcch.search_space[1].type       = srsran_search_space_type_common_1;
  cell_cfg.bwps[0].pdcch.search_space[1].nof_candidates[2] = 1;
  cell_cfg.bwps[0].pdcch.search_space[1].nof_formats       = 2;
  cell_cfg.bwps[0].pdcch.search_space[1].formats[0]        = srsran_dci_format_nr_1_0;
  cell_cfg.bwps[0].pdcch.search_space[1].formats[1]        = srsran_dci_format_nr_0_0;
  cell_cfg.bwps[0].pdcch.ra_search_space                   = cell_cfg.bwps[0].pdcch.search_space[1];
  return cell_cfg;
}

// Generate default UE-dedicated CORESET config
inline srsran_coreset_t get_default_ue_specific_coreset(uint32_t id, uint32_t pci)
{
  srsran_coreset_t coreset = {};
  coreset.id               = id;
  coreset.mapping_type     = srsran_coreset_mapping_type_non_interleaved;
  coreset.duration         = 1;
  for (uint32_t i = 0; i < 8; ++i) {
    coreset.freq_resources[i] = true;
  }
  coreset.dmrs_scrambling_id_present = false;
  coreset.precoder_granularity       = srsran_coreset_precoder_granularity_reg_bundle;
  coreset.interleaver_size           = srsran_coreset_bundle_size_n2;
  coreset.reg_bundle_size            = srsran_coreset_bundle_size_n6;
  coreset.shift_index                = pci;
  coreset.offset_rb                  = 0;
  return coreset;
}

inline srsran_search_space_t get_default_ue_specific_search_space(uint32_t id, uint32_t coreset_id)
{
  srsran_search_space_t ss = {};
  ss.id                    = id;
  ss.coreset_id            = coreset_id;
  ss.duration              = 1;
  ss.type                  = srsran_search_space_type_ue;
  ss.nof_formats           = 2;
  ss.formats[0]            = srsran_dci_format_nr_1_0;
  ss.formats[1]            = srsran_dci_format_nr_0_0;
  ss.nof_candidates[0]     = 2;
  ss.nof_candidates[1]     = 2;
  ss.nof_candidates[2]     = 2;
  ss.nof_candidates[3]     = 1;
  ss.nof_candidates[4]     = 0;
  return ss;
}

} // namespace srsenb

#endif // SRSRAN_SCHED_NR_CFG_GENERATORS_H
