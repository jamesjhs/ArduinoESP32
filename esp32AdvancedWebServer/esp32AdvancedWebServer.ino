// DOIT ESP32 DEVKIT V1 

#include <WiFi.h>
#include <NetworkClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char *ssid = "Kramig2.4";
const char *password = "7e988e0be3";
int switchPressed = 0;
int switchCount = 0;
float wheelDia = 13.5; // diesel's wheel diameter (cm)
float wheelCirfumf = 3.1415 * wheelDia / 100; // wheel circumference (m)
float distTravelled; 
int writeDelay = 2; // minutes
//int writeDelayMsec = writeDelay * 60 * 1000; // milliseconds
int writeDelayMsec = 2000; // milliseconds
int writeTime;
int arraySpins[200][2] = {};

WebServer server(80);

const int led = 2;
const int pinSwitch = 34;

void resetData() {
  switchCount = 0;
  distTravelled = 0;
}

void dataPhp() {  // when data.php is called,

  char temp[400];
  int sec = millis() / 1000;
  int hr = sec / 3600;
  int min = (sec / 60) % 60;
  float distance = switchCount * 13.5/100 * 3.1415;

  sec = sec % 60;

  snprintf(
    temp, 400,

    "<html>\
  <head>\
    <meta http-equiv='refresh' content='1'/>\
    <title>Diesel's Wheel</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Diesel's Wheel</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Reed switch state: %01d | Distance travelled (m): %1.2f</p>\
  </body>\
</html>",

    hr, min, sec, switchPressed, distance);
  server.send(200, "text/html", temp);
}

void handleRoot() {

  char temp[400];
  int sec = millis() / 1000;
  int hr = sec / 3600;
  int min = (sec / 60) % 60;
  sec = sec % 60;

  snprintf(
    temp, 400,

    "<html>\
  <head>\
    <meta http-equiv='refresh' content='1'/>\
    <title>ESP32 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP32!</h1>\
    <img src=\"/test.svg\" />\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p><a href=\"request.php\">Button Press Test Page</a></p>\
  </body>\
</html>",

    hr, min, sec);
  server.send(200, "text/html", temp);
}

void handleNotFound() {

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
}

/*
===============================================================================================
===============================================================================================
=======================================            ============================================
======================================= SETUP LOOP ============================================
=======================================            ============================================
===============================================================================================
=============================================================================================== 
*/

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode(pinSwitch, INPUT);

  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/reset", resetData);
  server.on("/data.php", dataPhp);
  server.on("/test.svg", drawGraph);
  server.on("/graph.svg", drawGraph2);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  writeTime = millis() + writeDelayMsec;
}

void loop(void) {

  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks

  if (digitalRead(pinSwitch)) {
    if (switchPressed == 0) {
      digitalWrite(led, 1);
      delay(10);
      digitalWrite(led, 0);
      switchPressed = 1;
      switchCount++;
    }
  } else {
    if (switchPressed == 1) {
      switchPressed = 0;
    }  
  } //end if pinSwitch

  if (writeTime < millis()) {
    writeTime = millis() + writeDelayMsec;


  }


} //end LOOP()

void drawGraph() { // test.svg
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";

  int y = rand() % 130;

  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";
  server.send(200, "image/svg+xml", out);
}

void drawGraph2() { // // graph.svg
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";

/*  SYNTAX of the line elements, one by one. Previous line's x2/y2 equals new line's x1/y1: 

<line x1="10" y1="124" x2="20" y2="132" stroke-width="1" />
<line x1="20" y1="132" x2="30" y2="79" stroke-width="1" />

*/
  int y = rand() % 130;

  for (int x = 10; x < 390; x += 10) {
    int y2 = sin(3.141 * x) * 130;

    //int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";
  server.send(200, "image/svg+xml", out);
}


/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/