/*
 Power Cycling Command ( Night CLI Command )
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

#ifndef _APP_CMD_SDR_
#define _APP_CMD_SDR_

/*-----------------------------------------------------------*/

//repeater command structures
extern const CLI_Command_t cmd_sdr;

/*-----------------------------------------------------------*/

//repeater command functions
extern int command_callback_sdr( char *command_output, int output_buf_len, const char *command_string );

#endif
