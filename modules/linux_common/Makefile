# $FreeBSD: releng/10.3/sys/modules/linux_common/Makefile 293575 2016-01-09 17:29:08Z dchagin $

.PATH: ${.CURDIR}/../../compat/linux

KMOD=	linux_common
SRCS=	linux_common.c linux_mib.c linux_util.c linux_emul.c \
	linux.c opt_compat.h device_if.h vnode_if.h bus_if.h

EXPORT_SYMS=
EXPORT_SYMS+=	linux_emul_path
EXPORT_SYMS+=	linux_ioctl_register_handler
EXPORT_SYMS+=	linux_ioctl_unregister_handler
EXPORT_SYMS+=	linux_get_osname
EXPORT_SYMS+=	linux_get_osrelease

.if !defined(KERNBUILDDIR)
.if defined(DEBUG)
CFLAGS+=-DDEBUG
.endif
.if defined(KTR)
CFLAGS+=-DKTR
.endif
.endif

.include <bsd.kmod.mk>
