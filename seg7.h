#include <SPI.h>

#define PinCS 5

void initSPI() {
  pinMode(PinCS,OUTPUT); digitalWrite(PinCS,HIGH);
  SPI.begin(); SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE0)); }

void writeSPI(uint8_t opcode,uint8_t data) {
  digitalWrite(PinCS,LOW);
  SPI.transfer(opcode);
  SPI.transfer(data);
  digitalWrite(PinCS,HIGH); }

void seg7Clear() { for (int i=1;i<9;i++) { writeSPI(i,0x0f); } }

void seg7Intensity(byte intensity) { writeSPI(0x0a,intensity); }

void seg7Set(byte digit,byte value,bool dp,uint8_t display) {
  if (dp) { value|=128; }
  writeSPI(digit,value); }

void initSeg7() {
  initSPI();
  writeSPI(0x09,0xff); // Code B decode On
  writeSPI(0x0b,0x07); // Scan Limit 8
  writeSPI(0x0c,0x01); // Shutdown Off
  seg7Intensity(2);
  seg7Clear(); }

void seg7Double(uint8_t display,double number) {
  long num=labs((long)(number*1000));
  uint8_t digit=1; bool dp;
  for (;num>0;digit++) {
    unsigned long temp=num/10;
    if (digit==4) { dp=true; } else { dp=false; }
    seg7Set(digit,num-10*temp,dp,display);
    num=temp; }
  for (;digit<5;digit++) {
    if (digit==4) { dp=true; } else { dp=false; }
    seg7Set(digit,0x00,dp,display); }
  if (number<0) { seg7Set(digit,0x0a,false,display); digit++; }
  for (;digit<9;digit++) { seg7Set(digit,0x0f,false,display); } }