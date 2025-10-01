#include "Display.h" 
#include <U8x8lib.h> 

extern U8X8 u8x8;

void PrintAccessGood(const char* text){
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,text); 
}

void PrintAccessDenied(const char* text){
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,text); 
}
