/*	$NetBSD: kern_mutex.c,v 1.43 2008/05/31 13:31:25 ad Exp $	*/

/*-
 * Copyright (c) 2002, 2006, 2007, 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe and Andrew Doran.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
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

/*
 * Kernel mutex implementation, modeled after those found in Solaris,
 * a description of which can be found in:
 *
 *	Solaris Internals: Core Kernel Architecture, Jim Mauro and
 *	    Richard McDougall.
 */

#define	__MUTEX_PRIVATE

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: kern_mutex.c,v 1.43 2008/05/31 13:31:25 ad Exp $");

#include <sys/param.h>
#include <sys/proc.h>
#include <sys/mutex.h>
#include <sys/sched.h>
#include <sys/sleepq.h>
#include <sys/systm.h>
#include <sys/lockdebug.h>
#include <sys/kernel.h>
#include <sys/atomic.h>
#include <sys/intr.h>
#include <sys/lock.h>
#include <sys/pool.h>

#include <dev/lockstat.h>

#include <machine/lock.h>

/*
 * When not running a debug kernel, spin mutexes are not much
 * more than an splraiseipl() and splx() pair.
 */

#if defined(DIAGNOSTIC) || defined(MULTIPROCESSOR) || defined(LOCKDEBUG)
#define	FULL
#endif

/*
 * Debugging support.
 */

#define	MUTEX_WANTLOCK(mtx)					\
    LOCKDEBUG_WANTLOCK(MUTEX_DEBUG_P(mtx), (mtx),		\
        (uintptr_t)__builtin_return_address(0), false, false)
#define	MUTEX_LOCKED(mtx)					\
    LOCKDEBUG_LOCKED(MUTEX_DEBUG_P(mtx), (mtx), NULL,		\
        (uintptr_t)__builtin_return_address(0), 0)
#define	MUTEX_UNLOCKED(mtx)					\
    LOCKDEBUG_UNLOCKED(MUTEX_DEBUG_P(mtx), (mtx),		\
        (uintptr_t)__builtin_return_address(0), 0)
#define	MUTEX_ABORT(mtx, msg)					\
    mutex_abort(mtx, __func__, msg)

#if defined(LOCKDEBUG)

