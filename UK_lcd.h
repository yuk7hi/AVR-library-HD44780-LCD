/****************************************************
Author			: Yukthi Lochana
Date created	: 14/08/2017
Last Updates	: 16/08/2017

Read me section	:

Functions to be used by the user...
*cursorPosLCD(int add, int N)
	Move the cursor to the required address.
	1-line mode, 80 positions
	2-line mode, 40 positions in each line
	add - address value

Functions to be used only by the header file...
*enableLCD(int i)
	Enables the data bus to read/write data through.
*shortEnLCD()
	Enables the data bus for a short period to
	read/write data.
*checkBFLCD()
	Checks the Busy Flag. (whether and internal 
	operation is running)
	BF
		1 -> an internal excution is taking place
		0 -> system is ready to accept/send data
*instructLCD(int vRS, int vRW)
	Alter the command bit values of the system.
	RS - Register Selection
		1 -> select Data Register (DR) for sending/
			 receiving data. (via D0-D7)
		0 -> select Instruction Register (IR) for
			 sending/recieving commands. (via D0-D7)
	R/W - Read/Write mode selection
		1 -> function data bus as to Read data from
			 the system.
		0 -> function data bus as to Write data to
			 the system.
*dataBusRWLCD(int i)
	Define pins connected to the LCD as input/output
	D0-D7
		 1 -> pins defined as inputs.
		 0 -> pins defined as outputs.	
****************************************************/
#ifndef UK_lcd_h
#define UK_lcd_h

#include <avr/io.h>
#include <util/delay.h>

#define ISDDR DDRC
#define ISPORT PORTC
#define RS PC5
#define RW PC4
#define En PC3

#define DTDDR DDRA
#define DTPORT PORTA
#define DTPIN PINA
#define DB0 PA0
#define DB1 PA1
#define DB2 PA2
#define DB3 PA3
#define DB4 PA4
#define DB5 PA5
#define DB6 PA6
#define DB7 PA7
#endif

void enableLCD(int i);
void shortEnLCD();
void checkBFLCD();
void instructLCD(int vRS, int vRW);
void dataBusRWLCD(int i);
void cursorPosLCD(int add, int N);
void functionSetLCD(int DL, int N, int F);
void dispContLCD(int D, int C, int B);
void cursorMovLCD(int SC, int RL);
void entryModeLCD(int ID, int S);
void returnHomeLCD();
void clearScrLCD();
void printChLCD(char ch);
void printStrLCD(char *str);


void enableLCD(int i)
{
	if(i == 0)
		ISPORT &= ~(1<<En);
	else
		ISPORT |= (1<<En);
}

void shortEnLCD()	//Enable LCD inputs for a short period
{
	enableLCD(1);
	_delay_us(50);
	enableLCD(0);
}

void checkBFLCD()		//Check Busy Flag - whether any internal executions are running
{
	instructLCD(0, 1);
	enableLCD(1);
	while(DTPIN & (1<<DB7))
		_delay_us(5);		//Hold till LCD display is available (Busy Flag -> 0)
	enableLCD(0);
}

void instructLCD(int vRS, int vRW)
{
	//IR = 0 (Instruction Register), DR = 1  (Data Register)
	if (vRS == 0)
		ISPORT &= ~(1<<RS);
	else
		ISPORT |= (1<<RS);

	//Write = 0, Read = 1
	if (vRW == 0){
		ISPORT &= ~(1<<RW);	//write mode
		dataBusRWLCD(0);	//data bus to output mode
	}
	else{
		ISPORT |= (1<<RW);	//read mode
		dataBusRWLCD(1);	//data bus to input mode
	}
}

void dataBusRWLCD(int i)
{
	//data bus write if 0 (pins as output), read if 1 (pins as input)
	if(i == 0)
		DTDDR = 0xFF;
	else{
		DTDDR = 0x00;
		DTPORT = 0x00;	//disable internal pullups
	}
}

