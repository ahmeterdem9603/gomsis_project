/*
 * esp8266.c
 *
 *  Created on: May 8, 2021
 *      Author: aeahm
 */
#include "esp8266.h"

void create_arry_data (uint32_t lidar_value){

	memset(buff_Snd, 0x00, sizeof(buff_Snd));
	memset(buff_Rcv, 0x00, sizeof(buff_Rcv));

	u8 buff_LidarData[4] = {0};
    u8 i  = 0;

		while ( lidar_value > 0 )
		{
			buff_LidarData[i]   = lidar_value % 10;
			lidar_value = (lidar_value - buff_LidarData[i])/10;
			i++;
		}

		memcpy(&buff_Snd[DATA_LOCATION], &values, 4);
		//uartlite_Send(&mUartLiteWifi, buff_LidarData, 4);

	return 0;
}

void send_data_to_cloud(u32 lidarData){

	memset(buff_Snd, 0x00, sizeof(buff_Snd));
	memset(buff_Rcv, 0x00, sizeof(buff_Rcv));

	uartlite_Send(&mUartLiteWifi, cloud_start_cmmnd, sizeof(cloud_start_cmmnd));
	sleep(1);
	wait_OK(10);
	uartlite_Send(&mUartLiteWifi, data_length_cmmnd, sizeof(data_length_cmmnd));
	sleep(1);
	wait_OK(10);
	uartlite_Send(&mUartLiteWifi, write_data_cloud_cmmnd, sizeof(write_data_cloud_cmmnd));
	sleep(1);
	wait_OK(10);

	return 0;
}


void connect_wifi(){

	memset(buff_Snd, 0x00, sizeof(buff_Snd));
	memset(buff_Rcv, 0x00, sizeof(buff_Rcv));

	uartlite_Send(&mUartLiteWifi, connection_ctrl_cmmnd, sizeof(connection_ctrl_cmmnd));
	sleep(1);
	wait_OK(10);
	uartlite_Send(&mUartLiteWifi, wifi_mode_cmmnd, sizeof(wifi_mode_cmmnd));
	sleep(1);
	wait_OK(10);
	uartlite_Send(&mUartLiteWifi, connection_cmmnd, sizeof(connection_cmmnd));
	sleep(1);
	wait_OK(10);

	return 0;
}


void wait_OK(u16 retry){
	while(retry > 0){
		uartlite_Recv(&mUartLiteWifi,buff_Rcv,sizeof(buff_Rcv));
		for(u16 i=0; i<sizeof(buff_Rcv); i++){
			if ((buff_Rcv[i] == 'O' && buff_Rcv[i+1] == 'K') || buff_Rcv[i] == '>'){
				printf("Command received succesfull !! \n\r");
				break;
			}
		}
		retry--;
	}
	printf("Command received failed !! \n\r");
	return 0;
}
