#include "sys_config.h"

/*-----------------------------------------------------------*/

// On-board LED number
const int LED_NUM_MAX = 2;

// Board serial number buffer size
const int BD_SN_LEN = 16;

// Board configuration buffer size
const int BD_CONFIG_LEN = 16;

/*-----------------------------------------------------------*/

// LEDs on mother board and daughter board
DigitalOut led_mb1(PC_14), led_mb2(PC_15);

// USART2 connect to PC for debug/cli
Serial serial_debug(SERIAL_TX, SERIAL_RX); // for debug

// I2C for sensors
I2C i2c_ms1(PB_7, PB_6);

// Analog input channels for ADC
AnalogIn adc_vcpu(PA_4), adc_vgpu(PA_5), adc_vddr(PA_6), adc_vsys(PA_7), adc_vio(PB_0);

/*-----------------------------------------------------------*/

// Board serail number buffer
unsigned char x411_serial_number[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 16-Byte board configuration buffer
unsigned char x411_config[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// Board information struct
Board_Info_t x411_board_info =
{
    "Fennec-BMC",
    "N/A (haixiaoli@gmail.com)",
    x411_serial_number,
    x411_config
};

/*-----------------------------------------------------------*/

// Initialize board relative functions
// Return: 0 - succeeded, non-0 - failed
int boardlib_init( void )
{    
    //LED initial states: all on
    led_mb1 = 0;
    led_mb2 = 0;
    
    // Set PC debug/cli serial port to 115200,8,n,1
    serial_debug.baud(115200);
    
    // Update serail number from MCU UUID
    uuid_to_sn( x411_serial_number );
    
    // Dump board info after board lib init 
    boardlib_info_dump();
    
    //LED states after board lib init: all off    
    led_mb1 = 1;
    led_mb2 = 1;

    // TBD: No failure handling for now!
    return 0;
}

/*-----------------------------------------------------------*/

// Dump board information to debug interface
void boardlib_info_dump( void )
{
    int i;
    
    serial_debug.printf("\n\r\n\r\n\r===============================\n\r=      Board Information      =\n\r===============================\n\r");
    serial_debug.printf(" Board  Name: %s\n\r", x411_board_info.board_name);
    serial_debug.printf(" Vendor Name: %s\n\r", x411_board_info.vendor_name);
    serial_debug.printf(" Serial Num : ");
    for( i = 0; i < BD_SN_LEN; i++ )
        serial_debug.printf("%02X", x411_board_info.serial_number[i]);    
    serial_debug.printf("\n\r\n\r");
    
    serial_debug.printf(" NV Config  : ");
    for( i = 0; i < BD_CONFIG_LEN; i++ )
        serial_debug.printf("0x%02X ", x411_board_info.config[i]);    
    serial_debug.printf("\n\r\n\r");
}

