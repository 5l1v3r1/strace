/*
 * Copyright (c) 2016 Fabien Siron <fabien.siron@epita.fr>
 * Copyright (c) 2017 JingPiao Chen <chenjingpiao@gmail.com>
 * Copyright (c) 2016-2017 The strace developers.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "defs.h"
#include "netlink_route.h"
#include "nlattr.h"
#include "print_fields.h"

#include "netlink.h"
#ifdef HAVE_LINUX_IF_LINK_H
# include <linux/if_link.h>
#endif
#include <linux/rtnetlink.h>

#include "xlat/rtnl_link_attrs.h"

static bool
decode_rtnl_link_stats(struct tcb *const tcp,
		       const kernel_ulong_t addr,
		       const unsigned int len,
		       const void *const opaque_data)
{
	struct rtnl_link_stats st;
	const unsigned int min_size =
		offsetofend(struct rtnl_link_stats, tx_compressed);
	const unsigned int size = len < sizeof(st) ? min_size : sizeof(st);

	if (len < min_size)
		return false;
	else if (!umoven_or_printaddr(tcp, addr, size, &st)) {
		PRINT_FIELD_U("{", st, rx_packets);
		PRINT_FIELD_U(", ", st, tx_packets);
		PRINT_FIELD_U(", ", st, rx_bytes);
		PRINT_FIELD_U(", ", st, tx_bytes);
		PRINT_FIELD_U(", ", st, rx_errors);
		PRINT_FIELD_U(", ", st, tx_errors);
		PRINT_FIELD_U(", ", st, rx_dropped);
		PRINT_FIELD_U(", ", st, tx_dropped);
		PRINT_FIELD_U(", ", st, multicast);
		PRINT_FIELD_U(", ", st, collisions);

		PRINT_FIELD_U(", ", st, rx_length_errors);
		PRINT_FIELD_U(", ", st, rx_over_errors);
		PRINT_FIELD_U(", ", st, rx_crc_errors);
		PRINT_FIELD_U(", ", st, rx_frame_errors);
		PRINT_FIELD_U(", ", st, rx_fifo_errors);
		PRINT_FIELD_U(", ", st, rx_missed_errors);

		PRINT_FIELD_U(", ", st, tx_aborted_errors);
		PRINT_FIELD_U(", ", st, tx_carrier_errors);
		PRINT_FIELD_U(", ", st, tx_fifo_errors);
		PRINT_FIELD_U(", ", st, tx_heartbeat_errors);
		PRINT_FIELD_U(", ", st, tx_window_errors);

		PRINT_FIELD_U(", ", st, rx_compressed);
		PRINT_FIELD_U(", ", st, tx_compressed);
#ifdef HAVE_STRUCT_RTNL_LINK_STATS_RX_NOHANDLER
		if (len >= sizeof(st))
			PRINT_FIELD_U(", ", st, rx_nohandler);
#endif
		tprints("}");
	}

	return true;
}

static bool
decode_rtnl_link_ifmap(struct tcb *const tcp,
		       const kernel_ulong_t addr,
		       const unsigned int len,
		       const void *const opaque_data)
{
	struct rtnl_link_ifmap map;
	const unsigned int sizeof_ifmap =
		offsetofend(struct rtnl_link_ifmap, port);

	if (len < sizeof_ifmap)
		return false;
	else if (!umoven_or_printaddr(tcp, addr, sizeof_ifmap, &map)) {
		PRINT_FIELD_X("{", map, mem_start);
		PRINT_FIELD_X(", ", map, mem_end);
		PRINT_FIELD_X(", ", map, base_addr);
		PRINT_FIELD_U(", ", map, irq);
		PRINT_FIELD_U(", ", map, dma);
		PRINT_FIELD_U(", ", map, port);
		tprints("}");
	}

	return true;
}

static bool
decode_rtnl_link_stats64(struct tcb *const tcp,
		         const kernel_ulong_t addr,
			 const unsigned int len,
			 const void *const opaque_data)
{
#ifdef HAVE_STRUCT_RTNL_LINK_STATS64
	struct rtnl_link_stats64 st;
	const unsigned int min_size =
		offsetofend(struct rtnl_link_stats64, tx_compressed);
	const unsigned int size = len < sizeof(st) ? min_size : sizeof(st);

	if (len < min_size)
		return false;
	else if (!umoven_or_printaddr(tcp, addr, size, &st)) {
		PRINT_FIELD_U("{", st, rx_packets);
		PRINT_FIELD_U(", ", st, tx_packets);
		PRINT_FIELD_U(", ", st, rx_bytes);
		PRINT_FIELD_U(", ", st, tx_bytes);
		PRINT_FIELD_U(", ", st, rx_errors);
		PRINT_FIELD_U(", ", st, tx_errors);
		PRINT_FIELD_U(", ", st, rx_dropped);
		PRINT_FIELD_U(", ", st, tx_dropped);
		PRINT_FIELD_U(", ", st, multicast);
		PRINT_FIELD_U(", ", st, collisions);

		PRINT_FIELD_U(", ", st, rx_length_errors);
		PRINT_FIELD_U(", ", st, rx_over_errors);
		PRINT_FIELD_U(", ", st, rx_crc_errors);
		PRINT_FIELD_U(", ", st, rx_frame_errors);
		PRINT_FIELD_U(", ", st, rx_fifo_errors);
		PRINT_FIELD_U(", ", st, rx_missed_errors);

		PRINT_FIELD_U(", ", st, tx_aborted_errors);
		PRINT_FIELD_U(", ", st, tx_carrier_errors);
		PRINT_FIELD_U(", ", st, tx_fifo_errors);
		PRINT_FIELD_U(", ", st, tx_heartbeat_errors);
		PRINT_FIELD_U(", ", st, tx_window_errors);

		PRINT_FIELD_U(", ", st, rx_compressed);
		PRINT_FIELD_U(", ", st, tx_compressed);
#ifdef HAVE_STRUCT_RTNL_LINK_STATS64_RX_NOHANDLER
		if (len >= sizeof(st))
			PRINT_FIELD_U(", ", st, rx_nohandler);
#endif
		tprints("}");
	}

	return true;
#else
	return false;
#endif
}

static const nla_decoder_t ifinfomsg_nla_decoders[] = {
	[IFLA_ADDRESS]		= NULL, /* unimplemented */
	[IFLA_BROADCAST]	= NULL, /* unimplemented */
	[IFLA_IFNAME]		= decode_nla_str,
	[IFLA_MTU]		= decode_nla_u32,
	[IFLA_LINK]		= decode_nla_u32,
	[IFLA_QDISC]		= decode_nla_str,
	[IFLA_STATS]		= decode_rtnl_link_stats,
	[IFLA_COST]		= NULL, /* unused */
	[IFLA_PRIORITY]		= NULL, /* unused */
	[IFLA_MASTER]		= decode_nla_u32,
	[IFLA_WIRELESS]		= NULL, /* unimplemented */
	[IFLA_PROTINFO]		= NULL, /* unimplemented */
	[IFLA_TXQLEN]		= decode_nla_u32,
	[IFLA_MAP]		= decode_rtnl_link_ifmap,
	[IFLA_WEIGHT]		= decode_nla_u32,
	[IFLA_OPERSTATE]	= decode_nla_u8,
	[IFLA_LINKMODE]		= decode_nla_u8,
	[IFLA_LINKINFO]		= NULL, /* unimplemented */
	[IFLA_NET_NS_PID]	= decode_nla_u32,
	[IFLA_IFALIAS]		= decode_nla_str,
	[IFLA_NUM_VF]		= decode_nla_u32,
	[IFLA_VFINFO_LIST]	= NULL, /* unimplemented */
	[IFLA_STATS64]		= decode_rtnl_link_stats64,
	[IFLA_VF_PORTS]		= NULL, /* unimplemented */
	[IFLA_PORT_SELF]	= NULL, /* unimplemented */
	[IFLA_AF_SPEC]		= NULL, /* unimplemented */
	[IFLA_GROUP]		= decode_nla_u32,
	[IFLA_NET_NS_FD]	= decode_nla_u32,
	[IFLA_EXT_MASK]		= decode_nla_u32,
	[IFLA_PROMISCUITY]	= decode_nla_u32,
	[IFLA_NUM_TX_QUEUES]	= decode_nla_u32,
	[IFLA_NUM_RX_QUEUES]	= decode_nla_u32,
	[IFLA_CARRIER]		= decode_nla_u8,
	[IFLA_PHYS_PORT_ID]	= NULL, /* default parser */
	[IFLA_CARRIER_CHANGES]	= decode_nla_u32,
	[IFLA_PHYS_SWITCH_ID]	= NULL, /* default parser */
	[IFLA_LINK_NETNSID]	= decode_nla_s32,
	[IFLA_PHYS_PORT_NAME]	= decode_nla_str,
	[IFLA_PROTO_DOWN]	= decode_nla_u8,
	[IFLA_GSO_MAX_SEGS]	= decode_nla_u32,
	[IFLA_GSO_MAX_SIZE]	= decode_nla_u32,
	[IFLA_PAD]		= NULL,
	[IFLA_XDP]		= NULL, /* unimplemented */
	[IFLA_EVENT]		= decode_nla_u32
};

