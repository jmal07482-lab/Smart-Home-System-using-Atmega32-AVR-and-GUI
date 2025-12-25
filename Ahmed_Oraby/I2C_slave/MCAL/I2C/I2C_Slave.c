/*
 * I2C_slave.c
 *
 *  Created on:
 *      Author:  Ahmed Oraby Mohamed
 */
#include "I2C_Slave.h"
#include "../../LIB/BIT_MATH.h"
#include <util/delay.h>
#include <string.h>

void I2C_SlaveInit(u8 Address) {
    TWAR = (Address << 1);
    TWCR = (1<<TWEN) | (1<<TWEA);
}

void I2C_ClearBuffer(char* Buffer, u8 Length) {
    if (Buffer != NULL) {
        memset(Buffer, '\0', Length);
    }
}

u8 I2C_SlaveReadData(void) {
    // تفعيل الـ I2C وانتظار الماستر ليبدأ العملية
    TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

void I2C_SlaveWriteData(u8 Data) {
    TWDR = Data;
    TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
}

/* دالة السليف المعدلة لتتوافق مع الماستر الذكي */
void I2C_SlaveSendString_Smart(char* Str, u8 IsReady) {
    // 1. انتظار اختيار الماستر للسليف (Address Match)
    TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
    while (!(TWCR & (1<<TWINT)));

    // 2. إرسال بايت الحالة (أول طلب من الماستر)
    if (IsReady) {
        TWDR = SLAVE_READY_BYTE;
    } else {
        TWDR = SLAVE_BUSY_BYTE;
    }
    TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
    while (!(TWCR & (1<<TWINT)));

    // إذا كان الماستر سيكمل القراءة (الطلب الثاني)
    if (IsReady) {
        u8 i = 0;
        // ننتظر حتى يرسل الماستر Start ثانية ويطلب العنوان
        TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
        while (!(TWCR & (1<<TWINT)));

        while (Str[i] != '\0') {
            TWDR = Str[i++];
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
            while (!(TWCR & (1<<TWINT)));
        }
    }
}

/* دالة الاستقبال الذكية (من الماستر للسليف) */
void I2C_SlaveReceiveString_Smart(char* Buffer, u8 MaxLen) {
    I2C_ClearBuffer(Buffer, MaxLen);
    u8 i = 0;

    // السليف هيفضل مستني الماستر يبعت عنوانه
    TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
    while (!(TWCR & (1<<TWINT)));

    // استقبال الحروف حرف حرف
    for(i = 0; i < MaxLen - 1; i++) {
        TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
        while (!(TWCR & (1<<TWINT)));

        Buffer[i] = TWDR;

        // لو استلمنا علامة النهاية المتفق عليها #
        if(Buffer[i] == '#' || Buffer[i] == '\0') {
            Buffer[i] = '\0';
            break;
        }
    }
}
