#ifndef RFIDHANDLER_H
#define RFIDHANDLER_H

#include <Arduino.h>
#include <MFRC522.h>

extern MFRC522 mfrc522;
extern MFRC522::MIFARE_Key key;

// Ecrire un nom/prénom dans un bloc (max 16 caractères)
bool writeBlock(byte block, const String &message);

// Lire un nom/prénom depuis un bloc
bool readBlock(byte block, String &out);

#endif // RFIDHANDLER_H
