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

// Definition check for mbed-os CPU and system statistics features.

// CPU statistics can't be used on FPGA-DEV-KIT platform due to no LSE available
//#if !defined(MBED_CPU_STATS_ENABLED) || !defined(DEVICE_LPTICKER) || !defined(DEVICE_SLEEP)
//#error [NOT_SUPPORTED] CPU statistics not supported
//#endif

#if !defined(MBED_SYS_STATS_ENABLED)
#error [NOT_SUPPORTED] mbed system statistics not supported
#endif

// TODO: This definition should be moved to I2C function codes!!!!
// 7-bit I2C address of I2C swtich
#define I2C_SW_ADDR             0x70

/*-----------------------------------------------------------*/

// Board serial number buffer size
const int BD_SN_LEN = 16;

// Board configuration buffer size
const int BD_CONFIG_LEN = 16;

// CPU ID & information list
const int cpu_code_list[] = {0xC20, 0xC60, 0xC23, 0xC24, 0xC27, 0xD20, 0xD21};
const char cpu_part_list[][11] = { "Cortex-M0 ", "Cortex-M0+", "Cortex-M3 ", "Cortex-M4 ", "Cortex-M7 ", "Cortex-M23", "Cortex-M33", "UNKNOWN   " };

// RX ring buffer size: might need to increace the size for higher baudrate
const int SERIAL_RX_BUFF_LEN = 127;

// Ring buffer for serial RX data - used by rx interrupt routines
char serial_rx_buffer[SERIAL_RX_BUFF_LEN+1];
// Ring buffer pointers
// volatile makes read-modify-write atomic 
volatile int serial_rx_in_inx=0;
volatile int serial_rx_out_inx=0;

/*-----------------------------------------------------------*/

// LEDs on mother board and daughter board
DigitalOut ssd_pwr_en0( PB_13, 0 );
DigitalOut ssd_pwr_en1( PB_14, 0 );

// USART connect to PC for debug/cli
RawSerial serial_debug( PA_2, PA_3, 115200 ); // CLI interface on debug header

// I2C master for repeater config
I2C i2c_ms1( PA_10, PA_9 );

// Misc control signals
DigitalOut reset1( PA_4, 0 ), reset2( PA_5, 0 );

/*-----------------------------------------------------------*/

