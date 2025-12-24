#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>


// ================= ADC INIT =================
void ADC_Init(void)
{
	ADMUX = (1 << REFS0);   // AVcc reference, ADC0

	ADCSRA = (1 << ADEN)   |  // Enable ADC
	(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler = 128
}

// ================= ADC READ FROM PA0 =================
uint16_t ADC_Read_PA0(void)
{
	ADMUX = (ADMUX & 0xF8);    // ?????? ?????? ADC0 (PA0)

	ADCSRA |= (1 << ADSC);    // ??? ???????
	while (ADCSRA & (1 << ADSC)); // ?????? ????????

	return ADC;               // ????? 10-bit
}

#define  LCD_DATA PORTC
#define rs PD0
#define rw PD1
#define en PD2

void LCD_init(void);
void LCD_cmd(unsigned char);
void LCD_write(unsigned char);
void LCD_String(char *str);
void lcd_argument(void)
{
	LCD_String("light : ");
	LCD_cmd(0x0E);
}
// ================= MAIN =================
int main(void)
{
	DDRC =0xFF;
	DDRD =0xFF;
	
	LCD_init();
	_delay_ms(1);
	LCD_cmd(0x0C);
	_delay_ms(1);
	lcd_argument();
	
	DDRB =0b00000011;
	PORTB=0x00;
	uint16_t adc_value;

	ADC_Init();

	DDRA =0b00000000;  // ????? ?? PA0 Input

	while (1)
	{
		
		adc_value = ADC_Read_PA0(); 
		unsigned char arr[10];
        sprintf(arr, "%u", adc_value);
		LCD_cmd(0x80 + 8);
		LCD_String(arr);
		
		if(adc_value < 400)
		{
			PORTB =0b00000001;
		}
		else
		{
			PORTB =0b00000010;
		}
		_delay_ms(100);
	}
}

void LCD_init(void)
{
	LCD_cmd(0x38);
	_delay_ms(1);
	LCD_cmd(0x01);
	_delay_ms(1);
	LCD_cmd(0x02);
	_delay_ms(1);
	LCD_cmd(0x06);
	_delay_ms(1);
	LCD_cmd(0x80);
	_delay_ms(1);
}


void LCD_cmd(unsigned char cmd)
{
	LCD_DATA = cmd;
	PORTD &= ~(1<<rs);
	PORTD &= ~(1<<rw);
	PORTD |= (1<<en);
	_delay_ms(2);
	PORTD &= ~(1<<en);
}


void LCD_write(unsigned char data)
{
	LCD_DATA = data;
	PORTD |= (1<<rs);
	PORTD &= ~(1<<rw);
	PORTD |= (1<<en);
	_delay_ms(2);
	PORTD &= ~(1<<en);
}
void LCD_String(char *str)
{
	while (*str)
	LCD_write(*str++);
}

