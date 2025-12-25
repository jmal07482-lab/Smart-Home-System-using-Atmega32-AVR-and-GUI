/*
 * UART.c - Receiver
 */
#include "UART.h"
#include <stdlib.h>
#include <string.h>

#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"

void UART_Init(u32 BoadRate, u8 DataSize)
{
	u16 UBRR = (8000000/(16*BoadRate))-1;
	UBRRL = (u8)UBRR;
	UBRRH = UBRR>>8;

	SET_BIT(UCSRB, TXEN);
	SET_BIT(UCSRB, RXEN);

	switch (DataSize)
	{
	case 5:
		SET_BIT(UCSRC, URSEL);
		CLR_BIT(UCSRB, UCSZ2);
		CLR_BIT(UCSRC, UCSZ1);
		CLR_BIT(UCSRC, UCSZ0);
		break;
	case 6:
		SET_BIT(UCSRC, URSEL);
		CLR_BIT(UCSRB, UCSZ2);
		CLR_BIT(UCSRC, UCSZ1);
		SET_BIT(UCSRC, UCSZ0);
		break;
	case 7:
		SET_BIT(UCSRC, URSEL);
		CLR_BIT(UCSRB, UCSZ2);
		SET_BIT(UCSRC, UCSZ1);
		CLR_BIT(UCSRC, UCSZ0);
		break;
	case 8:
		SET_BIT(UCSRC, URSEL);
		CLR_BIT(UCSRB, UCSZ2);
		SET_BIT(UCSRC, UCSZ1);
		SET_BIT(UCSRC, UCSZ0);
		break;
	case 9:
		SET_BIT(UCSRC, URSEL);
		SET_BIT(UCSRB, UCSZ2);
		SET_BIT(UCSRC, UCSZ1);
		SET_BIT(UCSRC, UCSZ0);
		break;
	}
}

void UART_Tx(u8 Data)
{
	while (READ_BIT(UCSRA, UDRE) == 0);
	UDR = Data;
}

u8 UART_RX(void)
{
	while (READ_BIT(UCSRA, RXC) == 0);
	return UDR;
}

void UART_ReadString(u8 *buffer, u8 maxLen)
{
	u8 i = 0;
	while(1)
	{
		u8 c = UART_RX();
		if(c == '\n')
		{
			buffer[i] = '\0';
			return;
		}
		buffer[i++] = c;
		if(i >= maxLen - 1)
		{
			buffer[i] = '\0';
			return;
		}
	}
}

u16 GetSensorValue(u8 *buffer, u8 *tag)
{
	u8 *p = strstr(buffer, tag);
	if(p == NULL) return 255;
	p += strlen(tag);

	u8 numStr[6];
	u8 i = 0;
	while(p[i] != ';' && p[i] != '\0' && i < 5)
	{
		numStr[i] = p[i];
		i++;
	}
	numStr[i] = '\0';
	return (u16)atoi((char*)numStr);
}
