/*
 * LED.h
 *
 *  Created on: Aug 12, 2025
 *      Author: Mohammed Ramadan
 */

#ifndef HAL_LED_LED_H_
#define HAL_LED_LED_H_

void LED_Init (u8 PORT_Name , u8 PIN_Number) ;
void LED_On   (u8 PORT_Name , u8 PIN_Number) ;
void LED_Off  (u8 PORT_Name , u8 PIN_Number) ;
void LED_TOG  (u8 PORT_Name , u8 PIN_Number) ;

#endif /* HAL_LED_LED_H_ */
