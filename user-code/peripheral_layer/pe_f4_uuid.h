/*
 *  STM32F4 UUID Peripheral Lib
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

#ifndef _PE_UUID_
#define _PE_UUID_

/*-----------------------------------------------------------*/

//On-board LED number
extern unsigned long mcu_uuid[];

/*-----------------------------------------------------------*/

// Read the 96-bit UUID from register
extern void uuid_update( void );

// Convert UUID to SHA256, use the mid 16-byte as an unique serial number
// This function calls uuid_update to read UUID
extern void uuid_to_sn( unsigned char *sn );

#endif
