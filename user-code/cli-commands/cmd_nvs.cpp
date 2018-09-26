/*
 *  NVS Commands ( Night CLI Command )
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

/*-----------------------------------------------------------*/

const CLI_Command_t cmd_nvs =
{
    "nvs",
    "\r\ncpu:\r\n Show NVS information and statistics.\r\n",
    command_callback_nvs,
    0
};


/*-----------------------------------------------------------*/

int command_callback_nvs( char *command_output, int output_buf_len, const char *command_string )
{
    if( nvs_fs_failure )
    {
        serial_debug.printf("No NVS available.\n\r");
        return -1;
    }
    
    nvs_usage_dump();
    return 0;
}
