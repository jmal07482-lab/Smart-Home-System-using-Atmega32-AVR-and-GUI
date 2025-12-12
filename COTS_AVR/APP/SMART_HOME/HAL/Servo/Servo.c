

#include "../../LIB/Bit_Math.h"
#include "../../LIB/STDTYPES.h"

#include "../../MCAL/Timer1/Timer1.h"

#include "Servo.h"

void Servo_Init (void)
{
	Timer1_Init(TIMER1_PRESCALLER_8, TIMER1_FAST_PWM_TOP_ICR1) ;
}
void Servo_SetAngle (s8 Angle)
{
	f32 TonInMill = 1.5 + Angle/180.0;
	ICR1 = 19999 ;
	Timer1_GeneratePWMA (TonInMill*1000) ;
}
