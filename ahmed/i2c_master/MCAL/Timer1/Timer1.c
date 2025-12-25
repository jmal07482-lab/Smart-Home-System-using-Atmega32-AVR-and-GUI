/*
 * Timer1.c
 *
 *  Created on: Aug 20, 2025
 *      Author: Mohammed Ramadan
 */

#include "../../LIB/Bit_Math.h"
#include "../../LIB/STD_TYPES.h"

#include "../DIO/DIO.h"

#include "Timer1.h"
u32 G_Prescaller ;
u32 G_Counts = 0 ;
void (*Timer1_ptr)(void) ;
void Timer1_Init           (u32 Prescaller , u8 Mode)
{
	G_Prescaller = Prescaller ;
	switch (Prescaller)
	{
	case TIMER1_PRESCALLER_1    :
		CLR_BIT(TCCR1B , CS12) ;
		CLR_BIT(TCCR1B , CS11) ;
		SET_BIT(TCCR1B , CS10) ;
		break ;
	case TIMER1_PRESCALLER_8    :
		CLR_BIT(TCCR1B , CS12) ;
		SET_BIT(TCCR1B , CS11) ;
		CLR_BIT(TCCR1B , CS10) ;
		break ;
	case TIMER1_PRESCALLER_64   :
		CLR_BIT(TCCR1B , CS12) ;
		SET_BIT(TCCR1B , CS11) ;
		SET_BIT(TCCR1B , CS10) ;
		break ;
	case TIMER1_PRESCALLER_256  :
		SET_BIT(TCCR1B , CS12) ;
		CLR_BIT(TCCR1B , CS11) ;
		CLR_BIT(TCCR1B , CS10) ;
		break ;
	case TIMER1_PRESCALLER_1024 :
		SET_BIT(TCCR1B , CS12) ;
		CLR_BIT(TCCR1B , CS11) ;
		SET_BIT(TCCR1B , CS10) ;
		break ;
	}

	switch (Mode)
	{
	case TIMER1_OVF                     :
		CLR_BIT(TCCR1B , WGM13) ;
		CLR_BIT(TCCR1B , WGM12) ;
		CLR_BIT(TCCR1A , WGM11) ;
		CLR_BIT(TCCR1A , WGM10) ;

		SET_BIT(TIMSK , TOIE1) ;
		break ;
	case TIMER1_CTC_TOP_OCR1A           :
		CLR_BIT(TCCR1B , WGM13) ;
		SET_BIT(TCCR1B , WGM12) ;
		CLR_BIT(TCCR1A , WGM11) ;
		CLR_BIT(TCCR1A , WGM10) ;

		SET_BIT(TIMSK , OCIE1B) ;
		SET_BIT(TIMSK , OCIE1A) ;
		break ;
	case TIMER1_CTC_TOP_ICR1            :
		SET_BIT(TCCR1B , WGM13) ;
		SET_BIT(TCCR1B , WGM12) ;
		CLR_BIT(TCCR1A , WGM11) ;
		CLR_BIT(TCCR1A , WGM10) ;

		SET_BIT(TIMSK , OCIE1B) ;
		SET_BIT(TIMSK , OCIE1A) ;
		break ;
	case TIMER1_FAST_PWM_8BIT           :
		CLR_BIT(TCCR1B , WGM13) ;
		SET_BIT(TCCR1B , WGM12) ;
		CLR_BIT(TCCR1A , WGM11) ;
		SET_BIT(TCCR1A , WGM10) ;

		SET_BIT(TCCR1A , COM1A1) ;
		CLR_BIT(TCCR1A , COM1A0) ;
		SET_BIT(TCCR1A , COM1B1) ;
		CLR_BIT(TCCR1A , COM1B0) ;

		DIO_InitPin(DIO_PORTD, DIO_PIN4, DIO_OUTPUT) ;
		DIO_InitPin(DIO_PORTD, DIO_PIN5, DIO_OUTPUT) ;
		break ;
	case TIMER1_FAST_PWM_9BIT           :
		CLR_BIT(TCCR1B , WGM13) ;
		SET_BIT(TCCR1B , WGM12) ;
		SET_BIT(TCCR1A , WGM11) ;
		CLR_BIT(TCCR1A , WGM10) ;

		SET_BIT(TCCR1A , COM1A1) ;
		CLR_BIT(TCCR1A , COM1A0) ;
		SET_BIT(TCCR1A , COM1B1) ;
		CLR_BIT(TCCR1A , COM1B0) ;

		DIO_InitPin(DIO_PORTD, DIO_PIN4, DIO_OUTPUT) ;
		DIO_InitPin(DIO_PORTD, DIO_PIN5, DIO_OUTPUT) ;
		break ;
	case TIMER1_FAST_PWM_10BIT          :
		CLR_BIT(TCCR1B , WGM13) ;
		SET_BIT(TCCR1B , WGM12) ;
		SET_BIT(TCCR1A , WGM11) ;
		SET_BIT(TCCR1A , WGM10) ;

		SET_BIT(TCCR1A , COM1A1) ;
		CLR_BIT(TCCR1A , COM1A0) ;
		SET_BIT(TCCR1A , COM1B1) ;
		CLR_BIT(TCCR1A , COM1B0) ;

		DIO_InitPin(DIO_PORTD, DIO_PIN4, DIO_OUTPUT) ;
		DIO_InitPin(DIO_PORTD, DIO_PIN5, DIO_OUTPUT) ;
		break ;
	case TIMER1_PHASE_CORRECT_8BIT      :
		CLR_BIT(TCCR1B , WGM13) ;
		CLR_BIT(TCCR1B , WGM12) ;
		CLR_BIT(TCCR1A , WGM11) ;
		SET_BIT(TCCR1A , WGM10) ;

		SET_BIT(TCCR1A , COM1A1) ;
		CLR_BIT(TCCR1A , COM1A0) ;
		SET_BIT(TCCR1A , COM1B1) ;
		CLR_BIT(TCCR1A , COM1B0) ;

		DIO_InitPin(DIO_PORTD, DIO_PIN4, DIO_OUTPUT) ;
		DIO_InitPin(DIO_PORTD, DIO_PIN5, DIO_OUTPUT) ;
		break ;
	case TIMER1_PHASE_CORRECT_9BIT      :
		CLR_BIT(TCCR1B , WGM13) ;
		CLR_BIT(TCCR1B , WGM12) ;
		SET_BIT(TCCR1A , WGM11) ;
		CLR_BIT(TCCR1A , WGM10) ;

		SET_BIT(TCCR1A , COM1A1) ;
		CLR_BIT(TCCR1A , COM1A0) ;
		SET_BIT(TCCR1A , COM1B1) ;
		CLR_BIT(TCCR1A , COM1B0) ;

		DIO_InitPin(DIO_PORTD, DIO_PIN4, DIO_OUTPUT) ;
		DIO_InitPin(DIO_PORTD, DIO_PIN5, DIO_OUTPUT) ;
		break ;
	case TIMER1_PHASE_CORRECT_10BIT     :
		CLR_BIT(TCCR1B , WGM13) ;
		CLR_BIT(TCCR1B , WGM12) ;
		SET_BIT(TCCR1A , WGM11) ;
		SET_BIT(TCCR1A , WGM10) ;

		SET_BIT(TCCR1A , COM1A1) ;
		CLR_BIT(TCCR1A , COM1A0) ;
		SET_BIT(TCCR1A , COM1B1) ;
		CLR_BIT(TCCR1A , COM1B0) ;

		DIO_InitPin(DIO_PORTD, DIO_PIN4, DIO_OUTPUT) ;
		DIO_InitPin(DIO_PORTD, DIO_PIN5, DIO_OUTPUT) ;
		break ;
	case TIMER1_PHASE_CORRECT_TOP_OCR1A :
		SET_BIT(TCCR1B , WGM13) ;
		CLR_BIT(TCCR1B , WGM12) ;
		SET_BIT(TCCR1A , WGM11) ;
		SET_BIT(TCCR1A , WGM10) ;

		SET_BIT(TCCR1A , COM1A1) ;
		CLR_BIT(TCCR1A , COM1A0) ;
		SET_BIT(TCCR1A , COM1B1) ;
		CLR_BIT(TCCR1A , COM1B0) ;

		DIO_InitPin(DIO_PORTD, DIO_PIN4, DIO_OUTPUT) ;
		DIO_InitPin(DIO_PORTD, DIO_PIN5, DIO_OUTPUT) ;
		break ;
	case TIMER1_PHASE_CORRECT_TOP_ICR1  :
		SET_BIT(TCCR1B , WGM13) ;
		CLR_BIT(TCCR1B , WGM12) ;
		SET_BIT(TCCR1A , WGM11) ;
		CLR_BIT(TCCR1A , WGM10) ;

		SET_BIT(TCCR1A , COM1A1) ;
		CLR_BIT(TCCR1A , COM1A0) ;
		SET_BIT(TCCR1A , COM1B1) ;
		CLR_BIT(TCCR1A , COM1B0) ;

		DIO_InitPin(DIO_PORTD, DIO_PIN4, DIO_OUTPUT) ;
		DIO_InitPin(DIO_PORTD, DIO_PIN5, DIO_OUTPUT) ;
		break ;
	case TIMER1_FREQ_CORRECT_TOP_OCR1A  :
		SET_BIT(TCCR1B , WGM13) ;
		CLR_BIT(TCCR1B , WGM12) ;
		CLR_BIT(TCCR1A , WGM11) ;
		SET_BIT(TCCR1A , WGM10) ;

		SET_BIT(TCCR1A , COM1A1) ;
		CLR_BIT(TCCR1A , COM1A0) ;
		SET_BIT(TCCR1A , COM1B1) ;
		CLR_BIT(TCCR1A , COM1B0) ;

		DIO_InitPin(DIO_PORTD, DIO_PIN4, DIO_OUTPUT) ;
		DIO_InitPin(DIO_PORTD, DIO_PIN5, DIO_OUTPUT) ;
		break ;
	case TIMER1_FREQ_CORRECT_TOP_ICR1   :
		SET_BIT(TCCR1B , WGM13) ;
		CLR_BIT(TCCR1B , WGM12) ;
		CLR_BIT(TCCR1A , WGM11) ;
		CLR_BIT(TCCR1A , WGM10) ;

		SET_BIT(TCCR1A , COM1A1) ;
		CLR_BIT(TCCR1A , COM1A0) ;
		SET_BIT(TCCR1A , COM1B1) ;
		CLR_BIT(TCCR1A , COM1B0) ;

		DIO_InitPin(DIO_PORTD, DIO_PIN4, DIO_OUTPUT) ;
		DIO_InitPin(DIO_PORTD, DIO_PIN5, DIO_OUTPUT) ;
		break ;
	case TIMER1_FAST_PWM_TOP_OCR1A      :
		SET_BIT(TCCR1B , WGM13) ;
		SET_BIT(TCCR1B , WGM12) ;
		SET_BIT(TCCR1A , WGM11) ;
		SET_BIT(TCCR1A , WGM10) ;

		SET_BIT(TCCR1A , COM1A1) ;
		CLR_BIT(TCCR1A , COM1A0) ;
		SET_BIT(TCCR1A , COM1B1) ;
		CLR_BIT(TCCR1A , COM1B0) ;

		DIO_InitPin(DIO_PORTD, DIO_PIN4, DIO_OUTPUT) ;
		DIO_InitPin(DIO_PORTD, DIO_PIN5, DIO_OUTPUT) ;
		break ;
	case TIMER1_FAST_PWM_TOP_ICR1       :
		SET_BIT(TCCR1B , WGM13) ;
		SET_BIT(TCCR1B , WGM12) ;
		SET_BIT(TCCR1A , WGM11) ;
		CLR_BIT(TCCR1A , WGM10) ;

		SET_BIT(TCCR1A , COM1A1) ;
		CLR_BIT(TCCR1A , COM1A0) ;
		SET_BIT(TCCR1A , COM1B1) ;
		CLR_BIT(TCCR1A , COM1B0) ;

		DIO_InitPin(DIO_PORTD, DIO_PIN4, DIO_OUTPUT) ;
		DIO_InitPin(DIO_PORTD, DIO_PIN5, DIO_OUTPUT) ;
		break ;
	}
	SET_BIT(SREG , I_BIT) ;
}
void Timer1_SetDesiredTime (u32 TimeInMicro , void (*fptr)(void))
{
	f32 OVF_Time = 8192*G_Prescaller ;
	f32 Desires_OVF_Counts = TimeInMicro/OVF_Time ;
	u32 Preload ;
	Timer1_ptr = fptr ;
	if (Desires_OVF_Counts < 1.0)
	{
		G_Counts = 1 ;
		Preload = 65536-(65536*Desires_OVF_Counts);
		TCNT1 = Preload ;
	}
	else if (Desires_OVF_Counts == 1.0)
	{
		G_Counts = 1 ;
	}
	else if (Desires_OVF_Counts > 1.0)
	{
		G_Counts = Desires_OVF_Counts ;
		if ((Desires_OVF_Counts - (u32)Desires_OVF_Counts) !=0.0)
		{
			Preload = 65536-(65536*(Desires_OVF_Counts - (u32)Desires_OVF_Counts));
			TCNT1 = Preload ;
			G_Counts++;
		}
	}
}
void Timer1_GeneratePWMA    (u32 CompareVal)
{
	OCR1A =  CompareVal;
}

