/*
 *  Main Entry of Firecube-BMC
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

// main() runs in its own thread in the OS
// (note the calls to Thread::wait below for delays)
int main()
{    
    boardlib_init();
    threads_init();  
    cli_commands_init();
    
    //serial_debug.printf("Thread: <Main thread> entered idle loop!\n\r");
    while(1)
    {
        //led_mb1 = !led_mb1;
        //Thread::wait(500);
			
				// LED Breathing indicating heart beats.
				for( int i = 0; i <= 100; i++)
				{
						led_mb1_pwm.write( i / 100.0 );
						Thread::wait(i/10);
				}
				Thread::wait(1000);
				for( int i = 100; i >= 0; i--)
				{
						led_mb1_pwm.write( i / 100.0 );
						Thread::wait(i/10);
				}
				
    }
}

