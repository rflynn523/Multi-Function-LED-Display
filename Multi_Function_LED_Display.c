#include <msp430.h>
#include "Multi_Function_LED_Display_Header.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

 void main(void)
 {
  // Stop Watchdog timer and re-purpose XIN and XOUT bits
  WDTCTL = WDTPW | WDTHOLD;
  P1SEL &= ~0xC0;
  P2SEL &= ~0xC0;

  // Set the oscillator to run at 1 MHz
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;

  // Direct pins as output
  P1DIR |= C0 | D0 | E0 | G0 | dot | B1 | E1;
  P2DIR |= A0 | B0 | F0 | A1 | C1 | D1 | F1 | G1;

  // Configure button and it's interrupt
  P1DIR &= ~button;     // Sets as input
  P1REN |= button;      // Enables built in resistors
  P1OUT |= button;      // resistors pulled up to Vcc
  P1IE |= button;       // Enable interrupts
  P1IES |= button;      // interrupt on falling edge
  P1IFG &= ~button;     // Clear the interrupt flag

  // Configure Timer for the use of button debouncing.
  TA0CCR0 = 8000;
  TA0CTL = TASSEL_2 | ID_3 | MC_1 | TACLR;  // SMCLK / div by 1 / up mode / Timer A Clear
  TA0CTL &= ~TAIFG;

  // Enable the global interrupt bit (an intrinsic function)
   _enable_interrupt();

   // Make sure the decimal point is off initially.
   P1OUT &= ~dot;

   while(stage == 0)
   {
       start_sequence();
   }

   main_menu();
}

// Handles the selection of the mode and then calls that function.
// After the SAME number has blinked 10 times that mode is chosen and
// the corresponding function is called.
void main_menu(void)
{
  while(1)
  {
    select = 0;
    i = 0;

  // Continuously blinks numbers until 10 iterations of the
  //same number where it then goes onto what ever mode is selected.
  while (select < 10)
  {
    blink(i);
    select++;
    mode = i;
  }

  stage = 2;
  i = 0;

  while (stage == 2)
  {
    if (mode == 0)
      count();

    if (mode == 1)
      morse_code();

    if (mode == 2)
      random_number();

    if (mode == 3)
      timer(0);

    if (mode == 4)
      stop_watch();

    if (mode == 5)
      shot_clock();

    if (mode > 5)
      display(23);
    }

    end_sequence();
  }
}

// ************************************** Mode 0: Count ***********************************************//
// Displays the value of i starting from 0 and ending at 99
// Increases when the button is pressed.
void count(void)
{
    display(i);
}

// ************************************ Mode 1: Morse Code ********************************************//
// Will display a hard coded message in morse code to the LED display.
// 88 - the message is starting.
// 8 - a new word is beginning (a space)
void morse_code()
{
    int index;
    unsigned int letter = 0;
    int j = 0;

    P1OUT &= ~dot;

    display(88);
    __delay_cycles(500000);
    clear();

    char str[] = "Go Knights Charge On";
    char alpha[][6] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--",
                         "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",
                         "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

    while (str[letter] != NULL)
    {
        j = 0;

        // Lower Case
        if (str[letter] >= 'a' && str[letter] <= 'z')
        {
          // Finds the corresponding index in the alpha array.
          index = str[letter] - 'a';

          // Loops through the string and determines if a dot or dash is needed.
          while (alpha[index][j] != '\0')
          {
            if (alpha[index][j] == '.')
              blink_dot();

            else if (alpha[index][j] == '-')
              blink_dash();

            j++;
          }
        }

        // Upper Case
        else if (str[letter] >= 'A' && str[letter] <= 'Z')
        {
          // Finds the corresponding index in the alpha array.
          index = str[letter] - 'A';

          // Loops through the string and determines if a dot or dash is needed.
          while (alpha[index][j] != '\0')
          {
            if (alpha[index][j] == '.')
              blink_dot();

            else if (alpha[index][j] == '-')
              blink_dash();

            j++;
          }
        }

        // Numbers
        else if (str[letter] >= '0' && str[letter] <= '9')
        {
          // Finds the corresponding index in the alpha array.
          index = str[letter] - 26;

          // Loops through the string and determines if a dot or dash is needed.
          while (alpha[index][j] != NULL)
          {
            if (alpha[index][j] == '.')
              blink_dot();

            if (alpha[index][j] == '-')
              blink_dash();

            j++;
          }
        }

        // Space
        else if(str[letter] == ' ')
        {
          // Clears the dash and the dot.
          P1OUT &= ~dot & ~G0;

          // Displays an 8 to the MSB
          P1OUT |= B1 | E1;
          P2OUT |= A1 | C1 | D1 | F1 | G1;

          __delay_cycles(500000);

          clear();
        }

        __delay_cycles(1000000);

        // Move along in the string
        letter++;
  }
  return;
}