#define	MUTEX_DASSERT(mtx, cond)				\
do {								\
	if (!(cond))						\
		MUTEX_ABORT(mtx, "assertion failed: " #cond);	\
} while (/* CONSTCOND */ 0);

#else	/* LOCKDEBUG */

#define	MUTEX_DASSERT(mtx, cond)	/* nothing */

#endif /* LOCKDEBUG */

#if defined(DIAGNOSTIC)

#define	MUTEX_ASSERT(mtx, cond)					\
do {								\
	if (!(cond))						\
		MUTEX_ABORT(mtx, "assertion failed: " #cond);	\
} while (/* CONSTCOND */ 0)

#else	/* DIAGNOSTIC */

#define	MUTEX_ASSERT(mtx, cond)	/* nothing */

#endif	/* DIAGNOSTIC */

/*
 * Spin mutex SPL save / restore.
 */
#ifndef MUTEX_COUNT_BIAS
#define	MUTEX_COUNT_BIAS	0
#endif

#define	MUTEX_SPIN_SPLRAISE(mtx)					\
do {									\
	struct cpu_info *x__ci;						\
	int x__cnt, s;							\
	s = splraiseipl(mtx->mtx_ipl);					\
	x__ci = curcpu();						\
	x__cnt = x__ci->ci_mtx_count--;					\
	__insn_barrier();						\
	if (x__cnt == MUTEX_COUNT_BIAS)					\
		x__ci->ci_mtx_oldspl = (s);				\
} while (/* CONSTCOND */ 0)

#define	MUTEX_SPIN_SPLRESTORE(mtx)					\
do {									\
	struct cpu_info *x__ci = curcpu();				\
	int s = x__ci->ci_mtx_oldspl;					\
	__insn_barrier();						\
	if (++(x__ci->ci_mtx_count) == MUTEX_COUNT_BIAS)		\
		splx(s);						\
} while (/* CONSTCOND */ 0)

/*
 * For architectures that provide 'simple' mutexes: they provide a
 * CAS function that is either MP-safe, or does not need to be MP
 * safe.  Adaptive mutexes on these architectures do not require an
 * additional interlock.
 */

#ifdef __HAVE_SIMPLE_MUTEXES

#define	MUTEX_OWNER(owner)						\
	(owner & MUTEX_THREAD)
#define	MUTEX_HAS_WAITERS(mtx)						\
	(((int)(mtx)->mtx_owner & MUTEX_BIT_WAITERS) != 0)

#define	MUTEX_INITIALIZE_ADAPTIVE(mtx, dodebug)				\
do {									\
	if (dodebug)							\
		(mtx)->mtx_owner |= MUTEX_BIT_DEBUG;			\
} while (/* CONSTCOND */ 0);

#define	MUTEX_INITIALIZE_SPIN(mtx, dodebug, ipl)			\
do {									\
	(mtx)->mtx_owner = MUTEX_BIT_SPIN;				\
	if (dodebug)							\
		(mtx)->mtx_owner |= MUTEX_BIT_DEBUG;			\
	(mtx)->mtx_ipl = makeiplcookie((ipl));				\
	__cpu_simple_lock_init(&(mtx)->mtx_lock);			\
} while (/* CONSTCOND */ 0)

#define	MUTEX_DESTROY(mtx)						\
do {									\
	(mtx)->mtx_owner = MUTEX_THREAD;				\
} while (/* CONSTCOND */ 0);

#define	MUTEX_SPIN_P(mtx)		\
    (((mtx)->mtx_owner & MUTEX_BIT_SPIN) != 0)
#define	MUTEX_ADAPTIVE_P(mtx)		\
    (((mtx)->mtx_owner & MUTEX_BIT_SPIN) == 0)

#define	MUTEX_DEBUG_P(mtx)	(((mtx)->mtx_owner & MUTEX_BIT_DEBUG) != 0)
#if defined(LOCKDEBUG)
#define	MUTEX_OWNED(owner)		(((owner) & ~MUTEX_BIT_DEBUG) != 0)
#define	MUTEX_INHERITDEBUG(new, old)	(new) |= (old) & MUTEX_BIT_DEBUG
#else /* defined(LOCKDEBUG) */
#define	MUTEX_OWNED(owner)		((owner) != 0)
#define	MUTEX_INHERITDEBUG(new, old)	/* nothing */
#endif /* defined(LOCKDEBUG) */

static inline int
MUTEX_ACQUIRE(kmutex_t *mtx, uintptr_t curthread)
{
	int rv;
	uintptr_t old = 0;
	uintptr_t new = curthread;

	MUTEX_INHERITDEBUG(old, mtx->mtx_owner);
	MUTEX_INHERITDEBUG(new, old);
	rv = MUTEX_CAS(&mtx->mtx_owner, old, new);
	MUTEX_RECEIVE(mtx);
	return rv;
}

static inline int
MUTEX_SET_WAITERS(kmutex_t *mtx, uintptr_t owner)
{
	int rv;
	rv = MUTEX_CAS(&mtx->mtx_owner, owner, owner | MUTEX_BIT_WAITERS);
	MUTEX_RECEIVE(mtx);
	return rv;
}

static inline void
MUTEX_RELEASE(kmutex_t *mtx)
{
	uintptr_t new;

	MUTEX_GIVE(mtx);
	new = 0;
	MUTEX_INHERITDEBUG(new, mtx->mtx_owner);
	mtx->mtx_owner = new;
}

static inline void
MUTEX_CLEAR_WAITERS(kmutex_t *mtx)
{
	/* nothing */
}
#endif	/* __HAVE_SIMPLE_MUTEXES */

/*
 * Patch in stubs via strong alias where they are not available.
 */

#if defined(LOCKDEBUG)
#undef	__HAVE_MUTEX_STUBS
#undef	__HAVE_SPIN_MUTEX_STUBS
#endif

#ifndef __HAVE_MUTEX_STUBS
__strong_alias(mutex_enter,mutex_vector_enter);
__strong_alias(mutex_exit,mutex_vector_exit);
#endif

#ifndef __HAVE_SPIN_MUTEX_STUBS
__strong_alias(mutex_spin_enter,mutex_vector_enter);
__strong_alias(mutex_spin_exit,mutex_vector_exit);
#endif

void	mutex_abort(kmutex_t *, const char *, const char *);
void	mutex_dump(volatile void *);
int	mutex_onproc(uintptr_t, struct cpu_info **);

lockops_t mutex_spin_lockops = {
	"Mutex",
	LOCKOPS_SPIN,
	mutex_dump
};

lockops_t mutex_adaptive_lockops = {
	"Mutex",
	LOCKOPS_SLEEP,
	mutex_dump
};

syncobj_t mutex_syncobj = {
	SOBJ_SLEEPQ_SORTED,
	turnstile_unsleep,
	turnstile_changepri,
	sleepq_lendpri,
	(void *)mutex_owner,
};

/* Mutex cache */
#define	MUTEX_OBJ_MAGIC	0x5aa3c85d
struct kmutexobj {
	kmutex_t	mo_lock;
	u_int		mo_magic;
	u_int		mo_refcnt;
};

static int	mutex_obj_ctor(void *, void *, int);

static pool_cache_t	mutex_obj_cache;

/*
 * mutex_dump:
 *
 *	Dump the contents of a mutex structure.
 */
void
mutex_dump(volatile void *cookie)
{
	volatile kmutex_t *mtx = cookie;

	printf_nolog("owner field  : %#018lx wait/spin: %16d/%d\n",
	    (long)MUTEX_OWNER(mtx->mtx_owner), MUTEX_HAS_WAITERS(mtx),
	    MUTEX_SPIN_P(mtx));
}

/*
 * mutex_abort:
 *
 *	Dump information about an error and panic the system.  This
 *	generates a lot of machine code in the DIAGNOSTIC case, so
 *	we ask the compiler to not inline it.
 */
void __noinline
mutex_abort(kmutex_t *mtx, const char *func, const char *msg)
{

	LOCKDEBUG_ABORT(mtx, (MUTEX_SPIN_P(mtx) ?
	    &mutex_spin_lockops : &mutex_adaptive_lockops), func, msg);
}

/*
 * mutex_init:
 *
 *	Initialize a mutex for use.  Note that adaptive mutexes are in
 *	essence spin mutexes that can sleep to avoid deadlock and wasting
 *	CPU time.  We can't easily provide a type of mutex that always
 *	sleeps - see comments in mutex_vector_enter() about releasing
 *	mutexes unlocked.
 */
void
mutex_init(kmutex_t *mtx, kmutex_type_t type, int ipl)
{
	bool dodebug;

	memset(mtx, 0, sizeof(*mtx));

	switch (type) {
	case MUTEX_ADAPTIVE:
		KASSERT(ipl == IPL_NONE);
		break;
	case MUTEX_DEFAULT:
	case MUTEX_DRIVER:
		if (ipl == IPL_NONE || ipl == IPL_SOFTCLOCK ||
		    ipl == IPL_SOFTBIO || ipl == IPL_SOFTNET ||
		    ipl == IPL_SOFTSERIAL) {
			type = MUTEX_ADAPTIVE;
		} else {
			type = MUTEX_SPIN;
		}
		break;
	default:
		break;
	}

	switch (type) {
	case MUTEX_NODEBUG:
		dodebug = LOCKDEBUG_ALLOC(mtx, NULL,
		    (uintptr_t)__builtin_return_address(0));
		MUTEX_INITIALIZE_SPIN(mtx, dodebug, ipl);
		break;
	case MUTEX_ADAPTIVE:
		dodebug = LOCKDEBUG_ALLOC(mtx, &mutex_adaptive_lockops,
		    (uintptr_t)__builtin_return_address(0));
		MUTEX_INITIALIZE_ADAPTIVE(mtx, dodebug);
		break;
	case MUTEX_SPIN:
		dodebug = LOCKDEBUG_ALLOC(mtx, &mutex_spin_lockops,
		    (uintptr_t)__builtin_return_address(0));
		MUTEX_INITIALIZE_SPIN(mtx, dodebug, ipl);
		break;
	default:
		panic("mutex_init: impossible type");
		break;
	}
}

/*
 * mutex_destroy:
 *
 *	Tear down a mutex.
 */
void
mutex_destroy(kmutex_t *mtx)
{

	if (MUTEX_ADAPTIVE_P(mtx)) {
		MUTEX_ASSERT(mtx, !MUTEX_OWNED(mtx->mtx_owner) &&
		    !MUTEX_HAS_WAITERS(mtx));
	} else {
		MUTEX_ASSERT(mtx, !__SIMPLELOCK_LOCKED_P(&mtx->mtx_lock));
	}

	LOCKDEBUG_FREE(MUTEX_DEBUG_P(mtx), mtx);
	MUTEX_DESTROY(mtx);
}

/*
 * mutex_onproc:
 *
 *	Return true if an adaptive mutex owner is running on a CPU in the
 *	system.  If the target is waiting on the kernel big lock, then we
 *	must release it.  This is necessary to avoid deadlock.
 *
 *	Note that we can't use the mutex owner field as an LWP pointer.  We
 *	don't have full control over the timing of our execution, and so the
 *	pointer could be completely invalid by the time we dereference it.
 */
#ifdef MULTIPROCESSOR
int
mutex_onproc(uintptr_t owner, struct cpu_info **cip)
{
	CPU_INFO_ITERATOR cii;
	struct cpu_info *ci;
	struct lwp *l;

	if (!MUTEX_OWNED(owner))
		return 0;
	l = (struct lwp *)MUTEX_OWNER(owner);

	/* See if the target is running on a CPU somewhere. */
	if ((ci = *cip) != NULL && ci->ci_curlwp == l)
		goto run;
	for (CPU_INFO_FOREACH(cii, ci))
		if (ci->ci_curlwp == l)
			goto run;

	/* No: it may be safe to block now. */
	*cip = NULL;
	return 0;

 run:
 	/* Target is running; do we need to block? */
 	*cip = ci;
	return ci->ci_biglock_wanted != l;
}
#endif	/* MULTIPROCESSOR */

/*
 * mutex_vector_enter:
 *
 *	Support routine for mutex_enter() that must handles all cases.  In
 *	the LOCKDEBUG case, mutex_enter() is always aliased here, even if
 *	fast-path stubs are available.  If an mutex_spin_enter() stub is
 *	not available, then it is also aliased directly here.
 */
void
mutex_vector_enter(kmutex_t *mtx)
{
	uintptr_t owner, curthread;
	turnstile_t *ts;
#ifdef MULTIPROCESSOR
	struct cpu_info *ci = NULL;
	u_int count;
#endif
	LOCKSTAT_COUNTER(spincnt);
	LOCKSTAT_COUNTER(slpcnt);
	LOCKSTAT_TIMER(spintime);
	LOCKSTAT_TIMER(slptime);
	LOCKSTAT_FLAG(lsflag);

	/*
	 * Handle spin mutexes.
	 */
	if (MUTEX_SPIN_P(mtx)) {
#if defined(LOCKDEBUG) && defined(MULTIPROCESSOR)
		u_int spins = 0;
#endif
		MUTEX_SPIN_SPLRAISE(mtx);
		MUTEX_WANTLOCK(mtx);
#ifdef FULL
		if (__cpu_simple_lock_try(&mtx->mtx_lock)) {
			MUTEX_LOCKED(mtx);
			return;
		}
#if !defined(MULTIPROCESSOR)
		MUTEX_ABORT(mtx, "locking against myself");
#else /* !MULTIPROCESSOR */

		LOCKSTAT_ENTER(lsflag);
		LOCKSTAT_START_TIMER(lsflag, spintime);
		count = SPINLOCK_BACKOFF_MIN;

		/*
		 * Spin testing the lock word and do exponential backoff
		 * to reduce cache line ping-ponging between CPUs.
		 */
		do {
			if (panicstr != NULL)
				break;
			while (__SIMPLELOCK_LOCKED_P(&mtx->mtx_lock)) {
				SPINLOCK_BACKOFF(count); 
#ifdef LOCKDEBUG
				if (SPINLOCK_SPINOUT(spins))
					MUTEX_ABORT(mtx, "spinout");
#endif	/* LOCKDEBUG */
			}
		} while (!__cpu_simple_lock_try(&mtx->mtx_lock));

		if (count != SPINLOCK_BACKOFF_MIN) {
			LOCKSTAT_STOP_TIMER(lsflag, spintime);
			LOCKSTAT_EVENT(lsflag, mtx,
			    LB_SPIN_MUTEX | LB_SPIN, 1, spintime);
		}
		LOCKSTAT_EXIT(lsflag);
#endif	/* !MULTIPROCESSOR */
#endif	/* FULL */
		MUTEX_LOCKED(mtx);
		return;
	}

	curthread = (uintptr_t)curlwp;

	MUTEX_DASSERT(mtx, MUTEX_ADAPTIVE_P(mtx));
	MUTEX_ASSERT(mtx, curthread != 0);
	MUTEX_WANTLOCK(mtx);

	if (panicstr == NULL) {
		LOCKDEBUG_BARRIER(&kernel_lock, 1);
	}

	LOCKSTAT_ENTER(lsflag);

	/*
	 * Adaptive mutex; spin trying to acquire the mutex.  If we
	 * determine that the owner is not running on a processor,
	 * then we stop spinning, and sleep instead.
	 */
	for (owner = mtx->mtx_owner;;) {
		if (!MUTEX_OWNED(owner)) {
			/*
			 * Mutex owner clear could mean two things:
			 *
			 *	* The mutex has been released.
			 *	* The owner field hasn't been set yet.
			 *
			 * Try to acquire it again.  If that fails,
			 * we'll just loop again.
			 */
			if (MUTEX_ACQUIRE(mtx, curthread))
				break;
			owner = mtx->mtx_owner;
			continue;
		}

		if (panicstr != NULL)
			return;
		if (MUTEX_OWNER(owner) == curthread)
			MUTEX_ABORT(mtx, "locking against myself");

#ifdef MULTIPROCESSOR
		/*
		 * Check to see if the owner is running on a processor.
		 * If so, then we should just spin, as the owner will
		 * likely release the lock very soon.
		 */
		if (mutex_onproc(owner, &ci)) {
			LOCKSTAT_START_TIMER(lsflag, spintime);
			count = SPINLOCK_BACKOFF_MIN;
			for (;;) {
				SPINLOCK_BACKOFF(count);
				owner = mtx->mtx_owner;
				if (!mutex_onproc(owner, &ci))
					break;
			}
			LOCKSTAT_STOP_TIMER(lsflag, spintime);
			LOCKSTAT_COUNT(spincnt, 1);
			if (!MUTEX_OWNED(owner))
				continue;
		}
#endif

		ts = turnstile_lookup(mtx);

		/*
		 * Once we have the turnstile chain interlock, mark the
		 * mutex has having waiters.  If that fails, spin again:
		 * chances are that the mutex has been released.
		 */
		if (!MUTEX_SET_WAITERS(mtx, owner)) {
			turnstile_exit(mtx);
			owner = mtx->mtx_owner;
			continue;
		}

#ifdef MULTIPROCESSOR
		/*
		 * mutex_exit() is permitted to release the mutex without
		 * any interlocking instructions, and the following can
		 * occur as a result:
		 *
		 *  CPU 1: MUTEX_SET_WAITERS()      CPU2: mutex_exit()
		 * ---------------------------- ----------------------------
		 *		..		    acquire cache line
		 *		..                   test for waiters
		 *	acquire cache line    <-      lose cache line
		 *	 lock cache line	           ..
		 *     verify mutex is held                ..
		 *	    set waiters  	           ..
		 *	 unlock cache line		   ..
		 *	  lose cache line     ->    acquire cache line
		 *		..	          clear lock word, waiters 
		 *	  return success
		 *
		 * There is a another race that can occur: a third CPU could
		 * acquire the mutex as soon as it is released.  Since
		 * adaptive mutexes are primarily spin mutexes, this is not
		 * something that we need to worry about too much.  What we
		 * do need to ensure is that the waiters bit gets set.
		 *
		 * To allow the unlocked release, we need to make some
		 * assumptions here:
		 *
		 * o Release is the only non-atomic/unlocked operation
		 *   that can be performed on the mutex.  (It must still
		 *   be atomic on the local CPU, e.g. in case interrupted
		 *   or preempted).
		 *
		 * o At any given time, MUTEX_SET_WAITERS() can only ever
		 *   be in progress on one CPU in the system - guaranteed
		 *   by the turnstile chain lock.
		 *
		 * o No other operations other than MUTEX_SET_WAITERS()
		 *   and release can modify a mutex with a non-zero
		 *   owner field.
		 *
		 * o The result of a successful MUTEX_SET_WAITERS() call
		 *   is an unbuffered write that is immediately visible
		 *   to all other processors in the system.
		 *
		 * o If the holding LWP switches away, it posts a store
		 *   fence before changing curlwp, ensuring that any
		 *   overwrite of the mutex waiters flag by mutex_exit()
		 *   completes before the modification of curlwp becomes
		 *   visible to this CPU.
		 *
		 * o mi_switch() posts a store fence before setting curlwp
		 *   and before resuming execution of an LWP.
		 * 
		 * o _kernel_lock() posts a store fence before setting
		 *   curcpu()->ci_biglock_wanted, and after clearing it. 
		 *   This ensures that any overwrite of the mutex waiters
		 *   flag by mutex_exit() completes before the modification
		 *   of ci_biglock_wanted becomes visible.
		 *
		 * We now post a read memory barrier (after setting the
		 * waiters field) and check the lock holder's status again.
		 * Some of the possible outcomes (not an exhaustive list):
		 *
		 * 1. The onproc check returns true: the holding LWP is
		 *    running again.  The lock may be released soon and
		 *    we should spin.  Importantly, we can't trust the
		 *    value of the waiters flag.
		 *
		 * 2. The onproc check returns false: the holding LWP is
		 *    not running.  We now have the opportunity to check
		 *    if mutex_exit() has blatted the modifications made
		 *    by MUTEX_SET_WAITERS().
		 *
		 * 3. The onproc check returns false: the holding LWP may
		 *    or may not be running.  It has context switched at
		 *    some point during our check.  Again, we have the
		 *    chance to see if the waiters bit is still set or
		 *    has been overwritten.
		 *
		 * 4. The onproc check returns false: the holding LWP is
		 *    running on a CPU, but wants the big lock.  It's OK
		 *    to check the waiters field in this case.
		 *
		 * 5. The has-waiters check fails: the mutex has been
		 *    released, the waiters flag cleared and another LWP
		 *    now owns the mutex.
		 *
		 * 6. The has-waiters check fails: the mutex has been
		 *    released.
		 *
		 * If the waiters bit is not set it's unsafe to go asleep,
		 * as we might never be awoken.
		 */
		if ((membar_consumer(), mutex_onproc(owner, &ci)) ||
		    (membar_consumer(), !MUTEX_HAS_WAITERS(mtx))) {
			turnstile_exit(mtx);
			owner = mtx->mtx_owner;
			continue;
		}
#endif	/* MULTIPROCESSOR */

		LOCKSTAT_START_TIMER(lsflag, slptime);

		turnstile_block(ts, TS_WRITER_Q, mtx, &mutex_syncobj);

		LOCKSTAT_STOP_TIMER(lsflag, slptime);
		LOCKSTAT_COUNT(slpcnt, 1);

		owner = mtx->mtx_owner;
	}

	LOCKSTAT_EVENT(lsflag, mtx, LB_ADAPTIVE_MUTEX | LB_SLEEP1,
	    slpcnt, slptime);
	LOCKSTAT_EVENT(lsflag, mtx, LB_ADAPTIVE_MUTEX | LB_SPIN,
	    spincnt, spintime);
	LOCKSTAT_EXIT(lsflag);

	MUTEX_DASSERT(mtx, MUTEX_OWNER(mtx->mtx_owner) == curthread);
	MUTEX_LOCKED(mtx);
}

/*
 * mutex_vector_exit:
 *
 *	Support routine for mutex_exit() that handles all cases.
 */
void
mutex_vector_exit(kmutex_t *mtx)
{
	turnstile_t *ts;
	uintptr_t curthread;

	if (MUTEX_SPIN_P(mtx)) {
#ifdef FULL
		if (__predict_false(!__SIMPLELOCK_LOCKED_P(&mtx->mtx_lock))) {
			if (panicstr != NULL)
				return;
			MUTEX_ABORT(mtx, "exiting unheld spin mutex");
		}
		MUTEX_UNLOCKED(mtx);
		__cpu_simple_unlock(&mtx->mtx_lock);
#endif
		MUTEX_SPIN_SPLRESTORE(mtx);
		return;
	}

	if (__predict_false((uintptr_t)panicstr | cold)) {
		MUTEX_UNLOCKED(mtx);
		MUTEX_RELEASE(mtx);
		return;
	}

	curthread = (uintptr_t)curlwp;
	MUTEX_DASSERT(mtx, curthread != 0);
	MUTEX_ASSERT(mtx, MUTEX_OWNER(mtx->mtx_owner) == curthread);
	MUTEX_UNLOCKED(mtx);

#ifdef LOCKDEBUG
	/*
	 * Avoid having to take the turnstile chain lock every time
	 * around.  Raise the priority level to splhigh() in order
	 * to disable preemption and so make the following atomic.
	 */
	{
		int s = splhigh();
		if (!MUTEX_HAS_WAITERS(mtx)) {
			MUTEX_RELEASE(mtx);
			splx(s);
			return;
		}
		splx(s);
	}
#endif

	/*
	 * Get this lock's turnstile.  This gets the interlock on
	 * the sleep queue.  Once we have that, we can clear the
	 * lock.  If there was no turnstile for the lock, there
	 * were no waiters remaining.
	 */
	ts = turnstile_lookup(mtx);

	if (ts == NULL) {
		MUTEX_RELEASE(mtx);
		turnstile_exit(mtx);
	} else {
		MUTEX_RELEASE(mtx);
		turnstile_wakeup(ts, TS_WRITER_Q,
		    TS_WAITERS(ts, TS_WRITER_Q), NULL);
	}
}

#ifndef __HAVE_SIMPLE_MUTEXES
/*
 * mutex_wakeup:
 *
 *	Support routine for mutex_exit() that wakes up all waiters.
 *	We assume that the mutex has been released, but it need not
 *	be.
 */
void
mutex_wakeup(kmutex_t *mtx)
{
	turnstile_t *ts;

	ts = turnstile_lookup(mtx);
	if (ts == NULL) {
		turnstile_exit(mtx);
		return;
	}
	MUTEX_CLEAR_WAITERS(mtx);
	turnstile_wakeup(ts, TS_WRITER_Q, TS_WAITERS(ts, TS_WRITER_Q), NULL);
}
#endif	/* !__HAVE_SIMPLE_MUTEXES */

/*
 * mutex_owned:
 *
 *	Return true if the current LWP (adaptive) or CPU (spin)
 *	holds the mutex.
 */
int
mutex_owned(kmutex_t *mtx)
{

	if (mtx == NULL)
		return 0;
	if (MUTEX_ADAPTIVE_P(mtx))
		return MUTEX_OWNER(mtx->mtx_owner) == (uintptr_t)curlwp;
#ifdef FULL
	return __SIMPLELOCK_LOCKED_P(&mtx->mtx_lock);
#else
	return 1;
#endif
}

/*
 * mutex_owner:
 *
 *	Return the current owner of an adaptive mutex.  Used for
 *	priority inheritance.
 */
lwp_t *
mutex_owner(kmutex_t *mtx)
{

	MUTEX_ASSERT(mtx, MUTEX_ADAPTIVE_P(mtx));
	return (struct lwp *)MUTEX_OWNER(mtx->mtx_owner);
}

/*
 * mutex_tryenter:
 *
 *	Try to acquire the mutex; return non-zero if we did.
 */
int
mutex_tryenter(kmutex_t *mtx)
{
	uintptr_t curthread;

	/*
	 * Handle spin mutexes.
	 */
	if (MUTEX_SPIN_P(mtx)) {
		MUTEX_SPIN_SPLRAISE(mtx);
#ifdef FULL
		if (__cpu_simple_lock_try(&mtx->mtx_lock)) {
			MUTEX_WANTLOCK(mtx);
			MUTEX_LOCKED(mtx);
			return 1;
		}
		MUTEX_SPIN_SPLRESTORE(mtx);
#else
		MUTEX_WANTLOCK(mtx);
		MUTEX_LOCKED(mtx);
		return 1;
#endif
	} else {
		curthread = (uintptr_t)curlwp;
		MUTEX_ASSERT(mtx, curthread != 0);
		if (MUTEX_ACQUIRE(mtx, curthread)) {
			MUTEX_WANTLOCK(mtx);
			MUTEX_LOCKED(mtx);
			MUTEX_DASSERT(mtx,
			    MUTEX_OWNER(mtx->mtx_owner) == curthread);
			return 1;
		}
	}

	return 0;
}

#if defined(__HAVE_SPIN_MUTEX_STUBS) || defined(FULL)
/*
 * mutex_spin_retry:
 *
 *	Support routine for mutex_spin_enter().  Assumes that the caller
 *	has already raised the SPL, and adjusted counters.
 */
void
mutex_spin_retry(kmutex_t *mtx)
{
#ifdef MULTIPROCESSOR
	u_int count;
	LOCKSTAT_TIMER(spintime);
	LOCKSTAT_FLAG(lsflag);
#ifdef LOCKDEBUG
	u_int spins = 0;
#endif	/* LOCKDEBUG */

	MUTEX_WANTLOCK(mtx);

	LOCKSTAT_ENTER(lsflag);
	LOCKSTAT_START_TIMER(lsflag, spintime);
	count = SPINLOCK_BACKOFF_MIN;

	/*
	 * Spin testing the lock word and do exponential backoff
	 * to reduce cache line ping-ponging between CPUs.
	 */
	do {
		if (panicstr != NULL)
			break;
		while (__SIMPLELOCK_LOCKED_P(&mtx->mtx_lock)) {
			SPINLOCK_BACKOFF(count); 
#ifdef LOCKDEBUG
			if (SPINLOCK_SPINOUT(spins))
				MUTEX_ABORT(mtx, "spinout");
#endif	/* LOCKDEBUG */
		}
	} while (!__cpu_simple_lock_try(&mtx->mtx_lock));

	LOCKSTAT_STOP_TIMER(lsflag, spintime);
	LOCKSTAT_EVENT(lsflag, mtx, LB_SPIN_MUTEX | LB_SPIN, 1, spintime);
	LOCKSTAT_EXIT(lsflag);

	MUTEX_LOCKED(mtx);
#else	/* MULTIPROCESSOR */
	MUTEX_ABORT(mtx, "locking against myself");
#endif	/* MULTIPROCESSOR */
}
#endif	/* defined(__HAVE_SPIN_MUTEX_STUBS) || defined(FULL) */

/*
 * mutex_obj_init:
 *
 *	Initialize the mutex object store.
 */
void
mutex_obj_init(void)
{

	mutex_obj_cache = pool_cache_init(sizeof(struct kmutexobj),
	    coherency_unit, 0, 0, "mutex", NULL, IPL_NONE, mutex_obj_ctor,
	    NULL, NULL);
}

/*
 * mutex_obj_ctor:
 *
 *	Initialize a new lock for the cache.
 */
static int
mutex_obj_ctor(void *arg, void *obj, int flags)
{
	struct kmutexobj * mo = obj;

	mo->mo_magic = MUTEX_OBJ_MAGIC;

	return 0;
}

/*
 * mutex_obj_alloc:
 *
 *	Allocate a single lock object.
 */
kmutex_t *
mutex_obj_alloc(kmutex_type_t type, int ipl)
{
	struct kmutexobj *mo;

	mo = pool_cache_get(mutex_obj_cache, PR_WAITOK);
	mutex_init(&mo->mo_lock, type, ipl);
	mo->mo_refcnt = 1;

	return (kmutex_t *)mo;
}

/*
 * mutex_obj_hold:
 *
 *	Add a single reference to a lock object.  A reference to the object
 *	must already be held, and must be held across this call.
 */
void
mutex_obj_hold(kmutex_t *lock)
{
	struct kmutexobj *mo = (struct kmutexobj *)lock;

	KASSERT(mo->mo_magic == MUTEX_OBJ_MAGIC);
	KASSERT(mo->mo_refcnt > 0);

	atomic_inc_uint(&mo->mo_refcnt);
}

/*
 * mutex_obj_free:
 *
 *	Drop a reference from a lock object.  If the last reference is being
 *	dropped, free the object and return true.  Otherwise, return false.
 */
bool
mutex_obj_free(kmutex_t *lock)
{
	struct kmutexobj *mo = (struct kmutexobj *)lock;

	KASSERT(mo->mo_magic == MUTEX_OBJ_MAGIC);
	KASSERT(mo->mo_refcnt > 0);

	if (atomic_dec_uint_nv(&mo->mo_refcnt) > 0) {
		return false;
	}
	mutex_destroy(&mo->mo_lock);
	pool_cache_put(mutex_obj_cache, mo);
	return true;
}
