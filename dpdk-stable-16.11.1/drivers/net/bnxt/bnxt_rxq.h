/*-
 *   BSD LICENSE
 *
 *   Copyright(c) Broadcom Limited.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Broadcom Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _BNXT_RQX_H_
#define _BNXT_RQX_H_

struct bnxt;
struct bnxt_rx_ring_info;
struct bnxt_cp_ring_info;
struct bnxt_rx_queue {
	struct rte_mempool	*mb_pool; /* mbuf pool for RX ring */
	struct rte_mbuf		*pkt_first_seg; /* 1st seg of pkt */
	struct rte_mbuf		*pkt_last_seg; /* Last seg of pkt */
	uint64_t		mbuf_initializer; /* val to init mbuf */
	uint16_t		nb_rx_desc; /* num of RX desc */
	uint16_t		rx_tail; /* cur val of RDT register */
	uint16_t		nb_rx_hold; /* num held free RX desc */
	uint16_t		rx_free_thresh; /* max free RX desc to hold */
	uint16_t		queue_id; /* RX queue index */
	uint16_t		reg_idx; /* RX queue register index */
	uint8_t			port_id; /* Device port identifier */
	uint8_t			crc_len; /* 0 if CRC stripped, 4 otherwise */

	struct bnxt		*bp;
	struct bnxt_vnic_info	*vnic;

	uint32_t			rx_buf_size;
	uint32_t			rx_buf_use_size;  /* useable size */
	struct bnxt_rx_ring_info	*rx_ring;
	struct bnxt_cp_ring_info	*cp_ring;
};

void bnxt_free_rxq_stats(struct bnxt_rx_queue *rxq);
int bnxt_mq_rx_configure(struct bnxt *bp);
void bnxt_rx_queue_release_op(void *rx_queue);
int bnxt_rx_queue_setup_op(struct rte_eth_dev *eth_dev,
			       uint16_t queue_idx,
			       uint16_t nb_desc,
			       unsigned int socket_id,
			       const struct rte_eth_rxconf *rx_conf,
			       struct rte_mempool *mp);
void bnxt_free_rx_mbufs(struct bnxt *bp);

#endif