// Morse Code Helpers.
// Blinks a dot for 0.5 second.
void blink_dot(void)
{
    P1OUT |= dot;
    __delay_cycles(500000);

    P1OUT &= ~dot;
    __delay_cycles(500000);
}

// Blinks a dash for about 1.5 seconds
void blink_dash(void)
{
    P1OUT |= G0;
    __delay_cycles(1500000);

    P1OUT &= ~G0;
    __delay_cycles(500000);
}

// ************************************ Mode 2: Random Number ********************************************//
// Select a maximum number using the button and then the LED will display
// a random number between 1 and the max.
// The program exits back to the main menu after 15 blinks of the decimal point.
void random_number(void)
{
    int random = 0, max = 1, k;
    select = 1;
    i = 0;

    clear();

    // Selects the max after ten blinks on the same number
    while (select < 10)
    {
      display(i);
      blink(DOT);

      select++;
      max = i;
    }

    // Seed the random number generator
    srand(time(NULL));

    // Play the sequence
    generating();

    clear();
    __delay_cycles(1000000);

    // Generate the random number and then display it.
    random = rand()%max + 1;

    // The random number will display for 15 blinks of the decimal point.
    for (k = 0; k < 15; k++)
    {
       display(random);
       blink(DOT);
    }

    stage = 3;
    return;
}

// Sequence helper used in the random number generator.
void generating(void)
{
    clear();

    P1OUT |= E1 | D0;
    sequence_pause();
    P1OUT &= ~E1 & ~D0;

    P2OUT |= A1 | B0;
    sequence_pause();
    P2OUT &= ~A1 & ~B0;

    P2OUT |= A0 | C1;
    sequence_pause();
    P2OUT &= ~A0 & ~C1;

    P1OUT |= G0 | B1;
    sequence_pause();
    P2OUT &= ~G0 & ~B1;

    P1OUT |= C0 | F1;
    sequence_pause();
    P2OUT &= ~C0 & ~F1;

    P2OUT |= G1 | D1;
    sequence_pause();
    P2OUT &= ~G1 & ~D1;

    P2OUT |= F0;
    P1OUT |= E0;
    sequence_pause();
    P2OUT &= ~F0;
    P1OUT &= ~E0;

    return;
}

// *************************************** Mode 3: Timer ***********************************************//
// Select an amount using the buttons and waiting for the d.p.
// to blink 10 times on the same number.
// Then select a unit using the same method. (sec, min, hour)
// 88 will flash and then the timer will start.
// When the time is up double 00 will flash.
void timer(int amount)
{
    int unit, k;
    i = 1;
    select = 0;
    num_select = 0;

    clear();

    if (amount == 0)
    {
      // Selecting the amount you want to set the timer to.
      while (select < 10)
      {
         display(i);
         blink(DOT);

         select++;
         amount = i;
      }

      // Select the units
      clear();
      __delay_cycles(1000000);

      select = 0;

      // Sets the flag used in the button ISR.
      num_select = 1;

      // s -> 18, m -> 12, h -> 7
      unit = 18;
      i = 18;
      while (select < 10)
      {
         display_letter(i);
         blink(DOT);

         select++;
         unit = i;
      }

      P1OUT |= dot;
      display(88);

      __delay_cycles(1000000);
    }

    if (mode == 5)
        unit = 18;

    // 1,000,000 cycles / 1 MHz = 1 second
    while (amount > 0)
    {
        // When on the final minute, it shows the
        // count down from 60 seconds
        if (unit == 12 && amount == 1)
        {
            amount = 60;
            unit = 18;
        }

        display(amount);

        // Seconds
       if (unit == 18)
           __delay_cycles(1000000);

       // Minutes
       else if (unit == 12)
           __delay_cycles(60000000);

       // Hours
       else if (unit == 7)
           __delay_cycles(360000000);

        amount--;
    }

    if (mode == 5)
    {
      display(0);
      __delay_cycles(500000);
      return;
    }

    // Time is up
    for (k = 0; k < 15; k++)
        blink(OO);

    stage = 3;
    return;
}

