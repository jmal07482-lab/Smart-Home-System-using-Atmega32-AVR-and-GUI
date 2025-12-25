/*
 * rain_sensor.c
 *
 * Created: 12/6/2025 6:41:29 AM
 * Author : Lenovo
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>

#define LED_PIN     PC0 
#define RAIN_SENSOR_PIN PD0

void setup() {
	
	DDRC |= (1 << LED_PIN);
	DDRD &= ~(1 << RAIN_SENSOR_PIN);
	PORTC &= ~(1 << LED_PIN);
}

void loop() {
	
	if (PIND & (1 << RAIN_SENSOR_PIN)) 
	    {
		  PORTC |= (1 << LED_PIN);
		}
 else {
		PORTC &= ~(1 << LED_PIN);
	  }
	
	
	_delay_ms(2);
}

int main(void)
{
	setup();
    /* Replace with your application code */
    while (1) 
    {
		loop();
    }
	return 0;
}

