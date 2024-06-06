#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h> //NOTE: This specifically requires the "data" folder for use!
#include <FS.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
// #include "../lib/uartCommunication.h"
// #include "../lib/wsCommunication.h"

// Needs to be formatted the first time it runs
#define FORMAT_SPIFFS_IF_FAILED true
#define PORTENTA_BAUDRATE 9600
#define UART_TX 17
#define UART_RX 18

// Set web server port number to 80
const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";
int starting;

AsyncWebServer server(80);

String recievedMessage;

// Create a WebSocket object
AsyncWebSocket ws("/ws");


void notifyClients(String message);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
void notFound(AsyncWebServerRequest *request);
bool handlepostData(AsyncWebServerRequest *request, uint8_t *datas, int& starting);
bool handlegetData(AsyncWebServerRequest *request, uint8_t *datas);

enum messageHeader{
    COMM_ERR = 5,
    STOP = 1, 
    START = 2,
    READYTOSTART = 3,
    DATA = 4,
};

enum dataHeader{
    DATA_ERR = 3,
    SPEED = 1,
    DISTANCE = 2,
};

boolean newData = false;
const byte numChars = 32;
char receivedChars[numChars];
messageHeader mHeader;
dataHeader dHeader;
char data[numChars] = {0};

// messageHeader serial_get_message();
// void parseData();
// void serial_send_message(messageHeader mHeader, dataHeader dHeader, String data);
// void process_data();
void listDir(fs::FS &fs, const char *dirname, uint8_t levels);

void setup()
{
    starting = 0;
    // Baud rate for is 9600, communication with Serial Monitor
    Serial.begin(9600);
    delay(500);

    // Setup communication with Portenta H7
    // Serial1.begin(PORTENTA_BAUDRATE, SERIAL_8N1, UART_RX, UART_TX);
    delay(500);


    // Set up file system via SPIFFS
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

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
         if (!handlepostData(request, data, starting)) {
            Serial.print("Something went wrong!!!");
            request->send(400, "text/plain", "false");
         }
         request->send(200, "text/plain", "true");
      } 
      if (request->method() == HTTP_GET) {
        if (!handlegetData(request, data)) {
            Serial.print("Something went wrong!!!");
            request->send(400, "text/plain", "false");
        }
      }
    });
    server.onNotFound(notFound);

    initWebSocket();
    server.begin();    // Start server, we can connect to it via device now

    // while (Serial1.available() > 0) {
    //     Serial1.read(); //clear buffer
    // }

    //Send READY to H7
    // serial_send_message(READYTOSTART, DATA_ERR, " ");
    // Serial1.println("3,-1");
    Serial.print("Sent Ready to start to Portenta!");

    // char c;
    // Wait for READY from H7
    // messageHeader mH;
    // while(mH != READYTOSTART){
    //     while(Serial1.available() > 0){
    //         mH = serial_get_message();
    //         Serial.print("Before break: ");
    //         Serial.println(mH);
    //         if(mH == READYTOSTART){
    //             break;
    //         }
    //     }
    // }
    // Serial.print("Got Ready to start to Portenta!");
    starting = 1;
}

void loop() {
    // if(Serial1.available() > 0){
    //     messageHeader mH = serial_get_message();
    // } else {
    //     //Do Nothing
    // }
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

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
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

bool handlepostData(AsyncWebServerRequest *request, uint8_t *datas, int& starting)
{
    Serial.printf("starting: %d", starting);
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
        // serial_send_message(START, DATA_ERR, " ");
        // Serial1.println(_distance);
    }
    if (jsonDoc.containsKey("direction")){
        String _direction = jsonDoc["direction"].as<String>();
        // Serial1.println(_direction);
    }
    if (jsonDoc.containsKey("directive")){
        String _directive = jsonDoc["directive"].as<String>();
        Serial.println(_directive);
        // serial_send_message(STOP, DATA_ERR, " ");
        // Serial1.println(_directive);
        // if(_directive == "STOP"){
        //     serial_send_message(STOP, DATA_ERR, " ");
        // } else if (_directive == "START") {
        //     serial_send_message(START, DATA_ERR, " ");
        // }
    }

    if(jsonDoc.containsKey("addSpeedProfile")){
        String _newFile = jsonDoc["addSpeedProfile"].as<String>();
        fs:File newFile = SPIFFS.open("/speedProfiles.json", "w");
        newFile.print(_newFile);
        newFile.close();

        //Test file
        File testFile = SPIFFS.open("/speedProfiles.json", "w");
        Serial.print(testFile.readString());
        testFile.close();
    }

    Serial.println("\n");

    return true;
}

