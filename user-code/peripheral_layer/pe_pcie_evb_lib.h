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
extern const int LED_NUM_MAX;

/*-----------------------------------------------------------*/

// LEDs on mother board and daughter board
extern DigitalOut led_mb1, led_mb2, led_mb3, led_mb4;

// USART1 connect to PC for debug/cli
extern Serial serial_debug; // CLI interface on debug header

// I2C master for repeater config
extern I2C i2c_ms1, i2c_ms2;

// I2C slave connected to PCH on mother board
extern I2CSlave i2c_sl1;

// Misc control signals
extern DigitalIn reset_cpu, reset_pch;       // PCIe reset input from CPU and PCH (reserved)
extern DigitalIn pgood_mb, pgood_db;        // Power good input from motherboard and daughter board 
extern DigitalIn pgood_ocl1, pgood_ocl2;     // Power good input from OCuLink
extern DigitalIn present_ocl1, present_ocl2; // OCuLink end point present input
extern DigitalIn wake_ocl1, wake_ocl2, wake_nvme1;     // Wake request singals, reserved
extern DigitalOut reset_ocl1, reset_ocl2, reset_nvme1; // PCIe reset output to OCuLink and NVMe

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
