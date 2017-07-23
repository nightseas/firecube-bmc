/*
 *  Night CLI Console Process
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

// Definition of specail control chars
#define KEY_CTL( c )              ( ( c ) - 'a' + 1 )
#define KEY_ESC                 ( 0x1B )
#define KEY_BACKSPACE           ( '\b' )
#define KEY_DEL                 ( ( char )0xFF )
#define KEY_DEL7                ( ( char )0x7F )
    
#define ERASE_SEQ               "\b \b"
#define NEWLINE_SEQ             "\n\r"

#define ERASE_TO_BEGINNING()            \
{                                       \
    while( console_cur_index )            \
    {                                   \
        console.puts( ERASE_SEQ );        \
        console_cur_index--;            \
    }                                   \
}

#define REFRESH_TO_END()                            \
{                                                   \
    console.puts( console_input_buff );               \
    console_cur_index = strlen( console_input_buff ); \
}

/*-----------------------------------------------------------*/

// Input console buffer, include '\0' at end of the string
char console_input_buff[ CLI_MAX_INPUT_SIZE + 1 ];
// History command list, include '\0' at end of the string
char console_hist_buff[ CLI_HIST_MAX ][ CLI_MAX_INPUT_SIZE + 1 ];

// Cursor index for input editing
int console_cur_index, console_end_index;
// Cursor index for history command
int hist_add_index, hist_read_index, hist_max_index;

static int flag_exit_reading_loop = 0, flag_exit_cmd_loop = 0;

/*-----------------------------------------------------------*/

// Return the previous command in history list
static char * _hist_prev( void )
{
    char *ret;
    int old_hist_read_index;
    
    // Empty list
    if ( hist_read_index < 0 )
        return NULL;

    old_hist_read_index = hist_read_index;
    
    if ( --hist_read_index < 0 )
        hist_read_index = hist_max_index;

    if ( hist_read_index == hist_add_index )
    {
        hist_read_index = old_hist_read_index;
        ret = NULL;
    }
    else
    {
        ret = console_hist_buff[hist_read_index];
    }

    return ret;
}

/*-----------------------------------------------------------*/

// Return the next command in history list
// Return null string "" for user to type new command if no more next historical command
static char * _hist_next( void )
{
    char *ret;

    if ( hist_read_index < 0 )
        return NULL;

    if ( hist_read_index == hist_add_index )
        return NULL;

    if ( ++hist_read_index > hist_max_index )
        hist_read_index = 0;

    if ( hist_read_index == hist_add_index )
        ret = "";
    else
        ret = console_hist_buff[hist_read_index];

    return ret;
}

/*-----------------------------------------------------------*/

// Add a new command to history list
static void _hist_add_new( char *newline )
{
    strcpy( console_hist_buff[hist_add_index], newline );
    
    if( ++hist_add_index >= CLI_HIST_MAX )
        hist_add_index = 0;
    
    // This is the last history
    if( hist_add_index > hist_max_index )
        hist_max_index = hist_add_index;
}

/*-----------------------------------------------------------*/

// Init history list: clear the list and set indexes to default
static void _hist_init( void )
{
    int i;
        
    hist_max_index = 0;
    hist_add_index = 0;
    hist_read_index = -1;
    
    for ( i = 0; i < CLI_HIST_MAX; i++ )
    {
        console_hist_buff[i][0] = '\0';
    }
}

/*-----------------------------------------------------------*/

