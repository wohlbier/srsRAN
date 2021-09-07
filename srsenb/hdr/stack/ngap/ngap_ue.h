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
#ifndef SRSENB_NGAP_UE_H
#define SRSENB_NGAP_UE_H

#include "ngap.h"
#include "ngap_ue_bearer_manager.h"
#include "ngap_ue_proc.h"
#include "ngap_ue_utils.h"
#include "srsran/asn1/asn1_utils.h"
#include "srsran/asn1/ngap.h"
#include "srsran/interfaces/enb_gtpu_interfaces.h"
namespace srsenb {

class ngap::ue : public ngap_interface_ngap_proc
{
public:
  explicit ue(ngap*                  ngap_ptr_,
              rrc_interface_ngap_nr* rrc_ptr_,
              gtpu_interface_rrc*    gtpu_ptr_,
              srslog::basic_logger&  logger_);
  virtual ~ue();
  // TS 38.413 - Section 9.2.5.1 - Initial UE Message
  bool send_initial_ue_message(asn1::ngap_nr::rrcestablishment_cause_e cause,
                               srsran::unique_byte_buffer_t            pdu,
                               bool                                    has_tmsi,
                               uint32_t                                s_tmsi = 0);
  // TS 38.413 - Section 9.2.5.3 - Uplink NAS Transport
  bool send_ul_nas_transport(srsran::unique_byte_buffer_t pdu);
  // TS 38.413 - Section 9.2.2.2 - Initial Context Setup Response
  bool send_initial_ctxt_setup_response();
  // TS 38.413 - Section 9.2.2.3 - Initial Context Setup Failure
  bool send_initial_ctxt_setup_failure(asn1::ngap_nr::cause_c cause);
  // TS 38.413 - Section 9.2.1.2 - PDU Session Resource Setup Response
  bool send_pdu_session_resource_setup_response(uint16_t                                    pdu_session_id,
                                                uint32_t                                    teid_out,
                                                asn1::bounded_bitstring<1, 160, true, true> transport_layer_address);
  // TS 38.413 - Section 9.2.2.1 - Initial Context Setup Request
  bool handle_initial_ctxt_setup_request(const asn1::ngap_nr::init_context_setup_request_s& msg);
  // TS 38.413 - Section 9.2.2.5 - UE Context Release Command
  bool handle_ue_ctxt_release_cmd(const asn1::ngap_nr::ue_context_release_cmd_s& msg);
  // TS 38.413 - Section 9.2.1.1 - PDU Session Resource Setup Request
  bool handle_pdu_session_res_setup_request(const asn1::ngap_nr::pdu_session_res_setup_request_s& msg);

  bool was_uectxtrelease_requested() const { return release_requested; }
  void ue_ctxt_setup_complete();
  void notify_rrc_reconf_complete(const bool reconf_complete_outcome);

  ngap_ue_ctxt_t ctxt      = {};
  uint16_t       stream_id = 1;

private:
  // args
  ngap* ngap_ptr = nullptr;

  // state
  bool                   release_requested = false;
  ngap_ue_bearer_manager bearer_manager;

  // logger
  srslog::basic_logger& logger;

  // procedures
  srsran::proc_t<ngap_ue_initial_context_setup_proc> initial_context_setup_proc;
  srsran::proc_t<ngap_ue_ue_context_release_proc>    ue_context_release_proc;
  srsran::proc_t<ngap_ue_pdu_session_res_setup_proc> ue_pdu_session_res_setup_proc;
};

} // namespace srsenb
#endif