/*
 * A2D.c
 *
 * Created: 11/15/2019 3:11:00 PM
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
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned short my_short = 0xABCD;
	unsigned char my_char = (char)my_short; // my_char = 0xCD
	my_char = (char)(my_short >> 4); // my_char = 0xBC
   
	ADC_init();
    
	while (1) 
    {
		my_short = PINA0;
		my_char = (char)my_short;
		PORTD = my_char;
		my_char = (char)(my_short >> 4);
		PORTB = my_char & 0x03;

    }
}

