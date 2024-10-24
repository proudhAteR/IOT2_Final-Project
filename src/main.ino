#include <Arduino.h>
#include "Services.ino"
// put function declarations here:
int myFunction(int, int);

void setup()
{
  const char *netName = "Cegeplabs";
  const char *password = "Cegepdept";

  int result = myFunction(2, 3);
  Services services(netName, password);
}
void loop()
{
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}