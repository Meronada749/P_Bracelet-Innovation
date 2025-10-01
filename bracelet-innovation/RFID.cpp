#include "RFID.h"

bool writeBlock(byte block, const String &message){
    byte buffer[16] = {0};
    message.getBytes(buffer, 16);

    // Authentification avec clé A
    MFRC522::StatusCode status = (MFRC522::StatusCode)
        mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));

    if (status != MFRC522::STATUS_OK){
        Serial.print(F("Auth erreur (A): "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false; // échec
    }

    // Écriture
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(block, buffer, 16);
    if (status != MFRC522::STATUS_OK){
        Serial.print(F("Ecriture impossible: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false; // échec
    }

    Serial.print(F("Nom et prénom écrits : "));
    Serial.println(message);
    return true; // succès
}

bool readBlock(byte block, String &out){
    byte buffer[18];
    byte size = sizeof(buffer);

    MFRC522::StatusCode status = (MFRC522::StatusCode)
        mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));

    if (status != MFRC522::STATUS_OK){
        Serial.print(F("Auth erreur (A) lors lecture: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }

    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(block, buffer, &size);
    if (status != MFRC522::STATUS_OK){
        Serial.print(F("Lecture impossible: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }

    // Conversion en String
    out = "";
    for(byte i = 0; i < 16; i++){
        if(buffer[i] != 0) out += (char)buffer[i];
    }
    return true;
}
