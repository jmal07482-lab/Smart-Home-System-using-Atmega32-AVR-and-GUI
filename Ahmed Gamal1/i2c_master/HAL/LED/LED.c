/*
 * LED.c
 *
 *  Created on: Nov 10, 2025
 *      Author: Ahmed Oraby
 */

#include "../../LIB/STD_TYPES.h"

#include "../../MCAL/DIO/DIO.h"

#include "LED.h"
#include "../../LIB/BIT_MATH.h"


void LED_Init (u8 LedPort , u8 LedPin)
{
	DIO_InitPin(LedPort, LedPin, DIO_OUTPUT) ;
}
void LED_On   (u8 LedPort , u8 LedPin)
{
	DIO_SetPinValue(LedPort, LedPin, DIO_HIGH) ;
}
void LED_Off  (u8 LedPort , u8 LedPin)
{
	DIO_SetPinValue(LedPort, LedPin, DIO_LOW) ;
}
void LED_Tog  (u8 LedPort , u8 LedPin)
{
	DIO_TogPinValue(LedPort, LedPin) ;
}
void LED_ON(u8 PORT_Name,u8 PIN_Number)
{
	DIO_SetPinValue(PORT_Name,PIN_Number,DIO_HIGH);
}
void LED_OFF(u8 PORT_Name,u8 PIN_Number)
{
	DIO_SetPinValue(PORT_Name,PIN_Number,DIO_LOW);
}
s16 isledon(u8 PORT_Name,u8 PIN_Number)
{
	if(DIO_ReadPinValue(PORT_Name,PIN_Number)==1)
		return 1;
	else
		return 0;

}
