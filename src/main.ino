#include <Arduino.h>
#include "Service.ino"
#include "nfcService.ino"
Service myServ;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup()
{
  const char *SCHOOL_NET_NAME = "Cegeplabs";
  const char *HOME_NET_NAME = "Otter";
  const char *SCHOOL_PWD = "Cegepdept";
  const char *HOME_PWD = "123456789";

  initializeService(&myServ, HOME_NET_NAME, HOME_PWD);
  connectToBroker(client, &myServ);
  nfc_init();
}
void loop()
{
  if (isCardPresent() && canReadUID())
  {
    // TODO: blue light
    displayCardType();

    String result = readData();
    if (isMessageValid(result))
    {
      publishData(result.c_str());
    }
    else
    {
      // TODO: red light
    }
    halt();
    delay(2000);
  }
}

void publishData(const char *message)
{
  const char *t = getTopic(&myServ);
  
  Serial.println(t);
  Serial.println(message);
  client.publish(t, message);
}

bool isMessageValid(String result)
{
  return !result.equals("");
}