// Board serail number buffer
unsigned char x411_serial_number[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// 16-Byte board configuration buffer
unsigned char x411_config[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
// Board information struct
FRU_Info_t x411_board_info =
{
    "X411-DEV-KIT",
    "N/A",
    "V01",
    "Firecube-BMC",
    "Xiaohai Li",
    "V01",
    x411_serial_number,
    x411_config
};

/*-----------------------------------------------------------*/

// Initialize board relative functions
// Return: 0 - succeeded, non-0 - failed
int boardlib_init( void )
{    
    //LED initial states: all on


    // Assert resets
    reset1 = 0;
    reset2 = 0;
    ssd_pwr_en0 = 0;
    ssd_pwr_en1 = 0;
        
    // Setup serial RX interrupt
    serial_debug.attach(&serial_rx_isr_handler, RawSerial::RxIrq);

    // Dump CPU & OS info
    sys_cpu_info_dump();
    
    // Update serail number from MCU UUID
    uuid_to_sn( x411_serial_number );
    
    // Dump board info 
    boardlib_info_dump();

    logo_ascii_show();

    // Release resets
    reset1 = 1;
    reset2 = 1;
     
    // TBD: No failure handling for now!
    return 0;
}

/*-----------------------------------------------------------*/

// Dump board information to debug interface
void boardlib_info_dump( void )
{
    int i;
    
    serial_debug.printf( "\n\r\n\r===============================\n\r=      FRU Information      =\n\r===============================\n\r" );    
    serial_debug.printf( " Board    Name: %s\n\r", x411_board_info.board_name );
    serial_debug.printf( " Vendor   Name: %s\n\r", x411_board_info.vendor_name );
    serial_debug.printf( " Board    Ver : %s\n\r\n\r", x411_board_info.board_version );
    
    serial_debug.printf( " Firmware Name: %s\n\r", x411_board_info.firmware_name );
    serial_debug.printf( " Firmware Auth: %s\n\r", x411_board_info.firmware_author );
    serial_debug.printf( " Firmware Ver : %s\n\r\n\r", x411_board_info.firmware_version );
    
    serial_debug.printf( " Serial   Num : " );
    for( i = 0; i < BD_SN_LEN; i++ )
        serial_debug.printf( "%02X", x411_board_info.serial_number[i] );    
    serial_debug.printf( "\n\r\n\r" );
    
    serial_debug.printf( " NV Config  : " );
    for( i = 0; i < BD_CONFIG_LEN; i++ )
        serial_debug.printf( "0x%02X ", x411_board_info.config[i] );    
    serial_debug.printf( "\n\r\n\r" );
}

/*-----------------------------------------------------------*/

// Dump CPU & OS information to debug interface
void sys_cpu_info_dump( void )
{
    int id;
    
    mbed_stats_sys_t stats;
    mbed_stats_sys_get(&stats);
    
    serial_debug.printf( "\n\r\n\r===============================\n\r=     CPU & OS Information    =\n\r===============================\n\r" );    
    
    /* CPUID Register information
    [31:24]Implementer      0x41 = ARM
    [23:20]Variant          Major revision 0x0  =  Revision 0
    [19:16]Architecture     0xC  = Baseline Architecture
                            0xF  = Constant (Mainline Architecture?)
    [15:4]PartNO            0xC20 =  Cortex-M0
                            0xC60 = Cortex-M0+
                            0xC23 = Cortex-M3
                            0xC24 = Cortex-M4
                            0xC27 = Cortex-M7
                            0xD20 = Cortex-M23
                            0xD21 = Cortex-M33
    [3:0]Revision           Minor revision: 0x1 = Patch 1.
    */
    serial_debug.printf( " CPU ID: 0x%X\n\r", stats.cpu_id );
    serial_debug.printf( " CPU Vendor: %s\n\r", ( ( stats.cpu_id >> 24 ) == 0x41 ) ? "ARM" : "UNKNOWN" );
        
    for (id = 0; id < sizeof( cpu_code_list ) / sizeof( int ); id++)
    {        
        if ( ( ( stats.cpu_id >> 4 ) & 0xFFF ) == cpu_code_list[id] )
            break;
    }
    serial_debug.printf( " CPU PartNum: %s\n\r", cpu_part_list[id]);
    
    serial_debug.printf( " CPU Major Ver: 0x%X\n\r", ( ( stats.cpu_id >> 20 ) & 0xF ) );
    serial_debug.printf( " CPU Minor Ver: 0x%X\n\r", ( stats.cpu_id & 0xF ) );

    serial_debug.printf( " OS Ver: 0x%X\n\r", stats.os_version );
    serial_debug.printf( " Compiler ID: 0x%X\n\r", stats.compiler_id );

    /* Compiler versions:
       ARM: PVVbbbb (P = Major; VV = Minor; bbbb = build number)
       GCC: VVRRPP  (VV = Version; RR = Revision; PP = Patch)
       IAR: VRRRPPP (V = Version; RRR = Revision; PPP = Patch)
    */
    serial_debug.printf( " Compiler Ver: 0x%X\n\r", stats.compiler_version );
}

void logo_ascii_show( void )
{
    serial_debug.printf("     _______  __  .______       _______   ______  __    __  .______    _______  \n\r");
    serial_debug.printf("    |   ____||  | |   _  \\     |   ____| /      ||  |  |  | |   _  \\  |   ____| \n\r");
    serial_debug.printf("    |  |__   |  | |  |_)  |    |  |__   |  ,----'|  |  |  | |  |_)  | |  |__    \n\r");
    serial_debug.printf("    |   __|  |  | |      /     |   __|  |  |     |  |  |  | |   _  <  |   __|   \n\r");
    serial_debug.printf("    |  |     |  | |  |\\  \\----.|  |____ |  `----.|  `--'  | |  |_)  | |  |____  \n\r");
    serial_debug.printf("    |__|     |__| | _| `._____||_______| \\______| \\______/  |______/  |_______| \n\r");
    serial_debug.printf("                                                                                \n\r");
    serial_debug.printf("                         .______   .___  ___.   ______                          \n\r");
    serial_debug.printf("                         |   _  \\  |   \\/   |  /      |                         \n\r");
    serial_debug.printf("                         |  |_)  | |  \\  /  | |  ,----'                         \n\r");
    serial_debug.printf("                         |   _  <  |  |\\/|  | |  |                              \n\r");
    serial_debug.printf("                         |  |_)  | |  |  |  | |  `----.                         \n\r");
    serial_debug.printf("                         |______/  |__|  |__|  \\______|                         \n\r");
    fflush(stdout);
}

/*-----------------------------------------------------------*/

// Interupt handler: read data from serial port and write to ring buffer
// The application will poll the ring buffer, fetch & process data later when available
void serial_rx_isr_handler( void )
{
    // In case there're more then 1 byte in UART HW buffer
    // Stop when buffer is full (and drop the bytes out of processing capacity)
    while( serial_debug.readable() && ( (serial_rx_in_inx + 1) % SERIAL_RX_BUFF_LEN != serial_rx_out_inx ) )
    {
        serial_rx_buffer[serial_rx_in_inx] = serial_debug.getc();        
        serial_rx_in_inx = ( serial_rx_in_inx + 1 ) % SERIAL_RX_BUFF_LEN;
    }
    return;
}
