/*
 * esp8266.h
 *
 *  Created on: May 8, 2021
 *      Author: aeahm
 */

#ifndef SRC_ESP8266_H_
#define SRC_ESP8266_H_

/******** INCLUDES ********/
//#include "uartlite.h"
//#include "stdint.h"
//#include "stdio.h"
//#include "string.h"
#include "platform.h"

/****** DEFINITONS ********/
#define wifi_name 		"Redmi"
#define wifi_key  		"12345678"
#define IP        		"184.106.153.149"          //thingspeak.com IP adresi

#define AT              "AT\r\n"
#define AT_CWMODE       "AT+CWMODE=1\r\n"
#define AT_CWJAP        "AT+CWJAP"
#define AT_CIPSEND      "AT+CIPSEND=50"
#define AT_CIPCLOSE     "AT+CIPCLOSE"
#define DATA_LOCATION	44

/**************************/

unsigned char connection_ctrl_cmmnd[] 	= AT;
unsigned char wifi_mode_cmmnd[]       	= AT_CWMODE;
unsigned char connection_cmmnd[]    	= "AT_CWJAP=\""wifi_name"\",\""wifi_key"\"\r\n";
unsigned char cloud_start_cmmnd[]       = "AT+CIPSTART=\"TCP\",\""IP"\",80";
unsigned char write_data_cloud_cmmnd[]  = "GET /update?api_key=09L14D6XZZ8ZG2J2&field1=";  //???
unsigned char data_length_cmmnd[]       = AT_CIPSEND;  //veri uzunlugu eklenecek
unsigned char cloud_close_cmmnd[]       = AT_CIPCLOSE;

/***************************/

unsigned int adc_value = 0;
unsigned char values[4];

static uint8_t buff_Snd[100];
static uint8_t buff_Rcv[100];

void create_arry_data (uint32_t lidar_value);
void send_data_to_cloud(u32 lidarData);
void connect_wifi();
void wait_OK(u16 retry);






#endif /* SRC_ESP8266_H_ */
