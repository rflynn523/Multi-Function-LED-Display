// Ryan Flynn
// Multi-Function LED Display Header File
// Contains all defines and function prototypes.

#ifndef __LED_DISPLAY_H
#define __LED_DISPLAY_H

#define button   BIT3    // Button at pin 1.3

// First LED device (LSB)
// ----------------------
#define E0       BIT0  // Pin 1.0 -> segment E
#define D0       BIT1  // Pin 1.1 -> segment D
#define C0       BIT2  // Pin 1.2 -> segment C

// 1.3 is reserved for the button

#define dot      BIT4  // Pin 1.4 -> decimal point
#define G0       BIT5  // Pin 1.5 -> segment G

#define F0       BIT0  // Pin 2.0 -> segment F
#define A0       BIT1  // Pin 2.1 -> segment A
#define B0       BIT2  // Pin 2.2 -> segment B

// Second LED display (MSB)
// -----------------------
#define B1       BIT6  // Pin 1.6 -> segment B
#define E1       BIT7  // Pin 1.7 -> segment E

// dot is not used due to having not enough pins

#define G1       BIT3  // Pin 2.3 -> segment G
#define F1       BIT4  // Pin 2.4 -> segment F
#define A1       BIT5  // Pin 2.5 -> segment A
#define C1       BIT6  // Pin 2.6 -> segment C
#define D1       BIT7  // Pin 2.7 -> segment D


// Global Variables
// ----------------
// Used to change the display value.
int i = 0;

// Indicates what mode the program is in.
int mode = 0;

 // 0 - start_sequence, 1 - Main menu/ selecting, 2 - a mode is in progress, 3 - end sequence
int stage = 0;

// Used for the iterations when the user is still selecting a value.
int select = 0;

// Used to blink the decimal point or double zero.
int DOT = -1;
int OO = -100;

// Used for the timer amount
int num_select;
int unit;

// Used to start and stop the stop watch.
// 0 - Going, 1 - stopped.
int stop = 1;
int amount = -1;

// Mode Functions
// --------------

// Mode 0: Count
// Displays all possible numbers that could be on the LEDs.
// Starts from 0 and ends at 99.
// Increases when the button is pressed.
void count(void);

// Mode 1: Morse Code
// Flashes a message in morse code that is hard coded.
// 88 - The message is starting
// 8 - A new word is beginning
void morse_code(void);

// Helper that blinks the dot for 0.5 second.
void blink_dot(void);

// Helper that blinks the dash for 1.5 seconds
void blink_dash(void);

// Mode 2: Random Number
// Select a maximum number using the button and then the LED will display
// a random number between 1 and the max.
// Select the number by waiting for the decimal point to blink 10 times.
// The program goes back to the main menu after the
//  decimal point blinks 15 times.
void random_number(void);

// Random Number sequence that plays after the max is selected and before
// the random number is shown.
void generating(void);

// Mode 3: Timer
// If 0 is passed select an amount using the buttons and waiting for the d.p.
// to blink 10 times on the same number or used the number that is passed.
// Then select a unit using the same method. (sec, min, hour)
// 88 will flash and then the timer will start.
// When the time is up double 00 will blink 15 times and then the program will
// exit back to the main menu.
void timer(int amount);

// void display_letter(void) is a helper but already declared.
// Used to cycle through s, m, and h.

// Mode 4: Stop Watch
// Time is in seconds.
// Starts when the button is pressed and pauses when the button is pressed again.
// 10 blinks and the time will go back to zero.
// 10 more blinks form that point will exit back to the main menu.
void stop_watch(void);

// Will display a letter to the LED Display, passed as a number in the alphabet,
// Starting from 0.
// For now, it only supports S(18), M(12), and H(7).
void display_letter(int letter);

// Mode 5: Shot Clock
// Acts as an NBA stop watch continuously.
// Passes 24 to the timer function.
void shot_clock(void);

// Flow functions
// --------------

// Handles selecting the mode.
void main_menu(void);

// Start sequence that plays when device is first plugged in.
void start_sequence(void);
void sequence_pause(void);

// 0 - build the segments up from nothing to 8.
// 1 - destroy the segments from 8 to nothing.
void build_destroy (int control);

// After the button is pressed to exit a mode this is the transition back
// to the main menu.
void end_sequence(void);

// Control Functions
// -----------------
// Will display the number passed, to the LED Display
void display(int number);

// Clears all of the segments NOT including the decimal point
void clear(void);

// Blinks whatever is passed once.
// DOT(-1) is passed when the decimal point needs to blink.
// OO(-100) is passed when double zero needs to be blinked.
void blink(int x);

// ISRs
// ---

// Button
// Clears the timer and the timer interrupt flag
// Enables the timer interrupt
// Clears the button interrupt

// Timer
// Used for debouncing.
// Performs different actions depending on the stage and mode.
// Then disables the timer interrupt and re-enables the button interrupt

// Intrinsic Functions
//--------------------

// _enable_interrupt()

// __delay_cycles()
// Takes in a value to determine the delay time
// from the equation Cycles = Freq  x Time.

#endif
