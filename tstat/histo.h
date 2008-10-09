/*
 *
 * Copyright (c) 2001
 *	Politecnico di Torino.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Author:	Marco Mellia, Andrea Carpani, Luca Muscariello, Dario Rossi
 * 		Telecomunication Networks Group
 * 		Politecnico di Torino
 * 		Torino, Italy
 *              http://www.tlc-networks.polito.it/index.html
 *		mellia@mail.tlc.polito.it, rossi@mail.tlc.polito.it
 *
 * Tstat is deeply based on TCPTRACE. The authors would like to thank
 * Shawn Ostermann for the development of TCPTRACE.
 *
*/

#ifndef _HISTO_H_
#define _HISTO_H_
#define RRD_NAME_MAXLEN    19
#define HISTO_NAME_MAXLEN  (RRD_NAME_MAXLEN+20)
#define HISTO_DESCR_MAXLEN 256

struct dhisto
{
  double min;
  double max;
  double bin_size;
  struct dato *data;
};

struct dato
{
  int val;
  struct dato *next;
};

struct vhisto
{
  double min;
  double max;
  double bin_size;
  struct vdato *data;
};

/****************aste************/
struct double_histo_list
{
  char name[HISTO_NAME_MAXLEN];
  char descr[HISTO_DESCR_MAXLEN];

#define HISTO_PRINT_GLOBAL  3 
#define HISTO_PRINT_CURRENT 1 

#define HISTO_OFF 0 
#define HISTO_ON  1
#define HISTO_RRD 2
  int flag; 
  
  
#ifdef HAVE_RRDTOOL
/*-----------------------------------------------------------*/
/* RRDtools 				                     */
  struct rrdstat st;
  struct rrdconf conf;
/*-----------------------------------------------------------*/
#endif
  long min;
  long max;
  long bin_size;

  long bin_num;  // ((max - min) / bin_size) + 3 

#define BIN_LESS_MIN(histo)	 (0)
/* data[0]: counts samples smaller than min  */

#define BIN_BETWEEN(histo, val)  (((int) val + histo->bin_size - histo->min) / (int) histo->bin_size)
/* data[1 ... bin_num-3]:  counts samples between min and max with a step equal to delta */

#define BIN_MORE_MAX(histo)      (histo->bin_num-2)
/* data[num_col-1]: counts samples larger than max */

#define BIN_SAMPLE_COUNT(histo)  (histo->bin_num-1)
/* data[num_col]:  count *all* samples (used to speed up RRD percentile estimation) */


  long *first_data;
  long *second_data;
  long *current_data;
  int64_t *global_data;
  struct double_histo_list *next;
};
/*********************************/


struct adx
{
  struct in_addr ip;
  unsigned long src_hits;
  unsigned long dst_hits;
  struct adx *next;
};

/* Limits for SHORT */
#define SHORT_MM_CL_B 100000	/*[bytes] */
#define SHORT_MM_CL_P 1000	/*[packets] */
#define SHORT_MM_TOT_TIME 5000	/*[ms] */
#define BITRATE_MIN_PKTS 5




/* prototypes of functions */

void histo_set_conf(char *fname); 
void histo_parse_conf();
struct double_histo_list *find_histo(char *hname);
void alloc_histo (struct double_histo_list *thisto);

void alloc_adx();


struct double_histo_list *first_histo_list ();
void create_all_histo (void);
struct double_histo_list *create_histo (char *name, char *descr, long min,
					long max, long bin_size);

#define add_histo(hp, v)  {if(hp->flag == HISTO_ON){ __add_histo (hp, v);}}
void set_histo (struct double_histo_list *p, int index, double val);
void __add_histo (struct double_histo_list *p, double val);
void update_fake_histos();
void swap_histo ();
long *whats_frozen (struct double_histo_list *histo);

int print_all_histo (int flag);
int print_all_histo_definition ();
int print_histo (struct double_histo_list *phisto, char *titolo, int flag);
void print_histo_definition (struct double_histo_list *phisto, char *title);

int clear_all_histo ();
int clear_histo (struct double_histo_list *phisto);

int add_adx (struct in_addr *adx, int dir);
int print_adx ();

