
#include <SPI.h>

byte address = 0x00;
int CS= 10;
// remote control commands for Pioneer
//values are in ohms
const int VOL_UP=16000;
const int VOL_DN=24000;
const int PREV_TR=11250;
const int NEXT_TR=8000;
const int MODE=1200;
const int MUTE=6;

int csPin=10; // digipot pin

int wheelPin=A0; // steering wheel resistance reading pin

int i=0;
int prevButton=0;

void setup() {                
  pinMode(csPin, OUTPUT);   
  pinMode(13, OUTPUT);
  delay(50);
  blink(13, 3, 100);//bling 3 times adter boot
  SPI.begin();
  delay(50);
  SPI.transfer(0); // command
  SPI.transfer(0); // value
  
  pinMode(wheelPin, INPUT);

  delay(100);
  Serial.begin(9600); 
}


int getR() { // this function returns pressed button code or 0 if all buttons are released

  // read resistance value from the steering wheel buttons
  int r=analogRead(wheelPin);
  //Serial.print("ADC value:");Serial.println(r);
  //Serial.print("Resistor value:");Serial.println(1000*(1/(5/(float(r)/1024)-1)));
  // below values are for Honda Civic steering wheel controls and 1 kOhms known resistor 
  // honda R calues are: 
  //Vol_up 370
  //vol down 100
  //prev track 840
  //next track 2000
  //mode 6200
  // we will get a voltage reading from the voltage divider
  // to be safe we use ranges around the values we expec to measure
  if (r>=838) return(VOL_DN);//above 4.09 volt //930=100 ohm
  if (r>=651 && r<=837) return(VOL_UP);//between 4.08 and 3.18 volt //749=360 ohm
  if (r>=448 && r<=650) return(PREV_TR);//between 3.17 and 2.19// 528=888 ohm
  if (r>=244 && r<=447) return(NEXT_TR);//between 2.18 and 1.19//344 = 2000ohm
  if (r>=120 && r<=244) return(MODE);//between 1.18 and 0.58 //133=6700 ohm
//  if (r>=2 && r<=6) return(MUTE); 
  return (0);//below 0.58. we have a 10kOhm paralell resitor in the Honda//93=10kohm
}
void blink(int pin, int times, int delaytime)
{
  for (i=0; i<times; i++)
  {
       digitalWrite(pin, HIGH);
      delay(delaytime);
      digitalWrite(pin,LOW);
            delay(delaytime);
    }
  }

void loop() {
  int currButton=getR(); // get current pressed button code
  if (currButton!=prevButton) { // if it has changed since last reading
    delay(10);
    currButton=getR(); // wait 10ms and read again to make sure this is not just some noise
    if (currButton!=prevButton) { 
      Serial.println(currButton);
      prevButton=currButton; 
      //button pressed. LEts bink
      blink(13, 2, 100);
      // send command to car stereo, below values are for JVC head units
      //SPI.transfer(0); //address byte. for 4131 the wiper's address is 0 
      switch(currButton) {//this determines the value sent to the above address round(expected resistance/(max resistance/wiper steps),0))
       case VOL_UP: digitalPotWrite(20); break;  // round(16000/(100000/127),0)
       case VOL_DN: digitalPotWrite(30); break;  // round(24000/(100000/127),0)    
       case PREV_TR: digitalPotWrite(14); break;  // round(11250/(100000/127),0)    
       case NEXT_TR: digitalPotWrite(10); break;  // round(8000/(100000/127),0)    
       case MODE: digitalPotWrite(2); break;  // round(1200/(100000/127),0)    
       //case MUTE: SPI.transfer(int (100000/127*MUTE)); break;  // 3k
       default: digitalPotWrite(0);       blink(13, 1, 100);break; // nothing. Blink a second time on button release
     }
   }
  }
  delay(100);
}

void digitalPotWrite(int value)
{
  Serial.println(value);
digitalWrite(CS, LOW);
SPI.transfer(address);
SPI.transfer(value);
digitalWrite(CS, HIGH);
}
