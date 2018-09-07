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
 
#include "sys_config.h"

/*-----------------------------------------------------------*/

// STM32F4 UUID register
#define UUID_STM32F411 ( ( unsigned long * ) 0x1FFF7A10 )

#define USE_TLS_SHA256

/*-----------------------------------------------------------*/

// Stored UUID for this MCU
unsigned long mcu_uuid[] = { 0, 0, 0 };

/*-----------------------------------------------------------*/

// Read the 96-bit UUID from register
void uuid_update( void )
{    
    mcu_uuid[ 0 ] = UUID_STM32F411[ 0 ];
    mcu_uuid[ 1 ] = UUID_STM32F411[ 1 ];
    mcu_uuid[ 2 ] = UUID_STM32F411[ 2 ];
    
    // serial_debug.printf( "MCU UUID: 0x%04lX %04lX %04lX.\n\r", mcu_uuid[ 2 ], mcu_uuid[ 1 ], mcu_uuid[ 0 ] );
}

/*-----------------------------------------------------------*/

// Convert UUID to SHA256, use the mid 16-byte as an unique serial number
// This function calls uuid_update to read UUID
void uuid_to_sn( unsigned char *sn )
{    
    unsigned char in_buff[ 12 ], out_buff[ MBEDTLS_MD_MAX_SIZE ];
    uuid_update();
    memcpy( in_buff, mcu_uuid, 12);
    
#ifdef USE_TLS_SHA256
    // Call generic MD fucntion and with SHA256 indicator
    //const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_MD5);
    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    
    if (md_info == NULL)
    {
        serial_debug.printf( "SHA256 hash in mbed TLS is not available.\n\r" );
        return;
    }

    int md5_ret = mbedtls_md( md_info, in_buff, 12, out_buff );
    if( md5_ret != 0 )
    {
        serial_debug.printf( "SHA256 hashing failed (%d).\n\r", md5_ret );
        return ;
    }   

    // Use mid 16-byte as serial number
    memcpy( sn, out_buff + 8, 16 );
#else
    // Warining: Direct copy of UUID to serail nunmber may cause security issue
    memcpy( sn, in_buff, 12 );
#endif
}
