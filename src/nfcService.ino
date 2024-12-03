#ifndef NFC_H
#define NFC_H

#include "Arduino.h"
#include "Services.ino"
#include <MFRC522.h>
#include "SPI.h"
void init();
boolean isCardPresent();
boolean canReadUID();
void displayCardType();

const int SS_PIN = 5;
const int RST_PIN = 2;
const int SCK_PIN = 18;
const int MOSI_PIN = 23;
const int MISO_PIN = 19;

MFRC522 RFID(SS_PIN, RST_PIN);

void init()
{
    Serial.begin(9600);
    SPI.begin(); // init SPI bus

    RFID.PCD_Init();
}

boolean isCardPresent()
{
    return RFID.PICC_IsNewCardPresent();
}
boolean canReadUID()
{
    return RFID.PICC_ReadCardSerial();
}
void displayCardType()
{
    MFRC522::PICC_Type piccType = RFID.PICC_GetType(RFID.uid.sak);

    Serial.print("RFID/NFC Tag Type: ");
    Serial.println(RFID.PICC_GetTypeName(piccType));
}

#endif