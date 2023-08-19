#include <Wire.h>
#include <MCP23017.h>

MCP23017 numpad=MCP23017(0x20);

void initNumpad() {
  Wire.begin();
  numpad.Init();
  numpad.setPortMode(0b11110000,A,INPUT_PULLUP);   // additional keys/led
  numpad.setPortMode(0b11110000,B,INPUT_PULLUP); } // numpad 4x4

uint8_t getKey() {
  uint8_t key=0xff; static uint8_t keyOld=0xff;

  if (key==0xff) { numpad.setPort(0b11100000,B); switch (numpad.getPort(B) & 0b00001111) {
    case 0b00001110: key=0x1; break; case 0b00001101: key=0x2; break; case 0b00001011: key=0x3; break; case 0b00000111: key=0xa; break; default: key=0xff; } }

  if (key==0xff) { numpad.setPort(0b11010000,B); switch (numpad.getPort(B) & 0b00001111) {
    case 0b00001110: key=0x4; break; case 0b00001101: key=0x5; break; case 0b00001011: key=0x6; break; case 0b00000111: key=0xb; break; default: key=0xff; } }

  if (key==0xff) { numpad.setPort(0b10110000,B); switch (numpad.getPort(B) & 0b00001111) {
    case 0b00001110: key=0x7; break; case 0b00001101: key=0x8; break; case 0b00001011: key=0x9; break; case 0b00000111: key=0xc; break; default: key=0xff; } }

  if (key==0xff) { numpad.setPort(0b01110000,B); switch (numpad.getPort(B) & 0b00001111) {
    case 0b00001110: key=0xe; break; case 0b00001101: key=0x0; break; case 0b00001011: key=0xf; break; case 0b00000111: key=0xd; break; default: key=0xff; } }

  if (key==0xff) { switch (numpad.getPort(A) & 0b00001111) {
    case 0b00001110: key=0x10; break; case 0b00001101: key=0x11; break; case 0b00001011: key=0x12; break; case 0b00000111: key=0x13; break; default: key=0xff; } }

  if (key!=0xff && keyOld==0xff) { keyOld=key; return key; } else { keyOld=key; return 0xff; } }
