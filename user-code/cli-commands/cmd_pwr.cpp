/*
 *  pwr Debug Command ( Night CLI Command )
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

const CLI_Command_t cmd_pwr =
{
    "pwr",
    "\r\npwr:\r\n Power cycling operations...\r\n \
    pwr <on|off> <channel>\r\n \
    <channel> Power control channel, 0 - CH0, 1 - CH1\r\n\r\n",
    command_callback_pwr,
    -1 // The user can enter any number of commands.
};

/*-----------------------------------------------------------*/

int command_callback_pwr( char *command_output, int output_buf_len, const char *command_string )
{
    int param_string_len, ret = 0;
    const char *pt_parameter;
    
    ( void ) command_string;
    ( void ) output_buf_len;
    if( command_output == NULL )
        return -1;
    
    int ch_num, pwr_on_flag = 0;

    // Get prameter 1: on|off
    pt_parameter = nightcli_command_get_param( command_string, 1, &param_string_len );

    if( pt_parameter == NULL )
    {
        serial_debug.printf( cmd_pwr.command_help_string );
        ret = -2;
    }    
    // Command: i2c on <ch> - detect I2C devices on specific bus
    else if( strncmp( pt_parameter, "on", param_string_len) == 0 )
    {
        pwr_on_flag = 1;
    }
    else if( strncmp( pt_parameter, "off", param_string_len) == 0 )
    {
        pwr_on_flag = 0;
    }
    else
    {
        serial_debug.printf( cmd_pwr.command_help_string );
        ret = -21;
    }    

    pt_parameter = nightcli_command_get_param( command_string, 2, &param_string_len );

    // Get prameter 2: channel number
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


    serial_debug.printf( "pwr: Turn %s power of channel %d...\n\r\n\r", pwr_on_flag?"ON":"OFF", ch_num);


    switch(ch_num)
    {
        case 0:
            // Relay control: LOW is connected, and HIGH is open circuit of power
            ssd_pwr_en0 = pwr_on_flag ? 0 : 1;
        break;

        case 1:
            ssd_pwr_en1 = pwr_on_flag ? 0 : 1;
        break;

        default:
            serial_debug.printf( "pwr: Invalid channel number. Cannot find the specific power channel.\n\r\n\r" );
            return -9;
    }

    return ret;
}
