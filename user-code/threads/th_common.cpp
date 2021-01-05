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
    thread_cli.start( thread_callback_cli );
    
    serial_debug.printf( "Thread: all threads init done!\n\r" );
    fflush(stdout);
    
    return 0;
}

