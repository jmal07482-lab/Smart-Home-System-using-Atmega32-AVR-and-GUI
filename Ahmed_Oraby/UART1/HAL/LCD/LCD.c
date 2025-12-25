/*
 * LCD.c
 *
 *   Created on: Nov 10, 2025
 *      Author: Ahmed Oraby
 */
#include "../../MCAL/DIO/DIO.h"

#include "LCD.h"
#include <util/delay.h>
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"

void LCD_Init      (void)
{
#if MODE == 8
	_delay_ms (50) ;
	DIO_InitPort(LCD_DATA_PORT, 0xff) ;
	DIO_InitPin(LCD_RS_PORT, LCD_RS_PIN, DIO_OUTPUT) ;
	DIO_InitPin(LCD_RW_PORT, LCD_RW_PIN, DIO_OUTPUT) ;
	DIO_InitPin(LCD_E_PORT, LCD_E_PIN, DIO_OUTPUT) ;

	LCD_WriteCmd(0x38) ;
	_delay_ms(1) ;

	LCD_WriteCmd(0x0E) ;
	_delay_ms(1) ;

	LCD_WriteCmd(0x01) ;
	_delay_ms(5) ;

	LCD_WriteCmd(0x06) ;
	_delay_ms(1) ;

#elif MODE == 4
	_delay_ms (50) ;
	DIO_InitPin(LCD_DATA_PORT, DIO_PIN4, DIO_OUTPUT) ;
	DIO_InitPin(LCD_DATA_PORT, DIO_PIN5, DIO_OUTPUT) ;
	DIO_InitPin(LCD_DATA_PORT, DIO_PIN6, DIO_OUTPUT) ;
	DIO_InitPin(LCD_DATA_PORT, DIO_PIN7, DIO_OUTPUT) ;

	DIO_InitPin(LCD_RS_PORT, LCD_RS_PIN, DIO_OUTPUT) ;
	DIO_InitPin(LCD_RW_PORT, LCD_RW_PIN, DIO_OUTPUT) ;
	DIO_InitPin(LCD_E_PORT, LCD_E_PIN, DIO_OUTPUT) ;

	LCD_WriteCmd(0x02) ;
	_delay_ms(1) ;

	LCD_WriteCmd(0x28) ;
	_delay_ms(1) ;

	LCD_WriteCmd(0x0E) ;
	_delay_ms(1) ;

	LCD_WriteCmd(0x01) ;
	_delay_ms(5) ;

	LCD_WriteCmd(0x06) ;
	_delay_ms(1) ;
#endif
}
void LCD_WriteData (u8 Data)
{
#if MODE == 8
	DIO_SetPinValue(LCD_RW_PORT, LCD_RW_PIN, DIO_LOW) ;
	DIO_SetPinValue(LCD_RS_PORT, LCD_RS_PIN, DIO_HIGH) ;
	DIO_SetPortValue(LCD_DATA_PORT, Data) ;
	DIO_SetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_HIGH) ;
	_delay_ms (1) ;
	DIO_SetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_LOW) ;
	_delay_ms (1) ;
#elif MODE == 4
	u8 HighNibble = Data >> 4 ;
	u8 LowNibble  = Data & 0x0f ;
	DIO_SetPinValue(LCD_RW_PORT, LCD_RW_PIN, DIO_LOW) ;
	DIO_SetPinValue(LCD_RS_PORT, LCD_RS_PIN, DIO_HIGH) ;

	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN4, READ_BIT(HighNibble , 0)) ;
	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN5, READ_BIT(HighNibble , 1)) ;
	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN6, READ_BIT(HighNibble , 2)) ;
	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN7, READ_BIT(HighNibble , 3)) ;

	DIO_SetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_HIGH) ;
	_delay_ms (1) ;
	DIO_SetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_LOW) ;
	_delay_ms (1) ;


	DIO_SetPinValue(LCD_RW_PORT, LCD_RW_PIN, DIO_LOW) ;
	DIO_SetPinValue(LCD_RS_PORT, LCD_RS_PIN, DIO_HIGH) ;

	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN4, READ_BIT(LowNibble , 0)) ;
	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN5, READ_BIT(LowNibble , 1)) ;
	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN6, READ_BIT(LowNibble , 2)) ;
	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN7, READ_BIT(LowNibble , 3)) ;

	DIO_SetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_HIGH) ;
	_delay_ms (1) ;
	DIO_SetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_LOW) ;
	_delay_ms (1) ;
