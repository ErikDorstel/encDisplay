uint8_t displayEdit=255;
bool editNegative=false;
uint8_t buffer[]={0,0,0,0,0,0,0,0};
uint8_t digit=0;

void bufferClear() {
  for (uint8_t d=0;d<8;d++) { buffer[d]=0; }
  digit=0; editNegative=false; }

void bufferShift(bool direction) {
  if (direction) { for (uint8_t d=digit;d>0;d--) { buffer[d]=buffer[d-1]; } digit++; }
  else { for (uint8_t d=0;d<digit-1;d++) { buffer[d]=buffer[d+1]; } buffer[digit-1]=0; digit--; } }

void bufferDisplay() {
  for (uint8_t d=0;d<8;d++) {
    if (d<digit) { seg7Set(displayEdit,d+1,buffer[d],false); }
    else { seg7Set(displayEdit,d+1,0xf,false); } }
  if (editNegative && digit<8) { seg7Set(displayEdit,digit+1,0xa,false); } }

void decodeKey(uint8_t key) {
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
    if (editNegative) { enc.value[displayEdit]*=-1; }
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
    key=255; }

  //alternate minus sign
  if (key==0xe && displayEdit<255) {
    if (editNegative) { editNegative=false; } else { editNegative=true; }
    bufferDisplay();
    key=255; } }

void uiWorker() {
  static uint64_t displayTimer=100;
  if (millis()>=displayTimer) { displayTimer=millis()+50;
    for (uint8_t display=0;display<displays;display++) {
      enc.value[display]+=random(-100,100);
      if (display!=displayEdit) {
        if (display<3) { seg7Float(display,enc.value[display],3); }
        else { seg7Float(display,enc.value[display],0); } } } }

  static uint64_t numpadTimer=125;
  if (millis()>=numpadTimer) { numpadTimer=millis()+50;
    decodeKey(getKey()); } }
