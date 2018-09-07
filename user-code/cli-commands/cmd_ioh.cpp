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

// IO Hub data transfer length
#define IOH_BUFF_SIZE    3

/*-----------------------------------------------------------*/

const CLI_Command_t cmd_ioh =
{
    "ioh",
    "\r\nioh:\r\n IO Hub register access operations...\r\n \
    ioh set <addr> <data> - IOH reg write operation\r\n \
    ioh get <addr> - IOH reg read operation\r\n \
    <addr> 7-bit reg address (0x00-0x7F)\r\n \
    <data> 16-bit data\r\n",
    command_callback_ioh,
    -1 // The user can enter any number of commands.
};


/*-----------------------------------------------------------*/

int command_callback_ioh( char *command_output, int output_buf_len, const char *command_string )
{    
    int param_string_len, ret = 0;
    const char *pt_parameter;        
    char ioh_buf[ IOH_BUFF_SIZE ];
    
    int ioh_addr = 0, ioh_data = 0;

    ( void ) command_string;
    ( void ) output_buf_len;
    if( command_output == NULL )
        return -1;
    
    // Get prameter 1: set|get
    pt_parameter = nightcli_command_get_param( command_string, 1, &param_string_len );
        
        // Just show help
    if( pt_parameter == NULL )
    {
        serial_debug.printf( cmd_ioh.command_help_string );
        ret = -2;
    }
    
    // Command: ioh set <addr> <data> - IOH reg write operation
    // <addr> 7-bit reg address (0x00-0x7F)
    // <data> 16-bit data
    if( strncmp( pt_parameter, "set", param_string_len) == 0 )
    {                   
        // Get prameter 2: IOH reg addr
        pt_parameter = nightcli_command_get_param( command_string, 2, &param_string_len );
        if ( !( pt_parameter != NULL && sscanf(pt_parameter, "%x", &ioh_addr ) == 1 && ioh_addr >= 0x00 && ioh_addr <= 0x7F ) )
        {
            serial_debug.printf( "IOH: Invalid reg addr. Enter 'help' for more information.\n\r\n\r" );
            return -3;            
        }
                
        // Get prameter 2: Data to be written to IOH
        pt_parameter = nightcli_command_get_param( command_string, 3, &param_string_len );
        if ( !( pt_parameter != NULL && sscanf(pt_parameter, "%x", &ioh_data ) == 1 ) )
        {
            serial_debug.printf( "IOH: No data given. Enter 'help' for more information.\n\r\n\r" );
            return -4;            
        }
                
        ioh_buf[0] = ioh_addr;
        ioh_buf[1] = ioh_data >> 8;
        ioh_buf[2] = ioh_data & 0xFF;
        
        spi_ms1_cs = 0;
        spi_ms1.write(ioh_buf[0] & 0x7F);
        spi_ms1.write(ioh_buf[1]);
        spi_ms1.write(ioh_buf[2]);
        spi_ms1_cs = 1;

        ret = 0;
    }
    
    // Command: ioh get <addr> - IOH reg read operation
    // <addr> 7-bit reg address (0x00-0x7F)
    else if( strncmp( pt_parameter, "get", param_string_len) == 0 )
    {
        // Get prameter 2: IOH reg addr
        pt_parameter = nightcli_command_get_param( command_string, 2, &param_string_len );
        if ( !( pt_parameter != NULL && sscanf(pt_parameter, "%x", &ioh_addr ) == 1 && ioh_addr >= 0x00 && ioh_addr <= 0x7F ) )
        {
            serial_debug.printf( "IOH: Invalid reg addr. Enter 'help' for more information.\n\r\n\r" );
            return -5;            
        }
                
        ioh_buf[0] = ioh_addr;
        
        spi_ms1_cs = 0;
        spi_ms1.write(ioh_buf[0] | 0x80);
        ioh_data = spi_ms1.write(0x00) << 8;
        ioh_data |= spi_ms1.write(0x00);
        spi_ms1_cs = 1;
        
        serial_debug.printf( "IOH: Read reg <0x%02X> = 0x%04X.\n\r\n\r" , ioh_addr, ioh_data);
                
        ret = 0;
    }
    
    // Incorrect command
    else
    {
        serial_debug.printf( "IOH: invalid parameter.\n\r" );
        serial_debug.printf( cmd_ioh.command_help_string );
        ret = -10;
    }   

    return ret;
}
