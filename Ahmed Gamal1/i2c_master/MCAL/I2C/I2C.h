/*
 * I2C.h
 * الميزات: دعم السترينج، تنظيف الذاكرة، وفحص جاهزية السليف
 */

#ifndef MCAL_I2C_I2C_H_
#define MCAL_I2C_I2C_H_

#include "../../LIB/STD_TYPES.h"

/* تعريف السجلات */
#define TWBR   (*(volatile u8*)(0x20))
#define TWCR   (*(volatile u8*)(0x56))
#define TWSR   (*(volatile u8*)(0x21))
#define TWDR   (*(volatile u8*)(0x23))
#define TWAR   (*(volatile u8*)(0x22))

/* تعريف البتات */
#define TWINT  7
#define TWEA   6
#define TWSTA  5
#define TWSTO  4
#define TWEN   2

/* الإعدادات */
#define PRESCALLER       1
#define I2C_WRITE        0
#define I2C_READ         1

/* أكواد الحالة المتفق عليها بين الماستر والسليف */
#define SLAVE_READY_BYTE  0xFF
#define SLAVE_BUSY_BYTE   0x00

/* البروتوتايب */
void I2C_MasterInit (u32 SCL);
void I2C_SendStartCond (void);
void I2C_SendAddress (u8 Address, u8 Type);
void I2C_SendData (u8 Data);
u8   I2C_ReadDataACK (void);
u8   I2C_ReadDataNACK (void);
void I2C_SendStopCondition (void);

/* الدوال الذكية للتعامل مع النصوص والجاهزية */
void I2C_ClearBuffer(char* Buffer, u8 Length);
void I2C_SendString (u8 Address, char* Str);
u8   I2C_ReceiveString_Smart(u8 Address, char* Buffer, u8 Length);

#endif
