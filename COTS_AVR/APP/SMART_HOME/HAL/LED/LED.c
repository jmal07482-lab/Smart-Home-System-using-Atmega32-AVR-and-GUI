/*
 * LED.c
 *
 *  Created on: Nov 10, 2025
 *      Author: Ahmed Oraby
 */

#include "../../LIB/Bit_Math.h"
#include "../../LIB/STDTYPES.h"

#include "../../MCAL/DIO/DIO.h"

#include "LED.h"


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
