#include <Timers.h>

// This code turns on and off two LEDs at two different rates.
// This code should work on both Arduino and chipKIT platforms. 
//
// This example comes from the making a library project on http://learn.digilentinc.com/

// Instantiate the Timers class
Timers timers;

const int LED1 = 6;
const int LED2 = 7;

void setup()
{
  pinMode(LED,OUTPUT);
  // Add the timers (functions to be run), with the period
  // default resolution is milliseconds
  timers.addTimer(900, toggleLED1);
  timers.addTimer(500, toggleLED2);
}

void loop()
{
  // This method is needed for the timers to work.
  timers.update();
}

void toggleLED1()
{
  static int state = LOW;
  state = !state;
  digitalWrite(LED1, state);
}

void toggleLED2()
{
  static int state = LOW;
  state = !state;
  digitalWrite(LED2, state);
}
