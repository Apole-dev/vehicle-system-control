<div align="center">

# AKS (Vehicle Control System) Telemetry Firmware

![Status: Development](https://img.shields.io/badge/Status-Development-yellow?style=flat-square)
![Platform: Arduino](https://img.shields.io/badge/Platform-Arduino-00979D?style=flat-square&logo=arduino)
![MCU: ATmega328P](https://img.shields.io/badge/MCU-ATmega328P-blue?style=flat-square)
![Interface: LoRa | CAN | GPS](https://img.shields.io/badge/Interface-LoRa_|_CAN_|_GPS-00599C?style=flat-square)

</div>

This repository contains the firmware for the **AKS (Vehicle Control System)** project. The software is written in C++ (Arduino framework) and is designed to run on a custom ATmega328P-based telemetry board. 

> 🔗 **Hardware Reference:** This firmware is specifically written for the custom AKS telemetry board. You can find the PCB designs and schematics in the **[AKS Hardware Repository](https://github.com/Apole-dev/vehicle-control-system-arduino-hw)**.

## Project Structure & Architecture

The codebase is designed to operate in two different roles depending on the node's purpose. Both routines are kept in the main file; you can switch between the **Transmitter (Tx)** and **Receiver (Rx)** by commenting/uncommenting the respective blocks before flashing to the ATmega328P.

### 1. Transmitter Node (Vehicle Side)
*Currently located in the commented-out section of the main code.*
* **Data Acquisition:** Uses the `MCP2515` module to read CAN bus messages from the vehicle and a SoftwareSerial GPS module to get location/speed data.
* **Timing:** Utilizes a non-blocking `millis()` timer to switch modes between reading GPS and transmitting CAN/GPS data every 3 seconds.
* **Transmission:** Sends structured data packets over long distances using the E32 LoRa module via UART.

### 2. Receiver Node (Base Station / Display Side)
*Currently the active code block.*
* **Data Reception:** Listens to the LoRa channel and receives the incoming 4-byte buffer data.
* **Data Parsing:** Uses a custom `Parser` class to decode the raw bytes into a readable `DataSet` structure (Channel, Type, Value).
* **Display Output:** Outputs the parsed data via Serial at 9600 baud. It includes a `NextionInit()` sequence (`0xFF 0xFF 0xFF`), indicating compatibility with Nextion HMI displays for real-time telemetry monitoring.

## Pinout Configuration (SoftwareSerial)
* **LoRa UART:** `RX = Pin 3`, `TX = Pin 2`
* **LoRa Control:** `M0 = Pin 5`, `M1 = Pin 6`, `AUX = Pin 4`
* **GPS UART (Tx Mode):** `RX = Pin 8`, `TX = Pin 7`
* **CAN SPI:** Default ATmega328P SPI pins (CS on Pin 6).

## Dependencies
This project relies on several custom libraries to handle specific hardware parsing:
* `LoRa_config.h` & `LoRa.h` (Custom LoRa implementation)
* `gpsParser.h` (NMEA parsing)
* `can.h` (MCP2515 interface)
* `Parser.h` (LoRa payload decoding)

---
