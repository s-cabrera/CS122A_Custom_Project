/*
 * Default_option.c
 *
 * Created: 11/27/2019 2:39:56 PM
 * Author : steph
 */ 

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger whenever
	// the previous conversion completes.
}


int main(void)
{
    /* Replace with your application code */
	//Initialize DDRX and PORTX
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	
	
	
	//We need a flag for when the value of the sensor is too low -> trigger the 
	const unsigned short tooWet = 0x00FF;  
	unsigned char sensor = 0x00;
	unsigned short x = 0xABCD;
	
	ADC_init();
	 
	while (1) 
    {
		x = ADC;		if(x < tooWet){ sensor =  0x01;} else{sensor = 0x00;}		PORTC =  sensor;		PORTB = (char)x;		PORTD = (char)((x >> 8));
    }
}

