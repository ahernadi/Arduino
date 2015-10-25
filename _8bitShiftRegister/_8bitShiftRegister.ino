/*
Adafruit Arduino - Lesson 4. 8 LEDs and a Shift Register
*/

int latchPin = 5;
int clockPin = 6;
int dataPin = 4;
int relayEnable=7;

byte leds = 255;

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

}

void loop() 
{
  leds = 127;
  updateShiftRegister();
  delay(500);
  for (int i = 1; i < 7; i++)
  {leds = 127;
    bitWrite(leds, i,0);
    updateShiftRegister();
    delay(500);
  }
}

void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
        Serial.print("relays ");
     Serial.println(leds, BIN);

   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}
