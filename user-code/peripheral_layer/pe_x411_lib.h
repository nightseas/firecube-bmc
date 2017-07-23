#ifndef _PE_BD_LIB_
#define _PE_BD_LIB_

/*-----------------------------------------------------------*/

//On-board LED number
extern const int LED_NUM_MAX;

/*-----------------------------------------------------------*/

//LEDs on mother board and daughter board
extern DigitalOut led_mb1, led_mb2, led_mb3, led_mb4;

//USART2 connect to PC for debug
extern Serial serial_debug;

//I2C for sensors
extern I2C i2c_ms1;
extern I2CSlave i2c_sl1;

//Analog input channels for ADC
//extern AnalogIn adc_ch10, adc_ch11;

/*-----------------------------------------------------------*/

// Type definition of board information
typedef struct BOARD_INFO_TYPE
{
    const char * const board_name;
    const char * const vendor_name;
    unsigned char * serial_number;
    unsigned char * config;
} Board_Info_t;

typedef struct SENSOR_EVENT_TYPE
{
    unsigned char id;
    unsigned char type;
    double value;
} Sensor_Event_t;

/*-----------------------------------------------------------*/

//Board lib functions
extern int boardlib_init( void );
extern void boardlib_info_dump( void );

#endif
