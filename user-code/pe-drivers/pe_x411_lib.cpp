/*
 *  X411 EVB Board Lib
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

// On-board LED number
const int LED_NUM_MAX = 4;

// Board serial number buffer size
const int BD_SN_LEN = 16;

// Board configuration buffer size
const int BD_CONFIG_LEN = 16;

/*-----------------------------------------------------------*/

// LEDs on mother board and daughter board
DigitalOut led_mb1(PA_5), led_mb2(PC_9), led_mb3(PC_8), led_mb4(PC_5);

// USART2 connect to PC for debug/cli
Serial serial_debug(SERIAL_TX, SERIAL_RX);

// I2C for sensors
I2C i2c_ms1(I2C_SDA, I2C_SCL);

I2CSlave i2c_sl1(PB_3, PB_10);

// Analog input channels for ADC
//AnalogIn adc_ch10(PC_0), adc_ch11(PC_1);

/*-----------------------------------------------------------*/

// Board serail number buffer
unsigned char x411_serial_number[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 16-Byte board configuration buffer
unsigned char x411_config[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// Board information struct
Board_Info_t x411_board_info =
{
    "X411 Eva Board",
    "N/A (haixiaolee@gmail.com)",
    x411_serial_number,
    x411_config
};
    
/*-----------------------------------------------------------*/

// Initialize board relative functions
// Return: 0 - succeeded, non-0 - failed
int boardlib_init( void )
{    
    // LED initial states: all on
    led_mb1 = 1;
    led_mb2 = 1;
    led_mb3 = 1;
    led_mb4 = 1;
    
    // Set PC debug/cli serial port to 115200,8,n,1
    serial_debug.baud(115200);
    
    // Update serail number from MCU UUID
    uuid_to_sn( x411_serial_number );
    
    // Dump board info after board lib init 
    boardlib_info_dump();
    
    // LED states after board lib init: all off    
    led_mb1 = 0;
    led_mb2 = 0;
    led_mb3 = 0;
    led_mb4 = 0;   

    // TBD: No failure handling for now!
    return 0;
}

/*-----------------------------------------------------------*/

// Dump board information to debug interface
void boardlib_info_dump( void )
{
    int i;
    
    serial_debug.printf("\n\r\n\r\n\r===============================\n\r=      Board Information      =\n\r===============================\n\r");
    serial_debug.printf(" Board  Name: %s\n\r", x411_board_info.board_name);
    serial_debug.printf(" Vendor Name: %s\n\r", x411_board_info.vendor_name);
    serial_debug.printf(" Serial  Num: ");
    for( i = 0; i < BD_SN_LEN; i++ )
        serial_debug.printf("%02X", x411_board_info.serial_number[i]);    
    serial_debug.printf("\n\r\n\r");
    
    serial_debug.printf(" NV   Config: ");
    for( i = 0; i < BD_CONFIG_LEN; i++ )
        serial_debug.printf("0x%02X ", x411_board_info.config[i]);    
    serial_debug.printf("\n\r\n\r");
}

