#include <Wire.h>
#include <MCP23017.h>

MCP23017 numpad=MCP23017(0x20);

void initNumpad() {
  Wire.begin();
  numpad.Init();
  numpad.setPortMode(0b00001111,B,INPUT_PULLUP); }

uint8_t getKey() {
  uint8_t key=0xff; static uint8_t keyOld=0xff;

  if (key==0xff) { numpad.setPort(0b00000111,B); switch (numpad.getPort(B) & 0b11110000) {
    case 0b11100000: key=0xa; break; case 0b11010000: key=0xb; break; case 0b10110000: key=0xc; break; case 0b01110000: key=0xd; break; default: key=0xff; } }

  if (key==0xff) { numpad.setPort(0b00001011,B); switch (numpad.getPort(B) & 0b11110000) {
    case 0b11100000: key=0x3; break; case 0b11010000: key=0x6; break; case 0b10110000: key=0x9; break; case 0b01110000: key=0xf; break; default: key=0xff; } }

  if (key==0xff) { numpad.setPort(0b00001101,B); switch (numpad.getPort(B) & 0b11110000) {
    case 0b11100000: key=0x2; break; case 0b11010000: key=0x5; break; case 0b10110000: key=0x8; break; case 0b01110000: key=0x0; break; default: key=0xff; } }

  if (key==0xff) { numpad.setPort(0b00001110,B); switch (numpad.getPort(B) & 0b11110000) {
    case 0b11100000: key=0x1; break; case 0b11010000: key=0x4; break; case 0b10110000: key=0x7; break; case 0b01110000: key=0xe; break; default: key=0xff; } }

  if (key!=0xff && keyOld==0xff) { keyOld=key; return key; } else { keyOld=key; return 0xff; } }
