#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/DIO/DIO.h"
#include "../MCAL/I2C/I2C.h"
#include"../MCAL/Timer1/Timer1.h"
#include "../HAL/LCD/LCD.h"
#include"../HAL/PB/PB.h"
#include "../HAL/LED/LED.h"
#include "../HAL/Servo/Servo.h"
#include <util/delay.h>
#include <string.h>




#define SLAVE1_ADDR  0x20
#define SLAVE2_ADDR  0x30
u8 finalData[50];
u8 temp_Buffer[17];
u8 MsgToSlave[25] ;
void main(void) {
	u16 s1_val=0;
	// تعطيل JTAG وتهيئة LCD و I2C
	(*(volatile u8*)(0x54)) |= (1<<7); (*(volatile u8*)(0x54)) |= (1<<7);
	LCD_Init();
	I2C_MasterInit(100000);
	LED_Init(DIO_PORTA,DIO_PIN5);
	PB_Init(DIO_PORTA,DIO_PIN1);
	Servo_Init();

	while(1) {
           //GAMAL CODE
		if(PB_Readstatues(DIO_PORTA,DIO_PIN1)==1)
		{
			LED_ON(DIO_PORTA,DIO_PIN5);
			Servo_SetAngle(90);
		}
		if(PB_Readstatues(DIO_PORTA,DIO_PIN2)==1 &&isledon(DIO_PORTA,DIO_PIN5)==1)
		{
			Servo_SetAngle(0);
			LED_OFF(DIO_PORTA,DIO_PIN5);
		}

	}



	I2C_ClearBuffer(finalData, 50);
	// --- التعامل مع السليف الأول (0x20) ---

			I2C_ClearBuffer(temp_Buffer, 17);

	if (I2C_ReceiveString_Smart(SLAVE1_ADDR, temp_Buffer, 16)) {
		Colecting_Data((u8*)finalData, (u8*)temp_Buffer);

		//LCD_WriteCmd(0x01);
		_delay_ms(2);
		LCD_Move_Cursor(0,0);
		LCD_WriteString((u8*)temp_Buffer);

		s1_val = GetSensorValue((u8*)finalData, (u8*)"T1:");

		if (s1_val <= 20)
		{
			sprintf(MsgToSlave, "AR:OFF,L1:ON,D1:OFF#");
			I2C_SendString(SLAVE1_ADDR, (char*)MsgToSlave);
		}
		else if (s1_val > 20 && s1_val < 50)
		{
			sprintf(MsgToSlave, "AR:ON,L1:ON,D1:OFF#");
			I2C_SendString(SLAVE1_ADDR, (char*)MsgToSlave);
		}
		else if (s1_val >= 50)
		{
			sprintf(MsgToSlave, "AR:OFF,L1:ON,D1:ON#");
			I2C_SendString(SLAVE1_ADDR, (char*)MsgToSlave);
		}
		_delay_ms(500);
	}

	// --- التعامل مع السليف الثاني (0x30) ---

	I2C_ClearBuffer(temp_Buffer, 17);

	if (I2C_ReceiveString_Smart(SLAVE2_ADDR, temp_Buffer, 16)) {
		Colecting_Data((u8*)finalData, (u8*)temp_Buffer);

		//	LCD_WriteCmd(0x01);
		_delay_ms(2);
		LCD_Move_Cursor(1,0);
		LCD_WriteString((u8*)temp_Buffer);

		s1_val = GetSensorValue((u8*)finalData, (u8*)"T2:");

		if (s1_val <= 20)
		{
			sprintf(MsgToSlave, "AR:OFF,L1:ON,D1:OFF#");
			I2C_SendString(SLAVE2_ADDR, (char*)MsgToSlave);
		}
		else if (s1_val > 20 && s1_val < 50)
		{
			sprintf(MsgToSlave, "AR:ON,L1:ON,D1:OFF#");
			I2C_SendString(SLAVE2_ADDR, (char*)MsgToSlave);
		}
		else if (s1_val >=50)
		{
			sprintf(MsgToSlave, "AR:OFF,L1:ON,D1:ON#");
			I2C_SendString(SLAVE2_ADDR, (char*)MsgToSlave);
		}
		_delay_ms(500);
	}
	_delay_ms(1000); // انتظار قبل بدء الدورة الجديدة
}



//// تعريف مسجل MCUCSR لتعطيل الـ JTAG لضمان عمل PORTB
//#define MCUCSR_REG    (*(volatile u8*)(0x54))
//
//// بفر لتخزين درجة الحرارة المستلمة
//char receivedTemp[17];
//
//// مصفوفة الأوامر التي سيتم إرسالها للسليف
//u8 ControlMsg[] = "L1:ON,L2:OFF,D1:ON,D2:ON#";
//
//void main(void) {
//    /* 1. تعطيل الـ JTAG لكي يعمل PORTB بشكل صحيح مع الـ LCD */
//    MCUCSR_REG |= (1<<7);
//    MCUCSR_REG |= (1<<7);
//
//    /* 2. تهيئة الشاشة */
//    LCD_Init();
//
//    /* 3. تهيئة الـ I2C كـ Master بسرعة 100kHz */
//    I2C_MasterInit(100000);
//
//    // رسالة ترحيبية بسيطة
//    LCD_WriteCmd(0x01);
//    _delay_ms(2);
//    LCD_WriteString((u8*)"Master System");
//    _delay_ms(1000);
//
//    while(1) {
//        // --- الخطوة الأولى: قراءة درجة الحرارة من السليف ---
//        // العنوان المتفق عليه 0x20
//        if (I2C_ReceiveString_Smart(0x20, receivedTemp, 16)) {
//
//            // مسح الشاشة لعرض الحرارة الجديدة
//            LCD_WriteCmd(0x01);
//            _delay_ms(2);
//
//            LCD_WriteCmd(0x80); // السطر الأول
//            LCD_WriteString((u8*)"Temp: ");
//            LCD_WriteString((u8*)receivedTemp);
//        } else {
//            // في حالة فشل الاتصال بالسليف
//            LCD_WriteCmd(0x01); _delay_ms(2);
//            LCD_WriteString((u8*)"Slave Busy...");
//        }
//
//        _delay_ms(1000); // انتظار بسيط قبل إرسال الأوامر
//        if (strstr(receivedTemp, "T:20") != NULL)
//        {
//            sprintf(ControlMsg, "L1:OFF,L2:OFF,D1:OFF,D2:OFF#");
//            I2C_SendString(0x20, (char*)ControlMsg);
//        }
//        else if (strstr(receivedTemp, "T:25") != NULL)
//        {
//               sprintf(ControlMsg, "L1:ON,L2:OFF,D1:ON,D2:OFF#");
//               I2C_SendString(0x20, (char*)ControlMsg);
//        }
//        else
//        {
//        	sprintf(ControlMsg, "L1:ON,L2:ON,D1:ON,D2:ON#");
//        	I2C_SendString(0x20, (char*)ControlMsg);
//        }
//        _delay_ms(2000);
//    }
//}
