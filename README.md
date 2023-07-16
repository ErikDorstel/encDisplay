# encDisplay
ESP32 based encoder display
#### Features
* uses MAX7219 as 8 digit 7-segment display driver
* uses LTV845 optocoupler for the encoder inputs
* supports floating point values
#### GPIO MAX7219 (SPI)
* GPIO 23 - MOSI
* GPIO 18 - CLK
* GPIO  5 - CS
#### GPIO LTV845
* GPIO 35 - Input A
* GPIO 36 - Input B
* GPIO 39 - Input Z