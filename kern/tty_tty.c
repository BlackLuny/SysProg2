/*-
 * Copyright (c) 2003 Poul-Henning Kamp.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: releng/10.3/sys/kern/tty_tty.c 216952 2011-01-04 10:59:38Z kib $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/sx.h>
#include <sys/vnode.h>

#include <fs/devfs/devfs.h>
#include <fs/devfs/devfs_int.h>

static	d_open_t	cttyopen;

static struct cdevsw ctty_cdevsw = {
	.d_version =	D_VERSION,
	.d_open =	cttyopen,
	.d_name =	"ctty",
};

static struct cdev *ctty;

static	int
cttyopen(struct cdev *dev, int flag, int mode, struct thread *td)
{

	return (ENXIO);
}

static void
ctty_clone(void *arg, struct ucred *cred, char *name, int namelen,
    struct cdev **dev)
{

	if (*dev != NULL)
		return;
	if (strcmp(name, "tty"))
		return;
	sx_sunlock(&clone_drain_lock);
	sx_slock(&proctree_lock);
	sx_slock(&clone_drain_lock);
	dev_lock();
	if (!(curthread->td_proc->p_flag & P_CONTROLT))
		*dev = ctty;
	else if (curthread->td_proc->p_session->s_ttyvp == NULL)
		*dev = ctty;
	else if (curthread->td_proc->p_session->s_ttyvp->v_type == VBAD ||
	    curthread->td_proc->p_session->s_ttyvp->v_rdev == NULL) {
		/* e.g. s_ttyvp was revoked */
		*dev = ctty;
	} else
		*dev = curthread->td_proc->p_session->s_ttyvp->v_rdev;
	dev_refl(*dev);
	dev_unlock();
	sx_sunlock(&proctree_lock);
}

static void
ctty_drvinit(void *unused)
{

	EVENTHANDLER_REGISTER(dev_clone, ctty_clone, 0, 1000);
	ctty = make_dev_credf(MAKEDEV_ETERNAL, &ctty_cdevsw, 0, NULL, UID_ROOT,
	    GID_WHEEL, 0666, "ctty");
}

SYSINIT(cttydev,SI_SUB_DRIVERS,SI_ORDER_MIDDLE,ctty_drvinit,NULL);