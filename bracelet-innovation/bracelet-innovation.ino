#include "Display.h"
#include <U8x8lib.h> 
#include <SPI.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 

void setup() {
  Serial.begin(9600);
  u8x8.begin();
  u8x8.setPowerSave(0);
}

void loop() {
  PrintAccessGood();
  //PrintAccessDenied();
  
}
