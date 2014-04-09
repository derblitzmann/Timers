#include <Timers.h>

// This code turns on and off an on-board LED at a 2 second period.
// This code should work on both Arduino and chipKIT platforms. 
//
// This example comes from the making a library project on http://learn.digilentinc.com/

// Instantiate the Timers class
Timers timers;

const int LED = 13;

int state = LOW;

void setup()
{
  pinMode(LED,OUTPUT);
  // Add a timer to toggle the on board LED very second 
  timers.addTimer(1000, toggleLED);
}

void loop()
{
  // This method is needed for the timers to work.
  timers.update();
}

void toggleLED()
{
  // toggle the state
  state = !state;
  digitalWrite(LED,state);
}
