#ifndef NFC_H
#define NFC_H

#include "Arduino.h"
#include "Services.ino"
#include <MFRC522.h>
#include <Wire.h>

void init();
boolean isCardPresent();
void displayCardType();

const int SDA_PIN = 21;
const int RST_PIN = 19;


MFRC522 RFID(SDA_PIN, RST_PIN);

void init()
{
    Wire.begin();
    Serial.begin(9600);
    RFID.PCD_Init();
}

boolean isCardPresent()
{
    return RFID.PICC_IsNewCardPresent();
}

void displayCardType()
{
    MFRC522::PICC_Type piccType = RFID.PICC_GetType(RFID.uid.sak);

    Serial.print("RFID/NFC Tag Type: ");
    Serial.println(RFID.PICC_GetTypeName(piccType));
}

#endif