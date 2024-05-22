# then moves the build files to the data folder in order to be uploaded to the ESP32
# Import should be defined? https://docs.platformio.org/en/stable/scripting/construction_environments.html
# Script from: https://blockdev.io/react-on-the-esp32/, edited to use windows commands rather than linux
Import("env")
import os

def before_build_spiffs(source, target, env):
    print("Building React App...")
    env.Execute("cd web-ui && npm run build")
    print("React App built!")

    print("Removing old SPIFFS image...")
    env.Execute("rmdir /s /q data")

    print("Copying React App to SPIFFS...")
    env.Execute("xcopy web-ui\\build data /s /i /y")    

env.AddPreAction("$BUILD_DIR/spiffs.bin", before_build_spiffs)