/*
 *
 * Copyright (c) 2001-2008
 *	Politecnico di Torino.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * For bug report and other information please visit Tstat site:
 * http://tstat.polito.it
 *
 * Tstat is deeply based on TCPTRACE. The authors would like to thank
 * Shawn Ostermann for the development of TCPTRACE.
 *
*/

#ifndef _TCPL7_H_
#define _TCPL7_H_

#include <sys/types.h>

/* Maximum number of packets to analyze per connection */
#define MAX_PACKETS_CON 50

/*Max packets to wait in each state*/
#define MAX_UNKNOWN_PACKETS 10
#define MAX_HTTP_COMMAND_PACKETS 20
#define MAX_HTTP_RESPONSE_PACKETS 30
#define MAX_RTSP_COMMAND_PACKETS 20
#define MAX_RTSP_RESPONSE_PACKETS 30



/* Definition of the RTP magic number */
#define RTP_MAGICNUMBER 0x24


/* Keyword definitions for fast compare */
/* considering both byte orders         */

#if(BYTE_ORDER == BIG_ENDIAN)

#define DESCRIBE 0x4445534352494245ULL
#define GET  0x47455420UL
#define POST 0x504F5354UL
#define HEAD 0x48454044UL
#define HTTP 0x48545450UL
#define RTSP 0x52545350UL
#define ICY  0x49435920UL
#define DESC 0x44455343UL

/* SMTP commands */
#define SMTP_220  0x32323020UL
#define SMTP_HELO 0x48454C4FUL
#define SMTP_EHLO 0x45484C4FUL

/* POP3 commands */
#define POP_OK    0x2B4F4B20UL
#define POP_ERR   0x2D455252UL
#define POP_USER  0x55534552UL
#define POP_QUIT  0x51554954UL
#define POP_APOP  0x41504F50UL

/* IMAP4 commands */
#define IMAP_OK   0x2A204F4BUL

#else

#define DESCRIBE 0x4542495243534544ULL
#define GET  0x20544547UL
#define POST 0x54534F50UL
#define HEAD 0x44404548UL
#define HTTP 0x50545448UL
#define RTSP 0x50535452UL
#define ICY  0x20594349UL
#define DESC 0x43534544UL

/* SMTP commands */
#define SMTP_220  0x20303232UL
#define SMTP_HELO 0x4F4C4548UL
#define SMTP_EHLO 0x4F4C4845UL

/* POP3 commands */
#define POP_OK    0x204B4F2BUL
#define POP_ERR   0x5252452DUL
#define POP_USER  0x52455355UL
#define POP_QUIT  0x54495551UL
#define POP_APOP  0x504F5041UL

/* IMAP4 commands */
#define IMAP_OK   0x4B4F202AUL

#endif

/* end Keyword definitions */

#endif
