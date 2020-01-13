# Multi-Function-LED-Display

This repository is holds the header file and the source code that my Multi-Function LED Display uses.
I made this using two seven segment LEDs and the MSP-EXP430 G2 Launchpad Microcontroller.

  The display currently has six modes:
 
 Mode 0 (Count): The display will simply show a number starting at 0 and ending at 99. The number is incremented when the button is pressed.
   
 Mode 1 (Morse Code): Flashes a pre-programmed message in morse code onto the seven segment display on the right.
 
 Mode 2 (Random Number): This mode allows the user to select a random number using the button and the LEDs will display a random number between 1 and that number.
 
 Mode 3 (Timer): The user can again select a number (1 - 99) to be set as standard timer. They also have the choice of selecting seconds, minutes, or hours.
 
 Mode 4 (Stopwatch): This mode acts as a typical stopwatch. Pressing the button will pause the time and will reset the stopwatch after it has been paused for more than a few seconds.
 
 Mode 5: (Shot Clock): Acts as an NBA 24 second shot clock. Once the time is up it will automatically reset back to 24.

A demo video can be seen at this link: https://youtu.be/Ino6i9vFSoI 
