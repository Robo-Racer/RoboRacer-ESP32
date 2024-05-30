#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h> //NOTE: This specifically requires the "data" folder for use!
#include <FS.h>
#include <ArduinoJson.h>

// Needs to be formatted the first time it runs
#define FORMAT_SPIFFS_IF_FAILED true

#define PORTENTA_BAUDRATE 9600
#define UART_TX 43
#define UART_RX 44

// Wifi name and password
const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

// Set web server port number to 80
AsyncWebServer server(80);
String receivedMessage;


// Create a WebSocket object
AsyncWebSocket ws("/ws");

struct uartPacket {
    String header;
    String data;
};

//THINGS TO GET WORKING:
//Send messages to user 
//Communication protocol to H7


void notifyClients(String message) {
  ws.textAll(message);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    //data[len] = 0;
    //String message = (char*)data;
    // Check if the message is "getReadings"
    //if (strcmp((char*)data, "getReadings") == 0) {
      //if it is, send current sensor readings
      Serial.print((char*)data);
    //}
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

// This is for testing what our filesystem looks like inside the esp32
// THANKS TO https://www.tutorialspoint.com/esp32_for_iot/esp32_for_iot_spiffs_storage.htm
void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
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
        Serial1.println(_name);
    }
    if (jsonDoc.containsKey("time")){
        String _time = jsonDoc["time"].as<String>();
        Serial1.println(_time);
    }
    if (jsonDoc.containsKey("distance")){
        String _distance = jsonDoc["distance"].as<String>();
        Serial1.println(_distance);
    }
    if (jsonDoc.containsKey("direction")){
        String _direction = jsonDoc["direction"].as<String>();
        Serial1.println(_direction);
    }
    if (jsonDoc.containsKey("directive")){
        String _directive = jsonDoc["directive"].as<String>();
        Serial1.println(_directive);
    }

    Serial1.println("\n");
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
    // Baud rate for is 460800, communication with Serial Monitor
    Serial.begin(460800);

    // Setup communication with Portenta H7
    // Serial1.begin(PORTENTA_BAUDRATE, SERIAL_8N1, UART_RX, UART_TX);

    delay(1000);

    // Serial1.println("y");
    // uartPacket packet;
    // while(1){
    //     while (Serial1.available() > 0) {
    //         char receivedChar = Serial1.read();
    //         if (receivedChar == 'y') {
    //             Serial.println(receivedMessage);  // Print the received message in the Serial monitor
    //             break;
    //         }
    //     }
    // }
    

    // Set up file system via SPIFFS
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Check our directories
    // listDir(SPIFFS, "/", 0);


    // Setup Access Point with SSID and password
    Serial.print("Setting AP (Access Point)…");
    WiFi.softAP(ssid, password, 1, 0, 1); //channel (default 1), ssid_hidden (default 0), max_connections set to 1

    // Get IP address for connecting
    // NOTE: SHOULD LATER LOOK INTO HAVING PERMANENT SET IP ADDRESS
    // NOTE FOR NOTE: THE IP ADDRESS DOESN'T SEEM TO CHANGE?
    //IP Address: 192.168.4.1
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // Trying a serveStatic solution
    // https://blockdev.io/react-on-the-esp32/
    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     request->send(SPIFFS, "/index.html", "text/html");
    // });

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    // server.serveStatic("/static/", SPIFFS, "/");

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
      if (request->method() == HTTP_GET) {
        //Nothing yet
      }
      });

    server.onNotFound(notFound);
    

    initWebSocket();

    // Start server, we can connect to it via device now
    server.begin();
}

void loop() {
    // ws.cleanupClients();
    notifyClients("Websocket Message sent?");
    delay(1000);
}