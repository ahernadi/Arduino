// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;
int speaker =1;

void setup(void) {
  //Serial.begin(9600);
  ////Serial.println(F("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  ////Serial.println(F("Using Adafruit 2.8\" TFT Arduino Shield Pinout"));
#else
  ////Serial.println(F("Using Adafruit 2.8\" TFT Breakout Board Pinout"));
#endif

  //Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); //Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();

  if(identifier == 0x9325) {
   //// //Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9327) {
    ////Serial.println(F("Found ILI9327 LCD driver"));
  } else if(identifier == 0x9328) {
    ////Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    //Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    //Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    //Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier == 0x0154) {
    //Serial.println(F("Found S6D0154 LCD driver"));
  } else {
    //Serial.print(F("Unknown LCD driver chip: "));
    //Serial.println(identifier, HEX);
    //Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    //Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    //Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    //Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    //Serial.println(F("Also if using the breakout, double-check that all wiring"));
    //Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);
      tft.setRotation(tft.getRotation()+4);

    // Serial.print(F("Rotation:"));//Serial.println(tft.getRotation());
    tft.fillScreen(BLACK);
  //  tft.setRotation(tft.getRotation()+2);
  //  tft.setRotation(tft.getRotation()+1);
    // Serial.print(F("Rotation:"));//Serial.println(tft.getRotation());
   
    //try tone
    tone(speaker, 1047, 1000/4);
    delay(1000/4*1.3);
    // stop the tone playing:
    noTone(speaker);
    //y1,x1,y2,x2
    
//tft.fillRect(0, 280, 240, 20, WHITE); 
int xmax=0;
int ymax=0;
//Draw face
 tft.fillCircle(120, 120, 100, YELLOW); //head
 tft.fillCircle(118, 120, 80, BLACK);//mouth
 tft.fillCircle(112, 120, 81, YELLOW);//mouth
 tft.fillCircle(90, 150, 20, BLACK);//left eye
 tft.fillCircle(90, 90, 20, BLACK);//right eye
//    tft.setRotation(tft.getRotation()+2);

bool isBlink;
int i=0;
isBlink=false;
for (i=0;i<8;i++)
{tft.setRotation(3);
  tft.setCursor(50, 0);
  tft.setTextColor(WHITE);  tft.setTextSize(5);
  tft.println(i+1);
  tft.setRotation(4);
progress(240, 45, (isBlink?BLACK:WHITE));
isBlink?openeye():closerighteye();
isBlink=!isBlink;
//progress(240, 45,BLACK);
//openeye();
//progress(240, 45,WHITE);
//tone
tone(speaker, 1047, 1000/4);
    delay(1000/4*1.3);
    // stop the tone playing:
    noTone(speaker);
tone(speaker, 1047, 1000/4);
    delay(1000/4*1.3);
    // stop the tone playing:
    noTone(speaker);
    tft.setRotation(3);
    tft.setCursor(50, 0);
  tft.setTextColor(BLACK);  tft.setTextSize(5);    
  tft.println(i+1);
  tft.setRotation(4);
}
for (i=0;i<4;i++)
{tft.setRotation(3);
  tft.setCursor(50, 0);
  tft.setTextColor(YELLOW);  tft.setTextSize(5);
  tft.println(i+1);
  tft.setRotation(4);
progress(240, 20, (isBlink?BLACK:WHITE));
isBlink?openeye():closerighteye();
isBlink=!isBlink;
//progress(240, 45,BLACK);
//openeye();
//progress(240, 45,WHITE);
tone(speaker, 1047, 1000/4);
    delay(1000/4*1.3);
    // stop the tone playing:
    noTone(speaker);
tone(speaker, 1047, 1000/4);
    delay(1000/4*1.3);
    // stop the tone playing:
    noTone(speaker); 
        tft.setRotation(3);
    tft.setCursor(50, 0);
  tft.setTextColor(BLACK);  tft.setTextSize(5);    
  tft.println(i+1);
  tft.setRotation(4);   
}
 tft.fillScreen(BLACK);//shutdown
tone(speaker, 1047, 1000/4);
    delay(1000/4*1.3);
    // stop the tone playing:
    noTone(speaker);
    tone(speaker, 1047, 1000/4);
    delay(1000/4*1.3);
    // stop the tone playing:
    noTone(speaker);
    tone(speaker, 1047, 1000/4);
    delay(1000/4*1.3);
    // stop the tone playing:
    noTone(speaker);
 }

void loop() {
  // put your main code here, to run repeatedly:
//delay(1000);
//blink();
}
void progress (int length, int seconds, int color)
{int i, increment;
 increment=(length-2)/seconds;
 //Serial.print("increment:");//Serial.println(increment);
 //Serial.print("increment*seconds+1:");//Serial.println(increment*seconds+1);
  tft.drawRect(0, 279, increment*seconds+2,22 , WHITE);
  //tft.drawRect(1, 280, increment*seconds,20 , WHITE);
  for (i=0; i<seconds; i++)
  {
tft.fillRect( i*increment+1,280, increment,20, color); 
delay(1000);
}
}
void closerighteye()
{//close eye
  tft.fillCircle(85, 90, 20, YELLOW);//right eye blink

}
void openeye()
{
//open/redraw right eye
 tft.fillCircle(90, 90, 20, BLACK);//right eye

}
//void blink()
//{ 
 //close eye
 // tft.fillCircle(85, 90, 20, YELLOW);//right eye blink

// delay(1000);
//open/redraw right eye
// tft.fillCircle(90, 90, 20, BLACK);//right eye

//}





