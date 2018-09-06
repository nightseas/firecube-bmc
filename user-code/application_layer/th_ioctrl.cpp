/*
 *  I/O Control Thread
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

Thread thread_ioctrl;

/*-----------------------------------------------------------*/

void thread_callback_ioctrl( void )
{
    serial_debug.printf( "Thread: <IO ctrl thread> started.\n\r" );
    
    while( 1 )
    {      
        // Update reset status to debug LEDs
//        // LED ON: pgood is asserted
//        led_mb2 = !pgood_3v;
//        
//        // LED ON: present is asserted
//        led_mb3 = present_slot1;
//        led_mb4 = present_slot2;
//        
//        // LED ON: reset is de-asserted
//        led_mb6 = !reset_ocl1;
//        led_mb5 = !reset_ocl2;
//        led_mb8 = !reset_ocl3;
//        led_mb7 = !reset_ocl4;
      
        // Poll input status every 10ms
        Thread::wait( 10 );
    }
}

