// #include "../lib/uartCommunication.h"

// // Message Format:
// // <mHeader,dHeader,Data>
// // mHeader = char (int)
// // dHeader = char (int)
// // Data = char*

// //Send data
// //Wait half a second
// //If no confirmation recieved, send data again
// //Do this 3 times
// //If no confirmation, error out message to user
// //if confirmation recieved, good.

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
// }

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
//           notifyClients(receivedMessage);
//           break;

//         case STOP:
//           recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//           break;

//         case START:
//           recievedMessage = Serial1.readStringUntil('\n'); //clear the buffer
//           break;

//         case READYTOSTART:
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
