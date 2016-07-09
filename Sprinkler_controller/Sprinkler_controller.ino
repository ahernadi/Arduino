#include <EEPROM.h> // For saving the schedule
#include <Wire.h>
#include "RTClib.h"
/*
Adafruit Arduino - Lesson 4. 8 LEDs and a Shift Register
*/

int latchPin = 5;
int clockPin = 6;
int dataPin = 4;
int relayEnable=7;

//byte leds = 255;//if a bit is 0 the relay is active
//127 = 01111111. The first realy is active
/*
relays 11111111 255 - all off
relays 01111111 127 - Main Pump start solenoid engaged 
relays 01111101 125 - 6th
relays 01111011 123 - 5th
relays 01110111 119 - 4th
relays 01101111 111 - 3rd
relays 01011111 95 - second
relays 00111111 63 - 1st realy open

*/
RTC_DS3231 rtc;
DateTime now;
//zone to shift register mapping
#define unsigned long wait=18000000 
//18000000=5 hours

byte off = 255;
byte zone1 = 63;
byte zone2 = 95;
byte zone3 = 111;
byte zone4 = 119;
byte zone5 = 123;
byte zone6 = 125;
byte pumpstart=127;
//byte zone7 = 1 << 6;
//byte zone8 = 1 << 7;

typedef struct {
  byte deleted;
  byte enabled;
  byte days; // 1 = Everyday, 2=Even Days, 3=Odd Days
  byte hour; // 0 - 23
  byte minute; // 0 - 59
  byte zone; // 0 - 8
} 
Schedule;

int workingScheduleNumber = 0;
Schedule workingSchedule;
#define SCHEDULELISTSIZE 8
Schedule scheduleList[SCHEDULELISTSIZE];




// State variables
int currentZone = 0;
int lastMinute = 0;
//byte shellState = 0;
unsigned long startTime = 0; // Unixtime of power-up or last reset
unsigned long lastTimeCheck = 0;

void initializeSchedule() {
  for (int i = 0; i < SCHEDULELISTSIZE; i++) {
    scheduleList[i] = (Schedule){ 1,0,1,0,0,0 };
  }
}

void loadSchedule() {
  initializeSchedule();
  // Load schedule from EEPROM
  int addr = 0;

  for (int i = 0; i < SCHEDULELISTSIZE; i++) {
    scheduleList[i].deleted = EEPROM.read(addr++);
    scheduleList[i].enabled = EEPROM.read(addr++);
    scheduleList[i].days = EEPROM.read(addr++);
    scheduleList[i].hour = EEPROM.read(addr++);
    scheduleList[i].minute = EEPROM.read(addr++);
    scheduleList[i].zone = EEPROM.read(addr++);
  }
}

void saveSchedule() {
  // Save schedule to EEPROM
  int addr = 0;

  for (int i = 0; i < SCHEDULELISTSIZE; i++) {
    EEPROM.write(addr++, scheduleList[i].deleted);
    EEPROM.write(addr++, scheduleList[i].enabled);
    EEPROM.write(addr++, scheduleList[i].days);
    EEPROM.write(addr++, scheduleList[i].hour);
    EEPROM.write(addr++, scheduleList[i].minute);
    EEPROM.write(addr++, scheduleList[i].zone);
  }
}
void setup() 
{ scheduleList[0]= (Schedule){0,1,1,1,1,59,7};
scheduleList[1]= (Schedule){0,1,1,1,2,0,1};
scheduleList[2]= (Schedule){0,1,1,1,2,45,2};
scheduleList[3]= (Schedule){0,1,1,1,3,0,3};
scheduleList[4]= (Schedule){0,1,1,1,3,15,4};
scheduleList[5]= (Schedule){0,1,1,1,4,0,5};
scheduleList[6]= (Schedule){0,1,1,1,4,15,6};
scheduleList[7]= (Schedule){0,1,1,1,4,30,0};
  saveSchedule();
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(relayEnable, OUTPUT);
  digitalWrite(relayEnable,LOW);//the state of the shift register is unknown. Lets power off all relays until we initialize
  //turn off all relays
  delay(1000); //lets wait a sec to avoid interference with all relays turning off
  shiftOut(dataPin, clockPin, LSBFIRST, off);
    Serial.begin (9600); 
delay(1000);
  digitalWrite(relayEnable,HIGH);//the state of the shift register is now known. we can power the relays back on
  Serial.print(hour());
  Serial.print(minute());
  Serial.print(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
//loadSchedule()

/*  leds = 127;//all off except first
  updateShiftRegister();
  delay(1000);
  for (int i = 1; i < 7; i++)//
  {leds = 127;
    bitWrite(leds, i,0);
    updateShiftRegister();
    delay(1000);
  }
  leds=255; //turn all off
 updateShiftRegister();  */
 unsigned long startMillis = millis();
while (millis() - startMillis < wait);//wait to start he cycle after a reset
  Serial.print(hour());
  Serial.print(minute());
  Serial.print(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void loop() 
{

    if (millis() > lastTimeCheck + 1000 || millis() < lastTimeCheck) {
    now = RTC.now();
    lastTimeCheck = millis();
  }

 if (lastMinute != now.minute()) {
    lastMinute = now.minute();

    // Figure out if it's an even or odd day. (use number of days since the unix epoch, not the current day of the month for a better even/odd pattern regardless of months with an odd number of days)
    boolean isEvenDay = ((now.unixtime() / 86400L) % 2 == 0);

    for (int i = 0; i < SCHEDULELISTSIZE; i++) {
      // TODO: this is a nasty looking if statement. Need to make it easier to read.
      if (scheduleList[i].deleted == 0 && // Not deleted
          scheduleList[i].enabled == 1 && // Is Enabled
          scheduleList[i].hour == now.hour() && // Is the current hour
          scheduleList[i].minute == now.minute() && // Is the current minute
          (
           (scheduleList[i].days == 1) || // This is an every-day schedule
           (scheduleList[i].days == 2 && isEvenDay) || // This is an even-day schedule, and today is an even day.
           (scheduleList[i].days == 3 && !isEvenDay) // This is an odd-day schedule, and today is an odd day.
          )
         ) {
        waterZone(scheduleList[i].zone);
      }
    }
  }
  }

}


// Helper function that sets the zone on or off. Takes an integer of the zone instead of the byte.
void waterZone(int zone) {
  currentZone = zone;
  switch (zone) {
  case 0:
    water(off);
    break;
  case 1:
    water(zone1);
    break;
  case 2:
    water(zone2);
    break;
  case 3:
    water(zone3);
    break;
  case 4:
    water(zone4);
    break;
  case 5:
    water(zone5);
    break;
  case 6:
    water(zone6);
    break;
  case 7:
    water(pumpstart);
    break;
  case 8:
    water(off);
    break;
  }
}


void water(byte zone) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, zone);  
  digitalWrite(latchPin, HIGH);
}
