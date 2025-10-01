#include "Display.h" 

void PrintAccessGood(){
  u8x8.setFont(u8x8_font_courB18_2x3_r);  // Use a valid font from U8x8 list
  u8x8.clearDisplay();
  u8x8.drawString(0, 0, "Access");
  u8x8.drawString(0, 5, "Granted");
}

void PrintAccessDenied() {
  u8x8.setFont(u8x8_font_courB18_2x3_r);  // Use a valid font from U8x8 list
  u8x8.clearDisplay();
  u8x8.drawString(0, 0, "Access");
  u8x8.drawString(0, 5, "Denied");
}

// Affiche un message multi-lignes (16 caractères par ligne max)
void PrintMessageMultiLine(const String &message){
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.clearDisplay();

    int maxLines = 8;       // max 8 lignes sur l'écran
    int lineHeight = 1;     // 1 unité = 8 pixels pour ce font approx
    int line = 0;

    int start = 0;
    while(start < message.length() && line < maxLines){
        // chercher le prochain '\n'
        int end = message.indexOf('\n', start);
        String sub;
        if(end == -1) { // pas de \n restant
            sub = message.substring(start);
            start = message.length();
        } else {
            sub = message.substring(start, end);
            start = end + 1; // passer après '\n'
        }

        // afficher la ligne
        u8x8.drawString(0, line * lineHeight, sub.c_str());
        ++line;
    }
}
