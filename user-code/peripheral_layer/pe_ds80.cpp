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

#include "sys_config.h"

/*-----------------------------------------------------------*/

#define DEVID_DS80PCI102    0x77
#define DEVID_DS80PCI402    0x44

#define DS80_REG_DEVID      0x51

/*-----------------------------------------------------------*/

// Default configuration of DS80:       { REG, VALUE }
const char ds80_default_config[][2] = { { 0x06, 0x18 },   // Enables SMBus Slave Mode Register Control
                                        { 0x0F, 0x00 },   // Set CHB_0 EQ to 0x00   (4.9 dB).
                                        { 0x10, 0xAD },   // Set CHB_0 VOD to 101'b (1.2 Vp-p).
                                        { 0x11, 0x00 },   // Set CHB_0 DEM to 000'b (0 dB).
                                        { 0x16, 0x00 },   // Set CHB_1 EQ to 0x00   (4.9 dB).
                                        { 0x17, 0xAD },   // Set CHB_1 VOD to 101'b (1.2 Vp-p).
                                        { 0x18, 0x00 },   // Set CHB_1 DEM to 000'b (0 dB).
                                        { 0x1D, 0x00 },   // Set CHB_2 EQ to 0x00   (4.9 dB).
                                        { 0x1E, 0xAD },   // Set CHB_2 VOD to 101'b (1.2 Vp-p).
                                        { 0x1F, 0x00 },   // Set CHB_2 DEM to 000'b (0 dB).
                                        { 0x24, 0x00 },   // Set CHB_3 EQ to 0x00   (4.9 dB).
                                        { 0x25, 0xAD },   // Set CHB_3 VOD to 101'b (1.2 Vp-p).
                                        { 0x26, 0x00 },   // Set CHB_3 DEM to 000'b (0 dB).
                                        { 0x2C, 0x00 },   // Set CHA_0 EQ to 0x00   (4.9 dB).
                                        { 0x2D, 0xAD },   // Set CHA_0 VOD to 101'b (1.2 Vp-p).
                                        { 0x2E, 0x00 },   // Set CHA_0 DEM to 000'b (0 dB).
                                        { 0x33, 0x00 },   // Set CHA_1 EQ to 0x00   (4.9 dB).
                                        { 0x34, 0xAD },   // Set CHA_1 VOD to 101'b (1.2 Vp-p).
                                        { 0x35, 0x00 },   // Set CHA_1 DEM to 000'b (0 dB).
                                        { 0x3A, 0x00 },   // Set CHA_2 EQ to 0x00   (4.9 dB).
                                        { 0x3B, 0xAD },   // Set CHA_2 VOD to 101'b (1.2 Vp-p).
                                        { 0x3C, 0x00 },   // Set CHA_2 DEM to 000'b (0 dB).
                                        { 0x41, 0x00 },   // Set CHA_3 EQ to 0x00   (4.9 dB).
                                        { 0x42, 0xAD },   // Set CHA_3 VOD to 101'b (1.2 Vp-p).
                                        { 0x43, 0x00 }    // Set CHA_3 DEM to 000'b (0 dB)
};


