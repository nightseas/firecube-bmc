/*
 *  Night CLI Command Process
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

typedef struct CLI_COMMAND_LIST_ITEM_TYPE
{
    const CLI_Command_t *cmd;
    struct CLI_COMMAND_LIST_ITEM_TYPE *next;
} CLI_Command_List_Item_t;

/*-----------------------------------------------------------*/

static int help_command_callback( char *command_output, int output_buf_len, const char *input_string );

/*-----------------------------------------------------------*/

// Default command: help
static const CLI_Command_t cmd_help =
{
    "help",
    "\r\nhelp:\r\n Show all available commands\r\n",
    help_command_callback,
    0
};

// Default entry to all registred command
static CLI_Command_List_Item_t registered_command =
{
    &cmd_help,    // Default first command: help
    NULL          // No next command by default
};

// CLI command writes output string, which will be displayed by console, to this buffer 
char command_output_buff[ CLI_MAX_OUTPUT_SIZE + 1 ]; 

/*-----------------------------------------------------------*/

int nightcli_command_register( const CLI_Command_t * const command )
{
static CLI_Command_List_Item_t *last_command = &registered_command;
CLI_Command_List_Item_t *new_command;
int ret = -1;

    /* Check the parameter is not NULL. */
    if(command == NULL)
        return ret = -2;

    /* Create a new list item that will reference the command being registered. */
    new_command = ( CLI_Command_List_Item_t * ) malloc( sizeof( CLI_Command_List_Item_t ) );
    if(new_command == NULL)
        return ret = -3;
    
    if( new_command != NULL )
    {
        //core_util_critical_section_enter();
        {
            /* Reference the command being registered from the newly created
            list item. */
            new_command->cmd = command;

            /* The new list item will get added to the end of the list, so
            next has nowhere to point. */
            new_command->next = NULL;

            /* Add the newly created list item to the end of the already existing
            list. */
            last_command->next = new_command;

            /* Set the end of list marker to the new list item. */
            last_command = new_command;
        }
        //core_util_critical_section_exit();

        ret = 0;
    }

    return ret;
}

/*-----------------------------------------------------------*/

int nightcli_command_process( const char * const command_input, char * command_output, unsigned int output_buf_len  )
{
static const CLI_Command_List_Item_t *pt_command_item = NULL;
int ret = 0;
const char *pt_registered_cmd_string;
int registered_cmd_len;

    /* Note:  This function is not re-entrant.  It must not be called from more
    thank one task. */

    if( pt_command_item == NULL )
    {
        // Search input command in registered command list
        for( pt_command_item = &registered_command; pt_command_item != NULL; pt_command_item = pt_command_item->next )
        {
            pt_registered_cmd_string = pt_command_item->cmd->command_name;
            registered_cmd_len = strlen( pt_registered_cmd_string );

            // Check input string: the byte after command name should be a space or NULL (end of the string)
            if( ( command_input[ registered_cmd_len ] == ' ' ) || ( command_input[ registered_cmd_len ] == 0x00 ) )
            {
                if( strncmp( command_input, pt_registered_cmd_string, registered_cmd_len ) == 0 )
                {
                    // Command name matched, check parameter number
                    // If expected parameter number is -1, then no need to check this
                    if( pt_command_item->cmd->parameter_num >= 0 )
                    {
                        if( nightcli_command_get_param_num( command_input ) != pt_command_item->cmd->parameter_num )
                        {
                            ret = -1;
                        }
                    }

                    break;
                }
            }
        }
    }

    if( ( pt_command_item != NULL ) && ( ret == -1 ) )
    {
        // Comannd name matched, but with wrong parameter number
        strncpy( command_output, "Invalid parameter(s).  Enter 'help' for more information.\r\n\r\n", output_buf_len );
        pt_command_item = NULL;
    }
    else if( pt_command_item != NULL )
    {
        // Prepare output buffer for callback function
        memset( command_output, 0x00, CLI_MAX_OUTPUT_SIZE + 1 );
        // Call the callback function of the input command
        // The return value of callback function will be checked by console loop
        ret = pt_command_item->cmd->command_callback_func( command_output, output_buf_len, command_input );
        pt_command_item = NULL;        
    }
    else
    {
        // pt_command_item == NULL: command not found
        strncpy( command_output, "Unknown command.  Enter 'help' to view available commands.\r\n\r\n", output_buf_len );
        ret = -2;
    }

    return ret;
}

/*-----------------------------------------------------------*/

char *nightcli_command_get_outbuf( void )
{
    return command_output_buff;
}

/*-----------------------------------------------------------*/

// param_index = 1 means fisrt parameter.
const char *nightcli_command_get_param( const char *input_string, unsigned int param_index, int *param_string_len )
{
    unsigned int param_index_found = 0;
    const char *ret_param = NULL;

    *param_string_len = 0;

    while( param_index_found < param_index )
    {
        // Pass the first word, which is the command name itself
        while( ( ( *input_string ) != '\0' ) && ( ( *input_string ) != ' ' ) )
        {
            input_string++;
        }

        // Find the start of the next string, by passing all spaces before it
        while( ( ( *input_string ) != '\0' ) && ( ( *input_string ) == ' ' ) )
        {
            input_string++;
        }

        // A parameter string is found 
        if( *input_string != '\0' )
        {
            param_index_found++;

            // Check if this is the parameter we want
            if( param_index_found == param_index )
            {
                // Get the length of the parameter string
                ret_param = input_string;
                while( ( ( *input_string ) != '\0' ) && ( ( *input_string ) != ' ' ) )
                {
                    ( *param_string_len )++;
                    input_string++;
                }

                // It's an empty string (Is this possible after passing all the previous checking ?)
                if( *param_string_len == 0 )
                {
                    ret_param = NULL;
                }

                break;
            }
        }
        else
        {
            break;
        }
    }

    return ret_param;
}

/*-----------------------------------------------------------*/

static int help_command_callback( char *command_output, int output_buf_len, const char *input_string )
{
    static const CLI_Command_List_Item_t * pt_command_item = NULL;
    int ret = 0;
    
    // Eliminate compiling warning of unsued variable
    ( void ) input_string;

    // Start from beginning of the list, which should be help command itself
    pt_command_item = &registered_command;

    
    do {
        // Copy the help string to output buffer and move to next command
        strcat( command_output, pt_command_item->cmd->command_help_string );
        pt_command_item = pt_command_item->next;
    // walk through the whole command list
    } while( pt_command_item != NULL );

    return ret;
}

/*-----------------------------------------------------------*/

int nightcli_command_get_param_num( const char *input_string )
{
int parameter_num_found = 0;
int flag_is_last_char_space = 0;

    // Count the number of space delimited words in input_string
    while( *input_string != '\0' )
    {
        if( ( *input_string ) == ' ' )
        {
            if( flag_is_last_char_space != 1 )
            {
                parameter_num_found++;
                flag_is_last_char_space = 1;
            }
        }
        else
        {
            flag_is_last_char_space = 0;
        }

        input_string++;
    }

    // If the command string ended with spaces, 
    // then there will have been too many parameters counted
    if( flag_is_last_char_space == 1 )
    {
        parameter_num_found--;
    }

    // The value returned is one less than the number of space delimited words,
    // as the first word should be the command itself
    return parameter_num_found;
}

