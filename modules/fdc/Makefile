# $FreeBSD: releng/10.3/sys/modules/fdc/Makefile 193530 2009-06-05 18:44:36Z jkim $

KMOD=	fdc

.if ${MACHINE} == "pc98"
.PATH:  ${.CURDIR}/../../pc98/cbus
SRCS=	fdc.c fdc_cbus.c
.else
.PATH:  ${.CURDIR}/../../dev/fdc
SRCS=	fdc.c fdc_isa.c fdc_pccard.c
.if ${MACHINE} == "i386" || ${MACHINE} == "amd64"
SRCS+=	opt_acpi.h acpi_if.h fdc_acpi.c
.endif
.endif

SRCS+=	opt_fdc.h bus_if.h card_if.h device_if.h \
	isa_if.h pccarddevs.h

FDC_DEBUG?=	0	# 0/1

.if ${FDC_DEBUG} > 0
	echo "#define FDC_DEBUG 1" >> ${.TARGET}
.endif

.include <bsd.kmod.mk>
