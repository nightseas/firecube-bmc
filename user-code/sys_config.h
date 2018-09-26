/*
 *  System Configuration
 *
 *  Copyright (C) 2017, Xiaohai Li (haixiaolee@gmail.com), All Rights Reserved
 *  This program is lincensed under Apache-2.0/GPLv3 dual-license
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Apache-2.0 License and GNU General Public License for more details.
 *
 *  You may obtain a copy of Apache-2.0 License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 * 
 *  And GPLv3 License at
 *
 *  http://www.gnu.org/licenses
 *
 */
 
#ifndef _SYS_CONFIG_
#define _SYS_CONFIG_

// System Includings

// mbed & RTOS lib
#include "mbed.h"
#include "rtos.h"

// mbed TLS lib for crypto & hash
#include "mbedtls/md.h"

// Files system lib
#include "SPIFBlockDevice.h"
#include "HeapBlockDevice.h"
#include "BlockDevice.h"
#include "LittleFileSystem.h"

// Standard C lib
#include <string.h>
#include <stdint.h>

/*-----------------------------------------------------------*/

// Application Layer Modules

// Threads and handler functions
#include "th_common.h"
#include "th_cli.h"
#include "th_ioctrl.h"

// Night-CLI and CLI relative commands
#include "nightcli.h"
#include "cmd_common.h"
#include "cmd_i2c.h"
#include "cmd_repeater.h"
#include "cmd_gpr.h"
#include "cmd_ioh.h"
#include "cmd_nvs.h"

/*-----------------------------------------------------------*/

// Peripheral Layer Modules

// Board lib
#include "pe_pcie_evb_lib.h"
// DS80PCIxxx Peripheral Lib
#include "pe_ds80.h"
// STM32F4 UUID Peripheral Lib
#include "pe_f4_uuid.h"
// NV Storage File System Peripheral Lib
#include "pe_nvs.h"

/*-----------------------------------------------------------*/

// Enable NV storage simulation with heap block device
#define NVS_HEAP_SIM        0

// Enable CPU statistics
// TODO: DO NOT enable it on FPGA-DevKit since the function relys on RTC/LSE
// No LSE is implemented on FPGA-DevKit HW, so the neither CPU usage nor RTC function will work
#define CPU_STAT_LSE        0

#endif
