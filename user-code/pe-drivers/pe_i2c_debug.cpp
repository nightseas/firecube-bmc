/*
 *  I2C Debug Peripheral Lib
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

int i2c_dbg_write( I2C &i2c_bus, int dev_id, const char *data, int length )
{
    return i2c_bus.write( dev_id << 1, data, length );
}

/*-----------------------------------------------------------*/

int i2c_dbg_read( I2C &i2c_bus, int dev_id, char *data, int length )
{
    return i2c_bus.write( dev_id << 1, data, length );
}