// ************************************ Mode 4: Stop Watch ********************************************//
// Time is in seconds.
// Starts when the button is pressed and pauses when the button is pressed again.
// 10 blinks and the time will go back to zero.
// 10 more blinks form that point will exit back to the main menu.
void stop_watch(void)
{
  int j;
  amount = 0;
  stop = 1;
  display(amount);

  // Wait for the button to be pressed.
  while (stop == 1){}

  while (amount < 100)
  {
      j = 0;

      // Paused
      while (stop == 1 && j < 10)
      {
         blink(amount);
         j++;
      }

      // Checks it needs to reset to zero.
      if (stop == 1 && j == 10)
          amount = 0;

      j = 0;

      // Checks if reset to zero.
      while (stop == 1 && j < 10)
      {
          blink(amount);
          j++;
      }

      // Exits the mode if zero has blinked ten times.
      if (stop == 1 && j == 10)
      {
          stage = 3;
          clear();
          return;
      }

      display(amount);
      __delay_cycles(1000000);
      amount++;
  }

  stage = 3;
  return;
}

// Stop Watch Helper that takes in a number that represents a
//letter in the alphabet, starting from 0.
// Then displays that letter to the LEDs.
void display_letter(int letter)
{
    clear();

    // S -> also a 5.
    if (letter == 18)
        display(5);

    // M -> an upper case M with top and both bottom sides.
    if (letter == 12)
    {
        P1OUT |= C0 | E0;
        P2OUT |=  A0;
    }

    // H -> a lowercase h.
    if (letter == 7)
    {
        P1OUT |= C0 | E0 | G0;
        P2OUT |= F0;
    }

    return;
}

// ************************************ Mode 5: Shot Clock ********************************************//
// Continuously acts as an NBA shot clock.
// Uses the timer function passing a 24.
void shot_clock(void)
{
  while (stage == 2)
  {
    timer(24);
  }

  return;
}

// Sequence that continuously runs when the microcontroller is first plugged in.
void start_sequence(void)
{
    i = 0;

    P2OUT |= A0 | A1;
    sequence_pause();

    P1OUT |= B1;
    P2OUT |= B0;
    sequence_pause();

    P1OUT |= C0;
    P2OUT |= C1;
    sequence_pause();

    P1OUT |= D0;
    P2OUT |= D1;
    sequence_pause();

    P1OUT |= E0 | E1;
    sequence_pause();

    P2OUT |= F0 | F1;
    sequence_pause();

    build_destroy(0);
    stage = 1;
    return;
}

// Sequence that runs once when exiting a mode and transitioning
// back to the main menu.
void end_sequence(void)
{
    clear();

    P1OUT |= C0;
    P2OUT |= B0;
    sequence_pause();

    P1OUT |= E0;
    P2OUT |= F0;
    sequence_pause();

    P1OUT |= B1;
    P2OUT |= C1;
    sequence_pause();

    P1OUT |= E1;
    P2OUT |= F1;
    sequence_pause();

    P2OUT |= A0 | A1;
    sequence_pause();

    P1OUT |= G0;
    P2OUT |= G1;
    sequence_pause();

    P1OUT |= D0;
    P2OUT |= D1;
    sequence_pause();

    build_destroy(1);

    stage = 1;
    return;
}

// Used in start and end sequences for blinking or other pauses.
void sequence_pause()
{
    int j;

    for(j = 0; j < 30000; j++);

    if (stage < 2)
        clear();

    for(j = 0; j < 30000; j++);
}

