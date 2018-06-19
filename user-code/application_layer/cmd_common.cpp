/*
 *  Common Night CLI Commands
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

const CLI_Command_t cmd_sysinfo =
{
    "sysinfo",
    "\r\nsysinfo:\r\n Show system information.\r\n",
    command_callback_sysinfo,
    0
};

const CLI_Command_t cmd_echo =
{
    "echo",
    "\r\necho <...>:\r\n Echos each input paramters.\r\n",
    command_callback_echo,
    -1 // The user can enter any number of commands.
};

const CLI_Command_t cmd_cpld =
{
    "cpld",
    "\r\ncpld:\r\n CPLD register access operations...\r\n \
    cpld set <addr> <data> - CPLD reg write operation\r\n \
    cpld get <addr> - CPLD reg read operation\r\n \
    <addr> 7-bit reg address (0x00-0x7F)\r\n \
    <data> 16-bit data\r\n",
    command_callback_cpld,
    -1 // The user can enter any number of commands.
};


/*-----------------------------------------------------------*/

// Register CLI commands
// Return: 0 - succeeded, non-0 - failed
int cli_commands_init( void )
{
    int ret = 0;
    ret += nightcli_command_register( &cmd_sysinfo );
    ret += nightcli_command_register( &cmd_echo );
    ret += nightcli_command_register( &cmd_i2c );
    ret += nightcli_command_register( &cmd_repeater );
    ret += nightcli_command_register( &cmd_rgpr );
		ret += nightcli_command_register( &cmd_cpld );
    return ret;
}

/*-----------------------------------------------------------*/

int command_callback_sysinfo( char *command_output, int output_buf_len, const char *command_string )
{
    // You can either print info inside the function
    boardlib_info_dump();
    // Or fill the info into output buffer
    sprintf(command_output, " System core clock frequency = %d Hz.\n\r\n\r", SystemCoreClock);
    return 0;
}

/*-----------------------------------------------------------*/

int command_callback_echo( char *command_output, int output_buf_len, const char *command_string )
{
	char temp_output_buff[ CLI_MAX_OUTPUT_SIZE + 1 ]; 
    const char *pt_parameter;
    int param_string_len, ret = 0;
    int param_index = 0;
    
    ( void ) command_string;
    ( void ) output_buf_len;
    if( command_output == NULL )
        return -1;

	sprintf( command_output, "The parameters were:\r\n" );
	param_index ++;
		
	while (1)
	{
		pt_parameter = nightcli_command_get_param
						(
							command_string,
							param_index,        // Specify which parameter is needed
							&param_string_len   // Get the parameter string length
						);

        if( pt_parameter != NULL )
        {
            // Return the parameter string
            sprintf( temp_output_buff, "  Param <%d>: ", ( int ) param_index );			
            strncat( temp_output_buff, pt_parameter, param_string_len );
            strcat( temp_output_buff, "\r\n" );
			
			strcat( command_output, temp_output_buff );			
            param_index++;
        }
		else
		{
			break;
		}
	}

    return ret;
}

/*-----------------------------------------------------------*/
#define CPLD_BUFF_SIZE	3
int command_callback_cpld( char *command_output, int output_buf_len, const char *command_string )
{	
    int param_string_len, ret = 0;
    const char *pt_parameter;        
    char cpld_buf[ CPLD_BUFF_SIZE ];
	
		int cpld_addr = 0, cpld_data = 0;
    
    ( void ) command_string;
    ( void ) output_buf_len;
    if( command_output == NULL )
        return -1;
    
    // Get prameter 1: set|get
    pt_parameter = nightcli_command_get_param( command_string, 1, &param_string_len );
		
		// Just show help
    if( pt_parameter == NULL )
    {
        serial_debug.printf( cmd_cpld.command_help_string );
        ret = -2;
    }
    
    // Command: cpld set <addr> <data> - CPLD reg write operation
    // <addr> 7-bit reg address (0x00-0x7F)
    // <data> 16-bit data
    if( strncmp( pt_parameter, "set", param_string_len) == 0 )
    {                   
        // Get prameter 2: CPLD reg addr
        pt_parameter = nightcli_command_get_param( command_string, 2, &param_string_len );
        if ( !( pt_parameter != NULL && sscanf(pt_parameter, "%x", &cpld_addr ) == 1 && cpld_addr >= 0x00 && cpld_addr <= 0x7F ) )
        {
            serial_debug.printf( "CPLD: Invalid reg addr. Enter 'help' for more information.\n\r\n\r" );
            return -3;            
        }
				
				// Get prameter 2: Data to be written to CPLD
				pt_parameter = nightcli_command_get_param( command_string, 3, &param_string_len );
        if ( !( pt_parameter != NULL && sscanf(pt_parameter, "%x", &cpld_data ) == 1 ) )
        {
            serial_debug.printf( "CPLD: No data given. Enter 'help' for more information.\n\r\n\r" );
            return -4;            
        }
				
				cpld_buf[0] = cpld_addr;
				cpld_buf[1] = cpld_data >> 8;
				cpld_buf[2] = cpld_data & 0xFF;
				
				spi_ms1_cs = 0;
				spi_ms1.write(cpld_buf[0] & 0x7F);
				spi_ms1.write(cpld_buf[1]);
				spi_ms1.write(cpld_buf[2]);
				spi_ms1_cs = 1;

        ret = 0;
    }
    
    // Command: cpld get <addr> - CPLD reg read operation
    // <addr> 7-bit reg address (0x00-0x7F)
    else if( strncmp( pt_parameter, "get", param_string_len) == 0 )
    {
        // Get prameter 2: CPLD reg addr
        pt_parameter = nightcli_command_get_param( command_string, 2, &param_string_len );
        if ( !( pt_parameter != NULL && sscanf(pt_parameter, "%x", &cpld_addr ) == 1 && cpld_addr >= 0x00 && cpld_addr <= 0x7F ) )
        {
            serial_debug.printf( "CPLD: Invalid reg addr. Enter 'help' for more information.\n\r\n\r" );
            return -5;            
        }
				
				cpld_buf[0] = cpld_addr;
				
				spi_ms1_cs = 0;
				spi_ms1.write(cpld_buf[0] | 0x80);
				cpld_data = spi_ms1.write(0x00) << 8;
				cpld_data |= spi_ms1.write(0x00);
				spi_ms1_cs = 1;
				
				serial_debug.printf( "CPLD: Read reg <0x%02X> = 0x%04X.\n\r\n\r" , cpld_addr, cpld_data);
				
        ret = 0;
    }
    
    // Incorrect command
    else
    {
        serial_debug.printf( "CPLD: invalid parameter.\n\r" );
				serial_debug.printf( cmd_cpld.command_help_string );
        ret = -10;
    }   

    return ret;
}

