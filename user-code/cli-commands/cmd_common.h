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
 
#ifndef _APP_CMD_COMMON_
#define _APP_CMD_COMMON_

/*-----------------------------------------------------------*/

//common command structures
extern const CLI_Command_t cmd_fru;
extern const CLI_Command_t cmd_echo;

/*-----------------------------------------------------------*/

//common command functions
extern int cli_commands_init( void );
extern int command_callback_fru( char *command_output, int output_buf_len, const char *command_string );
extern int command_callback_echo( char *command_output, int output_buf_len, const char *command_string );

#endif

