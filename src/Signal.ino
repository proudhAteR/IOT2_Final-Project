#ifndef SIGNAL_H
#define SIGNAL_H

#include <Arduino.h>

const int BLUE_LIGHT = 32;
const int RED_LIGHT = 33;
const int BUZZER = 13;

void signal_setup();
void playSound(int freq);
void resetLeds();
void successSignal();
void failureSignal();

void signal_setup()
{
    ledcSetup(0, 2000, 8); 

    pinMode(BLUE_LIGHT, OUTPUT);
    pinMode(RED_LIGHT, OUTPUT);
    pinMode(BUZZER, OUTPUT);
}

void resetLeds()
{
    digitalWrite(BLUE_LIGHT, LOW);
    digitalWrite(RED_LIGHT, LOW);
    digitalWrite(BUZZER, LOW);
}

void successSignal()
{
    digitalWrite(BLUE_LIGHT, HIGH);
    playSound(2500);
}
void failureSignal()
{
    digitalWrite(RED_LIGHT, HIGH);
    playSound(500);
}

void playSound(int freq)
{
    digitalWrite(BUZZER, HIGH);
    tone(BUZZER, freq, 750);
}

#endif