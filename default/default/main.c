/*
 * default.c
 *
 * Created: 12/3/2019 2:51:42 PM
 * Author : steph
 */ 

#include <avr/io.h>
#include <timer.h>

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
	// Tasks a should have members that include: state, period,
	//a measurement of elapsed time, and a function pointer
	signed char state; //Task's current state
	unsigned long int period; // Task period
	unsigned long int elapsedTime; // Time elapsed since last task tick
	int (*TickFct)(int); // Task tick function
}task;
// --------------End Task scheduler data structure

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger whenever
	// the previous conversion completes.
}

// Check the new ADC value
unsigned short x = 0xABCD;
unsigned char sensor = 0x00;

// Global Variables
//unsigned int arr[] = {0x00, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x80, 0x90}



//Tick Function #1: Button Logic

enum SM1_States{off, A, AB, B, BC, C, CD, D, DA};
int SM1Tick(int state){
	//Transitions
	x = ADC;
	switch(state){
		case off: if(x < 0x00FF){state = A;} else{state = off;}	break;
		case A: if(x < 0x00FF){state = AB;} else{state = off;}	break;
		case AB: if(x < 0x00FF){state = B;} else{state = off;}	break;
		case B: if(x < 0x00FF){state = BC;} else{state = off;}	break;
		case BC: if(x  < 0x00FF){state = C;} else{state = off;}	break;
		case C: if(x < 0x00FF){state = CD;} else{state = off;}	break;
		case CD: if(x < 0x00FF){state = D;} else{state = off;}	break;
		case D: if(x < 0x00FF){state = DA;} else{state = off;}	break;
		case DA: if(x < 0x00FF){state = A;} else{state = off;}	break;
		default: break;
	}
	
	//Actions
	switch(state){
		case off: PORTB = 0x00;	break;
		case A: PORTB = 0x01;	break;
		case AB: PORTB = 0x03;  break;
		case B: PORTB = 0x02;	break;
		case BC: PORTB = 0x06;	break;
		case C: PORTB = 0x04;	break;
		case CD: PORTB = 0x0C;	break;
		case D: PORTB = 0x80;	break;
		case DA: PORTB = 0x90; 	break;
		default: break;
	}
	return state;
}



int main(void)
{
    //Initialize DDRX and PORTX
    DDRA = 0x00; PORTA = 0xFF; //inputs for analog
    DDRB = 0xFF; PORTB = 0x00;
	
	// Period for the tasks
	unsigned long int SMTick1_calc = 2;
	//unsigned long int SMTick2_calc = 3;
	
	// Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, tmpGCD);//SMTick2_calc);	

	//Tasks code
	static task task1; //, task2;
	task *tasks[] = {&task1}; //, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	//Task #1
	task1.state = off; // Task initial state
	task1.period = SMTick1_calc/tmpGCD; //Task period
	task1.elapsedTime = SMTick1_calc/tmpGCD; //Task current elapsed time
	task1.TickFct = &SM1Tick; //Function pointer for the tick

/*
	//Task #2
	task2.state = -1; //Task initial state
	task2.period = SMTick2_calc/tmpGCD; // Task current elapsed time
	task2.elapsedTime = SMTick2_calc/tmpGCD; // Task current elapsed time 
	task2.TickFct = &SM2Tick; //Function pointer for the tick
*/	
	
	TimerSet(tmpGCD);
	TimerOn();
	ADC_init();
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

