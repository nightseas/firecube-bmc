/*
 *  I2C Debug Command ( Night CLI Command )
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

#define CMD_I2C_BUS2            1
#define CMD_I2C_CHECK_DEV_ID    1

#define I2C_BUFF_SIZE           16

/*-----------------------------------------------------------*/

const CLI_Command_t cmd_i2c =
{
    "i2c",
    "\r\ni2c:\r\n I2C bus operations...\r\n \
    i2c detect <bus> - detect I2C devices on specific bus\r\n \
    i2c set <bus> <dev> <values...> - I2C write operation\r\n \
    i2c get <bus> <dev> <len> [reg...] - I2C read operation\r\n\n\r \
    <bus> 1 - I2C1, 2 - I2C2\r\n \
    <dev> 7-bit I2C device address (0x03-0x77)\r\n \
    <len> bytes to read (1-16 bytes)\r\n \
    <regs> register address (0-16 bytes)\r\n\r\n",
    command_callback_i2c,
    -1 // The user can enter any number of commands.
};

/*-----------------------------------------------------------*/

int command_callback_i2c( char *command_output, int output_buf_len, const char *command_string )
{	
    int param_string_len, ret = 0;
    const char *pt_parameter;        
    char i2c_buf[ I2C_BUFF_SIZE ];
    
    ( void ) command_string;
    ( void ) output_buf_len;
    if( command_output == NULL )
        return -1;
    
    // Get prameter 1: detect|set|get
    pt_parameter = nightcli_command_get_param( command_string, 1, &param_string_len );
    
    // Just show help
    if( pt_parameter == NULL )
    {
        serial_debug.printf( cmd_i2c.command_help_string );
        ret = -2;
    }
    
    // Command: i2c detect <bus> - detect I2C devices on specific bus
    else if( strncmp( pt_parameter, "detect", param_string_len) == 0 )
    {          
        int bus_num;
        // Get prameter 2: bus number
        pt_parameter = nightcli_command_get_param( command_string, 2, &param_string_len );
             
        if (pt_parameter != NULL && sscanf(pt_parameter, "%d", &bus_num) == 1)
        {
            if( bus_num == 1)
            {
                serial_debug.printf( "I2C: Bus 1 selected.\n\r" );
            }            
#if( I2C_MS_MAX > 1 )
            else if( bus_num == 2 )
            {
                serial_debug.printf( "I2C: Bus 2 selected.\n\r" );
            }
#endif
            else
            {
                serial_debug.printf( "I2C: Invalid bus number. Enter 'help' for more information.\n\r\n\r" );
                return -3;
            }
        }
        else
        {
            serial_debug.printf( "I2C: Invalid bus number. Enter 'help' for more information.\n\r\n\r" );
            return -4;
        }
        
        serial_debug.printf( "\n\rI2C: detecting on bus %d...\n\r", bus_num );
            
        // Column head
        serial_debug.printf( "     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n\r" );
        for(int id=0x03; id<0x78; id++)
        {
            // Row head
            switch( id )
            {
                case 0x03:
                    serial_debug.printf( "00:          " );
                break;
                case 0x10:
                case 0x20:
                case 0x30:
                case 0x40:
                case 0x50:
                case 0x60:
                case 0x70:
                    serial_debug.printf( "\n\r%02X: ", id );
        
                break;                    
            }
            
            // ACK received from an 8-bit slave address
            if( bus_num == 1)
            {
                ret = i2c_ms1.write( id << 1, i2c_buf, 0 );
            }
#if( I2C_MS_MAX > 1 )
            else if( bus_num == 2 )
            {
                ret = i2c_ms2.write( id << 1, i2c_buf, 0 );                
            }
#endif
            if( ret == 0 )
            {                        
                serial_debug.printf( "%02X ", id );
    
            }
            else
            {
                serial_debug.printf( "-- " );
            }
        }
        serial_debug.printf( "\n\r\n\r" );
        ret = 0;
    }
    
    // Command: i2c set <bus> <dev> <values...> - I2C write
    // dev: 7-bit I2C device address (0x03-0x77)
    // values: bytes to write (1-16 bytes)
    else if( strncmp( pt_parameter, "set", param_string_len) == 0 )
    {
        int bus_num, dev_id, write_buf[ I2C_BUFF_SIZE ];
        // Get prameter 2: bus number
        pt_parameter = nightcli_command_get_param( command_string, 2, &param_string_len );
             
        if (pt_parameter != NULL && sscanf(pt_parameter, "%d", &bus_num) == 1)
        {
            if( bus_num == 1)
            {
                serial_debug.printf( "I2C: Bus 1 selected.\n\r" );
            }            
#if( I2C_MS_MAX > 1 )
            else if( bus_num == 2 )
            {
                serial_debug.printf( "I2C: Bus 2 selected.\n\r" );
            }
#endif
            else
            {
                serial_debug.printf( "I2C: Invalid bus number. Enter 'help' for more information.\n\r\n\r" );
                return -3;
            }
        }        
        else
        {
            serial_debug.printf( "I2C: Invalid bus number. Enter 'help' for more information.\n\r\n\r" );
            return -4;
        }
        
                    
        // Get prameter 3: I2C slave addr
        pt_parameter = nightcli_command_get_param( command_string, 3, &param_string_len );
        if ( !( pt_parameter != NULL && sscanf(pt_parameter, "%x", &dev_id ) == 1 && dev_id >= 0x03 && dev_id <= 0x77 ) )
        {
            serial_debug.printf( "I2C: Invalid device ID. Enter 'help' for more information.\n\r\n\r" );
            return -5;            
        }

#ifdef CMD_I2C_CHECK_DEV_ID
        if( bus_num == 1)
        {
            ret = i2c_ms1.write( dev_id << 1, i2c_buf, 0 );
        }
#if( I2C_MS_MAX > 1 )
        else if( bus_num == 2 )
        {
            ret = i2c_ms2.write( dev_id << 1, i2c_buf, 0 );                
        }
#endif        
        if( ret != 0 )
        {
            serial_debug.printf( "I2C: No response from 0x%02X!\n\r\n\r", dev_id );
            return -6;
        }
#endif

        int write_len = nightcli_command_get_param_num( command_string ) - 3;
        if( write_len < 1 )
        {
            serial_debug.printf( "I2C: Need at least one vaule to write. Enter 'help' for more information.\n\r\n\r" );
            return -7;
        }
        if( write_len > I2C_BUFF_SIZE )
        {
            write_len = I2C_BUFF_SIZE;
        }
        
        for( int index = 0; index < write_len; index ++ )
        {
            // Get value(s) to be written to I2C device, start from param 4
            pt_parameter = nightcli_command_get_param( command_string, index + 4, &param_string_len );
            
            if( !(sscanf(pt_parameter, "%x", &write_buf[ index ] ) == 1 && write_buf[ index ] >= 0x00 && write_buf[ index ] <= 0xFF) )
            {
                serial_debug.printf( "I2C: Invalid value to write. Enter 'help' for more information.\n\r\n\r" );
                return -8;
            }
            i2c_buf[ index ] = write_buf[ index ] & 0xFF;
        }


        if( bus_num == 1)
        {
            ret = i2c_ms1.write( dev_id << 1, i2c_buf, write_len );
        }
#if( I2C_MS_MAX > 1 )
        else if( bus_num == 2 )
        {
            ret = i2c_ms2.write( dev_id << 1, i2c_buf, write_len );
        }
#endif 
        if( ret != 0 )
        {
            serial_debug.printf( "I2C: write to bus %d, device 0x%02X failed!\n\r\n\r", bus_num, dev_id );
            return -9;
        }
        else
        {
            serial_debug.printf( "Data written: " );
            for ( int i = 0; i < write_len; i ++ )
            {
                serial_debug.printf( "0x%02X ", i2c_buf[ i ] );    
            }
            serial_debug.printf( "\n\r\n\r" );
        }
        ret = 0;
    }
    
    // Command: i2c get <bus> <dev> <len> [reg... ] - I2C read
    // dev: 7-bit I2C device address (0x03-0x77)
    // len: bytes to read (1-16 bytes)
    // regs: register address (0-16 bytes)
    else if( strncmp( pt_parameter, "get", param_string_len) == 0 )
    {
        int bus_num, dev_id, read_len, write_buf[ I2C_BUFF_SIZE ];
        // Get prameter 2: bus number
        pt_parameter = nightcli_command_get_param( command_string, 2, &param_string_len );
             
        if (pt_parameter != NULL && sscanf(pt_parameter, "%d", &bus_num) == 1)
        {
            if( bus_num == 1)
            {
                serial_debug.printf( "I2C: Bus 1 selected.\n\r" );
            }            
#if( I2C_MS_MAX > 1 )
            else if( bus_num == 2 )
            {
                serial_debug.printf( "I2C: Bus 2 selected.\n\r" );
            }
#endif
            else
            {
                serial_debug.printf( "I2C: Invalid bus number. Enter 'help' for more information.\n\r\n\r" );
                return -3;
            }
        }        
        else
        {
            serial_debug.printf( "I2C: Invalid bus number. Enter 'help' for more information.\n\r\n\r" );
            return -4;
        }
        
                    
        // Get prameter 3: I2C slave addr
        pt_parameter = nightcli_command_get_param( command_string, 3, &param_string_len );
        if ( !( pt_parameter != NULL && sscanf(pt_parameter, "%x", &dev_id ) == 1 && dev_id >= 0x03 && dev_id <= 0x77 ) )
        {
            serial_debug.printf( "I2C: Invalid device ID. Enter 'help' for more information.\n\r\n\r" );
            return -5;            
        }
        
        // Get prameter 4: read length
        pt_parameter = nightcli_command_get_param( command_string, 4, &param_string_len );        
        if ( !( pt_parameter != NULL && sscanf(pt_parameter, "%d", &read_len) == 1 && read_len >= 1 && read_len <= 16 ) )
        {
            serial_debug.printf( "I2C: Invalid read length. Enter 'help' for more information.\n\r\n\r" );
            return -11;            
        }

#ifdef CMD_I2C_CHECK_DEV_ID
        if( bus_num == 1)
        {
            ret = i2c_ms1.write( dev_id << 1, i2c_buf, 0 );
        }
#if( I2C_MS_MAX > 1 )
        else if( bus_num == 2 )
        {
            ret = i2c_ms2.write( dev_id << 1, i2c_buf, 0 );                
        }
#endif        
        if( ret != 0 )
        {
            serial_debug.printf( "I2C: No response from 0x%02X!\n\r\n\r", dev_id );
            return -6;
        }
#endif

        int write_len = nightcli_command_get_param_num( command_string ) - 4;
        serial_debug.printf( "\n\ri2c-write len = %d\n\r", write_len);
        serial_debug.printf( "\n\ri2c-read len = %d\n\r", read_len);
        if( write_len > I2C_BUFF_SIZE )
            write_len = I2C_BUFF_SIZE;
        
        for( int index = 0; index < write_len; index ++ )
        {
            // Get register address to be written to I2C device, start from param 5
            pt_parameter = nightcli_command_get_param( command_string, index + 5, &param_string_len );
            
            if( !( sscanf( pt_parameter, "%x", &write_buf[ index ] ) == 1 && write_buf[ index ] >= 0x00 && write_buf[ index ] <= 0xFF ) )
            {
                serial_debug.printf( "I2C: Invalid value to write. Enter 'help' for more information.\n\r\n\r" );
                return -7;
            }
            i2c_buf[ index ] = write_buf[ index ] & 0xFF;
        }

#if 0        
        serial_debug.printf( "\n\ri2c-write buffer:" );
        for ( int i = 0; i < write_len; i ++ )
        {
            serial_debug.printf( "0x%02X ", i2c_buf[ i ] );
        }
        serial_debug.printf( "\n\r\n\r" );
#endif
        
        if( write_len > 0 )
        {
            if( bus_num == 1)
            {
                ret = i2c_ms1.write( dev_id << 1, i2c_buf, write_len );
            }
#if( I2C_MS_MAX > 1 )
            else if( bus_num == 2 )
            {
                ret = i2c_ms2.write( dev_id << 1, i2c_buf, write_len );
            }
#endif 
            if( ret != 0 )
            {
                serial_debug.printf( "I2C: write to bus %d, device 0x%02X failed!\n\r\n\r", bus_num, dev_id );
                return -8;
            }
        }
        
        if( bus_num == 1)
        {
            ret = i2c_ms1.read( dev_id << 1, i2c_buf, read_len );
        }
#if( I2C_MS_MAX > 1 )
        else if( bus_num == 2 )
        {
            ret = i2c_ms2.read( dev_id << 1, i2c_buf, read_len );
        }
#endif 
        if( ret != 0 )
        {
            serial_debug.printf( "I2C: Read from bus %d, device 0x%02X failed!\n\r\n\r", bus_num, dev_id );
            return -9;
        }
        
        serial_debug.printf( "Data read: " );
        for ( int i = 0; i < read_len; i ++ )
        {
            serial_debug.printf( "0x%02X ", i2c_buf[ i ] );

        }
        serial_debug.printf( "\n\r\n\r" );
        
        ret = 0;
    }
    
    // Incorrect command
    else
    {
        serial_debug.printf( "I2C: invalid parameter.\n\r" );
				serial_debug.printf( cmd_i2c.command_help_string );
        ret = -10;
    }   

    return ret;
}
