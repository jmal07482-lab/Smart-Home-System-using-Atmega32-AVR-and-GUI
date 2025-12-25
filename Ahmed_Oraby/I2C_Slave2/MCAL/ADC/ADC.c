/*
 * ADC.c
 *
 *  Created on:
 *      Author:  Ahmed Oraby Mohamed
 */


#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"

#include "ADC.h"

void (*ADC_PTR_FUN)(void) ;
void ADC_init (u8 ADC_ref)
{
	switch (ADC_ref)
	{
	case ADC_REFERANCE_EXTERNAL :
		CLR_BIT(ADMUX , REFS1) ;
		CLR_BIT(ADMUX , REFS0) ;
		break ;
	case ADC_REFERANCE_AVCC     :
		CLR_BIT(ADMUX , REFS1) ;
		SET_BIT(ADMUX , REFS0) ;
		break ;
	case ADC_REFERANCE_INTERNAL :
		SET_BIT(ADMUX , REFS1) ;
		SET_BIT(ADMUX , REFS0) ;
		break ;
	}

	/*right adjust*/
	CLR_BIT(ADMUX , ADLAR) ;

	/*adc enable*/
	SET_BIT(ADCSRA , ADEN) ;

	/*enable ADC PIE*/
	SET_BIT(ADCSRA , ADIE) ;

	/*prescaller/64*/
	SET_BIT(ADCSRA , ADPS2) ;
	SET_BIT(ADCSRA , ADPS1) ;
	CLR_BIT(ADCSRA , ADPS0) ;

	/*disable auto trig*/
	CLR_BIT(ADCSRA , ADATE) ;
}

void ADC_StartConversion (u8 ChannelNum)
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

void ADC_CallBack (void (*fptr)(void))
{
	ADC_PTR_FUN = fptr ;
}

void __vector_16 (void)__attribute__((signal)) ;
void __vector_16 (void)
{
	if (ADC_PTR_FUN != 0)
	{
		ADC_PTR_FUN () ;
	}
}



