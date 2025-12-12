
/*
 * LCD.h
 *
 *  Created on: Nov 10, 2025
 *      Author:  Ahmed Oraby Mohamed
 */

#ifndef HAL_LCD_LCD_H_
#define HAL_LCD_LCD_H_

#define LCD_PORT DIO_PORTC

#define RS_PORT DIO_PORTD
#define RS_PIN DIO_PIN0

#define RW_PORT DIO_PORTD
#define RW_PIN DIO_PIN1

#define E_PORT DIO_PORTD
#define E_PIN DIO_PIN2

void LCD_Init(void);
void LCD_WriteData (u8 Data);
void LCD_WriteCmd (u8 Cmd);
void LCD_WriteString (u8* Ptr_String);
void LCD_Clear_Display (void);
void LCD_Shift_Display (void);
void LCD_Move_Cursor (u8 row,u8 col);
void LCD_WriteCustomChar (u8 ARR[],u8 blockNumber,u8 row,u8 col);

#endif /* HAL_LCD_LCD_H_ */

