// Copyright (C) 2020-2020 Michael Kuyper. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package.

#ifndef _bootloader_hw_h_
#define _bootloader_hw_h_

#include "nrf.h"
#include "nrf_mbr.h"
#include "nrf_sdm.h"


// ------------------------------------------------
// GPIO definition
#define GPIO(n,flags)   ((n) | (flags))

#define GPIO_F_ACTLOW   (1 << 16)

#define PORTN(gpio)     ((gpio) >> 8 & 0xff)
#define PORT(gpio)      NRF_P0
#define PIN(gpio)       ((gpio) & 0xff)

#endif
