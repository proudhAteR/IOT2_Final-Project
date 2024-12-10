#include <Arduino.h>
#include "Services.ino"
#include "nfcService.ino"
Services myServ;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup()
{
  const char *SCHOOL_NET_NAME = "Cegeplabs";
  const char *HOME_NET_NAME = "Otter's club";
  const char *SCHOOL_PWD = "Cegepdept";
  const char *HOME_PWD = "16241561101Cmr";

  initializeService(&myServ, SCHOOL_NET_NAME, SCHOOL_PWD);
  connectToBroker(client, &myServ);
  nfc_init();
}
void loop()
{
  if (isCardPresent() && canReadUID())
  {
    displayCardType();
    Serial.println(readData().c_str());
    client.publish(getTopic(&myServ), readData().c_str());
    halt();
    delay(2000);
  }
}
