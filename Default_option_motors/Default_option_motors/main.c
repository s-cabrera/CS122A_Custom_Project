/*
 * Default_option_motors.c
 *
 * Created: 11/27/2019 3:38:48 PM
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

// Global Variables

//unsigned int arr[] = {0x00, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x80, 0x90}

//Tick Function #1: Button Logic
enum SM1_States{off, A, AB, B, BC, C, CD, D, DA};	
int SM1Tick(int state){
	
	//Transitions
	switch(state){
		case off: if(PINC == 0xFF){state = A;} else{state = off;}	break;
		case A: if(PINC == 0xFF){state = AB;} else{state = off;}	break;
		case AB: if(PINC == 0xFF){state = B;} else{state = off;}	break;
		case B: if(PINC == 0xFF){state = BC;} else{state = off;}	break;
		case BC: if(PINC == 0xFF){state = C;} else{state = off;}	break;
		case C: if(PINC == 0xFF){state = CD;} else{state = off;}	break;
		case CD: if(PINC == 0xFF){state = D;} else{state = off;}	break;
		case D: if(PINC == 0xFF){state = DA;} else{state = off;}	break;
		case DA: if(PINC == 0xFF){state = A;} else{state = off;}	break;
		default: break;
	}
	
	//Actions
	switch(state){
		case off: PORTA = 0x00; break;
		case A: PORTA =	0x01;	break;
		case AB: PORTA = 0x03;  break;
		case B: PORTA = 0x02;	break;
		case BC: PORTA = 0x06;	break;
		case C: PORTA = 0x04;	break;
		case CD: PORTA = 0x0C;	break;
		case D: PORTA = 0x80;	break;
		case DA: PORTA = 0x90; 	break;
		default: break;
	}

	return state;
}


int main(void)
{

	//Ports and DDRX
	DDRA = 0xFF; PORTA = 0x00;
	DDRC = 0x00; PORTC = 0xFF;
	
	// Period for the tasks
	unsigned long int SMTick1_calc = 3;
	
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
	task1.state = off; // Task initial state
	task1.period = SMTick1_period; //Task period
	task1.elapsedTime = SMTick1_period; //Task current elapsed time
	task1.TickFct = &SM1Tick; //Function pointer for the tick
	
	TimerSet(tmpGCD);
	TimerOn();
	
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

