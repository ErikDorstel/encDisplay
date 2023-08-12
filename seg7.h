#include <SPI.h>

uint8_t displays=5;
uint8_t csPIN[]={4,5,13,14,25};

void initSPI() {
  for (uint8_t display=0;display<displays;display++) { pinMode(csPIN[display],OUTPUT); digitalWrite(csPIN[display],HIGH); }
  SPI.begin(); SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE0)); }

void writeSPI(uint8_t display,uint8_t opcode,uint8_t data) {
  digitalWrite(csPIN[display],LOW);
  SPI.transfer(opcode);
  SPI.transfer(data);
  digitalWrite(csPIN[display],HIGH); }

void seg7Clear(uint8_t display) { for (int digit=1;digit<9;digit++) { writeSPI(display,digit,0x0f); } }

void seg7Intensity(uint8_t display,uint8_t intensity) { writeSPI(display,0x0a,intensity); }

void seg7Set(uint8_t display,uint8_t digit,uint8_t value,bool dp) {
  if (digit>8) { return; }
  if (dp) { value|=128; }
  writeSPI(display,digit,value); }

void initSeg7() {
  initSPI();
  for (uint8_t display=0;display<displays;display++) {
    writeSPI(display,0x0f,0x00); // Normal Operation
    writeSPI(display,0x09,0xff); // Code B decode On
    writeSPI(display,0x0b,0x07); // Scan Limit 8
    writeSPI(display,0x0c,0x01); // Shutdown Off
    seg7Intensity(display,2);
    seg7Clear(display); } }

void seg7Float(uint8_t display,int32_t number,uint8_t places) {
  int32_t num=abs(number); places++; uint8_t digit=1; bool dp;
  for (;num>0;digit++) {
    int32_t temp=num/10;
    if (digit==places && places>1) { dp=true; } else { dp=false; }
    seg7Set(display,digit,num-10*temp,dp);
    num=temp; }
  for (;digit<=places;digit++) {
    if (digit==places && places>1) { dp=true; } else { dp=false; }
    seg7Set(display,digit,0x00,dp); }
  if (number<0) { seg7Set(display,digit,0x0a,false); digit++; }
  for (;digit<9;digit++) { seg7Set(display,digit,0x0f,false); } }
