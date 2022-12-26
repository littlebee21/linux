/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * ipmi_si_sm.h
 *
 * State machine interface for low-level IPMI system management
 * interface state machines.  This code is the interface between
 * the ipmi_smi code (that handles the policy of a KCS, SMIC, or
 * BT interface) and the actual low-level state machine.
 *
 * Author: MontaVista Software, Inc.
 *         Corey Minyard <minyard@mvista.com>
 *         source@mvista.com
 *
 * Copyright 2002 MontaVista Software Inc.
 */

#ifndef __IPMI_SI_SM_H__
#define __IPMI_SI_SM_H__

#include "ipmi_si.h"

/*
 * This is defined by the state machines themselves, it is an opaque
 * data type for them to use.
 * *这是由状态机自己定义的，它是一种不透明的*数据类型，供它们使用。
 */
struct si_sm_data;

/* Results of SMI events. */
enum si_sm_result {
	SI_SM_CALL_WITHOUT_DELAY, /* Call the driver again immediately */
	SI_SM_CALL_WITH_DELAY,	/* Delay some before calling again. */
	SI_SM_CALL_WITH_TICK_DELAY,/* Delay >=1 tick before calling again. */
	SI_SM_TRANSACTION_COMPLETE, /* A transaction is finished. */
	SI_SM_IDLE,		/* The SM is in idle state. */
	SI_SM_HOSED,		/* The hardware violated the state machine. */

	/*
	 * The hardware is asserting attn and the state machine is
	 * idle.
	 */
	SI_SM_ATTN
};

/* Handlers for the SMI state machine. */
struct si_sm_handlers {
	/*
	 * Put the version number of the state machine here so the
	 * upper layer can print it.
	 */
	char *version;

	/*
	 * Initialize the data and return the amount of I/O space to
	 * reserve for the space.
	 */
	unsigned int (*init_data)(struct si_sm_data *smi,
				  struct si_sm_io   *io);

	/*
	 * Start a new transaction in the state machine.  This will
	 * return -2 if the state machine is not idle, -1 if the size
	 * is invalid (to large or too small), or 0 if the transaction
	 * is successfully completed.
	 * *在状态机中启动新事务。如果状态机不空闲，则返回-2；如果大小*无效（太大或太小），则返回-1；如果事务*成功完成，则返回0。
	 */
	int (*start_transaction)(struct si_sm_data *smi,
				 unsigned char *data, unsigned int size);

	/*
	 * Return the results after the transaction.  This will return
	 * -1 if the buffer is too small, zero if no transaction is
	 * present, or the actual length of the result data.
	 * /**交易后返回结果。如果缓冲区太小，则返回*-1，如果不存在事务，则返回零，或者返回结果数据的实际长度*/
	 */
	int (*get_result)(struct si_sm_data *smi,
			  unsigned char *data, unsigned int length);

	/*
	 * Call this periodically (for a polled interface) or upon
	 * receiving an interrupt (for a interrupt-driven interface).
	 * If interrupt driven, you should probably poll this
	 * periodically when not in idle state.  This should be called
	 * with the time that passed since the last call, if it is
	 * significant.  Time is in microseconds.
	 * 
	 * *周期性地（对于轮询接口）或在*接收到中断时（对于中断驱动接口）调用此命令。
	 * *如果是中断驱动的，您可能应该在不处于空闲状态时定期轮询这个*。
	 *  如果它是*重要的，则应使用自上次调用以来经过的时间来调用*。时间单位为微秒。
	 */
	enum si_sm_result (*event)(struct si_sm_data *smi, long time);

	/*
	 * Attempt to detect an SMI.  Returns 0 on success or nonzero
	 * on failure.
	 */
	int (*detect)(struct si_sm_data *smi);

	/* The interface is shutting down, so clean it up. */
	void (*cleanup)(struct si_sm_data *smi);

	/* Return the size of the SMI structure in bytes. */
	int (*size)(void);
};

/* Current state machines that we can use. */
extern const struct si_sm_handlers kcs_smi_handlers; //外部定义的kcs的中断处理函数
extern const struct si_sm_handlers smic_smi_handlers;  //外部的smic的处理函数
extern const struct si_sm_handlers bt_smi_handlers;    //外部的bt

#endif /* __IPMI_SI_SM_H__ */
