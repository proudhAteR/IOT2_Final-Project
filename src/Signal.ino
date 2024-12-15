#ifndef SIGNAL_H
#define SIGNAL_H

#include <Arduino.h>

const int BLUE_LIGHT = 32;
const int RED_LIGHT = 33;

void signal_setup()
{
    pinMode(BLUE_LIGHT, OUTPUT);
    pinMode(RED_LIGHT, OUTPUT);
}

void resetLeds()
{
    digitalWrite(BLUE_LIGHT, LOW);
    digitalWrite(RED_LIGHT, LOW);
}

void successSignal()
{
    digitalWrite(BLUE_LIGHT, HIGH);
}
void failureSignal()
{
    digitalWrite(RED_LIGHT, HIGH);
}
#endif