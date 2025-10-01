#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <U8x8lib.h> 

extern U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;

void PrintAccessGood();
void PrintAccessDenied(const char* text);

#endif // DISPLAY_H
