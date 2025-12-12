/*
 * Timer1.h
 *
 *  Created on: Aug 20, 2025
 *      Author: Mohammed Ramadan
 */

#ifndef MCAL_TIMER1_TIMER1_H_
#define MCAL_TIMER1_TIMER1_H_

#define TCCR1A    (*(volatile u8*)(0x4F))
#define TCCR1B    (*(volatile u8*)(0x4E))
#define TCNT1     (*(volatile u16*)(0x4C))
#define OCR1A     (*(volatile u16*)(0x4A))
#define OCR1B     (*(volatile u16*)(0x48))
#define ICR1      (*(volatile u16*)(0x46))
#define TIMSK     (*(volatile u8*)(0x59))
#define TIFR      (*(volatile u8*)(0x58))
#define SREG      (*(volatile u8*)(0x5F))


#define COM1A1    7
#define COM1A0    6
#define COM1B1    5
#define COM1B0    4
#define FOC1A     3
#define FOC1B     2
#define WGM11     1
#define WGM10     0

#define ICNC1     7
#define ICES1     6
#define WGM13     4
#define WGM12     3
#define CS12      2
#define CS11      1
#define CS10      0

#define TICIE1    5
#define OCIE1A    4
#define OCIE1B    3
#define TOIE1     2

#define ICF1      5
#define OCF1A     4
#define OCF1B     3
#define TOV1      2

#define I_BIT     7


#define TIMER1_OVF                       1
#define TIMER1_CTC_TOP_OCR1A             2
#define TIMER1_CTC_TOP_ICR1              3
#define TIMER1_FAST_PWM_8BIT             4
#define TIMER1_FAST_PWM_9BIT             5
#define TIMER1_FAST_PWM_10BIT            6
#define TIMER1_PHASE_CORRECT_8BIT        7
#define TIMER1_PHASE_CORRECT_9BIT        8
#define TIMER1_PHASE_CORRECT_10BIT       9
#define TIMER1_PHASE_CORRECT_TOP_OCR1A   10
#define TIMER1_PHASE_CORRECT_TOP_ICR1    11
#define TIMER1_FREQ_CORRECT_TOP_OCR1A    12
#define TIMER1_FREQ_CORRECT_TOP_ICR1     13
#define TIMER1_FAST_PWM_TOP_OCR1A        14
#define TIMER1_FAST_PWM_TOP_ICR1         15

#define TIMER1_PRESCALLER_1     1
#define TIMER1_PRESCALLER_8     8
#define TIMER1_PRESCALLER_64    64
#define TIMER1_PRESCALLER_256   256
#define TIMER1_PRESCALLER_1024  1024

void Timer1_Init           (u32 Prescaller , u8 Mode) ;
void Timer1_SetDesiredTime (u32 TimeInMicro , void (*fptr)(void)) ;
void Timer1_GeneratePWMA    (u32 CompareVal) ;
void Timer1_GeneratePWMB    (u32 CompareVal) ;
u32 Timer1_CaptureTonCounts (void) ;
u32 Timer1_CaptureToffCounts (void) ;
#endif /* MCAL_TIMER1_TIMER1_H_ */
