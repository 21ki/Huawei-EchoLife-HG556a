#
# Makefile for the Linux networking core.
#

ifneq ($(strip $(BRCM_KERNEL_NETQOS)),)
CFLAGS  += -DSUPPORT_TC
endif

obj-y := sock.o skbuff.o iovec.o datagram.o stream.o scm.o

obj-$(CONFIG_SYSCTL) += sysctl_net_core.o

obj-y		     += flow.o dev.o ethtool.o dev_mcast.o dst.o \
			neighbour.o rtnetlink.o utils.o link_watch.o filter.o

obj-$(CONFIG_SYSFS) += net-sysfs.o
obj-$(CONFIG_NETFILTER) += netfilter.o
obj-$(CONFIG_NET_DIVERT) += dv.o
obj-$(CONFIG_NET_PKTGEN) += pktgen.o
obj-$(CONFIG_NET_RADIO) += wireless.o
obj-$(CONFIG_NETPOLL) += netpoll.o
