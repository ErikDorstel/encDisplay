# encDisplay
ESP32 based encoder display
#### Features
* uses 5 x MAX7219 as 8 digit 7-segment display driver
* supports floating point values
* uses MCP23017 as I2C port expander to scan a 4x4 numpad
* uses MCP23017 as I2C port expander for 8 switches/LED
* supports 2 encoder inputs
* works together with encInterface
#### GPIO MAX7219 (SPI)
* GPIO 23 - MOSI
* GPIO 18 - CLK
* GPIO 4,5,13,14,25 - CS
#### GPIO UART
* GPIO 16 - RX
* GPIO 17 - TX
#### GPIO MCP23017 (I2C)
* GPIO 21 - SDA
* GPIO 22 - SCL
* GPIO 26 - INT Port A
* GPIO 27 - INT Port B
#### GPIO Encoder
* GPIO 32 - 1 Switch
* GPIO 33 - 1 A
* GPIO 34 - 1 B
* GPIO 35 - 2 Switch
* GPIO 36 - 2 A
* GPIO 39 - 2 B