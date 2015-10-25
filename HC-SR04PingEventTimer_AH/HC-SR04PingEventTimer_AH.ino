/*
Arduino Uno HC-SR04 ultrasonic range finder program using interrupts 
Author:  Steve Garratt 
Date:    19/10/13
 
Interrupts from timer 1 are used to schedule and deliver the sensor
trigger pulse. The same interrupt is used to control the flash rate
of the onboard LED indicating distance.

The duration of the sensor echo signal is measured by generating an
external interrupt ervery time the echo signal changes state.

This sketch uses the TimerOne library downloadable from here:
http://code.google.com/p/arduino-timerone/downloads/detail?name=TimerOne-v9.zip&can=2&q=

Install the library using the following guide:
http://arduino.cc/en/Guide/Libraries
*/

//#include <TimerOne.h>                                 // Header file for TimerOne library

#define trigPin 12                                    // Pin 12 trigger output
#define echoPin 2                                     // Pin 11 Echo input
#define onBoardLED 13                                 // Pin 13 onboard LED
#define redLed 8
#define blueled 9
#define greenled 11
#define echo_int 0                                    // Interrupt id for echo pulse

//#define TIMER_US 1000000                                   // in  uS timer duration 
//#define TICK_COUNTS 100                              // 200 mS worth of timer ticks

volatile long echo_start = 0;                         // Records start of echo pulse 
volatile long echo_end = 0;                           // Records end of echo pulse
volatile long echo_duration = 0;                      // Duration - difference between end and start
//volatile int trigger_time_count = 0;                  // Count down counter to trigger pulse time
//volatile long range_flasher_counter = 0;              // Count down counter for flashing distance LED


// ----------------------------------
// setup() routine called first.
// A one time routine executed at power up or reset time.
// Used to initialise hardware.
// ----------------------------------
void setup() 
{
  pinMode(trigPin, OUTPUT);                           // Trigger pin set to output
  pinMode(echoPin, INPUT);                            // Echo pin set to input
  pinMode(onBoardLED, OUTPUT);                        // Onboard LED pin set to output
  pinMode(redLed, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(blueled, OUTPUT);
  digitalWrite( redLed, 1);
  digitalWrite( greenled, 1);
  digitalWrite( blueled, 1);
           
  //common anode led so need to drive pins high to turn them off
  
  //Timer1.initialize(TIMER_US);                        // Initialise timer 1
  //Timer1.attachInterrupt( timerIsr );                 // Attach interrupt to the timer service routine 
  attachInterrupt(echo_int, echo_interrupt, CHANGE);  // Attach interrupt to the sensor echo input
  Serial.begin (9600);                                // Initialise the serial monitor output
}

// ----------------------------------
// loop() Runs continuously in a loop.
// This is the background routine where most of the processing usualy takes place.
// Non time critical tasks should be run from here.
// ----------------------------------
void loop()
{
    //Serial.println(echo_duration);    // Print the distance in centimeters
    //Serial.println(echo_end);
    //Serial.println(echo_start);
    delay(1000);                                       // every 1 S //we can power down here
    trigger_pulse(); 
     Serial.print("distance ");
    Serial.println(echo_duration/58);
    if (echo_duration/58>100) 
    {digitalWrite( redLed, 0);
      digitalWrite( onBoardLED, 1);
      Serial.print("Red");
    delay(250);
     digitalWrite( onBoardLED, 0);
     digitalWrite( redLed, 1);
         }
    else
    {digitalWrite( onBoardLED, 1);
      digitalWrite( greenled, 0);
      Serial.print("green");
    delay(250);
    digitalWrite( greenled, 1);
    digitalWrite( onBoardLED, 0);
       }
    delay(1000);
    
}

// --------------------------
// timerIsr() 50uS second interrupt ISR()
// Called every time the hardware timer 1 times out.
// --------------------------
//void timerIsr()
//{
//    Serial.println("timer interrupt");
//    trigger_pulse();                                 // Schedule the trigger pulses
    //distance_flasher();                              // Flash the onboard LED distance indicator
//}

// --------------------------
// trigger_pulse() called every 50 uS to schedule trigger pulses.
// Generates a pulse one timer tick long.
// Minimum trigger pulse width for the HC-SR04 is 10 us. This system
// delivers a 50 uS pulse.
// --------------------------
void trigger_pulse()
{

           digitalWrite(trigPin, HIGH);              // Set the trigger output high
           delay(150);
           digitalWrite(trigPin, LOW);
           //Serial.println("trigger LOW");
        
}

// --------------------------
// echo_interrupt() External interrupt from HC-SR04 echo signal. 
// Called every time the echo signal changes state.
//
// Note: this routine does not handle the case where the timer
//       counter overflows which will result in the occassional error.
// --------------------------
void echo_interrupt()
{   // Serial.println(digitalRead(echoPin));

  switch (digitalRead(echoPin))                     // Test to see if the signal is high or low
  {
    case HIGH:                                      // High so must be the start of the echo pulse
      echo_end = 0;                                 // Clear the end time
      echo_start = micros();                        // Save the start time
      break;
      
    case LOW:                                       // Low so must be the end of the echo pulse
      echo_end = micros();                          // Save the end time
      echo_duration = echo_end - echo_start;        // Calculate the pulse duration
      break;
  }
}

