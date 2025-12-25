/*
 * UART.c - Transmitter
 */
#include "UART.h"
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"
#include <stdio.h>

void UART_Init (u32 BoadRate , u8 DataSize)
{
	u16 UBRR = (8000000/(16*BoadRate))-1 ;
	UBRRL = (u8)UBRR ;
	UBRRH = UBRR>>8 ;

	SET_BIT(UCSRB , TXEN) ;
	SET_BIT(UCSRB , RXEN) ;

	switch (DataSize)
	{
	case 5 :
		SET_BIT(UCSRC , URSEL) ;
		CLR_BIT(UCSRB , UCSZ2) ;
		CLR_BIT(UCSRC , UCSZ1) ;
		CLR_BIT(UCSRC , UCSZ0) ;
		break ;
	case 6 :
		SET_BIT(UCSRC , URSEL) ;
		CLR_BIT(UCSRB , UCSZ2) ;
		CLR_BIT(UCSRC , UCSZ1) ;
		SET_BIT(UCSRC , UCSZ0) ;
		break ;
	case 7 :
		SET_BIT(UCSRC , URSEL) ;
		CLR_BIT(UCSRB , UCSZ2) ;
		SET_BIT(UCSRC , UCSZ1) ;
		CLR_BIT(UCSRC , UCSZ0) ;
		break ;
	case 8 :
		SET_BIT(UCSRC , URSEL) ;
		CLR_BIT(UCSRB , UCSZ2) ;
		SET_BIT(UCSRC , UCSZ1) ;
		SET_BIT(UCSRC , UCSZ0) ;
		break ;
	case 9 :
		SET_BIT(UCSRC , URSEL) ;
		SET_BIT(UCSRB , UCSZ2) ;
		SET_BIT(UCSRC , UCSZ1) ;
		SET_BIT(UCSRC , UCSZ0) ;
		break ;
	}
}

void UART_Tx(u8 Data)
{
	while (READ_BIT(UCSRA , UDRE) == 0) ;
	UDR = Data ;
}

void UART_TxStr(u8 str[])
{
	u8 i = 0;
	while(str[i] != '\0')
	{
		UART_Tx(str[i]);
		i++;
	}

}

void SendSensors(u16 s1, u16 s2, u16 s3, u16 s4)
{
	u8 buffer[60];
	sprintf(buffer, "S1:%d;S2:%d;S3:%d;S4:%d;\n", s1, s2, s3, s4);
	UART_TxStr(buffer);
}

u8 UART_RX(void)
{
	while (READ_BIT(UCSRA, RXC) == 0);
	return UDR;
}
