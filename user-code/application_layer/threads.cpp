/*
 *  Multi-thread Management
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

Thread thread_com;
Thread thread_rst_ctrl;

/*-----------------------------------------------------------*/

static void thread_callback_com( void );
static void thread_callback_rst_ctrl( void );

/*
Thread thread_ipmb;
static void thread_callback_ipmb( void );
*/

/*-----------------------------------------------------------*/

int threads_init( void )
{
    thread_rst_ctrl.start( thread_callback_rst_ctrl );
       
    //thread_ipmb.start( thread_callback_ipmb );
    //thread_led.start( thread_callback_led );
    
    // Put the communication thread in the last, or it could impact other threads
    thread_com.start( thread_callback_com );
    
    serial_debug.printf( "Thread: all threads init done!\n\r" );
    
    return 0;
}

/*-----------------------------------------------------------*/

static void thread_callback_rst_ctrl( void )
{
    serial_debug.printf( "Thread: <Reset ctrl thread> started.\n\r" );
    
    while( 1 )
    {
        // Pass through CPU PCIe reset to OCuLink if OCL is present and EP board power is steady
        if( present_ocl1 == 0 )
        {
            if( pgood_ocl1 == 1 )
            {
                reset_ocl1 = reset_cpu;                
            }
        }
        else
        {
            reset_ocl1 = 0;
        }
        // Same for OCL2
        if( present_ocl2 == 0 )
        {
            if( pgood_ocl2 == 1 )
            {
                reset_ocl2 = reset_cpu;
            }
        }
        else
        {
            reset_ocl2 = 0;
        }
        
        // Pass through CPU PCIe reset to NVMe if daughter board power is steady
        if( pgood_db == 1 )
            reset_nvme1 = reset_cpu;
        else
            reset_nvme1 = 0;
        
        // Update reset status to debug LEDs
        // LED ON: reset is de-asserted
        led_mb2 = !reset_ocl1;
        led_mb3 = !reset_ocl2;
        led_mb4 = !reset_nvme1;
        
        // Poll input status every 10ms
        Thread::wait( 10 );
    }
}

/*-----------------------------------------------------------*/

static void thread_callback_com( void )
{    
    serial_debug.printf( "Thread: <Communication thread> started.\n\r" );    
        
    nightcli_console_loop( serial_debug );
}

/*-----------------------------------------------------------*/

/*
#define IPMB_DEV_ADDR       0x50

static void thread_callback_ipmb( void )
{
    char buf[10];
    char msg[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA};
    
    serial_debug.printf( "Thread: <IPMB thread> started.\n\r" );  

    i2c_sl1.address( IPMB_DEV_ADDR << 1 );
    while ( 1 )
    {
        int i = i2c_sl1.receive();
        switch ( i )
        {
            case I2CSlave::ReadAddressed:
            i2c_sl1.write( msg, 10 );
            break;
            case I2CSlave::WriteGeneral:
            i2c_sl1.read( buf, 10 );
            serial_debug.printf( "IPMB WG: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n\r", 
                                 buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9] );
            break;
            case I2CSlave::WriteAddressed:
            i2c_sl1.read( buf, 10 );
            serial_debug.printf( "IPMB WA: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n\r", 
                                 buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9] );
            break;
        }
        memset( buf, 0x00, 10 ); // Clear buffer
    }
}
*/


