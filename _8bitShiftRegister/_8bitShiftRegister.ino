/*
Adafruit Arduino - Lesson 4. 8 LEDs and a Shift Register
*/

int latchPin = 5;
int clockPin = 6;
int dataPin = 4;
int relayEnable=7;

byte leds = 255;//if a bit is 0 the relay is active
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



void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(relayEnable, OUTPUT);
  digitalWrite(relayEnable,LOW);//the state of the shift register is unknown. Lets power off all relays until we initialize
  //turn off all relays
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
    Serial.begin (9600); 
delay(1000);
  digitalWrite(relayEnable,HIGH);//the state of the shift register is unknown. Lets power off all relays until we initialize

  leds = 127;//all off except first
  updateShiftRegister();
  delay(1000);
  for (int i = 1; i < 7; i++)//
  {leds = 127;
    bitWrite(leds, i,0);
    updateShiftRegister();
    delay(1000);
  }
  leds=255; //turn all off
 updateShiftRegister();  
}

void loop() 
{

}

void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
        Serial.print("relays ");
     Serial.println(leds, BIN);

   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}