void cursorPosLCD(int add, int N)
{
	checkBFLCD();
	instructLCD(0, 0);		//select IR in Write mode
	DTPORT = (1<<DB7);		//set DDRAM (cursor) address instruction

	//add - cursor address  for 1-line line1:(1-80), for 2-lines line1:(1-40) & line2(41-80)
	//N - # of lines defined for function set instruction  1->1-line, 2->2-lines
	N -= 1;
	add -= 1;
	if(N == 0)
		DTPORT |= add;
	else{
		DTPORT |= add + 64;
	}
	shortEnLCD();
}

void functionSetLCD(int DL, int N, int F)
{
	ISDDR |= (1<<En) | (1<<RW) | (1<<RS);	//Instruction pins connected as outputs
	_delay_ms(50);
	checkBFLCD();			//wait for the LCD to get prepared after switching ON
	instructLCD(0, 0);		//select IR in Write mode
	DTPORT = (1<<DB5);		//funtion set instruction

	//DL - data length  1->8-bit, 0->4-bit
	if(DL == 1)
		DTPORT |= (1<<DB4);
	//N - # of lines  0->1-line, 1->2-lines
	if(N == 1)
		DTPORT |= (1<<DB3);
	//F - font size  0->5x8, 1->5x10
	if(F == 1)
		DTPORT |= (1<<DB2);
	shortEnLCD();
}

void cursorMovLCD(int SC, int RL)
{
	checkBFLCD();
	instructLCD(0, 0);		//select IR in Write mode
	DTPORT = (1<<DB4);		//Cursor/Display shift instruction

	//S/C - Screen/Cursor  1->screen shift, 0->cursor move
	if(SC == 1)
		DTPORT |= (1<<DB3);
	//R/L - shift screen/move cursor Right/Left  1->right, 0->left
	if(RL == 1)
		DTPORT |= (1<<DB2);
	shortEnLCD();
}

void dispContLCD(int D, int C, int B)
{
	checkBFLCD();
	instructLCD(0, 0);		//select IR in Write mode
	DTPORT = (1<<DB3);		//Display control instruction

	//D - Display  1->on, 0->off
	if(D == 1)
		DTPORT |= (1<<DB2);
	//C - Cursor  1->on, 0->off
	if(C == 1)
		DTPORT |= (1<<DB1);
	//B - cursor position blink  1->on, 0->off
	if(B == 1)
		DTPORT |= (1<<DB0);
	shortEnLCD();
}

void entryModeLCD(int ID, int S)
{
	checkBFLCD();
	instructLCD(0, 0);		//select IR in Write mode
	DTPORT = (1<<DB2);		//Entry mode set instruction

	//I/D - Increment(right)/Decrement(left)  1->data entry to right, 0->data entry to left
	if(ID == 1)
		DTPORT |= (1<<DB1);
	//S - Shift display with entry  1->enable, 0->disable
	if(S == 1)
		DTPORT |= (1<<DB0);
	shortEnLCD();
}

void returnHomeLCD()		//returns cursor to the 00H address (initial position) of the display
{
	checkBFLCD();
	instructLCD(0, 0);		//select IR in Write Mode
	DTPORT = (1<<DB1);
	shortEnLCD();
}

void clearScrLCD()			//clear the display, and resets entry mode (to increment/right)
{
	checkBFLCD();
	instructLCD(0, 0);
	DTPORT = (1<<DB0);
	shortEnLCD();
	_delay_ms(50);
}

void printChLCD(char ch)
{
	checkBFLCD();
	instructLCD(1, 0);		//select DR in Write mode
	DTPORT = ch;				//output charater to DR to print
	shortEnLCD();

	checkBFLCD();
	_delay_us(5);			//there's a technical 4us delay after every LCD print out - refer to data sheet
}

void printStrLCD(char *str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		checkBFLCD();
		_delay_us(5);		//there's a technical 4us delay after every LCD print out - refer to data sheet
		instructLCD(1, 0);	//select DR in Write mode
		DTPORT = str[i];		//output character to DR to print
		shortEnLCD();
		i++;
	}
	checkBFLCD();
	_delay_us(5);
}

