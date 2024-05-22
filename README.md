# RoboRacer ESP32

## Table of Contents
- [RoboRacer ESP32](#roboracer-esp32)
    - [Table of Contents](#table-of-contents)
    - [Installation](#installation)
        - [Using PlatformIO](#using-platformio)
    - [The Project](#the-project)
    - [ESP32-S3-DevkitM-1](#esp32-s3-devkitm-1)
        - [Components](#components)
        - [Memory Space Allocation via Partitioning](#memory-space-allocation-via-partitioning)
    - [The Code](#the-code)
        - [Web Server Code](#web-server-code)
        - [Web Application Code](#web-application-code)
        - [Libraries Used](#libraries-used)
    - [Compilation](#compilation)
    - [Usage Example](#usage-example)
    - [Extra Notes](#extra-notes)
        - [Why Are We Deleting Static?](#why-are-we-deleting-static)
    - [Sources](#sources)

## Installation
### Using PlatformIO
[PlatformIO](https://docs.platformio.org/en/latest/what-is-platformio.html) - "PlatformIO is a cross-platform, cross-architecture, multiple framework, professional tool for embedded systems engineers and for software developers who write applications for embedded products."

1. Install [PlatformIO IDE](http://platformio.org/platformio-ide)
2. Copy [esp32-s3-devkitm-1.json](./public/esp32-s3-devkitm-1.json) from the public directory in this project and paste it into the .platformio/platforms/espressif32/boards folder on your personal system.
3. In the web-ui directory, run the npm install command
3. Follow [Compilation](#compilation) steps below to compile and run this project 

## The Project

## ESP32-S3-DevkitM-1
### Components
<p align="center">
  <img src="https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/_images/ESP32-S3-DevKitM-1_v1-annotated-photo.png" alt="ESP32-S3-DevkitM-1" height=350 style="background-color: white;">
  <p align="center">Figure 1. ESP32 DevkitM-1 Components</p>
</p>

| Key Component | Description |
| ------------- | ----------- |
| ESP32-S3-MINI-1/1U | ESP32-S3-MINI-1 and ESP32-S3-MINI-1U are two general-purpose Wi-Fi and Bluetooth Low Energy combo modules that have a rich set of peripherals. ESP32-S3-MINI-1 comes with a PCB antenna. ESP32-S3-MINI-1U comes with an external antenna connector. At the core of the modules is ESP32-S3FN8, a chip equipped with an 8 MB flash. Since flash is packaged in the chip, rather than integrated into the module, ESP32-S3-MINI-1/1U has a smaller package size. |
| 5V to 3.3V LDO | Power regulator that converts a 5 V supply into a 3.3 V output. |
| Pin Headers | All available GPIO pins (except for the SPI bus for flash) are broken out to the pin headers on the board for easy interfacing and programming. For details, please see Header Block. |
| USB-to-UART Port | A Micro-USB port used for power supply to the board, for flashing applications to the chip, as well as for communication with the chip via the on-board USB-to-UART bridge. |
| Boot Button | Download button. Holding down Boot and then pressing Reset initiates Firmware Download mode for downloading firmware through the serial port. |
| Reset Button | Press this button to restart ESP32-S3. |
| ESP32-S3 USB Port | ESP32-S3 full-speed USB OTG interface, compliant with the USB 1.1 specification. The interface is used for power supply to the board, for flashing applications to the chip, for communication with the chip using USB 1.1 protocols, as well as for JTAG debugging. |
| USB-to-UART Bridge | Single USB-to-UART bridge chip provides transfer rates up to 3 Mbps. |
| RGB LED | Addressable RGB LED, driven by GPIO48. |
| 3.3 V Power On LED | Turns on when the USB power is connected to the board. |
| | |

### Memory Space Allocation via Partitioning
Generally, the ESP32 should have the [default partitioning scheme for 8MB](https://github.com/espressif/arduino-esp32/blob/master/tools/partitions/default_8MB.csv?plain=1), but due to the size needed to be allocated for the filesystem image, a new partitioning set, [partitions_custom.csv](./partitions_custom.csv), has been created. As can be seen below, we reallocated memory from app1 to also be included in the spiffs section.
```ini
#default_8MB.csv
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x5000,
otadata,  data, ota,     0xe000,  0x2000,
app0,     app,  ota_0,   0x10000, 0x330000,
app1,     app,  ota_1,   0x340000,0x330000,
spiffs,   data, spiffs,  0x670000,0x180000,
coredump, data, coredump,0x7F0000,0x10000,

#partitions_custom.csv
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x5000,
otadata,  data, ota,     0xe000,  0x2000,
app0,     app,  ota_0,   0x10000, 0x330000,
spiffs,   data, spiffs,  0x340000,0x4B0000,
coredump, data, coredump,0x7F0000,0x10000,
```


## The Code
### Web Server Code
The ESP32 is treated as an asynchronous web server, responding to a connected users HTTP requests as it recieves them. It holds the primary job of communication between the user and the RoboRacer, utilizing its UART (Universal asynchronous receiver-transmitter) capabilities to recieve and transmit data to the Portenta H7. This section will serve as a breakdown of how the code in [main.cpp](./src/main.cpp) accomplishes this task.

```cpp
if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
{
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
}
```
First we set up our SPIFFS (Serial Peripheral Interface Flash File System), which will allow us to format our files as listed in the ./data directory. SPIFFS can now act as a filesystem, which we use in the following code.

```cpp
server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
server.serveStatic("/static/", SPIFFS, "/");
```
This sets our server (which was set up on port 80) to serve our web app when connected to. The ESP32 WiFi is set up as an Access Point, which means we are simply allowing devices to connect with us, but not providing actual WiFi.
```cpp
WiFi.softAP(ssid, password, 1, 0, 1);

IPAddress IP = WiFi.softAPIP();
Serial.print("AP IP address: ");
Serial.println(IP);
```
This code sets up the Access Point with ssid (wifi name) "ESP32-Access-Point", password "123456789", and several other options, notably limiting the maximum number of connections to 1, so no others can connect when the user is connected. We are able to output the IP address, which we give the user on the side of the RoboRacer.

```cpp
server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      if (request->method() == HTTP_POST) {
         if (!handlepostData(request, data)) {
            Serial.print("Something went wrong!!!");
            request->send(400, "text/plain", "false");
         }
         request->send(200, "text/plain", "true");
      } 
      if (request->method() == HTTP_GET) {
        //...
      }
      //...
    });
```
The above code details how we respond to HTTP requests. When we receive a request, we check what method it is, then we handle that request with built in functionality that decodes the body of the request and checks what it contains, like so:
```cpp
    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, (const char *)datas);
    //...
    if (jsonDoc.containsKey("name")){
        String _name = jsonDoc["name"].as<String>();
        Serial.println(_name);
    }
    //...
```

### Web Application Code


### Libraries Used
- [WiFi.h](https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/src/WiFi.h)
    - Allows us to set up the ESP32 as an access point and connect to another device via WiFi
- [ESPAsyncWebServer.h](https://github.com/me-no-dev/ESPAsyncWebServer)
    - Treats the ESP32 as an asynchronous web server, allowing us to process HTTP Requests from the user
- [SPIFFS.h](https://github.com/espressif/arduino-esp32/blob/master/libraries/SPIFFS/src/SPIFFS.h)
    - Lets us work with the spiffs section of our board, storing a filesystem in the flash memory
- [FS.h](https://github.com/espressif/arduino-esp32/blob/master/libraries/FS/src/FS.h)
    - Makes a more cohesive filesystem on the ESP32, letting us navigate through it as one would a computer
- [ArduinoJson.h](https://arduinojson.org/)
    - Lets us both create and parse through JSON objects, allowing sending and retrieving of data from user
- [React Router DOM](https://reactrouter.com/en/main)
    - Builds user interface to route to pages
- [React Joystick Component](https://www.npmjs.com/package/react-joystick-component)
    - Allows usage of joystick on a dev page, to direct RoboRacer


## Compilation
1. In the web-ui directory, run the nmp run build command. This will compile the web application and move the appropriate files out of the /static directory.
2. Delete the contents of the ./data directory
3. Copy the contents of ./web-ui/build and paste them to ./data
4. In the [index.html](./data/index.html) file, search for the "static" keyword using ctrl+f. Update both instances of the word to reflect the new file structure, as demonstrated below ([why are we doing this?](#why-are-we-deleting-static)):
```html
<script defer="defer" src="/static/js/main.72aff11b.js"> --> <script defer="defer" src="/js/main.72aff11b.js">

<link href="/static/css/main.cf5403a1.css" rel="stylesheet"> --> <link href="/css/main.cf5403a1.css" rel="stylesheet">
```
5. Connect the ESP32 to your computer via it's USB Port
6. Run the platformio "Build", "Upload", "Build Filesystem Image", and "Upload Filesystem Image" commands (which can be found in the PlatformIO portion of VSCode's extention bar, or by using PlatformIO's commands in the terminal) 
7. You are now ready to connect your device to the ESP32's WIFI Access Point!

## Usage Example

## Extra Notes
### Why Are We Deleting Static?
We delete the /static directory and update index.html to no longer include it because spiffs can only store files up to 32 bytes long. data\static\js\main.72aff11b.js.map is 35 bytes long, and thus we remove the static directory from the name to reduce it to 28 bytes.

## Sources
[Esspressif ESP32-S3-DevkitM-1 User Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/hw-reference/esp32s3/user-guide-devkitm-1.html)
[]()
[]()