#endif
}
void LCD_WriteCmd  (u8 Cmd)
{
#if MODE == 8
	DIO_SetPinValue(LCD_RW_PORT, LCD_RW_PIN, DIO_LOW) ;
	DIO_SetPinValue(LCD_RS_PORT, LCD_RS_PIN, DIO_LOW) ;
	DIO_SetPortValue(LCD_DATA_PORT, Data) ;
	DIO_SetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_HIGH) ;
	_delay_ms (1) ;
	DIO_SetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_LOW) ;
	_delay_ms (1) ;
#elif MODE == 4
	u8 HighNibble = Cmd >> 4 ;
	u8 LowNibble  = Cmd & 0x0f ;
	DIO_SetPinValue(LCD_RW_PORT, LCD_RW_PIN, DIO_LOW) ;
	DIO_SetPinValue(LCD_RS_PORT, LCD_RS_PIN, DIO_LOW) ;

	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN4, READ_BIT(HighNibble , 0)) ;
	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN5, READ_BIT(HighNibble , 1)) ;
	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN6, READ_BIT(HighNibble , 2)) ;
	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN7, READ_BIT(HighNibble , 3)) ;

	DIO_SetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_HIGH) ;
	_delay_ms (1) ;
	DIO_SetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_LOW) ;
	_delay_ms (1) ;


	DIO_SetPinValue(LCD_RW_PORT, LCD_RW_PIN, DIO_LOW) ;
	DIO_SetPinValue(LCD_RS_PORT, LCD_RS_PIN, DIO_LOW) ;

	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN4, READ_BIT(LowNibble , 0)) ;
	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN5, READ_BIT(LowNibble , 1)) ;
	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN6, READ_BIT(LowNibble , 2)) ;
	DIO_SetPinValue(LCD_DATA_PORT, DIO_PIN7, READ_BIT(LowNibble , 3)) ;

	DIO_SetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_HIGH) ;
	_delay_ms (1) ;
	DIO_SetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_LOW) ;
	_delay_ms (1) ;
#endif
}

void LCD_WriteString (u8 str [])
{
	u8 i = 0 ;
	while (str [i] != '\0')
	{
		LCD_WriteData(str [i]) ;
		i ++ ;
	}
}

void LCD_Clear_Display (void)
{
	LCD_WriteCmd(0x01) ;
	_delay_ms(5) ;
}

void LCD_ShiftLeft (void)
{
	LCD_WriteCmd(0x18) ;
	_delay_ms(1) ;
}
void LCD_ShiftRight (void)
{
	LCD_WriteCmd(0x1C) ;
	_delay_ms(1) ;
}
void LCD_Move_Cursor (u8 row , u8 col)
{
	u8 AC = 0 ;
	if (row == 0)
	{
		AC = col ;
	}
	else if (row == 1)
	{
		AC = col+0x40 ;
	}
	LCD_WriteCmd(AC+0x80) ;
}

void LCD_DispalyCustomChar (u8 Arr [] , u8 BlockNumber , u8 row , u8 col)
{
	u8 i ;
	LCD_WriteCmd(BlockNumber*8 + 0x40) ;
	for (i = 0 ; i < 8 ; i ++)
	{
		LCD_WriteData(Arr [i]) ;
	}
	LCD_Move_Cursor(row, col) ;
	LCD_WriteData(BlockNumber) ;
}
