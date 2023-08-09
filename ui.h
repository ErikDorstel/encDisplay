uint8_t displayEdit=255;
uint8_t buffer[]={0,0,0,0,0,0,0,0};
uint8_t digit=0;
volatile struct encStruct { uint8_t seqIndex[8]; uint8_t nextCW[8]; uint8_t nextCCW[8]; int32_t value[8]; } enc;

void bufferClear() {
  for (uint8_t d=0;d<8;d++) { buffer[d]=0; }
  digit=0; }

void bufferShift(bool direction) {
  if (direction) { for (uint8_t d=digit+1;d>0;d--) { buffer[d]=buffer[d-1]; } digit++; }
  else { for (uint8_t d=0;d<digit-1;d++) { buffer[d]=buffer[d+1]; } buffer[digit-1]=0; digit--; } }

void bufferDisplay() {
  for (uint8_t d=0;d<8;d++) {
    if (d<digit) { seg7Set(displayEdit,d+1,buffer[d],false); }
    else { seg7Set(displayEdit,d+1,0xf,false); } } }

void uiWorker(uint8_t key) {
  if (key==255) { return; }
  if (debug) { Serial.print("Key: "); Serial.println(key,HEX); }

  //stop input
  if (key>=0xa && key<=0xc && displayEdit==key-10) {
    displayEdit=255;
    key=255; }

  //start input
  if (key>=0xa && key<=0xc) {
    seg7Clear(key-10);
    displayEdit=key-10;
    bufferClear();
    key=255; }

  // takeover input
  if (key==0xd && displayEdit<255) {
    enc.value[displayEdit]=0;
    for (uint8_t d=0;d<8;d++) { enc.value[displayEdit]+=buffer[d]*pow(10,d); }
    if (debug) { Serial.println("Display " + String(displayEdit) + ": " + String(enc.value[displayEdit])); }
    displayEdit=255;
    key=255; }

  // add number to input
  if (key<10 && digit<8 && displayEdit<255) {
    bufferShift(true);
    buffer[0]=key;
    bufferDisplay();
    key=255; }

  // delete number from input
  if (key==0xf && digit>0 && displayEdit<255) {
    bufferShift(false);
    bufferDisplay();
    key=255; } }
