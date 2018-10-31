# AVR-library-HD44780-LCD
This provides an AVR library (header file) for interfacing HD44780, aka 16x2 LCD display with Atmel ATmega32 microcontrollers.

## Functions defined (explained):

### Functions to be used by the user
#### cursorPosLCD(int add, int N)
	Move the cursor to the required address.
	1-line mode, 80 positions
	2-line mode, 40 positions in each line
	add - address value

### Functions to be used only by the header file
#### enableLCD(int i)
	Enables the data bus to read/write data through.
#### shortEnLCD()
	Enables the data bus for a short period to
	read/write data.
#### checkBFLCD()
	Checks the Busy Flag. (whether and internal 
	operation is running)
	BF - Busy Flag
		1 -> an internal excution is taking place
		0 -> system is ready to accept/send data
#### instructLCD(int vRS, int vRW)
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
#### dataBusRWLCD(int i)
	Define pins connected to the LCD as input/output
	D0-D7
		 1 -> pins defined as inputs.
		 0 -> pins defined as outputs.
