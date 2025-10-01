#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance

MFRC522::MIFARE_Key key;
byte blockAddr = 4;  // On écrit/relit sur le block 4 (secteur 1)
byte trailerBlock = 7; // Block trailer du secteur 1

// ------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  while (!Serial); // attendre port série
  SPI.begin();
  mfrc522.PCD_Init();

  // Clé par défaut (FF FF FF FF FF FF)
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  Serial.println(F("Approche une carte MIFARE Classic..."));
}

// ------------------------------------------------------------------
void loop() {
  // Attente d’une carte
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.print(F("UID: "));
  for (byte i = 0; i < mfrc522.uid.size; ++i) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // On va écrire un texte dans le block 4
  String text = "HELLO WORLD :)";
  writeBlock(blockAddr, text);

  // Lecture du même block
  String data = readBlock(blockAddr);
  Serial.print(F("Contenu lu: "));
  Serial.println(data);

  // Halt et stop crypto
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

// ------------------------------------------------------------------
// Fonction écriture d’un texte dans un block (16 bytes max)
void writeBlock(byte block, String message) {
  byte buffer[16];
  message.getBytes(buffer, 16); // convert String en tableau d’octets

  // Authentification avec clé A
  MFRC522::StatusCode status;
  status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(
    MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)
  );
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Auth erreur (A): "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Ecriture
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Ecriture impossible: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  } else {
    Serial.print(F("Ecriture OK -> "));
    Serial.println(message);
  }
}

// ------------------------------------------------------------------
// Fonction lecture d’un block et retour String
String readBlock(byte block) {
  byte buffer[18];
  byte size = sizeof(buffer);

  // Authentification
  MFRC522::StatusCode status;
  status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(
    MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)
  );
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Auth erreur (A): "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return "";
  }

  // Lecture
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(block, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Lecture impossible: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return "";
  }

  // Conversion en String
  String result = "";
  for (byte i = 0; i < 16; i++) {
    if (buffer[i] != 0) result += (char)buffer[i];
  }
  return result;
}
