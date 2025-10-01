#include "Display.h"
#include "RFID.h"
#include <SPI.h>

#define RST_PIN 9
#define SS_PIN 10
byte blockAddr = 4; // Bloc où on écrit/relit le nom/prénom

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin();            // Démarrer le bus SPI avant le lecteur RFID
  mfrc522.PCD_Init();     // Initialiser le lecteur RFID

  u8x8.begin();           // Initialiser l'écran
  u8x8.setPowerSave(0);   // Activer l'écran

  // Initialiser la clé par défaut
  for (byte i=0; i<6; ++i) key.keyByte[i] = 0xFF;

  Serial.println(F("Système prêt."));

  scenario();  // Exécuter le scénario une fois
}

void loop() {
}

void scenario(){
    String nameOnCard;

    // ---- Étape 1 : enregistrer badge et nom/prénom ----    
    PrintMessageMultiLine("Enregistrement\nnom: Jean\nprenom: Dupont");
    while(!mfrc522.PICC_IsNewCardPresent()){}
    delay(2000);
    PrintMessageMultiLine("Scan your badge");
    mfrc522.PICC_ReadCardSerial();

    // boucle jusqu'à écriture réussie
    while(!writeBlock(blockAddr,"Jean Dupont")){
        PrintMessageMultiLine("Repassez la carte");
        delay(1000);
        if(!mfrc522.PICC_IsNewCardPresent()) continue;
        mfrc522.PICC_ReadCardSerial();
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(2000);

    // ---- Étape 2 : contrôle du badge ----
    PrintMessageMultiLine("Scan your badge");
    while(!mfrc522.PICC_IsNewCardPresent()){}
    mfrc522.PICC_ReadCardSerial();

    // boucle jusqu'à lecture réussie
    while(!readBlock(blockAddr, nameOnCard)){
        PrintMessageMultiLine("Repassez la carte");
        delay(1000);
        if(!mfrc522.PICC_IsNewCardPresent()) continue;
        mfrc522.PICC_ReadCardSerial();
    }

    if(nameOnCard == "Jean Dupont"){
        PrintAccessGood();
        Serial.println("Access Granted");
        delay(2000);
        PrintMessageMultiLine("Welcome\nnom: Jean\nprenom: Dupont");
    } else {
        PrintAccessDenied();
        Serial.println("Access Denied");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(8000);

    // ---- Étape 3 : enregistrer un nouveau nom sur le même badge ----
    PrintMessageMultiLine("Enregistrement\nnom: Alice\nprenom: Martin");
    while(!mfrc522.PICC_IsNewCardPresent()){}
    delay(2000);
     
    PrintMessageMultiLine("Scan your badge");
    mfrc522.PICC_ReadCardSerial();

    while(!writeBlock(blockAddr,"Alice Martin")){
        PrintMessageMultiLine("Repassez la carte");
        delay(1000);
        if(!mfrc522.PICC_IsNewCardPresent()) continue;
        mfrc522.PICC_ReadCardSerial();
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1000);

    // ---- Étape 4 : contrôle avec la personne précédente ----
    PrintMessageMultiLine("Scan your badge");
    while(!mfrc522.PICC_IsNewCardPresent()){}
    mfrc522.PICC_ReadCardSerial();

    // boucle jusqu'à lecture réussie
    while(!readBlock(blockAddr, nameOnCard)){
        PrintMessageMultiLine("Repassez la carte");
        delay(1000);
        if(!mfrc522.PICC_IsNewCardPresent()) continue;
        mfrc522.PICC_ReadCardSerial();
    }

    if(nameOnCard == "Jean Dupont"){
        PrintAccessGood();
        Serial.println("Access Granted");
    } else {
        PrintAccessDenied();
        Serial.println("Access Denied");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}
