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
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
		unsigned short x = 0x0000; //LED variables
	const unsigned short feedMe = 0x00FF; //The threshold for how low the value must be for it trigger the motors. 	unsigned char sensor = 0x00;	ADC_init();	while(1){		x = ADC;		if(x < feedMe){ sensor =  0x01;} else{sensor = 0x00;}		PORTC =  sensor; 		PORTB = (char)x;		PORTD = (((char)(x >> 8));	}	return 0;
}
