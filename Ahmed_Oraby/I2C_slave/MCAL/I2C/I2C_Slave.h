#ifndef I2C_SLAVE_H_
#define I2C_SLAVE_H_

#include "../../LIB/STD_TYPES.h"

/* تعريف السجلات */
#define TWBR   (*(volatile u8*)(0x20))
#define TWSR   (*(volatile u8*)(0x21))
#define TWAR   (*(volatile u8*)(0x22))
#define TWDR   (*(volatile u8*)(0x23))
#define TWCR   (*(volatile u8*)(0x56))

/* تعريف البتات */
#define TWINT  7
#define TWEA   6
#define TWSTA  5
#define TWSTO  4
#define TWEN   2

/* أكواد الحالة المتفق عليها */
#define SLAVE_READY_BYTE  0xFF
#define SLAVE_BUSY_BYTE   0x00

/* الدوال */
void I2C_SlaveInit(u8 Address);
u8   I2C_SlaveReadData(void);
void I2C_SlaveWriteData(u8 Data);
void I2C_ClearBuffer(char* Buffer, u8 Length);

/* دوال الإرسال والاستقبال الذكية */
void I2C_SlaveSendString_Smart(char* Str, u8 IsReady);
void I2C_SlaveReceiveString_Smart(char* Buffer, u8 MaxLen);

#endif
