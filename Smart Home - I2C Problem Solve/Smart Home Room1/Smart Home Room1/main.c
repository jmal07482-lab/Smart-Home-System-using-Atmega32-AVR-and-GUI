/*
 * Smart Home Room1.c
 *
 * Created: 22/12/2025 07:57:46 م
 * Author : Ahmed Mostafa
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SLAVE_ADDR  0x10
#define Led_Reg     0x31
#define Air_Reg     0x32
#define Temp_Reg    0x33
#define Hum_Reg     0x34

volatile uint8_t Target;
volatile uint8_t Temp = 25;
volatile uint8_t Hum = 13;

void I2C_SlaveInit(uint8_t addr)
{
	TWAR = (addr << 1) | 0x00;
	TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWIE);
}
ISR(TWI_vect)
{
	uint8_t status = TWSR & 0xF8;
	switch (status)
	{
		case 0x60:
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
		break;
		
		case 0x80:
		Target = TWDR;
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
		break;
		
		case 0x88:
		Target = TWDR;
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
		break;
		
		case 0xA8:
		switch (Target)
		{
			case Led_Reg:
			PORTA ^= (1 << PA1);
			TWDR = Led_Reg;
			break;
			
			case Air_Reg:
			PORTA ^= (1 << PA2);
			TWDR = Air_Reg;
			break;
			
			case Temp_Reg:
			TWDR = Temp;
			break;
			
			case Hum_Reg:
			TWDR = Hum;
			break;
			
			default:
			break;
		};
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
		break;
		
		case 0xB8:
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
		break;
		
		case 0xC0:
		case 0xC8:
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
		break;
		
		case 0xA0:
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
		break;
		
		default:
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
		break;
	}
}
int main(void)
{
	I2C_SlaveInit(SLAVE_ADDR);
	sei();
	DDRA |= (1 << PA1);
	DDRA |= (1 << PA2);
	while (1)
	{}
}