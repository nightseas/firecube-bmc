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
 
#include "sys_config.h"

/*-----------------------------------------------------------*/

// Heap block device is only used for simulation
#if (NVS_HEAP_SIM == 1)
int erase_unit_size = 512;
HeapBlockDevice nvs_bd(160 * erase_unit_size, 1, 4, erase_unit_size);
#endif

// On board NVS block device (SPI Flash)
#if (NVS_HEAP_SIM == 0)
extern SPIFBlockDevice nvs_bd;
#endif

/*-----------------------------------------------------------*/

// Default NVS filesystem with LittleFS
LittleFileSystem nvs_fs("fs0");

/*-----------------------------------------------------------*/

// Indicate if NVS filesystem is unformatted (e.g. a blank flash) or corrupt
int nvs_fs_corrupt_flag = 0;  

// Indicate if NVS filesystem is good to use
int nvs_fs_failure = 0; 

/*-----------------------------------------------------------*/

// Initialize NVS filesystem with block device and LittleFS
int nvs_init( void )
{
    // Initializes the NVS block device
    serial_debug.printf("[LFS] Initializing NVS block device...");
    int err = nvs_bd.init();
    serial_debug.printf( "%s\n\r", ( err ? "Failed" : "OK" ) );    

     
    
    serial_debug.printf("[LFS] Mounting NVS filesystem... ");
    fflush( stdout );
    err = nvs_fs.mount(&nvs_bd);
    serial_debug.printf( "%s\n\r", ( err ? "Failed" : "OK" ) );
    fflush( stdout );
    
    if( err ) {
        // Reformat if we can't mount the filesystem
        // this should only happen on the first boot
        serial_debug.printf("[LFS] No filesystem found, re-formatting NVS... ");
        nvs_fs_corrupt_flag = 1;
        fflush( stdout );
        err = nvs_fs.reformat(&nvs_bd);
        serial_debug.printf( "%s\n\r", ( err ? "Failed" : "OK" ) );
        fflush( stdout );
        
        if( err )
            return -1;
    }
        
    // Display the root directory
    serial_debug.printf("[LFS] Opening NVS root directory... ");
    fflush( stdout );
    
    DIR *d = opendir("/fs0/");
    
    serial_debug.printf("%s\n\r", (!d ? "Failed" : "OK"));
    fflush( stdout );    

    serial_debug.printf("[LFS] NVS root directory:\n\r");
    while(1) {
        struct dirent *e = readdir(d);
        if (!e) {
            break;
        }
        serial_debug.printf("    %s\n\r", e->d_name);
    }

    serial_debug.printf("[LFS] Closing the NVS root directory... ");
    fflush( stdout );
    
    err = closedir(d);
    
    serial_debug.printf( "%s\n\r", ( err < 0 ? "Failed" : "OK" ) );
    fflush( stdout );  

    serial_debug.printf("[LFS] Unmounting filesystem... ");
    fflush( stdout );
    
    err = nvs_fs.unmount();
    
    serial_debug.printf( "%s\n\r", ( err < 0 ? "Failed" : "OK" ) );
    fflush( stdout );
    
    if( err )
            return -2;
        
    serial_debug.printf("[LFS] NVS file system init done!\n\r");
    
    return 0;
}

/*-----------------------------------------------------------*/

// Dump NVS file system statistics
int nvs_usage_dump( void )
{
    serial_debug.printf( "[LFS] Mounting the filesystem... " );
    fflush( stdout );
    
    int err = nvs_fs.mount( &nvs_bd );
    
    serial_debug.printf( "%s\n\r", ( err ? "Failed" : "OK" ) );
    fflush( stdout );
    
    if(err)
    {
        return -1;
    }
    
    serial_debug.printf( "\n\r\n\r===============================\n\r=  NVS Filesystem Statistics  =\n\r===============================\n\r" );
    
    struct statvfs fs_stat;
    nvs_fs.statvfs("fs0", &fs_stat);
    
    serial_debug.printf(" Block Size: %lu Byte(s)\n\r", fs_stat.f_bsize);
    serial_debug.printf(" Total Size: %lu KB\n\r", fs_stat.f_bsize * fs_stat.f_blocks / 1024);
    serial_debug.printf(" Free Size:  %lu KB\n\r", fs_stat.f_bsize * fs_stat.f_bfree / 1024 );
    
    unsigned int disk_usage = fs_stat.f_bfree * 100 / fs_stat.f_blocks;
    
    serial_debug.printf(" Disk Usage: [");
    for (int ucnt = 0; ucnt < disk_usage / 5; ucnt++)
    {
        serial_debug.printf("|");
    }
    for (int ucnt = 0; ucnt < (20 - disk_usage / 5); ucnt++)
    {
        serial_debug.printf(" ");
    }
    serial_debug.printf("] %lu %%\n\r\n\r", disk_usage);    
    
    serial_debug.printf("[LFS] Unmounting filesystem... ");
    fflush( stdout );
    
    err = nvs_fs.unmount();
    
    serial_debug.printf( "%s\n\r", ( err < 0 ? "Failed" : "OK" ) );
    fflush( stdout );
    
    if(err)
    {
        return -2;
    }
    
    return 0;
}

/*-----------------------------------------------------------*/
