APPLICATION = openwsn_test
include ../Makefile.tests_common

USEPKG += openwsn

USEMODULE += at86rf2xx

CFLAGS += -DTHREAD_STACKSIZE_MAIN=10240
CFLAGS += -DDAGROOT

include $(RIOTBASE)/Makefile.include
