/*
 *  NV Storage File System Peripheral Lib
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

#ifndef _PE_LFS_
#define _PE_LFS_

/*-----------------------------------------------------------*/

// Default NVS filesystem with LittleFS
extern LittleFileSystem nvs_fs;

// Indicate if NVS filesystem is unformatted (e.g. a blank flash) or corrupt
extern int nvs_fs_corrupt_flag;  

// Indicate if NVS filesystem is good to use
extern int nvs_fs_failure; 

/*-----------------------------------------------------------*/

// Initialize NVS file system with block device and LittleFS
extern int nvs_init( void );

// Dump NVS file system statistics
extern int nvs_usage_dump( void );

#endif
