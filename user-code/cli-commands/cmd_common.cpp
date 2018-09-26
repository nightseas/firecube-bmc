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

const CLI_Command_t cmd_fru =
{
    "fru",
    "\r\nfru:\r\n Show system information.\r\n",
    command_callback_fru,
    0
};

const CLI_Command_t cmd_cpu =
{
    "cpu",
    "\r\ncpu:\r\n Show cpu information and statistics.\r\n",
    command_callback_cpu,
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
    ret += nightcli_command_register( &cmd_fru );
    ret += nightcli_command_register( &cmd_cpu );
    ret += nightcli_command_register( &cmd_echo );
    ret += nightcli_command_register( &cmd_i2c );
    ret += nightcli_command_register( &cmd_repeater );
    ret += nightcli_command_register( &cmd_rgpr );
    ret += nightcli_command_register( &cmd_ioh );
    ret += nightcli_command_register( &cmd_nvs );
    return ret;
}

/*-----------------------------------------------------------*/

int command_callback_fru( char *command_output, int output_buf_len, const char *command_string )
{
    boardlib_info_dump();
    return 0;
}

/*-----------------------------------------------------------*/

int command_callback_cpu( char *command_output, int output_buf_len, const char *command_string )
{
    sys_cpu_info_dump();
    serial_debug.printf(" CPU core clock frequency = %d Hz.\n\r\n\r", SystemCoreClock);
    serial_debug.printf(" CPU Usage: [");
    for (int ucnt = 0; ucnt < (int)sys_cpu_usage / 5; ucnt++)
    {
        serial_debug.printf("|");
    }
    for (int ucnt = 0; ucnt < (20 - (int)sys_cpu_usage / 5); ucnt++)
    {
        serial_debug.printf(" ");
    }
    serial_debug.printf("] %.01f %%\n\r\n\r", sys_cpu_usage);
    
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


