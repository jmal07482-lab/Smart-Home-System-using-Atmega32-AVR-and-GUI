/*
 * EEPROM.h
 *
 *  Created on: Aug 18, 2025
 *      Author: Mohammed Ramadan
 */

#ifndef MCAL_EEPROM_EEPROM_H_
#define MCAL_EEPROM_EEPROM_H_

#define EEAR    (*(volatile u16*)(0x3E))
#define EEDR    (*(volatile u8*)(0x3D))
#define EECR    (*(volatile u8*)(0x3C))

#define EERIE   3
#define EEMWE   2
#define EEWE    1
#define EERE    0

void EEPROM_Write (u16 Address , u8 Data) ;
u8 EEPROM_Read (u16 Address) ;
#endif /* MCAL_EEPROM_EEPROM_H_ */
