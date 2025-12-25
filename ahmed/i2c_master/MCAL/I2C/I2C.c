/*
 * I2C.c
 */

#include "../../LIB/Bit_Math.h"
#include "I2C.h"
#include <util/delay.h> // لعمل تأخير بسيط عند فحص الجاهزية
#include <string.h>

void I2C_MasterInit (u32 SCL) {
    TWBR = ((8000000/SCL)-16)/(2*PRESCALLER);
    TWSR = 0;
}

void I2C_ClearBuffer(char* Buffer, u8 Length) {
    if (Buffer != NULL) {
        memset(Buffer, '\0', Length);
    }
}

void I2C_SendStartCond (void) {
    TWCR = (1<<TWEN) | (1<<TWSTA) | (1<<TWINT);
    while (READ_BIT(TWCR , TWINT) == 0);
}

void I2C_SendAddress (u8 Address, u8 Type) {
    TWDR = (Address << 1) | Type;
    TWCR = (1<<TWEN) | (1<<TWINT);
    while (READ_BIT(TWCR , TWINT) == 0);
}

void I2C_SendData (u8 Data) {
    TWDR = Data;
    TWCR = (1<<TWEN) | (1<<TWINT);
    while (READ_BIT(TWCR , TWINT) == 0);
}

u8 I2C_ReadDataACK (void) {
    TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
    while (READ_BIT(TWCR , TWINT) == 0);
    return TWDR;
}

u8 I2C_ReadDataNACK (void) {
    TWCR = (1<<TWEN) | (1<<TWINT);
    while (READ_BIT(TWCR , TWINT) == 0);
    return TWDR;
}

void I2C_SendStopCondition (void) {
    TWCR = (1<<TWEN) | (1<<TWSTO) | (1<<TWINT);
}

void I2C_SendString (u8 Address, char* Str) {
    if (Str == NULL) return;
    I2C_SendStartCond();
    I2C_SendAddress(Address, I2C_WRITE);
    for (int i = 0; Str[i] != '\0'; i++) {
        I2C_SendData(Str[i]);
    }
    I2C_SendStopCondition();
}

/* الدالة الذكية: تفحص الجاهزية، تنظف البفر، ثم تستقبل */
/* الدالة الذكية المحدثة في الماستر لمنع الرموز الغريبة */
u8 I2C_ReceiveString_Smart(u8 Address, char* Buffer, u8 Length) {
    u8 status = SLAVE_BUSY_BYTE;
    u16 trials = 0;

    // 1. فحص الجاهزية
    while (trials < 50) {
        I2C_SendStartCond();
        I2C_SendAddress(Address, I2C_READ);
        status = I2C_ReadDataNACK();
        I2C_SendStopCondition();
        if (status == SLAVE_READY_BYTE) break;
        trials++;
        _delay_ms(5);
    }

    if (status == SLAVE_READY_BYTE) {
        _delay_us(100);
        I2C_ClearBuffer(Buffer, Length);

        I2C_SendStartCond();
        I2C_SendAddress(Address, I2C_READ);

        for (u8 i = 0; i < Length - 1; i++) {
            u8 data = I2C_ReadDataACK();

            // فلترة: إذا كان الحرف غير مطبوع أو رمز غريب (0xFF أو > 127)
            if(data == 0xFF || data > 126 || data == '#') {
                Buffer[i] = '\0'; // إنهاء النص هنا فوراً
                // تنظيف الخط من باقي البيانات المهملة
                for(u8 k = i + 1; k < Length - 1; k++) I2C_ReadDataACK();
                I2C_ReadDataNACK();
                I2C_SendStopCondition();
                return 1;
            }
            Buffer[i] = data; // تخزين الحرف الصحيح فقط
        }
        Buffer[Length - 1] = I2C_ReadDataNACK();
        I2C_SendStopCondition();
        return 1;
    }
    return 0;
}


void Colecting_Data(u8 *data,u8 *p)
{
	strcat((char*)data, (char*)p);

}
u16 GetSensorValue(u8 *buffer, u8 *tag)
{
    u16 value = 0;
    // البحث عن الكلمة المفتاحية (Tag) مثل "S1:"
    char *ptr = strstr((char*)buffer, (char*)tag);

    if (ptr != NULL)
    {
        // التحرك بمقدار طول الـ Tag للوصول للرقم مباشرة
        ptr += strlen((char*)tag);
        // تحويل النص إلى رقم والتوقف عند أول حرف غير رقمي مثل ';'
        value = atoi(ptr);
    }
    return value;
}
