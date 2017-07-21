/*
Adafruit Arduino - Lesson 4. 8 zone and a Shift Register
*/

int latchPin = 5;
int clockPin = 6;
int dataPin = 4;
int relayEnable=7;

byte zone = 255;//if a bit is 0 the relay is active
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
int schedule[8][2]={
{127,1},
{63,45},
{95,15},
{111,15},
{119,45},
{123,15},
{125,15},
{255,1}
};
/*schedule[0]={127,1};
schedule[1]={63,45};
schedule[2]={95,15};
schedule[3]={111,15};
schedule[4]={119,45};
schedule[5]={123,15};
schedule[6]={125,15};
schedule[7]={255,1};
*/


void setup() 
{  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(relayEnable, OUTPUT);
  digitalWrite(relayEnable,LOW);//the state of the shift register is unknown. Lets power off all relays until we initialize
  //turn off all relays
  shiftOut(dataPin, clockPin, LSBFIRST, zone);
    Serial.begin (9600); 
delay(1000);
  digitalWrite(relayEnable,HIGH);//the state of the shift register is unknown. Lets power off all relays until we initialize

 
   
}

void loop() 
{
for (int i=0;i<8;i++)
 { Serial.print("Schedule ");Serial.print(schedule[i][0]);Serial.print(" "); Serial.println(schedule[i][1]);
  zone = schedule[i][0];//all off except first
  Serial.println(zone, BIN);
  updateShiftRegister();
   Serial.print("Will water for ");Serial.print(schedule[i][1]);Serial.print(" minutes "); Serial.print((long)1000*60* schedule[i][1]);Serial.println(" millisec ");
  delay((long)1000*60* schedule[i][1]);
 }
 delay((long)1000*60* 4170);//wait 21.5 hours (24*3-2.5)
}

void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
        Serial.print("relays ");
     Serial.println(zone, BIN);

   shiftOut(dataPin, clockPin, LSBFIRST, zone);
   digitalWrite(latchPin, HIGH);
}
