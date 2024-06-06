// #include "../lib/wsCommunication.h"
// #include "../lib/uartCommunication.h"

// void initWebSocket() {
//   ws.onEvent(onEvent);
//   server.addHandler(&ws);
// }

// void notifyClients(String message) {
//   ws.textAll(message);
// }

// void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
//   AwsFrameInfo *info = (AwsFrameInfo*)arg;
//   if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
//     //   Serial.print((char*)data);
//   }
// }

// void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
//   switch (type) {
//     case WS_EVT_CONNECT:
//       Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
//       break;
//     case WS_EVT_DISCONNECT:
//       Serial.printf("WebSocket client #%u disconnected\n", client->id());
//       break;
//     case WS_EVT_DATA:
//       handleWebSocketMessage(arg, data, len);
//       break;
//     case WS_EVT_PONG:
//     case WS_EVT_ERROR:
//       break;
//   }
// }

// bool handlepostData(AsyncWebServerRequest *request, uint8_t *datas)
// {
//     if(starting == 0){
//         notifyClients("The RoboRacer isn't ready to process this yet!");
//         return true;
//     }

//     Serial.printf("[REQUEST]\t%s\r\n", (const char *)datas);

//     JsonDocument jsonDoc;
//     DeserializationError error = deserializeJson(jsonDoc, (const char *)datas);
//     if (error)
//     {
//         Serial.print("deserializeJson() failed: ");
//         Serial.println(error.c_str());
//         return false;
//     }

//     if (jsonDoc.containsKey("name")){
//         String _name = jsonDoc["name"].as<String>();
//         // Serial1.println(_name);
//     }
//     if (jsonDoc.containsKey("time")){
//         String _time = jsonDoc["time"].as<String>();
//         // Serial1.println(_time);
//     }
//     if (jsonDoc.containsKey("distance")){
//         String _distance = jsonDoc["distance"].as<String>();
//         serial_send_message(START, DATA_ERR, " ");
//         // Serial1.println(_distance);
//     }
//     if (jsonDoc.containsKey("direction")){
//         String _direction = jsonDoc["direction"].as<String>();
//         // Serial1.println(_direction);
//     }
//     if (jsonDoc.containsKey("directive")){
//         String _directive = jsonDoc["directive"].as<String>();
//         Serial.println(_directive);
//         serial_send_message(STOP, DATA_ERR, " ");
//         // Serial1.println(_directive);
//         // if(_directive == "STOP"){
//         //     serial_send_message(STOP, DATA_ERR, " ");
//         // } else if (_directive == "START") {
//         //     serial_send_message(START, DATA_ERR, " ");
//         // }
//     }

//     Serial.println("\n");

//     return true;
// }

// void notFound(AsyncWebServerRequest *request)
// {
//     if (request->method() == HTTP_OPTIONS)
//     {
//         request->send(200);
//     }
//     else
//     {
//         request->send(404, "application/json", "{\"message\":\"Not found\"}");
//     }
// }