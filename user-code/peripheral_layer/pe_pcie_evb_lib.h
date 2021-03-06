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
 
#ifndef _PE_BD_LIB_
#define _PE_BD_LIB_

/*-----------------------------------------------------------*/

// On-board LED number
#define LED_NUM_MAX    4
// I2C master number
#define I2C_MS_MAX     1

/*-----------------------------------------------------------*/

// LEDs on mother board and daughter board
//extern DigitalOut led_mb1, led_mb2, led_mb3, led_mb4, led_mb5, led_mb6, led_mb7, led_mb8;
extern DigitalOut led_mb2, led_mb3, led_mb4, led_mb5, led_mb6, led_mb7, led_mb8;
extern PwmOut led_mb1_pwm;

// USART connect to PC for debug/cli
extern Serial serial_debug; // CLI interface on debug header

// I2C master for repeater config
extern I2C i2c_ms1;

// I2C slave connected to OCuLink remote control channels
extern I2CSlave i2c_slv1, i2c_slv2;

// SPI master for repeater config
extern SPI spi_ms1;
// SPI HW CS bug: see "STM32 SPI: fix NSS pin configuration #6951"
extern DigitalOut spi_ms1_cs;

// Misc control signals
extern DigitalIn reset_ocl1, reset_ocl2, reset_ocl3, reset_ocl4; // PCIe reset input from OCuLink
extern DigitalIn present_slot1, present_slot2; // PCIe present input from PCIe slot
extern DigitalIn pgood_3v; // Power good singal from DC/DC

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
