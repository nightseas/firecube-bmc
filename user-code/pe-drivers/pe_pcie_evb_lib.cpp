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

// CPU statistics sample period, the CPU usage during this period will be recorded
const int CPU_USAGE_SAMPLE_TIME = 2000; // unit: ms

// CPU ID & information list
const int cpu_code_list[] = {0xC20, 0xC60, 0xC23, 0xC24, 0xC27, 0xD20, 0xD21};
const char cpu_part_list[][11] = { "Cortex-M0 ", "Cortex-M0+", "Cortex-M3 ", "Cortex-M4 ", "Cortex-M7 ", "Cortex-M23", "Cortex-M33", "UNKNOWN   " };

// CPU statistics val
float sys_cpu_usage = 0;
static uint64_t sys_prev_idle_time = 0;
int id_cpu_stats_queue = 0;

EventQueue *sys_cpu_stats_queue;

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
//DigitalOut led_mb1( PA_0 ), led_mb2( PA_1 ), led_mb3( PA_2 ), led_mb4( PA_3 );
DigitalOut led_mb2( PA_1 ), led_mb3( PA_2 ), led_mb4( PA_3 );
PwmOut led_mb1_pwm( PA_0 );

// USART connect to PC for debug/cli
RawSerial serial_debug( PA_9, PA_10, 115200 ); // CLI interface on debug header

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

// On board NVS block device (SPI Flash)
#if (NVS_HEAP_SIM == 0)
SPIFBlockDevice nvs_bd(PB_15, PB_14, PB_13, PB_12);
#endif

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
    "FPGA-DEV-KIT",
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
    
    // Setup serial RX interrupt
    serial_debug.attach(&serial_rx_isr_handler, RawSerial::RxIrq);
    
    // Update serail number from MCU UUID
    uuid_to_sn( x411_serial_number );
    
    // Dump board info 
    boardlib_info_dump();

    if( nvs_init() )
    {
        serial_debug.printf("NVS init failed! Disable NVS function...\n\r");
        nvs_fs_failure = 1;
    }

    if( !nvs_fs_failure )
    {
        nvs_usage_dump();
    }
        
    serial_debug.printf("Waiting for system good...");
    int pcnt = 20;
    while(pcnt--)
    {
        // IO Hub GPIO<1>: System Power Good.
        if( io_cpld1 == 1 )
        {
            serial_debug.printf("OK!\r\n\r\n");
            break;
        }
        led_mb2 = !led_mb2;
        Thread::wait(100);
    }
    
    // Init all repeaters discovered on both two I2C buses
    if( ds80_config_set_all( i2c_ms1 ) != 0 )//|| ds80_config_dump_all( i2c_ms1 ) != 0 )
    {
        serial_debug.printf("No repeater found on bus 1.\r\n\r\n");
    }
#if( I2C_MS_MAX > 1 )
    if( ds80_config_set_all( i2c_ms2 ) != 0 || ds80_config_dump_all( i2c_ms2 ) != 0 )
    {
        serial_debug.printf("No repeater found on bus 2.\r\n\r\n");
    }
#endif    
        
    sys_cpu_stats_queue = mbed_event_queue();
    id_cpu_stats_queue = sys_cpu_stats_queue->call_every(CPU_USAGE_SAMPLE_TIME, sys_cpu_usage_update);

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

// Update cpu usage to public val
void sys_cpu_usage_update( void )
{
#if ( CPU_STAT_LSE == 1 )
    mbed_stats_cpu_t stats;
    mbed_stats_cpu_get(&stats);
    
    uint64_t diff = (stats.idle_time - sys_prev_idle_time);
    sys_cpu_usage = 100 - ((diff * 100.0f) / (CPU_USAGE_SAMPLE_TIME*1000));    // usec;;    
    
//    serial_debug.printf("\n\r\n\r\n\r[CPU Statistics] uptime: %lu, ", stats.uptime);
//    serial_debug.printf( "idle_time: %lu, ", stats.idle_time);
//    serial_debug.printf( "prev_idle_time: %lu, ", sys_prev_idle_time);
//    serial_debug.printf( "diff: %lu, ", diff);
//    serial_debug.printf( "sleep_time: %lu, ", stats.sleep_time);
//    serial_debug.printf( "deep_sleep_time: %lu\n\r", stats.deep_sleep_time);
//    serial_debug.printf("[CPU Statistics] CPU Usage: %.01f %%\n\r", sys_cpu_usage);//, idle);
    
    sys_prev_idle_time = stats.idle_time;
#endif
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

/*-----------------------------------------------------------*/

// Interupt handler: read data from serial port and write to ring buffer
// The application will poll the ring buffer, fetch & process data later when available
void serial_rx_isr_handler( void )
{
    led_mb4 = !led_mb4;
    // In case there're more then 1 byte in UART HW buffer
    // Stop when buffer is full (and drop the bytes out of processing capacity)
    while( serial_debug.readable() && ( (serial_rx_in_inx + 1) % SERIAL_RX_BUFF_LEN != serial_rx_out_inx ) )
    {
        serial_rx_buffer[serial_rx_in_inx] = serial_debug.getc();        
        serial_rx_in_inx = ( serial_rx_in_inx + 1 ) % SERIAL_RX_BUFF_LEN;
    }
    return;
}
