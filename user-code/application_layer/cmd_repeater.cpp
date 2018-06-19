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
    "\r\nrepeater:\r\n DS80PCIxxx repeater config & status operations...\r\n \
    repeater set - Auto-config RX EQ & TX DEM for all repeaters\r\n \
    repeater get - Dump RX EQ & TX DEM value for all repeaters\r\n \
    repeater status - Dump status for all repeaters\r\n",
    command_callback_repeater,
    1
};

/*-----------------------------------------------------------*/

int command_callback_repeater( char *command_output, int output_buf_len, const char *command_string )
{
    int param_string_len, ret = 0;
    const char *pt_parameter;
    
    // Get prameter 1: set|get|status
    pt_parameter = nightcli_command_get_param( command_string, 1, &param_string_len );
	
		// Just show help
    if( pt_parameter == NULL )
    {
        serial_debug.printf( cmd_repeater.command_help_string );
        ret = -2;
    }
   
    // Command: repeater set - Auto-config RX EQ & TX DEM for all repeaters
    if( strncmp( pt_parameter, "set", param_string_len) == 0 )
    {        
        // Init all repeaters discovered on both two I2C buses
        if( ds80_config_set_all( i2c_ms1 ) != 0 )
        {
            serial_debug.printf("No repeater found on bus 1.\r\n\r\n");
        }
        #if( I2C_MS_MAX > 1 )
        if( ds80_config_set_all( i2c_ms2 ) != 0 )
        {
            serial_debug.printf("No repeater found on bus 2.\r\n\r\n");
        }
        #endif
    }
    else if( strncmp( pt_parameter, "get", param_string_len) == 0 )
    {
        // Dump EQ & DEM of all repeaters discovered on both two I2C buses
        if( ds80_config_dump_all( i2c_ms1 ) != 0 )
        {
            serial_debug.printf("No repeater found on bus 1.\r\n\r\n");
        }
        #if( I2C_MS_MAX > 1 )
        if( ds80_config_dump_all( i2c_ms2 ) != 0 )
        {
            serial_debug.printf("No repeater found on bus 2.\r\n\r\n");
        }
        #endif
    }
    else if( strncmp( pt_parameter, "status", param_string_len) == 0 )
    {
        // Dump status of all repeaters discovered on both two I2C buses
        if( ds80_status_dump_all( i2c_ms1 ) != 0 )
        {
            serial_debug.printf("No repeater found on bus 1.\r\n\r\n");
        }
        #if( I2C_MS_MAX > 1 )
        if( ds80_status_dump_all( i2c_ms2 ) != 0 )
        {
            serial_debug.printf("No repeater found on bus 2.\r\n\r\n");
        }
        #endif
    }
    else
    {
        // Incorrect or NULL paratmeter: show help
				serial_debug.printf( "Repeater: invalid parameter(s).\n\r" );
        serial_debug.printf( cmd_repeater.command_help_string );
        ret = -10;
    }
    return ret;
}
