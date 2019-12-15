/*
 * Moisture_sensors.c
 *
 * Created: 11/9/2019 3:13:54 PM
 * Author : steph
 */ 

#include <avr/io.h>
//#include <timer.h>
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger whenever
	// the previous conversion completes.
}/*void ADC_on(){	}void ADC_off(){	}void ADC_read(){	}*/int main(){	//Initialize DDRX and PORTX	DDRA = 0x00; PORTA = 0xFF;	DDRB = 0xFF; PORTB = 0x00;	DDRD = 0xFF; PORTD = 0x00;	//LED variables	unsigned short x = 0xABCD;
	//unsigned char my_char = (char)my_short; // my_char = 0xCD
	//my_char = (char)(my_short >> 4); // my_char = 0xBC		ADC_init();	//ADC_on();	//ADC_off();	//x = ADC;	while(1){		x = ADC;		PORTB = (char)x;		PORTD = (((char)(x >> 2)) & 0xC0);	}	return 0;}
/*
//----------Find GCD function ------------------------------------------------//
unsigned long int findGCD(unsigned long int a, unsigned long int b){
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}


//----------End find gcd function --------------------------------------------//

//---------------Task scheduler data structure ------------------------------//
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task{
	 Tasks a should have members that include: state, period,
	a measurement of elapsed time, and a function pointer 
	signed char state; //Task's current state
	unsigned long int period; // Task period
	unsigned long int elapsedTime; // Time elapsed since last task tick
	int (*TickFct)(int); // Task tick function
}task;
// --------------End Task scheduler data structure 

//Tick Function #1: Button Logic
enum SM1_States{Release, Press};
int SM1Tick(int state){
	static unsigned char flag = 0;
	//Transitions
	switch(state){
		case Release:
			if(PINA == 0xFE){flag = flag ? 1:0;
				state = Press;}
			else{state = Release;}
		break;
		
		case Press:
			if(PINA == 0xFE){state = Press;}
			else{
				PORTB = flag ? 0x01:0x00;
				state = Release;
			}
		break;
		
		default: break;
	}
	
	//Actions
	switch(state){
		case Release:
		break;
		
		case Press:
		break;
		
		default: break;
	}
	
	return state;	
};


int main(void)
{

	//Ports and DDRX
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	// Period for the tasks
	unsigned long int SMTick1_calc = 50;
	
	// Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, tmpGCD);
	
	// Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/tmpGCD;
	
		
	//Tasks code
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task #1
	task1.state = -1; // Task initial state
	task1.period = SMTick1_period; //Task period
	task1.elapsedTime = SMTick1_period; //Task current elapsed time
	task1.TickFct = &SM1Tick; //Function pointer for the tick
	
	TimerSet(tmpGCD);
	TimerOn();
	
	//unsigned char sensorReading = 0;
	//unsigned char sensorOn = 0; 
	unsigned char i = 0;	
    while (1) 
    {
		//Scheduler code
		for(i = 0; i < numTasks; i++){
			// Task is ready to tick
			if(tasks[i]->elapsedTime == tasks[i]->period){
				//Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				//Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime +=1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    }
	return 0;
}
*/
