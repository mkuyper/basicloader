include $(MKDIR)/arm.mk

VPATH		+= $(SRCDIR)/arm/nrf5
VPATH		+= $(SRCDIR)/common

SRCS		+= bootloader.c
SRCS		+= util.S
SRCS		+= startup.S

#SRCS		+= update.c
SRCS		+= sha2.c
#SRCS		+= lz4.c

ifneq (,$(findstring NRF52,$(MCU)))
    MCPU	:= cortex-m4
else
    $(error Unsupported MCU: $(MCU))
endif

ifneq (,$(findstring NRF52832,$(MCU)))
    SOFTDEVICE	:= s132
endif

SOFTDEVICE_HEX	:= $(SRCDIR)/arm/nrf5/softdevice/$(SOFTDEVICE)/hex/softdevice.hex

DEFS		+= $(MCU)

DEFS		+= LZ4_PAGEBUFFER_SZ=128
DEFS		+= UP_PAGEBUFFER_SZ=128

FLAGS		+= -mcpu=$(MCPU)
FLAGS		+= -I$(SRCDIR)/common

CFLAGS		+= -Wall
CFLAGS		+= -Os
CFLAGS		+= -I$(SRCDIR)/arm/nrf5/mdk
CFLAGS		+= -I$(SRCDIR)/arm/nrf5/softdevice/mbr/headers
CFLAGS		+= -I$(SRCDIR)/arm/nrf5/softdevice/$(SOFTDEVICE)/headers

LDFLAGS		+= -mcpu=$(MCPU)
LDFLAGS		+= -T$(SRCDIR)/arm/nrf5/ld/nrf52832_xxaa.ld
LDFLAGS		+= -T$(SRCDIR)/arm/nrf5/ld/nrf52832.ld

OBJS		= $(addsuffix .o,$(basename $(SRCS)))

bootloader: $(OBJS)

bootloader0.hex: bootloader
	$(HEX) $< $@

bootloader.hex: bootloader0.hex $(SOFTDEVICE_HEX)
	srec_cat $(foreach x,$^,$(x) -Intel) -Output $@ -Intel

default: bootloader.hex


clean:
	rm -f *.o *.d *.map bootloader bootloader*.hex

.PHONY: clean default


MAKE_DEPS       := $(MAKEFILE_LIST)     # before we include all the *.d files

-include $(OBJS:.o=.d)

$(OBJS): $(MAKE_DEPS)
