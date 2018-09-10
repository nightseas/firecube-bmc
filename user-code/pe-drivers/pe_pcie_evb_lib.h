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
extern RawSerial serial_debug; // CLI interface on debug header

// I2C master for repeater config
extern I2C i2c_ms1;

// I2C slave connected to OCuLink remote control channels
extern I2CSlave i2c_slv1, i2c_slv2;

// SPI master for repeater config
extern SPI spi_ms1;
// SPI HW CS bug: see "STM32 SPI: fix NSS pin configuration #6951"
extern DigitalOut spi_ms1_cs;

// Misc control signals
extern DigitalIn io_cpld0, io_cpld1, io_cpld2, io_cpld3, io_cpld4, io_cpld5, io_cpld6, io_cpld7;
//extern DigitalIn reset_ocl1, reset_ocl2, reset_ocl3, reset_ocl4; // PCIe reset input from OCuLink
//extern DigitalIn present_slot1, present_slot2; // PCIe present input from PCIe slot
//extern DigitalIn pgood_3v; // Power good singal from DC/DC

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

/*-----------------------------------------------------------*/

// Interupt handler: read data from serial port and write to ring buffer
// The application will poll the ring buffer, fetch & process data later when available
extern void serial_rx_isr_handler( void );

// RX ring buffer size: might need to increace the size for higher baudrate
extern const int SERIAL_RX_BUFF_LEN;

// Ring buffer for serial RX data - used by rx interrupt routines
extern char serial_rx_buffer[];
// Ring buffer pointers
// volatile makes read-modify-write atomic 
extern volatile int serial_rx_in_inx;
extern volatile int serial_rx_out_inx;

/*-----------------------------------------------------------*/

#endif
