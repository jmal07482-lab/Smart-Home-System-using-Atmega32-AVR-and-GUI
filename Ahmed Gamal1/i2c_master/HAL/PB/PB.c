/*
 * PB.c
 *
 *  Created on: Dec 12, 2025
 *      Author: Getli
 */
#include"../../LIB/BIT_MATH.h"
#include"../../LIB/STD_Types.h"
#include"../../MCAL/DIO/DIO.h"
#include"../../MCAL/DIO/DIO.h"
#include"PB.h"

void PB_Init(u8 PORT_Name,u8 PIN_Number)
{
	DIO_InitPin(PORT_Name,PIN_Number,DIO_INPUT);
}
u8 PB_Readstatues(u8 PORT_Name,u8 PIN_Number)
{
	return DIO_ReadPinValue(PORT_Name ,  PIN_Number          );
}
