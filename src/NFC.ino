#ifndef NFC_H
#define NFC_H

#include "Arduino.h"
#include "Service.ino"
#include "Signal.ino"
#include <MFRC522.h>
#include "SPI.h"

void nfc_init();
boolean isCardPresent();
boolean canReadUID();
boolean operationFailed();
void displayCardType();
String readData();
int readBlock(int blockNumber, byte arrayAddress[]);
String parseNDEFMessage(byte *data, int length);
void halt();
bool operationFailed();

const int SS_PIN = 5;
const int RST_PIN = 2;
const int SCK_PIN = 18;
const int MOSI_PIN = 23;
const int MISO_PIN = 19;

MFRC522 RFID(SS_PIN, RST_PIN);
MFRC522::StatusCode status;
MFRC522::MIFARE_Key key;



byte blockAddr = 4;
byte readbackblock[18];

void nfc_init()
{
    SPI.begin();
    RFID.PCD_Init();
    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF; // Default key for MIFARE Classic
    }
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

String readData()
{
    int numberOfBlocks = 2;

    byte combinedBuffer[numberOfBlocks * 16];
    for (int i = 0; i < numberOfBlocks; i++)
    {
        if (readBlock(blockAddr + i, combinedBuffer + (i * 16)) != 0)
        {
            Serial.print("Failed to read block ");
            Serial.println(blockAddr + i);
            return "";
        }
    }

    String message = parseNDEFMessage(combinedBuffer, sizeof(combinedBuffer));

    if (message.length() > 7)
    {
        return "";
    }
    

    return message;
}

String parseNDEFMessage(byte *data, int length)
{
    // Check if data starts with NDEF TLV type
    if (data[2] != 0x03)
    {
        return "Invalid NDEF message!";
    }

    int ndefLength = data[3]; // Length of NDEF message
    if (ndefLength + 4 > length)
    {
        return "NDEF length mismatch!";
    }

    // Extract NDEF record
    byte recordHeader = data[4];
    if ((recordHeader & 0xD0) != 0xD0)
    { // Ensure it's an NDEF record
        return "Unsupported NDEF record!";
    }

    byte typeLength = data[5];
    byte payloadLength = data[6];
    byte langLength = data[7 + typeLength]; // Language code length

    if (payloadLength <= langLength)
    {
        return "Invalid payload length!";
    }

    // Extract Payload
    int actualPayloadLength = payloadLength - langLength;
    char payload[actualPayloadLength]; // +1 for null-terminator
    memcpy(payload, &data[8 + typeLength + langLength], actualPayloadLength);
    payload[actualPayloadLength] = '\0'; // Add null-terminator
    String res;
    // Debugging: Print raw payload data
    for (int i = 0; i < actualPayloadLength - 1; i++)
    {
        res += payload[i];
    }

    return String(res); 
}



int readBlock(int blockNumber, byte arrayAddress[])
{
    int largestModulo4Number = blockNumber / 4 * 4;
    int trailerBlock = largestModulo4Number + 3; // determine trailer block for the sector

    // authentication of the desired block for access
    status = RFID.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(RFID.uid));

    if (status != MFRC522::STATUS_OK)
    {
        Serial.print("Authentication failed: ");
        Serial.println(RFID.GetStatusCodeName(status));
        return 3; // Error
    }

    byte buffersize = 18;
    status = RFID.MIFARE_Read(blockNumber, arrayAddress, &buffersize);
    if (status != MFRC522::STATUS_OK)
    {
        return 4;
    }
    return 0;
}

void halt()
{
    RFID.PICC_HaltA();
    RFID.PCD_StopCrypto1();
}
bool operationFailed()
{
    return status != MFRC522::STATUS_OK;
}

#endif