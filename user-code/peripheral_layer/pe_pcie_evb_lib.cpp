/*
 *  PCIe Rpeater EVB Board Lib
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

// Board serial number buffer size
const int BD_SN_LEN = 16;

// Board configuration buffer size
const int BD_CONFIG_LEN = 16;

/*-----------------------------------------------------------*/

// LEDs on mother board and daughter board
DigitalOut led_mb1( PA_0 ), led_mb2( PA_1 ), led_mb3( PA_2 ), led_mb4( PA_3 );
DigitalOut led_mb5( PB_12 ), led_mb6( PB_13 ), led_mb7( PB_14 ), led_mb8( PB_15 );

// USART connect to PC for debug/cli
Serial serial_debug( PA_9, PA_10 ); // CLI interface on debug header

// I2C master for repeater config
I2C i2c_ms1( PB_4, PA_8 );

// I2C slave connected to OCuLink remote control channels
I2CSlave i2c_sl1( PB_7, PB_6 ), i2c_sl2( PB_3, PB_10 );

// Misc control signals
DigitalIn reset_ocl1( PB_2 ), reset_ocl2( PB_5 ), reset_ocl3( PB_8 ), reset_ocl4( PB_9 ); // PCIe reset input from OCuLink
DigitalIn present_slot1( PB_0 ), present_slot2( PB_1 ); // PCIe present input from PCIe slot
DigitalIn pgood_3v( PA_15 ); // Power good singal from DC/DC

/*-----------------------------------------------------------*/

// Board serail number buffer
unsigned char x411_serial_number[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// 16-Byte board configuration buffer
unsigned char x411_config[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
// Board information struct
Board_Info_t x411_board_info =
{
    "PCIe-Repeater-EVB-Pro",
    "N/A (haixiaolee@gmail.com)",
    x411_serial_number,
    x411_config
};

/*-----------------------------------------------------------*/

// Initialize board relative functions
// Return: 0 - succeeded, non-0 - failed
int boardlib_init( void )
{    
    //LED initial states: all on
    led_mb1 = 0;
    led_mb2 = 0;
    led_mb3 = 0;
    led_mb4 = 0;
    led_mb5 = 0;
    led_mb6 = 0;
    led_mb7 = 0;
    led_mb8 = 0;
    
    // Set pull-down mode for reset from OCL to initialize the states
    reset_ocl1.mode( PullDown );
    reset_ocl2.mode( PullDown );
    reset_ocl3.mode( PullDown );
    reset_ocl4.mode( PullDown );
    
    // Set PC debug/cli serial port to 115200,8,n,1
    serial_debug.baud( 115200 );
    
    // Update serail number from MCU UUID
    uuid_to_sn( x411_serial_number );
    
    // Dump board info 
    boardlib_info_dump();
    
    serial_debug.printf("Waiting for 3.3V system good...\r\n\r\n");
    while(1)
    {
        if( pgood_3v == 1 )
            break;
        wait(500);
    }
        
    // Init all repeaters discovered on both two I2C buses
    if( ds80_config_set_all( i2c_ms1 ) != 0 || ds80_config_dump_all( i2c_ms1 ) != 0 )
    {
        serial_debug.printf("No repeater found on bus 1.\r\n\r\n");
    }
#if( I2C_MS_MAX > 1 )
    if( ds80_config_set_all( i2c_ms2 ) != 0 || ds80_config_dump_all( i2c_ms2 ) != 0 )
    {
        serial_debug.printf("No repeater found on bus 2.\r\n\r\n");
    }
#endif

    //LED states after board lib init: all off    
    led_mb1 = 1;
    led_mb2 = 1;
    led_mb3 = 1;
    led_mb4 = 1;
    led_mb5 = 1;
    led_mb6 = 1;
    led_mb7 = 1;
    led_mb8 = 1;
    
    // TBD: No failure handling for now!
    return 0;
}

/*-----------------------------------------------------------*/

// Dump board information to debug interface
void boardlib_info_dump( void )
{
    int i;
    
    serial_debug.printf( "\n\r\n\r\n\r===============================\n\r=      Board Information      =\n\r===============================\n\r" );    
    serial_debug.printf( " Board  Name: %s\n\r", x411_board_info.board_name );
    serial_debug.printf( " Vendor Name: %s\n\r", x411_board_info.vendor_name );
    
    serial_debug.printf( " Serial Num : " );
    for( i = 0; i < BD_SN_LEN; i++ )
        serial_debug.printf( "%02X", x411_board_info.serial_number[i] );    
    serial_debug.printf( "\n\r\n\r" );
    
    serial_debug.printf( " NV Config  : " );
    for( i = 0; i < BD_CONFIG_LEN; i++ )
        serial_debug.printf( "0x%02X ", x411_board_info.config[i] );    
    serial_debug.printf( "\n\r\n\r" );
}

