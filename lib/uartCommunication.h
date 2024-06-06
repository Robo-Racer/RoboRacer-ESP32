// #ifndef UARTCOMMUNICATION_H
// #define UARTCOMMUNICATION_H

// #include <Arduino.h>
// #include "wsCommunication.h"

// //COMMUNICATION ENUMS
// enum messageHeader{
//     COMM_ERR = 5,
//     STOP = 1, 
//     START = 2,
//     READYTOSTART = 3,
//     DATA = 4,
// };

// enum dataHeader{
//     DATA_ERR = 3,
//     SPEED = 1,
//     DISTANCE = 2,
// };

// boolean newData = false;
// const byte numChars = 32;
// char receivedChars[numChars];
// messageHeader mHeader;
// dataHeader dHeader;
// char data[numChars] = {0};

// messageHeader serial_get_message();
// void parseData();
// void serial_send_message(messageHeader mHeader, dataHeader dHeader, String data);
// void process_data();

// #endif