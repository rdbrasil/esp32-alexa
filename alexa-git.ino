#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>   
#include <ESPAsyncTCP.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"


#define WIFI_SSID         "xxxxxxxxxxx"    
#define WIFI_PASS         "xxxxxxxxxxx"
#define APP_KEY           "xxxxxxxxxxxxxxxxxxxxx"      // 
#define APP_SECRET        "xxxxxxxxxxxxxxxxxxxxx"   // 


#define Lampada_ID       "xxxxxxxxxxx"    // 
#define Lampada_Pin 2  // 
#define BAUD_RATE         9600             

AsyncWebServer server(80);
void setupWiFi();
void setupSinricPro();
bool LampadaState(const String &deviceId, bool &state);


// main setup function
void setup() {
  

  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  setupWiFi();
  setupSinricPro(); 

  pinMode(Lampada_Pin, OUTPUT);
 
}

void loop() {

  SinricPro.handle();
}




bool LampadaState(const String &deviceId, bool &state) {
  digitalWrite(Lampada_Pin, state);
  return true; // request handled properly
}



//configs Wifi
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Conectando...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  Serial.printf("Conectado!\r\n[WiFi]: Endereço de IP e %s\r\n", WiFi.localIP().toString().c_str());
}

// configs SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro

  SinricProSwitch &mySwitch2 = SinricPro[Lampada_ID];
  mySwitch2.onPowerState(LampadaState);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Conectado a nuvem SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Desconectado a nuvem SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}
