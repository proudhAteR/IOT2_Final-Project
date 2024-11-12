#include <Arduino.h>
#include "Services.ino"
// put function declarations here:
int myFunction(int, int);

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
void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}