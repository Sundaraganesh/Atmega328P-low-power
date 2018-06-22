// Code of implementing power down mode
// 
// Author: Sundaraganesh Rengarajan
// Date:   15 June 2018


#include <avr/sleep.h>
#include <avr/wdt.h>

const byte LED = 9;
  
// watchdog interrupt
ISR (WDT_vect) 
{
   wdt_disable();  // disable watchdog
}  // end of WDT_vect
 
void setup () { }

void WatchDogDelay(const byte time_delay)
{
  // disable ADC
  ADCSRA = 0;  

  // clear various "reset" flags
  MCUSR = 0;     
  // allow changes, disable reset
  WDTCSR = bit (WDCE) | bit (WDE);
  // set interrupt mode and an interval 
  WDTCSR = bit (WDIE) | time_delay;    // set WDIE, to have a proper time_delay
  wdt_reset();  // pat the dog
  
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);  
  noInterrupts ();           // timed sequence follows
  sleep_enable();
 
  // turn off brown-out enable in software
  MCUCR = bit (BODS) | bit (BODSE);
  MCUCR = bit (BODS); 
  interrupts ();             // guarantees next instruction executed
  sleep_cpu ();   
  
  // cancel sleep as a precaution
  sleep_disable();
}

void loop () 
{
  // Region to write the work code

  //Currently blinking of LED
  pinMode (LED, OUTPUT);
  digitalWrite (LED, HIGH);
  delay (1000);
  digitalWrite (LED, LOW);
  pinMode (LED, INPUT);

  //End of work code

  int loop_times;
  for(loop_times = 0; loop_times < 2; loop_times++)    // 68 cycles is roughly 9 mins as each cycle is 8 secs 
  {
    WatchDogDelay(0b100001);
  }  
  
} // end of loop
