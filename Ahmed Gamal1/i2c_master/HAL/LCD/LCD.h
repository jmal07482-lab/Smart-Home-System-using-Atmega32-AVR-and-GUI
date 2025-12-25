/*
 * LCD.h
 *
 *   Created on: Nov 10, 2025
 *      Author: Ahmed Oraby
 */

#ifndef HAL_LCD_LCD_H_
#define HAL_LCD_LCD_H_

#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"

#define LCD_DATA_PORT DIO_PORTB

#define LCD_RS_PORT   DIO_PORTB
#define LCD_RS_PIN    DIO_PIN0

#define LCD_RW_PORT   DIO_PORTB
#define LCD_RW_PIN    DIO_PIN1

#define LCD_E_PORT   DIO_PORTB
#define LCD_E_PIN    DIO_PIN2

#define ROW_0       0
#define ROW_1       1

#define COL_0       0
#define COL_1       1
#define COL_2       2
#define COL_3       3
#define COL_4       4
#define COL_5       5
#define COL_6       6
#define COL_7       7
#define COL_8       8
#define COL_9       9
#define COL_10      10
#define COL_11      11
#define COL_12      12
#define COL_13      13
#define COL_14      14
#define COL_15      15

#define BLOCK_0     0
#define BLOCK_1     1
#define BLOCK_2     2
#define BLOCK_3     3
#define BLOCK_4     4
#define BLOCK_5     5
#define BLOCK_6     6
#define BLOCK_7     7

#define MODE        4

void LCD_Init      (void) ;
void LCD_WriteData (u8 Data) ;
void LCD_WriteCmd  (u8 Cmd)  ;
void LCD_WriteString (u8 str []) ;
void LCD_Clear_Display (void) ;
void LCD_ShiftLeft (void) ;
void LCD_ShiftRight (void) ;
void LCD_Move_Cursor (u8 row , u8 col) ;
void LCD_DispalyCustomChar (u8 Arr [] , u8 BlockNumber , u8 row , u8 col) ;
#endif /* HAL_LCD_LCD_H_ */
