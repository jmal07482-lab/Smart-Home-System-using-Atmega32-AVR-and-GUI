/*
 * KP.h
 *
 *  Created on: Nov 10, 2025
 *      Author:  Ahmed Oraby Mohamed
 */

#ifndef HAL_KP_KP_H_
#define HAL_KP_KP_H_

#define KP_NOT_PRESSED  255
#define KP_PRESSED      0


void KP_Init       (u8 KP_Port ) ;
u8   KP_GetValue (u8 KP_Port ) ;

#endif /* HAL_KP_KP_H_ */


