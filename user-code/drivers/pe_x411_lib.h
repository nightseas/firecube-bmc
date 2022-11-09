/*
 *  PCIe Rpeater EVB Fru Lib
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
#define LED_NUM_MAX         1
// I2C master number
#define I2C_MS_MAX          0
// PCIe reset delay after power up
#define PE_RST_DELAY_MS     100

/*-----------------------------------------------------------*/

// LEDs on mother board and daughter board
extern PwmOut led_mb1_pwm;

// SSD power control
extern DigitalOut ssd_pwr_en0, ssd_pwr_en1, ssd_pwr_en2, ssd_pwr_en3;

// USART connect to PC for debug/cli
extern RawSerial serial_debug; // CLI interface on debug header

// SSD reset control
extern DigitalIn sff_perst0, sff_perst1, sff_perst2, sff_perst3;
extern DigitalOut ssd_rst0, ssd_rst1, ssd_rst2, ssd_rst3;

// Power monitoring ADC
extern AnalogIn ssd_vmon0, ssd_vmon1;
extern AnalogIn mcu_vtemp, mcu_vref, mcu_vbat;

/*-----------------------------------------------------------*/

// Type definition of board information
typedef struct FRU_INFO_TYPE
{
    const char * const board_name;
    const char * const vendor_name;
    const char * const board_version;
    const char * const firmware_name;
    const char * const firmware_author;
    const char * const firmware_version;
    unsigned char * serial_number;
    unsigned char * config;
} FRU_Info_t;

typedef struct SENSOR_EVENT_TYPE
{
    unsigned char id;
    unsigned char type;
    double value;
} Sensor_Event_t;

/*-----------------------------------------------------------*/

//Fru lib functions
extern int boardlib_init( void );
extern void boardlib_info_dump( void );

// Dump CPU & OS information to debug interface
extern void sys_cpu_info_dump( void );

extern void logo_ascii_show( void );

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
