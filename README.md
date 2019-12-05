# USBreadTHD

Project to read ADC data from a microcontroller(MCU) and compute total harmonic distortion (THD). 
The combination of the MCU  with this program will allow the testing of audio amplifiers.

## Currently Working On

Each ADC sampe is 2 bytes. Reading from the Linux input stream may not return the whole buffer transmitted 
from the MCU. Some overhead is needed to make sure the alignement of the bytes is knonw. 
