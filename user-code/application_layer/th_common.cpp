/*
 *  Multi-thread Management & Common Thread
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

/*
Thread thread_ipmb;
static void thread_callback_ipmb( void );
*/

/*-----------------------------------------------------------*/

int threads_init( void )
{
    thread_ioctrl.start( thread_callback_ioctrl );
       
    //thread_ipmb.start( thread_callback_ipmb );
    
    // Put the CLI thread in the last, or it could impact other threads
    thread_cli.start( thread_callback_cli );
    
    serial_debug.printf( "Thread: all threads init done!\n\r" );
    
    return 0;
}

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


