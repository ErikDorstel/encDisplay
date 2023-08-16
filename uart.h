void initUART() {
  Serial2.begin(115200); }

void uartSend(uint8_t encoder,int32_t value) {
  uint8_t buffer[4];
  Serial2.write(encoder | ((~encoder) << 4));
  memcpy(buffer,&value,4);
  Serial2.write(buffer,4); }

void uartReceive() {
  static bool uartStatus=false; static uint8_t encoder;
  if (uartStatus==false && Serial2.available()>=1) {
    uint8_t receiveByte=Serial2.read();
    if ((((~receiveByte) & 0b11110000) >> 4) == (receiveByte & 0b00001111)) {
      uartStatus=true; encoder=receiveByte & 0b00001111; } }
  if (uartStatus==true && Serial2.available()>=4) {
    uint8_t buffer[4]; int32_t value;
    buffer[0]=Serial2.read();
    buffer[1]=Serial2.read();
    buffer[2]=Serial2.read();
    buffer[3]=Serial2.read();
    memcpy(&value,buffer,4);
    enc.value[encoder]=value;
    uartStatus=false; } }
