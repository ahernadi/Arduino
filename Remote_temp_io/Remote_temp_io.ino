#include <FS.h> 
#include <ArduinoJson.h>
//define your default values here, if there are different values in config.json, they are overwritten.


#include <ESP8266mDNS.h> 
// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
  bool shouldSaveConfig = false;
  
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10
/*
This code assume that the SDA and SCL are connected on GPIO 4 and GPIO 5. If you need to assign new pins to your BMP280 use the Wire.begin(2,0) where the GPIO 2 is connected to SDA and GPIO 0 is connected to  SCL.
*/
Adafruit_BMP280 bme; // I2C
//Adafruit_BMP280 bme(BMP_CS); // hardware SPI
//Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
  
/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int api_frequency = 1; //minutes
unsigned long previousMillis = 0; 
// set up the 'counter' feed
AdafruitIO_Feed *counter;// = io.feed("temperatures.temp-sensor-2");

//#include <FS.h>    
//#include <ArduinoJson.h>
char api_user[40]="ahernadi";
char api_key[40] = "87532256773b4364978df4e62709134d";
char feed_name[40] = "temperatures.temp-sensor-2";
//char feedname[255]="";
ESP8266WebServer server(80);

  WiFiManager wifiManager;
void handleRoot() {

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Options");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += _customHeadElement;
  page += FPSTR(HTTP_HEAD_END);
  page += String(F("<h2>"));
  page += "HWH temperature monitoring";
  page += String(F("</h2>"));
  page += String(F("<h3>Current temperature: "));
  page += bme.readTemperature()* 9/5 + 32;
  page += String(F("F </h3><P>Feed name is:"));
  page += feed_name;
  page += String(F("</P>"));
  page += FPSTR(HTTP_TEMP_PORTAL_OPTIONS);
  page += FPSTR(HTTP_END);

  server.sendHeader("Content-Length", String(page.length()));
  server.send(200, "text/html", page);

}


void handleNotFound() {
//  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
//  digitalWrite(led, 0);
}
void handleSetFeed() {
  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += _customHeadElement;
  page += FPSTR(HTTP_HEAD_END);
  page += F("Set the feed here");
  page += FPSTR(HTTP_END);

  server.sendHeader("Content-Length", String(page.length()));
  server.send(200, "text/html", page);

  //DEBUG_WM(F("Sent reset page"));
  delay(5000);
  //ESP.reset();
  delay(2000);
}void handleReset() {
  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += _customHeadElement;
  page += FPSTR(HTTP_HEAD_END);
  page += F("Module will reset in a few seconds.");
  page += FPSTR(HTTP_END);

  server.sendHeader("Content-Length", String(page.length()));
  server.send(200, "text/html", page);

  //DEBUG_WM(F("Sent reset page"));
  delay(5000);
  ESP.reset();
  //Wifi.disconnect();
  //wifiManager.resetSettings();
  delay(2000);
}

