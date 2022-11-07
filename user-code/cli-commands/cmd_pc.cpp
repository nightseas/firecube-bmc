/*
 *  pc Debug Command ( Night CLI Command )
 *
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

const CLI_Command_t cmd_pc =
{
    "pc",
    "\r\npc:\r\n Power cycling operations...\r\n \
    pc <channel> <cycles> <time>\r\n \
    <channel> Power control channel, 0 - CH0, 1 - CH1\r\n \
    <cycles> Number of power cycles, e.g. 1000 - 1000 times of power OFF and ON\r\n \
    <time> Time of power cycle in millisec, e.g. 100 - stay 100ms OFF and then 100ms ON \r\n\r\n",
    command_callback_pc,
    -1 // The user can enter any number of commands.
};

/*-----------------------------------------------------------*/

int command_callback_pc( char *command_output, int output_buf_len, const char *command_string )
{
    int param_string_len, ret = 0;
    const char *pt_parameter;
    
    ( void ) command_string;
    ( void ) output_buf_len;
    if( command_output == NULL )
        return -1;
    
    // Get prameter 1: channel
    pt_parameter = nightcli_command_get_param( command_string, 1, &param_string_len );
    
    // Just show help
    if( pt_parameter == NULL )
    {
        serial_debug.printf( cmd_pc.command_help_string );
        ret = -2;
    }

    int ch_num, cycle_num, delay_time;

    // Get prameter 1: channel number
    if (pt_parameter != NULL && sscanf(pt_parameter, "%d", &ch_num) == 1)
    {
        if( ch_num < 0 || ch_num > 1 )
        {
            serial_debug.printf( "pc: Invalid channel number. Enter 'help' for more information.\n\r\n\r" );
            return -3;
        }
    }
    else
    {
        serial_debug.printf( "pc: Invalid channel number. Enter 'help' for more information.\n\r\n\r" );
        return -4;
    }

    // Get prameter 2: cycle number
    pt_parameter = nightcli_command_get_param( command_string, 2, &param_string_len );

    if (pt_parameter != NULL && sscanf(pt_parameter, "%d", &cycle_num) == 1)
    {
        if( cycle_num < 1)
        {
            serial_debug.printf( "pc: Invalid cycle number. Enter 'help' for more information.\n\r\n\r" );
            return -5;
        }
    }
    else
    {
        serial_debug.printf( "pc: Invalid cycle number. Enter 'help' for more information.\n\r\n\r" );
        return -6;
    }
    // Get prameter 3: delay time
    pt_parameter = nightcli_command_get_param( command_string, 3, &param_string_len );

    if (pt_parameter != NULL && sscanf(pt_parameter, "%d", &delay_time) == 1)
    {
        // No conditioning required, continue
    }
    else
    {
        serial_debug.printf( "pc: Invalid cycle number. Enter 'help' for more information.\n\r\n\r" );
        return -8;
    }

    serial_debug.printf( "pc: Starting back-to-back power cycling...\n\r -> Channel: %d, Loop Num: %d, On/Off time %d ms.\n\r\n\r", ch_num, cycle_num, delay_time );

    for (int i=0; i<cycle_num*2; i++)
    {
        switch(ch_num)
        {
            case 0:
                ssd_pwr_en0 = !ssd_pwr_en0;
                ThisThread::sleep_for(delay_time);
            break;

            case 1:
                ssd_pwr_en1 = !ssd_pwr_en1;
                ThisThread::sleep_for(delay_time);
            break;

            default:
                serial_debug.printf( "pc: Invalid channel number. Cannot find the specific power channel.\n\r\n\r" );
                return -9;
        }
    }



    return ret;
}
