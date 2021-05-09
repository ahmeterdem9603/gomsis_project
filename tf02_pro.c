//** Include Libraries **//
#include "tf02_pro.h"
//#include "xuartlite.h"

//***********************//

//const uint8_t command_Reg[3][MAX_PACKET_SIZE-1] = {
//	{0xAA, 0x55, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xA0},			// get firmware version
//	{0xAA, 0x55, 0xF0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF},			// system reset
//	{0x5A, 0x04, 0x10, 0x6E, 0x00, 0x00, 0x00, 0x00}			// set restore factory settings
//};

static uint8_t buff_Snd[MAX_PACKET_SIZE];
static uint8_t buff_Rcv[MAX_PACKET_SIZE];


int tf02_pro_system_reset(){
	// clear the uart buffers
	memset(buff_Snd, 0x00, sizeof(buff_Snd));
	memset(buff_Rcv, 0x00, sizeof(buff_Rcv));

	buff_Snd[0] = 0xAA;
	buff_Snd[1] = 0x55;
	buff_Snd[2] = 0xF0;
	buff_Snd[3] = 0x00;
	buff_Snd[4] = 0xFF;
	buff_Snd[5] = 0xFF;
	buff_Snd[6] = 0xFF;
	buff_Snd[7] = 0xFF;

	// send uart buffer
	uartlite_Send(&mUartLiteLidar, buff_Snd, 8);

	usleep(1000*100);
	//while(!XUartLite_IsSending(&mUartLiteLidar));

	// read recieve buffer
	uartlite_Recv(&mUartLiteLidar, buff_Rcv, 8);

	int diff = 0;

	for (int i = 0; i < 8; i++) {
		diff = abs(buff_Snd[i] - buff_Rcv[i]) + diff;
	}

	if (diff == 0) {
		xil_printf("System reset successful.\n\r");
	}
	else{
		xil_printf("System reset failed.\n\r");
	}
	return XST_SUCCESS;
}

int tf02_pro_config_mode(ConfigMode_t mode){
	int diff = 0;
	// clear the uart buffers
	memset(buff_Snd, 0x00, sizeof(buff_Snd));
	memset(buff_Rcv, 0x00, sizeof(buff_Rcv));

	buff_Snd[0] = 0xAA;
	buff_Snd[1] = 0x55;
	buff_Snd[2] = 0xF0;
	buff_Snd[3] = 0x00;
	buff_Snd[4] = mode;
	buff_Snd[5] = 0x00;
	buff_Snd[6] = 0x00;
	buff_Snd[7] = 0x02;

	// send uart buffer
	uartlite_Send(&mUartLiteLidar, buff_Snd, sizeof(buff_Snd)-1);

	// wait to send uart buffer
	usleep(1000*100);
	//while(!XUartLite_IsSending(&mUartLiteLidar));

	// read recieve buffer
	uartlite_Recv(&mUartLiteLidar, buff_Rcv, sizeof(buff_Rcv)-1);

	for (int i = 0; i < sizeof(buff_Snd)-1; i++) {
		diff = abs(buff_Snd[i] - buff_Rcv[i]) + diff;
	}

	if (diff == 0) {
		xil_printf("Configuration mode changed successful.\n\r");
	}
	else{
		xil_printf("Configuration mode changed failed.\n\r");
	}
	return XST_SUCCESS;
}

int tf02_pro_get_firmare_version() {
	// clear the uart buffers
	memset(buff_Snd, 0x00, sizeof(buff_Snd));
	memset(buff_Rcv, 0x00, sizeof(buff_Rcv));

	buff_Snd[0] = 0xAA;
	buff_Snd[1] = 0x55;
	buff_Snd[2] = 0xF0;
	buff_Snd[3] = 0x00;
	buff_Snd[4] = 0x00;
	buff_Snd[5] = 0x00;
	buff_Snd[6] = 0x00;
	buff_Snd[7] = 0xA0;

	// send uart buffer
	uartlite_Send(&mUartLiteLidar, buff_Snd, sizeof(buff_Snd)-1);

	// wait to send uart buffer
	//while(!XUartLite_IsSending(&mUartLiteLidar));
	usleep(1000*200);

	// read recieve buffer
	uartlite_Recv(&mUartLiteLidar, buff_Rcv, sizeof(buff_Rcv)-1);

	int diff = 0;

	for (int i = 0; i < sizeof(buff_Snd)-4; i++) {
		diff = abs(buff_Snd[i] - buff_Rcv[i]) + diff;
	}

	if (diff == 0) {
		xil_printf("Firmware version = %d . %d . %d\n\r", buff_Rcv[5], buff_Rcv[6], buff_Rcv[7]);
	}
	else {
		xil_printf("Firmware version failed.\n\r");
	}
	return XST_SUCCESS;
}

