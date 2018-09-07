/*
 *  DS80PCIxxx Peripheral Lib
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

#ifndef _PE_DS80_
#define _PE_DS80_

/*-----------------------------------------------------------*/

//DS80PCIxxx debug functions
extern int ds80_scan_dev( I2C &i2c_bus, int *dev_ids );

extern int ds80_config_set_all( I2C &i2c_bus );
extern int ds80_config_dump_all( I2C &i2c_bus );
extern int ds80_status_dump_all( I2C &i2c_bus );

// config: d - default, r - RC, e - EP.
extern int ds80_set_eq_dem( I2C &i2c_bus, int dev_id, char config );
extern int ds80_dump_eq_dem( I2C &i2c_bus, int dev_id );
extern int ds80_dump_status( I2C &i2c_bus, int dev_id );

#endif
