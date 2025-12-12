/*
 * ADC.h
 *
 *  Created on: Aug 19, 2025
 *      Author: Mohammed Ramadan
 */

#ifndef MCAL_ADC_ADC_H_
#define MCAL_ADC_ADC_H_

#define ADMUX    (*(volatile u8*)(0x27))
#define ADCSRA   (*(volatile u8*)(0x26))
#define ADC      (*(volatile u16*)(0x24))
#define SFIOR    (*(volatile u8*)(0x50))
#define SREG     (*(volatile u8*)(0x5F))


#define REFS1   7
#define REFS0   6
#define ADLAR   5
#define MUX4    4
#define MUX3    3
#define MUX2    2
#define MUX1    1
#define MUX0    0

#define ADEN     7
#define ADSC     6
#define ADATE    5
#define ADIF     4
#define ADIE     3
#define ADPS2    2
#define ADPS1    1
#define ADPS0    0

#define ADTS2    7
#define ADTS1    6
#define ADTS0    5

#define I_BIT    7

#define EXTERNAL_AREF  1
#define AVCC_AREF      2
#define INTERNAL_AREF  3

#define ADC_PRESCALLER_2    2
#define ADC_PRESCALLER_4    4
#define ADC_PRESCALLER_8    8
#define ADC_PRESCALLER_16   16
#define ADC_PRESCALLER_32   32
#define ADC_PRESCALLER_64   64
#define ADC_PRESCALLER_128  128

#define ADC_CHANNEL0  0
#define ADC_CHANNEL1  1
#define ADC_CHANNEL2  2
#define ADC_CHANNEL3  3
#define ADC_CHANNEL4  4
#define ADC_CHANNEL5  5
#define ADC_CHANNEL6  6
#define ADC_CHANNEL7  7

/**
 * options 1 for enable , 0 for disable
 */
#define ADC_INTERRUPT  1


void ADC_Init (u8 Prescaller , u8 RefVolt) ;
void ADC_StartConvrsion (u8 ChannelNum) ;
u16 ADC_ReadDataUsingPolling (void) ;
u16 ADC_ReadDataUsingInterrupt (void) ;
void ADC_CallBack (void (*fptr)) ;
#endif /* MCAL_ADC_ADC_H_ */