//    struct dhisto *create_dhisto (double min, double max, double bin_size);
//    struct dhisto *create_dhisto2 (double min, double max, int num_col);
//    int add_dhisto (struct dhisto *phisto, double val);
//    int print_dhisto (struct dhisto *phisto, char *titolo, char *labels);
//    int print_text_histo (struct double_histo_list *phisto, char *titolo, char *labels);
//    struct histo *create_vhisto (char *formato);




/* topix bitrate */

void fake_histo_bitrate_update (struct double_histo_list *phisto,
				double elapsed_time,
				unsigned long long *, int );


#define fake_histo_set(histo,value,bin)\
(histo)->current_data[(bin)] = (value);

#define fake_histo_add(histo,value,bin)\
(histo)->current_data[(bin)]+= (value);

#define fake_histo_get(histo,index)\
i(histo)->current_data[(index)];




/*-------------------------------------------------------------*/
/* here are all the histo tracked by tstat                     */
/* struct double_histo_list *test;                             */
/* NOTE                                                         */
/*      variable name MUST be SHORTER (<) than 20               */
/*      char, otherwise RRD will complain without               */
/*      further explaining the error                            */
/*-------------------------------------------------------------*/


struct double_histo_list *ip_bitrate_in;
struct double_histo_list *ip_bitrate_out;
struct double_histo_list *ip_bitrate_loc;


struct double_histo_list *ip_protocol_in;
struct double_histo_list *ip_protocol_out;
struct double_histo_list *ip_protocol_loc;

struct double_histo_list *ip_len_out;
struct double_histo_list *ip_len_in;
struct double_histo_list *ip_len_loc;

struct double_histo_list *ip_ttl_out;
struct double_histo_list *ip_ttl_in;
struct double_histo_list *ip_ttl_loc;

struct double_histo_list *ip_tos_out;
struct double_histo_list *ip_tos_in;
struct double_histo_list *ip_tos_loc;

//IPv6 module
#ifdef SUPPORT_IPV6

struct double_histo_list *ip6_protocol_in;
struct double_histo_list *ip6_hop_limit_in;
struct double_histo_list *ip6_plen_in;

struct double_histo_list *ip6_protocol_out;
struct double_histo_list *ip6_hop_limit_out;
struct double_histo_list *ip6_plen_out;

struct double_histo_list *ip6_protocol_loc;
struct double_histo_list *ip6_hop_limit_loc;
struct double_histo_list *ip6_plen_loc;

//ICMPv6 module
struct double_histo_list *icmpv6_type_in;
struct double_histo_list *icmpv6_type_loc;
struct double_histo_list *icmpv6_type_out;
#endif
//IPv6 module


struct double_histo_list *tcp_port_src_in;
struct double_histo_list *tcp_port_src_out;
struct double_histo_list *tcp_port_src_loc;
struct double_histo_list *tcp_port_dst_in;
struct double_histo_list *tcp_port_dst_out;
struct double_histo_list *tcp_port_dst_loc;

struct double_histo_list *tcp_port_synsrc_in;
struct double_histo_list *tcp_port_synsrc_out;
struct double_histo_list *tcp_port_synsrc_loc;
struct double_histo_list *tcp_port_syndst_in;
struct double_histo_list *tcp_port_syndst_out;
struct double_histo_list *tcp_port_syndst_loc;

struct double_histo_list *tcp_bitrate_in;
struct double_histo_list *tcp_bitrate_out;
struct double_histo_list *tcp_bitrate_loc;

struct double_histo_list *tcp_opts_SACK;
struct double_histo_list *tcp_opts_WS;
struct double_histo_list *tcp_opts_TS;

struct double_histo_list *tcp_mss_a;
struct double_histo_list *tcp_mss_b;
struct double_histo_list *tcp_mss_used;

struct double_histo_list *tcp_win_min;
struct double_histo_list *tcp_win_avg;
struct double_histo_list *tcp_win_max;

struct double_histo_list *tcp_cwnd;

struct double_histo_list *tcp_cl_b_l_out;
struct double_histo_list *tcp_cl_b_l_in;
struct double_histo_list *tcp_cl_b_l_loc;
struct double_histo_list *tcp_cl_b_s_out;
struct double_histo_list *tcp_cl_b_s_in;
struct double_histo_list *tcp_cl_b_s_loc;
struct double_histo_list *tcp_cl_p_out;
struct double_histo_list *tcp_cl_p_in;
struct double_histo_list *tcp_cl_p_loc;