// CHA_INs are from OCulink, CHB_INs are from CPU (RC) or FPGA/GPU (EP)
// TI suggested config for both RC & EP:{ REG, VALUE }
const char ds80_rc_ep_config[][2] =   { { 0x06, 0x18 },   // Enables SMBus Slave Mode Register Control
                                        { 0x0F, 0x00 },   // Set CHB_0 EQ to 0x00   (4.9 dB).
                                        { 0x10, 0xAD },   // Set CHB_0 VOD to 101'b (1.2 Vp-p).
                                        { 0x11, 0x00 },   // Set CHB_0 DEM to 000'b (0 dB).
                                        { 0x16, 0x00 },   // Set CHB_1 EQ to 0x00   (4.9 dB).
                                        { 0x17, 0xAD },   // Set CHB_1 VOD to 101'b (1.2 Vp-p).
                                        { 0x18, 0x00 },   // Set CHB_1 DEM to 000'b (0 dB).
                                        { 0x1D, 0x00 },   // Set CHB_2 EQ to 0x00   (4.9 dB).
                                        { 0x1E, 0xAD },   // Set CHB_2 VOD to 101'b (1.2 Vp-p).
                                        { 0x1F, 0x00 },   // Set CHB_2 DEM to 000'b (0 dB).
                                        { 0x24, 0x00 },   // Set CHB_3 EQ to 0x00   (4.9 dB).
                                        { 0x25, 0xAD },   // Set CHB_3 VOD to 101'b (1.2 Vp-p).
                                        { 0x26, 0x00 },   // Set CHB_3 DEM to 000'b (0 dB).
                                        { 0x2C, 0x03 },   // Set CHA_0 EQ to 0x03   (11 dB).
                                        { 0x2D, 0xAD },   // Set CHA_0 VOD to 101'b (1.2 Vp-p).
                                        { 0x2E, 0x00 },   // Set CHA_0 DEM to 000'b (0 dB).
                                        { 0x33, 0x03 },   // Set CHA_1 EQ to 0x03   (11 dB).
                                        { 0x34, 0xAD },   // Set CHA_1 VOD to 101'b (1.2 Vp-p).
                                        { 0x35, 0x00 },   // Set CHA_1 DEM to 000'b (0 dB).
                                        { 0x3A, 0x03 },   // Set CHA_2 EQ to 0x03   (11 dB).
                                        { 0x3B, 0xAD },   // Set CHA_2 VOD to 101'b (1.2 Vp-p).
                                        { 0x3C, 0x00 },   // Set CHA_2 DEM to 000'b (0 dB).
                                        { 0x41, 0x03 },   // Set CHA_3 EQ to 0x03   (11 dB).
                                        { 0x42, 0xAD },   // Set CHA_3 VOD to 101'b (1.2 Vp-p).
                                        { 0x43, 0x00 }    // Set CHA_3 DEM to 000'b (0 dB)
};


// De-emphasis setting: 1.5 means -1.5dB
const float ds80_dem_db_table[] = { 0, 1.5, 3.5, 5, 6, 8, 9, 12 }; 

// Equalizer level and dB:               Lv1  --------------------------------------------------------------------------------->  Lv16
const char ds80_eq_level_table[]    = { 0x00, 0x01, 0x02, 0x03, 0x07, 0x15, 0x0B, 0x0F, 0x55, 0x1F, 0x2F, 0x3F, 0xAA, 0x7F, 0xBF, 0xFF };
const float ds80_eq_db_4ghz_table[] = {  4.9,  7.9,  9.9, 11.0, 14.3, 14.6, 17.0, 18.5, 18.0, 22.0, 24.4, 25.8, 27.4, 29.0, 31.4, 32.7 };

/*-----------------------------------------------------------*/

int ds80_init_all( I2C &i2c_bus )
{
    int ds80_i2c_devs[8];
    int dev_num;
    
    memset( ds80_i2c_devs, 0, 8);
    dev_num = ds80_scan_dev( i2c_bus, ds80_i2c_devs );
    
    if( dev_num > 0 )
    {
        for( int i=0; i< dev_num; i++ )
        {            
            ds80_set_eq_dem ( i2c_ms1, ds80_i2c_devs[i], 'd' );
            ds80_dump_eq_dem( i2c_ms1, ds80_i2c_devs[i] );
        }
    }
    else
    {
        serial_debug.printf( "No repeater found on I2C bus 1.\n\r\n\r" );
        return -1;
    } 
    
    return 0;
}

/*-----------------------------------------------------------*/

// config: d - default, r - RC, e - EP.
int ds80_set_eq_dem( I2C &i2c_bus, int dev_id, char config )
{
    int data_len = sizeof( ds80_default_config ) / sizeof( ds80_default_config[0] );
    for( int i = 0; i < data_len; i++ )
    {
        switch( config )
        {
            // Default configuration
            case 'd':
                serial_debug.printf( "DS80 I2C: Set device 0x%02X to default config.\n\r", dev_id );
                if( i2c_bus.write( dev_id << 1, ds80_default_config[i], 2 ) != 0 )
                {
                    serial_debug.printf( "DS80 I2C: write to device 0x%02X failed!\n\r\n\r", dev_id );
                    return -1;
                }
                break;
            
            // Configuration for both PCIe RootComplex and EndPoint
            case 'r':
            case 'e':
                serial_debug.printf( "DS80 I2C: Set device 0x%02X to optimized config.\n\r", dev_id );
                if( i2c_bus.write( dev_id << 1, ds80_rc_ep_config[i], 2 ) != 0 )
                {
                    serial_debug.printf( "DS80 I2C: write to device 0x%02X failed!\n\r\n\r", dev_id );
                    return -1;
                }
                break;
        }
    }
    return 0;
}

