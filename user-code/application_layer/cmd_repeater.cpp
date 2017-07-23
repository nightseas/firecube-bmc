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

// Enable second I2C bus
#define RPT_I2C_BUS2

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

    int ds80_i2c_devs[8];
    int dev_num;
    
    //Clear output buffer
    command_output[0] = '\0';
    
    memset( ds80_i2c_devs, 0, 8);
    dev_num = ds80_scan_dev( i2c_ms1, ds80_i2c_devs );
    
    if( dev_num > 0 )
    {
        for( int i=0; i< dev_num; i++ )
        {            
            ds80_set_eq_dem ( i2c_ms1, ds80_i2c_devs[i], 'r' );
            ds80_dump_eq_dem( i2c_ms1, ds80_i2c_devs[i] );
        }
    }
    else
    {
        serial_debug.printf("No repeater found on I2C bus 1.\n\r\n\r");
    }
    
#ifdef RPT_I2C_BUS2
    memset( ds80_i2c_devs, 0, 8);
    dev_num = ds80_scan_dev( i2c_ms2, ds80_i2c_devs );
    
    if( dev_num > 0 )
    {
        for( int i=0; i< dev_num; i++ )
        {           
            ds80_set_eq_dem ( i2c_ms2, ds80_i2c_devs[i], 'r' );
            ds80_dump_eq_dem( i2c_ms2, ds80_i2c_devs[i] );
        }
    }
    else
    {
        serial_debug.printf("No repeater found on I2C bus 2.\n\r\n\r");
    }
#endif
    
    return 0;
}
