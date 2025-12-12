/*
 * LCD.c
 *
 *  Created on: Nov 10, 2025
 *      Author:  Ahmed Oraby Mohamed
 */

#include "../../LIB/Bit_Math.h"
#include "../../LIB/STDTYPES.h"

#include "../../MCAL/DIO/DIO.h"
#include <util/delay.h>

#include "LCD.h"

void LCD_Init(void)
{
	_delay_ms(50);
	DIO_InitPort(LCD_PORT,0xff);
	DIO_InitPin(RW_PORT,RW_PIN,DIO_OUTPUT);
	DIO_InitPin(RS_PORT,RS_PIN,DIO_OUTPUT);
	DIO_InitPin(E_PORT,E_PIN,DIO_OUTPUT);

	LCD_WriteCmd (0x38);
	_delay_ms(1);
	LCD_WriteCmd (0xF);
	_delay_ms(1);
	LCD_WriteCmd (0x01);
	_delay_ms(5);
	LCD_WriteCmd (0x06);
	_delay_ms(1);
}

void LCD_WriteData (u8 Data)
{
	DIO_SetPinValue(RS_PORT,RS_PIN,DIO_HIGH);
	DIO_SetPinValue(RW_PORT,RW_PIN,DIO_LOW);
	DIO_SetPortValue(LCD_PORT,Data);
	DIO_SetPinValue(E_PORT,E_PIN,DIO_HIGH);
	_delay_ms(1);
	DIO_SetPinValue(E_PORT,E_PIN,DIO_LOW);
}

void LCD_WriteCmd (u8 Cmd)
{
	DIO_SetPinValue(RS_PORT,RS_PIN,DIO_LOW);
	DIO_SetPinValue(RW_PORT,RW_PIN,DIO_LOW);
	DIO_SetPortValue(LCD_PORT,Cmd);
	DIO_SetPinValue(E_PORT,E_PIN,DIO_HIGH);
	_delay_ms(1);
	DIO_SetPinValue(E_PORT,E_PIN,DIO_LOW);
}

void LCD_WriteString (u8* Ptr_String)
{
	u8 i=0;
	while(Ptr_String [i] !='\0')
	{
		LCD_WriteData (Ptr_String[i]);
		i++;
	}
}
void LCD_Clear_Display (void)
{
	LCD_WriteCmd (0x01);
	_delay_ms(5);
}
void LCD_Shift_Display (void)
{
	LCD_WriteCmd (0x1C);
		_delay_ms(5);
}

void LCD_Move_Cursor (u8 row,u8 col)
{
	u8 AC;
	if(row >= 0 && row <= 1)
	{
		if(col >=0 && col <= 15)
		{
			if(row == 0)
			{
				AC = col;
			}
			else
			{
				AC= col + 0x40;
			}
			LCD_WriteCmd (AC+0x80);
			_delay_ms(5);
		}
	}

}

void LCD_WriteCustomChar (u8 ARR[],u8 blockNumber,u8 row,u8 col)
{
	u8 i;
	LCD_WriteCmd (0x40+(8*blockNumber));
	for (i=0;i<8;i++)
	{
		LCD_WriteData (ARR[i]);
	}
	LCD_Move_Cursor (row,col);
	LCD_WriteData (blockNumber);
}








