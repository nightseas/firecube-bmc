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

