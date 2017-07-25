/*
 *  PCIe Repeater Debug Command ( Night CLI Command )
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

const CLI_Command_t cmd_repeater =
{
    "repeater",
    "\r\nrepeater:\r\n Repeater status debug.\r\n\r\n",
    command_callback_repeater,
    0
};

/*-----------------------------------------------------------*/

int command_callback_repeater( char *command_output, int output_buf_len, const char *command_string )
{

    // Init all repeaters discovered on both two I2C buses
    if( ds80_init_all( i2c_ms1, 'r' ) != 0 )
    {
        serial_debug.printf("No repeater found on bus 1.\r\n\r\n");
    }
#if( I2C_MS_MAX > 1 )
    if( ds80_init_all( i2c_ms2, 'r' ) != 0 )
    {
        serial_debug.printf("No repeater found on bus 2.\r\n\r\n");
    }
#endif
    
    return 0;
}
