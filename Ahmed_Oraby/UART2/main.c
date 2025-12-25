
/*
 * main.c - Receiver
 */
#include "MCAL/DIO/DIO.h"
#include "MCAL/UART/UART.h"
#include "HAL/LED/LED.h"
#include "HAL/LCD/LCD.h"

#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

#include "LIB/BIT_MATH.h"
#include "LIB/STD_TYPES.h"

#define RX_BUFFER_SIZE 60

void LCD_WriteNumber(u16 num)
{
    char buffer[6];
    sprintf(buffer, "%u", num);
    LCD_WriteString(buffer);
}

int main(void)
{
	u8 RX_Buffer[RX_BUFFER_SIZE];
	u16 S1, S2, S3, S4;

	LCD_Init();
	UART_Init(9600, 8);
	LED_Init(DIO_PORTA, DIO_PIN0);

	LCD_WriteString("Sensors:");

	while(1)
	{
		UART_ReadString(RX_Buffer, RX_BUFFER_SIZE);

		S1 = GetSensorValue(RX_Buffer, (u8*)"S1:");
		S2 = GetSensorValue(RX_Buffer, (u8*)"S2:");
		S3 = GetSensorValue(RX_Buffer, (u8*)"S3:");
		S4 = GetSensorValue(RX_Buffer, (u8*)"S4:");


		if(S1 > 150)
			LED_On(DIO_PORTA, DIO_PIN0);
		else
			LED_Off(DIO_PORTA, DIO_PIN0);


		LCD_Move_Cursor(1,0);
		LCD_WriteString("S1:");
		LCD_WriteNumber(S1);
		LCD_WriteString(" S2:");
		LCD_WriteNumber(S2);

		LCD_Move_Cursor(2,0);
		LCD_WriteString("S3:");
		LCD_WriteNumber(S3);
		LCD_WriteString(" S4:");
		LCD_WriteNumber(S4);


		_delay_ms(200);
	}

	return 0;
}


///*
// * main.c
// *
// *  Created on:
// *      Author: Ahmed Oraby Mohamed
// */
//
//#include "LIB/STDTYPES.h"
//#include "LIB/Bit_Math.h"
//
//#include "MCAL/DIO/DIO.h"
//#include "MCAL/UART/UART.h"
//#include "HAL/LCD/LCD.h"
//#include "HAL/LED/LED.h"
//
//#include <util/delay.h>
//#include <stdlib.h>
//#include <string.h>
//
//#define RX_BUFFER_SIZE 60
//
//
//
//
//int main(void)
//{
//    u8 RX_Buffer[RX_BUFFER_SIZE];
//    u16 S1, S2, S3, S4;
//
// //   LCD_Init();
//    UART_Init(9600, 8);
//    LED_Init(DIO_PORTA, DIO_PIN0);
//
//   // LCD_WriteString("Sensors:");
//
//    while(1)
//    {
//        UART_ReadString(RX_Buffer, RX_BUFFER_SIZE);
//
//        S1 = GetSensorValue(RX_Buffer, (u8*)"S1:");
//        S2 = GetSensorValue(RX_Buffer, (u8*)"S2:");
//        S3 = GetSensorValue(RX_Buffer, (u8*)"S3:");
//        S4 = GetSensorValue(RX_Buffer, (u8*)"S4:");
//
//        if(S1 > 100)
//            LED_On(DIO_PORTA, DIO_PIN0);
//        else
//            LED_Off(DIO_PORTA, DIO_PIN0);
//
////        LCD_Move_Cursor(1,0);
////        LCD_WriteString("S1:");
////        LCD_WriteData(S1);
////        LCD_WriteString(" S2:");
////        LCD_WriteData(S2);
////
////        LCD_Move_Cursor(2,0);
////        LCD_WriteString("S3:");
////        LCD_WriteData(S3);
////        LCD_WriteString(" S4:");
////        LCD_WriteData(S4);
//
// //       _delay_ms(200);
//    }
//
//    return 0;
//}
