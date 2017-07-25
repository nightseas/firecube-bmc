/*
 *  Fennec BMC Board Lib
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

#ifndef _PE_BD_LIB_
#define _PE_BD_LIB_

/*-----------------------------------------------------------*/

// On-board LED number
#define LED_NUM_MAX    2
// I2C master number
#define I2C_MS_MAX     1

/*-----------------------------------------------------------*/

// LEDs on mother board and daughter board
extern DigitalOut led_mb1, led_mb2;

// USART connect to PC for debug/cli
extern Serial serial_debug; // CLI interface on debug header

// I2C master for sensors
extern I2C i2c_ms1;

//Analog input channels for ADC
//extern AnalogIn adc_ch10, adc_ch11;

/*-----------------------------------------------------------*/

// Type definition of board information
typedef struct BOARD_INFO_TYPE
{
    const char * const board_name;
    const char * const vendor_name;
    unsigned char * serial_number;
    unsigned char * config;
} Board_Info_t;

typedef struct SENSOR_EVENT_TYPE
{
    unsigned char id;
    unsigned char type;
    double value;
} Sensor_Event_t;

/*-----------------------------------------------------------*/

//Board lib functions
extern int boardlib_init( void );
extern void boardlib_info_dump( void );

#endif
