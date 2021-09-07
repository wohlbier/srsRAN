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

#ifndef SRSENB_NGAP_UE_BEARER_MANAGER_H
#define SRSENB_NGAP_UE_BEARER_MANAGER_H

#include "srsran/asn1/asn1_utils.h"
#include "srsran/asn1/ngap.h"
#include "srsran/config.h"
#include "srsran/interfaces/enb_gtpu_interfaces.h"
#include "srsran/interfaces/gnb_rrc_nr_interfaces.h"

namespace srsenb {

/**
 * @brief Manages the GTPU bearers as part of the NGAP session procedures
 * */

class ngap_ue_bearer_manager
{
public:
  struct pdu_session_t {
    struct gtpu_tunnel {
      uint32_t                                    teid_out = 0;
      uint32_t                                    teid_in  = 0;
      asn1::bounded_bitstring<1, 160, true, true> address;
    };
    uint8_t                                    id   = 0;
    uint8_t                                    lcid = 0;
    asn1::ngap_nr::qos_flow_level_qos_params_s qos_params;
    std::vector<gtpu_tunnel>                   tunnels;
  };

  ngap_ue_bearer_manager(rrc_interface_ngap_nr* rrc_, gtpu_interface_rrc* gtpu_, srslog::basic_logger& logger_);
  ~ngap_ue_bearer_manager();

  int add_pdu_session(uint16_t                                           rnti,
                      uint8_t                                            pdu_session_id,
                      const asn1::ngap_nr::qos_flow_level_qos_params_s&  qos,
                      const asn1::bounded_bitstring<1, 160, true, true>& addr,
                      uint32_t                                           teid_out,
                      asn1::ngap_nr::cause_c&                            cause);

private:
  gtpu_interface_rrc*              gtpu = nullptr;
  rrc_interface_ngap_nr*           rrc  = nullptr;
  std::map<uint8_t, pdu_session_t> pdu_session_list;
  srslog::basic_logger&            logger;

  int  add_gtpu_bearer(uint16_t                                    rnti,
                       uint32_t                                    lcid,
                       uint32_t                                    pdu_session_id,
                       uint32_t                                    teid_out,
                       asn1::bounded_bitstring<1, 160, true, true> address,
                       pdu_session_t::gtpu_tunnel&                 tunnel, // out parameter
                       const gtpu_interface_rrc::bearer_props*     props = nullptr);
  void rem_gtpu_bearer(uint16_t rnti, uint32_t pdu_session_id);
};
} // namespace srsenb
#endif // SRSENB_NGAP_UE_BEARER_MANAGER_H