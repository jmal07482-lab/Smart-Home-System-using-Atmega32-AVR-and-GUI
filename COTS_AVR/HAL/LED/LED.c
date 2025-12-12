/*
 * LED.c
 *
 *  Created on: Aug 12, 2025
 *      Author: Mohammed Ramadan
 */
#include "../../LIB/Bit_Math.h"
#include "../../LIB/STDTYPES.h"

#include "../../MCAL/DIO/DIO.h"

#include "LED.h"


void LED_Init (u8 PORT_Name , u8 PIN_Number)
{
	DIO_InitPin(PORT_Name, PIN_Number, DIO_OUTPUT) ;
}
void LED_On   (u8 PORT_Name , u8 PIN_Number)
{
	DIO_SetPinValue(PORT_Name, PIN_Number, DIO_HIGH) ;
}
void LED_Off  (u8 PORT_Name , u8 PIN_Number)
{
	DIO_SetPinValue(PORT_Name, PIN_Number, DIO_LOW) ;
}
void LED_TOG  (u8 PORT_Name , u8 PIN_Number)
{
	DIO_TogPinValue(PORT_Name, PIN_Number) ;
}
