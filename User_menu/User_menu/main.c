/*
 * User_menu.c
 *
 * Created: 11/27/2019 1:56:19 PM
 * Author : steph
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <bit.h>
#include <timer.h>
#include <stdio.h>
#include "io.c"
//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}
//--------End find GCD function ----------------------------------------------
//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
	a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;
//--------End Task scheduler data structure-----------------------------------

//--------Shared Variables----------------------------------------------------

/*unsigned char message[37] = {'C','S','1','2','0','B',' ', 'i', 's', ' ', 'L',
'e', 'g', 'e', 'n', 'd', '.', '.', '.', ' ', 'w','a', 'i', 't', ' ', 'f', 'o',
'r', ' ', 'i', 't', ' ', 'D', 'A', 'R', 'Y', '!'}; 
*/
char welcome[] = "Welcome to Plant Parenthood ... Press Right to Start";
unsigned char strSizeWelcome = 52;
char plantType[] = "Pick a watering type ... use Right and Left buttons for scrolling options. Center to Select";
unsigned char strSizePlantType = 91;



/*
char low[] = "Low watering level"; // 
unsigned char strSizeLow = ; //
char lowType[] = "You selected a low watering level :)"; //trigger the motor every five seconds
unsigned char strSizeLowType = ; // 

char medium[] = "Medium watering level"; // 
unsigned char strSizeMedium = ; //
char mediumType[] = "You selected a medium watering level :)"; //trigger the motor every ten seconds
unsigned char strSizeMediumType = ; // 

char high[] = "High watering level"; // 
unsigned char strSizeHigh = ; //
char highType[] = "You selected a high watering level :)"; // trigger the motor every 20 seconds
unsigned char strSizeHighType = ; //
 

char default[] = "Default level"; // 
unsigned char strSizeDefault = ; //
char defaultType[] = "You have selected the default option :)"; // use the sensors to trigger
unsigned char strSizeDefaultType = ; //  
*/

//The temp array that gets updated to look like the string is moving. 
unsigned char temp[16]= {'1', '2', '3', '4', '5', '6', '7', '8',
		'9', '-', '*', '&', '%', '0', '0', '0'};	
//--------End Shared Variables------------------------------------------------




//--------User defined FSMs---------------------------------------------------

// State Machine 1:  Button Logic 
unsigned char option = 0x00;
enum SM1_States{release, press};
int SMTick1(int state){
	
	// Transitions
	switch(state){
		case release:
			if((~PINA == 0xFE) || (~PINA == 0xFD) || (~PINA == 0xFB)){
				if(~PINA == 0xFE){
					option = 1; // right
				}
				else if(~PINA == 0xFD){
					option = 2; // center
				}
				else if(~PINA == 0xFB){
					option = 3; // left
				}
				state = press;
			}
			else{
				state = release;
			}
		break;
	
		case press:
			if((~PINA == 0xFE) || (~PINA == 0xFD) || (~PINA == 0xFB)){
				if(~PINA == 0xFE){
					option = 1; // right
				}
				else if(~PINA == 0xFD){
					option = 2; // center
				}
				else if(~PINA == 0xFB){
					option = 3; // left
				}
				state = press;
			}
			else{
				option = 0;
				state = release;
			}
			state = release;
			//PORTB = PINA;			
		break;
	
		default: break;
	}
	
	//Actions
	switch(state){
		case release:
		break;
			
		case press:
			if(option == 3){
				PORTB = (0x04 << 3) | option;
			}
			else if(option == 1){
				PORTB = (0x01 << 3) | option;
			}
			else if(option == 2){
				PORTB = (0x02 << 3) | option;
			}
//			PORTB = PINA;
		break;
			
		default: break;
	}	
	return state;
};


//Enumeration of states.
enum SM2_States {SM2_Start,SM2_wait, SM2_setArray};
unsigned char j = 0;
unsigned char cnt;
unsigned char in = 0;//index for the str array
int SMTick2(int state) {
	//local variables

	//State machine transitions
	switch(state){
		case SM2_Start:
		state = SM2_wait;
		break;
		case SM2_wait:
		if(in < strSizeWelcome){
			state = SM2_setArray;
		}
		else{
			state = SM2_wait;
		}
		break;
		
		case SM2_setArray:
		if(in < strSizeWelcome){
			state = SM2_setArray;
		}
		else{
			state = SM2_wait;
		}
		break;
		
		default:
		break;
	}
	//State machine actions
	switch (state) {
		case SM2_Start:
		break;
		
		case SM2_wait:
		cnt = 0;
		j = 0;
		in = 0;
		break;
		
		case SM2_setArray:
		cnt = 0;
		for(j = 0; j <= 16; j++){
			if(in > 16){
				temp[j-1] = welcome[cnt + (in - 16)];
				cnt ++;
			}
			else if(j >= (16 - in) && (in - cnt) >= 0){
				temp[j-1] = welcome[cnt];
				cnt ++;
			}
			
			else{
				temp[j-1] = ' ';  // replace it with 
			}
		}
		in++;
		break;
		
		default:
		break;
	}
	
	return state;
}


unsigned char t = 0;

//Enumeration of states.
enum SM3_States { SM3_Start, SM3_wait, SM3_write};
int SMTick3(int state) {
	//State machine transitions
	switch (state) {
		case SM3_Start:
		LCD_init();
		state = SM3_wait;
		break;
		
		case SM3_wait:
		state = SM3_write;
		break;
		
		case SM3_write:
		state = SM3_wait;
		break;
		
		default:
		break;
	}
	//State machine actions
	switch(state) {
		
		case SM3_Start:
		break;
		
		case SM3_wait:
		break;
		
		case SM3_write:
		for(t = 0; t <= 16; t++){
			LCD_Cursor(t);
			LCD_WriteData(temp[t-1]);
			
		}
		//LCD_ClearScreen();
		//LCD_Cursor(1);
		//LCD_DisplayString(1, temp);
		//LCD_ClearScreen();
		break;
		
		default:
		break;
	}
	return state;
}


	

// --------END User defined FSMs-----------------------------------------------
// Implement scheduler code from PES.
int main()
{
	// Set Data Direction Registers
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRB = 0xFF; PORTB = 0x00; // For LEDS
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	

	// Period for the tasks
	unsigned long int SMTick1_calc = 20; //250
	unsigned long int SMTick2_calc = 275; //175
	unsigned long int SMTick3_calc = 285; // 
	
	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	tmpGCD = findGCD(tmpGCD, SMTick3_calc);

	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/tmpGCD;
	unsigned long int SMTick2_period = SMTick2_calc/tmpGCD;
	unsigned long int SMTick3_period = SMTick3_calc/tmpGCD;
	
	//Declare an array of tasks
	static task task1, task2, task3;
	task *tasks[] = {&task1, &task2, &task3};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	// Task 1
	
	task1.state = release;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &SMTick1;//Function pointer for the tick.

	
	// Task 2
	task2.state = SM2_Start;//Task initial state.
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period;//Task current elapsed time.
	task2.TickFct = &SMTick2;//Function pointer for the tick.
	
	//Task 3
	task3.state = SM3_Start;//Task initial state.
	task3.period = SMTick3_period;//Task Period.
	task3.elapsedTime = SMTick3_period;//Task current elapsed time.
	task3.TickFct = &SMTick3;//Function pointer for the tick.	
	
	// Set the timer and turn it on
	TimerSet(tmpGCD);
	TimerOn();
	unsigned short i; // Scheduler for-loop iterator
	
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	// Error: Program should not exit!
	return 0;
}


                                                                