// This is a endless loop to catch input of console and process it
int nightcli_console_loop( Serial &console )
{
    char console_input_char;
    unsigned int esc_seq_len;
    char *pt_hist_line;
    char *pt_output_buff;

    // Get address of CLI command output buffer
    pt_output_buff = nightcli_command_get_outbuf();
    
    // Init history list and indexs
    _hist_init();
    
#if 0
    while( 1 )
    {
         while( !console.readable() )
                Thread::yield(); // Allow other threads to run            
         console_input_char = console.getc();
         //console.printf( "0x%02X ", console_input_char );    
         console.putc( console_input_char );
    }
#endif
    // Console input command handling loop
    while( 1 )
    {   
        esc_seq_len = 0;
        
        console_cur_index = 0;
        console_end_index = 0;        
        console_input_buff[0] = '\0';      
        
        console.printf( "%s", CLI_PROMPT );
        
        // Console reading & echoing loop
        while( ! flag_exit_reading_loop )
        {
            // Read a char from console when available            
            #if 0 // mbed OS 5.4: console.readable() causes USART hang while receiving large number of data! DO NOT use it.
            while( !console.readable() )
                Thread::yield(); // Allow other threads to run  
            #endif     
            
            console_input_char = console.getc(); 
            
            // Enter detected: exit reading loop and start to handle this line of command string
            if( console_input_char == '\n' || console_input_char == '\r' )
            {
                console.puts( NEWLINE_SEQ );
                break;
            }
            
            // Process esc sequence to handle arrow keys
            if( esc_seq_len != 0 )
            {
                if( esc_seq_len == 1 )
                {
                    if( console_input_char == '[' || console_input_char == 'O' )
                    {                        
                        esc_seq_len ++;
                        continue;
                    }
                    else
                    {
                        esc_seq_len = 0;
                        continue;
                    }                    
                }
                else if( esc_seq_len == 2 )
                {
                    switch( console_input_char )
                    {
                        // Arrow up
                        case 'A':                            
                        // Arrow down
                        case 'B':
                            //fetch history command 
                            if( console_input_char == 'A' )
                                pt_hist_line = _hist_prev();
                            else
                                pt_hist_line = _hist_next();
                                                       
                            if( pt_hist_line != NULL )
                            {
                                ERASE_TO_BEGINNING();
                                strcpy( console_input_buff, pt_hist_line );
                                REFRESH_TO_END();
                            }
                            esc_seq_len = 0;
                            continue;
                        
                        // Arrow left
                        case 'D':                        
                        // Arrow right
                        case 'C':    
                        // Home key
                        case 'H':                        
                        // End key
                        case 'F':                              
                        // Other keys, finish esc seq processing
                        default:
                            esc_seq_len = 0;     
                            continue;                        
                    }                        
                }
            }
            
            // Input charactor analyzing, echo, and filling into console buffer
            switch( console_input_char )
            {
                // Delete a char from buffer
                case KEY_BACKSPACE:
                case KEY_DEL:
                case KEY_DEL7:
                    if( console_cur_index > 0 )
                    {
                        console.puts( ERASE_SEQ );         
                        console_cur_index --;
                        //console_input_buff[console_cur_index] = 0;
                    }
                    break;
                    
                case KEY_ESC:
                    if( esc_seq_len == 0 )
                    {
                        esc_seq_len = 1;
                    }
                    else
                    {
                        console.puts( "NightCLI: impossible condition #876\n\r" );
                    }
                    break;
                    
                case KEY_CTL( 'c' ):
                    console_cur_index = 0;
                    flag_exit_reading_loop = 1;
                    break;
                
                // Put any other displayable char into buffer
                default:
                    if( ( console_input_char >= ' ' || console_input_char <= '~' ) && console_cur_index < CLI_MAX_INPUT_SIZE )
                    {
                        console.putc( console_input_char );
                        console_input_buff[console_cur_index] = console_input_char;
                        console_cur_index ++;
                    }
            }    
        }
        
        // Add NULL to the end of the string. I
        // If it's an empty string, then set buff[0] to NULL
        console_input_buff[console_cur_index] = '\0';
                        
        if( console_input_buff[0] != '\0' )
        {            
            //console.printf( "NightCLI: command string received, len = %d\n\r --> %s\n\r", strlen( console_input_buff ) ,console_input_buff );
            
            // Pass console buffers to command handler
            nightcli_command_process( console_input_buff, pt_output_buff, CLI_MAX_OUTPUT_SIZE );
            console.puts( pt_output_buff );
            //Copy console input to history list
            _hist_add_new( console_input_buff );
        }
        hist_read_index = hist_add_index;
        
        // To eliminate compiler warning
        if( flag_exit_cmd_loop == 1 )
            break;
    }
    
    console.printf( "NightCLI: exiting console command loop... ( could an error? )\n\r" );
    return 0;
}
