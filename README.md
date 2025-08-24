# Meshtastic UI loader for WHY2025 badge

Work in progress: trying to port Meshtastic UI to the WHY2025 badge.

TODO document more 

## What is this

Note: this runs on the M.2 board and is just the gui. 

LoRA modem is connected to the ESP32-C6 on the carrier board.

You will need to solder 2 wires and flash firmware to the C6.

https://wiki.why2025.org/Project:Meshtastic_on_the_WHY2025_badge

You also will need to enable serial in Protobufs mode on pins

Firmware repos (either one should work):
https://github.com/pkoryzna/meshtastic-firmware-why2025-carrier/

https://github.com/n0p/mesthastic-fw-why-badge/tree/why2025-badge

## Current progress

![meshtastic ui stuck and rotated wrong](doc/ui_stuck.jpeg)

uart seems to connect, try to connect again and then ui gets stuck :(

## How to build

Clone and init submodules.

Install ESP-IDF v5.5 and use the regular build command.

## Why not just build meshtastic-standalone-ui for P4?

I could not get it to work with platformio or pioarduino, so I'm trying with ESP-IDF.