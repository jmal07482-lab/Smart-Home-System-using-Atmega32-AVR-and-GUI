#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


#define  LCD_DATA PORTC
#define rs PD0
#define rw PD1
#define en PD2

void LCD_init(void);
void LCD_cmd(unsigned char);
void LCD_write(unsigned char);
void LCD_String(char *str);
void lcd_argument(void);

#define DHT_PORT   PORTA
#define DHT_DDR    DDRA
#define DHT_PINREG PINA
#define DHT_PIN    PA0

#define LED_PORT PORTB
#define LED_DDR  DDRB
#define LED_OK   PB0
#define LED_FAIL PB1

// =====  DHT22 =====
void DHT22_Start(void)
{
	DHT_DDR |= (1 << DHT_PIN);      // Output
	DHT_PORT &= ~(1 << DHT_PIN);    // Low for start signal
	_delay_ms(20);                  // 20 ms  
	DHT_PORT |= (1 << DHT_PIN);     // High
	_delay_us(30);                  // 20-40 us
	DHT_DDR &= ~(1 << DHT_PIN);     // Input (release line)
}

// ==== check signal ====
uint8_t DHT22_CheckResponse(void)
{
	uint8_t response = 0;
	_delay_us(40);

	if (!(DHT_PINREG & (1 << DHT_PIN)))     // low 80us
	{
		_delay_us(80);

		if (DHT_PINREG & (1 << DHT_PIN))    // high 80us
		{
			response = 1;
		}

		_delay_us(80);
	}

	return response;
}

// ==== Byte ====
uint8_t DHT22_ReadByte(void)
{
	uint8_t i, data = 0;

	for (i = 0; i < 8; i++)
	{
		while (!(DHT_PINREG & (1 << DHT_PIN)));  //  HIGH

		_delay_us(40);  // 

		if (DHT_PINREG & (1 << DHT_PIN)) //  HIGH  bit = 1
		data |= (1 << (7 - i));

		while (DHT_PINREG & (1 << DHT_PIN)); //  LOW
	}
	return data;
}

// ==== DHT22 ====
uint8_t DHT22_ReadData(float *humidity, float *temperature)
{
	uint8_t hum_H, hum_L, temp_H, temp_L, checksum;

	DHT22_Start();
	if (!DHT22_CheckResponse()) return 0;

	hum_H   = DHT22_ReadByte();
	hum_L   = DHT22_ReadByte();
	temp_H  = DHT22_ReadByte();
	temp_L  = DHT22_ReadByte();
	checksum = DHT22_ReadByte();

	if ((uint8_t)(hum_H + hum_L + temp_H + temp_L) != checksum)
	return 0;

	uint16_t raw_humidity = hum_H * 256 + hum_L; //shift to left 8 bit
	uint16_t raw_temp     = temp_H * 256 + temp_L;

	*humidity = raw_humidity / 10.0; 

	if (raw_temp & 0x8000)      
	{
		raw_temp &= 0x7FFF;
		*temperature = -(raw_temp / 10.0);
	}
	else
	{
		*temperature = raw_temp / 10.0;
	}

	return 1; // success
}





float humidity=7, temperature=0;
int main(void)
{
	DDRC =0xFF;
	DDRD =0xFF;
	LCD_init();
	_delay_ms(1);
	LCD_cmd(0x0C);
	_delay_ms(1);
	lcd_argument();
	
	
	LED_DDR |= (1 << LED_OK) | (1 << LED_FAIL);
	
	unsigned char arr_hum[10];
	unsigned char arr_temp[10];
	
    while (1) 
    {
		if (DHT22_ReadData(&humidity, &temperature))
		{
			float hm=humidity;
			dtostrf(hm,6,1,arr_hum);
			LCD_cmd(0x80 + 10);
			LCD_String(arr_hum);
			float tp = temperature;
			dtostrf(tp, 5, 1, arr_temp); 
			LCD_cmd(0xC0 + 7);      
			LCD_String(arr_temp);
			
			LED_PORT &= ~(1 << LED_FAIL);  

			if (humidity <= 20.0 || humidity >=80)
			{
				LED_PORT |= (1 << LED_OK);  
			}
			else
			{
				LED_PORT &= ~(1 << LED_OK);
			}
		}
		else
		{
			LED_PORT |= (1 << LED_FAIL);  
		}
		
		
		
		/*LCD_String("Humidiy : ");
		_delay_ms(50);
		
		LCD_cmd(0xC0); // move to the second line
		_delay_ms(1);
		
		
		LCD_String("temp : ");
		_delay_ms(150);
		
		LCD_cmd(0x0E);
		_delay_ms(1);
		LCD_cmd(0x01);*/
		
		
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
void lcd_argument(void)
{
	LCD_String("Humidity : ");
	LCD_cmd(0xC0);
	LCD_String("temp : ");
	LCD_cmd(0x0E);
}