// 0 - build the segments up from nothing to 8.
// 1 - destroy the segments from 8 to nothing.
void build_destroy (int control)
{
    int j;

    // Build
    if (control == 0)
    {
        P2OUT |= A0 | A1;
        for(j = 0; j < 30000; j++);

        P1OUT |= B1;
        P2OUT |= B0;
        for(j = 0; j < 30000; j++);

        P1OUT |= C0;
        P2OUT |= C1;
        for(j = 0; j < 30000; j++);

        P1OUT |= D0;
        P2OUT |= D1;
        for(j = 0; j < 30000; j++);

        P1OUT |= E0 | E1;
        for(j = 0; j < 30000; j++);

        P2OUT |= F0 | F1;
        for(j = 0; j < 30000; j++);

        P1OUT |= G0;
        P2OUT |= G1;
    }

    // Destroy
    else if (control == 1)
    {
        P2OUT &= ~A0 & ~A1;
        for(j = 0; j < 30000; j++);

        P1OUT &= ~B1;
        P2OUT &= ~B0;
        for(j = 0; j < 30000; j++);

        P1OUT &= ~C0;
        P2OUT &= ~C1;
        for(j = 0; j < 30000; j++);

        P1OUT &= ~D0;
        P2OUT &= ~D1;
        for(j = 0; j < 30000; j++);

        P1OUT &= ~E0 & ~E1;
        for(j = 0; j < 30000; j++);

        P2OUT &= ~F0 & ~F1;
        for(j = 0; j < 30000; j++);

        P1OUT &= ~G0;
        P2OUT &= ~G1;
    }

    __delay_cycles(2000000);
}

// Displays the number passed, to the LEDs.
void display(int x)
{
    int number = x;
    clear();

    // Isolates the LSB
    number = number % 10;

    // Single digit numbers
    if (number == 0)
    {
        P1OUT |= C0 | D0 | E0;
        P2OUT |= A0 | B0 | F0;
    }

    else if (number == 1)
    {
        P1OUT |= C0;
        P2OUT |= B0;
    }

    else if (number == 2)
    {
     P1OUT |= D0 | E0| G0;
     P2OUT |= A0 | B0;
    }

    else if (number == 3)
    {
      P1OUT |= C0 | D0 | G0;
      P2OUT |= A0 | B0;
    }

    else if (number == 4)
    {
        P1OUT |= C0 | G0;
        P2OUT |= B0 | F0;
    }

    else if (number == 5)
    {
        P1OUT |= C0 | D0 | G0;
        P2OUT |= A0 | F0;
    }

    else if (number == 6)
    {
        P1OUT |= C0 | D0 | E0 | G0;
        P2OUT |= A0 | F0;
    }

    else if (number == 7)
    {
        P1OUT |= C0;
        P2OUT |= A0 | B0;
    }

    else if (number == 8)
    {
        P1OUT |= C0 | D0 | E0 | G0;
        P2OUT |= A0 | B0 | F0;
    }

    else if (number == 9)
    {
        P1OUT |= C0 | D0 | G0;
        P2OUT |= A0 | B0 | F0;
    }

    if (x < 10)
        return;

    // Handles double digit numbers
    else
    {
       number = x;

       // Isolates the MSB
       number = number / 10;

       if (number == 0)
       {
           P1OUT |= C1 | D1 | E1;
           P2OUT |= A1 | B1 | F1;
       }

       else if (number == 1)
       {
           P1OUT |= B1;
           P2OUT |= C1;
       }

       else if (number == 2)
       {
          P1OUT |= B1 | E1;
          P2OUT |= A1 | D1 | G1;
       }

       else if (number == 3)
       {
          P1OUT |= B1;
          P2OUT |= A1 | C1 | D1 | G1;
       }

       else if (number == 4)
       {
           P1OUT |= B1;
           P2OUT |= C1 | F1 | G1;
       }

       else if (number == 5)
       {
           P2OUT |= A1 | C1 | D1 | F1 | G1;
       }

       else if (number == 6)
       {
           P1OUT |= E1;
           P2OUT |= A1 | C1 | D1 | F1 | G1;
       }

       else if (number == 7)
       {
           P1OUT |= B1;
           P2OUT |= A1 | C1;
       }

       else if (number == 8)
       {
           P1OUT |= B1 | E1;
           P2OUT |= A1 | C1 | D1 | F1 | G1;
       }

       else if (number == 9)
       {
           P1OUT |= B1;
           P2OUT |= A1 | C1 | D1 | F1 | G1;
       }
    }
    return;
}