/*-----------------------------------------------------------*/

int ds80_scan_dev( I2C &i2c_bus, int *dev_ids )
{
    int dev_num = 0;
    char i2c_buf[1];
    for(int id=0x03; id<0x78; id++)
    {            
        i2c_buf[0] = DS80_REG_DEVID;
        if( i2c_bus.write( id << 1, i2c_buf, 1 ) == 0 && i2c_bus.read( id << 1, i2c_buf, 1 ) == 0 )
        {
            switch( i2c_buf[0] )
            {
                case DEVID_DS80PCI102:
                    serial_debug.printf( "DS80: Find DS80PCI102 at addr 0x%02X!\n\r", id );
                    dev_num++;
                    break;
                
                case DEVID_DS80PCI402:
                    serial_debug.printf( "DS80: Find DS80PCI402 at addr 0x%02X!\n\r", id );
                    *dev_ids = id;
                    dev_ids++;
                    dev_num++;
                    break;
            }
        }
    }
    return dev_num;
}

/*-----------------------------------------------------------*/

int ds80_dump_eq_dem( I2C &i2c_bus, int dev_id )
{
    char config[24];
    char i2c_buf[1];

    // Read all EQ, VOD and DEM configs.
    for( int i = 0; i < 24; i++ )
    {
        i2c_buf[0] = ds80_default_config[i+1][0];
        if( i2c_bus.write( dev_id << 1, i2c_buf, 1 ) != 0 )
        {
            serial_debug.printf( "DS80 I2C: write to device 0x%02X failed!\n\r\n\r", dev_id );
            return -1;
        }        
        if( i2c_bus.read( dev_id << 1, i2c_buf, 1 ) != 0 )
        {
            serial_debug.printf( "DS80 I2C: read from device 0x%02X failed!\n\r\n\r", dev_id );
            return -2;   
        }
        
        config[i] = i2c_buf[0];
        // Debug: dump raw value of all registers
        //serial_debug.printf( "DS80 I2C: read dev<0x%02X> reg<0x%02X> = 0x%02X.\n\r", dev_id, ds80_default_config[i+1][0], config[i] );
    }
  
    // Analyze configs.
    
    // Column head
    serial_debug.printf( "CH      DEM      VOD      EQ@4GHz\n\r" );
    serial_debug.printf( "----------------------------------\n\r" );
    
    for( int i=0; i<8; i++ )
    {
        // Raw head
        if( i < 4 )        
            serial_debug.printf( "B%01d      ", i );
        else
            serial_debug.printf( "A%01d      ", i - 4 );
        
        int reg_offset_eq = i * 3 + 0;
        int reg_offset_vod = i * 3 + 1;              
        int reg_offset_dem = i * 3 + 2;  

        
        switch( config[reg_offset_dem] & 0x07 )
        {
            case 0x00:
                serial_debug.printf( "0dB      " );
                break;
            
            case 0x07:
                serial_debug.printf( "-12dB    " );
                break;
            
            default:
                serial_debug.printf( "-%01.01fdB   ", ds80_dem_db_table[(config[reg_offset_dem] & 0x07 )] );
                
        }
            
        serial_debug.printf( "%01.01fV     ", (float)(config[reg_offset_vod] & 0x07 ) * 0.1f + 0.7f );
        
        int j = 0;
        do
        {
            if( config[reg_offset_eq] == ds80_eq_level_table[j] )
            {
                serial_debug.printf( "%.01fdB\n\r", ds80_eq_db_4ghz_table[j] );
                break;
            }
            j++;
        } while( j < 16 );
        
        if( j == 16 )
            serial_debug.printf( "0x%02X (Unkonw level)\n\r", config[reg_offset_eq] );
    }
    
    serial_debug.printf( "----------------------------------\n\r\n\r" );
    
    return 0;
}

/*-----------------------------------------------------------*/

int ds80_dump_status( I2C &i2c_bus, int dev_id )
{
    
    return 0;
}
