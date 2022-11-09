/*
 *  sdr Debug Command ( Night CLI Command )
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

const CLI_Command_t cmd_sdr =
{
    "sdr",
    "\r\nsdr:\r\n Read onboard sensor values.\r\n\r\n",
    command_callback_sdr,
    0 // The user can enter any number of commands.
};

/*-----------------------------------------------------------*/

int command_callback_sdr( char *command_output, int output_buf_len, const char *command_string )
{
    //Clear output buffer
    command_output[0] = '\0';

    serial_debug.printf("\n\r\n\r\n\r===============================\n\r=       Senor Readings        =\n\r===============================\n\r");
    serial_debug.printf("       Name         |  Raw   |   Val   |  Unit\n\r");
    serial_debug.printf(" SSD Power Volt <0> | 0x%04X |  %5.02f  |  V\n\r", ssd_vmon0.read_u16(), ssd_vmon0.read() * 3.3f * 2.0f );
    serial_debug.printf(" SSD Power Volt <1> | 0x%04X |  %5.02f  |  V\n\r", ssd_vmon1.read_u16(), ssd_vmon1.read() * 3.3f * 2.0f );
    // serial_debug.printf(" SSD Power Volt <2> | 0x%04X |  %5.02f  |  V\n\r", ssd_vmon2.read_u16(), ssd_vmon2.read()*3.3f );
    // serial_debug.printf(" SSD Power Volt <3> | 0x%04X |  %5.02f  |  V\n\r", ssd_vmon3.read_u16(), ssd_vmon3.read()*3.3f );
    serial_debug.printf(" MCU Int Ref Volt   | 0x%04X |  %5.02f  |  V\n\r", mcu_vref.read_u16(), mcu_vref.read()*3.3f );
    serial_debug.printf(" MCU Temp Sens Volt | 0x%04X |  %5.02f  |  C\n\r", mcu_vtemp.read_u16(), (mcu_vtemp.read() * 3.3f - 0.76f) / 0.0025f + 25.0f );


    serial_debug.printf("\n\r\n\r");

    return 0;
}