struct double_histo_list *tcp_cl_b_l_c2s;
struct double_histo_list *tcp_cl_b_l_s2c;
struct double_histo_list *tcp_cl_b_s_c2s;
struct double_histo_list *tcp_cl_b_s_s2c;
struct double_histo_list *tcp_cl_p_c2s;
struct double_histo_list *tcp_cl_p_s2c;

struct double_histo_list *tcp_rtt_min_out;
struct double_histo_list *tcp_rtt_min_in;
struct double_histo_list *tcp_rtt_min_loc;
struct double_histo_list *tcp_rtt_min_c2s;
struct double_histo_list *tcp_rtt_min_s2c;

/* v.1.2.0 -----------------------------------*/
struct hyper_histo *check_rtt_avg;
struct double_histo_list *tcp_rtt_avg_out;
struct double_histo_list *tcp_rtt_avg_in;
struct double_histo_list *tcp_rtt_avg_loc;
struct double_histo_list *tcp_rtt_avg_c2s;
struct double_histo_list *tcp_rtt_avg_s2c;
/* v.1.2.0 -----------------------------------*/

struct double_histo_list *tcp_rtt_max_out;
struct double_histo_list *tcp_rtt_max_in;
struct double_histo_list *tcp_rtt_max_loc;
struct double_histo_list *tcp_rtt_max_c2s;
struct double_histo_list *tcp_rtt_max_s2c;

struct double_histo_list *tcp_rtt_stdev_out;
struct double_histo_list *tcp_rtt_stdev_in;
struct double_histo_list *tcp_rtt_stdev_loc;
struct double_histo_list *tcp_rtt_stdev_c2s;
struct double_histo_list *tcp_rtt_stdev_s2c;

struct double_histo_list *tcp_rtt_cnt_out;
struct double_histo_list *tcp_rtt_cnt_in;
struct double_histo_list *tcp_rtt_cnt_loc;
struct double_histo_list *tcp_rtt_cnt_c2s;
struct double_histo_list *tcp_rtt_cnt_s2c;

struct double_histo_list *tcp_anomalies_c2s;
struct double_histo_list *tcp_anomalies_s2c;
struct double_histo_list *tcp_anomalies_out;
struct double_histo_list *tcp_anomalies_in;
struct double_histo_list *tcp_anomalies_loc;

struct double_histo_list *tcp_rtx_RTO_c2s;
struct double_histo_list *tcp_rtx_RTO_s2c;
struct double_histo_list *tcp_rtx_RTO_out;
struct double_histo_list *tcp_rtx_RTO_in;
struct double_histo_list *tcp_rtx_RTO_loc;

struct double_histo_list *tcp_rtx_FR_c2s;
struct double_histo_list *tcp_rtx_FR_s2c;
struct double_histo_list *tcp_rtx_FR_out;
struct double_histo_list *tcp_rtx_FR_in;
struct double_histo_list *tcp_rtx_FR_loc;

struct double_histo_list *tcp_reordering_c2s;
struct double_histo_list *tcp_reordering_s2c;
struct double_histo_list *tcp_reordering_out;
struct double_histo_list *tcp_reordering_in;
struct double_histo_list *tcp_reordering_loc;

struct double_histo_list *tcp_net_dup_c2s;
struct double_histo_list *tcp_net_dup_s2c;
struct double_histo_list *tcp_net_dup_out;
struct double_histo_list *tcp_net_dup_in;
struct double_histo_list *tcp_net_dup_loc;

struct double_histo_list *tcp_unknown_c2s;
struct double_histo_list *tcp_unknown_s2c;
struct double_histo_list *tcp_unknown_out;
struct double_histo_list *tcp_unknown_in;
struct double_histo_list *tcp_unknown_loc;

struct double_histo_list *tcp_flow_ctrl_c2s;
struct double_histo_list *tcp_flow_ctrl_s2c;
struct double_histo_list *tcp_flow_ctrl_out;
struct double_histo_list *tcp_flow_ctrl_in;
struct double_histo_list *tcp_flow_ctrl_loc;

struct double_histo_list *tcp_unnrtx_RTO_c2s;
struct double_histo_list *tcp_unnrtx_RTO_s2c;
struct double_histo_list *tcp_unnrtx_RTO_out;
struct double_histo_list *tcp_unnrtx_RTO_in;
struct double_histo_list *tcp_unnrtx_RTO_loc;

