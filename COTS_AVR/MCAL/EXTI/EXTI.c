/*
 * EXTI.c
 *
 *  Created on: Aug 14, 2025
 *      Author: Mohammed Ramadan
 */

#include "../../LIB/Bit_Math.h"
#include "../../LIB/STDTYPES.h"

#include "EXTI.h"
void (*ptr_EXTI0)(void) ;
void (*ptr_EXTI1)(void) ;
void (*ptr_EXTI2)(void) ;
void EXTI0_Init (u8 Sense)
{
	switch (Sense)
	{
	case FALING_EDGE_SENSE :
		SET_BIT(MCUCR , ISC01) ;
		CLR_BIT(MCUCR , ISC00) ;
		break ;
	case RISING_EDGE_SENSE :
		SET_BIT(MCUCR , ISC01) ;
		SET_BIT(MCUCR , ISC00) ;
		break ;
	case ON_CHANGE_SENSE   :
		CLR_BIT(MCUCR , ISC01) ;
		SET_BIT(MCUCR , ISC00) ;
		break ;
	case LOW_LEVEL_SENSE   :
		CLR_BIT(MCUCR , ISC01) ;
		CLR_BIT(MCUCR , ISC00) ;
		break ;
	}
	SET_BIT(SREG , I_BIT) ;
	SET_BIT(GICR , INT0) ;
}
void EXTI1_Init (u8 Sense)
{
	switch (Sense)
	{
	case FALING_EDGE_SENSE :
		SET_BIT(MCUCR , ISC11) ;
		CLR_BIT(MCUCR , ISC10) ;
		break ;
	case RISING_EDGE_SENSE :
		SET_BIT(MCUCR , ISC11) ;
		SET_BIT(MCUCR , ISC10) ;
		break ;
	case ON_CHANGE_SENSE   :
		CLR_BIT(MCUCR , ISC11) ;
		SET_BIT(MCUCR , ISC10) ;
		break ;
	case LOW_LEVEL_SENSE   :
		CLR_BIT(MCUCR , ISC11) ;
		CLR_BIT(MCUCR , ISC10) ;
		break ;
	}
	SET_BIT(SREG , I_BIT) ;
	SET_BIT(GICR , INT1) ;
}
void EXTI2_Init (u8 Sense)
{
	switch (Sense)
	{
	case FALING_EDGE_SENSE :
		CLR_BIT(MCUCR , ISC2) ;
		break ;
	case RISING_EDGE_SENSE :
		SET_BIT(MCUCR , ISC2) ;
		break ;
	}
	SET_BIT(SREG , I_BIT) ;
	SET_BIT(GICR , INT2) ;
}
void EXTI0_CallBack (void (*fptr)(void))
{
	ptr_EXTI0 = fptr ;
}
void EXTI1_CallBack (void (*fptr)(void))
{
	ptr_EXTI1 = fptr ;
}
void EXTI2_CallBack (void (*fptr)(void))
{
	ptr_EXTI2 = fptr ;
}
void __vector_1 (void)__attribute__((signal)) ;
void __vector_1 (void)
{
	if (ptr_EXTI0 != 0)
	{
		ptr_EXTI0 () ;
	}
}
void __vector_2 (void)__attribute__((signal)) ;
void __vector_2 (void)
{
	if (ptr_EXTI1 != 0)
	{
		ptr_EXTI1 () ;
	}
}
void __vector_3 (void)__attribute__((signal)) ;
void __vector_3 (void)
{
	if (ptr_EXTI2 != 0)
	{
		ptr_EXTI2 () ;
	}
}
