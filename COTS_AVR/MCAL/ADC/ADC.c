/*
 * ADC.c
 *
 *  Created on: Aug 19, 2025
 *      Author: Mohammed Ramadan
 */
#include "../../LIB/Bit_Math.h"
#include "../../LIB/STDTYPES.h"

#include "ADC.h"
void (*ADC_ptr)(void) ;
void ADC_Init (u8 Prescaller , u8 RefVolt)
{
	switch (RefVolt)
	{
	case EXTERNAL_AREF :
		CLR_BIT(ADMUX , REFS1) ;
		CLR_BIT(ADMUX , REFS0) ;
		break ;
	case AVCC_AREF     :
		CLR_BIT(ADMUX , REFS1) ;
		SET_BIT(ADMUX , REFS0) ;
		break ;
	case INTERNAL_AREF :
		SET_BIT(ADMUX , REFS1) ;
		SET_BIT(ADMUX , REFS0) ;
		break ;
	}

	switch (Prescaller)
	{
	case ADC_PRESCALLER_2    :
		CLR_BIT(ADCSRA , ADPS2) ;
		CLR_BIT(ADCSRA , ADPS1) ;
		SET_BIT(ADCSRA , ADPS0) ;
		break ;
	case ADC_PRESCALLER_4    :
		CLR_BIT(ADCSRA , ADPS2) ;
		SET_BIT(ADCSRA , ADPS1) ;
		CLR_BIT(ADCSRA , ADPS0) ;
		break ;
	case ADC_PRESCALLER_8    :
		CLR_BIT(ADCSRA , ADPS2) ;
		SET_BIT(ADCSRA , ADPS1) ;
		SET_BIT(ADCSRA , ADPS0) ;
		break ;
	case ADC_PRESCALLER_16   :
		SET_BIT(ADCSRA , ADPS2) ;
		CLR_BIT(ADCSRA , ADPS1) ;
		CLR_BIT(ADCSRA , ADPS0) ;
		break ;
	case ADC_PRESCALLER_32   :
		SET_BIT(ADCSRA , ADPS2) ;
		CLR_BIT(ADCSRA , ADPS1) ;
		SET_BIT(ADCSRA , ADPS0) ;
		break ;
	case ADC_PRESCALLER_64   :
		SET_BIT(ADCSRA , ADPS2) ;
		SET_BIT(ADCSRA , ADPS1) ;
		CLR_BIT(ADCSRA , ADPS0) ;
		break ;
	case ADC_PRESCALLER_128  :
		SET_BIT(ADCSRA , ADPS2) ;
		SET_BIT(ADCSRA , ADPS1) ;
		SET_BIT(ADCSRA , ADPS0) ;
		break ;
	}

	CLR_BIT(ADMUX , ADLAR) ;
#if ADC_INTERRUPT == 1
	SET_BIT(ADCSRA , ADIE) ;
	SET_BIT(SREG , I_BIT)  ;
#endif
	SET_BIT(ADCSRA , ADEN) ;
}
void ADC_StartConvrsion (u8 ChannelNum)
{
	switch (ChannelNum)
	{
	case ADC_CHANNEL0 :
		CLR_BIT(ADMUX , MUX2) ;
		CLR_BIT(ADMUX , MUX1) ;
		CLR_BIT(ADMUX , MUX0) ;
		break ;
	case ADC_CHANNEL1 :
		CLR_BIT(ADMUX , MUX2) ;
		CLR_BIT(ADMUX , MUX1) ;
		SET_BIT(ADMUX , MUX0) ;
		break ;
	case ADC_CHANNEL2 :
		CLR_BIT(ADMUX , MUX2) ;
		SET_BIT(ADMUX , MUX1) ;
		CLR_BIT(ADMUX , MUX0) ;
		break ;
	case ADC_CHANNEL3 :
		CLR_BIT(ADMUX , MUX2) ;
		SET_BIT(ADMUX , MUX1) ;
		SET_BIT(ADMUX , MUX0) ;
		break ;
	case ADC_CHANNEL4 :
		SET_BIT(ADMUX , MUX2) ;
		CLR_BIT(ADMUX , MUX1) ;
		CLR_BIT(ADMUX , MUX0) ;
		break ;
	case ADC_CHANNEL5 :
		SET_BIT(ADMUX , MUX2) ;
		CLR_BIT(ADMUX , MUX1) ;
		SET_BIT(ADMUX , MUX0) ;
		break ;
	case ADC_CHANNEL6 :
		SET_BIT(ADMUX , MUX2) ;
		SET_BIT(ADMUX , MUX1) ;
		CLR_BIT(ADMUX , MUX0) ;
		break ;
	case ADC_CHANNEL7 :
		SET_BIT(ADMUX , MUX2) ;
		SET_BIT(ADMUX , MUX1) ;
		SET_BIT(ADMUX , MUX0) ;
		break ;
	}

	SET_BIT(ADCSRA , ADSC) ;
}

u16 ADC_ReadDataUsingPolling (void)
{
	while (READ_BIT(ADCSRA , ADIF) == 0) ;
	SET_BIT(ADCSRA , ADIF) ;
	return ADC ;
}
u16 ADC_ReadDataUsingInterrupt (void)
{
	return ADC ;
}

void ADC_CallBack (void (*fptr))
{
	ADC_ptr = fptr ;
}
void __vector_16 (void)__attribute__((signal)) ;
void __vector_16 (void)
{
	if (ADC_ptr != 0)
	{
		ADC_ptr () ;
	}
}
