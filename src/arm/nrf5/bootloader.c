// Copyright (C) 2020-2020 Michael Kuyper. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package.

#include "bootloader_impl.h"
#include "update.h"
#include "bootloader_hw.h"
#include "sha2.h"


// ------------------------------------------------
// CRC-32

static void crc32 (uint32_t* pcrc, unsigned char* buf, uint32_t len) {
    int i;
    uint32_t byte, crc, mask;

    crc = ~(*pcrc);
    while( len-- != 0 ) {
        byte = *buf++;
        crc = crc ^ byte;
        for( i = 7; i >= 0; i-- ) {
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

// force inlining of reset call
__attribute__((always_inline)) static void NVIC_SystemReset (void);

#if defined(BOOT_LED_GPIO)

#define LED_INIT(gpio)  do { \
    PORT(gpio)->PIN_CNF[PIN(gpio)] = 3; \
} while( 0 )

#define LED_ON(gpio)  do { \
    *(((gpio) & GPIO_F_ACTLOW) ? &PORT(gpio)->OUTCLR \
            : &PORT(gpio)->OUTSET) = (1 << PIN(gpio)); \
} while( 0 )

#define LED_OFF(gpio)  do { \
    *(((gpio) & GPIO_F_ACTLOW) ? &PORT(gpio)->OUTSET \
            : &PORT(gpio)->OUTCLR) = (1 << PIN(gpio)); \
} while( 0 )

extern void delay (int); // provided by util.S

static void blink_value (uint32_t v) {
    // blink nibble-by-nibble
    // least-significant-nibble first, 0x0 -> 1 blink, 0xf -> 16 blinks
    do {
        uint32_t n = v & 0xf;
        do {
            LED_ON(BOOT_LED_GPIO);
            delay(6);
            LED_OFF(BOOT_LED_GPIO);
            delay(6);
        } while( n-- );
        v >>= 4;
        delay(12);
    } while( v );
}

static void blink_panic (uint32_t type, uint32_t reason, uint32_t addr) {
    LED_INIT(BOOT_LED_GPIO);

    int repeat = 3;
    while( repeat-- > 0 ) {
        // blink long
        LED_ON(BOOT_LED_GPIO);
        delay(30);
        LED_OFF(BOOT_LED_GPIO);
        delay(30);
        // blink type
        blink_value(type);
        delay(30);
        // blink reason
        blink_value(reason);
        delay(30);
        // blink address
        blink_value(addr);
        delay(30);
    }
}
#endif

__attribute__((noreturn))
void boot_panic (uint32_t type, uint32_t reason, uint32_t addr) {
    // disable all interrupts
    __disable_irq();

    // TODO - blink LED
#if defined(BOOT_LED_GPIO)
    blink_panic(type, reason, addr);
#endif

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
    boot_panic(BOOT_PANIC_TYPE_BOOTLOADER, BOOT_PANIC_REASON_CRC, 0); // XXX
}


// ------------------------------------------------
// Bootloader information table
//
// Version history:
//
//   0x100 - initial version

const boot_boottab boottab = {
    .version    = 0x100,
    .update     = NULL, // XXX
    .panic      = fw_panic,
    .crc32      = boot_crc32,
    .sha256     = sha256,
};
