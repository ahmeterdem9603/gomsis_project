/*
 * uartlite.h
 *
 *  Created on: 13 Jan 2021
 *      Author: ilker.turgut
 */

#ifndef SRC_UARTLITE_H_
#define SRC_UARTLITE_H_

#include "platform.h"

#include "xuartlite.h"

extern XUartLite mUartLiteLidar;
extern XUartLite mUartLiteWifi;

extern int uartlite_Init(void);
extern int uartlite_Send(XUartLite* ptrUart, u8* buff, u8 size);
extern int uartlite_Recv(XUartLite* ptrUart, u8* buff, u8 size);
extern int XUartLite_IsSending(XUartLite *InstancePtr);

#endif // SRC_UARTLITE_H_