struct double_histo_list *tcp_unnrtx_FR_c2s;
struct double_histo_list *tcp_unnrtx_FR_s2c;
struct double_histo_list *tcp_unnrtx_FR_out;
struct double_histo_list *tcp_unnrtx_FR_in;
struct double_histo_list *tcp_unnrtx_FR_loc;


struct double_histo_list *tcp_tot_time;
struct double_histo_list *tcp_thru_c2s;
struct double_histo_list *tcp_thru_s2c;

struct double_histo_list *tcp_interrupted;

struct double_histo_list *L4_flow_number;


    /* udp histograms */

struct double_histo_list *udp_cl_p_out;
struct double_histo_list *udp_cl_p_in;
struct double_histo_list *udp_cl_p_loc;

struct double_histo_list *udp_cl_b_l_out;
struct double_histo_list *udp_cl_b_l_in;
struct double_histo_list *udp_cl_b_l_loc;
struct double_histo_list *udp_cl_b_s_out;
struct double_histo_list *udp_cl_b_s_in;
struct double_histo_list *udp_cl_b_s_loc;

struct double_histo_list *udp_tot_time;
struct double_histo_list *udp_port_dst_in;
struct double_histo_list *udp_port_dst_out;
struct double_histo_list *udp_port_dst_loc;
struct double_histo_list *udp_port_flow_dst;

struct double_histo_list *udp_bitrate_in;
struct double_histo_list *udp_bitrate_out;
struct double_histo_list *udp_bitrate_loc;

      /* stream histograms */

struct double_histo_list *mm_type_in;
struct double_histo_list *mm_type_out;
struct double_histo_list *mm_type_loc;

struct double_histo_list *mm_rtp_pt_in;
struct double_histo_list *mm_rtp_pt_out;
struct double_histo_list *mm_rtp_pt_loc;

struct double_histo_list *mm_uni_multi_in;
struct double_histo_list *mm_uni_multi_out;
struct double_histo_list *mm_uni_multi_loc;

struct double_histo_list *mm_tot_time_in;
struct double_histo_list *mm_tot_time_out;
struct double_histo_list *mm_tot_time_loc;

struct double_histo_list *mm_tot_time_s_in;
struct double_histo_list *mm_tot_time_s_out;
struct double_histo_list *mm_tot_time_s_loc;


struct double_histo_list *mm_cl_p_out;
struct double_histo_list *mm_cl_p_in;
struct double_histo_list *mm_cl_p_loc;

struct double_histo_list *mm_cl_b_out;
struct double_histo_list *mm_cl_b_in;
struct double_histo_list *mm_cl_b_loc;

struct double_histo_list *mm_cl_p_s_out;
struct double_histo_list *mm_cl_p_s_in;
struct double_histo_list *mm_cl_p_s_loc;

struct double_histo_list *mm_cl_b_s_out;
struct double_histo_list *mm_cl_b_s_in;
struct double_histo_list *mm_cl_b_s_loc;

struct double_histo_list *mm_avg_bitrate_in;
struct double_histo_list *mm_avg_bitrate_out;
struct double_histo_list *mm_avg_bitrate_loc;

struct double_histo_list *mm_avg_ipg_in;
struct double_histo_list *mm_avg_ipg_out;
struct double_histo_list *mm_avg_ipg_loc;

struct double_histo_list *mm_avg_jitter_in;
struct double_histo_list *mm_avg_jitter_out;
struct double_histo_list *mm_avg_jitter_loc;

struct double_histo_list *mm_n_oos_in;
struct double_histo_list *mm_n_oos_out;
struct double_histo_list *mm_n_oos_loc;
struct double_histo_list *mm_p_oos_in;
struct double_histo_list *mm_p_oos_out;
struct double_histo_list *mm_p_oos_loc;
struct double_histo_list *mm_p_dup_in;
struct double_histo_list *mm_p_dup_out;
struct double_histo_list *mm_p_dup_loc;
struct double_histo_list *mm_p_lost_in;
struct double_histo_list *mm_p_lost_out;
struct double_histo_list *mm_p_lost_loc;
struct double_histo_list *mm_p_late_in;
struct double_histo_list *mm_p_late_out;
struct double_histo_list *mm_p_late_loc;
struct double_histo_list *mm_burst_loss_in;
struct double_histo_list *mm_burst_loss_out;
struct double_histo_list *mm_burst_loss_loc;
struct double_histo_list *mm_reord_p_n_in;
struct double_histo_list *mm_reord_p_n_out;
struct double_histo_list *mm_reord_p_n_loc;
struct double_histo_list *mm_reord_delay_in;
struct double_histo_list *mm_reord_delay_out;
struct double_histo_list *mm_reord_delay_loc;
struct double_histo_list *mm_oos_p_in;
struct double_histo_list *mm_oos_p_out;
struct double_histo_list *mm_oos_p_loc;

      /* rtcp histograms */

