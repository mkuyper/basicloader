// Copyright (C) 2020-2020 Michael Kuyper. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package.

#ifndef _boottab_h_
#define _boottab_h_

#include "bootloader.h"

// Bootloader information table on nRF5

typedef struct {
    uint32_t version;                                   // version of boot loader (values below 256 are reserved for legacy bootloaders)

    uint32_t* vector;                                   // address of the bootloader IRQ vector

    __attribute__((noreturn))
        void (*panic) (uint32_t reason, uint32_t addr); // bootloader panic function

    uint32_t (*update) (void* ptr, hash32* hash);       // function to set firmware update pointer

    uint32_t (*crc32) (void* buf, uint32_t nwords);     // calculate CRC32

    void (*sha256) (uint32_t* hash,                     // SHA-256
            const uint8_t* msg, uint32_t len);
} boot_boottab;

#endif
