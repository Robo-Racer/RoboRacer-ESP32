# All this does is when you try to build the filesystem image, it builds the react app from the web folder,
# then moves the build files to the data folder in order to be uploaded to the ESP32
# Import should be defined?
# https://docs.platformio.org/en/stable/scripting/construction_environments.html
# Script from: https://blockdev.io/react-on-the-esp32/
Import("env")
import os

def before_build_spiffs(source, target, env):
    print("Building React App...")
    env.Execute("cd web-ui && npm run build")
    print("React App built!")

    print("Removing old SPIFFS image...")
    env.Execute("rm -rf data")

    print("Copying React App to SPIFFS...")
    env.Execute("cp -r web-ui/build data")    

env.AddPreAction("$BUILD_DIR/spiffs.bin", before_build_spiffs)