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
int main()
{    
    boardlib_init();
    cli_commands_init();
    threads_init();    
    
    //serial_debug.printf("Thread: <Main thread> entered idle loop!\n\r");
    while(1)
    {            
        // LED Breathing indicating heart beats.
        // ssd_pwr_en0 = !ssd_pwr_en0;
        ThisThread::sleep_for(5000);
    }
}

