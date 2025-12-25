/*
 * Smart Home Master.c
 *
 * Created: 22/12/2025 07:56:08 م
 * Author : Ahmed Mostafa
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define Led_Reg     0x31
#define Air_Reg     0x32
#define Temp_Reg    0x33
#define Hum_Reg     0x34
#define Room_COUNT  2
#define BAUDRATE    9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

volatile uint8_t Set_addr;
volatile uint8_t Set_reg;

volatile uint8_t Trg_reg;
volatile uint8_t Trg_addr;

volatile uint8_t current_room = 0;
volatile uint8_t room_index = 0;
const uint8_t room_addr[Room_COUNT] = {0x10, 0x11};
	
volatile uint8_t Temp_value[Room_COUNT] = {0};
volatile uint8_t Hum_value[Room_COUNT] = {0};


volatile uint8_t i2c_busy = 0;
volatile uint8_t i2c_state = 0;


bool IsConnect = false;
bool IsSet = false;
char rx_buffer[100];
uint8_t rx_index = 0;
char Password[] = "123";


void I2C_Init(void)
{
	TWSR = 0x00;
	TWBR = 32;
	TWCR = (1 << TWEN);
}
void I2C_StartRequest(void)
{
	if (i2c_busy) return;	
	i2c_busy = 1;
	i2c_state = 1;
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}
ISR(TWI_vect)
{
	uint8_t status = TWSR & 0xF8;
	switch (status)
	{
		case 0x08:
		if (IsSet)
		{
			Trg_addr = Set_addr;
			Trg_reg  = Set_reg;
		}
		else
		{
			current_room = room_index;
			Trg_addr = room_addr[current_room];
			Trg_reg  = Temp_Reg;
		}
		TWDR = (Trg_addr << 1);
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		break;

		case 0x18:
		TWDR = Trg_reg;
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		break;

		case 0x28:
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
		break;

		case 0x10:
		TWDR = (Trg_addr << 1) | 0x01; // SLA+R
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		break;

		case 0x40:
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
		break;

		case 0x50:
		if (!IsSet) {
			Temp_value[current_room] = TWDR;
		}
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		break;
		
		case 0x58:
		if (!IsSet)
		{
			Temp_value[current_room] = TWDR;
			room_index++;
			if (room_index >= Room_COUNT)
			room_index = 0;
		}
		IsSet = false;
		i2c_busy = 0;
		i2c_state = 0;
		TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
		break;

		default:
		IsSet = false;
		i2c_busy = 0;
		i2c_state = 0;
		TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
		break;
	}
}
void UART_init(void);
unsigned char USART_receive(void);
void UART_send(unsigned char data);
void UART_send_string(const char* string);
void process_command(char* command);
void UART_init(void)
{
	UBRRH = (uint8_t)(BAUD_PRESCALLER >> 8);
	UBRRL = (uint8_t)BAUD_PRESCALLER;
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}
unsigned char UART_receive(void)
{
	while (!(UCSRA & (1 << RXC)));
	return UDR;
}
void UART_send(unsigned char data)
{
	while (!(UCSRA & (1 << UDRE)));
	UDR = data;
}
void UART_send_string(const char* string)
{
	while (*string)
	{
		UART_send(*string++);
	}
}
void process_command(char* command)
{
	if (command[0] == 'D')
	{
		Set_reg = Led_Reg;
	}
	else if (command[0] == 'C')
	{
		Set_reg= Air_Reg;
	}
	switch(command[1])
	{
		case '1':
		Set_addr = 0x10;
		break;
		case '2':
		Set_addr = 0x11;
		break;
		default:
		break;
	}
	I2C_StartRequest();
}

int main(void)
{
	char data[64];
	uint16_t counter = 0;
	UART_init();
	I2C_Init();
	sei();
	_delay_ms(100);
	UART_send_string("Enter Password:\r\n");
	while (1)
	{	
		if (UCSRA & (1 << RXC))
		{
			char received = UART_receive();

			if (received == '\n' || received == '\r')
			{
				if (rx_index > 0)
				{
					rx_buffer[rx_index] = '\0';
					if(IsConnect)
					{
						if (rx_buffer[0] == 'D' || rx_buffer[0] == 'C')
						{
							IsSet = true;
							process_command(rx_buffer);
						}
					}
					else
					{
						if (strcmp(rx_buffer, Password) == 0)
						{
							IsConnect  = true;
							UART_send_string("Correct Password:\r\n");
						}
						else
						{
							UART_send_string("Enter Password:\r\n");
						}
					}
					rx_index = 0;
				}
			}
			else
			{
				if (rx_index < sizeof(rx_buffer) - 1)
				{
					rx_buffer[rx_index++] = received;
				}
			}
		}
		else if(IsConnect)
		{
			if (++counter>= 10000)
			{
				counter = 0;
				if(!i2c_state&&!IsSet)
				{
					I2C_StartRequest();
				}				
				sprintf(data, "Sensor 1: %d, Sensor 2: %d\r\n",Temp_value[0], Temp_value[1]);
				UART_send_string(data);
			}
		}
	}
}