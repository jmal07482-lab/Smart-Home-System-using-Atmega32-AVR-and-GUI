/*
 * KP.c
 *
 *  Created on: Nov 10, 2025
 *      Author:  Ahmed Oraby Mohamed
 */

#include "../../LIB/Bit_Math.h"
#include "../../LIB/STDTYPES.h"

#include "../../MCAL/DIO/DIO.h"
#include "KP.h"
#include <util/delay.h>

u8 KP_arr[4][4]=
{
		{'7','4','1','C'},
		{'8','5','2','0'},
		{'9','6','3','='},
		{'/','*','-','+'}
};

void KP_Init       (u8 KP_Port )
{
	DIO_InitPort(KP_Port,0xf0);
	DIO_SetPortValue(KP_Port,0xff);
}
u8   KP_GetValue (u8 KP_Port )
{
	u8 value=KP_NOT_PRESSED;
	u8 state;
	u8 row,col;
	for(col=0;col<4;col++)
	{
		DIO_SetPinValue(KP_Port,col+4,DIO_LOW);
		for(row=0;row<4;row++)
		{
			state=DIO_ReadPinValue(KP_Port,row);
			if(state==KP_PRESSED)
			{
				_delay_ms(20);
				state=DIO_ReadPinValue(KP_Port,row);
				if(state==KP_PRESSED)
				{
					value=KP_arr[col][row];
					while(DIO_ReadPinValue(KP_Port,row)==KP_PRESSED);
					_delay_ms(20);

				}
			}
		}
		DIO_SetPinValue(KP_Port,col+4,DIO_HIGH);
	}
	return value;
}


