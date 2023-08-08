#define enc0A 35
#define enc0B 36
#define enc0R 39

volatile struct encStruct { uint8_t seqIndex[8]; uint8_t nextCW[8]; uint8_t nextCCW[8]; int32_t value[8]; } enc;

const uint8_t encSequence[6]={1,0,2,3,1,0};

void IRAM_ATTR enc0ISR() {
  uint8_t encIndex=0; uint8_t encInput=0;
  if (digitalRead(enc0A)) { encInput|=2; }
  if (digitalRead(enc0B)) { encInput|=1; }

  if (encInput==enc.nextCW[encIndex]) {
    if (enc.seqIndex[encIndex]<4) { enc.seqIndex[encIndex]+=1; } else { enc.seqIndex[encIndex]=1; }
    enc.nextCW[encIndex]=encSequence[enc.seqIndex[encIndex]+1];
    enc.nextCCW[encIndex]=encSequence[enc.seqIndex[encIndex]-1];
    enc.value[encIndex]+=1; } else

  if (encInput==enc.nextCCW[encIndex]) {
    if (enc.seqIndex[encIndex]>1) { enc.seqIndex[encIndex]-=1; } else { enc.seqIndex[encIndex]=4; }
    enc.nextCW[encIndex]=encSequence[enc.seqIndex[encIndex]+1];
    enc.nextCCW[encIndex]=encSequence[enc.seqIndex[encIndex]-1];
    enc.value[encIndex]-=1; } }

void setSeqIndex(uint8_t encIndex) {
  uint8_t encInput=0;
  if (digitalRead(enc0A)) { encInput|=2; }
  if (digitalRead(enc0B)) { encInput|=1; }
  for (int i=1;i<=4;i++) { if (encInput==encSequence[i]) { enc.seqIndex[encIndex]=i;
    enc.nextCW[encIndex]=encSequence[i+1]; enc.nextCCW[encIndex]=encSequence[i-1];
    if (debug) { Serial.printf("%i : %i - %i - %i\r\n",encIndex,enc.nextCCW[encIndex],encInput,enc.nextCW[encIndex]); } } } }

void initEncoder() {
  pinMode(enc0A,INPUT); pinMode(enc0B,INPUT); pinMode(enc0R,INPUT);
  setSeqIndex(0);
  attachInterrupt(enc0A,enc0ISR,CHANGE); attachInterrupt(enc0B,enc0ISR,CHANGE); attachInterrupt(enc0R,enc0ISR,CHANGE);
  for (uint8_t display=0;display<displays;display++) { seg7Float(display,enc.value[display],3); } }

void encoderWorker() {
  static uint64_t displayTimer=100;
  if (millis()>=displayTimer) { displayTimer=millis()+50;
    for (uint8_t display=0;display<displays;display++) {
      enc.value[display]+=random(-100,100); seg7Float(display,enc.value[display],3); } } }
