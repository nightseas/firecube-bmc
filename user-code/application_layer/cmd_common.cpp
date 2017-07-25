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

/*-----------------------------------------------------------*/

// Register CLI commands
// Return: 0 - succeeded, non-0 - failed
int cli_commands_init( void )
{
    int ret = 0;
    ret += nightcli_command_register( &cmd_sysinfo );
    ret += nightcli_command_register( &cmd_echo );
    ret += nightcli_command_register( &cmd_i2c );
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

