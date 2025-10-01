#include "Display.h" 

void PrintAccessGood(){
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,"Access Granted"); 
}

void PrintAccessDenied(const char* text){
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,text); 
}
