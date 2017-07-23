/*
 *  GPIO Debug Commands ( Night CLI Command )
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

#include "sys_config.h"

/*-----------------------------------------------------------*/

// Get integer value of GPIO status from a DigitalIn instance
#define GET_GPR_STATUS( x )   ( x == 0 ? '0' : '1' )

/*-----------------------------------------------------------*/

int command_callback_rgpr( char *command_output, int output_buf_len, const char *command_string );

const CLI_Command_t cmd_rgpr =
{
    "rgpr",
    "\r\nrgpr:\r\n Read GPIO registers or status.\r\n\r\n",
    command_callback_rgpr,
    0
};

/*-----------------------------------------------------------*/

int command_callback_rgpr( char *command_output, int output_buf_len, const char *command_string )
{
    //Clear output buffer
    command_output[0] = '\0';

    serial_debug.printf("\n\r\n\r\n\r===============================\n\r=        GPIO Registers       =\n\r===============================\n\r");
    
    serial_debug.printf(" PGOOD_MB      : %c\n\r", GET_GPR_STATUS(pgood_mb));
    serial_debug.printf(" PGOOD_DB      : %c\n\r", GET_GPR_STATUS(pgood_db));
    serial_debug.printf(" OCL1_PGOOD    : %c\n\r", GET_GPR_STATUS(pgood_ocl1));
    serial_debug.printf(" OCL2_PGOOD    : %c\n\r", GET_GPR_STATUS(pgood_ocl2)); 
    
    serial_debug.printf(" PERST_CPU_L   : %c\n\r", GET_GPR_STATUS(reset_cpu));
    serial_debug.printf(" PERST_PCH_L   : %c\n\r", GET_GPR_STATUS(reset_pch));
    serial_debug.printf(" OCL1_RST#     : %c\n\r", GET_GPR_STATUS(reset_ocl1));
    serial_debug.printf(" OCL2_RST#     : %c\n\r", GET_GPR_STATUS(reset_ocl2));
    serial_debug.printf(" NVME1_RST#    : %c\n\r", GET_GPR_STATUS(reset_nvme1));
    
    serial_debug.printf(" OCL1_PRSNT#   : %c\n\r", GET_GPR_STATUS(present_ocl1));
    serial_debug.printf(" OCL2_PRSNT#   : %c\n\r", GET_GPR_STATUS(present_ocl2));
    
    serial_debug.printf(" OCL1_WAKE#    : %c\n\r", GET_GPR_STATUS(wake_ocl1));
    serial_debug.printf(" OCL1_WAKE#    : %c\n\r", GET_GPR_STATUS(wake_ocl2));
    serial_debug.printf(" NVME1_WAKE#   : %c\n\r", GET_GPR_STATUS(wake_nvme1));
    
    serial_debug.printf("\n\r\n\r");
    
    return 0;
}

