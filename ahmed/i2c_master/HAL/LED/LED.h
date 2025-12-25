/*
 * LED.h
 *
 *  Created on: Nov 10, 2025
 *      Author: Ahmed Oraby
 */

#ifndef HAL_LED_LED_H_
#define HAL_LED_LED_H_
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"

void LED_Init (u8 LedPort , u8 LedPin) ;
void LED_On   (u8 LedPort , u8 LedPin) ;
void LED_Off  (u8 LedPort , u8 LedPin) ;
void LED_Tog  (u8 LedPort , u8 LedPin) ;
void LED_ON(u8 PORT_Name,u8 PIN_Number);
void LED_OFF(u8 PORT_Name,u8 PIN_Number);
s16 isledon(u8 PORT_Name,u8 PIN_Number);

#endif /* HAL_LED_LED_H_ */
