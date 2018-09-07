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
    
    serial_debug.printf(" CPLD IO <0>     : %c\n\r", GET_GPR_STATUS(io_cpld0));
    serial_debug.printf(" CPLD IO <1>     : %c\n\r", GET_GPR_STATUS(io_cpld1));
    serial_debug.printf(" CPLD IO <2>     : %c\n\r", GET_GPR_STATUS(io_cpld2));
    serial_debug.printf(" CPLD IO <3>     : %c\n\r", GET_GPR_STATUS(io_cpld3));
    serial_debug.printf(" CPLD IO <4>     : %c\n\r", GET_GPR_STATUS(io_cpld4));
    serial_debug.printf(" CPLD IO <5>     : %c\n\r", GET_GPR_STATUS(io_cpld5));
    serial_debug.printf(" CPLD IO <6>     : %c\n\r", GET_GPR_STATUS(io_cpld6));
    serial_debug.printf(" CPLD IO <7>     : %c\n\r", GET_GPR_STATUS(io_cpld7));
    
    serial_debug.printf("\n\r\n\r");
    
    return 0;
}

