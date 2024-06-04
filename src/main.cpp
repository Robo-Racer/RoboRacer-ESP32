#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h> //NOTE: This specifically requires the "data" folder for use!
#include <FS.h>
#include <ArduinoJson.h>

// Needs to be formatted the first time it runs
#define FORMAT_SPIFFS_IF_FAILED true

#define PORTENTA_BAUDRATE 9600
// #define UART_TX 43
// #define UART_RX 44
#define UART_TX 17
#define UART_RX 18

//COMMUNICATION ENUMS
enum messageHeader{
    COMM_ERR = -1,
    STOP = 1, 
    START = 2,
    READYTOSTART = 3,
    DATA = 4,
};

//COMMUNICATION ENUMS
enum dataHeader{
    DATA_ERR = -1,
    SPEED = 1,
    DISTANCE = 2,
};


// Wifi name and password
const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

// Set web server port number to 80
AsyncWebServer server(80);
String receivedMessage;
int starting;

// Create a WebSocket object
AsyncWebSocket ws("/ws");


messageHeader serial_get_message();
void serial_send_message(messageHeader mHeader, dataHeader dHeader, String data);
void notifyClients(String message);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
void notFound(AsyncWebServerRequest *request);
bool handlepostData(AsyncWebServerRequest *request, uint8_t *datas);
void process_data();

void setup()
{
    starting = 0;
    // Baud rate for is 9600, communication with Serial Monitor
    Serial.begin(9600);

    delay(1000);

    // Setup communication with Portenta H7
    Serial1.begin(PORTENTA_BAUDRATE, SERIAL_8N1, UART_RX, UART_TX);

    delay(1000);

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
    //IP Address: 192.168.4.1
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

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
    server.begin();    // Start server, we can connect to it via device now

    //Send READY to H7
    serial_send_message(READYTOSTART, DATA_ERR, " ");
    Serial.print("Sent Ready to start to Portenta!");
    // delay(1000);

    //Wait for READY from H7
    while(1){
        if(Serial1.available() > 0){
            if(serial_get_message() == READYTOSTART){
                break;
            }
        }
    }
    Serial.print("Got Ready to start to Portenta!");
    starting = 1;
}

void loop() {
    // ws.cleanupClients();
    // if(ws.availableForWriteAll()){
    //     notifyClients("Websocket Message sent?");
    // }
    // delay(1000);
    // Serial.println("Inside Loop!");
}

//Send data
//Wait half a second
//If no confirmation recieved, send data again
//Do this 3 times
//If no confirmation, error out message to user
//if confirmation recieved, good.

messageHeader serial_get_message(){

    String recievedMessage;
    String headerStr = "ok";
    messageHeader recievedHeader;

    while(Serial1.available() > 0){
      if(Serial1.available() > 0){
        headerStr = Serial1.readStringUntil(',');
      }
      Serial.println(headerStr);
      Serial.println(headerStr.toInt());

      recievedHeader = (messageHeader)(headerStr.toInt());

      Serial.println(recievedHeader);

      switch(recievedHeader){
        case COMM_ERR:
          recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
          notifyClients(receivedMessage);
          break;

        case STOP:
          recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
          break;

        case START:
          recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
          break;

        case READYTOSTART:
          recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
          notifyClients("The RoboRacer is ready to start!");
          break;

        case DATA:
          process_data();
          break;

        default:
          recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
          break;
      }

    //   notifyClients(recievedMessage);

    }
    return recievedHeader;
}

void serial_send_message(messageHeader mHeader, dataHeader dHeader, String data){
    Serial.print("Expecting: ");
    Serial.print(mHeader);
  switch(mHeader){
    case COMM_ERR:
      Serial1.print(mHeader);
      Serial1.print(",");
      Serial1.println(data);
      break;

    case START:
      Serial1.print(mHeader);
      Serial1.print(",");
      break;

    case STOP:
      Serial1.print(mHeader);
      Serial1.print(",");
      break;

    case READYTOSTART:
      Serial1.print(mHeader);
      Serial1.print(",");
      break;

    case DATA:
      Serial1.print(mHeader);
      Serial1.print(",");
      Serial1.print(dHeader);
      Serial1.print(",");
      Serial1.println(data);
      break;

    default:
      Serial1.print(mHeader);
      Serial1.println(",");
      break;
  }
}

void notifyClients(String message) {
  ws.textAll(message);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    //   Serial.print((char*)data);
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
    if(starting == 0){
        notifyClients("The RoboRacer isn't ready to process this yet!");
        return true;
    }

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
        // Serial1.println(_name);
    }
    if (jsonDoc.containsKey("time")){
        String _time = jsonDoc["time"].as<String>();
        // Serial1.println(_time);
    }
    if (jsonDoc.containsKey("distance")){
        String _distance = jsonDoc["distance"].as<String>();
        serial_send_message(START, DATA_ERR, " ");
        // Serial1.println(_distance);
    }
    if (jsonDoc.containsKey("direction")){
        String _direction = jsonDoc["direction"].as<String>();
        // Serial1.println(_direction);
    }
    if (jsonDoc.containsKey("directive")){
        String _directive = jsonDoc["directive"].as<String>();
        Serial.println(_directive);
        serial_send_message(STOP, DATA_ERR, " ");
        // Serial1.println(_directive);
        // if(_directive == "STOP"){
        //     serial_send_message(STOP, DATA_ERR, " ");
        // } else if (_directive == "START") {
        //     serial_send_message(START, DATA_ERR, " ");
        // }
    }

    Serial.println("\n");

    return true;
}

void process_data(){
  String headerStr;
  String recievedMessage;
  dataHeader recievedHeader;

  headerStr = Serial1.readStringUntil(',');
  recievedHeader = (dataHeader)(headerStr.toInt());
  
  switch(recievedHeader){
    case SPEED:
      recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
      break;

    case DISTANCE:
      recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
      break;

    default:
      break;
  }
}