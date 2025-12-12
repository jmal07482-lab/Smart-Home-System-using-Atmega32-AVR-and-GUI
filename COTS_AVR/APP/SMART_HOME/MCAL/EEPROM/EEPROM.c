/*
 * EEPROM.c
 *
 *  Created on: Aug 18, 2025
 *      Author: Mohammed Ramadan
 */

#include "../../LIB/Bit_Math.h"
#include "../../LIB/STDTYPES.h"

#include "EEPROM.h"
void EEPROM_Write (u16 Address , u8 Data)
{
	EEAR = Address ;

	SET_BIT(EECR , EEMWE) ;
	SET_BIT(EECR , EEWE) ;
	EEDR = Data ;

	while (READ_BIT(EECR , EEWE) == 1) ;
}
u8 EEPROM_Read (u16 Address)
{
	EEAR = Address ;
	SET_BIT(EECR , EERE) ;
	return EEDR ;
}
