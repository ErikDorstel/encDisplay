# encDisplay
ESP32 based encoder display
#### Features
* uses 5 x MAX7219 as 8 digit 7-segment display driver
* supports floating point values
* uses MCP23017 as I2C port expander to scan a 4x4 numpad
#### GPIO MAX7219 (SPI)
* GPIO 23 - MOSI
* GPIO 18 - CLK
* GPIO 4,5,16,17,25 - CS
#### GPIO MCP23017 (I2C)
* GPIO 21 - SDA
* GPIO 22 - SCL
* GPIO 26 - INT Port A
* GPIO 27 - INT Port B
#### GPIO LTV845
* GPIO 35 - Input A
* GPIO 36 - Input B
* GPIO 39 - Input Z