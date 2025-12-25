#include "../LIB/STD_TYPES.h"
#include "../MCAL/DIO/DIO.h"
#include "../MCAL/ADC/ADC.h"

#include "../MCAL/I2C/I2C_Slave.h"
#include "../HAL/LED/LED.h"
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

// بفر لاستقبال الأوامر من الماستر
char receivedBuffer[32];
// بفر لإرسال درجة الحرارة للماستر
char slaveDataBuffer[16];
// علم الجاهزية لإرسال البيانات
u8 isDataReady = SLAVE_BUSY_BYTE;
u16 Current_temp=0;       //Current temperature

void main(void) {
    // 1. تهيئة السليف بالعنوان 0x20
    I2C_SlaveInit(0x20);
	ADC_init(ADC_REFERANCE_INTERNAL);

    // 2. تهيئة الليدات والديودات ابتداءً من PIN 1
    LED_Init(DIO_PORTA, DIO_PIN1); // AR
    LED_Init(DIO_PORTA, DIO_PIN2); // L1
    LED_Init(DIO_PORTA, DIO_PIN3); // D1

    while(1) {

        // --- الجزء الأول: تحضير وإرسال درجة الحرارة ---
    	ADC_StartConversion(ADC_CHANNEL0) ;
    	Current_temp = ADC_ReadDataUsingPolling()*0.25 ;
    	isDataReady = SLAVE_BUSY_BYTE;

        I2C_ClearBuffer(slaveDataBuffer, 16);

        // تعديل: إرسال الرقم فقط. الـ sprintf تضع \0 تلقائياً في النهاية
        sprintf(slaveDataBuffer, "T1:%d", Current_temp);

        isDataReady = SLAVE_READY_BYTE;
        I2C_SlaveSendString_Smart(slaveDataBuffer, isDataReady);
        // --- الجزء الثاني: استقبال مصفوفة الأوامر وتنفيذها ---

        // السليف ينتظر هنا حتى يقوم الماستر بإرسال مصفوفة التحكم
        I2C_SlaveReceiveString_Smart(receivedBuffer, 32);

        // تحليل النص المستلم (Parsing) والتحكم في المخارج
        if (strstr(receivedBuffer, "AR:ON") != NULL)       LED_On(DIO_PORTA, DIO_PIN1);
        else if (strstr(receivedBuffer, "AR:OFF") != NULL) LED_Off(DIO_PORTA, DIO_PIN1);

        if (strstr(receivedBuffer, "L1:ON") != NULL)       LED_On(DIO_PORTA, DIO_PIN2);
        else if (strstr(receivedBuffer, "L1:OFF") != NULL) LED_Off(DIO_PORTA, DIO_PIN2);

        if (strstr(receivedBuffer, "D1:ON") != NULL)       LED_On(DIO_PORTA, DIO_PIN3);
        else if (strstr(receivedBuffer, "D1:OFF") != NULL) LED_Off(DIO_PORTA, DIO_PIN3);


        _delay_ms(500);
    }
}
