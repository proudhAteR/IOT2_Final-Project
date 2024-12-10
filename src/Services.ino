#ifndef SERVICES_H
#define SERVICES_H

#include "Arduino.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <malloc.h>
#include <time.h>
#include "Course.ino"
#include <ESP32Time.h>

// Constants
const char *NO_CLASS_MESSAGE = "No ongoing class found";
const int NUM_CLASSES = 3;

// Structure definition for the Service
typedef struct
{
    const char *ssid;
    const char *password;
    Course classes[2];
} Services;

// Function declarations
Course *getRoomClasses();
void connectToNetwork(const char *ssid, const char *password);
void connectToBroker(WiFiClient wifiClient, Services *service);
const char *getTopic(Services *service);
bool isOngoingClass(int index, Services *service);
boolean isInClassRange(Course course, int currentHour);
boolean isSameDay(Course course, int weekDay);
int getCurrentWeekDay();
int getCurrentHour();

void initializeService(Services *service, const char *ssid, const char *password)
{
    service->ssid = ssid;
    service->password = password;
    service->classes[0] = {"IOT", 2, 4, 23};

    connectToNetwork(ssid, password);
    configTime((3600 * (-5)), 3600, "pool.ntp.org");
}

void connectToNetwork(const char *ssid, const char *password)
{
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }
}

void connectToBroker(PubSubClient client, Services *service)
{
    const char *esp_id = "C1201_Card_Scanner";
    const char *domain = "35.183.155.93";
    const char *username = "ecosystem-connecte";
    const char *pass = "Omega123*";
    boolean connected;

    while (!client.connected() || !connected)
    {
        const char *topic = getTopic(service);
        client.setServer(domain, 1883);
        if (client.connect(esp_id))
        {
            if (topic != NO_CLASS_MESSAGE)
            {
                client.subscribe(topic);
                connected = true;
                Serial.println("connected to the broker");
                client.publish(topic, esp_id); // TODO: change hello for the data read in the nfc card
            }
            else
            {
                Serial.print(topic);
                connected = false;
                delay(5000);
            }
        }
        else
        {
            Serial.println("not connected to the broker");
            delay(5000);
        }
    }
}

const char *getTopic(Services *service)
{
    for (int i = 0; i < NUM_CLASSES; i++)
    {
        if (isOngoingClass(i, service))
        {
            static char buffer[256];
            sprintf(buffer, "C1201/%s", getClassName(&service->classes[i]));
            return buffer;
        }
    }
    return NO_CLASS_MESSAGE;
}

bool isOngoingClass(int index, Services *service)
{
    return isInClassRange(service->classes[index], getCurrentHour()) &&
           isSameDay(service->classes[index], getCurrentWeekDay());
}

boolean isInClassRange(Course course, int currentHour)
{
    return getStartingHour(&course) <= currentHour && getEndHour(&course) > currentHour;
}

boolean isSameDay(Course course, int weekDay)
{
    return weekDay == getWeekDay(&course);
}
int getCurrentWeekDay()
{
    // {"Sunday = 0", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    return now->tm_wday;
}

int getCurrentHour()
{
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    return now->tm_hour;
}

#endif
