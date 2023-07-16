bool debug=true;

#include "seg7.h"
#include "encoder.h"

void setup() {
  Serial.begin(115200);

  initSeg7();
  initEncoder(); }

void loop() { encoderWorker(); }
