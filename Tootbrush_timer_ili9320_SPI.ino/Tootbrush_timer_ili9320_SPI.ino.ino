// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
//by Open-Smart Team and Catalex Team
//catalex_inc@163.com
//Store:   http://dx.com
//Demo Function: Display graphics, characters

//Arduino IDE: 1.6.5
// Board: Arduino UNO R3, Arduino Mega2560,Arduino Leonardo

#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

//***********************************************//
// If you use OPEN-SMART TFT breakout board                 //
// Reconmmend you to add 5V-3.3V level converting circuit.
// Of course you can use OPEN-SMART UNO Black version with 5V/3.3V power switch,
// you just need switch to 3.3V.
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
//----------------------------------------|
// TFT Breakout  -- Arduino UNO / Mega2560 / OPEN-SMART UNO Black
// GND              -- GND
// 3V3               -- 3.3V
// CS                 -- A3
// RS                 -- A2
// WR                -- A1
// RD                 -- A0
// RST                -- RESET
// LED                -- GND
// DB0                -- 8
// DB1                -- 9
// DB2                -- 10
// DB3                -- 11
// DB4                -- 4
// DB5                -- 13
// DB6                -- 6
// DB7                -- 7

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;
uint16_t g_identifier;
int speaker =2;
void setup(void) {
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

#ifdef USE_OPENSMART_SHIELD_PINOUT
  Serial.println(F("Using OPENSMART 2.8\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using OPENSMART 2.8\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());


    uint32_t when = millis();
    //    while (!Serial) ;   //hangs a Leonardo until you connect a Serial
    if (!Serial) delay(5000);           //allow some time for Leonardo
    Serial.println("Serial took " + String((millis() - when)) + "ms to start");
    static uint16_t identifier;
    //    tft.reset();                 //we can't read ID on 9341 until begin()
    g_identifier = tft.readID(); //
    Serial.print("ID = 0x");
    Serial.println(g_identifier, HEX);
    if (g_identifier == 0x00D3 || g_identifier == 0xD3D3) g_identifier = 0x9481; // write-only shield
    if (g_identifier == 0xFFFF) g_identifier = 0x9341; // serial
//    g_identifier = 0x9329;                             // force ID
	

    tft.begin(g_identifier);
      //tft.setRotation(tft.getRotation()+4);
tft.setRotation(1);
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
//tft.setRotation(2);    
//tft.fillRect(0, 280, 240, 20, WHITE); 
int xmax=0;
int ymax=0;
//Draw face
 tft.fillCircle(200, 120, 100, YELLOW); //head
 tft.fillCircle(200, 120-2, 80, BLACK);//mouth
 tft.fillCircle(200, 120-8, 81, YELLOW);//mouth
 tft.fillCircle(320-150, 90, 20, BLACK);//left eye
 tft.fillCircle(320-90, 90, 20, BLACK);//right eye
//    tft.setRotation(tft.getRotation()+2);
bool isBlink;
int i=0;
isBlink=false;
for (i=0;i<8;i++)
{//tft.setRotation(1);
  tft.setCursor(50, 0);
  tft.setTextColor(WHITE);  tft.setTextSize(5);
  tft.println(i+1);
  //tft.setRotation(2);
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
    //tft.setRotation(1);
    tft.setCursor(50, 0);
  tft.setTextColor(BLACK);  tft.setTextSize(5);    
  tft.println(i+1);
  //tft.setRotation(2);
}
tft.drawRect(0, 0,22, ((240-2)/45)*45+2 , BLACK);
for (i=0;i<4;i++)
{//tft.setRotation(1);
  tft.setCursor(50, 0);
  tft.setTextColor(GREEN);  tft.setTextSize(5);
  tft.println(i+1);
  //tft.setRotation(2);
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
        //tft.setRotation(1);
    tft.setCursor(50, 0);
  tft.setTextColor(BLACK);  tft.setTextSize(5);    
  tft.println(i+1);
  //tft.setRotation(2);   
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


void loop(void) {

}

void progress (int length, int seconds, int color)
{int i, increment;
 increment=(length-2)/seconds;
 //Serial.print("increment:");Serial.println(increment);
 //Serial.print("increment*seconds+1:");Serial.println(increment*seconds+1);
  tft.drawRect(0, 0,22, increment*seconds+2 , WHITE);
  //tft.drawRect(1, 280, increment*seconds,20 , WHITE);
  for (i=1; i<=seconds; i++)
  {
//tft.fillRect( i*increment+1,280, increment,20, color); 
tft.fillRect( 1,1,22-2,i*increment, color); 
 //Serial.print("fillrect:1,1,20,");Serial.println(i*increment+1);
 delay(1000);
}
}
void closerighteye()
{//close eye
  tft.fillCircle(320-90, 85, 20, YELLOW);//right eye blink

}
void openeye()
{
//open/redraw right eye
 tft.fillCircle(320-90, 90, 20, BLACK);//right eye

}
