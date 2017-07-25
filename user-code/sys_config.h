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
#include "mbed.h"
#include "rtos.h"
#include "mbedtls/md.h"
#include <string.h>
#include <stdint.h>

/*-----------------------------------------------------------*/

// Application Layer Modules

// Threads and handler functions
#include "th_common.h"
#include "th_cli.h"

// Night-CLI and CLI relative commands
#include "nightcli.h"
#include "cmd_common.h"
#include "cmd_i2c.h"

/*-----------------------------------------------------------*/

// Peripheral Layer Modules

// Board lib
#include "pe_x411_lib.h"
// STM32F4 UUID Peripheral Lib
#include "pe_f4_uuid.h"

#endif