// Clears all of the segments on both LEDs.
// Does not clear the decimal point.
void clear(void)
{
    P1OUT &= ~C0 & ~D0 & ~E0 & ~G0 & ~B1 & ~E1;
    P2OUT &= ~A0 & ~B0 & ~F0 & ~A1 & ~C1 & ~D1 & ~F1 & ~G1;
    return;
}

// Blinks what ever is passed once.
// DOT(-1) is passed when the decimal point is to bo blinked.
// OO(-100) is passed when double zero is to be blinked.
void blink(int x)
{
    int j;

    // Decimal Point
    if (x == DOT)
        P1OUT |= dot;

    // Double Zero
    else if (x == OO)
    {
        display(0);

        // The MSB 0 is not supported in the display function.
        P1OUT |= B1 | E1 | dot;
        P2OUT |= A1 | C1 | D1 | F1;
    }

    // Number Passed
    else
        display(x);

    for (j = 0; j < 30000; j++);
    for (j = 0; j < 30000; j++);

    if (x == DOT)
        P1OUT &= ~dot;

    else if (x == OO)
    {
        clear();
        P1OUT &= ~dot;
    }

    else
        clear();

    for (j = 0; j < 30000; j++);
    for (j = 0; j < 30000; j++);

    return;
}

//******* Button ISR **********//
// Button debouncing with the timer
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR()
{
    // Clear the timer
    TA0CCTL0 |= TACLR;

    // Clear the timer interrupt flag
    TA0CCTL0 &= ~CCIFG;

    // Enable timer interrupts
    TA0CCTL0 |= CCIE;

    // Clear the button interrupt flag
    P1IFG &= ~button;
}

//******* Timer ISR **********//
// Used for debouncing and various other actions
// depending on the stage and mode the program is in.
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR()
{
    // Use polling to check if the button is still pressed
    // Then procedes based on the stage that the program is in.
    if((P1IN & button) == 0)
    {
        // Move program out of start sequence.
       if (stage == 0)
       {
          stage = 1;
       }

       // Increase the value displayed and change the mode that is being selected.
       // Resets the select variable so it it will blink 10 times when a mode is changed.
       else if (stage == 1)
       {
           // Stage is updated to 2 when the mode is selected in the main menu function.
           i++;
           select = 0;
       }

       // Clears the entire display and moves the program on to stage three.
       else if (stage == 2)
       {
           // Increases the value being displayed.
           if (mode == 0)
           {
               i++;
               display(i);

               if (i > 99)
               {
                   clear();
                   stage = 3;
               }
            }

           else if(mode == 1)
           {
               stage = 3;
           }

           // Increases the max number for the random number generator.
           else if (mode == 2)
           {
               stage = 2;

               i++;
               if (i > 99)
                 i = 0;

               display(i);
               select = 0;
           }

           // Increases the amount to set the timer
           // or cycles through the different time units.
           else if (mode == 3)
           {
              stage = 2;

              // For selecting the unit
              if (num_select == 1)
              {
                  // Change to minutes
                  if (i == 18)
                      i = 12;

                  // Change to hours
                  else if (i == 12)
                      i = 7;

                  // Change to seconds
                  else if (i == 7)
                      i = 18;

                  display_letter(i);
              }

              // For selecting the amount
              else
              {
                 i++;
                 if (i > 99)
                   i = 0;

                 display(i);
              }

              select = 0;
          }

          else if (mode == 4)
          {
            // Start the stop watch.
            if (stop == 1)
              stop = 0;

            // Stop the stop watch.
            else if (stop == 0)
              stop = 1;
          }

          else if (mode == 5)
          {
              stage = 3;
          }

          // Far any mode that has not been written yet
          // Just clear the board and go back to the main menu.
          else if (mode > 5)
          {
              clear();
              stage = 3;
          }
       }
    }

   // Disable the timer interrupt
   TA0CCTL0 &= ~CCIE;

   // Re-enable the button interrupt and clear the flag
   P1IE |= button;
   P1IFG &= ~button;

   // Hardware clears the timer interrupt flag.
}