int tf02_pro_set_restore_fact_settings() {
	// clear the uart buffers
	memset(buff_Snd, 0x00, sizeof(buff_Snd));
	memset(buff_Rcv, 0x00, sizeof(buff_Rcv));

	buff_Snd[0] = 0x5A;
	buff_Snd[1] = 0x04;
	buff_Snd[2] = 0x10;
	buff_Snd[3] = 0x6E;

	// send uart buffer
	uartlite_Send(&mUartLiteLidar, buff_Snd, sizeof(buff_Snd)-5);

	// wait to send uart buffer
	//while(!XUartLite_IsSending(&mUartLiteLidar));
	usleep(1000*200);

	// read recieve buffer
	uartlite_Recv(&mUartLiteLidar, buff_Rcv, sizeof(buff_Snd)-5);

	uint8_t crc = tf02_pro_set_checksum(&buff_Rcv[0], 4);
	
	if (crc == buff_Rcv[4]) {
		if (buff_Rcv[3] == 0x00 && buff_Rcv[4] == 0x6F) {
			xil_printf("Succeeded restore factory settings.\n\r");
		}
		else if(buff_Rcv[3] == 0x01 && buff_Rcv[4] == 0x70){
			xil_printf("Failed restore factory settings.\n\r");
		}
		else {
			xil_printf("Failed restore factory settings.\n\r");
		}
	}
	else {
		xil_printf("Checksum failed.\n\r");
	}
	return XST_SUCCESS;
}

int tf02_pro_set_frame_rate(uint16_t frameRate) {
	// clear the uart buffers
	memset(buff_Snd, 0x00, sizeof(buff_Snd));
	memset(buff_Rcv, 0x00, sizeof(buff_Rcv));

	buff_Snd[0] = 0x5A;
	buff_Snd[1] = 0x06;
	buff_Snd[2] = 0x03;
	buff_Snd[3] = (0x00FF & frameRate);
	buff_Snd[4] = (0xFF00 & frameRate) >> 8;
	buff_Snd[5] = 0x00;
	
	// send uart buffer
	uartlite_Send(&mUartLiteLidar, buff_Snd, sizeof(buff_Snd)-3);

	// wait to send uart buffer
	//while(!XUartLite_IsSending(&mUartLiteLidar));
	usleep(1000*200);

	// read recieve buffer
	uartlite_Recv(&mUartLiteLidar, buff_Rcv, sizeof(buff_Rcv)-3);

	// calculate crc
	uint8_t crc = tf02_pro_set_checksum(&buff_Rcv[0], 5);

	if (crc == buff_Rcv[5]) {
		xil_printf("Frame rate = %d\n\r", frameRate);
	}
	else {
		xil_printf("Checksum Failed. Frame rate is not updated.\n\r");
	}
	return XST_SUCCESS;
}

uint8_t tf02_pro_set_checksum(uint8_t* ptr, uint8_t size) {
	uint16_t crc = 0x00;
	for (int i = 0; i < size; i++) {
		crc += ptr[i];
	}
	return (uint8_t)crc;
}

// Recommended Uart interface to run this library correctly.
int tf02_pro_set_comm_interface(OptCommInterface_t interface) {
	// clear the uart buffers
	memset(buff_Snd, 0x00, sizeof(buff_Snd));
	buff_Snd[0] = 0x5A;
	buff_Snd[1] = 0x05;
	buff_Snd[2] = 0x0A;
	buff_Snd[3] = interface;
	buff_Snd[4] = 0x00;
	
	// send uart buffer
	uartlite_Send(&mUartLiteLidar, buff_Snd, sizeof(buff_Snd));

	// wait to send uart buffer
	//while(!XUartLite_IsSending(&mUartLiteLidar));
	usleep(1000*200);


	if (interface == 0) {
		xil_printf("Communication interface is UART.\n\r");
	}
	else if (interface == 1) {
		xil_printf("Communication interface is I2C.\n\r");
	}
	else {
		xil_printf("Communication interface failed.\n\r");
	}
	return XST_SUCCESS;
}  

int tf02_pro_set_output_format(OptOutputFormat_t outputFormat) {
	// clear the uart buffers
	memset(buff_Snd, 0x00, sizeof(buff_Snd));
	memset(buff_Rcv, 0x00, sizeof(buff_Rcv));

	buff_Snd[0] = 0x5A;
	buff_Snd[1] = 0x05;
	buff_Snd[2] = 0x05;
	buff_Snd[3] = outputFormat;
	buff_Snd[4] = tf02_pro_set_checksum(&buff_Snd[0], 4);

	// send uart buffer
	uartlite_Send(&mUartLiteLidar, buff_Snd, sizeof(buff_Snd)-4);

	// wait to send uart buffer
	//while(!XUartLite_IsSending(&mUartLiteLidar));
	usleep(1000*200);

	// read recieve buffer
	uartlite_Recv(&mUartLiteLidar, buff_Rcv, sizeof(buff_Rcv)-4);

	uint8_t crc = tf02_pro_set_checksum(&buff_Rcv[0], 4);

	if (crc == buff_Snd[4]) {
		xil_printf("Output format = %d\n\r", outputFormat);
	}
	else {
		xil_printf("Checksum Failed. Output format is not updated.\n\r");
	}
	return XST_SUCCESS;
}