struct double_histo_list *rtcp_cl_p_out;
struct double_histo_list *rtcp_cl_p_in;
struct double_histo_list *rtcp_cl_p_loc;
struct double_histo_list *rtcp_cl_b_out;
struct double_histo_list *rtcp_cl_b_in;
struct double_histo_list *rtcp_cl_b_loc;
struct double_histo_list *rtcp_avg_inter_in;
struct double_histo_list *rtcp_avg_inter_out;
struct double_histo_list *rtcp_avg_inter_loc;
struct double_histo_list *rtcp_bt_in;
struct double_histo_list *rtcp_bt_out;
struct double_histo_list *rtcp_bt_loc;
struct double_histo_list *rtcp_mm_bt_in;
struct double_histo_list *rtcp_mm_bt_out;
struct double_histo_list *rtcp_mm_bt_loc;
struct double_histo_list *rtcp_lost_in;
struct double_histo_list *rtcp_lost_out;
struct double_histo_list *rtcp_lost_loc;
struct double_histo_list *rtcp_dup_in;
struct double_histo_list *rtcp_dup_out;
struct double_histo_list *rtcp_dup_loc;
struct double_histo_list *rtcp_f_lost_in;
struct double_histo_list *rtcp_f_lost_out;
struct double_histo_list *rtcp_f_lost_loc;
struct double_histo_list *rtcp_t_lost_in;
struct double_histo_list *rtcp_t_lost_out;
struct double_histo_list *rtcp_t_lost_loc;
struct double_histo_list *rtcp_mm_cl_p_in;
struct double_histo_list *rtcp_mm_cl_p_out;
struct double_histo_list *rtcp_mm_cl_p_loc;
struct double_histo_list *rtcp_mm_cl_b_in;
struct double_histo_list *rtcp_mm_cl_b_out;
struct double_histo_list *rtcp_mm_cl_b_loc;
struct double_histo_list *rtcp_jitter_in;
struct double_histo_list *rtcp_jitter_out;
struct double_histo_list *rtcp_jitter_loc;
struct double_histo_list *rtcp_rtt_in;
struct double_histo_list *rtcp_rtt_out;
struct double_histo_list *rtcp_rtt_loc;

/* global bitrate */
struct double_histo_list *g_tcp_bitrate_in;
struct double_histo_list *g_tcp_bitrate_loc;
struct double_histo_list *g_tcp_bitrate_out;
struct double_histo_list *g_udp_bitrate_in;
struct double_histo_list *g_udp_bitrate_loc;
struct double_histo_list *g_udp_bitrate_out;
struct double_histo_list *g_icmp_bitrate_in;
struct double_histo_list *g_icmp_bitrate_loc;
struct double_histo_list *g_icmp_bitrate_out;
struct double_histo_list *g_other_bitrate_in;
struct double_histo_list *g_other_bitrate_loc;
struct double_histo_list *g_other_bitrate_out;

/* Application stat */

struct double_histo_list *L7_TCP_num_out;
struct double_histo_list *L7_TCP_num_in;
struct double_histo_list *L7_TCP_num_loc;

struct double_histo_list *L7_UDP_num_out;
struct double_histo_list *L7_UDP_num_in;
struct double_histo_list *L7_UDP_num_loc;

/* Chat stat */

#define MSN_CHAT_HISTO 1
#define MSN_PRESENCE_HISTO 2
#define JABBER_CHAT_HISTO 3
#define JABBER_PRESENCE_HISTO 4
#define YMSG_CHAT_HISTO 5
#define YMSG_PRESENCE_HISTO 6
#define MAX_CHAT_FLOW_NUM 7
struct double_histo_list *chat_flow_num;
#endif