void handleClear() {
  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += _customHeadElement;
  page += FPSTR(HTTP_HEAD_END);
  page += F("Module will reset in a few seconds.");
  page += FPSTR(HTTP_END);

  server.sendHeader("Content-Length", String(page.length()));
  server.send(200, "text/html", page);

  //DEBUG_WM(F("Saved Wifi network settings have been removed. THe module will reset and you will beed to go trough the setup again."));
  delay(5000);
  WiFi.disconnect();
  Serial.println("Resetting the module");
  delay(2000);
  ESP.reset();
}
void handleInfo() {
  //DEBUG_WM(F("Info"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += _customHeadElement;
  page += FPSTR(HTTP_HEAD_END);
  page += F("<dl>");
  page += F("<dt>Chip ID</dt><dd>");
  page += ESP.getChipId();
  page += F("</dd>");
  page += F("<dt>Flash Chip ID</dt><dd>");
  page += ESP.getFlashChipId();
  page += F("</dd>");
  page += F("<dt>IDE Flash Size</dt><dd>");
  page += ESP.getFlashChipSize();
  page += F(" bytes</dd>");
  page += F("<dt>Real Flash Size</dt><dd>");
  page += ESP.getFlashChipRealSize();
  page += F(" bytes</dd>");
  page += F("<dt>Soft AP IP</dt><dd>");
  page += WiFi.softAPIP().toString();
  page += F("</dd>");
  page += F("<dt>Soft AP MAC</dt><dd>");
  page += WiFi.softAPmacAddress();
  page += F("</dd>");
  page += F("<dt>Station MAC</dt><dd>");
  page += WiFi.macAddress();
  page += F("</dd>");
  page += F("</dl>");
  page += FPSTR(HTTP_END);

  server.sendHeader("Content-Length", String(page.length()));
  server.send(200, "text/html", page);

 /// DEBUG_WM(F("Sent info page"));
}
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
void setup() {

  // start the serial connection
  Serial.begin(115200);
  delay(500);
    //for ESP8266-01
 //   Wire.pins(2, 0);
 //   Wire.begin(2, 0);
  // wait for serial monitor to open
  //while(! Serial);
  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(api_user, json["api_user"]);
          strcpy(api_key, json["api_key"]);
          strcpy(feed_name, json["feed_name"]);

        } else {
          Serial.println("failed to parse json");
        }
        configFile.close();
      }
      else 
      {
          Serial.println("failed to load json config");
        }
    }
    else {
        Serial.println("/Config.json does not exist");      
      }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
  WiFiManagerParameter custom_api_user("apiuser", "API username", api_user, 40);
  WiFiManagerParameter custom_api_key("apikey", "API key", api_key, 40);
  WiFiManagerParameter custom_feed_name("feedname", "Feed name", feed_name, 40);
  wifiManager.addParameter(&custom_api_user);
  wifiManager.addParameter(&custom_api_key);
  wifiManager.addParameter(&custom_feed_name);

  wifiManager.setSaveConfigCallback(saveConfigCallback);

 
 if (!bme.begin(0x76)) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  Serial.print("Connecting to Adafruit IO");
//counter=io.feed("temperatures.temp-sensor-2");
  // connect to io.adafruit.com

  //wifiManager.resetSettings();// uncomment to forget previous wifi manager settings
  wifiManager.autoConnect("Hwh_temp_mon","");
  Serial.println("yay connected");
    strcpy(api_user, custom_api_user.getValue());
  strcpy(api_key, custom_api_key.getValue());
  strcpy(feed_name, custom_feed_name.getValue());
  counter=io.feed(feed_name);
  io.connect();
  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["api_user"] = api_user;
    json["api_key"] = api_key;
    json["feed_name"] = feed_name;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  //sprintf(feedname,"%s.%s","Temperatures",io.feed_name);
  //Serial.println(feedname);
  //feedname="temperatures."+io.feed_name;

//counter=io.feed("temperatures.temp-sensor-2");
//starting web server to service:
// feed anme change
// reset
// status
  server.on("/", handleRoot);
  server.on("/Clear",handleClear);
  server.on("/SetFeed", handleSetFeed);  
  server.on("/r",handleReset);
  server.on("/i", handleInfo);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
server.begin();
  if (!MDNS.begin("hwhtempmonsetup")) {             // Start the mDNS responder for esp8266.local
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
}

void loop() {
  server.handleClient();
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  MDNS.update();


    unsigned long currentMillis = millis();//4294967295

  //if ((currentMillis<previousMillis?currentMillis - previousMillis:4294967295-previousMillis+currentMillis) >= (long)api_frequency*1000*60) {
  if (currentMillis - previousMillis >= (long)api_frequency*1000*60 || currentMillis<previousMillis) {
   // (currentMillis<previousMillis) ? Serial.println ("Overflow True:" + (4294967295 - previousMillis + currentMillis)) : Serial.println("Have not over false:" +(currentMillis - previousMillis));
 //Serial.println("time -> "+currentMillis);
 //Serial.println("time -> "+previousMillis);
// Serial.println("time -> "+(currentMillis<previousMillis) ? (currentMillis - previousMillis) : (4294967295 - previousMillis + currentMillis) );
 //Serial.println("time -> "+(currentMillis<previousMillis ? currentMillis - previousMillis : 4294967295 - previousMillis + currentMillis) );
 
    previousMillis = currentMillis;

   counter->save(bme.readTemperature()* 9/5 + 32); //in Farenheit
  // save count to the 'counter' feed on Adafruit IO
  Serial.println("sending -> ");
  Serial.print(currentMillis);
    }

}
