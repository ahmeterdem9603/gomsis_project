/*
 * tf02_pro.h
 *
 *  Created on: Jan 9, 2021
 *      Author: ahmet.erdem
 */

#ifndef TF02_PRO_H_
#define TF02_PRO_H_

//********** TF02-PRO LIDAR SENSOR UART CONTROLLER HEADER FILE **********//

//** Include Libraries **//
//#include "stdint.h"
//#include "stdio.h"
//#include "string.h"
////#include "xuartlite.h"
////#include "uart.h"
//#include "uartlite.h"
//#include "stdlib.h"
//#include "sleep.h"
#include "platform.h"
//***********************//


//** Define Parameters **//
//#define FRAME_HEADER    0x5959          // Frame header for standart data output format
#define MAX_PACKET_SIZE     0x9

//***********************//

typedef enum
{
	bd_9600		= 9600,
	bd_38400	= 38400,
    bd_115200   = 115200,
    bd_256000	= 256000,
    bd_460800	= 460800,
    bd_921600	= 921600,
}OptBaudRate_t;

typedef enum
{
	uart,
	iic,
}OptCommInterface_t;

typedef enum
{
	cm = 1,
	m  = 2,
    mm = 6,
}OptOutputFormat_t;

typedef enum
{
	disable_output,
	enable_output
}OptOutputMode_t;

typedef enum
{
	disable,
	enable,
}ConfigMode_t;

//** Function Prototypes **//

//void uart_init();         // This function change for different platforms.
int tf02_pro_init(uint16_t frameRate, OptBaudRate_t baudRate, OptOutputFormat_t outputFormat);
int tf02_pro_system_reset();
int tf02_pro_config_mode(ConfigMode_t mode);
int tf02_pro_get_firmare_version();
int tf02_pro_get_lidar_data();
int tf02_pro_set_frame_rate(uint16_t frameRate);
int tf02_pro_set_output_format(OptOutputFormat_t outputFormat);
int tf02_pro_set_baudrate(OptBaudRate_t baudRate);
int tf02_pro_set_data_output_mode(OptOutputMode_t outputMode);
int tf02_pro_set_restore_fact_settings();
int tf02_pro_set_comm_interface(OptCommInterface_t);  // Recommended Uart interface to run this library correctly.
uint8_t tf02_pro_set_checksum(uint8_t* ptr, uint8_t size);
//*************************//

#endif