int tf02_pro_set_baudrate(OptBaudRate_t baudRate) {
	uint32_t baudrate = baudRate;
	// clear the uart buffers
	memset(buff_Snd, 0x00, sizeof(buff_Snd));
	memset(buff_Rcv, 0x00, sizeof(buff_Rcv));

	buff_Snd[0] = 0x5A;
	buff_Snd[1] = 0x08;
	buff_Snd[2] = 0x06;
	buff_Snd[3] = (baudrate & 0x000000FF);
	buff_Snd[4] = (baudrate & 0x0000FF00) >> 8;
	buff_Snd[5] = (baudrate & 0x00FF0000) >> 16;
	buff_Snd[6] = (baudrate & 0xFF000000) >> 24;
	buff_Snd[7] = 0x00;

	// send uart buffer
	uartlite_Send(&mUartLiteLidar, buff_Snd, sizeof(buff_Snd)-1);

	// wait to send uart buffer
	//while(!XUartLite_IsSending(&mUartLiteLidar));
	usleep(1000*200);

	// read recieve buffer
	uartlite_Recv(&mUartLiteLidar, buff_Rcv, sizeof(buff_Rcv)-1);

	// calculate crc
	uint8_t crc = tf02_pro_set_checksum(&buff_Rcv[0], 7);

	if (crc == buff_Rcv[7]) {
		xil_printf("Baudrate = %d\n\r", baudrate);
	}
	else {
		xil_printf("Checksum Failed. Baudrate is not updated.\n\r");
	}
	return XST_SUCCESS;
}

int tf02_pro_set_data_output_mode(OptOutputMode_t outputMode) {
	// clear the uart buffers
	memset(buff_Snd, 0x00, sizeof(buff_Snd));
	memset(buff_Rcv, 0x00, sizeof(buff_Rcv));

	buff_Snd[0] = 0x5A;
	buff_Snd[1] = 0x05;
	buff_Snd[2] = 0x07;
	buff_Snd[3] = outputMode;
	buff_Snd[4] = tf02_pro_set_checksum(&buff_Snd[0], 4);

	// send uart buffer
	uartlite_Send(&mUartLiteLidar, buff_Snd, sizeof(buff_Snd)-4);

	// wait to send uart buffer
	//while(!XUartLite_IsSending(&mUartLiteLidar));
	usleep(1000*200);

	// read recieve buffer
	uartlite_Recv(&mUartLiteLidar, buff_Rcv, sizeof(buff_Rcv)-4);

	// calculate crc
	uint8_t crc = tf02_pro_set_checksum(&buff_Rcv[0], 4);

	if (crc == buff_Snd[4]) {
		if (outputMode == 0) {
			xil_printf("Disable data output.\n\r");
		}
		else if (outputMode == 1) {
			xil_printf("Enable data output.\n\r");
		}
	}
	else {
		xil_printf("Checksum failed.\n\r");
	}
	return XST_SUCCESS;
}

int tf02_pro_init(uint16_t frameRate, OptBaudRate_t baudRate, OptOutputFormat_t outputFormat){

	tf02_pro_config_mode(enable);
    usleep(1000*200);
    tf02_pro_get_firmare_version();
    usleep(1000*200);
    tf02_pro_set_restore_fact_settings();
    usleep(1000*200);
    tf02_pro_set_frame_rate(frameRate);  //max 1000
    usleep(1000*200);
    tf02_pro_set_output_format(outputFormat);
    usleep(1000*200);
    tf02_pro_set_baudrate(baudRate);
    usleep(1000*200);
    //tf02_pro_set_data_output_mode(enable_output);
    tf02_pro_config_mode(disable);
    usleep(1000*200);

	return XST_SUCCESS;
}


int tf02_pro_get_lidar_data() {
	//uint32_t result = 0;
	memset(buff_Rcv, 0x00, sizeof(buff_Rcv));
	
	// read recieve buffer
	uartlite_Recv(&mUartLiteLidar, buff_Rcv, sizeof(buff_Rcv));

	if (buff_Rcv[0] == 0x59 && buff_Rcv[1] == 0x59) {

		// calculate crc
		uint8_t crc = tf02_pro_set_checksum(&buff_Rcv[0], 8);
		
		if (buff_Rcv[8] == crc) {
			xil_printf("Distance = %d\n\r", (buff_Rcv[2] + (buff_Rcv[3] << 8)));
			xil_printf("Strength = %d\n\r", (buff_Rcv[4] + (buff_Rcv[5] << 8)));
		}
		else {
			xil_printf("Checksum Failed.\n\r");
		}
	}
	return ((buff_Rcv[3] << 24) + (buff_Rcv[2] << 16) + (buff_Rcv[5] << 8) + buff_Rcv[4]);
}