DECL_NETLINK_ROUTE_DECODER(decode_ifinfomsg)
{
	struct ifinfomsg ifinfo = { .ifi_family = family };
	size_t offset = sizeof(ifinfo.ifi_family);
	bool decode_nla = false;

	PRINT_FIELD_XVAL("{", ifinfo, ifi_family, addrfams, "AF_???");

	tprints(", ");
	if (len >= sizeof(ifinfo)) {
		if (!umoven_or_printaddr(tcp, addr + offset,
					 sizeof(ifinfo) - offset,
					 (void *) &ifinfo + offset)) {
			PRINT_FIELD_XVAL("", ifinfo, ifi_type,
					 arp_hardware_types, "ARPHRD_???");
			PRINT_FIELD_IFINDEX(", ", ifinfo, ifi_index);
			PRINT_FIELD_FLAGS(", ", ifinfo, ifi_flags,
					  iffflags, "IFF_???");
			PRINT_FIELD_X(", ", ifinfo, ifi_change);
			decode_nla = true;
		}
	} else
		tprints("...");
	tprints("}");

	offset = NLMSG_ALIGN(sizeof(ifinfo));
	if (decode_nla && len > offset) {
		tprints(", ");
		decode_nlattr(tcp, addr + offset, len - offset,
			      rtnl_link_attrs, "IFLA_???",
			      ifinfomsg_nla_decoders,
			      ARRAY_SIZE(ifinfomsg_nla_decoders), NULL);
	}
}