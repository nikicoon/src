/*	$NetBSD: mach_notify.h,v 1.4 2003/11/17 13:20:06 manu Exp $ */

/*-
 * Copyright (c) 2003 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Emmanuel Dreyfus
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MACH_NOTIFICATION_H_
#define _MACH_NOTIFICATION_H_

/* Notifications */

#define MACH_NOTIFY_DELETED_MSGID 65
#define MACH_NOTIFY_DESTROYED_MSGID 69
#define MACH_NOTIFY_NO_SENDERS_MSGID 70
#define MACH_NOTIFY_SEND_ONCE_MSGID 71
#define MACH_NOTIFY_DEAD_NAME_MSGID 72

typedef struct {
	mach_msg_header_t req_msgh;
	mach_ndr_record_t req_ndr;
	mach_port_name_t req_name;
	mach_msg_trailer_t req_trailer;
} mach_notify_port_deleted_request_t;

typedef struct {
	mach_msg_header_t req_msgh;
	mach_msg_body_t req_body;
	mach_msg_port_descriptor_t req_rights;
	mach_msg_trailer_t req_trailer;
} mach_notify_port_destroyed_request_t;

typedef struct {
	mach_msg_header_t req_msgh;
	mach_ndr_record_t req_ndr;
	mach_port_mscount_t req_mscount;
	mach_msg_trailer_t req_trailer;
} mach_notify_port_no_senders_request_t;

typedef struct {
	mach_msg_header_t req_msgh;
	mach_msg_trailer_t req_trailer;
} mach_notify_send_once_request_t;

typedef struct {
	mach_msg_header_t req_msgh;
	mach_ndr_record_t req_ndr;
	mach_port_name_t req_name;
	mach_msg_trailer_t req_trailer;
} mach_notify_port_dead_name_request_t;

void mach_notify_port_destroyed(struct lwp *, struct mach_right *);
void mach_notify_port_no_senders(struct lwp *, struct mach_right *);
void mach_notify_port_dead_name(struct lwp *, struct mach_right *);

/* Exceptions */

#define MACH_EXC_BAD_ACCESS		1
#define MACH_EXC_BAD_INSTRUCTION	2
#define MACH_EXC_ARITHMETIC		3
#define MACH_EXC_EMULATION		4
#define MACH_EXC_SOFTWARE		5
#define MACH_EXC_BREAKPOINT		6
#define MACH_EXC_SYSCALL		7
#define MACH_EXC_MACH_SYSCALL		8
#define MACH_EXC_RPC_ALERT		9
#define MACH_EXC_MAX			MACH_EXC_RPC_ALERT

#define MACH_EXC_MASK_BAD_ACCESS	(1 << MACH_EXC_BAD_ACCESS)
#define MACH_EXC_MASK_BAD_INSTRUCTION	(1 << MACH_EXC_BAD_INSTRUCTION)	
#define MACH_EXC_MASK_ARITHMETIC	(1 << MACH_EXC_ARITHMETIC)
#define MACH_EXC_MASK_EMULATION		(1 << MACH_EXC_EMULATION)
#define MACH_EXC_MASK_SOFTWARE		(1 << MACH_EXC_SOFTWARE)
#define MACH_EXC_MASK_BREAKPOINT	(1 << MACH_EXC_BREAKPOINT)
#define MACH_EXC_MASK_SYSCALL		(1 << MACH_EXC_SYSCALL)
#define MACH_EXC_MASK_MACH_SYSCALL	(1 << MACH_EXC_MACH_SYSCALL)
#define MACH_EXC_MASK_RPC_ALERT		(1 << MACH_EXC_RPC_ALERT)

/* exceptions codes */
#define MACH_BUS_ADRALN		1
#define MACH_SEGV_MAPERR	2
#define MACH_TRAP_BRKPT		1
#define MACH_ILL_ILLOPC		2
#define MACH_ILL_PRVOPC		3

/* Exception behaviors and associated messages Id */

#define MACH_EXCEPTION_DEFAULT		1
#define MACH_EXCEPTION_STATE		2
#define MACH_EXCEPTION_STATE_IDENTITY	3

#define MACH_EXC_RAISE_MSGID		2401
#define MACH_EXC_RAISE_STATE_MSGID	2402
#define MACH_EXC_RAISE_STATE_IDENTITY_MSGID	2403


typedef struct {
	mach_msg_header_t req_msgh;
	mach_msg_body_t req_body;
	mach_msg_port_descriptor_t req_thread;
	mach_msg_port_descriptor_t req_task;
	mach_ndr_record_t req_ndr;
	mach_exception_type_t req_exc;
	mach_msg_type_number_t req_codecount;
	mach_integer_t req_code[2];
	mach_msg_trailer_t req_trailer;
} mach_exception_raise_request_t;

typedef struct {
	mach_msg_header_t req_msgh;
	mach_ndr_record_t req_ndr;
	mach_exception_type_t req_exc;
	mach_msg_type_number_t req_codecount;
	mach_integer_t req_code[2];
	int req_flavor;
	mach_msg_type_number_t req_statecount;
	mach_natural_t req_state[144];
	mach_msg_trailer_t req_trailer;
} mach_exception_raise_state_request_t;

typedef struct {
	mach_msg_header_t req_msgh;
	mach_msg_body_t req_body;
	mach_msg_port_descriptor_t req_thread;
	mach_msg_port_descriptor_t req_task;
	mach_ndr_record_t req_ndr;
	mach_exception_type_t req_exc;
	mach_msg_type_number_t req_codecount;
	mach_integer_t req_code[2];
	int req_flavor;
	mach_msg_type_number_t req_statecount;
	mach_natural_t req_state[144];
	mach_msg_trailer_t req_trailer;
} mach_exception_raise_state_identity_request_t;

void mach_trapsignal(struct lwp *, const struct ksiginfo *);
int mach_trapsignal1(struct lwp *, const struct ksiginfo *);
int mach_exception(struct lwp *, const struct ksiginfo *, 
    struct mach_port *, int, struct mach_right *, struct mach_right *);

#endif /* _MACH_NOTIFICATION_H_ */

