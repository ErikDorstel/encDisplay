#define enc3A 33
#define enc3B 34
#define enc4A 36
#define enc4B 39

volatile struct encStruct { uint8_t seqIndex[8]; uint8_t nextCW[8]; uint8_t nextCCW[8]; int32_t value[8]; } enc;
const uint8_t encSequence[6]={1,0,2,3,1,0};

void IRAM_ATTR enc3ISR() {
  uint8_t encIndex=3; uint8_t encInput=0;
  if (digitalRead(enc3A)) { encInput|=2; }
  if (digitalRead(enc3B)) { encInput|=1; }

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

void IRAM_ATTR enc4ISR() {
  uint8_t encIndex=4; uint8_t encInput=0;
  if (digitalRead(enc4A)) { encInput|=2; }
  if (digitalRead(enc4B)) { encInput|=1; }

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

void setSeqIndex3() {
  uint8_t encIndex=3; uint8_t encInput=0;
  if (digitalRead(enc3A)) { encInput|=2; }
  if (digitalRead(enc3B)) { encInput|=1; }
  for (int i=1;i<=4;i++) { if (encInput==encSequence[i]) { enc.seqIndex[encIndex]=i;
    enc.nextCW[encIndex]=encSequence[i+1]; enc.nextCCW[encIndex]=encSequence[i-1];
    if (debug) { Serial.printf("%i : %i - %i - %i\r\n",encIndex,enc.nextCCW[encIndex],encInput,enc.nextCW[encIndex]); } } } }

void setSeqIndex4() {
  uint8_t encIndex=4; uint8_t encInput=0;
  if (digitalRead(enc4A)) { encInput|=2; }
  if (digitalRead(enc4B)) { encInput|=1; }
  for (int i=1;i<=4;i++) { if (encInput==encSequence[i]) { enc.seqIndex[encIndex]=i;
    enc.nextCW[encIndex]=encSequence[i+1]; enc.nextCCW[encIndex]=encSequence[i-1];
    if (debug) { Serial.printf("%i : %i - %i - %i\r\n",encIndex,enc.nextCCW[encIndex],encInput,enc.nextCW[encIndex]); } } } }

void initEncoder() {
  pinMode(enc3A,INPUT_PULLUP); pinMode(enc3B,INPUT_PULLUP);
  setSeqIndex3();
  attachInterrupt(enc3A,enc3ISR,CHANGE); attachInterrupt(enc3B,enc3ISR,CHANGE);
  pinMode(enc4A,INPUT_PULLUP); pinMode(enc4B,INPUT_PULLUP);
  setSeqIndex4();
  attachInterrupt(enc4A,enc4ISR,CHANGE); attachInterrupt(enc4B,enc4ISR,CHANGE); }
