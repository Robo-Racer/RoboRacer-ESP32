#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>   //NOTE: This specifically requires the "data" folder for use!
#include <FS.h>
#include <ArduinoJson.h>


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


void notFound(AsyncWebServerRequest *request){
  if (request->method() == HTTP_OPTIONS){
    request->send(200);
  } else {
    request->send(404, "application/json", "{\"message\":\"Not found\"}");
  }
}

bool handlepostData(AsyncWebServerRequest *request, uint8_t *datas) {
   Serial.printf("[REQUEST]\t%s\r\n", (const char*)datas);

   JsonDocument jsonDoc; // Adjust the size as per your JSON size requirement
   DeserializationError error = deserializeJson(jsonDoc, (const char*)datas);
   if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return false;
   }

   if (!jsonDoc.containsKey("time")) return false;
   String _time = jsonDoc["time"].as<String>();  
   Serial.println(_time);

   if (!jsonDoc.containsKey("distance")) return false;
   String _distance = jsonDoc["distance"].as<String>();
   Serial.println(_distance);

  
  return true;
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

   //Trying a serveStatic solution
   //https://blockdev.io/react-on-the-esp32/
   server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
   server.serveStatic("/static/", SPIFFS, "/");
   
   // server.on("/postData", HTTP_POST, [](AsyncWebServerRequest *request){
   //    Serial.println("Received request for /postData");
   //    Serial.printf("Type of content: %s", request->contentType());
   //    int headers = request->headers();
   //    int i;
   //    for(i=0;i<headers;i++){
   //       AsyncWebHeader* h = request->getHeader(i);
   //       Serial.printf("HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
   //    }

   //    int params = request->params();
   //    Serial.printf("Number of parameters: %d", params);
   //    for(int i = 0; i < params; i++){
   //       AsyncWebParameter* p = request->getParam(i);
   //       if(p->isFile()){ //p->isPost() is also true
   //          Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
   //       } else if(p->isPost()){
   //          Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
   //       } else {
   //          Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
   //       }
   //    }

   //    if (request->hasParam("value", true)) {
   //       String postData;
   //       AsyncWebParameter* bodyParam = request->getParam("value", true);
   //       postData = bodyParam->value();
   //       // Process postData as needed
   //       Serial.println("Received POST data:");
   //       Serial.println(postData);
   //       request->send(200, "text/plain", "Data received successfully");
   //    } else {
   //       request->send(400, "text/plain", "Bad Request: 'value' parameter missing");
   //    }});
   server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      if (request->url() == "/postData") {
         if (!handlepostData(request, data)) {
            Serial.print("Something went wrong!!!");
            request->send(400, "text/plain", "false");
         }
         request->send(200, "text/plain", "true");
      }
   });
   
   server.onNotFound(notFound);

   //Start server, we can connect to it via device now
   server.begin();
}

void loop(){}