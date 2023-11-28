# M5IsobusVT

[![License](https://img.shields.io/badge/license-EUPL1.2-green)](https://joinup.ec.europa.eu/collection/eupl/eupl-text-eupl-12)

Implementation of an ISOBUS-VT on the *M5-Stack core2* module. 
...maybe the smallest ISOBUS-VT of the world !

Most of the code is from my College Reinhard Hübner (thank you very much for this great work). I only rearange some part of the code to compile it with PlatformIO and remove some unused code.
See download and instalation for details.

## Download and Installation
To download click the DOWNLOAD ZIP button, rename the uncompressed folder 'M5IsobusVT'... or clone it with GIT.
the project contains in the 'lib'-directory all references to used libs. There is no need to add external libs. 

## Prepare SD-Card
The actual version needs some font data from an external SD-card of the M5-Stack module. Copy the contence of the 'SD_CARD' folder to an formated empty SD-card. 
After an objectpool upload the data was stored on the SD-card.

## Remark
Don't add M5Stack as external lib, because there are some changes to the original lib.

### PlatformIO and Arduino-IDE
Projekt was build and testet with PlatformIO, for Arduino-IDE we will add in future some extra infos.

...still a first apha-version...there will be more documentation in near future ;-)


## Helpful Infos

### ISOBUS-Basics

https:/https://de.wikipedia.org/wiki/ISOBUS
https://github.com/Meisterschulen-am-Ostbahnhof-Munchen

### PConvert ISOBUS Test and Simulation Program

https://meisterschulen-am-ostbahnhof-munchen-docs.readthedocs.io/projects/isobus-other-docs/de/latest/pconvert.html


## Licence
[Licensed under the European Union Public License (EUPL)-1.2-or-later](https://joinup.ec.europa.eu/collection/eupl/eupl-text-eupl-12)

[Why licensed under EUPL-1.2: it is compatible to GPL and compatible to EU-rights and regulations](https://joinup.ec.europa.eu/collection/eupl/join-eupl-licensing-community)

[Compare different licences](https://joinup.ec.europa.eu/collection/eupl/solution/joinup-licensing-assistant/jla-find-and-compare-software-licenses)
