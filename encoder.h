QueueHandle_t encQueue;
UBaseType_t queueSizeEncQueue=1024;

#define enc0A 35
#define enc0B 36
#define enc0R 39

struct encStruct { int seqIndex[8]; int32_t value[8]; } enc;

uint16_t encSequence=0b000111100001;

void IRAM_ATTR enc0ISR() {
  uint8_t encValue=0;
  if (digitalRead(enc0A)) { encValue|=128; }
  if (digitalRead(enc0B)) { encValue|=64; }
  if (digitalRead(enc0R)) { encValue|=32; }
  xQueueSendFromISR(encQueue,&encValue,NULL); }

void setSeqIndex(uint8_t encIndex) {
  uint8_t encInput=0;
  if (digitalRead(enc0A)) { encInput|=2; }
  if (digitalRead(enc0B)) { encInput|=1; }
  for (int i=2;i<=8;i+=2) { if (encInput==((encSequence >> i) & 0b11)) { enc.seqIndex[encIndex]=i;
    if (debug) { Serial.printf("%i - %i\r\n",encIndex,(encSequence >> i) & 0b11); } } } }

void initEncoder() {
  encQueue=xQueueCreate(queueSizeEncQueue,sizeof(uint8_t));
  pinMode(enc0A,INPUT); attachInterrupt(enc0A,enc0ISR,CHANGE);
  pinMode(enc0B,INPUT); attachInterrupt(enc0B,enc0ISR,CHANGE);
  pinMode(enc0R,INPUT); attachInterrupt(enc0R,enc0ISR,CHANGE);
  setSeqIndex(0);
  seg7Float(0,enc.value[0],3); }

void encoderWorker() {
  uint8_t encValue; uint8_t encIndex; uint8_t encInput;
  if (xQueuePeekFromISR(encQueue,&encValue)) {
    xQueueReceiveFromISR(encQueue,&encValue,NULL);
    encIndex=encValue & 0b00011111;
    encInput=(encValue & 0b11000000) >> 6;

    if (encInput==((encSequence >> (enc.seqIndex[encIndex]+2)) & 0b11)) {
      if (enc.seqIndex[encIndex]==8) { enc.seqIndex[encIndex]=2; } else { enc.seqIndex[encIndex]+=2; }
      enc.value[encIndex]+=1; seg7Float(encIndex,enc.value[encIndex],3);
      if (debug) { Serial.printf("%i - %i - %i - %i\r\n",encIndex,encInput,1,uxQueueMessagesWaitingFromISR(encQueue)); } }

    else if (encInput==((encSequence >> (enc.seqIndex[encIndex]-2)) & 0b11)) {
      if (enc.seqIndex[encIndex]==2) { enc.seqIndex[encIndex]=8; } else { enc.seqIndex[encIndex]-=2; }
      enc.value[encIndex]-=1; seg7Float(encIndex,enc.value[encIndex],3);
      if (debug) { Serial.printf("%i - %i - %i - %i\r\n",encIndex,encInput,-1,uxQueueMessagesWaitingFromISR(encQueue)); } } } }
