
#include <SPI.h>

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

  SPI.begin();
  delay(50);
  SPI.transfer(0); // command
  SPI.transfer(0); // value
  
  pinMode(wheelPin, INPUT);

  delay(100);
  //Serial.begin(9600); 
}


int getR() { // this function returns pressed button code or 0 if all buttons are released

  // read resistance value from the steering wheel buttons
  int r=analogRead(wheelPin);
  //Serial.println(r);
 
  // below values are for Honda Civic steering wheel controls and 1 kOhms known resistor 
  // honda R calues are: 
  //Vol_up 370
  //vol down 100
  //prev track 840
  //next track 2000
  //mode 6200
  // we will get a voltage reading from the voltage divider
  // to be safe we use ranges around the values we expec to measure
  if (r>=838) return(VOL_UP);//above 4.09 volt
  if (r>=651 && r<=837) return(VOL_DN);//between 4.08 and 3.18 volt
  if (r>=448 && r<=650) return(PREV_TR);//between 3.17 and 2.19
  if (r>=244 && r<=447) return(NEXT_TR);//between 2.18 and 1.19
  if (r>=120 && r<=244) return(MODE);//between 1.18 and 0.58
//  if (r>=2 && r<=6) return(MUTE); 
  return (0);//below 0.58. we have a 10kOhm paralell resitor in the Honda
}

void loop() {
  int currButton=getR(); // get current pressed button code
  if (currButton!=prevButton) { // if it has changed since last reading
    delay(10);
    currButton=getR(); // wait 10ms and read again to make sure this is not just some noise
    if (currButton!=prevButton) { 
      //Serial.println(currButton);
      prevButton=currButton; 
      //button pressed. LEts bink
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13,LOW);
      // send command to car stereo, below values are for JVC head units
      SPI.transfer(0); //address byte. for 4131 the wiper's address is 0 
      switch(currButton) {//this determines the value sent to the above address round(expected resistance/(max resistance/wiper steps),0))
       case VOL_UP: SPI.transfer(20); break;  // round(16000/(100000/127),0)
       case VOL_DN: SPI.transfer(30); break;  // round(24000/(100000/127),0)    
       case PREV_TR: SPI.transfer(15); break;  // round(11250/(100000/127),0)    
       case NEXT_TR: SPI.transfer(10); break;  // round(8000/(100000/127),0)    
       case MODE: SPI.transfer(2); break;  // round(1200/(100000/127),0)    
       //case MUTE: SPI.transfer(int (100000/127*MUTE)); break;  // 3k
       default: SPI.transfer(0); break; // nothing
     }
   }
  }
  delay(100);
}

