#include "Display.h" 

void PrintAccessGood(){
  u8x8.setFont(u8x8_font_courB18_2x3_r);  // Use a valid font from U8x8 list
  u8x8.drawString(0, 0, "Access");
  u8x8.drawString(0, 5, "Granted");
}

void PrintAccessDenied() {
  u8x8.setFont(u8x8_font_courB18_2x3_r);  // Use a valid font from U8x8 list
  u8x8.drawString(0, 0, "Access");
  u8x8.drawString(0, 5, "Denied");
}
