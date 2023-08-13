bool debug=true;

#include "seg7.h"
#include "encoder.h"
#include "uart.h"
#include "numpad.h"
#include "ui.h"

void setup() {
  Serial.begin(115200);
  initSeg7();
  initEncoder();
  initUART();
  initNumpad();
  initUI(); }

void loop() { uiWorker(); }
