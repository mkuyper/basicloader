// Copyright (C) 2020-2020 Michael Kuyper. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package.

#ifndef _bootloader_impl_h_
#define _bootloader_impl_h_

#include "bootloader.h"
#include "boottab.h"

extern uint32_t _ebl;
extern uint32_t _efw;
#define BOOT_FW_START   ((uint32_t) (&_ebl))
#define BOOT_FW_END     ((uint32_t) (&_efw))

#endif
