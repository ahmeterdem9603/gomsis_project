/*
 * uartlite.c
 *
 *  Created on: 13 Jan 2021
 *      Author: ilker.turgut
 */

#include "uartlite.h"

XUartLite mUartLiteLidar;
XUartLite mUartLiteWifi;

int uartlite_Init(void)
{
	int Status;
	XUartLite_Config* cfgUartLite;

	// mUartLiteLidar
	cfgUartLite = XUartLite_LookupConfig(XPAR_AXI_UARTLITE_LIDAR_DEVICE_ID);
	if (cfgUartLite == NULL) {
		xil_printf("Error : uartlite_Init() : XUartLite_LookupConfig() \n");
		return XST_DEVICE_NOT_FOUND;
	}

	Status = XUartLite_CfgInitialize(&mUartLiteLidar, cfgUartLite, cfgUartLite->RegBaseAddr);
	if (Status != XST_SUCCESS) {
		xil_printf("Error : uartlite_Init() : XUartPs_CfgInitialize() \n");
		return XST_FAILURE;
	}

	Status = XUartLite_SelfTest(&mUartLiteLidar);
	if (Status != XST_SUCCESS) {
		xil_printf("Error : uartlite_Init() : XUartPs_SelfTest() \n");
		return XST_FAILURE;
	}



	// mUartLiteWifi
	cfgUartLite = XUartLite_LookupConfig(XPAR_AXI_UARTLITE_WIFI_DEVICE_ID);
	if (cfgUartLite == NULL) {
		xil_printf("Error : uartlite_Init() : XUartLite_LookupConfig() \n");
		return XST_DEVICE_NOT_FOUND;
	}

	Status = XUartLite_CfgInitialize(&mUartLiteLidar, cfgUartLite, cfgUartLite->RegBaseAddr);
	if (Status != XST_SUCCESS) {
		xil_printf("Error : uartlite_Init() : XUartPs_CfgInitialize() \n");
		return XST_FAILURE;
	}

	Status = XUartLite_SelfTest(&mUartLiteLidar);
	if (Status != XST_SUCCESS) {
		xil_printf("Error : uartlite_Init() : XUartPs_SelfTest() \n");
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int uartlite_Send(XUartLite* ptrUart, u8* buff, u8 size)
{
	u32 count = XUartLite_Send(ptrUart, buff, size);
	if(count != size) {
		xil_printf("Error : uartlite_Send() : XUartLite_Send() \n");
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

int uartlite_Recv(XUartLite* ptrUart, u8* buff, u8 size)
{
	u32 count = XUartLite_Recv(ptrUart, buff, size);
	if(count != size) {
		xil_printf("Error : uartlite_Send() : XUartLite_Recv() \n");
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}
