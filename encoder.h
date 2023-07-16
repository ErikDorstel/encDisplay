QueueHandle_t encQueue;
UBaseType_t queueSizeEncQueue=256;

#define enc0A 35
#define enc0B 36
#define enc0Z 39

struct encStruct { double value[10]; } enc;

void IRAM_ATTR enc0ISR() {
  uint8_t encValue=0;
  if (digitalRead(enc0A)) { encValue|=128; }
  if (digitalRead(enc0B)) { encValue|=64; }
  if (digitalRead(enc0Z)) { encValue|=32; }
  xQueueSendFromISR(encQueue,&encValue,NULL); }

void initEncoder() {
  encQueue=xQueueCreate(queueSizeEncQueue,sizeof(uint8_t));
  pinMode(enc0A,INPUT); attachInterrupt(enc0A,enc0ISR,CHANGE);
  pinMode(enc0B,INPUT); attachInterrupt(enc0B,enc0ISR,CHANGE);
  pinMode(enc0Z,INPUT); attachInterrupt(enc0Z,enc0ISR,CHANGE);
  seg7Double(0,enc.value[0]); }

void encoderWorker() {
  uint8_t encIndex=0; uint8_t encValue; uint8_t encNowValue; static uint8_t encOldValue[10]; int encStep=0;
  if (xQueuePeekFromISR(encQueue,&encValue)) {
    xQueueReceiveFromISR(encQueue,&encValue,NULL);
    encIndex=encValue & 0b00011111;
    encNowValue=(encValue & 0b11000000) >> 6;

    if (encOldValue[encIndex]==0x10 && encNowValue==0x11) { encStep=1; } else
    if (encOldValue[encIndex]==0b11 && encNowValue==0b01) { encStep=1; } else
    if (encOldValue[encIndex]==0b01 && encNowValue==0b00) { encStep=1; } else
    if (encOldValue[encIndex]==0b00 && encNowValue==0b10) { encStep=1; } else
    if (encOldValue[encIndex]==0b10 && encNowValue==0b00) { encStep=-1; } else
    if (encOldValue[encIndex]==0b11 && encNowValue==0b10) { encStep=-1; } else
    if (encOldValue[encIndex]==0b01 && encNowValue==0b11) { encStep=-1; } else
    if (encOldValue[encIndex]==0b00 && encNowValue==0b01) { encStep=-1; }

    encOldValue[encIndex]=encNowValue;
    if (encStep==1) { enc.value[encIndex]+=0.001; seg7Double(encIndex,enc.value[encIndex]); }
    if (encStep==-1) { enc.value[encIndex]-=0.001; seg7Double(encIndex,enc.value[encIndex]); }
    if (debug) { Serial.printf("%i - %i - %i\r\n",encIndex,encNowValue,encStep); } } }