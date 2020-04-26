include $(MKDIR)/arm.mk

VPATH		+= $(SRCDIR)/arm/nrf5
VPATH		+= $(SRCDIR)/common
VPATH		+= $(SRCDIR)/common/micro-ecc

SRCS		+= bootloader.c
#SRCS		+= util.S
SRCS		+= startup.S

#SRCS		+= update.c
SRCS		+= sha2.c
#SRCS		+= lz4.c

ifneq (,$(findstring NRF52,$(MCU)))
    MCPU	:= cortex-m4
else
    $(error Unsupported MCU: $(MCU))
endif

DEFS		+= $(MCU)

DEFS		+= LZ4_PAGEBUFFER_SZ=128
DEFS		+= UP_PAGEBUFFER_SZ=128

FLAGS		+= -mcpu=$(MCPU)
FLAGS		+= -I$(SRCDIR)/common

CFLAGS		+= -Wall
CFLAGS		+= -Os
CFLAGS		+= -I$(SRCDIR)/arm/nrf5/mdk

LDFLAGS		+= -mcpu=$(MCPU)
LDFLAGS		+= -T$(SRCDIR)/arm/nrf5/ld/nrf52832_xxaa.ld
LDFLAGS		+= -T$(SRCDIR)/arm/nrf5/ld/nrf52832.ld

OBJS		= $(addsuffix .o,$(basename $(SRCS)))

bootloader: $(OBJS)

bootloader.hex: bootloader
	$(HEX) $< $@

default: bootloader.hex


clean:
	rm -f *.o *.d *.map bootloader bootloader.hex bootloader.bin

.PHONY: clean default


MAKE_DEPS       := $(MAKEFILE_LIST)     # before we include all the *.d files

-include $(OBJS:.o=.d)

$(OBJS): $(MAKE_DEPS)
