bool debug=true;

#include "seg7.h"
#include "ui.h"
#include "numpad.h"
#include "encoder.h"

void setup() {
  Serial.begin(115200);
  initSeg7();
  initNumpad();
  initEncoder(); }

void loop() { numpadWorker(); encoderWorker(); }
