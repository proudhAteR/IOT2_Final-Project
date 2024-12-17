#include <Arduino.h>
#include "Service.ino"
#include "NFC.ino"
#include "Signal.ino"
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
  nfc_init();
  signal_setup();
}
void loop()
{
  if(!client.connected()){
    connectToBroker(client, &myServ);
  }
  
  if (isCardPresent() && canReadUID())
  {
    processCard();
  }
  
}

void processCard()
{
  displayCardType();

  String result = readData();
  if (isMessageValid(result) && isclassGoingOn(&myServ))
  {
    handleSuccess(result);
  }
  else
  {
    handleFailure();
  }
  resetLeds();
  halt();
  delay(1000);
}

void handleSuccess(String result)
{
  successSignal();
  publishData(result.c_str());
  delay(750);
}

void handleFailure()
{
  failureSignal();
  delay(750);
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