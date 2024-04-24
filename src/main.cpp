#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h> //NOTE: This specifically requires the "data" folder for use!
#include <FS.h>
#include <ArduinoJson.h>

// Needs to be formatted the first time it runs
#define FORMAT_SPIFFS_IF_FAILED true

#define PORTENTA_BAUDRATE 9600
#define UART_TX_PIN 16
#define UART_RX_PIN 17

// Wifi name and password
const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

// Set web server port number to 80
AsyncWebServer server(80);
String receivedMessage;


// This is for testing what our filesystem looks like inside the esp32
// THANKS TO https://www.tutorialspoint.com/esp32_for_iot/esp32_for_iot_spiffs_storage.htm
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("\nListing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("− failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println(" − not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(fs, file.name(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void notFound(AsyncWebServerRequest *request)
{
    if (request->method() == HTTP_OPTIONS)
    {
        request->send(200);
    }
    else
    {
        request->send(404, "application/json", "{\"message\":\"Not found\"}");
    }
}

bool handlepostData(AsyncWebServerRequest *request, uint8_t *datas)
{
    Serial.printf("[REQUEST]\t%s\r\n", (const char *)datas);

    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, (const char *)datas);
    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return false;
    }

    if (jsonDoc.containsKey("name")){
        String _name = jsonDoc["name"].as<String>();
        Serial.println(_name);
    }
    if (jsonDoc.containsKey("time")){
        String _time = jsonDoc["time"].as<String>();
        Serial.println(_time);
    }
    if (jsonDoc.containsKey("distance")){
        String _distance = jsonDoc["distance"].as<String>();
        Serial.println(_distance);
    }
    if (jsonDoc.containsKey("direction")){
        String _direction = jsonDoc["direction"].as<String>();
        Serial.println(_direction);
    }
    if (jsonDoc.containsKey("directive")){
        String _directive = jsonDoc["directive"].as<String>();
        Serial.println(_directive);
    }


    // if (!jsonDoc.containsKey("time"))
    //     return false;
    // String _time = jsonDoc["time"].as<String>();
    // Serial.println(_time);

    // if (!jsonDoc.containsKey("distance"))
    //     return false;
    // String _distance = jsonDoc["distance"].as<String>();
    // Serial.println(_distance);

    return true;
}

void setup()
{
    // Baud rate for esp32s3 is 460800, communication with Serial Monitor
    Serial.begin(460800);

    // Setup communication with Portenta H7
    Serial1.begin(PORTENTA_BAUDRATE, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);

/*  // To read from Serial1
    while (Serial1.available() > 0) {
        char receivedChar = Serial1.read();
        if (receivedChar == '\n') {
            Serial.println(receivedMessage);  // Print the received message in the Serial monitor
            receivedMessage = "";  // Reset the received message
        } else {
            receivedMessage += receivedChar;  // Append characters to the received message
        }
    }

    OR

    if(Serial1.available() > 0){
        recievedMessage = Serial.readStringUntil('\n);
    }
*/

/*  // To Write to Serial1
    // (Depending on what we want to do this is all we need)
    Serial1.println(message);   //MAKE SURE MESSAGE ENDS WITH A '\n' on either side
*/

    // Set up file system via SPIFFS
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Check our directories
    listDir(SPIFFS, "/", 0);

    // Setup Access Point with SSID and password
    Serial.print("Setting AP (Access Point)…");
    WiFi.softAP(ssid, password);

    // Get IP address for connecting
    // NOTE: SHOULD LATER LOOK INTO HAVING PERMANENT SET IP ADDRESS
    // NOTE FOR NOTE: THE IP ADDRESS DOESN'T SEEM TO CHANGE?
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // Trying a serveStatic solution
    // https://blockdev.io/react-on-the-esp32/
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
    server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      if (request->method() == HTTP_POST) {
         if (!handlepostData(request, data)) {
            Serial.print("Something went wrong!!!");
            request->send(400, "text/plain", "false");
         }
         request->send(200, "text/plain", "true");
      } 
      
      
      
      });

    server.onNotFound(notFound);

    // Start server, we can connect to it via device now
    server.begin();
}

void loop() {}





/*
//Code for Portenta?
void setup() {
    Serial.begin(460800);

    while(1){
        if (Serial.available() > 0) {
            String message = Serial.readStringUntil('\n');
            if (message == "Hello") {
                break;
            }
        }
    }
}

void loop() {
    //Direction Grabbing from Dev Page
    if(Serial.available() > 0) {
        String message = Serial.readStringUntil('\n');

        if (message == "left"){
            //Code to go left
        } else if (message == "right") {
            //Code to go right
        } else if (message == "forward") {
            //Code to go forward
        } else if (message == "back") {
            //Code to go backward
        } else if (message == "stop") {
            //Code to stop
        }
    }

    //Obstacle Avoidance

    //Color Sensing

    //When finished with code

}
*/