bool handlegetData(AsyncWebServerRequest *request, uint8_t *datas){
    Serial.printf("[REQUEST]\t%s\r\n", (const char *)datas);

    // JsonDocument jsonDoc;
    // DeserializationError error = deserializeJson(jsonDoc, (const char *)datas);
    // if (error)
    // {
    //     Serial.print("deserializeJson() failed: ");
    //     Serial.println(error.c_str());
    //     return false;
    // }
    Serial.println(request->url());
    if (request->url() == "/speedProfiles"){
        File file = SPIFFS.open("/speedProfiles.json", "r");
        request->send(file, "/speedProfiles.json", "text/json", false, nullptr);
        file.close();
    }

    return true;
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


// messageHeader serial_get_message(){

//     String recievedMessage;
//     String headerStr = "ok";
//     messageHeader recievedHeader;

//     while(Serial1.available() > 0){
//       if(Serial1.available() > 0){
//         headerStr = Serial1.readStringUntil(',');
//       }
//       Serial.println(headerStr);
//       Serial.println(headerStr.toInt());

//       recievedHeader = (messageHeader)(headerStr.toInt());

//       Serial.println(recievedHeader);

//       switch(recievedHeader){
//         case COMM_ERR:
//           recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//           notifyClients(recievedMessage);
//           break;

//         case STOP:
//           recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//           break;

//         case START:
//           recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//           break;

//         case READYTOSTART:
//           Serial.println("WE GOT READYTOSTART");
//           recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//           notifyClients("The RoboRacer is ready to start!");
//           break;

//         case DATA:
//           process_data();
//           break;

//         default:
//           recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//           break;
//       }

//     //   notifyClients(recievedMessage);

//     }
//     return recievedHeader;
// }

// void serial_send_message(messageHeader mHeader, dataHeader dHeader, String data){
//     Serial.print("Expecting: ");
//     Serial.print(mHeader);
//   switch(mHeader){
//     case COMM_ERR:
//       Serial1.print(mHeader);
//       Serial1.print(",");
//       Serial1.println(data);
//       break;

//     case START:
//       Serial1.print(mHeader);
//       Serial1.print(",");
//       break;

//     case STOP:
//       Serial1.print(mHeader);
//       Serial1.print(",");
//       break;

//     case READYTOSTART:
//       Serial1.print(mHeader);
//       Serial1.print(",");
//       break;

//     case DATA:
//       Serial1.print(mHeader);
//       Serial1.print(",");
//       Serial1.print(dHeader);
//       Serial1.print(",");
//       Serial1.println(data);
//       break;

//     default:
//       Serial1.print(mHeader);
//       Serial1.println(",");
//       break;
//   }
// }

// void process_data(){
//   String headerStr;
//   String recievedMessage;
//   dataHeader recievedHeader;

//   headerStr = Serial1.readStringUntil(',');
//   recievedHeader = (dataHeader)(headerStr.toInt());
  
//   switch(recievedHeader){
//     case SPEED:
//       recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//       break;

//     case DISTANCE:
//       recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//       break;

//     default:
//       break;
//   }
// }



// messageHeader serial_get_message(){
//     static boolean recvInProgress = false;
//     static byte ndx = 0;

//     char startMarker = '<';
//     char endMarker = '>';
//     char rc;
 
//     while (Serial1.available() > 0 && newData == false) {
//         rc = Serial1.read();

//         if (recvInProgress == true) {
//             if (rc != endMarker) {
//                 receivedChars[ndx] = rc;
//                 ndx++;
//                 if (ndx >= numChars) {
//                     ndx = numChars - 1;
//                 }
//             } else {
//                 receivedChars[ndx] = '\0'; // terminate the string
//                 recvInProgress = false;
//                 ndx = 0;
//                 newData = true;
//             }
//         } else if (rc == startMarker) {
//             recvInProgress = true;
//         }
//     }

//     parseData();

//     switch(mHeader){
//         case COMM_ERR:
//             // recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//             notifyClients(data);
//             break;

//         case STOP:
//             // recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//             break;

//         case START:
//             // recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//             break;

//         case READYTOSTART:
//             // recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//             notifyClients("The RoboRacer is ready to start!");
//             break;

//         case DATA:
//             process_data();
//             break;

//         default:
//             break;
//     }

//     return mHeader;
// }

// void parseData() {      // split the data into its parts
//     char * strtokIndx; // this is used by strtok() as an index

//     strtokIndx = strtok(receivedChars,",");      // get the first part - the string
//     mHeader = (messageHeader)atoi(strtokIndx);     // convert this part to an integer
 
//     strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
//     dHeader = (dataHeader)atoi(strtokIndx);     // convert this part to an integer

//     strtokIndx = strtok(NULL, ">");
//     strcpy(data, strtokIndx); // copy it to data

// }


// void serial_send_message(messageHeader mHeader, dataHeader dHeader, String data){
//     Serial.print("Expecting: ");
//     Serial.print(mHeader);

//     int messageSize = 2 + 2 + 2 + data.length(); //2 for <>, 2 for ,'s, and 2 for mHeader + dHeader
//     char* sentMessage = new char[messageSize]; 
//     sentMessage[0] = '<';
//     sentMessage[1] = '0' + (int)mHeader;
//     sentMessage[2] = ',';
//     sentMessage[3] = '0' + (int)dHeader;
//     sentMessage[4] = ',';

//     for(int i = 0; i < data.length(); i++){
//         sentMessage[5 + i] = data[i];
//     }
//     sentMessage[messageSize] = '>';
    
//     Serial1.print(sentMessage);
// // }

// void process_data(){
//   switch(dHeader){
//     case SPEED:
//     //   recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//       break;

//     case DISTANCE:
//     //   recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//       break;

//     default:
//       break;
//   }
// }