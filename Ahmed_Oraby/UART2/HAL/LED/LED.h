/*
 * LED.h
 *
 *  Created on: Nov 10, 2025
 *      Author: Ahmed Oraby
 */

#ifndef HAL_LED_LED_H_
#define HAL_LED_LED_H_

void LED_Init (u8 LedPort , u8 LedPin) ;
void LED_On   (u8 LedPort , u8 LedPin) ;
void LED_Off  (u8 LedPort , u8 LedPin) ;
void LED_Tog  (u8 LedPort , u8 LedPin) ;

#endif /* HAL_LED_LED_H_ */
