/*
 *  Night CLI ( command line interface )
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
 *
 * The command process part of Night CLI simple edition is based on FreeRTOS+CLI V1.0.3.
 * Console process part is based on u-boot CLI.
 *
 */

#ifndef _APP_NIGHT_CLI_
#define _APP_NIGHT_CLI_

/*-----------------------------------------------------------*/

// Buffer size for console input and each item in command history list
#define CLI_MAX_INPUT_SIZE      60

// Xiaohai Li: A 2K-Byte output buffer is enough for most applications.
// Usually the help command ocuppies the largest buffer size when you have dozens of command registered.
// FreeRTOS has very small buffer so that it has call the callback function of a command again and again,
// and each time the callback function do part of its work and flush the buffer so it won't overflow.
// To be frank, I don't like that way so I just put a large buffer here, but users still need to take care
// the possibility of buffer overflow by themselves :)
#define CLI_MAX_OUTPUT_SIZE     2048

// Max items of command history list
#define CLI_HIST_MAX            20

// The prompt should end with a blank space
#define CLI_PROMPT              "CLI# "

/*-----------------------------------------------------------*/

// Definition of CLI command callback function, all callback functions need to be same format
typedef int ( *CLI_COMMAND_CALLBACK )( char *output_string, int output_buf_len, const char *input_string );

// Type definition of a command
typedef struct CLI_COMMAND_TYPE
{
    const char * const command_name;                    // Name string of the command, like "help"
    const char * const command_help_string;             // Help string, a guide of how to use the command, should end with "\n\r"
    const CLI_COMMAND_CALLBACK command_callback_func;   // A pointer to the callback function which will excute when a command is typed by user
    int parameter_num;                                  // Number of paramter expected by the command ( >=0 )
} CLI_Command_t;

/*-----------------------------------------------------------*/

extern int nightcli_console_loop( RawSerial &console );
extern int nightcli_command_register( const CLI_Command_t * const command );
extern int nightcli_command_process( const char * const command_input, char * command_output, unsigned int output_buf_len  );
extern char *nightcli_command_get_outbuf( void );
extern const char *nightcli_command_get_param( const char *input_string, unsigned int param_num_expect, int *param_string_len );
extern int nightcli_command_get_param_num( const char *input_string );

#endif /* NIGHT_CLI_H */
