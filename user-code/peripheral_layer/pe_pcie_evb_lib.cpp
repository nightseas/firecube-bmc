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

#define I2C_SW_ADDR        0x70

/*-----------------------------------------------------------*/

// LEDs on mother board and daughter board
//DigitalOut led_mb1( PA_0 ), led_mb2( PA_1 ), led_mb3( PA_2 ), led_mb4( PA_3 );
DigitalOut led_mb2( PA_1 ), led_mb3( PA_2 ), led_mb4( PA_3 );
PwmOut led_mb1_pwm( PA_0 );

// USART connect to PC for debug/cli
Serial serial_debug( PA_9, PA_10 ); // CLI interface on debug header

// I2C master for repeater config
I2C i2c_ms1( PB_4, PA_8 );

// I2C slave connected to OCuLink remote control channels
I2CSlave i2c_sl1( PB_3, PB_10 ), i2c_sl2( PB_7, PB_6 );

// SPI master for repeater config
SPI spi_ms1( PA_7, PA_6, PA_5 );
// SPI HW CS bug: see "STM32 SPI: fix NSS pin configuration #6951"
DigitalOut spi_ms1_cs (PA_15, 1);

// Misc control signals
DigitalIn io_cpld0(PB_0), io_cpld1(PB_1), io_cpld2(PB_2), io_cpld3(PA_4), io_cpld4(PB_5), io_cpld5(PB_8), io_cpld6(PB_9), io_cpld7(PC_13);
//DigitalIn reset_ocl1( PB_2 ), reset_ocl2( PB_5 ), reset_ocl3( PB_8 ), reset_ocl4( PB_9 ); // PCIe reset input from OCuLink
//DigitalIn present_slot1( PB_1 ), present_slot2( PB_0 ); // PCIe present input from PCIe slot
//DigitalIn pgood_3v(  ); // Power good singal from DC/DC

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
    "FPGA-DEV-KIT",
    "N/A (haixiaolee@gmail.com)",
    x411_serial_number,
    x411_config
};

/*-----------------------------------------------------------*/

// Initialize board relative functions
// Return: 0 - succeeded, non-0 - failed
int boardlib_init( void )
{    
    //LED initial states: all on except led_mb2 which indicates 3.3V power good
    led_mb1_pwm.period_ms(1);
    led_mb1_pwm.write(0);
    //led_mb1 = 0;
    led_mb2 = 0;
    led_mb3 = 0;
    led_mb4 = 0;
    
    // Switch I2C buffer to channel 1 (repeater)
    char i2c_buf = 0x1;
    if( i2c_ms1.write( I2C_SW_ADDR << 1, &i2c_buf, 1 ) != 0 )
    
    i2c_sl1.address(0x2D << 1);
    i2c_sl2.address(0x2E << 1);
    
    // Set PC debug/cli serial port to 115200,8,n,1
    serial_debug.baud( 115200 );
    
    // Update serail number from MCU UUID
    uuid_to_sn( x411_serial_number );
    
    // Dump board info 
    boardlib_info_dump();
        
    wait_ms(500);
    
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
    led_mb1_pwm.write(1);
    //led_mb1 = 1;
    led_mb2 = 1;
    led_mb3 = 1;
    led_mb4 = 1;
    
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

