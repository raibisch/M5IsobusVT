# M5IsobusVT

[![License](https://img.shields.io/badge/license-EUPL1.2-green)](https://joinup.ec.europa.eu/collection/eupl/eupl-text-eupl-12)

Implementation of an ISOBUS-VT on the *M5-Stack core2* module. 
...maybe the smallest ISOBUS-VT of the world !

This is still a first apha-version...there will be more documentation and development in near future ;-)
!! DONT USE IN IN A REAL TRACTOR - ONLY FOR EDUCATION AND TRAINING IN LAB !!

![picture_m5stack1](/docs/vt1.jpg) ![picture_m5stack1](/docs/vt2.jpg)

Most of the code is from my College Reinhard Hübner (thank you very much for this great work). I only rearange some part of the code to compile it with PlatformIO and remove some unused code.
See download and installation for details.

## Download and Installation

* To download click the DOWNLOAD ZIP button, rename the uncompressed folder 'M5IsobusVT'
...or clone it with git.

* the project contains in the 'lib'-directory all references to used libs. There is no need to add external libs.

* copy the 'SD_CARD' folder to the root directory of a SD-card and put it into the M5-Module

## Software and Hardware CAN adaption
The actual Implementation use the "M5-Stack core-2" Hardware (we need the PSRAM,so the old "M5-Stack-core" does not work).
For the future we plan to adopt it to hardware with other (higher resolution) TFT-Displays. But even for this we need a ESP-32 (or ESP32-S3!?) with PSRAM.

For the CAN communication we use the internal ESP32 CAN controller and map it to GPIO36 (Rx) and GPIO26(Tx). Only for the hardware adaption a external CAN-Transmitter was used. The D-SUB connecter is wired according to a common used pinning of USB-CAN adapter.

![picture_m5stack1](/docs/M5StackM5Core2GPIO.png) 

![picture_m5stack1](/docs/can%20_adapter.jpg) ![picture_m5stack1](/docs/CANopen-Stecker%209-Pin%20D-Sub%20.png)

## Remark

* Don't add M5Stack as external lib, because there are some changes to the original lib. Changes from the original code are marked with 'huebner'
* We have changed the ESP32-Arduino default stack size, so if you compile it outside from Platform-IO change the stack size in the arduino startup code.

### PlatformIO and Arduino-IDE

Projekt was build and testet with PlatformIO, for Arduino-IDE we will add in future some extra infos.

## Helpful Infos

### ISOBUS-Basics

https://de.wikipedia.org/wiki/ISOBUS

### ISOBUS open source and documentation

https://github.com/Meisterschulen-am-Ostbahnhof-Munchen
https://isobus-plus-plus.readthedocs.io/en/latest/index.html

### 'PConvert' ISOBUS Test and Simulation Program

https://meisterschulen-am-ostbahnhof-munchen-docs.readthedocs.io/projects/isobus-other-docs/de/latest/pconvert.html

## Licence

[Licensed under the European Union Public License (EUPL)-1.2-or-later](https://joinup.ec.europa.eu/collection/eupl/eupl-text-eupl-12)

[Why licensed under EUPL-1.2: it is compatible to GPL and compatible to EU-rights and regulations](https://joinup.ec.europa.eu/collection/eupl/join-eupl-licensing-community)

[Compare different licences](https://joinup.ec.europa.eu/collection/eupl/solution/joinup-licensing-assistant/jla-find-and-compare-software-licenses)
