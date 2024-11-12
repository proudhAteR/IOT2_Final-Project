#include <Arduino.h>
#include "Services.ino"

Services myServ;
WiFiClient wifiClient;

void setup()
{
  const char *SCHOOL_NET_NAME = "Cegeplabs";
  const char *SCHOOL_PWD = "Cegepdept";

  initializeService(&myServ, SCHOOL_NET_NAME, SCHOOL_PWD);
  connectToBroker(wifiClient, &myServ);
}
void loop()
{
}
