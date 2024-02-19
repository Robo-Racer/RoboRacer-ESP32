#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>   //NOTE: This specifically requires the "data" folder for use!
#include <FS.h>

//Needs to be formatted the first time it runs
#define FORMAT_SPIFFS_IF_FAILED true

// Wifi name and password
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

// Set web server port number to 80
AsyncWebServer server(80);

//This is for testing what our filesystem looks like inside the esp32
//THANKS TO https://www.tutorialspoint.com/esp32_for_iot/esp32_for_iot_spiffs_storage.htm
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
   Serial.printf("\nListing directory: %s\r\n", dirname);

   File root = fs.open(dirname);
   if(!root){
      Serial.println("− failed to open directory");
      return;
   }
   if(!root.isDirectory()){
      Serial.println(" − not a directory");
      return;
   }

   File file = root.openNextFile();
   while(file){
      if(file.isDirectory()){
         Serial.print("  DIR : ");
         Serial.println(file.name());
         if(levels){
            listDir(fs, file.name(), levels -1);
         }
      } else {
         Serial.print("  FILE: ");
         Serial.print(file.name());
         Serial.print("\tSIZE: ");
         Serial.println(file.size());
      }
      file = root.openNextFile();
   }
}



void setup() {
  //Baud rate for esp32s3 is 460800
  Serial.begin(460800);

  // Set up file system via SPIFFS
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
  }

  //Check our directories
  listDir(SPIFFS, "/", 0);

  // Setup Access Point with SSID and password
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  //Get IP address for connecting
  //NOTE: SHOULD LATER LOOK INTO HAVING PERMANENT SET IP ADDRESS
  //NOTE FOR NOTE: THE IP ADDRESS DOESN'T SEEM TO CHANGE? 
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // THIS WORKS, THEREFORE WE KNOW THE CONNECTION OCCURS AND *CAN* SEND THINGS
  //SO WHY WON'T THE OTHER WORK? NOTE: PERHAPS THERE IS AN ISSUE WITH SPIFFS?
  //IT WORKS NOW, I'M JUST STUPID
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //     request->send(200, "text/html", "<h2>Hello ESP32!!!</h2");
  // });

  // Serve default index.html and demo.js
   // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
   //     Serial.println("Received request for /");
   //     request->send(SPIFFS, "/index.html", "text/html");
   // });

   //Trying a serveStatic solution
   //https://blockdev.io/react-on-the-esp32/
   //IT WORKS. The issue was I am stupid.
   server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
   server.serveStatic("/static/", SPIFFS, "/");
  
  //Start server, we can connect to it via device now
  server.begin();
}

void loop(){}

//TESTER CODE
//THIS CODE GUARUNTEED SPIFFS WAS WORKING AS INTENDED, it correctly read an index.html
// #include <Arduino.h>
// #include "SPIFFS.h"

// void setup() {
//   Serial.begin(460800);
  
//   if(!SPIFFS.begin(true)){
//     Serial.println("An Error has occurred while mounting SPIFFS");
//     return;
//   }
  
//   File file = SPIFFS.open("/index.html");
//   if(!file){
//     Serial.println("Failed to open file!");
//     return;
//   }
  
//   Serial.println("Content of file:");
//   while(file.available()){
//     Serial.write(file.read());
//   }
//   file.close();
// }