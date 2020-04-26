// Copyright (C) 2020-2020 Michael Kuyper. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package.

#include "bootloader_impl.h"
#include "update.h"
#include "bootloader_hw.h"
#include "sha2.h"


// force inlining of reset call
__attribute__((always_inline)) static void NVIC_SystemReset (void);


// ------------------------------------------------
// CRC-32

static void crc32 (uint32_t* pcrc, unsigned char* buf, uint32_t len) {
    int i;
    uint32_t byte, crc, mask;

    crc = ~(*pcrc);
    while( len-- != 0 ) {
	byte = *buf++;
	crc = crc ^ byte;
	for (i = 7; i >= 0; i--) {
	    mask = -(crc & 1);
	    crc = (crc >> 1) ^ (0xEDB88320 & mask);
	}
    }
    *pcrc = ~crc;
}

static uint32_t boot_crc32 (void* buf, uint32_t nwords) {
    uint32_t crc = 0;
    crc32(&crc, buf, nwords * 4);
    return crc;
}


// ------------------------------------------------
// Panic handler

__attribute__((noreturn))
void boot_panic (uint32_t type, uint32_t reason, uint32_t addr) {
    // disable all interrupts
    __disable_irq();

    // TODO - blink LED

    NVIC_SystemReset();
    // not reached
    while (1);
}

__attribute__((noreturn, naked))
static void fw_panic (uint32_t reason, uint32_t addr) {
    boot_panic(BOOT_PANIC_TYPE_FIRMWARE, reason, addr);
}


// ------------------------------------------------
// Bootloader main entry point

void* bootloader (void) {
    return NULL; // XXX
}


// ------------------------------------------------
// Bootloader information table
//
// Version history:
//
//   0x100 - initial version

const boot_boottab boottab = {
    .version	= 0x100,
    .update	= NULL, // XXX
    .panic	= fw_panic,
    .crc32      = boot_crc32,
    .sha256     = sha256,
};
