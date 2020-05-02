// Copyright (C) 2020-2020 Michael Kuyper. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package.

#include "bootloader.h"

#ifndef UTIL_DELAY_SHIFT
#define UTIL_DELAY_SHIFT #20
#endif

    // --------------------------------------------
    // assembler settings
    .syntax unified
    .thumb


    // --------------------------------------------
    // void delay (uint32_t length)
    .section .boot.delay,"ax",%progbits
    .thumb_func
delay:
	lsls r0, r0, UTIL_DELAY_SHIFT
     1: subs r0, #1
	bne 1b
	bx lr

    .size delay, .-delay
    .global delay