void Timer1_GeneratePWMB    (u32 CompareVal)
{
	OCR1B =  CompareVal;
}
void __vector_9 (void)__attribute__((signal)) ;
void __vector_9 (void)
{
	static u32 Counter = 0 ;
	Counter++ ;
	if (Counter == G_Counts)
	{
		if (Timer1_ptr != 0)
		{
			Timer1_ptr () ;
			Counter = 0 ;
		}
	}
}

u32 Timer1_CaptureTonCounts (void)
{
	u32 CountsForFirstRising ;
	u32 CountsForFirstFailing ;
	u32 Counts ;
	//cfg ICU to detect first Rising
	SET_BIT(TCCR1B , ICNC1) ;
	SET_BIT(TCCR1B , ICES1) ;
	while (READ_BIT(TIFR , ICF1) == 0) ;
	SET_BIT(TIFR , ICF1) ;
	CountsForFirstRising = ICR1 ;

	//cfg ICU to detect first Failing
	SET_BIT(TCCR1B , ICNC1) ;
	CLR_BIT(TCCR1B , ICES1) ;
	while (READ_BIT(TIFR , ICF1) == 0) ;
	SET_BIT(TIFR , ICF1) ;
	CountsForFirstFailing = ICR1 ;

	Counts = CountsForFirstFailing-CountsForFirstRising ;
	return Counts ;
}
u32 Timer1_CaptureToffCounts (void)
{
	u32 CountsForFirstRising ;
	u32 CountsForFirstFailing ;
	u32 Counts ;
	//cfg ICU to detect first Failing
	SET_BIT(TCCR1B , ICNC1) ;
	SET_BIT(TCCR1B , ICES1) ;
	while (READ_BIT(TIFR , ICF1) == 0) ;
	SET_BIT(TIFR , ICF1) ;
	CountsForFirstFailing = ICR1 ;

	//cfg ICU to detect first Rising
	SET_BIT(TCCR1B , ICNC1) ;
	CLR_BIT(TCCR1B , ICES1) ;
	while (READ_BIT(TIFR , ICF1) == 0) ;
	SET_BIT(TIFR , ICF1) ;
	CountsForFirstRising = ICR1 ;

	Counts = CountsForFirstRising-CountsForFirstFailing ;
	return Counts ;
}
